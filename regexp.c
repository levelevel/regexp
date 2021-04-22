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

/* If this bit is set, then . matches newline.
   If not set, then it doesn't.  */
//# define RE_DOT_NEWLINE (RE_CONTEXT_INVALID_OPS << 1)

/* If this bit is set, then . doesn't match NUL.
   If not set, then it does.  */
//# define RE_DOT_NOT_NULL (RE_DOT_NEWLINE << 1)

/* If this bit is set, nonmatching lists [^...] do not match newline.
   If not set, they do.  */
//# define RE_HAT_LISTS_NOT_NEWLINE (RE_DOT_NOT_NULL << 1)

/* If this bit is set, then '{...}' defines an interval, and \{ and \}
     are literals.
  If not set, then '\{...\}' defines an interval.  */
//# define RE_NO_BK_BRACES (RE_NEWLINE_ALT << 1)

/* If this bit is set, (...) defines a group, and \( and \) are literals.
   If not set, \(...\) defines a group, and ( and ) are literals.  */
//# define RE_NO_BK_PARENS (RE_NO_BK_BRACES << 1)

/* If this bit is set, then | is an alternation operator, and \| is literal.
   If not set, then \| is an alternation operator, and | is literal.  */
//# define RE_NO_BK_VBAR (RE_NO_BK_REFS << 1)

/* If this bit is set, then an unmatched ) is ordinary.
   If not set, then an unmatched ) is invalid.  */
//# define RE_UNMATCHED_RIGHT_PAREN_ORD (RE_NO_EMPTY_RANGES << 1)

/* If this bit is set, then ignore case when matching.
   If not set, then case is significant.  */
//# define RE_ICASE (RE_INVALID_INTERVAL_ORD << 1)

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
    PAT_NULL,   //パターン配列の終わり
} pattern_type_t;

static const char *pattern_type_str[] = {
    "0", "PAT_CHAR", "PAT_CHARSET", "PAT_DOT", "PAT_REPEAT",
    "PAT_SUBREG", "PAT_BACKREF", "PAT_OR", "PAT_CARET", "PAT_DOLLAR", "PAT_NULL"
};

//[s], [^s]を格納する文字セット
typedef struct {
    char reverse;       //否定 [^...]
    char chars[256];    //0:指定あり、1:指定あり
} char_set_t;

//パターン
typedef struct {
    pattern_type_t  type;
    union {
        char        c;      //type=PAT_CHAR
        char_set_t *cset;   //type=PAT_CHARSET
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
    unsigned int newline_anchor:1;  //\nをアンカーとする（^/$にマッチする）
    unsigned int match_newline:1;   //このパターンで改行が^にマッチした（検索時の作業用）
    unsigned int submatch_newline:1;//サブパターンで改行が^にマッチした（検索時の作業用）
    short nparen;           //()の数（0-9）
    short ref_num;          //SUBREGの場合の後方参照の番号（0-9）
    int cflags;             //正規表現のコンパイルフラグ(REG_EXTENDED|...)
    int eflags;             //正規表現の実行フラグ(REG_NOTBOL|...)
    int syntax;             //正規表現のsyntax。RE_*
    //コンパイル時の作業用
    int rlen;               //regexpの長さ
    const char *regexp;     //元の正規表現
    const char *p;          //regexpを指す作業ポインタ
    pattern_t *prev_pat;    //直前のパターン
    pattern_type_t mode;    //処理中のパターン（PAT_SUBREG:"\)"が終了）
} reg_compile_t;

//関数の戻り値
typedef enum {
    REG_OK,                 //要素抽出
    REG_END,                //最後まで処理
    REG_ERR,                //エラー時
} reg_stat_t;

static void push_char_set(char_set_t *char_set, unsigned char c);
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

static int reg_exec_main(reg_compile_t *preg_compile, const char *text, int *match_len);
static int reg_match_here(reg_compile_t *preg_compile, pattern_t **pat, const char *text, const char **rm_ep);
static int set_match(reg_compile_t *preg_compile, const char *rm_sp, const char *rm_ep);
static int reg_match_pat(reg_compile_t *preg_compile, pattern_t *pat, const char *text, int *match_len);
static int reg_match_repeat(reg_compile_t *preg_compile, pattern_t **pat, const char *text, const char **rm_ep);
static void reg_pattern_free(pattern_t *pat);

static void reg_set_err(reg_err_code_t err_code);

#define MAX_PAREN 9
static int g_nparen;            //()の数(0-9)
static int g_nparen_finished;   //完了した()の数
int reg_syntax;                 //syntax (RE_*)
static const char* g_regexp_end;//regexpの末尾の次の位置

#define token_is_end(p)     ((p)>=g_regexp_end)

#define token1_is(p, c)     (*(p)==c)
#define token2_is(p, str)   (memcmp(p, str, 2)==0)

#define token_is_open_brace(p)  ((reg_syntax&RE_NO_BK_BRACES) ? token1_is(p, '{') : token2_is(p, "\\{"))
#define token_is_close_brace(p) ((reg_syntax&RE_NO_BK_BRACES) ? token1_is(p, '}') : token2_is(p, "\\}"))
#define token_is_open_paren(p)  ((reg_syntax&RE_NO_BK_PARENS) ? token1_is(p, '(') : token2_is(p, "\\("))
#define token_is_close_paren(p) ((reg_syntax&RE_NO_BK_PARENS) ? token1_is(p, ')') : token2_is(p, "\\)"))
#define token_is_plus(p)       (!(reg_syntax&RE_BK_PLUS_QM)   ? token1_is(p, '+') : token2_is(p, "\\+"))
#define token_is_question(p)   (!(reg_syntax&RE_BK_PLUS_QM)   ? token1_is(p, '?') : token2_is(p, "\\?"))
#define token_is_vbar(p)        ((reg_syntax&RE_NO_BK_VBAR)   ? token1_is(p, '|') : token2_is(p, "\\|"))

// reg_compile: compile regexp
// reg_exp      = sequence_exp ( "|" sequence_exp )*                            ERE
//              | "^"? sequence_exp* "$"?                                   BRE
// sequence_exp = repeat_exp repeat_exp*                                    BRE/ERE
//バイト列版。lenが負の場合は文字列として扱う。
reg_compile_t* reg_compile(const char *regexp, size_t len, size_t *nsub, int cflags) {
    int syntax;
    syntax = ((cflags & REG_EXTENDED) ? RE_SYNTAX_POSIX_EXTENDED : RE_SYNTAX_POSIX_BASIC);
    return reg_compile2(regexp, len, nsub, cflags, syntax);
}
//reg_compileと同じであるが、cflagsのREG_EXTENDEDの有無による機能の変更は無効になり、代わりにsyntaxを指定する。
reg_compile_t* reg_compile2(const char *regexp, size_t len, size_t *nsub, int cflags, int syntax) {
    if (regexp==NULL) {
        reg_set_err(1);
        return NULL;
    }
    reg_set_err(0);
    reg_compile_t *preg_compile;
    g_nparen = g_nparen_finished = 0;
    g_regexp_end = regexp + (len>=0?len:strlen(regexp));
    if (cflags&REG_ICASE) {
        syntax |= RE_ICASE;                 //ignore case
    }
    if (cflags&REG_NEWLINE) {
        syntax &= ~RE_DOT_NEWLINE;          //.を\nにマッチさせない
        syntax |= RE_HAT_LISTS_NOT_NEWLINE; //[^...]を\nにマッチさせない
    }
    reg_syntax = syntax;
    preg_compile = reg_exp(regexp, 0);
    if (preg_compile) {
        if (nsub) *nsub = g_nparen;
        preg_compile->nparen = g_nparen;
        preg_compile->cflags = cflags;
        preg_compile->syntax = reg_syntax;
        if (cflags&REG_NEWLINE) preg_compile->newline_anchor = 1;
    }

    return preg_compile;
}

//push_char_set: 文字セットに文字を追加する。
static void push_char_set(char_set_t *char_set, unsigned char c) {
    char_set->chars[c] = !char_set->reverse;
    unsigned uc = toupper(c);
    if ((reg_syntax&RE_ICASE) && c!=uc)
        char_set->chars[uc] = !char_set->reverse;
}
//push_char_set_str: 文字セットに文字列を追加する。
static void push_char_set_str(char_set_t *char_set, const char *str) {
    for (; *str; str++) push_char_set(char_set, *str);
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
        assert(token_is_end(preg_compile->p));
    }
    //reg_dump(stdout, preg_compile, 0);

    return preg_compile;
}

// sequence_exp = repeat_exp*                                                   BRE/ERE
// repeat_exp   = primary_exp ( "*" | "\+" | "\?" | "\{" repeat_range "\}" )?   BRE
//              | repeat_exp ( "*" | "+" | "?" | "{" repeat_range "}" )?            ERE
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

// repeat_exp   = primary_exp ( "*" | "\+" | "\?" | "\{" repeat_range "\}" )?   BRE
//              | repeat_exp ( "*" | "+" | "?" | "{" repeat_range "}" )?            ERE
// repeat_range = num                                                           BRE/ERE
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
            if ((reg_syntax&RE_CONTEXT_INDEP_OPS)) continue;
        } else if (token_is_open_brace(preg_compile->p)) {  //'{' or '\{'
            if (new_repeat(preg_compile)==REG_ERR) return REG_ERR;
            if ((reg_syntax&RE_CONTEXT_INDEP_OPS)) continue;
        }
        break;
    }

    if (token_is_end(preg_compile->p)) {
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
        if (token_is_end(preg_compile->p)) {
            if (preg_compile->mode==PAT_SUBREG) {
                reg_set_err(REG_ERR_CODE_UNMATCHED_PAREN);
                return REG_ERR;
            }
            return REG_END;
        } else if (token1_is(preg_compile->p, '.')) {
            pat = new_pattern(PAT_DOT);
        } else if ((token1_is(preg_compile->p, '*') ||
                    token_is_plus(preg_compile->p) ||               //'?'
                    token_is_question(preg_compile->p))             //'?'
                && (reg_syntax&RE_CONTEXT_INVALID_OPS || (preg_compile->prev_pat && preg_compile->prev_pat->type!=PAT_CARET))) {
            reg_set_err(REG_ERR_CODE_INVALID_PRECEDING_REGEXP);
        } else if (token_is_open_brace(preg_compile->p)) {          //'{'
            reg_set_err(REG_ERR_CODE_INVALID_PRECEDING_REGEXP);
        } else if (token_is_open_paren(preg_compile->p)) {          //'('
            return new_subreg(preg_compile);
        } else if (token_is_close_paren(preg_compile->p)) {         //')'
            if (preg_compile->mode==PAT_SUBREG) return REG_END;
            if (reg_syntax&RE_UNMATCHED_RIGHT_PAREN_ORD) goto L_CHAR;  //EREでは単独の')'はリテラル
            reg_set_err(REG_ERR_CODE_UNMATCHED_PAREN);
        } else if (token1_is(preg_compile->p, '\\') && preg_compile->p[1]>='1' && preg_compile->p[1]<='9') {
            int num = preg_compile->p[1] - '0';
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
        } else if (token1_is(preg_compile->p, '$') &&
                   (reg_syntax&RE_CONTEXT_INDEP_ANCHORS ||
                    token_is_end(preg_compile->p+1) ||
                    (token_is_close_paren(preg_compile->p+1) && preg_compile->mode==PAT_SUBREG))) {
            pat = new_pattern(PAT_DOLLAR);
            cont_flag = 1;
        } else if (token1_is(preg_compile->p, '^') &&
                   ((reg_syntax&RE_CONTEXT_INDEP_ANCHORS) || preg_compile->regexp==preg_compile->p)) {
            pat = new_pattern(PAT_CARET);
            cont_flag = 1;
        } else {
            L_CHAR:
            pat = new_pattern(PAT_CHAR);
            char c = preg_compile->p[0];
            pat->c = (reg_syntax&RE_ICASE)?toupper(c):c;
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
    g_nparen++;
    int ref_num = g_nparen; //ここで確定させる

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
    for (; !token_is_end(regexp) && mode<3; regexp++) {
        switch (mode) {
        case 1:             //第1パラメータの取得
            if        (token1_is(regexp, ',')) {
                mode = 2;
                break;
            } else if (token_is_close_brace(regexp)) {  //')'
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
            if        (token_is_close_brace(regexp)) {  //')'
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
    char_set_t *char_set;
    pat->cset = char_set = calloc(1, sizeof(char_set_t));;
    int bracket_is_closed = 0;  //']'が出現済み
    int accept_minus = 0;       //次の文字で範囲指定の'-'が利用できる

    regexp++;
    if (token1_is(regexp, '^')) {
        if (token1_is(regexp+1, ']')) {
            reg_pattern_free(pat);
            reg_set_err(REG_ERR_CODE_UNMATCHED_BRACKET);
            return REG_ERR;
        }
        char_set->reverse = 1;
        memset(char_set->chars, 1, sizeof(char_set->chars)*sizeof(char_set->chars[0]));
        regexp++;
    }
    if (token1_is(regexp, ']') || token1_is(regexp, '-')) {
        push_char_set(char_set, *regexp);
        regexp++;
        accept_minus = 1;
    }
    for (; !token_is_end(regexp); regexp++) {
        if (token1_is(regexp, '-') && !token_is_end(regexp+1) && !token1_is(regexp+1,']')) {
            if (!accept_minus || regexp[-1]>regexp[1]) {
                reg_pattern_free(pat);
                reg_set_err(REG_ERR_CODE_INVALID_RANGE_END);
                return REG_ERR;
            }
            for (int c=regexp[-1]+1; c<regexp[1]; c++) {
                push_char_set(char_set, c);
            }
            push_char_set(char_set, regexp[1]);
            regexp++;
            accept_minus = 0;
        } else if (token2_is(regexp, "[:")) {
            preg_compile->p = regexp;
            if (set_char_class(preg_compile, char_set)==REG_ERR) {
                reg_pattern_free(pat);
                return REG_ERR;
            }
            regexp = preg_compile->p-1;
            accept_minus = 0;
        } else if (token1_is(regexp, ']')) {
            regexp++;
            bracket_is_closed = 1;
            break;
        } else {
            push_char_set(char_set, *regexp);
            accept_minus = 1;
        }
    }
    if (!bracket_is_closed) {
        reg_pattern_free(pat);
        reg_set_err(REG_ERR_CODE_UNMATCHED_BRACKET);
        return REG_ERR;
    }
    push_pattern(preg_compile, pat);
    preg_compile->p = regexp;
    return REG_OK;
}

static void set_alpha(char_set_t *char_set) {
    for (int c='A'; c<='Z'; c++) push_char_set(char_set, c);
    for (int c='a'; c<='z'; c++) push_char_set(char_set, c);
}
static void set_alnum(char_set_t *char_set) {
    set_alpha(char_set);
    for (int c='0'; c<='9'; c++) push_char_set(char_set, c);
}
static void set_punct(char_set_t *char_set) {
    push_char_set_str(char_set, "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~");
}
// new_char_class: 文字クラスを設定する。入力："[:...:]"
// char_class   = "upper"       #[A-Z]                                      POSIX
//              | "lower"       #[a-z]                                      POSIX
//              | "alpha"       #[A-Za-z]                                   POSIX
//              | "alnum"       #[A-Za-z0-9]                                POSIX
//              | "word"        #[A-Za-z0-9_]
//              | "digit"       #[0-9]                                      POSIX
//              | "xdigit"      #[0-9A-Fa-f]                                POSIX
//              | "punct"       #[]!"#$%&'()*+,./:;<=>?@[\^_`{|}~-]         POSIX
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
        set_alpha(char_set);
        regexp += 5;
    } else if (strncmp(regexp, "alnum", 5)==0) {
        set_alnum(char_set);
        regexp += 5;
    } else if (strncmp(regexp, "word", 4)==0) {             //alnum + '_'
        set_alnum(char_set);
        push_char_set(char_set, '_');
        regexp += 4;
    } else if (strncmp(regexp, "digit", 5)==0) {
        for (int c='0'; c<='9'; c++) push_char_set(char_set, c);
        regexp += 5;
    } else if (strncmp(regexp, "xdigit", 6)==0) {
        for (int c='0'; c<='9'; c++) push_char_set(char_set, c);
        for (int c='A'; c<='F'; c++) push_char_set(char_set, c);
        if (!(reg_syntax&RE_ICASE))
            for (int c='a'; c<='f'; c++) push_char_set(char_set, c);
        regexp += 6;
    } else if (strncmp(regexp, "punct", 5)==0) {
        set_punct(char_set);
        regexp += 5;
    } else if (strncmp(regexp, "blank", 5)==0) {
        push_char_set_str(char_set, " \t");
        regexp += 5;
    } else if (strncmp(regexp, "space", 5)==0) {
        push_char_set_str(char_set, " \t\n\r\f\v");
        regexp += 5;
    } else if (strncmp(regexp, "cntrl", 5)==0) {
        for (int c=0x00; c<=0x1f; c++) push_char_set(char_set, c);
        push_char_set(char_set, 0x7f);
        regexp += 5;
    } else if (strncmp(regexp, "graph", 5)==0) {            //alnum + punct
        set_alnum(char_set);
        set_punct(char_set);
        regexp += 5;
    } else if (strncmp(regexp, "print", 5)==0) {            //alnum + punct + ' '
        set_alnum(char_set);
        set_punct(char_set);
        push_char_set(char_set, ' ');
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

#define text_is_end(p) ((p)>=g_text_end)
//検索時の作業用
static const char *g_text;      //オリジナル検索文字列（^にマッチさせる位置）
static const char *g_text_end;  //検索文字列の末尾の次の位置
static const char *g_start;     //パターン一致の開始位置
static int g_newline_anchor;    //\nをアンカーとする（^/$にマッチする）
static size_t      g_nmatch;    //pmatchの要素数
static regmatch_t *g_pmatch;    //マッチング位置
static int         g_eflags;

// reg_exec: search for compiled regexp anywhere in text
//成功した場合は0、失敗した場合は0以外を返す。
int reg_exec(reg_compile_t *preg_compile, const char *text, size_t len, size_t nmatch, regmatch_t *pmatch, int eflags) {
    if (preg_compile==NULL || text==NULL) return 1;
    g_text = text;
    g_text_end = text + (len>=0?len:strlen(text));
    reg_syntax = preg_compile->syntax;
    g_newline_anchor = preg_compile->newline_anchor;
    preg_compile->eflags = g_eflags = eflags;
    preg_compile->match_newline = 0;
    if (pmatch) {
        memset(pmatch, 0, sizeof(pmatch[0])*nmatch);
        g_nmatch = nmatch;
        g_pmatch = pmatch;
    } else {
        g_nmatch = 0;
        g_pmatch = NULL;
    }
    int match_len;
    return reg_exec_main(preg_compile, text, &match_len);
}
// reg_exec_main: search for compiled regexp anywhere in text
//成功した場合は0、失敗した場合は0以外を返す。
static int reg_exec_main(reg_compile_t *preg_compile, const char *text, int *match_len) {
    assert(preg_compile);
    assert(preg_compile->array);
    const char *rm_ep;
    pattern_t **pat = (pattern_t**)preg_compile->array->buckets;
    do {    /* must look even if string is empty */
        g_start = text;
        if (reg_match_here(preg_compile, pat, text, &rm_ep)) {
            set_match(preg_compile, text, rm_ep);
            *match_len = rm_ep - text;
            return 0;   //成功
        }
        if (preg_compile->match_here) break;
    } while (!text_is_end(text++));
    g_pmatch[0].rm_so = 0;
    g_pmatch[0].rm_eo = 0;
    return 1;   //失敗
}

//マッチング位置のインデックスを記録し、マッチした文字列の長さを返す。
static int set_match(reg_compile_t *preg_compile, const char *rm_sp, const char *rm_ep) {
    int so = rm_sp - g_text;
    int eo = rm_ep - g_text;
    int match_len = eo - so;
    if (g_pmatch) {
        int idx = preg_compile->ref_num;
        //\nに^がマッチした場合は開始位置を\nの次の位置に調整する。
        if (preg_compile->match_newline || preg_compile->submatch_newline) {
            assert(g_text[so]=='\n');
            so++;
        }
        g_pmatch[idx].rm_so = so;
        g_pmatch[idx].rm_eo = eo;
    }
    return match_len;
}

// reg_match_here: search for regexp at beginning of text
static int reg_match_here(reg_compile_t *preg_compile, pattern_t **pat, const char *text, const char **rm_ep) {
    switch (pat[0]->type) {
    case PAT_NULL:
        *rm_ep = text;
        return 1;
    case PAT_REPEAT:
        return reg_match_repeat(preg_compile, pat, text, rm_ep);
    default:
        break;
    }
    int match_len;
    if (reg_match_pat(preg_compile, pat[0], text, &match_len)) {
        return reg_match_here(preg_compile, pat+1, text+match_len, rm_ep);
    }
    return 0;
}

// reg_match_pat: 1個のパターンがマッチするかを判定
static int reg_match_pat(reg_compile_t *preg_compile, pattern_t *pat, const char *text, int *match_len) {
    *match_len = 1;   //patにマッチしたtextの長さ
    switch (pat->type) {
    case PAT_CHAR:
        return pat->c == ((reg_syntax&RE_ICASE)?toupper(*text):*text);
    case PAT_CHARSET:
        if (text_is_end(text)) return 0;
        if (*text == '\n' && pat->cset->reverse && reg_syntax&RE_HAT_LISTS_NOT_NEWLINE) return 0;
        return pat->cset->chars[(reg_syntax&RE_ICASE)?toupper(*text):*text];
    case PAT_DOT:
        if (!(reg_syntax&RE_DOT_NEWLINE) && *text=='\n') return 0;
        if ((reg_syntax&RE_DOT_NOT_NULL) && *text=='\0') return 0;
        return !text_is_end(text);
    case PAT_SUBREG:
        if (reg_exec_main(pat->regcomp, text, match_len)==0) {
            preg_compile->submatch_newline = (pat->regcomp->match_newline || pat->regcomp->submatch_newline);
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
            *match_len = sublen;
            return 1;
        }
        return 0;
    }
    case PAT_OR:
    {
        int num = num_array(pat->or_array);
        for (int i=0; i<num; i++) {
            reg_compile_t *subreg = peek_array(pat->or_array, i);
            if (reg_exec_main(subreg, text, match_len)==0) {
                preg_compile->submatch_newline = (subreg->match_newline || subreg->submatch_newline);
                return 1;
            }
        }
        return 0;
    }
    case PAT_CARET:
        if (g_newline_anchor && g_start==text && *text=='\n') {
            preg_compile->match_newline = 1;
            *match_len = 1;
            return 1;
        } else if (g_newline_anchor && g_start<text && *(text-1)=='\n') {
            *match_len = 0;
            return 1;
        } else if (!(g_eflags&REG_NOTBOL) && text==g_text) {
            *match_len = 0;
            return 1;
        }
        return 0;
    case PAT_DOLLAR:
        if (!(g_eflags&REG_NOTEOL) && text_is_end(text)) {
            *match_len = 0;
            return 1;
        } else if (g_newline_anchor && *text=='\n') {
            *match_len = 0;
            return 1;
        }
        return 0;
    default:
        assert(0);
    }
    return 0;
}

// reg_match_repeat: search for c\{m,n\} regexp at beginning of text
static int reg_match_repeat(reg_compile_t *preg_compile, pattern_t **pat, const char *text, const char **rm_ep) {
    pattern_t *rep = pat[0];
    pattern_t *c   = pat[1];
    assert(rep->type==PAT_REPEAT);
    int cnt = 0;
    int ret = 0;
    int len;
    do {    /* a \{0,n\} matches zero or more instances */
        if (cnt++>=rep->min && reg_match_here(preg_compile, pat+2, text, rm_ep)) ret = 1;   //最短一致ならここでreturn 1する
    } while (reg_match_pat(preg_compile, c, text, &len) && (text+=len) && cnt<=rep->max);
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
        free(pat->cset);
        pat->cset = NULL;
        break;
    case PAT_SUBREG:
        reg_compile_free(pat->regcomp);
        pat->regcomp = NULL;
        break;
    case PAT_OR:
        if (pat->or_array) {
            int num = num_array(pat->or_array);
            for (int i=0; i<num; i++) {
                reg_compile_free(peek_array(pat->or_array, i));
            }
            free_array(pat->or_array);
            pat->or_array = NULL;
        }
        break;
    default:
        break;
    }
    free(pat);
}

typedef struct {
    int num;
    const char *name;
} name_tbl_t;
#define REG_TBL(s) {REG_##s, #s}
#define RE_TBL(s) {RE_##s, #s}
static name_tbl_t cflags_tbl[] = {
    REG_TBL(EXTENDED),
    REG_TBL(ICASE),
    REG_TBL(NEWLINE),
};
static name_tbl_t eflags_tbl[] = {
    REG_TBL(NOTBOL),
    REG_TBL(NOTEOL),
};
static name_tbl_t syntax_tbl[] = {
    RE_TBL(CONTEXT_INDEP_ANCHORS),
    RE_TBL(CONTEXT_INDEP_OPS),
    RE_TBL(CONTEXT_INVALID_OPS),
    RE_TBL(BK_PLUS_QM),
    RE_TBL(DOT_NEWLINE),
    RE_TBL(DOT_NOT_NULL),
    RE_TBL(HAT_LISTS_NOT_NEWLINE),
    RE_TBL(NO_BK_BRACES),
    RE_TBL(NO_BK_PARENS),
    RE_TBL(NO_BK_VBAR),
    RE_TBL(UNMATCHED_RIGHT_PAREN_ORD),
    RE_TBL(ICASE),
};
static const char* reg_flag2str(int flags, name_tbl_t *tbl, size_t size, char buf[]) {
    char *p = buf;
    int cnt = 0;
    int junk = flags;
    *p = '\0';
    for (int i=0; i<size; i++) {
        if (flags&tbl[i].num) {
            p += sprintf(p, "%s%s", cnt?"|":"REG_", tbl[i].name);
            cnt++;
            junk &= ~tbl[i].num;
        }
    }
    if (junk) sprintf(p, "|0x%x", junk);
    return buf;
}
const char* reg_cflags2str(int cflags) {
    static char buf[256];
    return reg_flag2str(cflags, cflags_tbl, sizeof(cflags_tbl)/sizeof(cflags_tbl[0]), buf);
}
const char* reg_eflags2str(int eflags) {
    static char buf[256];
    return reg_flag2str(eflags, eflags_tbl, sizeof(eflags_tbl)/sizeof(eflags_tbl[0]), buf);
}
const char* reg_syntax2str(int syntax) {
    static char buf[1024];
    return reg_flag2str(syntax, syntax_tbl, sizeof(syntax_tbl)/sizeof(syntax_tbl[0]), buf);
}
void reg_print_str(FILE *fp, const char *bstr, int len) {
    const unsigned char *p = (const unsigned char*)bstr;
    for(int i=0; i<len; i++, p++) {
        switch (*p) {
        case '\f': fputs("\\f", fp); break;
        case '\n': fputs("\\n", fp); break;
        case '\r': fputs("\\r", fp); break;
        case '\t': fputs("\\t", fp); break;
        case '\v': fputs("\\v", fp); break;
        default:
            if (iscntrl(*p) || *p>0x7f) fprintf(fp, "\\%03o", *p);
            else fputc(*p, fp);
        }
    }
}
static void reg_print_char_set(FILE *fp, char_set_t *char_set) {
    for (int i=0; i<256; i++) {
        if (char_set->chars[i]) {
            const unsigned char c = i;
            reg_print_str(fp, (const char*)&c, 1);
        }
    }
}

//reg_dump: dump compiles regexp
void reg_dump(FILE *fp, reg_compile_t *preg_compile, int indent) {
    fprintf(stderr, "%*s", indent, ""); // indent個の空白を出力
    if (preg_compile==NULL) {
        fprintf(stderr, "preg_compile=(nul)\n");
        return;
    }
    fprintf(fp, "regexp='");
    reg_print_str(fp, preg_compile->regexp, preg_compile->rlen);
    fprintf(fp, "', match_here=%d, newline_anchor=%d, nparen=%d, ref_num=%d, cflags=%s\n",
        preg_compile->match_here, preg_compile->newline_anchor, preg_compile->nparen, preg_compile->ref_num,
        reg_cflags2str(preg_compile->cflags));

    fprintf(stderr, "%*s", indent, ""); // indent個の空白を出力
    fprintf(fp, "syntax=%s\n", reg_syntax2str(preg_compile->syntax));

    array_t *array = preg_compile->array;
    for (int i=0; i<array->num; i++) {
        pattern_t *pat = array->buckets[i];
        fprintf(stderr, "%*s", indent, ""); // indent個の空白を出力
        fprintf(fp, "%2d: type=%-11s: ", i, pattern_type_str[pat->type]);
        switch (pat->type) {
        case PAT_CHAR:
        {
            fprintf(fp, "'");
            reg_print_str(fp, &pat->c, 1);
            fprintf(fp, "'\n");
            break;
        }
        case PAT_CHARSET:
            fprintf(fp, "%s[", pat->cset->reverse?"^":"");
            reg_print_char_set(fp, pat->cset);
            fprintf(fp, "]\n");
            break;
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
    {REG_ERR_CODE_OK,                       "Success"},
    {REG_ERR_CODE_NOMATCH,                  "No match"},
    {REG_ERR_CODE_INVALID_PATTERN,          "Invalid pattern"},
    {REG_ERR_CODE_INVALID_CHAR_CLASS,       "Invalid character class name"},
    {REG_ERR_CODE_INVALID_BACK_REF,         "Invalid back reference"},
    {REG_ERR_CODE_UNMATCHED_BRACKET,        "Unmatched [, [^, [:, [., or [="},
    {REG_ERR_CODE_UNMATCHED_PAREN,          "Unmatched ( or \\("},
    {REG_ERR_CODE_UNMATCHED_BRACE,          "Unmatched \\{"},
    {REG_ERR_CODE_INVALID_CONTENT_OF_BRACE, "Invalid content of \\{\\}"},
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
