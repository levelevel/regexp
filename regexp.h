#pragma once

#include <regex.h>

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

//エラーコードの定義
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
} reg_err_code_t;

//エラー情報
typedef struct {
    reg_err_code_t err_code;    //エラーコード
    const char    *err_msg;     //エラーメッセージ
} reg_err_info_t;

extern reg_err_info_t reg_err_info; //エラー情報
extern int reg_syntax;              //syntax

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
//各ビットの詳細は/usr/include/regex.hを参照。ただし定義されているすべての機能を実装しているわけではない。
reg_compile_t* reg_compile2(const char *regexp, size_t len, size_t *nsub, int cflags, int syntax);

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
const char* reg_syntax2str(int syntax);
