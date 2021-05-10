#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "locale.h"

#define __USE_GNU
#include <regex.h>

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

#include "../regexp.h"

#define REG_ENABLE_PCRE2    //PCRE2有効化

//テスト項目
typedef struct {
    int no;                 //テストNo:行番号
    struct {
        const char *text;   //検索文字列。'\0'を含むことができる。
        int tlen;           //長さ。0の場合はstrlen(text)
    };
    struct {
        const char *regexp; //正規表現。'\0'を含むことができる。
        int rlen;           //長さ。0の場合はstrlen(regexp)
    };
    struct {                //[0]:正規表現全体、[1]...:サブパターン
        const char *bstr;   //マッチした文字列。'\0'を含むことができる。
        int len;            //長さ。0の場合はstrlen(regexp)
    } match[16];            //マッチした文字列の配列（全体＋グループ分）
    int nmatch;             //サブパターンの数
    int expect;             //戻り値 0:OK, 1:不一致, -1:コンパイルエラー
    int cflags;             //REG_EXTENDED|REG_ICASE|REG_NEWLINE|REG_NOSUB
    int eflags;             //REG_NOTBOL|REG_NOTEOL|REG_STARTEND
    int on_syntax;          //追加するsyntax
    int off_syntax;         //削除するsyntax
    struct {
        int start;          //検索文字列の開始・終了位置のインデックス(REG_STARTEND指定時)
        int end;            //endが0の場合はtlen
    };
} test_bstr_t;

//リファレンス
typedef enum {
    REF_GNUE_REGEX,     //regcomp/regexec
    REF_GNU,            //re_compile_pattern/re_serch
    REF_PCRE2,          //
} ref_func_type_t;

extern reg_err_info_t ref_err_info; //リファレンス(GNU/PCRE2)のエラー情報

extern int use_posix_version;

int gnu_regcomp (void *preg, const char *pattern, size_t len, int cflags, int on_syntax, int off_syntax, size_t *nmatch);
int gnu_regexec (void *preg, const char *string, size_t len, size_t nmatch, regmatch_t pmatch[], test_bstr_t *test);
void gnu_dump(regex_t *preg);
regex_t *gnu_alloc_regex(void);
void gnu_free_regex(regex_t *preg);

int pc_regcomp(void **preg, const char *pattern, size_t len, int cflags, int on_syntax, int off_syntax);
int pc_regexec(void *preg, const char *string, size_t len, size_t *nmatch, regmatch_t **pmatch, test_bstr_t *test);
void pc_dump(pcre2_code_8 *pcode);
void pc_free_regex(pcre2_code_8 *pcode);
