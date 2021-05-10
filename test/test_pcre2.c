#include "test.h"

#ifdef REG_ENABLE_PCRE2

static uint32_t syntax_gnu2pcre2(int syntax) {
    uint32_t options = 0;
    if (syntax&RE_DOT_NEWLINE)           options |= PCRE2_DOTALL;           //. matches anything including NL
    return options;
}

//パターンをコンパイルする。
//成功時は0、異常時は0以外を返す。
int pc_regcomp(void **vpreg, const char *pattern, size_t len, int cflags, int on_syntax, int off_syntax) {
    (void)off_syntax;
    int ret = 0;
    pcre2_code_8 *pcode = NULL;
    uint32_t options = PCRE2_UTF    //Treat pattern and subjects as UTF strings
                      |PCRE2_UCP;   //Use Unicode properties for \d, \w, etc.
    if (cflags&REG_ICASE)   options |= PCRE2_CASELESS;  //Do caseless matching
    if (cflags&REG_NEWLINE) options |= PCRE2_MULTILINE; //^ and $ match newlines within data
    if (on_syntax)          options |=  syntax_gnu2pcre2(on_syntax);
//  if (off_syntax)         options &= ~syntax_gnu2pcre2(off_syntax);
    int errorcode;
    PCRE2_SIZE erroffset;
    pcode = pcre2_compile_8((PCRE2_SPTR8)pattern, len, options, &errorcode, &erroffset, NULL);
    *vpreg = pcode;
    if (pcode == NULL) {
        static PCRE2_UCHAR buf[256];
        pcre2_get_error_message(errorcode, buf, sizeof(buf));
        ref_err_info.err_code = errorcode;
        ref_err_info.err_msg = (const char*)buf;
        switch (errorcode) {    //PCRE2のエラーコードをregex相当に変換して返す。
            case 104: errorcode = 10; break;
            case 106: errorcode =  7; break;
            case 108: errorcode = 11; break;
            case 109: errorcode = 13; break;
            case 114:
            case 122: errorcode =  8; break;
            case 115: errorcode =  6; break;
            case 130: errorcode =  4; break;
            default: break;
        }
        ret = errorcode;
    }
    return ret;
}

int pc_regexec(void *preg, const char *string, size_t len, size_t *nmatch, regmatch_t **pmatch, test_bstr_t *test) {
    int ret = 0;
    pcre2_code_8 *pcode = preg;
    pcre2_match_data_8 *match_data;
    uint32_t options = 0;   //PCRE2_NO_UTF_CHECK;  //Do not check the subject for UTF validity
    if (test->eflags&REG_NOTBOL)  options |= PCRE2_NOTBOL; //Subject string is not the beginning of a line
    if (test->eflags&REG_NOTEOL)  options |= PCRE2_NOTEOL; //Subject string is not the end of a line
    PCRE2_SIZE startoffset = 0;
    if (test->eflags&REG_STARTEND) {
        startoffset = test->start;
        len = test->end;
    }
    match_data = pcre2_match_data_create_from_pattern(pcode, NULL);
    ret = pcre2_match_8(pcode, (PCRE2_SPTR8)string, len, startoffset, options, match_data, NULL);
    ref_err_info.err_code = ret;
    if (ret<0) {
        *nmatch = 1;
        *pmatch = calloc(*nmatch, sizeof(regmatch_t));
        ref_err_info.err_msg = "No match";
        ret = 1;
    } else {
        *nmatch = pcre2_get_ovector_count_8(match_data);
        //assert(ret==*nmatch);
        *pmatch = calloc(*nmatch, sizeof(regmatch_t));
        PCRE2_SIZE *ovector = pcre2_get_ovector_pointer(match_data);
        //ovectorをpmatchに変換する。
        for (int i=0; i<ret; i++) {
            (*pmatch)[i].rm_so = ovector[2*i];
            (*pmatch)[i].rm_eo = ovector[2*i+1];
        }
        ret = 0;
    }
    pcre2_match_data_free(match_data);
    return ret;
}

void pc_dump(pcre2_code_8 *pcode) {
    (void)pcode;
}

void pc_free_regex(pcre2_code_8 *pcode) {
    pcre2_code_free_8(pcode);
}

#endif //REG_ENABLE_PCRE2
