// c
// \c
// ^
// $
// .
// [s]
// [^s]
// *
// \{m,n\}

// https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html
// A Regular Expression Matcher
// Code by Rob Pike 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "array.h"
#include "regexp.h"

#define SPECIAL_CHARS "\\^$.[*"  //バックスラッシュでエスケープできる文字

//パターンタイプ
typedef enum {
    PAT_CHAR,   // c
    PAT_CHARSET,// [s] [^s]
    PAT_DOT,    // .
    PAT_REPEAT, // *, \{m,n\}
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
        char_set_t  chars;  //type=PAT_CHARSET
        struct {
            int min, max;   //type=PAT_REPEAT
        };
    };
} pattern_t;

//正規表現のコンパイル結果
typedef struct regcomp {
    array_t *array;
    char match_begin;
    char match_end;
    int err_code;
    const char *err_msg;
} reg_compile_t;

int reg_err_code = 0;           //エラーコード
const char *reg_err_msg = "";   //エラーメッセージ

static pattern_t *new_pattern(pattern_type_t type);
static pattern_t *new_repeat(const char*regexp, int *len);
static pattern_t *new_char_set(const char*regexp, int *len);
static int reg_check_prev(reg_compile_t *preg_compile);
static int reg_match_here(pattern_t **pat, const char *text);
static int reg_match_pat(pattern_t *pat, const char *text);
static int reg_match_repeat(pattern_t *c, pattern_t *rep, pattern_t **pat, const char *text);

// reg_compile: compile regexp
reg_compile_t* reg_compile(const char *regexp) {
    reg_compile_t *preg_compile = calloc(1, sizeof(reg_compile_t));
    assert(preg_compile);
    preg_compile->array = new_array(0);
    const char *p = regexp;
    reg_err_code = 0;
    reg_err_msg = "";

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
            if (reg_check_prev(preg_compile)==0) {
                reg_err_code = 13;
                reg_err_msg = "Invalid preceding regular expression";
            } else {
                pat = new_pattern(PAT_REPEAT);
                pat->min = 0;
                pat->max = -1;
            }
        } else if (p[0]=='\\' && p[1]=='{') {
            if (num_array(preg_compile->array)==0 ||
                reg_check_prev(preg_compile)==0) {
                reg_err_code = 13;
                reg_err_msg = "Invalid preceding regular expression";
            } else {
                int len = 0;
                pat = new_repeat(p, &len);
                p += len-1;
            }
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

// new_char_set: 繰り返し条件を生成する
static pattern_t *new_repeat(const char*regexp, int *len) {
    pattern_t *pat = new_pattern(PAT_REPEAT);
    assert(regexp[0] == '\\');
    assert(regexp[1] == '{');
    int mode = 0;
    int min=0, max=-1;
    const char *org = regexp;

    regexp += 2;
    for (; *regexp != '\0' && mode<2; regexp++) {
        switch (mode) {
        case 0:
            if (*regexp==',') {
                mode = 1;
                break;
            } else if (regexp[0]=='\\' && regexp[1]=='}') {
                mode = 2;
                max = min;
                regexp++;
            } else if (isdigit(*regexp)) {
                min = min*10 + *regexp - '0';
            } else {
                mode = 10;  //Error
            }
            break;
        case 1:
            if (regexp[0]=='\\' && regexp[1]=='}') {
                mode = 2;
                regexp++;
            } else if (isdigit(*regexp)) {
                if (max<0) max = 0;
                max = max*10 + *regexp - '0';
            } else {
                mode = 10;  //Error
            }
            break;
        }
    }
    if (mode<2) {
        free(pat);
        reg_err_code = 9;
        reg_err_msg = "IUnmatched \\{";
        return NULL;
    }
    if (mode==10 || (max>0 && min>max)) {
        free(pat);
        reg_err_code = 10;
        reg_err_msg = "Invalid content of \\{\\}";
        return NULL;
    }
    pat->min = min;
    pat->max = max;
    *len = regexp-org;
    return pat;
}

// new_char_set: 文字セットを生成する
static pattern_t *new_char_set(const char*regexp, int *len) {
    pattern_t *pat = new_pattern(PAT_CHARSET);
    assert(*regexp == '[');
    int chars_len = 256;
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
                reg_err_code = 11;
                reg_err_msg = "Invalid range end";
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
    case PAT_CHARSET:
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
    if (pat[0]->type==PAT_NULL)
        return 1;
    if (pat[1]->type==PAT_REPEAT)
        return reg_match_repeat(pat[0], pat[1], pat+2, text);

    switch (pat[0]->type) {
    case PAT_CHAR:
    case PAT_CHARSET:
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

// reg_match_pat: 1個のパターンがマッチするかを判定
static int reg_match_pat(pattern_t *pat, const char *text) {
    switch (pat->type) {
    case PAT_DOT:
        return *text != '\0';
    case PAT_CHAR:
        return pat->c == *text;
    case PAT_CHARSET:
        if (*text == '\0') return 0;
        if (strchr(pat->chars.chars, *text))
            return !pat->chars.reverse;
        return pat->chars.reverse;
    default:
        return 0;
    }
    return 0;
}

// reg_match_repeat: search for c\{m,n\} regexp at beginning of text
static int reg_match_repeat(pattern_t *c, pattern_t *rep, pattern_t **pat, const char *text) {
    assert(rep->type==PAT_REPEAT);
    int max = rep->max<0?999:rep->max;
    int cnt = 0;
    do {    /* a \{0,n\} matches zero or more instances */
        if (cnt++>=rep->min && reg_match_here(pat, text)) return 1;   //最短一致
    } while (reg_match_pat(c, text++) && cnt<=max);
    return 0;
}

void reg_compile_free(reg_compile_t* preg_compile) {
    if (preg_compile) {
        free_array(preg_compile->array);
        free(preg_compile);
    }
}
