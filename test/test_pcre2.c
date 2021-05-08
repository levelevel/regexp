#include "test.h"

int pc_regcomp(void **vpreg, const char *pattern, size_t len, int cflags, int on_syntax, int off_syntax) {
    (void)cflags;
    (void)on_syntax;
    (void)off_syntax;
    int ret = 0;
    pcre2_code_8 *pcode = NULL;
    uint32_t options = 0;
    int errorcode;
    PCRE2_SIZE erroffset;
    pcode = pcre2_compile_8((PCRE2_SPTR8)pattern, len, options, &errorcode, &erroffset, NULL);
    *vpreg = pcode;
    if (pcode == NULL) {
        static PCRE2_UCHAR buf[256];
        pcre2_get_error_message(errorcode, buf, sizeof(buf));
        posix_err_info.err_code = errorcode;
        posix_err_info.err_msg = (const char*)buf;
        ret = 1;
    }
    return ret;
}

int pc_regexec(void *preg, const char *string, size_t len, size_t *nmatch, regmatch_t **pmatch, test_bstr_t *test) {
    (void)test;
    int ret = 0;
    pcre2_code_8 *pcode = preg;
    pcre2_match_data_8 *match_data;
    uint32_t options = 0;
    match_data = pcre2_match_data_create_from_pattern(pcode, NULL);
    ret = pcre2_match_8(pcode, (PCRE2_SPTR8)string, len, 0, options, match_data, NULL);
    posix_err_info.err_code = ret;
    if (ret<0) {
        *nmatch = 1;
        *pmatch = calloc(*nmatch, sizeof(regmatch_t));
        posix_err_info.err_msg = "No match";
        ret = 1;
    } else {
        *nmatch = pcre2_get_ovector_count_8(match_data);
        assert(ret==*nmatch);
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