#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#include "../regexp.h"

typedef struct {
    int no;
    const char *text;
    const char *regexp;
    const char *match;
    const int expect;
} test_t;
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

    regmatch_t *pmatch1 = NULL;
    errcode = regcomp(&preg, regexp, 0);
    if (errcode!=0) {
        regerror(errcode, &preg, buf, sizeof(buf));
        ret1 = -1;
    } else {
        size_t nmatch1 = preg.re_nsub+1;
        pmatch1 = calloc(nmatch1, sizeof(regmatch_t));
        ret1 = regexec(&preg, text, nmatch1, pmatch1, 0);
    }

    regmatch_t *pmatch2 = NULL;
    preg_compile = reg_compile(regexp);
    if (preg_compile==NULL) {
        ret2 = -1;
    } else {
        size_t nmatch2 = 1;
        pmatch2 = calloc(nmatch2, sizeof(regmatch_t));
        ret2 = reg_exec(preg_compile, text, nmatch2, pmatch2);
    }

    int len = strlen(test_data->match);
    int match_sub = ret1<0 || ret2<0 ||
        (len==pmatch2->rm_eo-pmatch2->rm_so &&
        strncmp(test_data->text+pmatch2->rm_so, test_data->match, len)==0 &&
        pmatch1->rm_so==pmatch2->rm_so && pmatch1->rm_eo==pmatch2->rm_eo);
    if (ret1 != ret2 || ret1 != test_data->expect || !match_sub) {
        printf("ERROR: %d: %d:%d text='%s', regexp='%s'\n", n, ret1, ret2, text, regexp);
        if (!match_sub) {
            printf("  pmatch1=[%d,%d], pmatch2=[%d,%d], expected_match='%s'\n",
                pmatch1->rm_so, pmatch1->rm_eo, pmatch2->rm_so, pmatch2->rm_eo, test_data->match);
        }
        reg_dump(stdout, preg_compile, 2);
        result = 0;
    } else {
        //printf("DATA: %d regexp='%s'\n", n, regexp);
        //reg_dump(stdout, preg_compile, 2);
    }
    if (errcode!=reg_err_code) {
        fprintf(stderr, "%d: regex  Error: regexp='%s', errcode=%d, %s\n", n, regexp, errcode, buf);
        fprintf(stderr, "%d: regexp Error: regexp='%s', errcode=%d, %s\n", n, regexp, reg_err_code, reg_err_msg);
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
    int size = sizeof(data)/sizeof(test_t);
    for (int i=0; i<size; i++) {
        if (data[i].text==NULL) break;
        if (test_regexp(&data[i])==0) err_cnt++;
    }
    if (err_cnt==0) {
        printf("PASS: %d/%d\n", size, size);
        return 0;
    } else {
        printf("FAIL: %d/%d\n", err_cnt, size);
        return 1;
    }
}
