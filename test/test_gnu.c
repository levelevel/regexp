#include "test.h"

//パターンをコンパイルする。
//成功時は0、異常時は0以外を返す。
int gnu_regcomp(void *vpreg, const char *pattern, size_t len, int cflags, int on_syntax, int off_syntax, size_t *nmatch) {
    int ret = 0;
    regex_t *preg = vpreg;
    static char buf[128];
    ref_err_info.err_code = 0;
    ref_err_info.err_msg = "";
    if (use_posix_version) {
        assert(pattern==NULL || strlen(pattern)==len);
        ret = regcomp(preg, pattern, cflags);
        ref_err_info.err_code = ret;
        ref_err_info.err_msg = buf;
        regerror(ret, preg, buf, sizeof(buf));
    } else {
        int syntax = ((cflags & REG_EXTENDED) ? RE_SYNTAX_POSIX_EXTENDED : RE_SYNTAX_POSIX_BASIC);
        syntax |= (cflags & REG_ICASE) ? RE_ICASE : 0;
        if (cflags & REG_NEWLINE)
            { /* REG_NEWLINE implies neither . nor [^...] match newline.  */
            syntax &= ~RE_DOT_NEWLINE;
            syntax |= RE_HAT_LISTS_NOT_NEWLINE;
            /* It also changes the matching behavior.  */
            preg->newline_anchor = 1;
            }
        else
            preg->newline_anchor = 0;
        syntax |= on_syntax;
        syntax &= ~off_syntax;
        preg->no_sub = !!(cflags & REG_NOSUB);

        re_set_syntax(syntax);
        const char *err_str = re_compile_pattern(pattern, len, preg);

        if (err_str) {
            //printf("pattern='%s' :%s\n", pattern, err_str);
            ref_err_info.err_code = 1;
            ref_err_info.err_msg = err_str;
            ret = 1;
        }

        //re_compile_patternでは無条件にnewline_anchor=1とするので再設定する。
        preg->newline_anchor = !!(cflags & REG_NEWLINE);
        //gnu_dump(preg);
    }
    if (ret==0) *nmatch = preg->re_nsub;
    return ret;
}

int gnu_regexec(void *vpreg, const char *string, size_t len, size_t nmatch, regmatch_t pmatch[], test_t *test) {
    int ret = 0;
    regex_t *preg = vpreg;
    if (use_posix_version) {
        assert(strlen(string)==len);
        ret = regexec(preg, string, nmatch, pmatch, test->eflags);
    } else {
        struct re_registers regs;
        re_set_registers(preg, &regs, nmatch, malloc(nmatch*sizeof(regoff_t)), malloc(nmatch*sizeof(regoff_t)));
        int start = 0;
        int range = len;
        //re_serchにはeflagsが存在しないので自力で設定する。
        if (test->eflags&REG_NOTBOL) preg->not_bol = 1;
        if (test->eflags&REG_NOTEOL) preg->not_eol = 1;
        if (test->eflags&REG_STARTEND) {
            start = test->start;
            range = test->end - start;
        }
        int so = re_search(preg, string, len, start, range, &regs);
        if (so>=0) {
            //regsをpmatchに変換する。
            for (int i=0; i<nmatch; i++) {
                pmatch[i].rm_so = regs.start[i];
                pmatch[i].rm_eo = regs.end[i];
            }
        } else ret = 1;
        free(regs.start);
        free(regs.end);
    }
    return ret;
}

void gnu_dump(regex_t *preg) {
    printf("  newline_anchor=%d, not_bol=%d, not_eol=%d\n",
        preg->newline_anchor, preg->not_bol, preg->not_eol);
    printf("  syntax=%s\n", reg_syntax2str(preg->syntax));
    //printf("  syntax=%s\n", reg_syntax2str(re_syntax_options));
}

regex_t *gnu_alloc_regex(void) {
    static regex_t preg;
    memset(&preg, 0, sizeof(regex_t));
    return &preg;
}

void gnu_free_regex(regex_t *preg) {
    regfree(preg);
}
