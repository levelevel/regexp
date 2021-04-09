#pragma once

#include <regex.h>

#define RE_DUP_MAX 255
//#define RE_DUP_MAX 0x7fff //regexライブラリの最大値(32767)。
                            //\(1,32767\)をコンパイルするとメモリ2GB以上消費する。

typedef struct regcomp reg_compile_t;

extern int reg_err_code;        //エラーコード
extern const char *reg_err_msg; //エラーメッセージ

reg_compile_t* reg_compile(const char *regexp);
int reg_exec(reg_compile_t *preg_compile, const char *text, size_t nmatch, regmatch_t *pmatch);
void reg_compile_free(reg_compile_t* preg_compile);
int match(const char *regexp, const char *text);
void reg_dump(FILE *fp, reg_compile_t *preg_compile, int indent);
