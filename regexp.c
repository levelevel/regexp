// reg_exp      = sequence_exp ( "|" sequence_exp )*                                ERE
//              | "^"? sequence_exp* "$"?                                       BRE
// sequence_exp = repeat_exp*                                                   BRE/ERE
// repeat_exp   = primary_exp ( "*" | "\+" | "\?" | "\{" repeat_range "\}" )?   BRE
//              | repeat_exp ( "*" | "+" | "?" | "{" repeat_range "}" )?            ERE
//              | repeat_exp ( "*?" | "+?" | "??" | "{" repeat_range "}?" )?        PCRE
//              | repeat_exp ( "*+" | "++" | "?+" | "{" repeat_range "}+" )?        PCRE
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
//              | "(?:" reg_exp ")"                                             PCRE
//              | "(?=" reg_exp ")" |" (?<=" reg_exp ")"                        PCRE
//              | "(?!" reg_exp ")" |" (?<!" reg_exp ")"                        PCRE
//              | "\" ( "1" | ... | "9" )                                   BRE/ERE
//              | "^" | "$"                                                     ERE
//              | "\A" | "\z" | "\Z" | "\K"                                     PCRE
//              | "\w" | "\W" | "\s" | "\S"| "\b" | "\B"                    GNU/PCRE
//              | "\d" | "^D" | "\h" | "^H" | "\v" | "\V" | "\R" | "\N"         PCRE
//              | "\<" | "\>" | "\`" | "\'"                                 GNU
//              | "\a" | "\e" | "\f" | "\n" | "\r" | "\t"                       PCRE
//              | "\cx" | "\0dd" | "\ddd" | "\o{ddd..}"                         PCRE
//              | "\N{U+hh.}" |"\xhh" | "\x{hh..}"                              PCRE
// char_set     = char char_set*                                            BRE/ERE
//              | char "-" char
//              | "[:" char_class ":]"
// char_class   = "upper"       #[A-Z]                                      POSIX
//              | "lower"       #[a-z]                                      POSIX
//              | "alpha"       #[A-Za-z]                                   POSIX
//              | "alnum"       #[A-Za-z0-9]                                POSIX
//              | "digit"       #[0-9]                                      POSIX
//              | "xdigit"      #[0-9A-Fa-f]                                POSIX
//              | "punct"       #[]!"#$%&'()*+,-./:;<=>?@[\^_`{|}~-]        POSIX
//              | "blank"       #[ \t]                                      POSIX
//              | "space"       #[ \t\n\r\f\v]                              POSIX
//              | "cntrl"       #0x00-0x1f, 0x7f                            POSIX
//              | "graph"       #[^ \t\n\r\f\v[:cntrl:]]                    POSIX
//              | "print"       #[^\t\n\r\f\v[:cntrl:]]                     POSIX
//              | "ascii"       #[\x00-\x7f]                                PCRE
//              | "word"        #[_A-Za-z0-9]                               PCRE

// https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html
// A Regular Expression Matcher
// Code by Rob Pike

// /usr/include/regex.h ---------------------------------------------------

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

/* If this bit is set, do not process the GNU regex operators.
   If not set, then the GNU regex operators are recognized. */
//# define RE_NO_GNU_OPS (RE_NO_POSIX_BACKTRACKING << 1)

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
/* Use PMATCH[0] to delimit the start and end of the search in the
   buffer.  */
//#define REG_STARTEND (1 << 2)

// /usr/include/regex.h ---------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>

#include "array.h"
#define __USE_GNU
#include "regexp.h"

//パターンタイプ
typedef enum {
    PAT_CHAR=1,         // 1バイト文字(ASCII)
#ifdef REG_ENABLE_UTF8
    PAT_WC,             // ワイド文字
#endif
    PAT_CHARSET,        // [s] [^s]
    PAT_DOT,            // .
    PAT_REPEAT,         // *, {m,n}
    PAT_SUBREG,         // ()
    PAT_BACKREF,        // \1
    PAT_OR,             // |
    PAT_CARET,          // ^
    PAT_DOLLAR,         // $
    PAT_TEXT_FIRST,     // \A, \`
    PAT_TEXT_LAST,      // \z, \'
    PAT_TEXT_LAST_NL,   // \Z
    PAT_SET_START,      //\K
    PAT_WORD_FIRST,     // \<
    PAT_WORD_LAST,      // \>
    PAT_WORD_DELIM,     // \b
    PAT_NOT_WORD_DELIM, // \B
    PAT_SET_OPTION,     //syntaxをセットする
    PAT_NULL,           //パターン配列の終わり
} pattern_type_t;

static const char *pattern_type_str[] = {
    "0", "PAT_CHAR", "PAT_WC", "PAT_CHARSET", "PAT_DOT", "PAT_REPEAT",
    "PAT_SUBREG", "PAT_BACKREF", "PAT_OR", "PAT_CARET", "PAT_DOLLAR",
    "PAT_TEXT_FIRTS", "PAT_TEXT_LAST", "PAT_TEXT_LAST_NL", "PAT_SET_START",
    "PAT_WORD_FIRST", "PAT_WORD_LAST", "PAT_WORD_DELIM", "PAT_NOT_WORD_DELIM",
    "PAT_SET_OPTION", "PAT_NULL"
};

#ifdef REG_ENABLE_UTF8
enum {
    CHAR_CLASS_ALPHA  = (1),      //文字クラス
    CHAR_CLASS_ALNUM  = (1<<1),
    CHAR_CLASS_DIGIT  = (1<<2),
    CHAR_CLASS_XDIGIT = (1<<3),
    CHAR_CLASS_LOWER  = (1<<4),
    CHAR_CLASS_UPPER  = (1<<5),
    CHAR_CLASS_BLANK  = (1<<6),
    CHAR_CLASS_SPACE  = (1<<7),
    CHAR_CLASS_PRINT  = (1<<8),
    CHAR_CLASS_GRAPH  = (1<<9),
    CHAR_CLASS_CNTRL  = (1<<10),
    CHAR_CLASS_PUNCT  = (1<<11),
};
typedef struct {
    wchar_t wc_start;       //開始文字（ワイド文字）
    wchar_t wc_end;         //終了文字（ワイド文字）
} char_range_t;
#endif //REG_ENABLE_UTF8

//[s], [^s]を格納する文字セット
typedef struct {
    char reverse;           //否定 [^...]
    char chars[256];        //ASCIIのみ、0:指定あり、1:指定あり
#ifdef REG_ENABLE_UTF8
    struct {
        wchar_t *wcstr;     //null終端ワイド文字列（1バイト文字はcharsで管理するのでここには含まない）
        int wcs_len;        //wcstrに格納された文字列の長さ（終端文字含まない文字数）
        int wcs_size;       //wcstrの最大要素数
    };
    int char_class;         //文字クラス（CHARA_CLASS_*のbit-OR）
    array_t *ranges;        //char_range_t*のアレイ
#endif //REG_ENABLE_UTF8
} char_set_t;

//パターン
typedef struct {
    pattern_type_t  type;
    union {
        char        c;      //type=PAT_CHAR
        wchar_t     wc;     //type==PAT_WC
        char_set_t *cset;   //type=PAT_CHARSET
        struct {
            int min, max;   //type=PAT_REPEAT
            enum {REP_GREEDY=0, REP_POSEESSIVE, REP_LAZY} rep_mode;
        };
        reg_compile_t *regcomp; //type=PAT_SUBREG
        int ref_num;        //type=PAT_BACKREF, 1-9
        array_t *or_array;  //type=PAT_OR, regcomp_t*のarray
        struct {
            reg_syntax_t set_syntax; //type=PAT_SET_OPTION
            reg_syntax_t unset_syntax;
        };
    };
} pattern_t;

//正規表現のコンパイル結果
typedef struct regcomp {
    array_t *array;         //pattern_t*のアレイ
    unsigned int match_here :1;     //現在位置からマッチさせる
    short nparen;           //()の数（0-9）
    short ref_num;          //SUBREGの場合の後方参照の番号（0-9）
    short assertion;        //LOOKAHEAD,LOOKBEHIND
    int total_len;          //このパターン全体の長さ。NEGATIVE_LOOKBEHINDで使用。
    int backref_flags;      //(1 << n)ビットが1なら\nによる後方参照対象がこのsubreg内に存在する
    int cflags;             //正規表現のコンパイルフラグ(REG_EXTENDED|...)
    int eflags;             //正規表現の実行フラグ(REG_NOTBOL|...)
    reg_syntax_t syntax;    //正規表現のsyntax。RE_*
    struct {
        reg_syntax_t set_syntax; //SET_OPTION
        reg_syntax_t unset_syntax;//UNSET_OTION
    };
    //コンパイル時の作業用
    const char *regexp;     //コンパイル時に指定された正規表現
    int rlen;               //regexpの長さ
    const char *p;          //regexpを指す作業ポインタ
    pattern_t *prev_pat;    //直前のパターン
    pattern_type_t mode;    //処理中のパターン（PAT_SUBREG:"\)"が終了）
} reg_compile_t;

//先読み・後読みアサーション
enum {
    POSITIVE_LOOKAHEAD = 1,
    NEGATIVE_LOOKAHEAD,
    POSITIVE_LOOKBEHIND,
    NEGATIVE_LOOKBEHIND,
};
static const char* assertion_str[] = {
    "", "POSITIVE_LOOKAHEAD", "NEGATIVE_LOOKAHEAD", "POSITIVE_LOOKBEHIND", "NEGATIVE_LOOKBEHIND",
};

//関数の戻り値
typedef enum {
    REG_OK,                 //要素抽出
    REG_END,                //最後まで処理
    REG_ERR,                //エラー時
} reg_stat_t;

static pattern_t *new_pattern(pattern_type_t type);
static reg_compile_t *new_reg_compile(void);
static reg_compile_t *reg_exp(const char *regexp, pattern_type_t mode);
static reg_compile_t *sequence_exp(const char *regexp, pattern_type_t mode);
static reg_stat_t repeat_exp  (reg_compile_t *preg_compile);
static reg_stat_t primary_exp (reg_compile_t *preg_compile);
static reg_stat_t set_backref(reg_compile_t *preg_compile);
static reg_stat_t set_subreg  (reg_compile_t *preg_compile);
static int get_length(reg_compile_t *preg_compile);
static reg_stat_t set_options(reg_compile_t *preg_compile, reg_syntax_t* set_syntax, reg_syntax_t* unset_syntax);
static reg_stat_t set_repeat  (reg_compile_t *preg_compile);
static void push_char(reg_compile_t *preg_compile, int c);
static void push_char_set(char_set_t *char_set, unsigned char c);
static void push_char_set_str(char_set_t *char_set, const char *str);
static reg_stat_t set_char_set(reg_compile_t *preg_compile);
static reg_stat_t set_char_set_ext(reg_compile_t *preg_compile);
static void set_alpha(char_set_t *char_set);
static void set_alnum(char_set_t *char_set);
static void set_digit(char_set_t *char_set);
static void set_punct(char_set_t *char_set);
static reg_stat_t set_char_class(reg_compile_t *preg_compile, char_set_t *char_set);
static reg_stat_t set_escape_char(reg_compile_t *preg_compile);
static int get_escape_char(reg_compile_t *preg_compile, int enable_backref);

static int reg_exec_main(reg_compile_t *preg_compile, const char *text, int *match_len);
static int set_match(reg_compile_t *preg_compile, const char *rm_sp, const char *rm_ep);
static int reg_match_here(reg_compile_t *preg_compile, pattern_t **pat, const char *text, const char **rm_ep);
static int reg_match_repeat(reg_compile_t *preg_compile, pattern_t **pat, const char *text, const char **rm_ep);
static int reg_match_pat(reg_compile_t *preg_compile, pattern_t *pat, const char *text, int *match_len);
static void reg_pattern_free(pattern_t *pat);
static void reg_set_err(reg_err_code_t err_code);

#ifdef REG_ENABLE_UTF8
static int reg_mblen(const char *str, int str_len);
#endif //REG_ENABLE_UTF8

#define MAX_PAREN 9
static int g_nparen;            //()の数(0-9)

reg_syntax_t reg_syntax;        //syntax (RE_*)
#define syntax_is(b)        (reg_syntax&(b))

static const char* g_regexp_end;//regexpの末尾の次の位置
#define token_is_end(p)     ((p)>=g_regexp_end)

#define token1_is(p, c)     (*(p)==c)
#define token2_is(p, str)   (memcmp(p, str, 2)==0)
#define token1_in(p, str)   (strchr(str,*(p)))

#define token_is_open_brace(p)  (syntax_is(RE_NO_BK_BRACES) ? token1_is(p, '{') : token2_is(p, "\\{"))
#define token_is_close_brace(p) (syntax_is(RE_NO_BK_BRACES) ? token1_is(p, '}') : token2_is(p, "\\}"))
#define token_is_open_paren(p)  (syntax_is(RE_NO_BK_PARENS) ? token1_is(p, '(') : token2_is(p, "\\("))
#define token_is_close_paren(p) (syntax_is(RE_NO_BK_PARENS) ? token1_is(p, ')') : token2_is(p, "\\)"))
#define token_is_plus(p)       (!syntax_is(RE_BK_PLUS_QM)   ? token1_is(p, '+') : token2_is(p, "\\+"))
#define token_is_question(p)   (!syntax_is(RE_BK_PLUS_QM)   ? token1_is(p, '?') : token2_is(p, "\\?"))
#define token_is_vbar(p)        (syntax_is(RE_NO_BK_VBAR)   ? token1_is(p, '|') : token2_is(p, "\\|"))
#define token_is_octal(p)       ((*p)>='0' && (*p)<='7')
#define token_is_digit(p)       isdigit(*(p))
#define token_is_xdigit(p)      isxdigit(*(p))
#define token_len(p)            (g_regexp_end-(p))

#define error_if_token_end(p,code)  if (token_is_end(p)) {reg_set_err(code); return REG_ERR;}

// reg_compile: compile regexp
// reg_exp      = sequence_exp ( "|" sequence_exp )*                            ERE
//              | "^"? sequence_exp* "$"?                                   BRE
// sequence_exp = repeat_exp repeat_exp*                                    BRE/ERE
//バイト列版。lenが負の場合は文字列として扱う。
reg_compile_t* reg_compile(const char *regexp, size_t len, size_t *nsub, int cflags) {
    reg_syntax_t syntax;
    syntax = ((cflags & REG_EXTENDED) ? RE_SYNTAX_POSIX_EXTENDED : RE_SYNTAX_POSIX_BASIC);
    return reg_compile2(regexp, len, nsub, cflags, syntax);
}
//reg_compileと同じであるが、cflagsのREG_EXTENDEDの有無による機能の変更は無効になり、代わりにsyntaxを指定する。
reg_compile_t* reg_compile2(const char *regexp, size_t len, size_t *nsub, int cflags, reg_syntax_t syntax) {
    if (regexp==NULL) {
        reg_set_err(1);
        return NULL;
    }
    reg_set_err(0);
    reg_compile_t *preg_compile;
    g_nparen = 0;
    g_regexp_end = regexp + (len>=0?len:strlen(regexp));
    if (cflags&REG_ICASE) {
        syntax |= RE_ICASE;                 //ignore case
    }
    if (cflags&REG_NEWLINE) {
        syntax |= RE_MULTILINE;             //マルチラインモード
        syntax &= ~RE_DOT_NEWLINE;          //.を\nにマッチさせない
        syntax |= RE_HAT_LISTS_NOT_NEWLINE; //[^...]を\nにマッチさせない
    }
    reg_syntax = syntax;
    preg_compile = reg_exp(regexp, 0);
    if (preg_compile) {
        if (nsub) *nsub = g_nparen;
        preg_compile->nparen = g_nparen;
        preg_compile->cflags = cflags;
    }

    return preg_compile;
}

// new_pattern: allocate new pattern for the type
static pattern_t *new_pattern(pattern_type_t type) {
    pattern_t *pat = calloc(1, sizeof(pattern_t));
    assert(pat);
    pat->type = type;
    return pat;
}

// new_reg_compile: allocate new reg_compile
static reg_compile_t *new_reg_compile(void) {
    reg_compile_t *preg_compile = calloc(1, sizeof(reg_compile_t));
    assert(preg_compile);
    preg_compile->syntax = reg_syntax;
    preg_compile->array = new_array(0);
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
            pat_subreg->regcomp = subreg = new_reg_compile();
            subreg->match_here = 1;
            subreg->ref_num = -1;
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
    while (token_is_vbar(subreg->p)) {          //'|'
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
            subreg->ref_num = -1;
            push_array(pat->or_array, subreg);
            //親を作り、以後この下にSUBREGを追加する
            preg_compile = new_reg_compile();
            preg_compile->regexp = preg_compile->p = regexp;
            preg_compile->rlen = g_regexp_end - regexp;
            preg_compile->mode = mode;
            preg_compile->backref_flags = subreg->backref_flags;
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
        subreg->ref_num = -1;
        push_array(pat->or_array, subreg);
        preg_compile->p = subreg->p;
        preg_compile->rlen = preg_compile->p - regexp;
        preg_compile->backref_flags |= subreg->backref_flags;
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
//戻り値：エラー時はNULLを返し、reg_err_code/reg_err_msgにエラー情報を設定する。
static reg_compile_t *sequence_exp(const char *regexp, pattern_type_t mode) {
    reg_compile_t *preg_compile = new_reg_compile();
    preg_compile->regexp = preg_compile->p = regexp;
    preg_compile->rlen = g_regexp_end - regexp;
    preg_compile->mode = mode;
    reg_stat_t ret = REG_OK;

    while (ret==REG_OK) {
        ret = repeat_exp(preg_compile);
    }
    preg_compile->rlen = preg_compile->p - regexp;

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
//              | repeat_exp ( "*?" | "+?" | "??" | "{" repeat_range "}?" )?        PCRE
//              | repeat_exp ( "*+" | "++" | "?+" | "{" repeat_range "}+" )?        PCRE
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
            if (syntax_is(RE_ERROR_DUP_REPEAT)) {
                switch (preg_compile->prev_pat->type) {
                case PAT_CARET:
                case PAT_REPEAT:
                    reg_set_err(REG_ERR_CODE_INVALID_PRECEDING_REGEXP);
                    return REG_ERR;
                default:
                    break;
                }
            }
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
            if (syntax_is(RE_PCRE2) && !token_is_end(preg_compile->p)) {
                if (token1_is(preg_compile->p, '?')) {          //"*?", "+?", "??"
                    pat->rep_mode = REP_LAZY;
                    preg_compile->p++;
                } else if (token1_is(preg_compile->p, '+')) {   //"*+", "++", "?+"
                    pat->rep_mode = REP_POSEESSIVE;
                    preg_compile->p++;
                }
            }
            if (syntax_is(RE_CONTEXT_INDEP_OPS)) continue;
        } else if (token_is_open_brace(preg_compile->p)) {  //'{' or '\{'
            if (syntax_is(RE_ERROR_DUP_REPEAT)) {
                switch (preg_compile->prev_pat->type) {
                case PAT_CARET:
                case PAT_REPEAT:
                    reg_set_err(REG_ERR_CODE_INVALID_PRECEDING_REGEXP);
                    return REG_ERR;
                default:
                    break;
                }
            }
            if (set_repeat(preg_compile)==REG_ERR) return REG_ERR;
            if (syntax_is(RE_CONTEXT_INDEP_OPS)) continue;
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
//              | "(?i)" | "(?m)" | (?s) | "(?U)" | "(?x)" | "(?xx)"            PCRE
//              | "(?-...)" | "(?^)"                                            PCRE
//              | "\" ( "1" | ... | "9" )                                   BRE
//              | "^"  | "$"                                                    ERE
//              | "\A" | "\z" | "\Z" | "\K"                                     PCRE
//              | "\w" | "\W" | "\s" | "\S"| "\b" | "\B"                    GNU/PCRE
//              | "\d" | "^D" | "\h" | "^H" | "\v" | "\V" | "\R" | "\N"         PCRE
//              | "\<" | "\>" | "\`" | "\'"                                 GNU
//              | "\a" | "\e" | "\f" | "\n" | "\r" | "\t"                       PCRE
//              | "\cx" | "\0dd" | "\ddd" | "\o{ddd..}"                         PCRE
//              | "\N{U+hh.}" |"\xhh" | "\x{hh..}"                              PCRE
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
                    token_is_plus(preg_compile->p) ||               //'+'
                    token_is_question(preg_compile->p))             //'?'
                && (syntax_is(RE_CONTEXT_INVALID_OPS) ||
                    (preg_compile->prev_pat && preg_compile->prev_pat->type!=PAT_CARET))) {
            reg_set_err(REG_ERR_CODE_INVALID_PRECEDING_REGEXP);
        } else if (token_is_open_brace(preg_compile->p)) {          //'{'
            if (syntax_is(RE_ALLOW_ILLEGAL_REPEAT) && strchr(preg_compile->p, '}')==NULL) goto L_CHAR;
            reg_set_err(REG_ERR_CODE_INVALID_PRECEDING_REGEXP);
        } else if (token_is_open_paren(preg_compile->p)) {          //'('
            return set_subreg(preg_compile);
        } else if (token_is_close_paren(preg_compile->p)) {         //')'
            if (preg_compile->mode==PAT_SUBREG) return REG_END;
            if (syntax_is(RE_UNMATCHED_RIGHT_PAREN_ORD)) goto L_CHAR;  //EREでは単独の')'はリテラル
            reg_set_err(REG_ERR_CODE_UNMATCHED_PAREN);
        } else if (token_is_vbar(preg_compile->p)) {
            return REG_END;
        } else if (token1_is(preg_compile->p, '\\')) {
            preg_compile->p++;
            if (syntax_is(RE_PCRE2) && (token1_in(preg_compile->p, "acefnrtox01234567") ||
                                          token2_is(preg_compile->p, "N{"))) {  //"N{U+hh..}"を"\N"に優先させる
                return set_escape_char(preg_compile);
            } else if ((!syntax_is(RE_NO_GNU_OPS)  && token1_in(preg_compile->p, "wWsS")) ||      //"\s","\w",..
                       ( syntax_is(RE_PCRE2)       && token1_in(preg_compile->p, "wWsSdDhHvVRN"))) {//"\d","\N",..
                return set_char_set_ext(preg_compile);
            } else if (*preg_compile->p>='1' && *preg_compile->p<='9') {
                return set_backref(preg_compile);
            } else if (!syntax_is(RE_NO_GNU_OPS) && token1_is(preg_compile->p, '<')) {
                pat = new_pattern(PAT_WORD_FIRST);
            } else if (!syntax_is(RE_NO_GNU_OPS) && token1_is(preg_compile->p, '>')) {
                pat = new_pattern(PAT_WORD_LAST);
            } else if ((!syntax_is(RE_NO_GNU_OPS) || syntax_is(RE_PCRE2))
                                                   && token1_is(preg_compile->p, 'b')) {
                pat = new_pattern(PAT_WORD_DELIM);
            } else if ((!syntax_is(RE_NO_GNU_OPS) || syntax_is(RE_PCRE2))
                                                   && token1_is(preg_compile->p, 'B')) {
                pat = new_pattern(PAT_NOT_WORD_DELIM);
            } else if((!syntax_is(RE_NO_GNU_OPS) && token1_is(preg_compile->p, '`')) ||
                      (syntax_is(RE_PCRE2)       && token1_is(preg_compile->p, 'A'))) {
                pat = new_pattern(PAT_TEXT_FIRST);
            } else if((!syntax_is(RE_NO_GNU_OPS) && token1_is(preg_compile->p, '\'')) ||
                      (syntax_is(RE_PCRE2)       && token1_is(preg_compile->p, 'z'))) {
                pat = new_pattern(PAT_TEXT_LAST);
            } else if((syntax_is(RE_PCRE2)       && token1_is(preg_compile->p, 'Z'))) {
                pat = new_pattern(PAT_TEXT_LAST_NL);
            } else if((syntax_is(RE_PCRE2)       && token1_is(preg_compile->p, 'K'))) {
                pat = new_pattern(PAT_SET_START);
            } else if (syntax_is(RE_ERROR_UNKNOWN_ESCAPE) && isalnum(*preg_compile->p)) {
                reg_set_err(REG_ERR_CODE_UNKNOWN_ESCAPE);
                return REG_ERR;
            } else {
                pat = new_pattern(PAT_CHAR);
                pat->c = *preg_compile->p;
            }
        } else if (token1_is(preg_compile->p, '[')) {
            return set_char_set(preg_compile);
        } else if (token1_is(preg_compile->p, '$') &&
                   (syntax_is(RE_CONTEXT_INDEP_ANCHORS) ||
                    token_is_end(preg_compile->p+1) ||
                    (token_is_close_paren(preg_compile->p+1) && preg_compile->mode==PAT_SUBREG))) {
            pat = new_pattern(PAT_DOLLAR);
            //cont_flag = 1;
        } else if (token1_is(preg_compile->p, '^') &&
                   (syntax_is(RE_CONTEXT_INDEP_ANCHORS) || preg_compile->regexp==preg_compile->p)) {
            pat = new_pattern(PAT_CARET);
            cont_flag = 1;
        } else if (syntax_is((RE_COMMENT|RE_COMMENT_EXT)) && token1_in(preg_compile->p, " \t\n\r\f\v#")) {
            if (*preg_compile->p=='#') {
                while (!token_is_end(preg_compile->p) && !token1_is(preg_compile->p, '\n')) preg_compile->p++;
            } else preg_compile->p++;
            return REG_OK;
        } else {
            L_CHAR:;
#ifdef REG_ENABLE_UTF8
            int mbc_len = mblen(preg_compile->p, g_regexp_end-preg_compile->p);
            if (mbc_len>1) {
                pat = new_pattern(PAT_WC);
                mbtowc(&pat->wc, preg_compile->p, mbc_len);
                preg_compile->p += mbc_len-1;
            } else
#endif //REG_ENABLE_UTF8
            {
                pat = new_pattern(PAT_CHAR);
                char c = preg_compile->p[0];
                pat->c = syntax_is(RE_ICASE)?toupper(c):c;
            }
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

// set_backref: \nを後方参照として登録する。
static reg_stat_t set_backref(reg_compile_t *preg_compile) {
    assert(token_is_digit(preg_compile->p));
    int num = *preg_compile->p - '0';
    if (num>MAX_PAREN || (preg_compile->backref_flags&(1 << num))==0) {
        reg_set_err(REG_ERR_CODE_INVALID_BACK_REF);
        return REG_ERR;
    }
    pattern_t *pat = new_pattern(PAT_BACKREF);
    pat->ref_num = num;
    push_pattern(preg_compile, pat);
    preg_compile->p++;
    return REG_OK;
}

// set_subreg: \(\)の中身を抽出する
static reg_stat_t set_subreg(reg_compile_t *preg_compile) {
    const char *regexp = preg_compile->p;
    if (*regexp=='\\') regexp++;
    regexp ++;  //'('をスキップ
    error_if_token_end(regexp, REG_ERR_CODE_UNMATCHED_PAREN);
    int ref_num = 0;
    int assertion = 0;
    reg_syntax_t set_syntax = 0;
    reg_syntax_t unset_syntax = 0;

    if (syntax_is(RE_PCRE2) && token1_is(regexp, '?')) {
        regexp++;
        error_if_token_end(regexp, REG_ERR_CODE_UNMATCHED_PAREN);
        switch (*regexp) {
        case ':':           //"(?:)"グループ番号にカウントしない
            ref_num = -1;
            break;
        case '#':           //"(?#)"コメント
            while (!token_is_end(regexp) && !token1_is(regexp, ')')) regexp++;
            error_if_token_end(regexp, REG_ERR_CODE_UNMATCHED_PAREN);
            preg_compile->p = regexp+1;
            return REG_OK;
        case '=':           //"(?=)"positive lookahead
            assertion = POSITIVE_LOOKAHEAD;
            ref_num = -1;
            break;
        case '!':           //"(?!)"negative lookahead
            assertion = NEGATIVE_LOOKAHEAD;
            ref_num = -1;
            break;
        case '<':
            regexp++;
            error_if_token_end(regexp, REG_ERR_CODE_SUBPATTERN_NAME_EXPECTED);
            if      (token1_is(regexp, '=')) assertion = POSITIVE_LOOKBEHIND;
            else if (token1_is(regexp, '!')) assertion = NEGATIVE_LOOKBEHIND;
            else {
                reg_set_err(REG_ERR_CODE_SYNTAX_ERR_IN_SUBPTN_NAME);
                return REG_ERR;
            }
            ref_num = -1;
            break;
        case 'i':           //"(?i)"
        case 'm':           //"(?m)"
        case 's':           //"(?s)"
        case 'n':           //"(?n)"
        case 'x':           //"(?x)", "(?xx)"
        case 'U':           //"(?U)"
        case '^':           //"(?^)"
        case '-':           //"(?-...)"
            preg_compile->p = regexp;
            switch (set_options(preg_compile, &set_syntax, &unset_syntax)) {
            case REG_ERR:
                return REG_ERR;
            case REG_END:
            {
                pattern_t *pat = new_pattern(PAT_SET_OPTION);
                pat->set_syntax = set_syntax;
                if (unset_syntax&(RE_COMMENT|RE_COMMENT_EXT)) unset_syntax |= RE_COMMENT|RE_COMMENT_EXT;    //Unsetting x or xx unsets both.
                pat->unset_syntax = unset_syntax;

                reg_syntax |= set_syntax;
                reg_syntax &= ~unset_syntax;
                push_pattern(preg_compile, pat);
                return REG_OK;
            }
            case REG_OK:    //"(?i:...)"
                break;
            }
            ref_num = -1;
            regexp = preg_compile->p;
            break;
        default:
            reg_set_err(REG_ERR_CODE_UNRECOGNIZED_CHAR_PAREN);
            return REG_ERR;
        }
        regexp++;
    }
    if (ref_num==0) {   //ここで参照番号を確定させる
        g_nparen++;
        ref_num = g_nparen;
    }

    reg_syntax_t org_syntax = reg_syntax;       //reg_syntax保存
    pattern_t *pat = new_pattern(PAT_SUBREG);
    reg_syntax |= set_syntax;
    reg_syntax &= ~unset_syntax;
    pat->regcomp = reg_exp(regexp, PAT_SUBREG);
    reg_syntax = org_syntax;                    //reg_syntax復元
    if (pat->regcomp==NULL) {
        reg_pattern_free(pat);
        return REG_ERR;
    }
    pat->regcomp->set_syntax   = set_syntax;
    pat->regcomp->unset_syntax = unset_syntax;
    pat->regcomp->syntax |= set_syntax;
    pat->regcomp->syntax &= ~unset_syntax;
    pat->regcomp->match_here = 1;
    push_pattern(preg_compile, pat);
    preg_compile->p = pat->regcomp->p;
    pat->regcomp->ref_num = ref_num;
    pat->regcomp->assertion = assertion;
    if (assertion==POSITIVE_LOOKBEHIND || assertion==NEGATIVE_LOOKBEHIND) {
        pat->regcomp->total_len = get_length(pat->regcomp);
        if (pat->regcomp->total_len<0) {
            reg_set_err(REG_ERR_CODE_LOOKBEHIND_NOT_FIXLEN);
            return REG_ERR;
        }
    }
    preg_compile->backref_flags |= pat->regcomp->backref_flags;
    if (ref_num>0) preg_compile->backref_flags |= (1 << ref_num);
    return REG_OK;
}

static int get_length(reg_compile_t *preg_compile) {
    int match_len = 0;
    int len;
    int size = num_array(preg_compile->array);
    for (int i=0; i<size; i++) {
        pattern_t *pat = peek_array(preg_compile->array, i);
        switch (pat->type) {
        case PAT_CHAR:
        case PAT_CHARSET:
        case PAT_DOT:
            match_len += 1; break;
        case PAT_WC:
            len = wcrtomb(NULL, pat->wc, NULL);
            if (len>0) match_len += len;
            else return -1;
            break;
        case PAT_SUBREG:
            len = get_length(pat->regcomp);
        case PAT_REPEAT:
            return -1;
        default:
            break;
        }
    }
    return match_len;
}

//(?i)などのオプションを設定する。
static reg_stat_t set_options(reg_compile_t *preg_compile, reg_syntax_t* set_syntax, reg_syntax_t* unset_syntax) {
    int set_mode = 1;
    int reset_flag = 0;
    while (1) {
        error_if_token_end(preg_compile->p, REG_ERR_CODE_UNMATCHED_PAREN);
        int c = *preg_compile->p++;
        reg_syntax_t syntax = 0;
        switch (c) {
        case 'i':           //"(?i)"
            syntax = RE_ICASE; break;
        case 'm':           //"(?m)"
            syntax = RE_MULTILINE; break;
        case 's':           //"(?s)"
            syntax = RE_DOT_NEWLINE; break;
//      case 'n':           //"(?n)"
//          break;
        case 'x':           //"(?x)", "(?xx)"
            if (!token_is_end(preg_compile->p) && token1_is(preg_compile->p, 'x')) {
                syntax = RE_COMMENT_EXT;
                preg_compile->p++;
            } else syntax = RE_COMMENT;
            break;
//      case 'U':           //"(?U)"
//          break;
        case '^':           //"(?^)"
            if (*set_syntax || *unset_syntax) {
                reg_set_err(REG_ERR_CODE_UNRECOGNIZED_CHAR_PAREN);
                return REG_ERR;
            }
            reset_flag = 1;
            *unset_syntax = RE_ICASE|RE_MULTILINE|RE_DOT_NEWLINE|RE_COMMENT|RE_COMMENT_EXT;
            continue;
        case '-':           //"(?-...)"
            if (set_mode==0 || reset_flag) {
                reg_set_err(REG_ERR_CODE_INVALID_HYPHEM_IN_OPTION);
                return REG_ERR;
            }
            set_mode = 0;
            continue;
        case ':':           //"(?i:...)"
            preg_compile->p--;
            return REG_OK;
        case ')':
            return REG_END;
        default:
            reg_set_err(REG_ERR_CODE_UNRECOGNIZED_CHAR_PAREN);
            return REG_ERR;
        }
        if (set_mode) {
            *set_syntax   |= syntax;
            *unset_syntax &= ~syntax;
        } else *unset_syntax |= syntax;
    }
    return REG_OK;
}

// set_repeat: 繰り返し条件を生成する
static reg_stat_t set_repeat(reg_compile_t *preg_compile) {
    const char *regexp = preg_compile->p;
    assert(token_is_open_brace(regexp));
    int mode = 1;   //第nパラメータ取得モード
    int min=-1, max=-1;

    if (*regexp=='\\') regexp++;    //'\\'の分
    regexp ++;                      //'{'の分
    for (; !token_is_end(regexp) && mode<3; regexp++) {
        switch (mode) {
        case 1:             //第1パラメータの取得
            if        (token1_is(regexp, ',')) {
                mode = 2;
                break;
            } else if (token_is_close_brace(regexp)) {  //'}'
                mode = 3;
                if (min<0) min = 0;
                max = min;
                if (*regexp=='\\') regexp++;
            } else if (token_is_digit(regexp)) {
                if (min<0) min = 0;
                min = min*10 + *regexp - '0';
            } else {
                mode = 10;  //Error
            }
            break;
        case 2:             //第2パラメータの取得
            if        (token_is_close_brace(regexp)) {  //'}'
                mode = 3;   //完了
                if (*regexp=='\\') regexp++;
            } else if (token_is_digit(regexp)) {
                if (max<0) max = 0;
                max = max*10 + *regexp - '0';
            } else {
                mode = 10;  //Error
            }
            break;
        }
    }
    if ((mode<3 || mode==10 || min<0) && syntax_is(RE_ALLOW_ILLEGAL_REPEAT)) {
        //不正な構文の場合、全体をリテラルとする。
        for (; *preg_compile->p; preg_compile->p++) {
            push_char(preg_compile, *preg_compile->p);
        }
        return REG_OK;
    }
    if (min<0) min = 0;
    if (max<0) max = RE_DUP_MAX;
    if (mode<3) {           //閉じカッコ無し
        reg_set_err(REG_ERR_CODE_UNMATCHED_BRACE);
        return REG_ERR;
    } else if (mode==10 || min>max) {
        reg_set_err(REG_ERR_CODE_INVALID_CONTENT_OF_BRACE);
        return REG_ERR;
    } else if (max>RE_DUP_MAX) {
        reg_set_err(REG_ERR_CODE_REGEXP_TOO_BIG);
        return REG_ERR;
    }
    pattern_t *pat = new_pattern(PAT_REPEAT);
    pat->min = min;
    pat->max = max;
    if (syntax_is(RE_PCRE2) && !token_is_end(regexp)) {
        if (token1_is(regexp, '?')) {          //"{n,m}?"
            pat->rep_mode = REP_LAZY;
            regexp++;
        } else if (token1_is(regexp, '+')) {   //"{n,m}+"
            pat->rep_mode = REP_POSEESSIVE;
            regexp++;
        }
    }
    push_pattern(preg_compile, pat);
    preg_compile->p = regexp;
    return REG_OK;
}

//文字（1バイト文字またはワイド文字）を登録する。
static void push_char(reg_compile_t *preg_compile, int c) {
    pattern_t *pat;
    if (c<=0xff) {
        pat = new_pattern(PAT_CHAR);
        pat ->c = syntax_is(RE_ICASE)?toupper(c):c;
    } else {
#ifdef REG_ENABLE_UTF8
        pat = new_pattern(PAT_WC);
        pat->wc = (wchar_t)c;
#else
        assert(0);
#endif
    }
    push_pattern(preg_compile, pat);
}

#ifdef REG_ENABLE_UTF8
#define WCSTR_INIT_SIZE 4
//push_char_set_wc: 文字セットにワイド文字を1個追加する。
static void push_char_set_wc(char_set_t *char_set, wchar_t wc) {
    if (char_set->wcstr==NULL) {
        char_set->wcstr = calloc(WCSTR_INIT_SIZE, sizeof(wchar_t));
        char_set->wcs_len = 0;
        char_set->wcs_size = WCSTR_INIT_SIZE;
    }
    if (char_set->wcs_len+1>=char_set->wcs_size) {
        char_set->wcs_size *= 2;
        char_set->wcstr = realloc(char_set->wcstr, char_set->wcs_size*sizeof(wchar_t));
    }
    char_set->wcstr[char_set->wcs_len++] = wc;
    char_set->wcstr[char_set->wcs_len]   = L'\0';
}

//push_char_set_mbc: 文字セットにUTF-8の1文字を追加し、消費したUTF-8のバイト数（1以上）を返す。
static int push_char_set_mbc(char_set_t *char_set, const char *mbc, int max_len) {
    wchar_t wc;
    int mbc_len = mbtowc(&wc, mbc, max_len);
    assert(mbc_len>=0);
    if (mbc_len==0) mbc_len = 1;
    if (mbc_len==1) push_char_set(char_set, *mbc);
    else            push_char_set_wc(char_set, wc);
    return mbc_len;
}

static void push_char_set_range(char_set_t *char_set, const char *start, int slen, const char *end, int elen) {
    char_range_t *range = calloc(1, sizeof(char_range_t));
    push_array(char_set->ranges, range);
    int ret;
    ret = mbtowc(&range->wc_start, start, slen);
    if (ret<0) range->wc_start = L'■';
    ret = mbtowc(&range->wc_end  , end,   elen);
    if (ret<0) range->wc_end = L'■';
}
#define push_char_set_class(char_set, type) ((char_set)->char_class|=(type))
#else
#define push_char_set_mbc(a,b,c)
#define push_char_set_class(a, b)
#endif //REG_ENABLE_UTF8

//push_char_set: 文字セットに1バイト文字を追加する。
static void push_char_set(char_set_t *char_set, unsigned char c) {
    char_set->chars[c] = !char_set->reverse;
    unsigned uc = toupper(c);
    if (syntax_is(RE_ICASE) && c!=uc)
        char_set->chars[uc] = !char_set->reverse;
}
//push_char_set_str: 文字セットに1バイト文字の文字列を追加する。
static void push_char_set_str(char_set_t *char_set, const char *str) {
    for (; *str; str++) push_char_set(char_set, *str);
}

// set_char_set: 文字セットを生成する。入力："[...]"
static reg_stat_t set_char_set(reg_compile_t *preg_compile) {
    pattern_t *pat = new_pattern(PAT_CHARSET);
    const char *regexp = preg_compile->p;
    assert(token1_is(regexp, '['));
    char_set_t *char_set;
    pat->cset = char_set = calloc(1, sizeof(char_set_t));
#ifdef REG_ENABLE_UTF8
    char_set->ranges = new_array(0);
#endif //REG_ENABLE_UTF8
    const char* range_start = NULL; //範囲指定の開始文字の位置
    int range_start_len = 0;        //範囲指定の開始文字の長さ。0でない場合は次の文字で範囲指定の'-'が利用できる
    int bracket_is_closed = 0;  //']'が出現済み

    regexp++;
    error_if_token_end(regexp, REG_ERR_CODE_UNMATCHED_BRACKET);
    if (token1_is(regexp, '^')) {
        if (token1_is(regexp+1, ']')) {
            reg_pattern_free(pat);
            reg_set_err(REG_ERR_CODE_UNMATCHED_BRACKET);
            return REG_ERR;
        }
        char_set->reverse = 1;
        memset(char_set->chars, 1, 256);
        regexp++;
        error_if_token_end(regexp, REG_ERR_CODE_UNMATCHED_BRACKET);
    }
    if (token1_is(regexp, ']') || token1_is(regexp, '-')) {
        push_char_set(char_set, *regexp);
        range_start = regexp;
        range_start_len = 1;
        regexp++;
        error_if_token_end(regexp, REG_ERR_CODE_UNMATCHED_BRACKET);
    }

    for (; !token_is_end(regexp); regexp++) {
        if (token1_is(regexp, '-') && !token_is_end(regexp+1) && !token1_is(regexp+1,']')) {
            if (syntax_is(RE_ALLOW_UNBALANCED_MINUS_IN_LIST) && range_start_len==0) goto L_CHAR;
            if (range_start_len==0 || regexp[-1]>regexp[1]) {
                reg_pattern_free(pat);
                reg_set_err(REG_ERR_CODE_INVALID_RANGE_END);
                return REG_ERR;
            }
#ifdef REG_ENABLE_UTF8
            int range_end_len = reg_mblen(regexp+1, g_regexp_end-regexp-1);
            if (range_start_len==1 && range_end_len==1) {   //1バイト文字の範囲指定
                for (int c=regexp[-1]+1; c<=regexp[1]; c++) push_char_set(char_set, c);
            } else {
                push_char_set_range(char_set, range_start, range_start_len, regexp+1, range_end_len);
            }
            regexp += range_end_len;
#else
            for (int c=regexp[-1]+1; c<=regexp[1]; c++) push_char_set(char_set, c);
            regexp++;
#endif //REG_ENABLE_UTF8
            range_start_len = 0;
        } else if (token2_is(regexp, "[:")) {
            preg_compile->p = regexp;
            if (set_char_class(preg_compile, char_set)==REG_ERR) {
                reg_pattern_free(pat);
                return REG_ERR;
            }
            regexp = preg_compile->p-1;
            range_start_len = 0;
        } else if (token1_is(regexp, ']')) {
            regexp++;
            bracket_is_closed = 1;
            break;
        } else if (syntax_is(RE_PCRE2) && token1_is(regexp, '\\')) {
            range_start = regexp;
            preg_compile->p = regexp + 1;
            int c = get_escape_char(preg_compile, /*enable_backref=*/0);
            regexp = preg_compile->p;
            range_start_len = regexp - range_start;
            regexp--;
            if (c<0) return REG_ERR;
            if (c<=0xff) push_char_set(char_set, c);
#ifdef REG_ENABLE_UTF8
            else         push_char_set_wc(char_set, (wchar_t)c);
#endif
        } else if (syntax_is(RE_COMMENT_EXT) && token1_in(regexp, " \t")) {
            ;   //コメント
        } else {
            L_CHAR:;
#ifdef REG_ENABLE_UTF8
            int mbc_len = push_char_set_mbc(char_set, regexp, g_regexp_end-regexp);
            range_start = regexp;
            range_start_len = mbc_len;
            regexp += mbc_len-1;
#else
            push_char_set(char_set, *regexp);
            range_start_len = 1;
#endif //REG_ENABLE_UTF8
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

//"\w"等に対応した文字セットを登録する。
static reg_stat_t set_char_set_ext(reg_compile_t *preg_compile) {
    pattern_t *pat = new_pattern(PAT_CHARSET);
    char_set_t *char_set;
    pat->cset = char_set = calloc(1, sizeof(char_set_t));;
#ifdef REG_ENABLE_UTF8
    char_set->ranges = new_array(0);
#endif //REG_ENABLE_UTF8

    switch (*preg_compile->p++) {
    case 'w':           //[_[:alnum:]]
        push_char_set(char_set, '_');
        set_alnum(char_set);
        push_char_set_class(char_set, CHAR_CLASS_ALNUM);
        break;
    case 'W':           //[^_[:alnum:]]
        char_set->reverse = 1;
        memset(char_set->chars, 1, 256);
        push_char_set(char_set, '_');
        set_alnum(char_set);
        push_char_set_class(char_set, CHAR_CLASS_ALNUM);
        break;
    case 's':           //[[:space:]]
        push_char_set_str(char_set, " \t\n\r\f\v");
        push_char_set_class(char_set, CHAR_CLASS_SPACE);
        break;
    case 'S':           //[^[:space:]]
        char_set->reverse = 1;
        memset(char_set->chars, 1, 256);
        push_char_set_str(char_set, " \t\n\r\f\v");
        push_char_set_class(char_set, CHAR_CLASS_SPACE);
        break;
    case 'd':           //[[:digit:]]
        set_digit(char_set);
#ifdef REG_ENABLE_UTF8
        if (syntax_is(RE_PCRE2)) for (wchar_t wc=L'０'; wc<=L'９'; wc++) push_char_set_wc(char_set, wc);
#endif
        push_char_set_class(char_set, CHAR_CLASS_DIGIT);
        break;
    case 'D':           //[^[:digit:]]
        char_set->reverse = 1;
        memset(char_set->chars, 1, 256);
        set_digit(char_set);
#ifdef REG_ENABLE_UTF8
        if (syntax_is(RE_PCRE2)) for (wchar_t wc=L'０'; wc<=L'９'; wc++) push_char_set_wc(char_set, wc);
#endif
        push_char_set_class(char_set, CHAR_CLASS_DIGIT);
        break;
    case 'h':           //[ \t]     //PCRE
        push_char_set_str(char_set, " \t");
#ifdef REG_ENABLE_UTF8
        push_char_set_wc(char_set, L'　');  //#Unicodeには簡易対応
#endif
        break;
    case 'H':           //[^ \t]    //PCRE
        char_set->reverse = 1;
        memset(char_set->chars, 1, 256);
        push_char_set_str(char_set, " \t");
#ifdef REG_ENABLE_UTF8
        push_char_set_wc(char_set, L'　');  //#Unicodeには簡易対応
#endif
        break;
    case 'v':           //'\v'      //PCRE
        push_char(preg_compile, '\v');
        return REG_OK;
    case 'V':           //[^\v]     //PCRE
        char_set->reverse = 1;
        memset(char_set->chars, 1, 256);
        push_char_set(char_set, '\v');
        break;
    case 'R':           //[\n\r]    //PCRE
        push_char_set_str(char_set, "\n\r");
        break;
    case 'N':           //[^\n\r]   //PCRE
        char_set->reverse = 1;
        memset(char_set->chars, 1, 256);
        push_char_set_str(char_set, "\n");  //'\r'は含まない？
        break;
    default:
        reg_pattern_free(pat);
        return REG_ERR;
    }
    push_pattern(preg_compile, pat);
    return REG_OK;
}

static void set_alpha(char_set_t *char_set) {
    for (int c='A'; c<='Z'; c++) push_char_set(char_set, c);
    for (int c='a'; c<='z'; c++) push_char_set(char_set, c);
}
static void set_alnum(char_set_t *char_set) {
    set_alpha(char_set);
    set_digit(char_set);
}
static void set_digit(char_set_t *char_set) {
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
//              | "digit"       #[0-9]                                      POSIX
//              | "xdigit"      #[0-9A-Fa-f]                                POSIX
//              | "punct"       #[]!"#$%&'()*+,./:;<=>?@[\^_`{|}~-]         POSIX
//              | "blank"       #[ \t]                                      POSIX
//              | "space"       #[ \t\n\r\f\v]                              POSIX
//              | "cntrl"       #0x00-0x1f, 0x7f                            POSIX
//              | "graph"       #[^ \t\n\r\f\v[:cntrl:]]                    POSIX
//              | "print"       #[^\t\n\r\f\v[:cntrl:]]                     POSIX
//              | "ascii"       #[\x00-\x7f]                                PCRE
//              | "word"        #[_A-Za-z0-9]                               PCRE
static reg_stat_t set_char_class(reg_compile_t *preg_compile, char_set_t *char_set) {
    const char *regexp = preg_compile->p;
    assert(token2_is(regexp, "[:"));
    regexp += 2;
    if        (strncmp(regexp, "upper", 5)==0) {
        for (int c='A'; c<='Z'; c++) push_char_set(char_set, c);
        push_char_set_class(char_set, CHAR_CLASS_UPPER);
        regexp += 5;
    } else if (strncmp(regexp, "lower", 5)==0) {
        for (int c='a'; c<='z'; c++) push_char_set(char_set, c);
        push_char_set_class(char_set, CHAR_CLASS_LOWER);
        regexp += 5;
    } else if (strncmp(regexp, "alpha", 5)==0) {
        set_alpha(char_set);
        push_char_set_class(char_set, CHAR_CLASS_ALPHA);
        regexp += 5;
    } else if (strncmp(regexp, "alnum", 5)==0) {
        set_alnum(char_set);
        push_char_set_class(char_set, CHAR_CLASS_ALNUM);
        regexp += 5;
    } else if (strncmp(regexp, "digit", 5)==0) {
        set_digit(char_set);
        push_char_set_class(char_set, CHAR_CLASS_DIGIT);
        regexp += 5;
    } else if (strncmp(regexp, "xdigit", 6)==0) {
        for (int c='0'; c<='9'; c++) push_char_set(char_set, c);
        for (int c='A'; c<='F'; c++) push_char_set(char_set, c);
        if (!syntax_is(RE_ICASE))
            for (int c='a'; c<='f'; c++) push_char_set(char_set, c);
        push_char_set_class(char_set, CHAR_CLASS_XDIGIT);
        regexp += 6;
    } else if (strncmp(regexp, "punct", 5)==0) {
        set_punct(char_set);
        push_char_set_class(char_set, CHAR_CLASS_PUNCT);
        regexp += 5;
    } else if (strncmp(regexp, "blank", 5)==0) {
        push_char_set_str(char_set, " \t");
        push_char_set_class(char_set, CHAR_CLASS_BLANK);
        regexp += 5;
    } else if (strncmp(regexp, "space", 5)==0) {
        push_char_set_str(char_set, " \t\n\r\f\v");
        push_char_set_class(char_set, CHAR_CLASS_SPACE);
        regexp += 5;
    } else if (strncmp(regexp, "cntrl", 5)==0) {
        for (int c=0x00; c<=0x1f; c++) push_char_set(char_set, c);
        push_char_set_class(char_set, CHAR_CLASS_CNTRL);
        push_char_set(char_set, 0x7f);
        regexp += 5;
    } else if (strncmp(regexp, "graph", 5)==0) {            //alnum + punct
        set_alnum(char_set);
        set_punct(char_set);
        push_char_set_class(char_set, CHAR_CLASS_GRAPH);
        regexp += 5;
    } else if (strncmp(regexp, "print", 5)==0) {            //alnum + punct + ' '
        set_alnum(char_set);
        set_punct(char_set);
        push_char_set(char_set, ' ');
        push_char_set_class(char_set, CHAR_CLASS_PRINT);
        regexp += 5;
    } else if (syntax_is(RE_PCRE2) && strncmp(regexp, "word", 4)==0) {
        push_char_set(char_set, '_');
        set_alnum(char_set);
        push_char_set_class(char_set, CHAR_CLASS_ALNUM);
        regexp += 4;
    } else if (syntax_is(RE_PCRE2) && strncmp(regexp, "ascii", 5)==0) {
        for (int c=0x00; c<=0x7f; c++) push_char_set(char_set, c);
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

//エスケープ文字を登録する。
static reg_stat_t set_escape_char(reg_compile_t *preg_compile) {
    int c = get_escape_char(preg_compile, /*enable_backref=*/1);
    if (c==-1) return REG_ERR;
    if (c==-2) return REG_OK;   //後方参照
    push_char(preg_compile, c);
    return REG_OK;
}


//エスケープ文字を取得する。
//エラーの場合は-1を返す。後方参照だった場合は-2を返す。
#define MAX_OCTAL_LEN 10
#define MAX_HEX_LEN 8
#ifdef REG_ENABLE_UTF8
#define MAX_CODE_POINT  L'\U000e01ef'
#else
#define MAX_CODE_POINT  0xff
#endif
static int get_escape_char(reg_compile_t *preg_compile, int enable_backref) {
    int c = *preg_compile->p++;
    int len = 0;
    switch (c) {
    case 'a': c = '\a';   break;
    case 'e': c = '\x1b'; break;
    case 'f': c = '\f';   break;
    case 'n': c = '\n';   break;
    case 'r': c = '\r';   break;
    case 't': c = '\t';   break;
    case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': //"\0dd", "\ddd": octal
        c = c - '0'; len = 1;
        while (len<3 && !token_is_end(preg_compile->p) && token_is_octal(preg_compile->p)) {
            c = c*8 + *preg_compile->p++ - '0';
            len++;
        }
        if (enable_backref && len==1 && c>0) {  //"\1" - "\7"後方参照
            preg_compile->p--;
            if (set_backref(preg_compile)==REG_OK) return -2;
            else return -1;
        }
        break;
    case 'c':   //\cx   制御文字。小文字を大文字化してbit6を反転させる。
        if (token_is_end(preg_compile->p)) {
            reg_set_err(REG_ERR_CODE_CTRL_C_AT_END);
            return -1;
        }
        c = toupper(*preg_compile->p);
        if (c<' ' || c >'~') {
            reg_set_err(REG_ERR_CODE_NOT_PRINTABLE_ASCII);
            return -1;
        }
        c ^= 0x40;
        preg_compile->p++;
        break;
    case 'o':   //"\o{ddd...}
        if (token_is_end(preg_compile->p) || !token1_is(preg_compile->p, '{')) {
            reg_set_err(REG_ERR_CODE_MISSING_OPEN_BRACE_O);
            return -1;
        }
        c = len = 0;
        preg_compile->p++;  //'{'をスキップ
        while (len<MAX_OCTAL_LEN && !token_is_end(preg_compile->p)) {
            if (token_is_octal(preg_compile->p)) {
                c = c*8 + *preg_compile->p++ - '0';
                len++;
                if (c>MAX_CODE_POINT) {
                    reg_set_err(REG_ERR_CODE_CODE_POINT_TOO_LARGE);
                    return -1;
                }
            } else if (token1_is(preg_compile->p, '}')) {
                break;
            } else {
                reg_set_err(REG_ERR_CODE_NON_OCTAL_CHAR);
                return -1;
            }
        }
        if (len==0) {
            reg_set_err(REG_ERR_CODE_DIGITS_MISSING);
            return -1;
        } else if (token_is_end(preg_compile->p)) {
            reg_set_err(REG_ERR_CODE_NON_OCTAL_CHAR);
            return -1;
        }
        preg_compile->p++;  //'}'をスキップ
        break;
    case 'x':
        c = 0; len = 0;
        if (!token_is_end(preg_compile->p) && token1_is(preg_compile->p, '{')) {  //"\x{hh..}"
            preg_compile->p++;  //'{'をスキップ
#ifdef REG_ENABLE_UTF8
            L_HEX:
#endif
            while (len<MAX_HEX_LEN && !token_is_end(preg_compile->p)) {
                if (token_is_digit(preg_compile->p)) {
                    c = c*16 + *preg_compile->p++ - '0';
                    len++;
                    if (c>MAX_CODE_POINT) {
                        reg_set_err(REG_ERR_CODE_CODE_POINT_TOO_LARGE);
                        return -1;
                    }
                } else if (token_is_xdigit(preg_compile->p)) {
                    c = c*16 + tolower(*preg_compile->p) - 'a' + 10;
                    preg_compile->p++;
                    len++;
                } else if (token1_is(preg_compile->p, '}')) {
                    break;
                } else {
                    reg_set_err(REG_ERR_CODE_NON_HEX_CHAR);
                    return -1;
                }
            }
            if (len==0) {
                reg_set_err(REG_ERR_CODE_DIGITS_MISSING);
                return -1;
            } else if (token_is_end(preg_compile->p)) {
                reg_set_err(REG_ERR_CODE_NON_HEX_CHAR);
                return -1;
            }
            preg_compile->p++;  //'}'をスキップ
        } else {                                //"\xhh"
            while (len<2 && !token_is_end(preg_compile->p) && token_is_xdigit(preg_compile->p)) {
                if (token_is_digit(preg_compile->p)) c = c*16 + *preg_compile->p - '0';
                else                                 c = c*16 + tolower(*preg_compile->p) - 'a' + 10;
                len++; preg_compile->p++;
            }
        }
        break;
#ifdef REG_ENABLE_UTF8
    case 'N':   //"N{U+hh..}"
        preg_compile->p++;  //'{'をスキップ
        if (token_len(preg_compile->p)>=2 && token2_is(preg_compile->p, "U+")) {
            preg_compile->p += 2;
            c = 0; len = 0;
            goto L_HEX;
        }
        reg_set_err(REG_ERR_CODE_INVALID_UNICODE);
        return -1;
#endif
    default:
        c = -1;
        break;
    }

    if (c<0) {
        reg_set_err(REG_ERR_CODE_UNKNOWN_ESCAPE);
        return -1;
    }
    return c;
}

//--------------------------------------------------------------------------------

//textバッファの文字を検査するマクロ
#define text_is_start(p)    ((p)==g_text_start)             //pがtextの先頭にマッチする
#define text_is_end(p)      ((p)>=g_text_end)               //pがtextの最後にマッチする
#define text_is_word(p)     (isalnum(*(p))||*(p)=='_')      //*pが単語を構成する文字(alnum+'_')である
#define text_is_word_first(p)   (!text_is_end((p)) && \
                                 text_is_word((p)) && \
                                 (text_is_start((p)) || !text_is_word((p)-1)))
#define text_is_word_last(p)    (!text_is_start((p)) && \
                                 (text_is_end((p)) || !text_is_word((p))) && \
                                 text_is_word((p)-1))       //pが単語境界の先頭/最後

//検索時の作業用
static const char* g_text_org;  //オリジナル検索文字列（reg_exec関数のtext引数）
static const char *g_text_start;//検索文字列の先頭（^にマッチさせる位置）
static const char *g_text_end;  //検索文字列の末尾の次の位置（文字列の終了判定に用いる）（$にマッチさせる位置）
static const char *g_text_match_start;  //全体のマッチ開始位置（\K,lookbehindで変更される）。
static size_t      g_nmatch;    //pmatchの要素数              （reg_exec関数のnmatch引数）
static regmatch_t *g_pmatch;    //マッチング位置を格納する配列（reg_exec関数のpmatch引数）
static int         g_eflags;    //検索モード                  （reg_exec関数のeflags引数）

// reg_exec: search for compiled regexp anywhere in text
//成功した場合は0、失敗した場合は0以外を返す。
int reg_exec(reg_compile_t *preg_compile, const char *text, size_t len, size_t nmatch, regmatch_t *pmatch, int eflags) {
    if (preg_compile==NULL || text==NULL) return 1;
    if (len<0) len = strlen(text);  //lenが負の値の場合はtextをnul終端の文字列とみなす
    g_text_org = text;
    g_text_match_start = NULL;
    if ((eflags&REG_STARTEND) && pmatch) {
        g_text_start = text + pmatch[0].rm_so;
        g_text_end   = text + pmatch[0].rm_eo;
    } else {
        g_text_start = text;
        g_text_end   = text + len;
    }
    reg_syntax = preg_compile->syntax;
    preg_compile->eflags = g_eflags = eflags;
    if (pmatch) {
        pmatch[0].rm_so = 0;
        pmatch[0].rm_eo = 0;
        for (int i=1; i<nmatch; i++) {
            pmatch[i].rm_so = -1;
            pmatch[i].rm_eo = -1;
        }
        g_nmatch = nmatch;
        g_pmatch = pmatch;
    } else {
        g_nmatch = 0;
        g_pmatch = NULL;
    }

    int match_len;
    int ret = reg_exec_main(preg_compile, g_text_start, &match_len);
    if (ret==0 && pmatch && g_text_match_start) { //\K, lookbehindを全体のマッチング範囲に反映する
        pmatch[0].rm_so = g_text_match_start - g_text_org;
    }
    return ret;
}
// reg_exec_main: search for compiled regexp anywhere in text
//成功した場合は0、失敗した場合は0以外を返す。
static int reg_exec_main(reg_compile_t *preg_compile, const char *text, int *match_len) {
    assert(preg_compile);
    assert(preg_compile->array);
    const char *rm_ep;
    pattern_t **pat = (pattern_t**)preg_compile->array->buckets;
    *match_len = 0;

    do {    /* must look even if string is empty */
        //reg_syntax_t org_syntax = reg_syntax;   //reg_syntax保存
        reg_syntax = preg_compile->syntax;
        if (preg_compile->set_syntax)   reg_syntax |= preg_compile->set_syntax;
        if (preg_compile->unset_syntax) reg_syntax |= preg_compile->unset_syntax;
        int ret = reg_match_here(preg_compile, pat, text, &rm_ep);
        //reg_syntax = org_syntax;                //reg_syntax復元
        if (ret) {
            set_match(preg_compile, text, rm_ep);
            *match_len = rm_ep - text;
            reg_syntax = preg_compile->syntax;
            return 0;   //成功
        }
        if (preg_compile->match_here) break;
    }
#ifdef REG_ENABLE_UTF8
    while (!text_is_end(text) && (text+=reg_mblen(text, g_text_end-text)));
#else
    while (!text_is_end(text++));
#endif //REG_ENABLE_UTF8

    if (g_pmatch && g_pmatch[preg_compile->ref_num].rm_so<0) {
        g_pmatch[preg_compile->ref_num].rm_so = 0;
        g_pmatch[preg_compile->ref_num].rm_eo = 0;
    }
    return 1;   //失敗
}

//マッチング位置のインデックスを記録し、マッチした文字列の長さを返す。
//マッチング位置の基準はg_text_startではなくg_text_org
static int set_match(reg_compile_t *preg_compile, const char *rm_sp, const char *rm_ep) {
    int so = rm_sp - g_text_org;
    int eo = rm_ep - g_text_org;
    int match_len = eo - so;
    int idx = preg_compile->ref_num;
    if (g_pmatch && idx>=0) {   //idx(ref_num)が-1の場合は記録しない
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

// reg_match_repeat: search for c\{m,n\} regexp at beginning of text
static int reg_match_repeat(reg_compile_t *preg_compile, pattern_t **pat, const char *text, const char **rm_ep) {
    pattern_t *rep = pat[0];
    pattern_t *c   = pat[1];
    assert(rep->type==PAT_REPEAT);
    int cnt = 0;    //繰り返しマッチ回数
    int ret = 0;
    int match_len;

    if (rep->rep_mode==REP_POSEESSIVE) {    //バックトラックしない
        //繰り返しを最大限マッチさせてから後続のマッチ処理を行う。
        while (cnt<rep->max && reg_match_pat(preg_compile, c, text, &match_len)) {
            text += match_len;  //マッチした文字だけtextを進めて次の処理を行う
            cnt++;
        }
        if (cnt>=rep->min && reg_match_here(preg_compile, pat+2, text, rm_ep)) ret = 1;
    } else {                                //バックトラックする
        //0回の繰り返しマッチ&&後続マッチ->1回の繰り返し&&後続マッチ->...を続けていって、
        //REP_GREEDY: 最後にマッチしたものを採用（最長マッチ）
        //REP_LAZY:   最後にマッチしたものを採用（最短マッチ）
        do {    /* a \{0,n\} matches zero or more instances */
            if (cnt++>=rep->min && reg_match_here(preg_compile, pat+2, text, rm_ep)) {
                ret = 1;
                if (rep->rep_mode==REP_LAZY) break; //最短一致
            }
        } while (reg_match_pat(preg_compile, c, text, &match_len) &&
                (text+=match_len) &&   //マッチした文字だけtextを進めて次の処理を行う
                cnt<=rep->max);
    }

    return ret;
}

// reg_match_pat: 1個のパターンがマッチするかを判定
// match_lenにはpatでマッチしたtextの長さを設定して返す。
static int reg_match_pat(reg_compile_t *preg_compile, pattern_t *pat, const char *text, int *match_len) {
#ifdef REG_ENABLE_UTF8
    int mbc_len;
#endif

    switch (pat->type) {
    case PAT_CHAR:
        if (text_is_end(text)) return 0;
        *match_len = 1;
        return pat->c == (syntax_is(RE_ICASE)?toupper(*text):*text);
#ifdef REG_ENABLE_UTF8
    case PAT_WC:
        if (text_is_end(text)) return 0;
        mbc_len = reg_mblen(text, g_text_end-text);
        if (mbc_len>1) {
            wchar_t wc, wc2;
            int ret = mbtowc(&wc, text, mbc_len);
            if (ret>=2) {
                wc2 = pat->wc;
                if (syntax_is(RE_ICASE)) {
                    wc  = towupper(wc);
                    wc2 = towupper(wc2);
                }
                if (wc==wc2) {
                    *match_len = mbc_len;
                    return 1;
                }
            }
        }
        return 0;
#endif //REG_ENABLE_UTF8
    case PAT_CHARSET:
        if (text_is_end(text)) return 0;
        char_set_t *cset = pat->cset;
        if (*text == '\n' && cset->reverse && syntax_is(RE_HAT_LISTS_NOT_NEWLINE)) return 0;
#ifdef REG_ENABLE_UTF8
        mbc_len = reg_mblen(text, g_text_end-text);
        if (mbc_len<=1) {   //1バイト文字。UTF-8として不正な文字と'\0'は1バイト文字として処理する。
            if (cset->chars[syntax_is(RE_ICASE)?toupper(*text):*text]) {
                *match_len = 1;
                return 1;
            };
        } else {
            wchar_t wc;
            int ret = mbtowc(&wc, text, mbc_len);  //mblen()>=2の場合なのでエラーは起こらない
            assert(ret>0);
            if (syntax_is(RE_ICASE)) wc = towupper(wc);
            int match_flag = 0;
            if (cset->wcstr) {
                for (int i=0; i<cset->wcs_len; i++) {
                    wchar_t wc2 = pat->cset->wcstr[i];
                    if (syntax_is(RE_ICASE)) wc2 = towupper(wc2);
                    if (wc2==wc) {
                        match_flag = 1;
                        if (cset->reverse) return 0;
                        *match_len = mbc_len;
                        return 1;
                    }
                }
            }
            if (cset->char_class) {
                for (int i=1; i<=CHAR_CLASS_PUNCT; i*=2) {
                    if (!(cset->char_class&i)) continue;
                    switch (i) {
                    case CHAR_CLASS_ALPHA:  match_flag = iswalpha (wc); break;
                    case CHAR_CLASS_ALNUM:  match_flag = iswalnum (wc); break;
                    case CHAR_CLASS_DIGIT:  match_flag = iswdigit (wc); break;
                    case CHAR_CLASS_XDIGIT: match_flag = iswxdigit(wc); break;
                    case CHAR_CLASS_LOWER:  match_flag = syntax_is(RE_ICASE)?
                                                         iswalpha (wc):
                                                         iswlower (wc); break;
                    case CHAR_CLASS_UPPER:  match_flag = syntax_is(RE_ICASE)?
                                                         iswalpha (wc):
                                                         iswupper (wc); break;
                    case CHAR_CLASS_BLANK:  match_flag = iswblank (wc); break;
                    case CHAR_CLASS_SPACE:  match_flag = iswspace (wc); break;
                    case CHAR_CLASS_PRINT:  match_flag = iswprint (wc); break;
                    case CHAR_CLASS_GRAPH:  match_flag = iswgraph (wc); break;
                    case CHAR_CLASS_CNTRL:  match_flag = iswcntrl (wc); break;
                    case CHAR_CLASS_PUNCT:  match_flag = iswpunct (wc); break;
                    default: break;
                    }
                    if (match_flag) {
                        if (cset->reverse) return 0;
                        *match_len = mbc_len;
                        return 1;
                    }
                }
            }
            if (cset->ranges) {
                for (int i=0; i<num_array(cset->ranges); i++) {
                    char_range_t *range = get_array(cset->ranges, i);
                    wchar_t wc_start = range->wc_start;
                    wchar_t wc_end   = range->wc_end;
                    if (syntax_is(RE_ICASE)) {
                        wc_start = towupper(wc_start);
                        wc_end   = towupper(wc_end);
                    }
                    if (wc_start<=wc && wc<=wc_end) {
                        match_flag = 1;
                        if (cset->reverse) return 0;
                        *match_len = mbc_len;
                        return 1;
                    }
                }
            }
            if (!match_flag && cset->reverse) {
                *match_len = mbc_len;
                return 1;
            }
        }
        return 0;
#else
        *match_len = 1;
        return cset->chars[syntax_is(RE_ICASE)?toupper(*text):*text];
#endif //REG_ENABLE_UTF8
    case PAT_DOT:
        if (text_is_end(text)) return 0;
        if (!syntax_is(RE_DOT_NEWLINE) && *text=='\n') return 0;
        if (syntax_is(RE_DOT_NOT_NULL) && *text=='\0') return 0;
#ifdef REG_ENABLE_UTF8
        *match_len = mblen(text, g_text_end-text);
        if (*match_len<0) return 0;
        if (*match_len==0) *match_len = 1;  //'\0'にもマッチさせる
#else
        *match_len = 1;
#endif //REG_ENABLE_UTF8
        return 1;
    case PAT_SUBREG:
    {
        int ret = reg_exec_main(pat->regcomp, text, match_len);
        switch (pat->regcomp->assertion) {
        case POSITIVE_LOOKAHEAD:
            *match_len = 0;
            break;
        case NEGATIVE_LOOKAHEAD:
            *match_len = 0;
            ret = !ret;
            break;
        case POSITIVE_LOOKBEHIND:
            if (ret==0) g_text_match_start = text + *match_len;
            break;
        case NEGATIVE_LOOKBEHIND:
            ret = !ret;
            if (ret==0) {
                *match_len = pat->regcomp->total_len;
                g_text_match_start = text + *match_len;
            }
            break;
        }
        return ret==0;
    }
    case PAT_BACKREF:
    {
        if (text_is_end(text)) return 0;
        assert(pat->ref_num<g_nmatch);
        regmatch_t *regmat = g_pmatch + pat->ref_num;
        if (regmat->rm_so < 0) return 0;
        const char *substr = g_text_start + regmat->rm_so;
        int sublen = regmat->rm_eo - regmat->rm_so;
        assert(sublen>=0);
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
            if (reg_exec_main(subreg, text, match_len)==0) return 1;
            for (int i=1; i<g_nmatch; i++) {
                if (preg_compile->backref_flags&(1 << i)) {
                    g_pmatch[i].rm_so = -1;
                    g_pmatch[i].rm_eo = -1;
                }
            }
        }
        return 0;
    }
    case PAT_CARET:             //'^'
        if ((!(g_eflags&REG_NOTBOL) && text_is_start(text)) ||              //textの先頭にマッチ
            (syntax_is(RE_MULTILINE) && !text_is_start(text) && *(text-1)=='\n')) {//'\n'の次の空白文字にマッチ
            *match_len = 0; return 1;
        }
        return 0;
    case PAT_DOLLAR:            //'$'
        if ((!(g_eflags&REG_NOTEOL) && text_is_end(text)) ||                //textの最後にマッチ
            (syntax_is(RE_MULTILINE) && !text_is_end(text) && *text=='\n') ||//'\n'の前の空白文字にマッチ
                                                                            //PCREの$はtextの最後の'\n'にもマッチする
            (syntax_is(RE_PCRE2) && !text_is_end(text) && *text=='\n' && text_is_end(text+1))) {
            *match_len = 0; return 1;
        }
        return 0;
    case PAT_TEXT_FIRST:        //"\A", "\`" textの先頭にマッチするが'\n'にはマッチしない
        if (text_is_start(text)) {*match_len = 0; return 1;}
        return 0;
    case PAT_TEXT_LAST:         //"\z", "\'" textの最後にマッチするが'\n'にはマッチしない
        if (text_is_end(text)) {*match_len = 0; return 1;}
        return 0;
    case PAT_TEXT_LAST_NL:      //"\Z" textの最後と最後の'\n'にマッチする
        if (text_is_end(text) || (*text=='\n' && text_is_end(text+1))) {*match_len = 0; return 1;}
        return 0;
    case PAT_SET_START:         //"\K"
        g_text_match_start = text;
        *match_len = 0;
        return 1;
    case PAT_WORD_FIRST:        //"\<"
        if (text_is_word_first(text)) {*match_len = 0; return 1;}
        return 0;
    case PAT_WORD_LAST:         //"\>"
        if (text_is_word_last(text))  {*match_len = 0; return 1;}
        return 0;
    case PAT_WORD_DELIM:        //"\b"
        if ( text_is_word_first(text) ||  text_is_word_last(text)) {*match_len = 0; return 1;}
        return 0;
    case PAT_NOT_WORD_DELIM:    //"\B"
        if (!text_is_word_first(text) && !text_is_word_last(text)) {*match_len = 0; return 1;}
        return 0;
    case PAT_SET_OPTION:
        reg_syntax |=  pat->set_syntax;
        reg_syntax &= ~pat->unset_syntax;
        *match_len = 0; return 1;
    default:
        assert(0);
    }
    return 0;
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
#ifdef REG_ENABLE_UTF8
        for (int i=num_array(pat->cset->ranges)-1; i>=0; i--) {
            free(get_array(pat->cset->ranges, i));
        }
        free_array(pat->cset->ranges);
#endif //REG_ENABLE_UTF8
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

//定数とそのシンボル名のテーブル
typedef struct {
    uint64_t num;
    const char *name;
} name_tbl_t;
#define REG_TBL(s)  {REG_##s, #s}   //REG_TBL(EXTENDED) -> {REG_EXTENDED, "REG_EXTENDED"}
#define RE_TBL(s)   {RE_##s, #s}
static name_tbl_t cflags_tbl[] = {
    REG_TBL(EXTENDED),
    REG_TBL(ICASE),
    REG_TBL(NEWLINE),
};
static name_tbl_t eflags_tbl[] = {
    REG_TBL(NOTBOL),
    REG_TBL(NOTEOL),
    REG_TBL(STARTEND),
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
    RE_TBL(NO_GNU_OPS),
    RE_TBL(ICASE),
    RE_TBL(MULTILINE),
    RE_TBL(PCRE2),
    RE_TBL(COMMENT),
    RE_TBL(COMMENT_EXT),
    RE_TBL(ERROR_UNKNOWN_ESCAPE),
    RE_TBL(ERROR_DUP_REPEAT),
    RE_TBL(ALLOW_UNBALANCED_MINUS_IN_LIST),
    RE_TBL(ALLOW_ILLEGAL_REPEAT),
};
static const char* reg_flag2str(uint64_t flags, name_tbl_t *tbl, size_t size, char buf[]) {
    char *p = buf;
    int cnt = 0;
    uint64_t junk = flags;
    *p = '\0';
    for (int i=0; i<size; i++) {
        if (flags&tbl[i].num) {
            p += sprintf(p, "%s%s", cnt?"|":"RE_", tbl[i].name);
            cnt++;
            junk &= ~tbl[i].num;
        }
    }
    if (junk) sprintf(p, "|0x%lx", junk);
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
const char* reg_syntax2str(reg_syntax_t syntax) {
    static char buf[1024];
    return reg_flag2str(syntax, syntax_tbl, sizeof(syntax_tbl)/sizeof(syntax_tbl[0]), buf);
}
#ifdef REG_ENABLE_UTF8
static name_tbl_t class_tbl[] = {
    {CHAR_CLASS_ALPHA, "alpha"},
    {CHAR_CLASS_ALNUM, "alnum"},
    {CHAR_CLASS_DIGIT, "digit"},
    {CHAR_CLASS_XDIGIT,"xdigit"},
    {CHAR_CLASS_LOWER, "lower"},
    {CHAR_CLASS_UPPER, "upper"},
    {CHAR_CLASS_BLANK, "blank"},
    {CHAR_CLASS_SPACE, "space"},
    {CHAR_CLASS_PRINT, "print"},
    {CHAR_CLASS_GRAPH, "graph"},
    {CHAR_CLASS_CNTRL, "cntrl"},
    {CHAR_CLASS_PUNCT, "punct"},
};
static const char* reg_class2str(int type) {
    for (int i=0; i<sizeof(class_tbl)/sizeof(class_tbl[0]); i++) {
        if (class_tbl[i].num==type) return class_tbl[i].name;
    }
    return "?";
}
static void reg_print_wc(FILE* fp, wchar_t wc) {
    char buf[8] = {0};
    int mbs_len = wctomb(buf, wc);
    if (mbs_len==0) mbs_len = 1;    //wcがL'\0'の場合は'\0'1文字
    if (mbs_len<0) strcpy(buf, "\\???");
    fprintf(fp, "%s", buf);
}
#endif //REG_ENABLE_UTF8
void reg_print_str(FILE *fp, const char *bstr, int len) {
    const unsigned char *p = (const unsigned char*)bstr;
    for(int i=0; i<len; i++, p++) {
        switch (*p) {
        case '\a': fputs("\\a", fp); break;     //7
        case '\t': fputs("\\t", fp); break;     //9
        case '\n': fputs("\\n", fp); break;     //10
        case '\v': fputs("\\v", fp); break;     //11
        case '\f': fputs("\\f", fp); break;     //12
        case '\r': fputs("\\r", fp); break;     //13
        default:;
            if (iscntrl(*p)) {
                fprintf(fp, "\\%03o", *p);
            } else if (*p<0x1f) {
                fputc(*p, fp);
            } else {
#ifdef REG_ENABLE_UTF8
                int mlen = mblen((const char*)p, len);
                if (mlen>0) {
                    for (int j=0; j<mlen; j++) fputc(p[j], fp);
                    i += mlen-1;
                    p += mlen-1;
                } else
#endif //REG_ENABLE_UTF8
                fprintf(fp, "\\%03o", *p);
            }
        }
    }
}
static void reg_print_char_set(FILE *fp, char_set_t *char_set) {
    for (int i=0; i<256; i++) {
        if (char_set->chars[i]) {
            unsigned char c;
            if (i+2<' ' && char_set->chars[i+1] && char_set->chars[i+2]) {
                c = i;
                reg_print_str(fp, (const char*)&c, 1);
                for (i+=3; i<' ' && char_set->chars[i]; i++);
                i--;
                fputc('-', fp);
            }
            if (i>=0x7F && i+2<=0xFF && char_set->chars[i+1] && char_set->chars[i+2]) {
                c = i;
                reg_print_str(fp, (const char*)&c, 1);
                for (i+=3; i<256 && char_set->chars[i]; i++);
                i--;
                fputc('-', fp);
            }
            c = i;
            reg_print_str(fp, (const char*)&c, 1);
        }
    }
#ifdef REG_ENABLE_UTF8
    if (char_set->wcstr) fprintf(fp, "%S", char_set->wcstr);
#endif //REG_ENABLE_UTF8
}

//reg_dump: dump compiled regexp
void reg_dump(FILE *fp, reg_compile_t *preg_compile, int indent) {
    static const char* rep_mode_tbl[] = {"", "+", "?"};    //greedy, possessive, lazy
    fprintf(stderr, "%*s", indent, ""); // indent個の空白を出力
    if (preg_compile==NULL) {
        fprintf(stderr, "preg_compile=(nul)\n");
        return;
    }
    fprintf(fp, "regexp='");
    reg_print_str(fp, preg_compile->regexp, preg_compile->rlen);
    fprintf(fp, "', match_here=%d, nparen=%d, ref_num=%d, backref_flags=%x",
        preg_compile->match_here, preg_compile->nparen, preg_compile->ref_num,
        preg_compile->backref_flags);
    if (preg_compile->assertion)
        fprintf(fp, ", assertion=%s, total_len=%d, ", assertion_str[preg_compile->assertion], preg_compile->total_len);
    fprintf(fp, ", cflags=%s\n", reg_cflags2str(preg_compile->cflags));

    if (preg_compile->syntax) {
        fprintf(stderr, "%*s", indent, ""); // indent個の空白を出力
        fprintf(fp, "syntax=%lx:%s\n", preg_compile->syntax, reg_syntax2str(preg_compile->syntax));
    }
    if (preg_compile->set_syntax) {
        fprintf(stderr, "%*s", indent, ""); // indent個の空白を出力
        fprintf(fp, "set_syntax=%s\n", reg_syntax2str(preg_compile->set_syntax));
    }
    if (preg_compile->unset_syntax) {
        fprintf(stderr, "%*s", indent, ""); // indent個の空白を出力
        fprintf(fp, "unset_syntax=%s\n", reg_syntax2str(preg_compile->unset_syntax));
    }

    array_t *array = preg_compile->array;
    for (int i=0; i<array->num; i++) {
        pattern_t *pat = array->buckets[i];
        if (pat->type==PAT_NULL) break;
        fprintf(stderr, "%*s", indent, ""); // indent個の空白を出力
        fprintf(fp, "PAT[%2d]: type=%-11s: ", i, pattern_type_str[pat->type]);
        switch (pat->type) {
        case PAT_CARET:
            fprintf(fp, "^\n");
            break;
        case PAT_CHAR:
            fprintf(fp, "'");
            reg_print_str(fp, &pat->c, 1);
            fprintf(fp, "'\n");
            break;
#ifdef REG_ENABLE_UTF8
        case PAT_WC:
            fprintf(fp, "'%C'\n", pat->wc);
            break;
#endif //REG_ENABLE_UTF8
        case PAT_CHARSET:
            fprintf(fp, "%s", pat->cset->reverse?"^":"");
            char_set_t *cset = pat->cset;
#ifdef REG_ENABLE_UTF8
            int cnt = 0;
            for (int i=0; i<256; i++) if (cset->chars[i]) cnt++;
            if (cnt && cnt!=256) {
                fprintf(fp, "[");
                reg_print_char_set(fp, cset);
                fprintf(fp, "] ");
            }
            fprintf(fp, "[");
            for (int i=1; i<=CHAR_CLASS_PUNCT; i*=2) {
                if (!(cset->char_class&i)) continue;
                fprintf(fp, "[:%s:]", reg_class2str(i));
            }
            if (cset->ranges) {
                for (int i=0; i<num_array(cset->ranges); i++) {
                    char_range_t *range = get_array(cset->ranges, i);
                    reg_print_wc(fp, range->wc_start);
                    fputc('-', fp);
                    reg_print_wc(fp, range->wc_end);
                }
            }
#else
            fprintf(fp, "[");
            reg_print_char_set(fp, cset);
#endif //REG_ENABLE_UTF8
            fprintf(fp, "]\n");
            break;
        case PAT_REPEAT:  fprintf(fp, "{%d,%d}%s\n", pat->min, pat->max, rep_mode_tbl[pat->rep_mode]); break;
        case PAT_SUBREG:  fprintf(fp, "\n");
                          reg_dump(fp, pat->regcomp, indent+2); break;
        case PAT_BACKREF: fprintf(fp, "\\%d\n", pat->ref_num); break;
        case PAT_OR:      fprintf(fp, "\n");
                          for (int i=0; i<num_array(pat->or_array); i++)
                            reg_dump(fp, peek_array(pat->or_array, i), indent+2);
                          break;
        case PAT_SET_OPTION:
            if (pat->set_syntax)   fprintf(fp, "%s", reg_syntax2str(pat->set_syntax));
            if (pat->unset_syntax) fprintf(fp, "-%s", reg_syntax2str(pat->unset_syntax));
            fprintf(fp, "\n"); break;
        default:          fprintf(fp, "\n"); break;
        }
    }
}

//regex関数から流用。コメント行はPCRE2から。
static reg_err_info_t reg_err_def[] = {
    {/* 0*/REG_ERR_CODE_OK,                         "Success"},
    {/* 1*/REG_ERR_CODE_NOMATCH,                    "No match"},
    {/* 2*/REG_ERR_CODE_INVALID_PATTERN,            "Invalid pattern"},
    {/* 4*/REG_ERR_CODE_INVALID_CHAR_CLASS,         "Invalid character class name"},
//  { 130,                                          "unknown POSIX class name"},
    {/* 6*/REG_ERR_CODE_INVALID_BACK_REF,           "Invalid back reference"},
//  { 115,                                          "reference to non-existent subpattern"},
    {/* 7*/REG_ERR_CODE_UNMATCHED_BRACKET,          "Unmatched [, [^, [:, [., or [="},
//  { 106,                                          "missing terminating ] for character class"},
    {/* 8*/REG_ERR_CODE_UNMATCHED_PAREN,            "Unmatched ( or \\("},
//  { 114,                                          "missing closing parenthesis"},
//  { 122,                                          "unmatched closing parenthesis"},
    {/* 9*/REG_ERR_CODE_UNMATCHED_BRACE,            "Unmatched \\{"},
    {/*10*/REG_ERR_CODE_INVALID_CONTENT_OF_BRACE,   "Invalid content of \\{\\}"},
//  { 104,                                          "numbers out of order in {} quantifier"},
    {/*11*/REG_ERR_CODE_INVALID_RANGE_END,          "Invalid range end"},
//  { 108,                                          "range out of order in character class"},
    {/*13*/REG_ERR_CODE_INVALID_PRECEDING_REGEXP,   "Invalid preceding regular expression"},
//  { 109,                                          "quantifier does not follow a repeatable item"},
    {/*15*/REG_ERR_CODE_REGEXP_TOO_BIG,             "Regular expression too big"},
    {/*102*/REG_ERR_CODE_CTRL_C_AT_END,             "\\c at end of pattern"},
    {/*103*/REG_ERR_CODE_UNKNOWN_ESCAPE,            "unrecognized character follows"},
    {/*111*/REG_ERR_CODE_UNRECOGNIZED_CHAR_PAREN,   "unrecognized character after (? or (?-"},
    {/*125*/REG_ERR_CODE_LOOKBEHIND_NOT_FIXLEN,     "lookbehind assertion is not fixed length"},
    {/*134*/REG_ERR_CODE_CODE_POINT_TOO_LARGE,      "character code point value in \\x{} or \\o{} is too large"},
    {/*142*/REG_ERR_CODE_SYNTAX_ERR_IN_SUBPTN_NAME, "syntax error in subpattern name (missing terminator?)"},
    {/*137*/REG_ERR_CODE_INVALID_UNICODE,           "invalid \\N{U+}"},
    {/*155*/REG_ERR_CODE_MISSING_OPEN_BRACE_O,      "missing opening brace after \\o"},
    {/*162*/REG_ERR_CODE_SUBPATTERN_NAME_EXPECTED,  "subpattern name expected"},
    {/*164*/REG_ERR_CODE_NON_OCTAL_CHAR,            "non-octal character in \\o{} (closing brace missing?)"},
    {/*167*/REG_ERR_CODE_NON_HEX_CHAR,              "non-hex character in \\x{} (closing brace missing?)"},
    {/*168*/REG_ERR_CODE_NOT_PRINTABLE_ASCII,       "\\c must be followed by a printable ASCII character"},
    {/*178*/REG_ERR_CODE_DIGITS_MISSING,            "digits missing in \\x{} or \\o{} or \\N{U+}"},
    {/*194*/REG_ERR_CODE_INVALID_HYPHEM_IN_OPTION,  "invalid hyphen in option setting"},
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

#ifdef REG_ENABLE_UTF8
#define char_is_utf8_later(c) ((c)>=0x80 && (c)<=0xBF)
//マルチバイト文字のバイト数を返す。
//有効なマルチバイト文字でない場合は1を返す。
static int reg_mblen(const char *str, int len_str) {
    int mb_cur_max = MB_CUR_MAX;
    assert (len_str>=0);
    if (len_str<mb_cur_max) mb_cur_max = len_str;
    int len = mblen(str, mb_cur_max);
    if (len<=0) len = 1;
    //if (len<=0) len = 1;
    return len;
}
#endif //REG_ENABLE_UTF8
