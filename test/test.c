#define USE_GNU
#include "test.h"
#include "../regexp.h"

//cflags拡張
#define REG_BRE         0x10000             //BRE(GNU)
#define REG_ERE         0x20000             //BRE(GNU)
#define REG_PCRE2       0x40000             //PCRE2
#define REG_GNU         (REG_BRE|REG_ERE)   //BRE/ERE共通テスト項目（REG_EXTENDED有無両方でテストする）
#define REG_EXT         (REG_ERE|REG_PCRE2)
#define REG_ALL         (REG_GNU|REG_PCRE2)
#define REG_DUMP        0x80000             //強制的にダンプする
#define REG_CFLAGS_MASK 0xffff

//テストデータ
test_bstr_t bdata[] = {
    #include "test_dat.h"
};

//リファレンス関数として何を使うかを指定する。
int use_gnu_version;
int use_posix_version;

reg_err_info_t posix_err_info; //エラー情報

static int test_regexp(test_bstr_t *test_data) {
    int n = test_data->no;
    const char *text = test_data->text;
    const char *regexp = test_data->regexp;
    int tlen = test_data->tlen;
    int rlen = test_data->rlen;
    int errcode = 0;
    int ret1 = 0, ret2 = 0; //1:一致、0:不一致、-1:エラー
    int result = 1;         //1:OK, 0:NG
    memset(&posix_err_info, 0, sizeof(reg_err_info_t));
    if (tlen==0 && text)   tlen = strlen(text);
    if (rlen==0 && regexp) rlen = strlen(regexp);
    if ((test_data->eflags&REG_STARTEND) && test_data->end==0) test_data->end = tlen;

    //regex
    void *void_preg = NULL;
    regmatch_t *pmatch1 = NULL;     //GNU/PCRE2共通の構造体
    size_t nmatch1 = 0;
    if (use_gnu_version) {
        void_preg = gnu_alloc_regex();
        errcode = gnu_regcomp(void_preg, regexp, rlen, test_data->cflags&REG_CFLAGS_MASK, test_data->on_syntax, test_data->off_syntax, &nmatch1);
        if (errcode!=0) {
            ret1 = -1;
        } else {
            nmatch1++;
            pmatch1 = calloc(nmatch1, sizeof(regmatch_t));
            if (test_data->eflags&REG_STARTEND) {
                pmatch1[0].rm_so = test_data->start;
                pmatch1[0].rm_eo = test_data->end;
            }
            ret1 = gnu_regexec(void_preg, text, tlen, nmatch1, pmatch1, test_data);
        }
    } else {
        errcode = pc_regcomp(&void_preg, regexp, rlen, test_data->cflags&REG_CFLAGS_MASK, test_data->on_syntax, test_data->off_syntax);
        if (errcode!=0) {
            ret1 = -1;
        } else {
            ret1 = pc_regexec(void_preg, text, tlen, &nmatch1, &pmatch1, test_data);
        }
    }

    //regexp
    reg_compile_t *preg_compile;
    regmatch_t *pmatch2 = NULL;
    size_t nmatch2 = 0;
    if (test_data->on_syntax || test_data->off_syntax) {
        int syntax = ((test_data->cflags & REG_EXTENDED) ? RE_SYNTAX_POSIX_EXTENDED : RE_SYNTAX_POSIX_BASIC);
        syntax |= test_data->on_syntax;
        syntax &= ~test_data->off_syntax;
        preg_compile = reg_compile2(regexp, rlen, &nmatch2, test_data->cflags&REG_CFLAGS_MASK, syntax);
    } else {
        preg_compile = reg_compile(regexp, rlen, &nmatch2, test_data->cflags&REG_CFLAGS_MASK);
    }
    if (preg_compile==NULL) {
        ret2 = -1;
    } else {
        nmatch2++;
        pmatch2 = calloc(nmatch2, sizeof(regmatch_t));
        if (test_data->eflags&REG_STARTEND) {
            pmatch2[0].rm_so = test_data->start;
            pmatch2[0].rm_eo = test_data->end;
        }
        ret2 = reg_exec(preg_compile, text, tlen, nmatch2, pmatch2, test_data->eflags);
    }

    int dump_flag = test_data->cflags&REG_DUMP;
    for (int i=0; i<test_data->nmatch; i++) {
        const char *bstr = test_data->match[i].bstr;
        int len = test_data->match[i].len;
        if (len==0) len = strlen(bstr);
        int so1 = pmatch1?pmatch1[i].rm_so:-9;
        int so2 = pmatch2?pmatch2[i].rm_so:-9;
        int eo1 = pmatch1?pmatch1[i].rm_eo:-9;
        int eo2 = pmatch2?pmatch2[i].rm_eo:-9;
        int match_sub = ret1<0 || ret2<0 ||
            (len==eo2-so2 && memcmp(test_data->text+so2, bstr, len)==0 && so1==so2 && eo1==eo2);
        if (ret1 != ret2 || ret1 != test_data->expect || !match_sub || nmatch1!=nmatch2) result = 0;
        if (result==0 || dump_flag) {
            printf("%s(%s%s): line=%d[%d]: ret=%d:%d text='",
                result==0?"ERROR":"DUMP", use_gnu_version?"GNU":"PCRE2", use_posix_version?":POSIX":"", n, i, ret1, ret2);
            reg_print_str(stdout, text, tlen);
            printf("', regexp='");
            reg_print_str(stdout, regexp, rlen);
            printf("', nmatch=%ld:%ld, cflags=%s, eflags=%s",
                nmatch1, nmatch2, reg_cflags2str(test_data->cflags),
                reg_eflags2str(test_data->eflags));
            if (test_data->on_syntax)  printf(", syntax+=%s", reg_syntax2str(test_data->on_syntax));
            if (test_data->off_syntax) printf(", syntax-=%s", reg_syntax2str(test_data->off_syntax));
            if (test_data->end) printf(", startend=%d,%d", test_data->start, test_data->end);
            printf("\n");
            if (1 || !match_sub || dump_flag) {
                printf("  pmatch1[%d,%d]='", so1, eo1);
                reg_print_str(stdout, text+so1, eo1-so1);
                printf("', pmatch2[%d,%d]='", so2, eo2);
                reg_print_str(stdout, text+so2, eo2-so2);
                printf("', expected_match='");
                reg_print_str(stdout, bstr, len);
                printf("'\n");
            }
        }
    }
    if (result==0 || dump_flag) {
        reg_dump(stdout, preg_compile, 2);
        if (use_gnu_version) gnu_dump(void_preg);
        else                 pc_dump(void_preg);
    }
    if (result==0 || (errcode>1 && errcode!=reg_err_info.err_code) || dump_flag ||
        (errcode && strncmp(posix_err_info.err_msg, reg_err_info.err_msg, 10)!=0)) {
        printf("%d: regex:  regexp='", n);
        reg_print_str(stdout, regexp, rlen);
        printf("', errcode=%d, %s\n", posix_err_info.err_code, posix_err_info.err_msg);
        for (int i=0; i<nmatch1; i++) {
            printf("    pmatch1[%d][%d,%d]='", i, pmatch1[i].rm_so, pmatch1[i].rm_eo);
            reg_print_str(stdout, text+pmatch1[i].rm_so, pmatch1[i].rm_eo-pmatch1[i].rm_so);
            printf("'\n");
        }
        printf("%d: regexp: regexp='", n);
        reg_print_str(stdout, regexp, rlen);
        printf("', errcode=%d, %s\n", reg_err_info.err_code, reg_err_info.err_msg);
        for (int i=0; i<nmatch2; i++) {
            printf("    pmatch2[%d][%d,%d]='", i, pmatch2[i].rm_so, pmatch2[i].rm_eo);
            reg_print_str(stdout, text+pmatch2[i].rm_so, pmatch2[i].rm_eo-pmatch2[i].rm_so);
            printf("'\n");
        }
    }

    free(pmatch1);
    free(pmatch2);
    if (use_gnu_version) gnu_free_regex(void_preg);
    else                 pc_free_regex(void_preg);
    reg_compile_free(preg_compile);
    preg_compile = NULL;
    return result;
}

static int test_bstr(void) {
    int cnt = 0;
    int err_cnt = 0;
    int bre_cnt = 0;
    int ere_cnt = 0;
    int posix_cnt = 0;  //POSIX版(regcomp/regexec)
    int gnu_cnt   = 0;  //GNU版(re_set_syntax/re_search)
    int pcre2_cnt = 0;  //PCRE2版

    int size = sizeof(bdata)/sizeof(test_bstr_t);
    for (int i=0; i<size; i++) {
        test_bstr_t *btest = &bdata[i];
        if (btest->no==0) break;
        cnt++;
        if (use_posix_version) posix_cnt++;
        else                   gnu_cnt++;
        int org_cflags = btest->cflags;
        btest->cflags &= REG_CFLAGS_MASK;
        if (org_cflags&REG_BRE) {
            use_gnu_version = 1;
            use_posix_version = 1;
            if (btest->tlen || btest->rlen) use_posix_version = 0;
            if (test_regexp(btest)==0) err_cnt++;
            bre_cnt++;
        }
        if (org_cflags&REG_ERE) {
            use_gnu_version = 1;
            use_posix_version = 1;
            if (btest->tlen || btest->rlen) use_posix_version = 0;
            btest->cflags |= REG_EXTENDED;
            if (test_regexp(btest)==0) err_cnt++;
            ere_cnt++;
        }
        if (org_cflags&REG_PCRE2) {
            use_gnu_version = 0;
            use_posix_version = 0;
            //if (test_regexp(btest)==0) err_cnt++;
            //pcre2_cnt++;
        }
    }
    printf("%s: %d/%d (BRE=%d, ERE=%d, skip=%d) (posix=%d, gnu=%d, pcre2=%d)\n",
        err_cnt?"FAIL":"PASS", err_cnt?err_cnt:cnt, cnt, bre_cnt, ere_cnt, size-cnt, posix_cnt, gnu_cnt, pcre2_cnt);
    return err_cnt;
}

static int test_misc(void) {
    reg_compile_t *preg_compile;
    size_t nsub;
    int ret;

    preg_compile = reg_compile(NULL, 0, NULL, 0);
    assert(preg_compile==NULL);
    assert(reg_err_info.err_code>0);
    assert(reg_err_info.err_msg!=NULL);

    preg_compile = reg_compile("(a)(b)", 6, &nsub, REG_EXTENDED);
    assert(preg_compile);
    assert(reg_err_info.err_code==0);
    assert(reg_err_info.err_msg!=NULL);
    assert(nsub==2);
    ret = reg_exec(preg_compile, "ab", 2, 0, /*pmatch=*/NULL, 0);
    assert(ret==0);
    reg_compile_free(preg_compile);

    printf("PASS: Misc Test\n");
    return 0;
}

int main(void) {
    setlocale(LC_ALL, "");          //環境変数に沿ってロケールを設定する
    setlocale(LC_MESSAGES, "C");    //メッセージは英語とする
    int cnt = 0;
    cnt += test_bstr();
    cnt += test_misc();
    return !(cnt==0);
}
