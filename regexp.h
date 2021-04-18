#pragma once

#include <regex.h>

#define MULTI_REPEAT            //EREで多重繰り返し（s**やs{2}{3}）をサポートする
#define MULTI_REPEAT_SUBREG     //多重繰り返しをSUBREGで表現する（(s*)*）

//#define RE_DUP_MAX 0x7fff //regexライブラリの最大値(32767)。
                            //\(1,32767\)をコンパイルするとメモリ2GB以上消費する。
#undef  RE_DUP_MAX
#define RE_DUP_MAX 255

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
