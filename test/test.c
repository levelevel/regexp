#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#include "../regexp.h"

typedef struct {
    int no;
    const char *text;
    const char *regexp;
    const int expect;
} test_t;
test_t data[] = {
    #include "test_dat.h"
};

static int test_regexp(test_t *test_data) {
    int n = test_data->no;
    const char *text = test_data->text;
    const char *regexp = test_data->regexp;
    regex_t preg;
    reg_compile_t *preg_compile;
    int errcode;
    int ret1, ret2; //1:一致、0:不一致、-1:エラー
    int result = 1; //1:OK, 0:NG
    char buf[1024];

    //regmatch_t *pmatch;
    errcode = regcomp(&preg, regexp, REG_NOSUB);
    if (errcode!=0) {
        regerror(errcode, &preg, buf, sizeof(buf));
        ret1 = -1;
    } else {
        //size_t nmatch = preg.re_nsub+1;
        //pmatch = calloc(nmatch, sizeof(regmatch_t));
        //int ret1 = regexec(&preg, text, nmatch, pmatch, REG_NEWLINE)==0;
        ret1 = regexec(&preg, text, 0, NULL, 0)==0;
    }

    preg_compile = reg_compile(regexp);
    if (preg_compile==NULL) {
        ret2 = -1;
    } else {
        ret2 = reg_exec(preg_compile, text);
    }

    if (ret1 != ret2 || ret1 != test_data->expect) {
        printf("ERROR: %d: %d:%d text='%s', regexp='%s'\n", n, ret1, ret2, text, regexp);
        result = 0;
    }
    if (errcode!=reg_err_code) {
        fprintf(stderr, "%d: regex  Error: regexp='%s', errcode=%d, %s\n", n, regexp, errcode, buf);
        fprintf(stderr, "%d: regexp Error: regexp='%s', errcode=%d, %s\n", n, regexp, reg_err_code, reg_err_msg);
    }

    //free(pmatch);
    regfree(&preg);
    reg_compile_free(preg_compile);
    preg_compile = NULL;
    return result;
}

int main(int argc, char **argv) {
    int err_cnt = 0;
    int size = sizeof(data)/sizeof(test_t);
    for (int i=0; i<size; i++) {
        if (test_regexp(&data[i])==0) err_cnt++;
    }
    if (err_cnt==0) {
        printf("PASS: %d\n", size);
        return 0;
    } else {
        printf("FAIL: %d/%d\n", err_cnt, size);
        return 1;
    }
}