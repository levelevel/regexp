#pragma once

#include <regex.h>

typedef struct regcomp reg_compile_t;

extern int reg_err_code;        //エラーコード
extern const char *reg_err_msg; //エラーメッセージ

reg_compile_t* reg_compile(const char *regexp);
int reg_exec(reg_compile_t *preg_compile, const char *text, size_t nmatch, regmatch_t *pmatch);
void reg_compile_free(reg_compile_t* preg_compile);
int match(const char *regexp, const char *text);
