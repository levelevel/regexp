#pragma once

#include <regex.h>

#define MULTI_REPEAT_SUBREG     //多重繰り返しをSUBREGで表現する（(s*)*）

//#define RE_DUP_MAX 0x7fff //regexライブラリの最大値(32767)。
                            //{1,32767}をコンパイルするとメモリ2GB以上消費する。
#undef  RE_DUP_MAX
#define RE_DUP_MAX 255

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

typedef struct regcomp reg_compile_t;

//エラーコードの定義
typedef enum {
    REG_ERR_CODE_OK = 0,
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

reg_compile_t* reg_compile(const char *regexp, size_t *re_nsub, int cflags);
int reg_exec(reg_compile_t *preg_compile, const char *text, size_t nmatch, regmatch_t *pmatch, int eflags);
void reg_compile_free(reg_compile_t* preg_compile);
void reg_dump(FILE *fp, reg_compile_t *preg_compile, int indent);
