#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "../regexp.h"

//テスト項目
typedef struct {
    const int no;           //テストNo:行番号
    const char *text;       //検索文字列
    const char *regexp;     //正規表現
    const char *match[10];  //マッチした文字列の配列（全体＋グループ分）
    const int nmatch;       //グループの数
    const int expect;       //戻り値 0:OK, 1:不一致, -1:コンパイルエラー
          int cflags;       //REG_EXTENDED|REG_ICASE|REG_NEWLINE|REG_NOSUB
    const int eflags;       //REG_NOTBOL|REG_NOTEOL|REG_STARTEND
} test_t;
#define REG_BASIC   0
#define REG_BRE_ERE 0x10000     //cflagsでBER/ERB共通テスト項目（REG_EXTENDED有無両方でテストする）
#define REG_DUMP    0x20000     //強制的にダンプする
#define REG_FLAGS_MASK 0xff

test_t data[] = {
    #include "test_dat.h"
};

static int test_regexp(test_t *test_data) {
    int n = test_data->no;
    const char *text = test_data->text;
    const char *regexp = test_data->regexp;
    regex_t preg = {0};
    reg_compile_t *preg_compile;
    int errcode;
    int ret1, ret2; //1:一致、0:不一致、-1:エラー
    int result = 1; //1:OK, 0:NG
    char buf[1024];

    //regex
    regmatch_t *pmatch1 = NULL;
    size_t nmatch1 = 0;
    errcode = regcomp(&preg, regexp, test_data->cflags&REG_FLAGS_MASK);
    if (errcode!=0) {
        regerror(errcode, &preg, buf, sizeof(buf));
        ret1 = -1;
    } else {
        nmatch1 = preg.re_nsub+1;
        pmatch1 = calloc(nmatch1, sizeof(regmatch_t));
        ret1 = regexec(&preg, text, nmatch1, pmatch1, test_data->eflags);
    }

    //regexp
    regmatch_t *pmatch2 = NULL;
    size_t nmatch2 = 0;
    preg_compile = reg_compile(regexp, &nmatch2, test_data->cflags&REG_FLAGS_MASK);
    if (preg_compile==NULL) {
        ret2 = -1;
    } else {
        nmatch2++;
        pmatch2 = calloc(nmatch2, sizeof(regmatch_t));
        ret2 = reg_exec(preg_compile, text, nmatch2, pmatch2, test_data->eflags);
    }

    for (int i=0; i<test_data->nmatch; i++) {
        int len = strlen(test_data->match[i]);
        int match_sub = ret1<0 || ret2<0 ||
            (len==pmatch2[i].rm_eo-pmatch2[i].rm_so &&
            strncmp(test_data->text+pmatch2[i].rm_so, test_data->match[i], len)==0 &&
            pmatch1[i].rm_so==pmatch2[i].rm_so && pmatch1[i].rm_eo==pmatch2[i].rm_eo);
        if (ret1 != ret2 || ret1 != test_data->expect || !match_sub || nmatch1!=nmatch2) result = 0;
        if (result==0 || (test_data->cflags&REG_DUMP)) {
            printf("%s: line=%d[%d]: ret=%d:%d text='%s', regexp='%s', nmatch=%ld:%ld, cflags=%x\n", 
                result==0?"ERROR":"DUMP", n, i, ret1, ret2, text, regexp, nmatch1, nmatch2, test_data->cflags);
            if (!match_sub || (test_data->cflags&REG_DUMP)) {
                printf("  pmatch1=[%d,%d], pmatch2=[%d,%d], expected_match='%s'\n",
                    pmatch1[i].rm_so, pmatch1[i].rm_eo, 
                    pmatch2[i].rm_so, pmatch2[i].rm_eo, test_data->match[i]);
            }
        }
    }
    if (result==0) {
        reg_dump(stdout, preg_compile, 2);
    } else {
        //printf("DATA: %d regexp='%s'\n", n, regexp);
        //reg_dump(stdout, preg_compile, 2);
    }
    if (errcode!=reg_err_info.err_code) {
        fprintf(stderr, "%d: regex  Error: regexp='%s', errcode=%d, %s\n", n, regexp, errcode, errcode?buf:"");
        for (int i=0; i<nmatch1; i++) printf("    pmatch1[%d]=[%d,%d]\n", i, pmatch1[i].rm_so, pmatch1[i].rm_eo);
        fprintf(stderr, "%d: regexp Error: regexp='%s', errcode=%d, %s\n", n, regexp, reg_err_info.err_code, reg_err_info.err_msg);
    }

    free(pmatch1);
    free(pmatch2);
    regfree(&preg);
    reg_compile_free(preg_compile);
    preg_compile = NULL;
    return result;
}

#define UNUSED(var) (void)var
int main(int argc, char **argv) {
    UNUSED(argc);
    UNUSED(argv);
    int err_cnt = 0;
    int bre_cnt = 0;
    int ere_cnt = 0;
    int size = sizeof(data)/sizeof(test_t);
    for (int i=0; i<size; i++) {
        test_t *test = &data[i];
        if (test->text==NULL) break;
        if (test->cflags&REG_BRE_ERE) {
            test->cflags &= ~(REG_EXTENDED|REG_BRE_ERE);
            if (test_regexp(test)==0) err_cnt++;    //BREでテスト
            bre_cnt++;
            test->cflags |= REG_EXTENDED;
            if (test_regexp(test)==0) err_cnt++;    //EREでテスト
            ere_cnt++;
        } else {
            if (test_regexp(test)==0) err_cnt++;
            if (test->cflags&REG_EXTENDED) ere_cnt++;
            else                           bre_cnt++;
        }
    }
    printf("%s: %d/%d (BRE=%d, ERE=%d)\n", err_cnt?"FAIL":"PASS", err_cnt?err_cnt:size, size, bre_cnt, ere_cnt);
    return !(err_cnt==0);
}
