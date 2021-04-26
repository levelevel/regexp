#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "locale.h"

#define __USE_GNU
#include <regex.h>

#include "../regexp.h"

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
#define REG_BASIC   0
#define REG_BRE_ERE 0x10000     //cflags:BER/ERB共通テスト項目（REG_EXTENDED有無両方でテストする）
#define REG_DUMP    0x20000     //cflags:強制的にダンプする
#define REG_CFLAGS_MASK 0xffff

test_bstr_t bdata[] = {
    #include "test_dat.h"
};

//リファレンス関数として何を使うかを指定する。
//1: POSIX版(regcomp/regexec)
//0: GNU版(re_set_syntax/re_search)
static int use_posix_version;

static int gnu_regcomp (regex_t *preg, const char *pattern, size_t len, int cflags, int on_syntax, int off_syntax);
static int gnu_regexec (regex_t *preg, const char *string, size_t len, size_t nmatch, regmatch_t pmatch[], test_bstr_t *test);
static void gnu_dump(regex_t *preg);
static reg_err_info_t gnu_err_info; //エラー情報

static int test_regexp(test_bstr_t *test_data) {
    int n = test_data->no;
    const char *text = test_data->text;
    const char *regexp = test_data->regexp;
    int tlen = test_data->tlen;
    int rlen = test_data->rlen;
    int errcode;
    int ret1, ret2; //1:一致、0:不一致、-1:エラー
    int result = 1; //1:OK, 0:NG
    if (tlen==0 && text)   tlen = strlen(text);
    if (rlen==0 && regexp) rlen = strlen(regexp);
    if ((test_data->eflags&REG_STARTEND) && test_data->end==0) test_data->end = tlen;

    //regex
    regex_t preg = {0};
    regmatch_t *pmatch1 = NULL;
    size_t nmatch1 = 0;
    errcode = gnu_regcomp(&preg, regexp, rlen, test_data->cflags&REG_CFLAGS_MASK, test_data->on_syntax, test_data->off_syntax);
    if (errcode!=0) {
        ret1 = -1;
    } else {
        nmatch1 = preg.re_nsub+1;
        pmatch1 = calloc(nmatch1, sizeof(regmatch_t));
        if (test_data->eflags&REG_STARTEND) {
            pmatch1[0].rm_so = test_data->start;
            pmatch1[0].rm_eo = test_data->end;
        }
        ret1 = gnu_regexec(&preg, text, tlen, nmatch1, pmatch1, test_data);
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
            printf("%s: line=%d[%d]: ret=%d:%d text='", result==0?"ERROR":"DUMP", n, i, ret1, ret2);
            reg_print_str(stdout, text, tlen);
            printf("', regexp='");
            reg_print_str(stdout, regexp, rlen);
            printf("', nmatch=%ld:%ld, cflags=%s, eflags=%s, use_posix=%d",
                nmatch1, nmatch2, reg_cflags2str(test_data->cflags),
                reg_eflags2str(test_data->eflags), use_posix_version);
            if (test_data->on_syntax)  printf(", syntax+=%s", reg_syntax2str(test_data->on_syntax));
            if (test_data->off_syntax) printf(", syntax-=%s", reg_syntax2str(test_data->off_syntax));
            printf(", startend=%d,%d", test_data->start, test_data->end);
            printf("\n");
            if (1 || !match_sub || dump_flag) {
                printf("  pmatch1=[%d,%d], pmatch2=[%d,%d], expected_match='", so1, eo1, so2, eo2);
                reg_print_str(stdout, bstr, len);
                printf("'\n");
            }
        }
    }
    if (result==0 || dump_flag) {
        reg_dump(stdout, preg_compile, 2);
        gnu_dump(&preg);
    }
    if (result==0 || (errcode>1 && errcode!=reg_err_info.err_code) || dump_flag ||
        (errcode && strncmp(gnu_err_info.err_msg, reg_err_info.err_msg, 10)!=0)) {
        printf("%d: regex:  regexp='", n);
        reg_print_str(stdout, regexp, rlen);
        printf("', errcode=%d, %s\n", gnu_err_info.err_code, gnu_err_info.err_msg);
        for (int i=0; i<nmatch1; i++) printf("    pmatch1[%d]=[%d,%d]\n", i, pmatch1[i].rm_so, pmatch1[i].rm_eo);
        printf("%d: regexp: regexp='", n);
        reg_print_str(stdout, regexp, rlen);
        printf("', errcode=%d, %s\n", reg_err_info.err_code, reg_err_info.err_msg);
        for (int i=0; i<nmatch2; i++) printf("    pmatch2[%d]=[%d,%d]\n", i, pmatch2[i].rm_so, pmatch2[i].rm_eo);
    }

    free(pmatch1);
    free(pmatch2);
    regfree(&preg);
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

    int size = sizeof(bdata)/sizeof(test_bstr_t);
    for (int i=0; i<size; i++) {
        test_bstr_t *btest = &bdata[i];
        if (btest->no==0) break;
        use_posix_version = 1;
        if (btest->tlen || btest->rlen) use_posix_version = 0;
        if (use_posix_version) posix_cnt++;
        else                   gnu_cnt++;
        cnt++;
        if (btest->cflags&REG_BRE_ERE) {
            btest->cflags &= ~(REG_EXTENDED|REG_BRE_ERE);
            if (test_regexp(btest)==0) err_cnt++;    //BREでテスト
            bre_cnt++;
            btest->cflags |= REG_EXTENDED;
            if (test_regexp(btest)==0) err_cnt++;    //EREでテスト
            ere_cnt++;
        } else {
            if (test_regexp(btest)==0) err_cnt++;
            if (btest->cflags&REG_EXTENDED) ere_cnt++;
            else                            bre_cnt++;
        }
    }
    printf("%s: %d/%d (BRE=%d, ERE=%d, skip=%d) (posix=%d, gnu=%d)\n",
        err_cnt?"FAIL":"PASS", err_cnt?err_cnt:cnt, cnt, bre_cnt, ere_cnt, size-cnt, posix_cnt, gnu_cnt);
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

static int gnu_regcomp(regex_t *preg, const char *pattern, size_t len, int cflags, int on_syntax, int off_syntax) {
    int ret = 0;
    static char buf[128];
    gnu_err_info.err_code = 0;
    gnu_err_info.err_msg = "";
    if (use_posix_version) {
        assert(pattern==NULL || strlen(pattern)==len);
        ret = regcomp(preg, pattern, cflags);
        gnu_err_info.err_code = ret;
        gnu_err_info.err_msg = buf;
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
            gnu_err_info.err_code = 1;
            gnu_err_info.err_msg = err_str;
            ret = 1;
        }

        //re_compile_patternでは無条件にnewline_anchor=1とするので再設定する。
        preg->newline_anchor = !!(cflags & REG_NEWLINE);
        //gnu_dump(preg);
    }
    return ret;
}
static int gnu_regexec(regex_t *preg, const char *string, size_t len, size_t nmatch, regmatch_t pmatch[], test_bstr_t *test) {
    int ret = 0;
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

static void gnu_dump(regex_t *preg) {
    printf("  newline_anchor=%d, not_bol=%d, not_eol=%d\n",
        preg->newline_anchor, preg->not_bol, preg->not_eol);
    printf("  syntax=%s\n", reg_syntax2str(preg->syntax));
    //printf("  syntax=%s\n", reg_syntax2str(re_syntax_options));
}