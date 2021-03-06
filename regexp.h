#pragma once

#include <regex.h>
#include <stdint.h>

#define REG_ENABLE_UTF8    //UTF-8対応

//#define RE_DUP_MAX 0x7fff //regexライブラリの最大値(32767)。
                            //{1,32767}をコンパイルするとメモリ2GB以上消費する。
#undef  RE_DUP_MAX
#define RE_DUP_MAX 255

// /usr/include/regex.h ---------------------------------------------------
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
// /usr/include/regex.h ---------------------------------------------------

typedef struct regcomp reg_compile_t;  //コンパイル結果

//エラーコードの定義（値はregex関数から流用）
typedef enum {
    REG_ERR_CODE_OK                       = 0,
    REG_ERR_CODE_NOMATCH                  = 1,
    REG_ERR_CODE_INVALID_PATTERN          = 2,
    REG_ERR_CODE_INVALID_CHAR_CLASS       = 4,
    REG_ERR_CODE_INVALID_BACK_REF         = 6,
    REG_ERR_CODE_UNMATCHED_BRACKET        = 7,
    REG_ERR_CODE_UNMATCHED_PAREN          = 8,
    REG_ERR_CODE_UNMATCHED_BRACE          = 9,
    REG_ERR_CODE_INVALID_CONTENT_OF_BRACE = 10,
    REG_ERR_CODE_INVALID_RANGE_END        = 11,
    REG_ERR_CODE_INVALID_PRECEDING_REGEXP = 13,
    REG_ERR_CODE_REGEXP_TOO_BIG           = 15,
    //ここからはPCRE2から流用
    REG_ERR_CODE_CTRL_C_AT_END            = 102,   //\c at end of pattern
    REG_ERR_CODE_UNKNOWN_ESCAPE           = 103,
    REG_ERR_CODE_UNRECOGNIZED_CHAR_PAREN  = 111,   //unrecognized character after (? or (?-
    REG_ERR_CODE_LOOKBEHIND_NOT_FIXLEN    = 125,   //lookbehind assertion is not fixed length
    REG_ERR_CODE_CODE_POINT_TOO_LARGE     = 134,   //character code point value in \x{} or \o{} is too large
    REG_ERR_CODE_INVALID_UNICODE          = 137,   //PCRE2 does not support \F, \L, \l, \N{name}, \U, or \u
    REG_ERR_CODE_SYNTAX_ERR_IN_SUBPTN_NAME= 142,   //syntax error in subpattern name (missing terminator?)
    REG_ERR_CODE_MISSING_OPEN_BRACE_O     = 155,   //missing opening brace after \o
    REG_ERR_CODE_SUBPATTERN_NAME_EXPECTED = 162,   //subpattern name expected
    REG_ERR_CODE_NON_OCTAL_CHAR           = 164,   //non-octal character in \o{} (closing brace missing?)
    REG_ERR_CODE_NON_HEX_CHAR             = 167,   //non-hex character in \x{} (closing brace missing?)
    REG_ERR_CODE_NOT_PRINTABLE_ASCII      = 168,   //\c must be followed by a printable ASCII character
    REG_ERR_CODE_DIGITS_MISSING           = 178,   //digits missing in \x{} or \o{} or \N{U+}
    REG_ERR_CODE_INVALID_HYPHEM_IN_OPTION = 194,   //invalid hyphen in option setting
} reg_err_code_t;

//エラー情報
typedef struct {
    reg_err_code_t err_code;    //エラーコード
    const char    *err_msg;     //エラーメッセージ
} reg_err_info_t;

//typedef unsigned long int reg_syntax_t;    //regex.hで定義(64bit)

extern reg_err_info_t reg_err_info; //エラー情報
extern reg_syntax_t reg_syntax;     //syntax

//REG_NEWLINE相当（regexのnewline_anchor）
#define RE_MULTILINE                      0x0001000000000000LL

//PCRE2互換
//PCER2拡張機能を有効にする。
//\A,\z
#define RE_PCRE2                          0x0002000000000000LL
//正規表現中の空白文字、#以降行末までを無視する。ただし[]内は除く。
#define RE_COMMENT                        0x0004000000000000LL  //PCRE2_EXTENDED
//上記に加えて[]内にも適用する。
#define RE_COMMENT_EXT                    0x0008000000000000LL  //PCRE2_EXTENDED_MORE
//'\'による不正なエスケープをエラーとする。
//指定されていない場合は、次の文字のリテラルとして処理する。
#define RE_ERROR_UNKNOWN_ESCAPE           0x0010000000000000LL
//"a**"のような繰り返し指定の連続をエラーとする。
#define RE_ERROR_DUP_REPEAT               0x0020000000000000LL
//"[a-]"のような-をリテラルとして扱う。
#define RE_ALLOW_UNBALANCED_MINUS_IN_LIST 0x0040000000000000LL
//"a{1b}"のような不正な繰り返し指定を単なるリテラル文字列として扱う。
#define RE_ALLOW_ILLEGAL_REPEAT           0x0080000000000000LL

#define RE_SYNTAX_PCRE  ((RE_SYNTAX_POSIX_EXTENDED|\
   RE_NO_GNU_OPS|RE_PCRE2|RE_ERROR_UNKNOWN_ESCAPE|RE_ERROR_DUP_REPEAT|RE_ALLOW_UNBALANCED_MINUS_IN_LIST|RE_ALLOW_ILLEGAL_REPEAT)\
   &(~RE_UNMATCHED_RIGHT_PAREN_ORD))

//reg_compileは正規表現regexp（長さlenのバイト列）をコンパイルして、コンパイル結果のポインタを返す。
//lenに負の値を指定すると、regexpをNUL終端の文字列として処理する。
//エラー発生時はNULLを返し、reg_err_infoにエラー情報が格納される。
//NULLでないre_nsubが指定された場合は、()で囲まれたサブパターンの数を返す。
//cflagsにはregex互換の以下に示す定数一つ以上のビットごとの OR (bitwise-or) を指定する。
// - REG_EXTENDED
// - REG_ICASE
// - REG_NEWLINE
//REG_EXTENDEDを指定しないと基本正規表現(BRE)、指定すると拡張正規表現(ERE)となる。
//7bit ASCIIのみ対応する。
reg_compile_t* reg_compile (const char *regexp, size_t len, size_t *nsub, int cflags);

//reg_compile2はreg_compiletとほぼ同等の機能であるが、cflagsのREG_EXTENDEDの有無による機能の変更は無効になり、
//代わりにsyntaxで機能を指定する。
//cflagsにはregex互換の以下に示す定義済み機能や、個別の機能をビットごとの OR (bitwise-or) で指定する。
// - RE_SYNTAX_AWK
// - RE_SYNTAX_GNU_AWK
// - RE_SYNTAX_POSIX_AWK
// - RE_SYNTAX_GREP
// - RE_SYNTAX_EGREP
// - RE_SYNTAX_POSIX_EGREP
// - RE_SYNTAX_ED
// - RE_SYNTAX_SED
// - RE_SYNTAX_POSIX_BASIC             # reg_compileのデフォルト
// - RE_SYNTAX_POSIX_MINIMAL_BASIC
// - RE_SYNTAX_POSIX_EXTENDED          # reg_compileでREG_EXTENDEDを指定した場合
// - RE_SYNTAX_POSIX_MINIMAL_EXTENDED
// - RE_SYNTAX_PCRE                    # PCRE互換拡張
//各ビットの詳細は/usr/include/regex.hを参照。ただし定義されているすべての機能を実装しているわけではない。
reg_compile_t* reg_compile2(const char *regexp, size_t len, size_t *nsub, int cflags, reg_syntax_t syntax);

//reg_compileでコンパイルした結果をもとに、文字列text（長さlenのバイト列）を検索する。
//lenに負の値を指定すると、textをNUL終端の文字列として処理する。
//成功した場合は0、失敗した場合は0以外を返す。
//NULLでないpmatchを指定した場合は、マッチングした文字列のオフセットを得ることができる。
//pmatchのサイズは少なくともnsub+1個以上が必要となる。
//eflagsにはregex互換の以下に示す定数一つ以上のビットごとの OR (bitwise-or) を指定する。
// - REG_NOTBOL
// - REG_NOTEOL
// - REG_STARTEND
int reg_exec(reg_compile_t *preg_compile, const char *text, size_t len, size_t nmatch, regmatch_t *pmatch, int eflags);

//reg_compileが返したコンパイル結果を解放する。
void reg_compile_free(reg_compile_t* preg_compile);

//reg_compileが返したコンパイル結果をダンプする。
void reg_dump(FILE *fp, reg_compile_t *preg_compile, int indent);

//長さlenのバイト列bstrをダンプする。
void reg_print_str(FILE *fp, const char *bstr, int len);

//cflags,eflags,syntaxのビット情報を文字列に変換する。
//文字列は各関数内の静的領域を指している。
const char* reg_cflags2str(int cflags);
const char* reg_eflags2str(int eflags);
const char* reg_syntax2str(reg_syntax_t syntax);
