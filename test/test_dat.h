#define TEST_DATA(text,regexp,match,expect) {__LINE__,text,regexp,match,expect},
    TEST_DATA("abc", "a", "a", 0)
    TEST_DATA("abc", "b", "b", 0)
    TEST_DATA("abc", "c", "c", 0)
    TEST_DATA("abc", "d", "", 1)
    TEST_DATA("abc", "ab", "ab", 0)
    TEST_DATA("abc", "bc", "bc", 0)
    TEST_DATA("abc", "ac", "", 1)
    TEST_DATA("abc", "cd", "", 1)
    TEST_DATA("abc", "abc", "abc", 0)
    TEST_DATA("abc", "", "", 0)

    TEST_DATA("abc", "^a", "a", 0)
    TEST_DATA("abc", "a$", "", 1)
    TEST_DATA("abc", "^b", "", 1)
    TEST_DATA("abc", "b$", "", 1)
    TEST_DATA("abc", "c$", "c", 0)
    TEST_DATA("abc", "^ab", "ab", 0)
    TEST_DATA("abc", "^bc", "", 1)
    TEST_DATA("abc", "ab$", "", 1)
    TEST_DATA("abc", "bc$", "bc", 0)
    TEST_DATA("abc", "^ab$","",  1)
    TEST_DATA("abc", "^bc$", "", 1)
    TEST_DATA("abc", "^abc$", "abc", 0)

    TEST_DATA("abc", "*", "", 1)
    TEST_DATA("abc", "a*", "a", 0)
    TEST_DATA("abc", "aa*", "a", 0)
    TEST_DATA("aac", "aa*", "aa", 0)
    TEST_DATA("abc", "ab*c", "abc", 0)
    TEST_DATA("abc", "b*", "", 0)
    TEST_DATA("abc", "bc*", "bc", 0)
    TEST_DATA("abc", "d*", "", 0)
    TEST_DATA("abc", "^a*", "a", 0)
    TEST_DATA("abc", "^b*", "", 0)
    TEST_DATA("abc", "^c*", "", 0)
    TEST_DATA("abc", "^d*", "", 0)
    TEST_DATA("abc", "a*$", "", 0)
    TEST_DATA("aaabbbccc", "aaaa*", "aaa",0)
    TEST_DATA("aaabbbccc", "ab*", "a", 0)

    TEST_DATA("abc", ".", "a", 0)
    TEST_DATA("abc", "..", "ab", 0)
    TEST_DATA("abc", "...", "abc", 0)
    TEST_DATA("abc", "....", "", 1)
    TEST_DATA("abc", "^...$", "abc", 0)
    TEST_DATA("abc", "a.c", "abc", 0)
    TEST_DATA("abc", "^a.c$", "abc", 0)
    TEST_DATA("abc", "a..c", "", 1)

    TEST_DATA("abc  xyz", "abc *xyz", "abc  xyz", 0)
    TEST_DATA("*", "*", "*", 0)
    TEST_DATA("*", "^*", "*", 0)
    TEST_DATA("**", "**", "**", 0)

    TEST_DATA("a*c", "\\*", "*", 0)
    TEST_DATA("a*c", "\\*\\*", "", 1)
    TEST_DATA("a**c", "\\*\\*", "**", 0)
    TEST_DATA("a**c", "^a\\*\\*c$", "a**c", 0)
    TEST_DATA("a\\c", "\\\\", "\\", 0)
    TEST_DATA("a\\b\\c", "\\\\b\\\\", "\\b\\", 0)
    TEST_DATA("a..c", "a\\.\\.c", "a..c", 0)
    TEST_DATA("a$c", "a\\$", "a$", 0)
    TEST_DATA("a$c", "c\\$", "", 1)
    TEST_DATA("a$c", "a$c", "a$c", 0)               //BREでは$は最後だけ特殊な意味を持つ
    TEST_DATA("$bc", "$b", "$b", 0)                 //BREでは$は最後だけ特殊な意味を持つ
    TEST_DATA("\\abc", "\\a", "a", 0)
    TEST_DATA("^abc", "^a", "", 1)
    TEST_DATA("a^bc", "\\^b", "^b", 0)
    TEST_DATA("a^bc", "a^b", "a^b", 0)              //BREでは^は最初だけ特殊な意味を持つ
    TEST_DATA("abc", "a**", "", -1)
    TEST_DATA("abc", ".*", "abc", 0)
    TEST_DATA("abc", "a.*", "abc", 0)
    TEST_DATA("abc", "a.*c", "abc", 0)

    TEST_DATA("abc", "[a]", "a", 0)
    TEST_DATA("abc", "a[ab]", "ab", 0)
    TEST_DATA("abc", "a[abc]c", "abc", 0)
    TEST_DATA("abc", "[^a]", "b", 0)
    TEST_DATA("aaa", "[^a]", "", 1)
    TEST_DATA("abc", "[^abc]", "", 1)
    TEST_DATA("abc", "[^]", "", -1)
    TEST_DATA("^a", "[^^]", "a", 0)
    TEST_DATA("]", "[]]", "]", 0)
    TEST_DATA("-", "[-]", "-", 0)
    TEST_DATA("-", "[a-]", "-", 0)
    TEST_DATA("-]", "[]-]*", "-]", 0)
    TEST_DATA("abc", "[a-c]", "a", 0)
    TEST_DATA("abc", "[c-a]", "", -1)    //Error
    TEST_DATA("abc", "[a-a]", "a", 0)
    TEST_DATA("abc", "[x-z]", "", 1)
    TEST_DATA("abc", "[0-9]", "", 1)
    TEST_DATA("123", "[0-9]", "1", 0)
    TEST_DATA("abc/5", "[a-z]/[0-9]", "c/5", 0)
    TEST_DATA("abc/z5", "[a-z]/[0-9]", "", 1)

    TEST_DATA("abc", "[b]*", "", 0)
    TEST_DATA("abc", "[b]*c", "bc", 0)
    TEST_DATA("abc", "a[b]*c", "abc", 0)
    TEST_DATA("abbc", "a[b-c]*[a-b]*c", "abbc", 0)

    TEST_DATA("abc", "a\\{1\\}", "a", 0)
    TEST_DATA("abc", "a\\{1,\\}", "a", 0)
    TEST_DATA("abc", "a\\{2,\\}", "", 1)
    TEST_DATA("aabc", "a\\{2,\\}", "aa", 0)
    TEST_DATA("baaac", "a\\{2,\\}", "aaa", 0)
    TEST_DATA("abc", "\\{1\\}", "", -1)
    TEST_DATA("abc", "a\\{1b\\}", "", -1)
    TEST_DATA("abc", "a\\{1,b\\}", "", -1)
    TEST_DATA("abc", "a\\{1,2", "", -1)
    TEST_DATA("abc", "a\\{2,1\\}", "", -1)
    TEST_DATA("abc", "a\\{0\\}", "",0)
    TEST_DATA("abc", "a\\{0,0\\}", "", 0)
    TEST_DATA("abc", "a\\{,1\\}", "a", 0)
    TEST_DATA("abc", "\\}", "", 1)
    TEST_DATA("{",   "\\{", "", -1)
    TEST_DATA("abc", "a\\{0,32768\\}", "a", -1)
    TEST_DATA("abc", "a\\{32768\\}", "a", -1)

    TEST_DATA("abc", "[a-z]\\{3\\}", "abc", 0)
    TEST_DATA("abc", "[a-z]\\{4\\}", "", 1)
    TEST_DATA("abc", "[a-z]\\{3,4\\}", "abc", 0)
    TEST_DATA("abc", "[a-z]\\{4,5\\}", "", 1)
    TEST_DATA("abc", ".\\{3\\}", "abc", 0)
    TEST_DATA("abc", ".\\{4\\}", "", 1)

    TEST_DATA("abc", "\\(a\\)", "a", 0)
    TEST_DATA("abc", "\\(c\\)", "c", 0)
    TEST_DATA("abc", "\\(abc\\)", "abc", 0)
    TEST_DATA("abc", "^\\(a\\)", "a", 0)
    TEST_DATA("abc", "\\(c\\)$", "c", 0)
    TEST_DATA("abc", "^\\(abc\\)$", "abc", 0)
    TEST_DATA("abc", "\\(a", "", -1)
    TEST_DATA("abc", "\\([bc]\\)", "b", 0)
    TEST_DATA("abc", "a\\([bc]*\\)", "abc", 0)
    TEST_DATA("abc", "a\\([bc]\\)*", "abc", 0)
    TEST_DATA("abc", "\\(a\\)\\(bc\\)", "abc", 0)
    TEST_DATA("abc", "\\(...\\)", "abc", 0)
    TEST_DATA("abc", "\\([a-z]\\{1,3\\}\\)", "abc", 0)
    TEST_DATA("abc", "\\)", "", -1)
    TEST_DATA("abc", "\\(^a\\)b", "ab", 0)
    TEST_DATA("0abc", "\\(^a\\)b", "", 1)
    TEST_DATA("abc", "b\\(c$\\)", "bc", 0)
    TEST_DATA("abcd", "b\\(c$\\)", "", 1)
    TEST_DATA("abc", "\\(^abc$\\)", "abc", 0)
    TEST_DATA("abcd", "\\(^abc$\\)", "", 1)
    TEST_DATA("abcd", "\\(^abc$\\)d", "", 1)
    TEST_DATA("abc", "\\(^[abc]*$\\)", "abc", 0)
    TEST_DATA("abc", "\\(\\(abc\\)\\)", "abc", 0)
    TEST_DATA("abc", "\\(a\\(b\\)c\\)", "abc", 0)
    TEST_DATA("abcabc", "\\(a\\(b\\)c\\)*", "abcabc", 0)
    TEST_DATA("aba", "\\(\\)", "", 0)
    TEST_DATA("aba", "\\(a\\).\\1", "aba", 0)
    TEST_DATA("ab0", "\\0",         "0", 0)
    TEST_DATA("aba", "\\1",         "", -1)
    TEST_DATA("aba", "\\(a\\).\\9", "", -1)
    TEST_DATA("aba", "\\(a\\1\\).", "", -1)
 
    TEST_DATA("XYZ123_", "[[:upper:]]*", "XYZ", 0)
    TEST_DATA("abcXYZ123_", "[[:upper:][:lower:]]*", "abcXYZ", 0)
    TEST_DATA("abcXYZ123_", "[[:alpha:]]*", "abcXYZ", 0)
    TEST_DATA("abcXYZ123_", "[[:alnum:]]*", "abcXYZ123", 0)
//  TEST_DATA("abcXYZ123_", "[[:word:]]*", "abcXYZ123_", 0) //非POSIX
    TEST_DATA("x123ABCdefx", "x[[:digit:]]*", "x123", 0)
    TEST_DATA("x123ABCdefx", "x[[:xdigit:]]*", "x123ABCdef", 0)
    TEST_DATA("[:-+a]", "[[:punct:]]*", "[:-+", 0)
    TEST_DATA("\01a", "[[:cntrl:]]*", "\01", 0)
    TEST_DATA("abc \t", "[[:graph:]]*", "abc", 0)
    TEST_DATA("abc \t", "[[:print:]]*", "abc ", 0)

    TEST_DATA("abc", "[:alpha:]*", "a", 0)
    TEST_DATA("abc", "[[:Alpha:]]*", "", -1)
    TEST_DATA("abc", "[[:alpha]]*", "", -1)
