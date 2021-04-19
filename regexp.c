// reg_exp      = sequence_exp ( "|" sequence_exp )*                                ERE
//              | "^"? sequence_exp* "$"?                                       BRE
// sequence_exp = repeat_exp*                                                   BRE/ERE
// repeat_exp   = primary_exp ( "*" | "\+" | "\?" | "\{" repeat_range "\}" )?   BRE
//              | repeat_exp ( "*" | "+" | "?" | "{" repeat_range "}" )?            ERE
// repeat_range = num                                                           BRE/ERE
//              | num? "," num
//              | num "," num?
// primary_exp  = char                                                      BRE/ERE
//              = "."                                                       BRE/ERE
//              | "\" special_char                                          BRE/ERE
//              | "[" char_set "]"                                          BRE/ERE
//              | "[^" char_set "]"                                         BRE/ERE
//              | "\(" reg_exp "\)"                                         BRE
//              | "(" reg_exp ")"                                               ERE
//              | "\" ( "1" | ... | "9" )                                   BRE/ERE
//              = "^"                                                           ERE
//              = "$"                                                           ERE
// char_set     = char char_set*                                            BRE/ERE
//              | char "-" char
//              | "[:" char_class ":]"
// char_class   = "upper"       #[A-Z]                                      POSIX
//              | "lower"       #[a-z]                                      POSIX
//              | "alpha"       #[A-Za-z]                                   POSIX
//              | "alnum"       #[A-Za-z0-9]                                POSIX
//              | "word"        #[A-Za-z0-9_]
//              | "digit"       #[0-9]                                      POSIX
//              | "xdigit"      #[0-9A-Fa-f]                                POSIX
//              | "punct"       #[]!"#$%&'()*+,-./:;<=>?@[\^_`{|}~-]        POSIX
//              | "blank"       #[ \t]                                      POSIX
//              | "space"       #[ \t\n\r\f\v]                              POSIX
//              | "cntrl"       #0x00-0x1f, 0x7f                            POSIX
//              | "graph"       #[^ \t\n\r\f\v[:cntrl:]]                    POSIX
//              | "print"       #[^\t\n\r\f\v[:cntrl:]]                     POSIX

// https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html
// A Regular Expression Matcher
// Code by Rob Pike

//regex.h -------------------------------------------------------------

/* If this bit is set, then ^ and $ are always anchors (outside bracket
     expressions, of course).
   If this bit is not set, then it depends:
	^  is an anchor if it is at the beginning of a regular
	   expression or after an open-group or an alternation operator;
	$  is an anchor if it is at the end of a regular expression, or
	   before a close-group or an alternation operator.

   This bit could be (re)combined with RE_CONTEXT_INDEP_OPS, because
   POSIX draft 11.2 says that * etc. in leading positions is undefined.
   We already implemented a previous draft which made those constructs
   invalid, though, so we haven't changed the code back.  */
//# define RE_CONTEXT_INDEP_ANCHORS (RE_CHAR_CLASSES << 1)

/* If this bit is set, then special characters are always special
     regardless of where they are in the pattern.
   If this bit is not set, then special characters are special only in
     some contexts; otherwise they are ordinary.  Specifically,
     * + ? and intervals are only special when not after the beginning,
     open-group, or alternation operator.  */
//# define RE_CONTEXT_INDEP_OPS (RE_CONTEXT_INDEP_ANCHORS << 1)

/* If this bit is set, then *, +, ?, and { cannot be first in an re or
     immediately after an alternation or begin-group operator.  */
//# define RE_CONTEXT_INVALID_OPS (RE_CONTEXT_INDEP_OPS << 1)

/* If this bit is not set, then + and ? are operators, and \+ and \? are
     literals.
   If set, then \+ and \? are operators and + and ? are literals.  */
//# define RE_BK_PLUS_QM (RE_BACKSLASH_ESCAPE_IN_LISTS << 1)

/* If this bit is set, then an unmatched ) is ordinary.
   If not set, then an unmatched ) is invalid.  */
//# define RE_UNMATCHED_RIGHT_PAREN_ORD (RE_NO_EMPTY_RANGES << 1)

/* If this bit is set, (...) defines a group, and \( and \) are literals.
   If not set, \(...\) defines a group, and ( and ) are literals.  */
//# define RE_NO_BK_PARENS (RE_NO_BK_BRACES << 1)

/* If this bit is set, then | is an alternation operator, and \| is literal.
   If not set, then \| is an alternation operator, and | is literal.  */
//# define RE_NO_BK_VBAR (RE_NO_BK_REFS << 1)

/* Syntax bits common to both basic and extended POSIX regex syntax.  */
//# define _RE_SYNTAX_POSIX_COMMON
//  (RE_CHAR_CLASSES | RE_DOT_NEWLINE      | RE_DOT_NOT_NULL
//   | RE_INTERVALS  | RE_NO_EMPTY_RANGES)
//
//# define RE_SYNTAX_POSIX_BASIC
//  (_RE_SYNTAX_POSIX_COMMON | RE_BK_PLUS_QM | RE_CONTEXT_INVALID_DUP)
//
//# define RE_SYNTAX_POSIX_EXTENDED
//  (_RE_SYNTAX_POSIX_COMMON  | RE_CONTEXT_INDEP_ANCHORS
//   | RE_CONTEXT_INDEP_OPS   | RE_NO_BK_BRACES
//   | RE_NO_BK_PARENS        | RE_NO_BK_VBAR
//   | RE_CONTEXT_INVALID_OPS | RE_UNMATCHED_RIGHT_PAREN_ORD)

/* POSIX 'cflags' bits (i.e., information for 'regcomp').  */
/* If this bit is set, then use extended regular expression syntax.
   If not set, then use basic regular expression syntax.  */
//#define REG_EXTENDED 1
/* If this bit is set, then ignore case when matching.
   If not set, then case is significant.  */
//#define REG_ICASE (1 << 1)
/* If this bit is set, then anchors do not match at newline
     characters in the string.
   If not set, then anchors do match at newlines.  */
//#define REG_NEWLINE (1 << 2)

/* POSIX 'eflags' bits (i.e., information for regexec).  */
/* If this bit is set, then the beginning-of-line operator doesn't match
     the beginning of the string (presumably because it's not the
     beginning of a line).
   If not set, then the beginning-of-line operator does match the
     beginning of the string.  */
//#define REG_NOTBOL 1
/* Like REG_NOTBOL, except for the end-of-line.  */
//#define REG_NOTEOL (1 << 1)

//regex.h -------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "array.h"
#define __USE_GNU
#include "regexp.h"

#define SPECIAL_CHARS "\\^$.[*"  //バックスラッシュでエスケープできる文字

//パターンタイプ
typedef enum {
    PAT_CHAR=1, // c
    PAT_CHARSET,// [s] [^s]
    PAT_DOT,    // .
    PAT_REPEAT, // *, \{m,n\}
    PAT_SUBREG, // \(r\)
    PAT_BACKREF,// \1
    PAT_OR,     // |
    PAT_CARET,  // ^
    PAT_DOLLAR, // $
    PAT_NULL,   //パターンの終わり
} pattern_type_t;

static const char *pattern_type_str[] = {
    "0", "PAT_CHAR", "PAT_CHARSET", "PAT_DOT", "PAT_REPEAT",
    "PAT_SUBREG", "PAT_BACKREF", "PAT_OR", "PAT_CARET", "PAT_DOLLAR", "PAT_NULL"
};

//[s], [^s]を格納する文字セット
typedef struct {
    char reverse;       //否定 [^...]
    unsigned int size;  //charsのバッファサイズ
    char *chars;
} char_set_t;

//パターン
typedef struct {
    pattern_type_t  type;
    union {
        char        c;      //type=PAT_CHAR
        char_set_t  cset;   //type=PAT_CHARSET
        struct {
            int min, max;   //type=PAT_REPEAT
        };
        reg_compile_t *regcomp; //type=PAT_SUBREG
        int ref_num;        //type=PAT_BACKREF, 1-9
        array_t *or_array;  //type=PAT_OR, regcomp_t*のarray
    };
} pattern_t;

//正規表現のコンパイル結果
typedef struct regcomp {
    array_t *array;         //pattern_t*のアレイ
    unsigned int match_here :1;     //現在位置からマッチさせる
    short nparen;           //()の数（0-9）
    int cflags;             //正規表現のsyntax。RE_*
    int ref_num;            //SUBREGの場合の後方参照の番号（0-9）
    //コンパイル時の作業用
    const char *regexp;     //元の正規表現
    const char *p;          //regexpを指す作業ポインタ
    pattern_t *prev_pat;    //直前のパターン
    pattern_type_t mode;    //処理中のパターン（PAT_SUBREG:"\)"が終了）
} reg_compile_t;

//関数の戻り値
typedef enum {
    REG_OK,     //要素抽出
    REG_END,    //最後まで処理
    REG_ERR,    //エラー時
} reg_stat_t;

#define MAX_PAREN 9
static int g_nparen;            //()の数(0-9)
static int g_nparen_finished;   //完了した()の数
static int g_syntax;            //構文の制御 RE_*
static int g_cflags;

static void push_char_set(char_set_t *char_set, char c);
static void push_char_set_str(char_set_t *char_set, const char *str);
static pattern_t *new_pattern(pattern_type_t type);
static reg_compile_t *new_reg_compile(const char *regexp);
static reg_compile_t *reg_exp     (const char *regexp, pattern_type_t mode);
static reg_compile_t *sequence_exp(const char *regexp, pattern_type_t mode);
static reg_stat_t repeat_exp  (reg_compile_t *preg_compile);
static reg_stat_t primary_exp (reg_compile_t *preg_compile);
static reg_stat_t new_repeat  (reg_compile_t *preg_compile);
static reg_stat_t new_char_set(reg_compile_t *preg_compile);
static reg_stat_t set_char_class(reg_compile_t *preg_compile, char_set_t *char_set);
static reg_stat_t new_subreg  (reg_compile_t *preg_compile);

static int reg_exec_main(reg_compile_t *preg_compile, const char *text);
static int reg_match_here(pattern_t **pat, const char *text, const char **rm_ep);
static void set_match(int idx, const char *rm_sp, const char *rm_ep);
static int reg_match_pat(pattern_t *pat, const char *text, int *len);
static int reg_match_repeat(pattern_t **pat, const char *text, const char **rm_ep);
static void reg_pattern_free(pattern_t *pat);

static void reg_set_err(reg_err_code_t err_code);

#define token1_is(p, c)     (*(p)==c)
#define token2_is(p, str)   (memcmp(p, str, 2)==0)
#define token3_is(p, str)   (memcmp(p, str, 3)==0)

#define token_is_open_brace(p)  ((g_syntax&RE_NO_BK_BRACES) ? token1_is(p, '{') : token2_is(p, "\\{"))
#define token_is_close_brace(p) ((g_syntax&RE_NO_BK_BRACES) ? token1_is(p, '}') : token2_is(p, "\\}"))
#define token_is_open_paren(p)  ((g_syntax&RE_NO_BK_PARENS) ? token1_is(p, '(') : token2_is(p, "\\("))
#define token_is_close_paren(p) ((g_syntax&RE_NO_BK_PARENS) ? token1_is(p, ')') : token2_is(p, "\\)"))
#define token_is_plus(p)       (!(g_syntax&RE_BK_PLUS_QM)   ? token1_is(p, '+') : token2_is(p, "\\+"))
#define token_is_question(p)   (!(g_syntax&RE_BK_PLUS_QM)   ? token1_is(p, '?') : token2_is(p, "\\?"))
#define token_is_vbar(p)        ((g_syntax&RE_NO_BK_VBAR)   ? token1_is(p, '|') : token2_is(p, "\\|"))

// reg_compile: compile regexp
// reg_exp      = sequence_exp ( "|" sequence_exp )*                            ERE
//              | "^"? sequence_exp* "$"?                                   BRE
// sequence_exp = repeat_exp repeat_exp*                                    BRE/ERE
reg_compile_t* reg_compile(const char *regexp, size_t *re_nsub, int cflags) {
    reg_err_info.err_code = 0;
    reg_err_info.err_msg = "";
    reg_compile_t *preg_compile;
    g_nparen = g_nparen_finished = 0;
    g_syntax = ((cflags & REG_EXTENDED) ? RE_SYNTAX_POSIX_EXTENDED : RE_SYNTAX_POSIX_BASIC);
    g_cflags = cflags;

    preg_compile = reg_exp(regexp, 0);
    if (preg_compile) {
        if (re_nsub) *re_nsub = preg_compile->nparen;
        preg_compile->cflags = cflags;
    }

    return preg_compile;
}

//push_char_set: 文字セットに文字を追加する。
static void push_char_set(char_set_t *char_set, char c) {
    assert(char_set);
    size_t len = strlen(char_set->chars);
    if (len+1>=char_set->size) {
        char_set->size *= 2;
        char_set->chars = realloc(char_set->chars, char_set->size*sizeof(char));
        assert(char_set->chars);
    }
    char_set->chars[len++] = (g_cflags&REG_ICASE)?toupper(c):c;
    char_set->chars[len] = '\0';
}
//push_char_set_str: 文字セットに文字列を追加する。
static void push_char_set_str(char_set_t *char_set, const char *str) {
    assert(char_set);
    size_t len1 = strlen(char_set->chars);
    size_t len2 = strlen(str);
    if (len1+len2>=char_set->size) {
        while (len1+len2>=char_set->size) char_set->size *= 2;
        char_set->chars = realloc(char_set->chars, char_set->size*sizeof(char));
        assert(char_set->chars);
    }
    strcat(char_set->chars+len1, str);
}

// new_pattern: allocate new pattern for the type
static pattern_t *new_pattern(pattern_type_t type) {
    pattern_t *pat = calloc(1, sizeof(pattern_t));
    assert(pat);
    pat->type = type;
    return pat;
}

// new_reg_compile: allocate new reg_compile
static reg_compile_t *new_reg_compile(const char *regexp) {
    reg_compile_t *preg_compile = calloc(1, sizeof(reg_compile_t));
    assert(preg_compile);
    preg_compile->array = new_array(0);
    preg_compile->regexp = preg_compile->p = regexp;
    return preg_compile;
}

// push_pattern
// patがPAT_REPEATの場合、直前の要素の前に挿入する。
static void push_pattern(reg_compile_t *preg_compile, pattern_t *pat) {
    if (pat->type==PAT_REPEAT) {
        int num = num_array(preg_compile->array);
        assert(num>0);
#ifdef MULTI_REPEAT_SUBREG
        if (num<=1 || ((pattern_t*)peek_array(preg_compile->array, num-2))->type!=PAT_REPEAT) {
            push_array(preg_compile->array, peek_array(preg_compile->array, num-1));
            put_array (preg_compile->array, num-1, pat);
        } else {
            pattern_t *pat_subreg = new_pattern(PAT_SUBREG);
            reg_compile_t *subreg;
            pat_subreg->regcomp = subreg = new_reg_compile(NULL);
            subreg->match_here = 1;
            push_array(subreg->array, peek_array(preg_compile->array, num-2));
            push_array(subreg->array, peek_array(preg_compile->array, num-1));
            push_array(subreg->array, new_pattern(PAT_NULL));
            put_array (preg_compile->array, num-1, pat_subreg);
            put_array (preg_compile->array, num-2, pat);
        }
        //reg_dump(stdout, preg_compile, 0);
#else
        int idx = num-2;
        while (idx>=0 && ((pattern_t*)peek_array(preg_compile->array, idx))->type==PAT_REPEAT) idx--;
        idx ++;
        for (int i=num-1; i>=idx; i--) {
            put_array(preg_compile->array, i+1, peek_array(preg_compile->array, i));
        }
        put_array(preg_compile->array, idx, pat);
#endif
    } else {
        push_array(preg_compile->array, pat);
    }
    preg_compile->prev_pat = pat;
}

// reg_exp      = sequence_exp ( "|" sequence_exp )*                            ERE
//              | "^"? sequence_exp* "$"?                                   BRE
// sequence_exp = repeat_exp repeat_exp*                                    BRE/ERE
//戻り値：エラー時はNULLを返し、reg_err_code/reg_err_msgにエラー情報を設定する。
static reg_compile_t *reg_exp(const char *regexp, pattern_type_t mode) {
    reg_compile_t *preg_compile = sequence_exp(regexp, mode);
    if (preg_compile==NULL) return NULL;

    pattern_t *pat = NULL;
    reg_compile_t *subreg = preg_compile;
    while (token_is_vbar(subreg->p)) {
        //"a|b|c"のパターンは以下の構成とする
        //reg_compile->array
        //    [0]pattern(PAT_OR)->or_array
        //       +[0]reg_compile "a"
        //       +[1]reg_compile "b"
        //       +[2]reg_compile "c"
        if (pat==NULL) {
            pat = new_pattern(PAT_OR);
            pat->or_array = new_array(0);
            subreg->match_here = 1;
            subreg->ref_num = preg_compile->ref_num;
            push_array(pat->or_array, subreg);
            preg_compile = new_reg_compile(regexp);
            preg_compile->mode = mode;
            preg_compile->regexp = regexp;
            push_pattern(preg_compile, pat);
            push_pattern(preg_compile, new_pattern(PAT_NULL));
        }
        subreg->p++;
        subreg = sequence_exp(subreg->p, mode);
        if (subreg==NULL) {
            reg_compile_free(preg_compile);
            return NULL;
        }
        subreg->match_here = 1;
        subreg->ref_num = preg_compile->ref_num;
        push_array(pat->or_array, subreg);
        preg_compile->p = subreg->p;
    }

    if (mode==PAT_SUBREG && token_is_close_paren(preg_compile->p)) {
        if (*preg_compile->p=='\\') preg_compile->p++;
        preg_compile->p++;
    } else {
        assert(token1_is(preg_compile->p, '\0'));
    }
    //reg_dump(stdout, preg_compile, 0);

    return preg_compile;
}

// sequence_exp = repeat_exp*                                               BRE/ERE
// repeat_exp   = primary_exp ( "*" | "\{" repeat_range "\}" )?             BRE
//              | repeat_exp ( "*" | "+" | "?" | "{" repeat_range "}" )?        ERE
//戻り値：エラー時はNULLを返し、reg_err_code/reg_err_msgにエラー情報を設定する。
static reg_compile_t *sequence_exp(const char *regexp, pattern_type_t mode) {
    reg_compile_t *preg_compile = new_reg_compile(regexp);
    preg_compile->mode = mode;
    reg_stat_t ret = REG_OK;

    while (ret==REG_OK) {
        ret = repeat_exp(preg_compile);
    }

    if (ret==REG_ERR) {
        reg_compile_free(preg_compile);
        preg_compile = NULL;
    } else {
        push_pattern(preg_compile, new_pattern(PAT_NULL));
    }
    return preg_compile;
}

// repeat_exp   = primary_exp ( "*" | "\{" repeat_range "\}" )?             BRE
//              | repeat_exp ( "*" | "+" | "?" | "{" repeat_range "}" )?        ERE
// repeat_range = num                                                       BRE/ERE
//              | num? "," num
//              | num "," num?
//戻り値：エラー時はreg_err_code/reg_err_msgにエラー情報を設定する。
static reg_stat_t repeat_exp(reg_compile_t *preg_compile) {
    reg_stat_t ret = primary_exp(preg_compile);
    if (ret!=REG_OK) return ret;

    pattern_t *pat = NULL;
    for (;;) {
        if (token1_is(preg_compile->p, '*') ||
            token_is_plus(preg_compile->p) ||       //'+' or '/+'
            token_is_question(preg_compile->p)) {   //'?' or '/?'
            if (*preg_compile->p=='\\') preg_compile->p++;
            pat = new_pattern(PAT_REPEAT);
            switch (*preg_compile->p) {
            case '*':
                pat->min = 0;
                pat->max = RE_DUP_MAX;
                break;
            case '+':
                pat->min = 1;
                pat->max = RE_DUP_MAX;
                break;
            case '?':
                pat->min = 0;
                pat->max = 1;
            }
            push_pattern(preg_compile, pat);
            preg_compile->p++;
            if ((g_syntax&RE_CONTEXT_INDEP_OPS)) continue;
        } else if (token_is_open_brace(preg_compile->p)) {  //'{' or '\{'
            if (new_repeat(preg_compile)==REG_ERR) return REG_ERR;
            if ((g_syntax&RE_CONTEXT_INDEP_OPS)) continue;
        }
        break;
    }

    if (token1_is(preg_compile->p, '\0')) {
        if (preg_compile->mode==PAT_SUBREG) {
            reg_set_err(REG_ERR_CODE_UNMATCHED_PAREN);
            return REG_ERR;
        }
        return REG_END;
    }
    return REG_OK;
}

// primary_exp  = char                                                      BRE/ERE
//              = "."                                                       BRE/ERE
//              | "\" special_char                                          BRE/ERE
//              | "[" char_set "]"                                          BRE/ERE
//              | "[^" char_set "]"                                         BRE/ERE
//              | "\(" reg_exp "\)"                                         BRE
//              | "(" reg_exp ")"                                               ERE
//              | "\" ( "1" | ... | "9" )                                   BRE
//              = "^"                                                           ERE
//              = "$"                                                           ERE
static reg_stat_t primary_exp(reg_compile_t *preg_compile) {
    pattern_t *pat = NULL;
    int cont_flag;
    do {
        cont_flag = 0;
        if (token1_is(preg_compile->p, '\0')) {
            if (preg_compile->mode==PAT_SUBREG) {
                reg_set_err(REG_ERR_CODE_UNMATCHED_PAREN);
                return REG_ERR;
            }
            return REG_END;
        } else if (token1_is(preg_compile->p, '.')) {
            pat = new_pattern(PAT_DOT);
        } else if (((token1_is(preg_compile->p, '*') || token_is_plus(preg_compile->p) || token_is_question(preg_compile->p))
                 && ((g_syntax&RE_CONTEXT_INVALID_OPS) || (preg_compile->prev_pat && preg_compile->prev_pat->type!=PAT_CARET)))
                || token_is_open_brace(preg_compile->p)) {
            reg_set_err(REG_ERR_CODE_INVALID_PRECEDING_REGEXP);
        } else if (token_is_open_paren(preg_compile->p)) {          //'('
            return new_subreg(preg_compile);
        } else if (token_is_close_paren(preg_compile->p)) {         //')'
            if (preg_compile->mode==PAT_SUBREG) return REG_END;
            if (g_syntax&RE_UNMATCHED_RIGHT_PAREN_ORD) goto L_CHAR;  //EREでは単独の')'はリテラル
            reg_set_err(REG_ERR_CODE_UNMATCHED_PAREN);
        } else if (token1_is(preg_compile->p, '\\') && preg_compile->p[1]>='1' && preg_compile->p[1]<='9') {
            int num = preg_compile->p[1]-'0';
            if (num>g_nparen_finished || num>MAX_PAREN) {
                reg_set_err(REG_ERR_CODE_INVALID_BACK_REF);
            } else {
                pat = new_pattern(PAT_BACKREF);
                pat->ref_num = num;
                preg_compile->p++;
            }
        } else if (token_is_vbar(preg_compile->p)) {
            return REG_END;
        } else if (token1_is(preg_compile->p, '\\') /*&& strchr(SPECIAL_CHARS, p[1])*/) {
            pat = new_pattern(PAT_CHAR);
            pat->c = preg_compile->p[1];
            preg_compile->p++;
        } else if (token1_is(preg_compile->p, '[')) {
            return new_char_set(preg_compile);
        } else if ((token1_is(preg_compile->p, '$') && (g_syntax&RE_CONTEXT_INDEP_ANCHORS))
                || token2_is(preg_compile->p, "$\0")
                || (token3_is(preg_compile->p, "$\\)") && preg_compile->mode==PAT_SUBREG)) {
            pat = new_pattern(PAT_DOLLAR);
            cont_flag = 1;
        } else if (token1_is(preg_compile->p, '^') &&
                 ((g_syntax&RE_CONTEXT_INDEP_ANCHORS) || preg_compile->regexp==preg_compile->p)) {
            pat = new_pattern(PAT_CARET);
            cont_flag = 1;
        } else {
            L_CHAR:
            pat = new_pattern(PAT_CHAR);
            char c = preg_compile->p[0];
            pat->c = (g_cflags&REG_ICASE)?toupper(c):c;
        }
        if (pat) {
            push_pattern(preg_compile, pat);
            pat = NULL;
        } else {
            return REG_ERR;
        }
        preg_compile->p++;
    } while (cont_flag);
    return REG_OK;
}

// new_subreg: \(\)の中身を抽出する
static reg_stat_t new_subreg(reg_compile_t *preg_compile) {
    pattern_t *pat = new_pattern(PAT_SUBREG);
    const char *regexp = preg_compile->p;
    assert(token_is_open_paren(regexp));
    preg_compile->nparen = ++g_nparen;
    int ref_num = g_nparen;

    if (*regexp=='\\') regexp++;
    regexp ++;
    pat->regcomp = reg_exp(regexp, PAT_SUBREG);
    if (pat->regcomp==NULL) {
        reg_pattern_free(pat);
        return REG_ERR;
    }
    pat->regcomp->match_here = 1;
    push_pattern(preg_compile, pat);
    preg_compile->p = pat->regcomp->p;
    g_nparen_finished++;  //nparen番目のカッコの処理完了
    pat->regcomp->ref_num = ref_num;
    preg_compile->nparen = g_nparen;
    return REG_OK;
}

// new_repeat: 繰り返し条件を生成する
// repeat_exp   = primary_exp ( "*" | "\{" repeat_range "\}" )?             BRE
//              | primary_exp ( "*" | "+" | "?" | "{" repeat_range "}" )?       ERE
// repeat_range = num                                                       BRE/ERE
//              | num? "," num
//              | num "," num?
static reg_stat_t new_repeat(reg_compile_t *preg_compile) {
    pattern_t *pat = new_pattern(PAT_REPEAT);
    const char *regexp = preg_compile->p;
    assert(token_is_open_brace(regexp));
    int mode = 1;   //第nパラメータ取得モード
    int min=0, max=-1;

    if (*regexp=='\\') regexp++;    //'\\'の分
    regexp ++;                      //'{'の分
    for (; *regexp != '\0' && mode<3; regexp++) {
        switch (mode) {
        case 1:             //第1パラメータの取得
            if        (token1_is(regexp, ',')) {
                mode = 2;
                break;
            } else if (token_is_close_brace(regexp)) {
                mode = 3;
                max = min;
                if (*regexp=='\\') regexp++;
            } else if (isdigit(*regexp)) {
                min = min*10 + *regexp - '0';
            } else {
                mode = 10;  //Error
            }
            break;
        case 2:             //第2パラメータの取得
            if        (token_is_close_brace(regexp)) {
                mode = 3;   //完了
                if (*regexp=='\\') regexp++;
            } else if (isdigit(*regexp)) {
                if (max<0) max = 0;
                max = max*10 + *regexp - '0';
            } else {
                mode = 10;  //Error
            }
            break;
        }
    }
    if (max<0) max = RE_DUP_MAX;
    if (mode<3) {           //閉じカッコ無し
        reg_pattern_free(pat);
        reg_set_err(REG_ERR_CODE_UNMATCHED_BRACE);
        return REG_ERR;
    } else if (mode==10 || min>max) {
        reg_pattern_free(pat);
        reg_set_err(REG_ERR_CODE_INVALID_CONTENT_OF_BRACE);
        return REG_ERR;
    } else if (max>RE_DUP_MAX) {
        reg_pattern_free(pat);
        reg_set_err(REG_ERR_CODE_REGEXP_TOO_BIG);
        return REG_ERR;
    }
    pat->min = min;
    pat->max = max;
    push_pattern(preg_compile, pat);
    preg_compile->p = regexp;
    return REG_OK;
}

// new_char_set: 文字セットを生成する。入力："[...]"
static reg_stat_t new_char_set(reg_compile_t *preg_compile) {
    pattern_t *pat = new_pattern(PAT_CHARSET);
    const char *regexp = preg_compile->p;
    assert(token1_is(regexp, '['));
    char_set_t *char_set = &pat->cset;
    char_set->size = 256;
    char_set->chars = calloc(char_set->size, sizeof(char));

    regexp++;
    if (token1_is(regexp, '^')) {
        if (token1_is(regexp+1, ']')) {
            reg_pattern_free(pat);
            reg_set_err(REG_ERR_CODE_UNMATCHED_BRACKET);
            return REG_ERR;
        }
        char_set->reverse = 1;
        regexp++;
    }
    if (token1_is(regexp, ']') || token1_is(regexp, '-')) {
        push_char_set(char_set, *regexp);
        regexp++;
    }
    for (; *regexp != '\0'; regexp++) {
        if (token1_is(regexp, '-') && !token1_is(regexp+1, '\0') && !token1_is(regexp+1,']')) {
            if (regexp[-1]>regexp[1]) {
                reg_pattern_free(pat);
                reg_set_err(REG_ERR_CODE_INVALID_RANGE_END);
                return REG_ERR;
            }
            for (int c=regexp[-1]+1; c<regexp[1]; c++) {
                push_char_set(char_set, c);
            }
            push_char_set(char_set, regexp[1]);
            regexp++;
        } else if (token2_is(regexp, "[:")) {
            preg_compile->p = regexp;
            if (set_char_class(preg_compile, char_set)==REG_ERR) {
                reg_pattern_free(pat);
                return REG_ERR;
            }
            regexp = preg_compile->p-1;
        } else if (!token1_is(regexp, ']')) {
            push_char_set(char_set, *regexp);
        } else {
            regexp++;
            break;
        }
    }
    push_pattern(preg_compile, pat);
    preg_compile->p = regexp;
    return REG_OK;
}

// new_char_class: 文字クラスを設定する。入力："[:...:]"
// char_class   = "upper"       #[A-Z]                                      POSIX
//              | "lower"       #[a-z]                                      POSIX
//              | "alpha"       #[A-Za-z]                                   POSIX
//              | "alnum"       #[A-Za-z0-9]                                POSIX
//              | "word"        #[A-Za-z0-9_]
//              | "digit"       #[0-9]                                      POSIX
//              | "xdigit"      #[0-9A-Fa-f]                                POSIX
//              | "punct"       #[]!"#$%&'()*+,-./:;<=>?@[\^_`{|}~-]        POSIX
//              | "blank"       #[ \t]                                      POSIX
//              | "space"       #[ \t\n\r\f\v]                              POSIX
//              | "cntrl"       #0x00-0x1f, 0x7f                            POSIX
//              | "graph"       #[^ \t\n\r\f\v[:cntrl:]]                    POSIX
//              | "print"       #[^\t\n\r\f\v[:cntrl:]]                     POSIX
static reg_stat_t set_char_class(reg_compile_t *preg_compile, char_set_t *char_set) {
    const char *regexp = preg_compile->p;
    assert(token2_is(regexp, "[:"));
    regexp += 2;
    if        (strncmp(regexp, "upper", 5)==0) {
        for (int c='A'; c<='Z'; c++) push_char_set(char_set, c);
        regexp += 5;
    } else if (strncmp(regexp, "lower", 5)==0) {
        for (int c='a'; c<='z'; c++) push_char_set(char_set, c);
        regexp += 5;
    } else if (strncmp(regexp, "alpha", 5)==0) {
        for (int c='A'; c<='Z'; c++) push_char_set(char_set, c);
        if (!(g_cflags&REG_ICASE))
            for (int c='a'; c<='z'; c++) push_char_set(char_set, c);
        regexp += 5;
    } else if (strncmp(regexp, "alnum", 5)==0) {
        for (int c='A'; c<='Z'; c++) push_char_set(char_set, c);
        if (!(g_cflags&REG_ICASE))
            for (int c='a'; c<='z'; c++) push_char_set(char_set, c);
        for (int c='0'; c<='9'; c++) push_char_set(char_set, c);
        regexp += 5;
    } else if (strncmp(regexp, "word", 4)==0) {
        for (int c='A'; c<='Z'; c++) push_char_set(char_set, c);
        if (!(g_cflags&REG_ICASE))
            for (int c='a'; c<='z'; c++) push_char_set(char_set, c);
        for (int c='0'; c<='9'; c++) push_char_set(char_set, c);
        push_char_set(char_set, '_');
        regexp += 4;
    } else if (strncmp(regexp, "digit", 5)==0) {
        for (int c='0'; c<='9'; c++) push_char_set(char_set, c);
        regexp += 5;
    } else if (strncmp(regexp, "xdigit", 6)==0) {
        for (int c='0'; c<='9'; c++) push_char_set(char_set, c);
        for (int c='A'; c<='F'; c++) push_char_set(char_set, c);
        if (!(g_cflags&REG_ICASE))
            for (int c='a'; c<='f'; c++) push_char_set(char_set, c);
        regexp += 6;
    } else if (strncmp(regexp, "punct", 5)==0) {
        push_char_set_str(char_set, "#[]!\"#$%&'()*+,-./:;<=>?@[\\^_`{|}~-]");
        regexp += 5;
    } else if (strncmp(regexp, "blanc", 5)==0) {
        push_char_set_str(char_set, " \t");
        regexp += 5;
    } else if (strncmp(regexp, "space", 5)==0) {
        push_char_set_str(char_set, " \t\n\r\f\v");
        regexp += 5;
    } else if (strncmp(regexp, "cntrl", 5)==0) {
        for (int c=0x01; c<=0x1f; c++) push_char_set(char_set, c);    //0x00は除外
        push_char_set(char_set, 0x7f);
        regexp += 5;
    } else if (strncmp(regexp, "graph", 5)==0) {
        char_set->reverse = 1;
        push_char_set_str(char_set, " \t\n\r\f\v");
        for (int c=0x01; c<=0x1f; c++) push_char_set(char_set, c);    //0x00は除外
        regexp += 5;
    } else if (strncmp(regexp, "print", 5)==0) {
        char_set->reverse = 1;
        push_char_set_str(char_set, "\t\n\r\f\v");
        for (int c=0x01; c<=0x1f; c++) push_char_set(char_set, c);    //0x00は除外
        regexp += 5;
    } else {
        reg_set_err(REG_ERR_CODE_INVALID_CHAR_CLASS);
        return REG_ERR;
    }
    if (!token2_is(regexp, ":]")) {
        reg_set_err(REG_ERR_CODE_UNMATCHED_BRACKET);
        return REG_ERR;
    }
    regexp +=2;
    preg_compile->p = regexp;
    return REG_OK;
}

//--------------------------------------------------------------------------------

//検索時の作業用
static const char *g_text;      //オリジナル検索文字列（^にマッチさせる位置）
static size_t      g_nmatch;    //pmatchの要素数
static regmatch_t *g_pmatch;    //マッチング位置

// reg_exec: search for compiled regexp anywhere in text
int reg_exec(reg_compile_t *preg_compile, const char *text, size_t nmatch, regmatch_t *pmatch, int eflags) {
    (void)eflags;
    g_text = text;
    g_nparen = 0;
    g_cflags = preg_compile->cflags;
    if (pmatch) {
        memset(pmatch, 0, sizeof(pmatch[0])*nmatch);
        g_nmatch = nmatch;
        g_pmatch = pmatch;
    } else {
        g_nmatch = 0;
        g_pmatch = NULL;
    }
    return reg_exec_main(preg_compile, text);
}
// reg_exec_main: search for compiled regexp anywhere in text
static int reg_exec_main(reg_compile_t *preg_compile, const char *text) {
    assert(preg_compile);
    assert(preg_compile->array);
    const char *rm_ep;
    pattern_t **pat = (pattern_t**)preg_compile->array->buckets;
    if (preg_compile->match_here) {
        if (reg_match_here(pat, text, &rm_ep)) {
            set_match(preg_compile->ref_num, text, rm_ep);
            return 0;
        }
    } else {
        do {    /* must look even if string is empty */
            if (reg_match_here(pat, text, &rm_ep)) {
                set_match(preg_compile->ref_num, text, rm_ep);
                return 0;
            }
        } while (*text++!='\0');
    }
    set_match(0, g_text, g_text);   //0クリア
    return 1;
}

//マッチング位置のインデックスを記録する
static void set_match(int idx, const char *rm_sp, const char *rm_ep) {
    if (g_pmatch) {
        g_pmatch[idx].rm_so = rm_sp-g_text;
        g_pmatch[idx].rm_eo = rm_ep-g_text;
    }
}

// reg_match_here: search for regexp at beginning of text
static int reg_match_here(pattern_t **pat, const char *text, const char **rm_ep) {
    switch (pat[0]->type) {
    case PAT_NULL:
        *rm_ep = text;
        return 1;
    case PAT_REPEAT:
        return reg_match_repeat(pat, text, rm_ep);
    default:
        break;
    }
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
        return pat->c == ((g_cflags&REG_ICASE)?toupper(*text):*text);
    case PAT_CHARSET:
        if (*text == '\0') return 0;
        if (strchr(pat->cset.chars, (g_cflags&REG_ICASE)?toupper(*text):*text))
            return !pat->cset.reverse;
        return pat->cset.reverse;
    case PAT_DOT:
        return *text != '\0';
    case PAT_SUBREG:
        if (reg_exec_main(pat->regcomp, text)==0) {
            *len = g_pmatch[pat->regcomp->ref_num].rm_eo - g_pmatch[pat->regcomp->ref_num].rm_so;
            return 1;
        }
        return 0;
    case PAT_BACKREF:
    {
        assert(pat->ref_num<g_nmatch);
        regmatch_t *regmat = g_pmatch + pat->ref_num;
        const char *substr = g_text + regmat->rm_so;
        int sublen = regmat->rm_eo - regmat->rm_so;
        assert(sublen>0);
        if (strncmp(text, substr, sublen)==0) {
            *len = sublen;
            return 1;
        }
        return 0;
    }
    case PAT_OR:
        for (int i=0; i<num_array(pat->or_array); i++) {
            reg_compile_t *subreg = peek_array(pat->or_array, i);
            if (reg_exec_main(subreg, text)==0) {
                *len = g_pmatch[subreg->ref_num].rm_eo - g_pmatch[subreg->ref_num].rm_so;
                return 1;
            }
        }
        return 0;
    case PAT_CARET:
        if (text==g_text) {
            *len = 0;
            return 1;
        }
        return 0;
    case PAT_DOLLAR:
        if (*text=='\0') {
            *len = 0;
            return 1;
        }
        return 0;
    default:
        assert(0);
    }
    return 0;
}

// reg_match_repeat: search for c\{m,n\} regexp at beginning of text
static int reg_match_repeat(pattern_t **pat, const char *text, const char **rm_ep) {
    pattern_t *rep = pat[0];
    pattern_t *c   = pat[1];
    assert(rep->type==PAT_REPEAT);
    int cnt = 0;
    int ret = 0;
    int len;
    do {    /* a \{0,n\} matches zero or more instances */
        if (cnt++>=rep->min && reg_match_here(pat+2, text, rm_ep)) ret = 1;   //最短一致ならここでreturn 1する
    } while (reg_match_pat(c, text, &len) && (text=text+len) && cnt<=rep->max);
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
        free(pat->cset.chars);
        break;
    case PAT_SUBREG:
        reg_compile_free(pat->regcomp);
        break;
    default:
        break;
    }
    free(pat);
}

typedef struct {
    int num;
    const char *name;
} flags_tbl_t;
#define REG_TBL(s) {s, #s}
static flags_tbl_t cflags_tbl[] = {
    REG_TBL(REG_EXTENDED),
    REG_TBL(REG_ICASE),
    REG_TBL(REG_NEWLINE),
};
static const char*cflags2str(int cflags) {
    static char buf[256];
    char *p = buf;
    int cnt = 0;
    *p = '\0';
    for (int i=0; i<sizeof(cflags_tbl)/sizeof(cflags_tbl[0]); i++) {
        if (cflags&cflags_tbl[i].num) {
            if (cnt++) *p++ = '|';
            strcpy(p, cflags_tbl[i].name);
            p += strlen(cflags_tbl[i].name);
        }
    }
    return buf;
}

//reg_dump: dump compiles regexp
void reg_dump(FILE *fp, reg_compile_t *preg_compile, int indent) {
    fprintf(stderr, "%*s", indent, ""); // indent個の空白を出力
    if (preg_compile==NULL) {
        fprintf(stderr, "preg_compile=(nul)\n");
        return;
    }
    fprintf(fp, "regexp='%s', match_here=%d, nparen=%d, ref_num=%d, cflags=%s\n",
        preg_compile->regexp, preg_compile->match_here, preg_compile->nparen, preg_compile->ref_num,
        cflags2str(preg_compile->cflags));
    array_t *array = preg_compile->array;
    for (int i=0; i<array->num; i++) {
        pattern_t *pat = array->buckets[i];
        fprintf(stderr, "%*s", indent, ""); // indent個の空白を出力
        fprintf(fp, "%2d: type=%-11s: ", i, pattern_type_str[pat->type]);
        switch (pat->type) {
        case PAT_CHAR:    fprintf(fp, "'%c'\n", pat->c); break;
        case PAT_CHARSET: fprintf(fp, "%s[%s]\n", pat->cset.reverse?"^":"", pat->cset.chars); break;
        case PAT_REPEAT:  fprintf(fp, "{%d,%d}\n", pat->min, pat->max); break;
        case PAT_SUBREG:  fprintf(fp, "\n");
                          reg_dump(fp, pat->regcomp, indent+2); break;
        case PAT_BACKREF: fprintf(fp, "\\%d\n", pat->ref_num); break;
        case PAT_OR:      fprintf(fp, "\n");
                          for (int i=0; i<num_array(pat->or_array); i++)
                            reg_dump(fp, peek_array(pat->or_array, i), indent+2);
                          break;
        default:          fprintf(fp, "\n"); break;
        }
    }
}

//regex関数から流用
static reg_err_info_t reg_err_def[] = {
    {REG_ERR_CODE_INVALID_CHAR_CLASS,       "Invalid character class name"},
    {REG_ERR_CODE_INVALID_BACK_REF,         "Invalid back reference"},
    {REG_ERR_CODE_UNMATCHED_BRACKET,        "Unmatched [, [^, [:, [., or [="},
    {REG_ERR_CODE_UNMATCHED_PAREN,          "Unmatched ( or \\("},
    {REG_ERR_CODE_UNMATCHED_BRACE,          "Unmatched { or \\{"},
    {REG_ERR_CODE_INVALID_CONTENT_OF_BRACE, "Invalid content of {} or \\{\\}"},
    {REG_ERR_CODE_INVALID_RANGE_END,        "Invalid range end"},
    {REG_ERR_CODE_INVALID_PRECEDING_REGEXP, "Invalid preceding regular expression"},
    {REG_ERR_CODE_REGEXP_TOO_BIG,           "Regular expression too big"},
};
reg_err_info_t reg_err_info = {0,""};    //エラーコード

//エラー情報をセットする。
static void reg_set_err(reg_err_code_t err_code) {
    for (int i=0; i<sizeof(reg_err_def)/sizeof(reg_err_def[0]); i++) {
        if (reg_err_def[i].err_code == err_code) {
            reg_err_info = reg_err_def[i];
            return;
        }
    }
    assert(0);
}
