//test_str_t data[] = {
//   no,       text,        regexp,             pmatch,             nmatch,result,
    {__LINE__, "abc",       "a",                {"a"},              1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "b",                {"b"},              1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "c",                {"c"},              1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "d",                {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "abc",       "ab",               {"ab"},             1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "bc",               {"bc"},             1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "ac",               {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "abc",       "cd",               {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "abc",       "abc",              {"abc"},            1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "",                 {""},               1, 0, REG_BRE_ERE},

    {__LINE__, "abc",       "^a",               {"a"},              1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "a$",               {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "abc",       "^b",               {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "abc",       "b$",               {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "abc",       "c$",               {"c"},              1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "^ab",              {"ab"},             1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "^bc",              {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "abc",       "ab$",              {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "abc",       "bc$",              {"bc"},             1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "^ab$",             {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "abc",       "^bc$",             {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "abc",       "^abc$",            {"abc"},            1, 0, REG_BRE_ERE},
    {__LINE__, "^abc",      "^^a",              {"^a"},             1, 0, REG_BASIC},
    {__LINE__, "abc",       "^^a",              {"a"},              1, 0, REG_EXTENDED},
    {__LINE__, "abc$",      "c$$",              {"c$"},             1, 0, REG_BASIC},
    {__LINE__, "abc",       "c$$",              {"c"},              1, 0, REG_EXTENDED},

    {__LINE__, "abc",       "a*",               {"a"},              1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "aa*",              {"a"},              1, 0, REG_BRE_ERE},
    {__LINE__, "aac",       "aa*",              {"aa"},             1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "ab*c",             {"abc"},            1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "b*",               {""},               1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "bc*",              {"bc"},             1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "d*",               {""},               1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "^a*",              {"a"},              1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "^b*",              {""},               1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "^c*",              {""},               1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "^d*",              {""},               1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "a*$",              {""},               1, 0, REG_BRE_ERE},
    {__LINE__, "aaabbbccc", "aaaa*",            {"aaa"},            1, 0, REG_BRE_ERE},
    {__LINE__, "aaabbbccc", "ab*",              {"a"},              1, 0, REG_BRE_ERE},

    {__LINE__, "abc",       ".",                {"a"},              1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "..",               {"ab"},             1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "...",              {"abc"},            1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "....",             {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "abc",       "^...$",            {"abc"},            1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "a.c",              {"abc"},            1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "^a.c$",            {"abc"},            1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "a..c",             {""},               1, 1, REG_BRE_ERE},

    {__LINE__, "abc  xyz",  "abc *xyz",         {"abc  xyz"},       1, 0, REG_BRE_ERE},
    {__LINE__, "*",         "*",                {"*"},              1, 0, REG_BASIC},
    {__LINE__, "*",         "^*",               {"*"},              1, 0, REG_BASIC},
    {__LINE__, "**",        "**",               {"**"},             1, 0, REG_BASIC},
    {__LINE__, "*",         "*",                {""},               1,-1, REG_EXTENDED},
    {__LINE__, "*",         "^*",               {""},               1,-1, REG_EXTENDED},
    {__LINE__, "**",        "**",               {""},               1,-1, REG_EXTENDED},

    {__LINE__, "a*c",       "\\*",              {"*"},              1, 0, REG_BRE_ERE},
    {__LINE__, "a*c",       "\\*\\*",           {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "a**c",      "\\*\\*",           {"**"},             1, 0, REG_BRE_ERE},
    {__LINE__, "a**c",      "^a\\*\\*c$"        , {"a**c"},         1, 0, REG_BRE_ERE},
    {__LINE__, "a\\c",      "\\\\",             {"\\"},             1, 0, REG_BRE_ERE},
    {__LINE__, "a\\b\\c",   "\\\\b\\\\",        {"\\b\\"},          1, 0, REG_BRE_ERE},
    {__LINE__, "a..c",      "a\\.\\.c",         {"a..c"},           1, 0, REG_BRE_ERE},
    {__LINE__, "a$c",       "a\\$",             {"a$"},             1, 0, REG_BRE_ERE},
    {__LINE__, "a$c",       "c\\$",             {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "a$c",       "a$c",              {"a$c"},            1, 0, REG_BASIC},   //BREでは^,$は最初、最後だけ特殊な意味を持つ
    {__LINE__, "$bc",       "$b",               {"$b"},             1, 0, REG_BASIC},
    {__LINE__, "a$c",       "a$c",              {""},               1, 1, REG_EXTENDED},//EREでは~,$は常に特殊な意味を持つ
    {__LINE__, "$bc",       "$b",               {""},               1, 1, REG_EXTENDED},
    {__LINE__, "\\abc",     "\\a",              {"a"},              1, 0, REG_BRE_ERE},
    {__LINE__, "^abc",      "^a",               {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "a^bc",      "\\^b",             {"^b"},             1, 0, REG_BRE_ERE},
    {__LINE__, "a^bc",      "a^b",              {"a^b"},            1, 0, REG_BASIC},
    {__LINE__, "a^bc",      "a^b",              {""},               1, 1, REG_EXTENDED},
    {__LINE__, "abc",       "a**",              {""},               1,-1, REG_BASIC},
    {__LINE__, "abc",       "a**",              {"a"},              1, 0, REG_EXTENDED},//EREでは多重repeatが許される
    {__LINE__, "abc",       ".*",               {"abc"},            1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "a.*",              {"abc"},            1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "a.*c",             {"abc"},            1, 0, REG_BRE_ERE},

    {__LINE__, "abc",       "a\\?",             {"a"},              1, 0, REG_BASIC},
    {__LINE__, "abc",       "a\\+",             {"a"},              1, 0, REG_BASIC},
    {__LINE__, "abc",       "a\\?b",            {"ab"},             1, 0, REG_BASIC},
    {__LINE__, "abc",       "a\\+b",            {"ab"},             1, 0, REG_BASIC},
    {__LINE__, "abc",       "a?",               {"a"},              1, 0, REG_EXTENDED},
    {__LINE__, "abc",       "a+",               {"a"},              1, 0, REG_EXTENDED},
    {__LINE__, "abc",       "a?b",              {"ab"},             1, 0, REG_EXTENDED},
    {__LINE__, "abc",       "a+b",              {"ab"},             1, 0, REG_EXTENDED},
    {__LINE__, "a?+",       "?+",               {"?+"},             1, 0, REG_BASIC},
    {__LINE__, "a?+",       "\\?\\+",           {"?+"},             1, 0, REG_EXTENDED},

    {__LINE__, "abc",       "[a]",              {"a"},              1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "a[ab]",            {"ab"},             1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "a[abc]c",          {"abc"},            1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "[a",               {""},               1,-1, REG_BRE_ERE},
    {__LINE__, "abc",       "[^a]",             {"b"},              1, 0, REG_BRE_ERE},
    {__LINE__, "aaa",       "[^a]",             {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "abc",       "[^abc]",           {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "abc",       "[^]",              {""},               1,-1, REG_BRE_ERE},
    {__LINE__, "^a",        "[^^]",             {"a"},              1, 0, REG_BRE_ERE},
    {__LINE__, "]",         "[]]",              {"]"},              1, 0, REG_BRE_ERE},
    {__LINE__, "-",         "[-]",              {"-"},              1, 0, REG_BRE_ERE},
    {__LINE__, "-",         "[a-]",             {"-"},              1, 0, REG_BRE_ERE},
    {__LINE__, "-]",        "[]-]*",            {"-]"},             1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "[a-c]",            {"a"},              1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "[c-a]",            {""},               1,-1, REG_BRE_ERE},
    {__LINE__, "abc",       "[a-a]",            {"a"},              1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "[x-z]",            {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "abc",       "[0-9]",            {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "123",       "[0-9]",            {"1"},              1, 0, REG_BRE_ERE},
    {__LINE__, "abc/5",     "[a-z]/[0-9]",      {"c/5"},            1, 0, REG_BRE_ERE},
    {__LINE__, "abc/z5",    "[a-z]/[0-9]",      {""},               1, 1, REG_BRE_ERE},
    {__LINE__, "abc",       "[a-c-e]",          {""},               1,-1, REG_BRE_ERE},

    {__LINE__, "abc",       "[b]*",             {""},               1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "[b]*c",            {"bc"},             1, 0, REG_BRE_ERE},
    {__LINE__, "abc",       "a[b]*c",           {"abc"},            1, 0, REG_BRE_ERE},
    {__LINE__, "abbc",      "a[b-c]*[a-b]*c",   {"abbc"},           1, 0, REG_BRE_ERE},

    {__LINE__, "abc",       "a\\{1\\}",         {"a"},              1, 0, REG_BASIC},
    {__LINE__, "abc",       "a{1}",             {"a"},              1, 0, REG_EXTENDED},
    {__LINE__, "abc",       "a{1,}",            {"a"},              1, 0, REG_EXTENDED},
    {__LINE__, "abc",       "a{2,}",            {""},               1, 1, REG_EXTENDED},
    {__LINE__, "aabc",      "a{2,}",            {"aa"},             1, 0, REG_EXTENDED},
    {__LINE__, "baaac",     "a{2,}",            {"aaa"},            1, 0, REG_EXTENDED},
    {__LINE__, "abc",       "\\{1\\}",          {""},               1,-1, REG_BASIC},
    {__LINE__, "abc",       "{1}",              {""},               1,-1, REG_EXTENDED},
    {__LINE__, "abc",       "a{1b}",            {""},               1,-1, REG_EXTENDED},
    {__LINE__, "abc",       "a{1,b}",           {""},               1,-1, REG_EXTENDED},
    {__LINE__, "abc",       "a{1,2",            {""},               1,-1, REG_EXTENDED},
    {__LINE__, "abc",       "a{2,1}",           {""},               1,-1, REG_EXTENDED},
    {__LINE__, "abc",       "a{0}",             {""},               1, 0, REG_EXTENDED},
    {__LINE__, "abc",       "a{0,0}",           {""},               1, 0, REG_EXTENDED},
    {__LINE__, "abc",       "a{,1}",            {"a"},              1, 0, REG_EXTENDED},
    {__LINE__, "a}",        "\\}",              {"}"},              1, 0, REG_BASIC},
    {__LINE__, "{",         "\\{",              {""},               1,-1, REG_BASIC},
    {__LINE__, "{}",        "{}",               {"{}"},             1, 0, REG_BASIC},
    {__LINE__, "a}",        "}",                {"}"},              1, 0, REG_EXTENDED},
    {__LINE__, "{",         "{",                {""},               1,-1, REG_EXTENDED},
    {__LINE__, "{}",        "\\{\\}",           {"{}"},             1, 0, REG_EXTENDED},

    {__LINE__, "1234567",   "[0-9]\\{2\\}\\{3\\}",  {""},           1,-1, REG_BASIC},
    {__LINE__, "1234567",   "[0-9]{2}{3}",      {"123456"},         1, 0, REG_EXTENDED},//EREでは多重repeatが許される
    {__LINE__, "1234567",   "([0-9]{2}){3}",    {"123456","56"},    2, 0, REG_EXTENDED},
    {__LINE__, "1234567",   "[3-9]{2}+",        {"3456",},          1, 0, REG_EXTENDED},
    {__LINE__, "1234567",   "[3-9]{2}{3}",      {"",},              1, 1, REG_EXTENDED},
    {__LINE__, "aaaaa",     "a{2}+",            {"aaaa",},          1, 0, REG_EXTENDED},
    {__LINE__, "aaaaaaa",   "a{2,4}+",          {"aaaaaaa",},       1, 0, REG_EXTENDED},
    {__LINE__, "aaaaaaaa",  "^a{2,4}+$",        {"aaaaaaaa",},      1, 0, REG_EXTENDED},

    {__LINE__, "abc",       "a{0,32768}",       {"a"},              1,-1, REG_EXTENDED},
    {__LINE__, "abc",       "a{32768}",         {"a"},              1,-1, REG_EXTENDED},

    {__LINE__, "abc",       "[a-z]{3}",         {"abc"},            1, 0, REG_EXTENDED},
    {__LINE__, "abc",       "[a-z]{4}",         {""},               1, 1, REG_EXTENDED},
    {__LINE__, "abc",       "[a-z]{3,4}",       {"abc"},            1, 0, REG_EXTENDED},
    {__LINE__, "abc",       "[a-z]{4,5}",       {""},               1, 1, REG_EXTENDED},
    {__LINE__, "abc",       ".{3}",             {"abc"},            1, 0, REG_EXTENDED},
    {__LINE__, "abc",       ".{4}",             {""},               1, 1, REG_EXTENDED},

    {__LINE__, "abc",       "\\(a\\)",          {"a","a"},          2, 0, REG_BASIC},
    {__LINE__, "abc",       "(a)",              {"a","a"},          2, 0, REG_EXTENDED},
    {__LINE__, "abc",       "(c)",              {"c","c"},          2, 0, REG_EXTENDED},
    {__LINE__, "abc",       "(abc)",            {"abc","abc"},      2, 0, REG_EXTENDED},
    {__LINE__, "abc",       "^(a)",             {"a","a"},          2, 0, REG_EXTENDED},
    {__LINE__, "abc",       "(c)$",             {"c","c"},          2, 0, REG_EXTENDED},
    {__LINE__, "abc",       "^(abc)$",          {"abc","abc"},      2, 0, REG_EXTENDED},
    {__LINE__, "abc",       "([bc])",           {"b","b"},          2, 0, REG_EXTENDED},
    {__LINE__, "abc",       "a([bc]*)",         {"abc","bc"},       2, 0, REG_EXTENDED},
    {__LINE__, "abc",       "a(.)*",            {"abc","c"},        2, 0, REG_EXTENDED},
    {__LINE__, "abc",       "\\(a\\)\\(bc\\)",  {"abc","a","bc"},   3, 0, REG_BASIC},
    {__LINE__, "abc",       "(a)(bc)",          {"abc","a","bc"},   3, 0, REG_EXTENDED},
    {__LINE__, "abc",       "(a)(b)(c)",        {"abc","a","b","c"},4, 0, REG_EXTENDED},
    {__LINE__, "abc",       "(...)",            {"abc","abc"},      2, 0, REG_EXTENDED},
    {__LINE__, "abc",       "([a-z]{1,3})",     {"abc","abc"},      2, 0, REG_EXTENDED},
    {__LINE__, "a*c",       "\\(*\\)",          {"*"},              1, 0, REG_BASIC},
    {__LINE__, "a*c",       "(*)",              {""},               1,-1, REG_EXTENDED},
    {__LINE__, "a+c",       "\\(\\+\\)",        {"+"},              1, 0, REG_BASIC},
    {__LINE__, "a+c",       "(+)",              {""},               1,-1, REG_EXTENDED},
    {__LINE__, "a?c",       "\\(\\?\\)",        {"?"},              1, 0, REG_BASIC},
    {__LINE__, "a?c",       "(?)",              {""},               1,-1, REG_EXTENDED},
    {__LINE__, "*bc",       "\\(^*b\\)",        {"*b"},             1, 0, REG_BASIC},
    {__LINE__, "*bc",       "(^*b)",            {""},               1,-1, REG_EXTENDED},
    {__LINE__, "+bc",       "\\(^\\+b\\)",      {"+b"},             1, 0, REG_BASIC},
    {__LINE__, "+bc",       "(^+b)",            {""},               1,-1, REG_EXTENDED},
    {__LINE__, "?bc",       "\\(^\\?b\\)",      {"?b"},             1, 0, REG_BASIC},
    {__LINE__, "?bc",       "(^?b)",            {""},               1,-1, REG_EXTENDED},
    {__LINE__, "(a)",       "\\(a",             {""},               1,-1, REG_BASIC},
    {__LINE__, "(a)",       "a\\)",             {""},               1,-1, REG_BASIC},
    {__LINE__, "(a)",       "a)",               {"a)"},             1, 0, REG_EXTENDED},    //???
    {__LINE__, "(a)",       "(a",               {""},               1,-1, REG_EXTENDED},
    {__LINE__, "abc",       "\\(\\)",           {"",""},            2, 0, REG_BASIC},
    {__LINE__, "abc",       "()",               {"",""},            2, 0, REG_EXTENDED},

    {__LINE__, "abc",       "\\(^a\\)b",        {"ab","a"},         2, 0, REG_BASIC},
    {__LINE__, "0abc",      "\\(^a\\)b",        {""},               1, 1, REG_BASIC},
    {__LINE__, "abc",       "b\\(c$\\)",        {"bc","c"},         2, 0, REG_BASIC},
    {__LINE__, "abcd",      "b\\(c$\\)",        {""},               1, 1, REG_BASIC},
    {__LINE__, "abc",       "\\(^abc$\\)",      {"abc","abc"},      2, 0, REG_BASIC},
    {__LINE__, "abcd",      "\\(^abc$\\)",      {""},               1, 1, REG_BASIC},
    {__LINE__, "abcd",      "\\(^abc$\\)d",     {""},               1, 1, REG_BASIC},
    {__LINE__, "abc",       "\\(^[abc]*$\\)",   {"abc","abc"},      2, 0, REG_BASIC},
    {__LINE__, "abc",       "(^a)b",            {"ab","a"},         2, 0, REG_EXTENDED},
    {__LINE__, "0abc",      "(^a)b",            {""},               1, 1, REG_EXTENDED},
    {__LINE__, "abc",       "b(c$)",            {"bc","c"},         2, 0, REG_EXTENDED},
    {__LINE__, "abcd",      "b(c$)",            {""},               1, 1, REG_EXTENDED},
    {__LINE__, "abc",       "(^abc$)",          {"abc","abc"},      2, 0, REG_EXTENDED},
    {__LINE__, "abcd",      "(^abc$)",          {""},               1, 1, REG_EXTENDED},
    {__LINE__, "abcd",      "(^abc$)d",         {""},               1, 1, REG_EXTENDED},
    {__LINE__, "abc",       "(^[abc]*$)",       {"abc","abc"},      2, 0, REG_EXTENDED},

    {__LINE__, "abc",       "((abc))",          {"abc","abc","abc"},3, 0, REG_EXTENDED},
    {__LINE__, "abc",       "(a(b)c)",          {"abc","abc","b"},  3, 0, REG_EXTENDED},
    {__LINE__, "abcabc",    "(a(b)c)*",         {"abcabc","abc","b"},3, 0, REG_EXTENDED},
    {__LINE__, "aba",       "(a).\\1",          {"aba","a"},        2, 0, REG_EXTENDED},
    {__LINE__, "abcabc",    "a(bc)a\\1",        {"abcabc","bc"},    2, 0, REG_EXTENDED},
    {__LINE__, "abcabc",    "(a(bc))\\1",       {"abcabc","abc","bc"},3, 0, REG_EXTENDED},
    {__LINE__, "abcabc",    "(a)(b)c\\1\\2",    {"abcab","a","b"},  3, 0, REG_EXTENDED},
    {__LINE__, "abcabc",    "(...)\\1",         {"abcabc","abc"},   2, 0, REG_EXTENDED},
    {__LINE__, "<span>abc</span>",
                        "<(span)>.*</\\1>",     {"<span>abc</span>","span"},2, 0, REG_EXTENDED},
    {__LINE__, "aaabbbbc",  "([a-z])\\1{3}",    {"bbbb","b"},       2, 0, REG_EXTENDED},

    {__LINE__, "ab0",       "\\0",              {"0"},              1, 0, REG_BRE_ERE},
    {__LINE__, "aba",       "\\1",              {""},               1,-1, REG_BRE_ERE},
    {__LINE__, "aba",       "\\(a\\).\\9",      {""},               1,-1, REG_BASIC},
    {__LINE__, "aba",       "\\(a\\1\\).",      {""},               1,-1, REG_BASIC},
    {__LINE__, "aba",       "(a).\\9",          {""},               1,-1, REG_EXTENDED},
    {__LINE__, "aba",       "(a\\1).",          {""},               1,-1, REG_EXTENDED},

    {__LINE__, "a|b",       "a\\|b",            {"a"},              1, 0, REG_BASIC},
    {__LINE__, "a|b",       "a|b",              {"a|b"},            1, 0, REG_BASIC},
    {__LINE__, "a|b",       "a|b",              {"a"},              1, 0, REG_EXTENDED},
    {__LINE__, "a|b",       "a\\|b",            {"a|b"},            1, 0, REG_EXTENDED},
    {__LINE__, "abc",       "c|b",              {"b"},              1, 0, REG_EXTENDED},
    {__LINE__, "abc",       "ac|ab",            {"ab"},             1, 0, REG_EXTENDED},
    {__LINE__, "abc",       "(a|b)c",           {"bc","b"},         2, 0, REG_EXTENDED},
    {__LINE__, "abc",       "^(a|b)",           {"a","a"},          2, 0, REG_EXTENDED},
    {__LINE__, "abc",       "(^c|^b|^a)",       {"a","a"},          2, 0, REG_EXTENDED},
    {__LINE__, "abc",       "(a|b|c)$",         {"c","c"},          2, 0, REG_EXTENDED},
    {__LINE__, "abc",       "(a$|b$|c$)",       {"c","c"},          2, 0, REG_EXTENDED},
    {__LINE__, "abc123",    "[A-Z]+|[0-9]+",    {"123"},            1, 0, REG_EXTENDED},
    {__LINE__, "abc123",    "([A-Z]|[0-9])+",   {"123","3"},        2, 0, REG_EXTENDED},
    {__LINE__, "abc123",    "([A-Z]|[0-9]){4}", {""},               1, 1, REG_EXTENDED},
    {__LINE__, "abc",       "(a)(c|b)(b|c)",    {"abc","a","b","c"},4, 0, REG_EXTENDED},
    {__LINE__, "12345678910","(1)(2)(3)(4)(5)(6)(7)(8)(9)(10)",
        {"12345678910","1","2","3","4","5","6","7","8","9","10"},  11, 0, REG_EXTENDED},

    {__LINE__, "XYZ123_",    "[[:upper:]]*",      {"XYZ"},          1, 0, REG_BRE_ERE},
    {__LINE__, "abcXYZ123_", "[[:upper:][:lower:]]*", {"abcXYZ"},   1, 0, REG_BRE_ERE},
    {__LINE__, "abcXYZ123_", "[[:alpha:]]*",     {"abcXYZ"},        1, 0, REG_BRE_ERE},
    {__LINE__, "abcXYZ123_", "[[:alnum:]]*",     {"abcXYZ123"},     1, 0, REG_BRE_ERE},
//  {__LINE__, "abcXYZ123_", "[[:word:]]*",      {"abcXYZ123_"},    1, 0, REG_BRE_ERE}, //非POSIX
    {__LINE__, "x123ABCdefx","x[[:digit:]]*",    {"x123"},          1, 0, REG_BRE_ERE},
    {__LINE__, "x123ABCdefx","x[[:xdigit:]]*",   {"x123ABCdef"},    1, 0, REG_BRE_ERE},
    {__LINE__, "[:-+a]",     "[[:punct:]]*",     {"[:-+"},          1, 0, REG_BRE_ERE},
    {__LINE__, "\01a",       "[[:cntrl:]]*",     {"\01"},           1, 0, REG_BRE_ERE},
    {__LINE__, "abc \t",     "[[:graph:]]*",     {"abc"},           1, 0, REG_BRE_ERE},
    {__LINE__, "abc \t",     "[[:print:]]*",     {"abc "},          1, 0, REG_BRE_ERE},

    {__LINE__, "abc",        "[:alpha:]*",       {"a"},             1, 0, REG_BRE_ERE},
    {__LINE__, "abc",        "[[:Alpha:]]*",     {""},              1,-1, REG_BRE_ERE},
    {__LINE__, "abc",        "[[:alpha]]*",      {""},              1,-1, REG_BRE_ERE},
    {__LINE__, "abc",        "[[:alpha:]",       {""},              1,-1, REG_BRE_ERE},
    {__LINE__, "abc",        "[[:alpha:]-z]",    {""},              1,-1, REG_BRE_ERE},

    {__LINE__, "aBc",       "aBc",               {"aBc"},           1, 0, REG_BRE_ERE|REG_ICASE},
    {__LINE__, "aBc",       "Abc",               {"aBc"},           1, 0, REG_BRE_ERE|REG_ICASE},
    {__LINE__, "aBc",       "[a-z]*",            {"aBc"},           1, 0, REG_BRE_ERE|REG_ICASE},
    {__LINE__, "aBc",       "[^a-z]+",           {""},              1, 1, REG_BRE_ERE|REG_ICASE},
    {__LINE__, "aBc",       "[^A-Z]+",           {""},              1, 1, REG_BRE_ERE|REG_ICASE},
    {__LINE__, "aBc",       "[[:lower:]]*",      {"aBc"},           1, 0, REG_BRE_ERE|REG_ICASE},
    {__LINE__, "aBc",       "[[:alpha:]]*",      {"aBc"},           1, 0, REG_BRE_ERE|REG_ICASE},

    {__LINE__, "abc\n",     "c\n",               {"c\n"},           1, 0, REG_BRE_ERE},
    {__LINE__, "abc\n",     "c\n",               {"c\n"},           1, 0, REG_BRE_ERE|REG_NEWLINE},
    {__LINE__, "ab\nc",     "b\nc",              {"b\nc"},          1, 0, REG_BRE_ERE|REG_NEWLINE},
    {__LINE__, "\nabc",     "\na",               {"\na"},           1, 0, REG_BRE_ERE|REG_NEWLINE},
    {__LINE__, "ab\nxy",    "^x",                {""},              1, 1, REG_BRE_ERE},
    {__LINE__, "ab\nxy",    "^x",                {"x"},             1, 0, REG_BRE_ERE|REG_NEWLINE},
    {__LINE__, "ab\nxy",    "b^x",               {""},              1, 1, REG_BRE_ERE|REG_NEWLINE},
    {__LINE__, "\nxy",      "^x",                {"x"},             1, 0, REG_BRE_ERE|REG_NEWLINE},
    {__LINE__, "ab\nxy",    "b$",                {""},              1, 1, REG_BRE_ERE},
    {__LINE__, "ab\nxy",    "b$",                {"b"},             1, 0, REG_BRE_ERE|REG_NEWLINE},
    {__LINE__, "ab\nxy",    "b$x",               {""},              1, 1, REG_BRE_ERE|REG_NEWLINE},
    {__LINE__, "ab\nxy",    "b$^x",              {""},              1, 1, REG_BRE_ERE|REG_NEWLINE},
    {__LINE__, "a\nb",      "a$\n^b",            {""},              1, 1, REG_BASIC   |REG_NEWLINE},
    {__LINE__, "a\nb",      "a$\n^b",            {"a\nb"},          1, 0, REG_EXTENDED|REG_NEWLINE},
    {__LINE__, "a\nb",      "a$^b",              {""},              1, 1, REG_EXTENDED|REG_NEWLINE},

    {__LINE__, "ab\nxy",    "\\(^x\\)y",         {"xy","x"},        2, 0, REG_BASIC   |REG_NEWLINE},
    {__LINE__, "ab\nxy",    "(^x)y",             {"xy","x"},        2, 0, REG_EXTENDED|REG_NEWLINE},
    {__LINE__, "ab\nxy",    "\\(b$\\)\nx",       {"b\nx","b"},      2, 0, REG_BASIC   |REG_NEWLINE},
    {__LINE__, "ab\nxy",    "(b$)\nx",           {"b\nx","b"},      2, 0, REG_EXTENDED|REG_NEWLINE},
    {__LINE__, "ab\nxy",    "^(x)y",             {"xy","x"},        2, 0, REG_EXTENDED|REG_NEWLINE},
    {__LINE__, "ab\nxy",    "(b)$",              {"b","b"},         2, 0, REG_EXTENDED|REG_NEWLINE},
    {__LINE__, "ab\nxy",    "^^xy",              {"xy"},            1, 0, REG_EXTENDED|REG_NEWLINE},
    {__LINE__, "ab\nxy",    "ab$$",              {"ab"},            1, 0, REG_EXTENDED|REG_NEWLINE},

    {__LINE__, "xy",       "((^x)y)",            {"xy","xy","x"},   3, 0, REG_EXTENDED|REG_NEWLINE},
    {__LINE__, "\nxy",     "(^x)y",              {"xy","x"},        2, 0, REG_EXTENDED|REG_NEWLINE},
    {__LINE__, "\nxy",     "((^x)y)",            {"xy","xy","x"},   3, 0, REG_EXTENDED|REG_NEWLINE},

    {__LINE__, "ab\nxy",    "^xz|^xy",           {"xy"},            1, 0, REG_EXTENDED|REG_NEWLINE},
    {__LINE__, "ab\nxy",    "^(xz|xy)",          {"xy","xy"},       2, 0, REG_EXTENDED|REG_NEWLINE},
    {__LINE__, "ab\nxy",    "(^xy|z)",           {"xy","xy"},       2, 0, REG_EXTENDED|REG_NEWLINE},
    {__LINE__, "ab\nxyz",   "((^xy|z)z)$",       {"xyz","xyz","xy"},3, 0, REG_EXTENDED|REG_NEWLINE},

    {__LINE__, "ab\nxy",    "ab$|az$",           {"ab"},            1, 0, REG_EXTENDED|REG_NEWLINE},
    {__LINE__, "ab\nxy",    "(ab|az)$",          {"ab","ab"},       2, 0, REG_EXTENDED|REG_NEWLINE},
    {__LINE__, "ab\nxy",    "(ab$|az$)",         {"ab","ab"},       2, 0, REG_EXTENDED|REG_NEWLINE},
    {__LINE__, "0ab\nxy",   "^(0(ab$|az$))",     {"0ab","0ab","ab"},3, 0, REG_EXTENDED|REG_NEWLINE},

    {__LINE__, "ab\nxy",    "ab.",               {"ab\n"},          1, 0, REG_BRE_ERE},
    {__LINE__, "ab\nxy",    "ab.",               {""},              1, 1, REG_BRE_ERE|REG_NEWLINE}, //.は\nにマッチしない
    {__LINE__, "ab\nxy",    "ab[^a-z]",          {"ab\n"},          1, 0, REG_BRE_ERE},
    {__LINE__, "ab\nxy",    "ab[^a-z]",          {""},              1, 1, REG_BRE_ERE|REG_NEWLINE}, //[^...]は\nにマッチしない
    {__LINE__, "ab\nxy",    "ab[\n]",            {"ab\n"},          1, 0, REG_BRE_ERE|REG_NEWLINE},

    {__LINE__, "ab\nxy",    "^ab",               {"ab"},            1, 0, REG_BRE_ERE},
    {__LINE__, "ab\nxy",    "^ab",               {""},              1, 1, REG_BRE_ERE,REG_NOTBOL},  //文字列の開始を^にマッチさせない
    {__LINE__, "ab\nxy",    "^xy",               {"xy"},            1, 0, REG_BRE_ERE|REG_NEWLINE,REG_NOTBOL},
    {__LINE__, "ab\nxy",    "xy$",               {"xy"},            1, 0, REG_BRE_ERE},
    {__LINE__, "ab\nxy",    "xy$",               {""},              1, 1, REG_BRE_ERE,REG_NOTEOL},  //文字列の最後を$にマッチさせない
    {__LINE__, "ab\nxy",    "ab$",               {"ab"},            1, 0, REG_BRE_ERE|REG_NEWLINE,REG_NOTEOL},
//}
