#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include "regexp.h"

typedef struct {
    const char *text;
    const char *regexp;
    const int expect;
} test_t;
test_t data[] = {
    #include "test_dat.h"
};

static int test_regexp(int n, const char *text, const char *regexp) {
    regex_t preg;
    int errcode;
    int ret1, ret2; //1:一致、0:不一致、-1:エラー
    //regmatch_t *pmatch;
    errcode = regcomp(&preg, regexp, REG_NOSUB);
    if (errcode!=0) {
        char buf[1024];
        regerror(errcode, &preg, buf, sizeof(buf));
        fprintf(stderr, "%d: regex Error: regexp='%s', %s\n", n, regexp, buf);
        ret1 = -1;
    } else {
        //size_t nmatch = preg.re_nsub+1;
        //pmatch = calloc(nmatch, sizeof(regmatch_t));
        //int ret1 = regexec(&preg, text, nmatch, pmatch, REG_NEWLINE)==0;
        ret1 = regexec(&preg, text, 0, NULL, 0)==0;
    }
    ret2 = match(regexp, text);
    if (ret1 != ret2) {
        printf("ERROR: %d: %d:%d text='%s', regexp='%s'\n", n, ret1, ret2, text, regexp);
    }
    //free(pmatch);
    regfree(&preg);
    return 1;
}

int main(int argc, char **argv) {
    int result = 0;
    for (int i=0; i<sizeof(data)/sizeof(test_t); i++) {
        test_t *t = &data[i];
        test_regexp(i, t->text, t->regexp);
    }
    return result;
}