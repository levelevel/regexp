// c
// \c
// ^
// $
// .
// [s]
// [^s]
// *

// https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html
// A Regular Expression Matcher
// Code by Rob Pike 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "array.h"
#include "regexp.h"

#define SPECIAL_CHARS "\\^$.[*"  //バックスラッシュでエスケープできる文字

//パターンタイプ
typedef enum {
    PAT_CHAR,   // c
    PAT_CHARS,  // [s] [^s]
    PAT_DOT,    // .
    PAT_STAR,   // *
    PAT_END,    // $
    PAT_NULL,   //パターンの終わり
} pattern_type_t;

//[s], [^s]を格納する文字セット
typedef struct {
    int reverse;
    char *chars;
} char_set_t;

//パターン
typedef struct {
    pattern_type_t  type;
    union {
        char        c;      //type=PAT_CHAR
        char_set_t  chars;  //type=PAT_CHARS
    };
} pattern_t;

//正規表現のコンパイル結果
typedef struct regcomp {
    array_t *array;
    char match_begin;
    char match_end;
} reg_compile_t;

static pattern_t *new_pattern(pattern_type_t type);
static pattern_t *new_char_set(const char*regexp, int *len);
static int reg_check_prev(reg_compile_t *preg_compile);
static int reg_match_here(pattern_t **pat, const char *text);
static int reg_match_pat(pattern_t *pat, const char *text);
static int reg_match_star(pattern_t *pre, pattern_t **pat, const char *text);

static int matchhere(const char *regexp, const char *text);
static char_set_t *get_char_class(const char *regexp, int *len);
static int matchchar(const char *regexp, const char *text, int *len);
static int matchstar(int c, const char *regexp, const char *text);

// reg_compile: compile regexp
reg_compile_t* reg_compile(const char *regexp) {
    reg_compile_t *preg_compile = calloc(1, sizeof(reg_compile_t));
    assert(preg_compile);
    preg_compile->array = new_array(0);
    const char *p = regexp;
    if (*p=='^') {
        preg_compile->match_begin = 1;
        p++;
    }
    for (; *p; p++) {
        pattern_t *pat = NULL;
        if (p[0]=='$' && p[1]=='\0') {
            preg_compile->match_end = 1;
            pat = new_pattern(PAT_END);
        } else if (p[0]=='.') {
            pat = new_pattern(PAT_DOT);
        } else if (p[0]=='*' && num_array(preg_compile->array)) {
            if (reg_check_prev(preg_compile)==0) return NULL;
            pat = new_pattern(PAT_STAR);
        } else if (p[0]=='\\' && strchr(SPECIAL_CHARS, p[1])) {
            pat = new_pattern(PAT_CHAR);
            pat->c = p[1];
            p++;            
        } else if (*p=='[') {
            int len = 0;
            pat = new_char_set(p, &len);
            p += len-1;
        } else {
            pat = new_pattern(PAT_CHAR);
            pat->c = p[0];
        }
        if (pat) {
            push_array(preg_compile->array, pat);
        } else {
            reg_compile_free(preg_compile);
            return NULL;
        }
    }
    push_array(preg_compile->array, new_pattern(PAT_NULL));
    return preg_compile;
}

// new_pattern: create new pattern for the type
static pattern_t *new_pattern(pattern_type_t type) {
    pattern_t *pat = calloc(1, sizeof(pattern_t));
    assert(pat);
    pat->type = type;
    return pat;
}

static pattern_t *new_char_set(const char*regexp, int *len) {
    pattern_t *pat = new_pattern(PAT_CHARS);
    assert(*regexp == '[');
    int chars_len = 128;
    int pos = 0;
    char_set_t *char_class = &pat->chars;
    char_class->chars = calloc(chars_len, sizeof(char));
    const char *org = regexp;

    regexp++;
    if (*regexp == '^') {
        char_class->reverse = 1;
        regexp++;
    }
    if (*regexp == ']' || *regexp == '-') {
        char_class->chars[pos++] = *regexp;
        regexp++;
    }
    for (; *regexp != '\0'; regexp++) {
        if (*regexp == '-' && regexp[1] != '\0' && regexp[1] != ']') {
            if (regexp[-1]>regexp[1]) {
                free(char_class->chars);
                free(pat);
                return NULL;
            }
            for (int c=regexp[-1]+1; c<regexp[1]; c++) {
                char_class->chars[pos++] = c;
            }
            char_class->chars[pos++] = regexp[1];
            regexp++;
        } else if (*regexp != ']') {
            char_class->chars[pos++] = *regexp;
        } else {
            regexp++;
            break;
        }
    }
    *len = regexp-org;
    //printf("regexp=%s, len=%d\n", org, *len);
    //printf("[%s]\n", char_class->chars);
    return pat;
}

//　reg_check_prev :直前のパターンが繰り返し対象かどうか
static int reg_check_prev(reg_compile_t *preg_compile) {
    pattern_t *pat = preg_compile->array->buckets[num_array(preg_compile->array)-1];
    switch (pat->type) {
    case PAT_CHAR:
    case PAT_CHARS:
    case PAT_DOT:
        return 1;   //繰り返し可能
    default:
        return 0;
    }
    return 0;
}

// reg_exec: search for compiled regexp anywhere in text
int reg_exec(reg_compile_t *preg_compile, const char *text) {
    assert(preg_compile);
    assert(preg_compile->array);
    pattern_t **pat = (pattern_t**)preg_compile->array->buckets;
    if (preg_compile->match_begin) {
        return reg_match_here(pat, text);
    } else {
        do {    /* must look even if string is empty */
            if (reg_match_here(pat, text)) return 1;
        } while (*text++!='\0');
    }
    return 0;
}

// reg_match_here: search for regexp at beginning of text
static int reg_match_here(pattern_t **pat, const char *text) {
    if (pat[0]->type==PAT_NULL) return 1;
    if (pat[1]->type==PAT_STAR) return reg_match_star(pat[0], pat+2, text);

    switch (pat[0]->type) {
    case PAT_CHAR:
    case PAT_CHARS:
    case PAT_DOT:
        if (reg_match_pat(pat[0], text))
            return reg_match_here(pat+1, text+1);
        else return 0;
    case PAT_END:
        return *text=='\0';
    default:
        return 0;
    }
    return 0;
}

// reg_match_pat: 1このパターンがマッチするかを判定
static int reg_match_pat(pattern_t *pat, const char *text) {
    switch (pat->type) {
    case PAT_DOT:
        return *text != '\0';
    case PAT_CHAR:
        return pat->c == *text;
    case PAT_CHARS:
        if (*text == '\0') return 0;
        if (strchr(pat->chars.chars, *text))
            return !pat->chars.reverse;
        return pat->chars.reverse;
    default:
        return 0;
    }
    return 0;
}

// reg_match_star: search for c*regexp at beginning of text
static int reg_match_star(pattern_t *c, pattern_t **pat, const char *text) {
    do {    /* a * matches zero or more instances */
        if (reg_match_here(pat, text)) return 1;
    } while (reg_match_pat(c, text++));
    return 0;
}

void reg_compile_free(reg_compile_t* preg_compile) {
    if (preg_compile) {
        free_array(preg_compile->array);
        free(preg_compile);
    }
}

/* match: search for regexp anywhere in text */
int match(const char *regexp, const char *text)
{
    if (regexp[0] == '^')
        return matchhere(regexp+1, text);
    do {    /* must look even if string is empty */
        if (matchhere(regexp, text))
            return 1;
    } while (*text++ != '\0');
    return 0;
}

/* matchhere: search for regexp at beginning of text */
static int matchhere(const char *regexp, const char *text)
{
    int len;
    if (regexp[0] == '\0')
        return 1;
    if (regexp[0] == '\\' && strchr(SPECIAL_CHARS, regexp[1])) {
        if (regexp[1] == text[0]) return matchhere(regexp+2, text+1);
        else return 0;
    }
    if (regexp[1] == '*')
        return matchstar(regexp[0], regexp+2, text);
    if (regexp[0] == '$' && regexp[1] == '\0')
        return *text == '\0';
    if (*text!='\0' && matchchar(regexp, text, &len))
        return matchhere(regexp+len, text+1);
    return 0;
}

static char_set_t *get_char_class(const char *regexp, int *len) {
    assert(*regexp == '[');
    int chars_len = 128;
    int pos = 0;
    char_set_t *char_class = calloc(1, sizeof(char_set_t));
    char_class->chars = calloc(chars_len, sizeof(char));
    const char *org = regexp;

    regexp++;
    if (*regexp == '^') {
        char_class->reverse = 1;
        regexp++;
    }
    if (*regexp == ']' || *regexp == '-') {
        char_class->chars[pos++] = *regexp;
        regexp++;
    }
    for (; *regexp != '\0'; regexp++) {
        if (*regexp == '-' && regexp[1] != '\0' && regexp[1] != ']') {
            //rangeの逆転はチェックしていない。[z-a]は[za]と同じ。
            for (int c=regexp[-1]+1; c<regexp[1]; c++) {
                char_class->chars[pos++] = c;
            }
            char_class->chars[pos++] = regexp[1];
            regexp++;
        } else if (*regexp != ']') {
            char_class->chars[pos++] = *regexp;
        } else {
            regexp++;
            break;
        }
    }
    *len = regexp-org;
    //printf("regexp=%s, len=%d\n", org, *len);
    //printf("[%s]\n", char_class->chars);
    return char_class;
}

static int matchchar(const char *regexp, const char *text, int *len) {
    *len = 1;
    if (*regexp == '.')
        return 1;
    if (*regexp == '[') {
        char_set_t *char_class = get_char_class(regexp, len);
        if (strchr(char_class->chars, *text))
            return !char_class->reverse;
        return char_class->reverse;
    }
    if (*regexp == *text)
        return 1;
    return 0;
}

/* matchstar: search for c*regexp at beginning of text */
static int matchstar(int c, const char *regexp, const char *text)
{
    do {    /* a * matches zero or more instances */
        if (matchhere(regexp, text))
            return 1;
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}
