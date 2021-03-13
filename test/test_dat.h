    {"abc", "a", 1},
    {"abc", "b", 1},
    {"abc", "c", 1},
    {"abc", "d", 0},
    {"abc", "d", 0},
    {"abc", "ab", 1},
    {"abc", "bc", 1},
    {"abc", "ac", 0},
    {"abc", "cd", 0},
    {"abc", "abc", 1},
    {"abc", "", 1},

    {"abc", "^a", 1},
    {"abc", "a$", 0},
    {"abc", "^b", 0},
    {"abc", "b$", 0},
    {"abc", "c$", 1},
    {"abc", "^ab", 1},
    {"abc", "^bc", 0},
    {"abc", "ab$", 0},
    {"abc", "bc$", 1},
    {"abc", "^ab$", 0},
    {"abc", "^bc$", 0},
    {"abc", "^abc$", 1},

    {"abc", "*", 0},
    {"abc", "a*", 1},
    {"abc", "aa*", 1},
    {"abc", "ab*c", 1},
    {"abc", "b*", 1},
    {"abc", "c*", 1},
    {"abc", "d*", 1},
    {"abc", "^a*", 1},
    {"abc", "^b*", 1},
    {"abc", "^c*", 1},
    {"abc", "^d*", 1},
    {"abc", "a*$", 1},
    {"aaabbbccc", "aaaa*", 1},
    {"aaabbbccc", "ab*", 1},

    {"abc", ".", 1},
    {"abc", "..", 1},
    {"abc", "...", 1},
    {"abc", "....", 0},
    {"abc", "^...$", 1},
    {"abc", "a.c", 1},
    {"abc", "^a.c$", 1},
    {"abc", "a..c", 0},

    {"abc  xyz", "abc *xyz", 1},
    {"*", "*", 1},

    {"a*c", "\\*", 1},
    {"a*c", "\\*\\*", 0},
    {"a**c", "\\*\\*", 1},
    {"a**c", "^a\\*\\*c$", 1},
    {"a\\c", "\\\\", 1},
    {"a\\b\\c", "\\\\b\\\\", 1},
    {"a..c", "a\\.\\.c", 1},
    {"a$c", "a\\$", 1},
    {"a$c", "c\\$", 0},
    {"\\abc", "\\a", 1},
    {"^abc", "^a", 0},
    {"a^bc", "\\^b", 1},

    {"abc", "[a]", 1},
    {"abc", "a[ab]", 1},
    {"abc", "a[abc]c", 1},
    {"abc", "[^a]", 1},
    {"aaa", "[^a]", 0},
    {"abc", "[^abc]", 0},
    {"]", "[]]", 1},
    {"-", "[-]", 1},
    {"-", "[a-]", 1},
    {"abc", "[a-c]", 1},
    {"abc", "[c-a]", -1},    //Error
    {"abc", "[x-z]", 0},
    {"abc", "[0-9]", 0},
    {"123", "[0-9]", 1},
    {"abc/5", "[a-z]/[0-9]", 1},
    {"abc/z5", "[a-z]/[0-9]", 0},
