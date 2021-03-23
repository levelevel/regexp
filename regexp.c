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
    PAT_SUBREG, // \(r\)
    PAT_END,    // $
    PAT_NULL,   //パターンの終わり
} pattern_type_t;

//[s], [^s]を格納する文字セット
typedef struct {
    int reverse;
    char *chars;
} char_set_t;

typedef struct regcomp regcomp_t;

//パターン
typedef struct {
    pattern_type_t  type;
    union {
        char        c;      //type=PAT_CHAR
        char_set_t  chars;  //type=PAT_CHARSET
        struct {
            int min, max;   //type=PAT_REPEAT
        };
        regcomp_t *regcomp; //type=PAT_SUBREG
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
static pattern_t *new_subreg(const char*regexp, int *len);
static int reg_check_prev(reg_compile_t *preg_compile);
static int reg_match_here(pattern_t **pat, const char *text, const char **rm_ep);
static void set_match(regmatch_t *pmatch, const char *text_org, const char *rm_sp, const char *rm_ep);
static int reg_match_pat(pattern_t *pat, const char *text, int *len);
static int reg_match_repeat(pattern_t *c, pattern_t *rep, pattern_t **pat, const char *text, const char **rm_ep);
static void reg_pattern_free(pattern_t *pat);

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
        } else if (p[0]=='\\' && p[1]=='(') {
            int len = 0;
            pat = new_subreg(p, &len);
            p += len-1;
        } else if (p[0]=='\\' /*&& strchr(SPECIAL_CHARS, p[1])*/) {
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

// new_subreg: \(\)の中身を抽出する
static pattern_t *new_subreg(const char*regexp, int *len) {
    pattern_t *pat = new_pattern(PAT_SUBREG);
    assert(regexp[0] == '\\');
    assert(regexp[1] == '(');
    const char *org = regexp;
    
    regexp += 2;
    char *p = strstr(regexp, "\\)");
    if (p==NULL) {
        reg_pattern_free(pat);
        reg_err_code = 8;
        reg_err_msg = "Unmatched \\(";
        return NULL;
    }

    int sublen = p-regexp;
    char *sub_regexp = malloc(sublen+1);
    strncpy(sub_regexp, regexp, sublen);
    sub_regexp[sublen] = '\0';
    pat->regcomp = reg_compile(sub_regexp);
    free(sub_regexp);
    if (pat->regcomp==NULL) {
        reg_pattern_free(pat);
        return NULL;
    }
    pat->regcomp->match_begin = 1;

    *len = (p+2)-org;
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
        reg_pattern_free(pat);
        reg_err_code = 9;
        reg_err_msg = "Unmatched \\{";
        return NULL;
    }
    if (mode==10 || (max>0 && min>max)) {
        reg_pattern_free(pat);
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
        if (regexp[1]==']') {
            reg_pattern_free(pat);
            reg_err_code = 7;
            reg_err_msg = "Unmatched [, [^, [:, [., or [=";
            return NULL;
        }
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
                reg_pattern_free(pat);
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
    case PAT_SUBREG:
        return 1;   //繰り返し可能
    default:
        return 0;
    }
    return 0;
}

// reg_exec: search for compiled regexp anywhere in text
int reg_exec(reg_compile_t *preg_compile, const char *text, size_t nmatch, regmatch_t *pmatch) {
    (void)nmatch;
    assert(preg_compile);
    assert(preg_compile->array);
    const char *text_org = text;
    const char *rm_ep;
    pattern_t **pat = (pattern_t**)preg_compile->array->buckets;
    if (pmatch) pmatch->rm_so = pmatch->rm_eo = 0;//-1;
    if (preg_compile->match_begin) {
        if (reg_match_here(pat, text, &rm_ep)) {
            set_match(pmatch, text_org, text_org, rm_ep);
            return 0;
        }
        return 1;
    } else {
        do {    /* must look even if string is empty */
            if (reg_match_here(pat, text, &rm_ep)) {
                set_match(pmatch, text_org, text, rm_ep);
                return 0;
            }
        } while (*text++!='\0');
    }
    return 1;
}

//マッチング位置のインデックスを記録する
static void set_match(regmatch_t *pmatch, const char *text_org, const char *rm_sp, const char *rm_ep) {
    if (pmatch) {
        pmatch->rm_so = rm_sp-text_org;
        pmatch->rm_eo = rm_ep-text_org;
    }
}

// reg_match_here: search for regexp at beginning of text
static int reg_match_here(pattern_t **pat, const char *text, const char **rm_ep) {
    if (pat[0]->type==PAT_NULL) {
        *rm_ep = text;
        return 1;
    }
    if (pat[1]->type==PAT_REPEAT)
        return reg_match_repeat(pat[0], pat[1], pat+2, text, rm_ep);
    int len;
    if (reg_match_pat(pat[0], text, &len))
        return reg_match_here(pat+1, text+len, rm_ep);
    return 0;
}

// reg_match_pat: 1個のパターンがマッチするかを判定
static int reg_match_pat(pattern_t *pat, const char *text, int *len) {
    *len = 1;   //patにマッチしたtextの長さ
    switch (pat->type) {
    case PAT_CHAR:
        return pat->c == *text;
    case PAT_CHARSET:
        if (*text == '\0') return 0;
        if (strchr(pat->chars.chars, *text))
            return !pat->chars.reverse;
        return pat->chars.reverse;
    case PAT_DOT:
        return *text != '\0';
    case PAT_SUBREG:
    {
        regmatch_t pmatch;
        if (reg_exec(pat->regcomp, text, 1, &pmatch)==0) {
            *len = pmatch.rm_eo - pmatch.rm_so;
            return 1;
        }
        return 0;
    }
    case PAT_END:
        if (*text=='\0') {
            *len = 0;
            return 1;
        }
        return 0;
    default:
        return 0;
    }
    return 0;
}

// reg_match_repeat: search for c\{m,n\} regexp at beginning of text
static int reg_match_repeat(pattern_t *c, pattern_t *rep, pattern_t **pat, const char *text, const char **rm_ep) {
    assert(rep->type==PAT_REPEAT);
    int max = rep->max<0?999:rep->max;
    int cnt = 0;
    int ret = 0;
    int len;
    do {    /* a \{0,n\} matches zero or more instances */
        if (cnt++>=rep->min && reg_match_here(pat, text, rm_ep)) ret = 1;   //最短一致ならここでreturn 1する
    } while (reg_match_pat(c, text, &len) && (text=text+len) && cnt<=max);
    return ret;
}

void reg_compile_free(reg_compile_t* preg_compile) {
    if (preg_compile==NULL) return;
    array_t *array = preg_compile->array;
    if (array) {
        for (int i=0; i<array->num; i++) {
            reg_pattern_free(array->buckets[i]);
        }
        free_array(preg_compile->array);
    } 
    free(preg_compile);
}

static void reg_pattern_free(pattern_t *pat) {
    if (pat==NULL) return;
    switch (pat->type) {
    case PAT_CHARSET:
        free(pat->chars.chars);
        break;
    case PAT_SUBREG:
        reg_compile_free(pat->regcomp);
        break;
    default:
        break;
    }
    free(pat);
}