//test_t test_tbl[] = {
//   no.        text,tlen,       regexp,rlen,        {bstr,len},...         nmatch,expect,cflags,eflags,on_syntax,off_syntax,{start,end}
    {__LINE__, {"abc"},         {"a"},              {{"a"}},                1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"b"},              {{"b"}},                1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"c"},              {{"c"}},                1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"d"},              {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc"},         {"ab"},             {{"ab"}},               1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"bc"},             {{"bc"}},               1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"ac"},             {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc"},         {"cd"},             {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc"},         {"abc"},            {{"abc"}},              1, 0, REG_ALL},
    {__LINE__, {"abc"},         {""},               {{""}},                 1, 0, REG_ALL},
    {__LINE__, {""},            {""},               {{""}},                 1, 0, REG_ALL},

    {__LINE__, {"abc"},         {"^a"},             {{"a"}},                1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"a$"},             {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc"},         {"^b"},             {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc"},         {"b$"},             {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc"},         {"c$"},             {{"c"}},                1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"^ab"},            {{"ab"}},               1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"^bc"},            {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc"},         {"ab$"},            {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc"},         {"bc$"},            {{"bc"}},               1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"^ab$"},           {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc"},         {"^bc$"},           {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc"},         {"^abc$"},          {{"abc"}},              1, 0, REG_ALL},
    {__LINE__, {"^abc"},        {"^^a"},            {{"^a"}},               1, 0, REG_BRE},
    {__LINE__, {"abc"},         {"^^a"},            {{"a"}},                1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc$"},        {"c$$"},            {{"c$"}},               1, 0, REG_BRE},
    {__LINE__, {"abc"},         {"c$$"},            {{"c"}},                1, 0, REG_ERE_PCRE2},

    {__LINE__, {"abc"},         {"a*"},             {{"a"}},                1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"aa*"},            {{"a"}},                1, 0, REG_ALL},
    {__LINE__, {"aac"},         {"aa*"},            {{"aa"}},               1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"ab*c"},           {{"abc"}},              1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"b*"},             {{""}},                 1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"bc*"},            {{"bc"}},               1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"d*"},             {{""}},                 1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"^a*"},            {{"a"}},                1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"^b*"},            {{""}},                 1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"^c*"},            {{""}},                 1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"^d*"},            {{""}},                 1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"a*$"},            {{""}},                 1, 0, REG_ALL},
    {__LINE__, {"aaabbbccc"},   {"aaaa*"},          {{"aaa"}},              1, 0, REG_ALL},
    {__LINE__, {"aaabbbccc"},   {"ab*"},            {{"a"}},                1, 0, REG_ALL},

//  greedy
    {__LINE__, {"1aaa"},        {"1a*"},            {{"1aaa"}},             1, 0, REG_ALL},
    {__LINE__, {"1aaa"},        {"1a+"},            {{"1aaa"}},             1, 0, REG_ERE_PCRE2},
    {__LINE__, {"1aaa"},        {"1a?"},            {{"1a"}},               1, 0, REG_ERE_PCRE2},
    {__LINE__, {"1aaa"},        {"1a{1,2}"},        {{"1aa"}},              1, 0, REG_ERE_PCRE2},
    {__LINE__, {"aaa1"},        {"a*1"},            {{"aaa1"}},             1, 0, REG_ALL},
    {__LINE__, {"aaa1"},        {"a+1"},            {{"aaa1"}},             1, 0, REG_ERE_PCRE2},
    {__LINE__, {"aaa1"},        {"a?1"},            {{"a1"}},               1, 0, REG_ERE_PCRE2},
    {__LINE__, {"aaa1"},        {"a{1,2}1"},        {{"aa1"}},              1, 0, REG_ERE_PCRE2},
    {__LINE__, {"cat!"},        {".*!"},            {{"cat!"}},             1, 0, REG_ERE_PCRE2},
//  lazy
    {__LINE__, {"1aaa"},        {"1a*?"},           {{"1"}},                1, 0, REG_PCRE2},
    {__LINE__, {"1aaa"},        {"1a+?"},           {{"1a"}},               1, 0, REG_PCRE2},
    {__LINE__, {"1aaa"},        {"1a??"},           {{"1"}},                1, 0, REG_PCRE2},
    {__LINE__, {"1aaa"},        {"1a{1,2}?"},       {{"1a"}},               1, 0, REG_PCRE2},
    {__LINE__, {"aaa1"},        {"a*?1"},           {{"aaa1"}},             1, 0, REG_PCRE2},   //こういうもの？
    {__LINE__, {"aaa1"},        {"a+?1"},           {{"aaa1"}},             1, 0, REG_PCRE2},
    {__LINE__, {"aaa1"},        {"a??1"},           {{"a1"}},               1, 0, REG_PCRE2},
    {__LINE__, {"aaa1"},        {"a{1,2}?1"},       {{"aa1"}},              1, 0, REG_PCRE2},
    {__LINE__, {"cat!"},        {".*?!"},           {{"cat!"}},             1, 0, REG_PCRE2},
//  possessive
    {__LINE__, {"1aaa"},        {"1a*+"},           {{"1aaa"}},             1, 0, REG_PCRE2},
    {__LINE__, {"1aaa"},        {"1a++"},           {{"1aaa"}},             1, 0, REG_PCRE2},
    {__LINE__, {"1aaa"},        {"1a?+"},           {{"1a"}},               1, 0, REG_PCRE2},
    {__LINE__, {"1aaa"},        {"1a{1,2}+"},       {{"1aa"}},              1, 0, REG_PCRE2},
    {__LINE__, {"aaa1"},        {"a*+1"},           {{"aaa1"}},             1, 0, REG_PCRE2},
    {__LINE__, {"aaa1"},        {"a++1"},           {{"aaa1"}},             1, 0, REG_PCRE2},
    {__LINE__, {"aaa1"},        {"a?+1"},           {{"a1"}},               1, 0, REG_PCRE2},
    {__LINE__, {"aaa1"},        {"a{1,2}+1"},       {{"aa1"}},              1, 0, REG_PCRE2},
    {__LINE__, {"cat!"},        {".*+!"},           {{""}},                 1, 1, REG_PCRE2},

    {__LINE__, {"abc"},         {"."},              {{"a"}},                1, 0, REG_ALL},
    {__LINE__, {"abc"},         {".."},             {{"ab"}},               1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"..."},            {{"abc"}},              1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"...."},           {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc"},         {"^...$"},          {{"abc"}},              1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"a.c"},            {{"abc"}},              1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"^a.c$"},          {{"abc"}},              1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"a..c"},           {{""}},                 1, 1, REG_ALL},

    {__LINE__, {"abc  xyz"},    {"abc *xyz"},       {{"abc  xyz"}},         1, 0, REG_ALL},
    {__LINE__, {"*"},           {"*"},              {{"*"}},                1, 0, REG_BRE},
    {__LINE__, {"*"},           {"^*"},             {{"*"}},                1, 0, REG_BRE},
    {__LINE__, {"**"},          {"**"},             {{"**"}},               1, 0, REG_BRE},
    {__LINE__, {"*"},           {"*"},              {{""}},                 1,-1, REG_ERE_PCRE2},
    {__LINE__, {"*"},           {"^*"},             {{""}},                 1,-1, REG_ERE_PCRE2},
    {__LINE__, {"**"},          {"**"},             {{""}},                 1,-1, REG_ERE_PCRE2},

    {__LINE__, {"a*c"},         {"\\*"},            {{"*"}},                1, 0, REG_ALL},
    {__LINE__, {"a*c"},         {"\\*\\*"},         {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"a**c"},        {"\\*\\*"},         {{"**"}},               1, 0, REG_ALL},
    {__LINE__, {"a**c"},        {"^a\\*\\*c$"},     {{"a**c"}},             1, 0, REG_ALL},
    {__LINE__, {"a\\c"},        {"\\\\"},           {{"\\"}},               1, 0, REG_ALL},
    {__LINE__, {"a\\b\\c"},     {"\\\\b\\\\"},      {{"\\b\\"}},            1, 0, REG_ALL},
    {__LINE__, {"a..c"},        {"a\\.\\.c"},       {{"a..c"}},             1, 0, REG_ALL},
    {__LINE__, {"a$c"},         {"a\\$"},           {{"a$"}},               1, 0, REG_ALL},
    {__LINE__, {"a$c"},         {"c\\$"},           {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"a$c"},         {"a$c"},            {{"a$c"}},              1, 0, REG_BRE}, //BREでは^,$は最初、最後だけ特殊な意味を持つ
    {__LINE__, {"$bc"},         {"$b"},             {{"$b"}},               1, 0, REG_BRE},
    {__LINE__, {"a$c"},         {"a$c"},            {{""}},                 1, 1, REG_ERE_PCRE2}, //EREでは~,$は常に特殊な意味を持つ
    {__LINE__, {"$bc"},         {"$b"},             {{""}},                 1, 1, REG_ERE_PCRE2},
    {__LINE__, {"\\abc"},       {"\\a"},            {{"a"}},                1, 0, REG_GNU},
    {__LINE__, {"\\abc"},       {"\\i"},            {{""}},                 1,-1, REG_PCRE2},   //PCRE2では不正なエスケープ指定はエラーになる
    {__LINE__, {"^abc"},        {"^a"},             {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"a^bc"},        {"\\^b"},           {{"^b"}},               1, 0, REG_ALL},
    {__LINE__, {"a^bc"},        {"a^b"},            {{"a^b"}},              1, 0, REG_BRE},
    {__LINE__, {"a^bc"},        {"a^b"},            {{""}},                 1, 1, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"a**"},            {{""}},                 1,-1, REG_BRE},
    {__LINE__, {"abc"},         {"a**"},            {{"a"}},                1, 0, REG_ERE},     //EREでは多重repeatが許される
    {__LINE__, {"abc"},         {"a**"},            {{""}},                 1,-1, REG_PCRE2},   //PCRE2ではエラー
    {__LINE__, {"abc"},         {".*"},             {{"abc"}},              1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"a.*"},            {{"abc"}},              1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"a.*c"},           {{"abc"}},              1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"a\\?"},           {{"a"}},                1, 0, REG_BRE},
    {__LINE__, {"abc"},         {"a\\+"},           {{"a"}},                1, 0, REG_BRE},
    {__LINE__, {"abc"},         {"a\\?b"},          {{"ab"}},               1, 0, REG_BRE},
    {__LINE__, {"abc"},         {"a\\+b"},          {{"ab"}},               1, 0, REG_BRE},
    {__LINE__, {"abc"},         {"a?"},             {{"a"}},                1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"a+"},             {{"a"}},                1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"a?b"},            {{"ab"}},               1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"a+b"},            {{"ab"}},               1, 0, REG_ERE_PCRE2},
    {__LINE__, {"a?+"},         {"?+"},             {{"?+"}},               1, 0, REG_BRE},
    {__LINE__, {"a?+"},         {"\\?\\+"},         {{"?+"}},               1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"?"},              {{""}},                 1,-1, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"+"},              {{""}},                 1,-1, REG_ERE_PCRE2},

    {__LINE__, {"abc"},         {"[a]"},            {{"a"}},                1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"a[ab]"},          {{"ab"}},               1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"a[abc]c"},        {{"abc"}},              1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"[^a]"},           {{"b"}},                1, 0, REG_ALL},
    {__LINE__, {"aaa"},         {"[^a]"},           {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc"},         {"[^abc]"},         {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc"},         {"["},              {{""}},                 1,-1, REG_ALL},
    {__LINE__, {"abc"},         {"[a"},             {{""}},                 1,-1, REG_ALL},
    {__LINE__, {"abc"},         {"[^"},             {{""}},                 1,-1, REG_ALL},
    {__LINE__, {"abc"},         {"[^a"},            {{""}},                 1,-1, REG_ALL},
    {__LINE__, {"abc"},         {"[^]"},            {{""}},                 1,-1, REG_ALL},
    {__LINE__, {"^a"},          {"[^^]"},           {{"a"}},                1, 0, REG_ALL},
    {__LINE__, {"]"},           {"[]]"},            {{"]"}},                1, 0, REG_ALL},
    {__LINE__, {"-"},           {"[-]"},            {{"-"}},                1, 0, REG_ALL},
    {__LINE__, {"-"},           {"[a-]"},           {{"-"}},                1, 0, REG_ALL},
    {__LINE__, {"-]"},          {"[]-]*"},          {{"-]"}},               1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"[a-c]"},          {{"a"}},                1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"[c-a]"},          {{""}},                 1,-1, REG_ALL},
    {__LINE__, {"abc"},         {"[a-a]"},          {{"a"}},                1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"[x-z]"},          {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc"},         {"[0-9]"},          {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"123"},         {"[0-9]"},          {{"1"}},                1, 0, REG_ALL},
    {__LINE__, {"abc/5"},       {"[a-z]/[0-9]"},    {{"c/5"}},              1, 0, REG_ALL},
    {__LINE__, {"abc/z5"},      {"[a-z]/[0-9]"},    {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc"},         {"[a-c-e]"},        {{""}},                 1,-1, REG_GNU},
    {__LINE__, {"a-ed"},        {"[a-c-e]*"},       {{"a-e"}},              1, 0, REG_PCRE2},

    {__LINE__, {"abc"},         {"[b]*"},           {{""}},                 1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"[b]*c"},          {{"bc"}},               1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"a[b]*c"},         {{"abc"}},              1, 0, REG_ALL},
    {__LINE__, {"abbc"},        {"a[b-c]*[a-b]*c"}, {{"abbc"}},             1, 0, REG_ALL},

    {__LINE__, {"abc"},         {"a\\{1\\}"},       {{"a"}},                1, 0, REG_BRE},
    {__LINE__, {"abc"},         {"a{1}"},           {{"a"}},                1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"a{1,}"},          {{"a"}},                1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"a{2,}"},          {{""}},                 1, 1, REG_ERE_PCRE2},
    {__LINE__, {"aabc"},        {"a{2,}"},          {{"aa"}},               1, 0, REG_ERE_PCRE2},
    {__LINE__, {"baaac"},       {"a{2,}"},          {{"aaa"}},              1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"\\{1\\}"},        {{""}},                 1,-1, REG_BRE},
    {__LINE__, {"abc"},         {"{1}"},            {{""}},                 1,-1, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"a{1b}"},          {{""}},                 1,-1, REG_ERE},     //EREは不正な構文の{m,n}はエラーとなる
    {__LINE__, {"a{1b}"},       {"a{1b}"},          {{"a{1b}"}},            1, 0, REG_PCRE2},   //PCRE2は不正な構文の{m,n}はリテラルとなる
    {__LINE__, {"abc"},         {"a{1,b}"},         {{""}},                 1,-1, REG_ERE},
    {__LINE__, {"a{1,b}"},      {"a{1,b}"},         {{"a{1,b}"}},           1, 0, REG_PCRE2},
    {__LINE__, {"abc"},         {"a{1,2"},          {{""}},                 1,-1, REG_ERE},
    {__LINE__, {"a{1,2"},       {"a{1,2"},          {{"a{1,2"}},            1, 0, REG_PCRE2},
    {__LINE__, {"abc"},         {"a{2,1}"},         {{""}},                 1,-1, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"a{0}"},           {{""}},                 1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"a{0,0}"},         {{""}},                 1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"a{,1}"},          {{"a"}},                1, 0, REG_ERE},
    {__LINE__, {"a{,1}"},       {"a{,1}"},          {{"a{,1}"}},            1, 0, REG_PCRE2},
    {__LINE__, {"a}"},          {"\\}"},            {{"}"}},                1, 0, REG_BRE},
    {__LINE__, {"{"},           {"\\{"},            {{""}},                 1,-1, REG_BRE},
    {__LINE__, {"{}"},          {"{}"},             {{"{}"}},               1, 0, REG_BRE},
    {__LINE__, {"a}"},          {"}"},              {{"}"}},                1, 0, REG_ERE_PCRE2},
    {__LINE__, {"{"},           {"{"},              {{""}},                 1,-1, REG_ERE},
    {__LINE__, {"{"},           {"{"},              {{"{"}},                1, 0, REG_PCRE2},
    {__LINE__, {"{}"},          {"\\{\\}"},         {{"{}"}},               1, 0, REG_ERE_PCRE2},

    {__LINE__, {"1234567"},     {"[0-9]\\{2\\}\\{3\\}"},  {{""}},           1,-1, REG_BRE},
    {__LINE__, {"1234567"},     {"[0-9]{2}{3}"},    {{"123456"}},           1, 0, REG_ERE},     //EREでは多重repeatが許される
    {__LINE__, {"1234567"},     {"[0-9]{2}{3}"},    {{""}},                 1,-1, REG_PCRE2},   //PCRE2ではエラー
    {__LINE__, {"1234567"},     {"([0-9]{2}){3}"},  {{"123456"},{"56"}},    2, 0, REG_ERE_PCRE2},
    {__LINE__, {"1234567"},     {"[3-9]{2}+"},      {{"3456"}},             1, 0, REG_ERE},     //EREでは{}+は多重repeat
//  {__LINE__, {"1234567"},     {"[3-9]{2}+"},      {{"34"}},               1, 0, REG_PCRE2},   //PCRE2では{}+は単独repeat
    {__LINE__, {"1234567"},     {"[3-9]{2}{3}"},    {{""}},                 1, 1, REG_ERE},
    {__LINE__, {"1234567"},     {"[3-9]{2}{3}"},    {{""}},                 1,-1, REG_PCRE2},
    {__LINE__, {"aaaaa"},       {"a{2}+"},          {{"aaaa"}},             1, 0, REG_ERE},     //EREでは{}+は多重repeat
//  {__LINE__, {"aaaaa"},       {"a{2}+"},          {{"aa"}},               1, 0, REG_PCRE2},   //PCRE2では{}+は単独repeat
    {__LINE__, {"aaaaaaa"},     {"a{2,4}+"},        {{"aaaaaaa"}},          1, 0, REG_ERE},
    {__LINE__, {"aaaaaaaa"},    {"^a{2,4}+$"},      {{"aaaaaaaa"}},         1, 0, REG_ERE},

    {__LINE__, {"abc"},         {"a{0,32768}"},     {{"a"}},                1,-1, REG_ERE},     //EREでは繰り返しの上限は0x7fff
    {__LINE__, {"abc"},         {"a{32768}"},       {{"a"}},                1,-1, REG_ERE},

    {__LINE__, {"abc"},         {"[a-z]{3}"},       {{"abc"}},              1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"[a-z]{4}"},       {{""}},                 1, 1, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"[a-z]{3,4}"},     {{"abc"}},              1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"[a-z]{4,5}"},     {{""}},                 1, 1, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {".{3}"},           {{"abc"}},              1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {".{4}"},           {{""}},                 1, 1, REG_ERE_PCRE2},

//   no.        text,tlen,       regexp,rlen,        {bstr,len},...         nmatch,expect,cflags,eflags,on_syntax,off_syntax,{start,end}
    {__LINE__, {"abc"},         {"\\(a\\)"},        {{"a"},{"a"}},          2, 0, REG_BRE},
    {__LINE__, {"abc"},         {"(a)"},            {{"a"},{"a"}},          2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"(c)"},            {{"c"},{"c"}},          2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"(abc)"},          {{"abc"},{"abc"}},      2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"^(a)"},           {{"a"},{"a"}},          2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"(c)$"},           {{"c"},{"c"}},          2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"^(abc)$"},        {{"abc"},{"abc"}},      2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"([bc])"},         {{"b"},{"b"}},          2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"a([bc]*)"},       {{"abc"},{"bc"}},       2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"a(.)*"},          {{"abc"},{"c"}},        2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"\\(a\\)\\(bc\\)"},{{"abc"},{"a"},{"bc"}}, 3, 0, REG_BRE},
    {__LINE__, {"abc"},         {"(a)(bc)"},        {{"abc"},{"a"},{"bc"}}, 3, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"(a)(b)(c)"},      {{"abc"},{"a"},{"b"},{"c"}},4, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"(...)"},          {{"abc"},{"abc"}},      2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"([a-z]{1,3})"},   {{"abc"},{"abc"}},      2, 0, REG_ERE_PCRE2},
    {__LINE__, {"a*c"},         {"\\(*\\)"},        {{"*"}},                1, 0, REG_BRE},
    {__LINE__, {"a*c"},         {"(*)"},            {{""}},                 1,-1, REG_ERE_PCRE2},
    {__LINE__, {"a+c"},         {"\\(\\+\\)"},      {{"+"}},                1, 0, REG_BRE},
    {__LINE__, {"a+c"},         {"(+)"},            {{""}},                 1,-1, REG_ERE_PCRE2},
    {__LINE__, {"a?c"},         {"\\(\\?\\)"},      {{"?"}},                1, 0, REG_BRE},
    {__LINE__, {"a?c"},         {"(?)"},            {{""}},                 1,-1, REG_ERE},
    {__LINE__, {"*bc"},         {"\\(^*b\\)"},      {{"*b"}},               1, 0, REG_BRE},
    {__LINE__, {"*bc"},         {"(^*b)"},          {{""}},                 1,-1, REG_ERE_PCRE2},
    {__LINE__, {"+bc"},         {"\\(^\\+b\\)"},    {{"+b"}},               1, 0, REG_BRE},
    {__LINE__, {"+bc"},         {"(^+b)"},          {{""}},                 1,-1, REG_ERE_PCRE2},
    {__LINE__, {"?bc"},         {"\\(^\\?b\\)"},    {{"?b"}},               1, 0, REG_BRE},
    {__LINE__, {"?bc"},         {"(^?b)"},          {{""}},                 1,-1, REG_ERE_PCRE2},
    {__LINE__, {"(a)"},         {"\\(a"},           {{""}},                 1,-1, REG_BRE},
    {__LINE__, {"(a)"},         {"a\\)"},           {{""}},                 1,-1, REG_BRE},
    {__LINE__, {"(a)"},         {"a)"},             {{"a)"}},               1, 0, REG_ERE},     //ERE:RE_UNMATCHED_RIGHT_PAREN_ORD
    {__LINE__, {"(a)"},         {"a)"},             {{"a)"}},               1,-1, REG_PCRE2},
    {__LINE__, {"(a)"},         {"(a"},             {{""}},                 1,-1, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"\\(\\)"},         {{""},{""}},            2, 0, REG_BRE},
    {__LINE__, {"abc"},         {"()"},             {{""},{""}},            2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"("},              {{""}},                 1,-1, REG_ERE_PCRE2},

    {__LINE__, {"abc"},         {"\\(^a\\)b"},      {{"ab"},{"a"}},         2, 0, REG_BRE},
    {__LINE__, {"0abc"},        {"\\(^a\\)b"},      {{""}},                 1, 1, REG_BRE},
    {__LINE__, {"abc"},         {"b\\(c$\\)"},      {{"bc"},{"c"}},         2, 0, REG_BRE},
    {__LINE__, {"abcd"},        {"b\\(c$\\)"},      {{""}},                 1, 1, REG_BRE},
    {__LINE__, {"abc"},         {"\\(^abc$\\)"},    {{"abc"},{"abc"}},      2, 0, REG_BRE},
    {__LINE__, {"abcd"},        {"\\(^abc$\\)"},    {{""}},                 1, 1, REG_BRE},
    {__LINE__, {"abcd"},        {"\\(^abc$\\)d"},   {{""}},                 1, 1, REG_BRE},
    {__LINE__, {"abc"},         {"\\(^[abc]*$\\)"}, {{"abc"},{"abc"}},      2, 0, REG_BRE},
    {__LINE__, {"abc"},         {"(^a)b"},          {{"ab"},{"a"}},         2, 0, REG_ERE_PCRE2},
    {__LINE__, {"0abc"},        {"(^a)b"},          {{""}},                 1, 1, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"b(c$)"},          {{"bc"},{"c"}},         2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abcd"},        {"b(c$)"},          {{""}},                 1, 1, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"(^abc$)"},        {{"abc"},{"abc"}},      2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abcd"},        {"(^abc$)"},        {{""}},                 1, 1, REG_ERE_PCRE2},
    {__LINE__, {"abcd"},        {"(^abc$)d"},       {{""}},                 1, 1, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"(^[abc]*$)"},     {{"abc"},{"abc"}},      2, 0, REG_ERE_PCRE2},

    {__LINE__, {"abc"},         {"((abc))"},        {{"abc"},{"abc"},{"abc"}},      3, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"(a(b)c)"},        {{"abc"},{"abc"},{"b"}},        3, 0, REG_ERE_PCRE2},
    {__LINE__, {"abcabc"},      {"(a(b)c)*"},       {{"abcabc"},{"abc"},{"b"}},     3, 0, REG_ERE_PCRE2},
    {__LINE__, {"aba"},         {"(a).\\1"},        {{"aba"},{"a"}},                2, 0, REG_ERE_PCRE2},
    {__LINE__, {"aba"},         {"((a).\\2)"},      {{"aba"},{"aba"},{"a"}},        3, 0, REG_ERE_PCRE2},
    {__LINE__, {"aba"},         {"((a).)\\2"},      {{"aba"},{"ab"},{"a"}},         3, 0, REG_ERE_PCRE2},
    {__LINE__, {"abcabc"},      {"a(bc)a\\1"},      {{"abcabc"},{"bc"}},            2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abcabc"},      {"(a(bc))\\1"},     {{"abcabc"},{"abc"},{"bc"}},    3, 0, REG_ERE_PCRE2},
    {__LINE__, {"abcabc"},      {"(a)(b)c\\1\\2"},  {{"abcab"},{"a"},{"b"}},        3, 0, REG_ERE_PCRE2},
    {__LINE__, {"abcabc"},      {"((a)(b)c\\2)\\3"},{{"abcab"},{"abca"},{"a"},{"b"}},3, 0, REG_ERE_PCRE2},
    {__LINE__, {"abcabc"},      {"(...)\\1"},       {{"abcabc"},{"abc"}},           2, 0, REG_ERE_PCRE2},
    {__LINE__, {"<span>abc</span>"}, {"<(span)>.*</\\1>"},
                                                    {{"<span>abc</span>"},{"span"}},2, 0, REG_ERE_PCRE2},
    {__LINE__, {"aaabbbbc"},    {"([a-z])\\1{3}"},  {{"bbbb"},{"b"}},               2, 0, REG_ERE_PCRE2},

    {__LINE__, {"ab0"},         {"\\0"},            {{"0"}},                1, 0, REG_GNU},
//  {__LINE__, {"ab0",4},       {"\\0"},            {{"",1}},               1, 0, REG_PCRE2},   //PCRE2では\0はoctal
    {__LINE__, {"aba"},         {"\\1"},            {{""}},                 1,-1, REG_ALL},
    {__LINE__, {"aba"},         {"\\(a\\).\\9"},    {{""}},                 1,-1, REG_BRE},
    {__LINE__, {"aba"},         {"\\(a\\1\\)."},    {{""}},                 1,-1, REG_BRE},
    {__LINE__, {"aba"},         {"(a).\\9"},        {{""}},                 1,-1, REG_ERE_PCRE2},
    {__LINE__, {"aba"},         {"(a\\1)"},         {{""}},                 1,-1, REG_ERE},
    {__LINE__, {"abb"},         {"\\1(b)"},         {{""}},                 1,-1, REG_ERE},
//  {__LINE__, {"aba"},         {"(a\\1)"},         {{""}},                 1, 1, REG_PCRE2},   //PCRE2ではエラーにならない
//  {__LINE__, {"abb"},         {"\\1(b)"},         {{""}},                 1, 1, REG_PCRE2},   //PCRE2ではエラーにならない

    {__LINE__, {"a|b"},         {"a\\|b"},          {{"a"}},                1, 0, REG_BRE},
    {__LINE__, {"a|b"},         {"a|b"},            {{"a|b"}},              1, 0, REG_BRE},
    {__LINE__, {"a|b"},         {"a|b"},            {{"a"}},                1, 0, REG_ERE_PCRE2},
    {__LINE__, {"a|b"},         {"a\\|b"},          {{"a|b"}},              1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"c|b"},            {{"b"}},                1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"ac|ab"},          {{"ab"}},               1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"(a|b)c"},         {{"bc"},{"b"}},         2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"^(a|b)"},         {{"a"},{"a"}},          2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"(^c|^b|^a)"},     {{"a"},{"a"}},          2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"(a|b|c)$"},       {{"c"},{"c"}},          2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"(a$|b$|c$)"},     {{"c"},{"c"}},          2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc123"},      {"[A-Z]+|[0-9]+"},  {{"123"}},              1, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc123"},      {"([A-Z]|[0-9])+"}, {{"123"},{"3"}},        2, 0, REG_ERE_PCRE2},
    {__LINE__, {"abc123"},      {"([A-Z]|[0-9]){4}"},{{""}},                1, 1, REG_ERE_PCRE2},
    {__LINE__, {"abc"},         {"(a)(c|b)(b|c)"},  {{"abc"},{"a"},{"b"},{"c"}},4, 0, REG_ERE_PCRE2},
    {__LINE__, {"012345678910"},{"(1)(2)(3)(4)(5)(6)(7)(8)(9)(10)"},
               {{"12345678910"},{"1"},{"2"},{"3"},{"4"},{"5"},{"6"},{"7"},{"8"},{"9"},{"10"}},
                                                                            11,0, REG_ERE_PCRE2},//9を超えるサブパターンもOK

    {__LINE__, {"aabb"},        {"(a)|(\\1b)"},     {{""}},                 1,-1, REG_ERE},     //エラーになるべき
//  {__LINE__, {"aabb"},        {"(a)|(\\1b)"},     {{""}},                 1, 1, REG_PCRE2},   //PCRE2ではエラーにならない
    {__LINE__, {"aabb"},        {"(a)\\1|b)"},      {{"aa"},{"a"}},         2, 0, REG_ERE},     //EREでは余計な)はリテラル
    {__LINE__, {"aabb"},        {"(a)\\1|b)"},      {{""}},                 1,-1, REG_PCRE2},   //PCRE2では余計な)はエラー
    {__LINE__, {"aabb"},        {"(c)\\1|b)"},      {{""}},                 1, 1, REG_ERE},
    {__LINE__, {"aabb"},        {"(c)\\1|b)"},      {{""}},                 1,-1, REG_PCRE2},
    {__LINE__, {"aabb"},        {"(a)\\1|\\1b)"},   {{""}},                 1,-1, REG_ERE},
//  {__LINE__, {"aabb"},        {"(a)\\1|\\1b)"},   {{""}},                 1,-1, REG_PCRE2},   //PCRE2では不正な\1がエラーにならない
    {__LINE__, {"aabb"},        {"((a)|(b))\\1"},   {{"aa"},{"a"},{"a"}},   3, 0, REG_ERE_PCRE2},
    {__LINE__, {"aabb"},        {"((a)|(b))\\2"},   {{"aa"},{"a"},{"a"}},   3, 0, REG_ERE_PCRE2},
    {__LINE__, {"aabb"},        {"((a)|(b))\\3"},   {{"bb"},{"b"},{""},{"b"}}, 4, 0, REG_ERE_PCRE2},
    {__LINE__, {"ab"},          {"((a)|(b))\\1"},   {{""}},                 1, 1, REG_ERE_PCRE2},
    {__LINE__, {"ab"},          {"((a)|(b))\\2"},   {{""}},                 1, 1, REG_ERE_PCRE2},
    {__LINE__, {"ab"},          {"((a)|(b))\\3"},   {{""}},                 1, 1, REG_ERE_PCRE2},

    {__LINE__, {"abc"},         {"(??a)(b)"},       {{""}},                 1,-1, REG_PCRE2},
    {__LINE__, {"abc"},         {"(?"},             {{""}},                 1,-1, REG_PCRE2},
    {__LINE__, {"abc"},         {"(?="},            {{""}},                 1,-1, REG_PCRE2},
    {__LINE__, {"abc"},         {"(?<"},            {{""}},                 1,-1, REG_PCRE2},
    {__LINE__, {"abc"},         {"(?<A"},           {{""}},                 1,-1, REG_PCRE2},
    {__LINE__, {"abc"},         {"(?#a)(b)"},       {{"b"},{"b"}},          2, 0, REG_PCRE2},
//参照なし
    {__LINE__, {"abc"},         {"(?:a)(b)"},       {{"ab"},{"b"}},         2, 0, REG_PCRE2},
//lookahead assertion
    {__LINE__, {"abc"},         {"ab(?=c)"},        {{"ab"}},               1, 0, REG_PCRE2},
    {__LINE__, {"abcd"},        {"ab(?=c)cd"},      {{"abcd"}},             1, 0, REG_PCRE2},   //!!!
    {__LINE__, {"abc"},         {"ab(?=d)"},        {{""}},                 1, 1, REG_PCRE2},
    {__LINE__, {"abc"},         {"ab(?!d)"},        {{"ab"}},               1, 0, REG_PCRE2},
    {__LINE__, {"abc"},         {"ab(?!c)"},        {{""}},                 1, 1, REG_PCRE2},
    {__LINE__, {"abc"},         {"(ab(?=c))"},      {{"ab"},{"ab"}},        2, 0, REG_PCRE2},
    {__LINE__, {"abc"},         {"(ab(?!d))"},      {{"ab"},{"ab"}},        2, 0, REG_PCRE2},
    {__LINE__, {"abc"},         {"a(?=b)(?=c)"},    {{""}},                 1, 1, REG_PCRE2},
    {__LINE__, {"abc"},         {"a(?=b)(?=bc)"},   {{"a"}},                1, 0, REG_PCRE2},
    {__LINE__, {"abc"},         {"ab(?=d|c)"},      {{"ab"}},               1, 0, REG_PCRE2},
    {__LINE__, {"abc"},         {"ab(?=?=c)"},      {{""}},                 1,-1, REG_PCRE2},
//lookbehind assertion
    {__LINE__, {"abc"},         {"(?<=a)bc"},       {{"bc"}},               1, 0, REG_PCRE2},
//  {__LINE__, {"@abc"},        {"@(?<=a)bc"},      {{""}},                 1, 1, REG_PCRE2},   //PCRE2では"bc"にならない
    {__LINE__, {"abc"},         {"(?<=z)bc"},       {{""}},                 1, 1, REG_PCRE2},
//  {__LINE__, {"aabc"},        {"(?<=a*)bc"},      {{""}},                 1,-1, REG_PCRE2},   //固定長でない場合はエラー
    {__LINE__, {"abc"},         {"(?<!d)bc"},       {{"bc"}},               1, 0, REG_PCRE2},
    {__LINE__, {"abc"},         {"(?<!a)bc"},       {{""}},                 1, 1, REG_PCRE2},
//    {__LINE__, {"abc"},         {"(?<=.)(?<!z)bc"}, {{"bc"}},               1, 0, REG_PCRE2},
    {__LINE__, {"abc"},         {"(?<=d|a)bc"},     {{"bc"}},               1, 0, REG_PCRE2},
    {__LINE__, {"abc"},         {"(?<=de|a)bc"},    {{"bc"}},               1, 0, REG_PCRE2},

    {__LINE__, {"abc"},         {"a\\Kbc"},         {{"bc"}},               1, 0, REG_PCRE2},
    {__LINE__, {"abc"},         {"(a)\\Kbc"},       {{"bc"},{"a"}},         2, 0, REG_PCRE2},
    {__LINE__, {"abc"},         {"(a\\Kbc)"},       {{"bc"},{"abc"}},       2, 0, REG_PCRE2},
    {__LINE__, {"aabc"},        {"(a*\\Kbc)"},      {{"bc"},{"aabc"}},      2, 0, REG_PCRE2},
    {__LINE__, {"abc"},         {"a\\Kb\\Kc"},      {{"c"}},                1, 0, REG_PCRE2},

    {__LINE__, {"XYZ123_"},     {"[[:upper:]]*"},   {{"XYZ"}},              1, 0, REG_ALL},
    {__LINE__, {"abcXYZ123_"},  {"[[:upper:][:lower:]]*"}, {{"abcXYZ"}},    1, 0, REG_ALL},
    {__LINE__, {"abcXYZ123_"},  {"[^[:upper:][:lower:]]"}, {{"1"}},         1, 0, REG_ALL},
    {__LINE__, {"abcXYZ123_"},  {"[[:alpha:]]*"},   {{"abcXYZ"}},           1, 0, REG_ALL},
    {__LINE__, {"abcXYZ123_"},  {"[[:alnum:]]*"},   {{"abcXYZ123"}},        1, 0, REG_ALL},
    {__LINE__, {"<>[]_ aX1"},   { "[^[:alnum:]]*"}, {{"<>[]_ "}},           1, 0, REG_ALL},
    {__LINE__, {"abcXYZ123_"},  {"\\w*"},           {{"abcXYZ123_"}},       1, 0, REG_ALL}, //GNU,PCRE
    {__LINE__, {"!$abc"},       {"\\W*"},           {{"!$"}},               1, 0, REG_ALL}, //GNU,PCRE
    {__LINE__, {"x123ABCdefx"}, {"x[[:digit:]]*"},  {{"x123"}},             1, 0, REG_ALL},
    {__LINE__, {"x123ABCdefx"}, {"x\\d*"},          {{"x123"}},             1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"x123ABCdefx"}, {"\\D{3}"},         {{"ABC"}},              1, 0, REG_PCRE2},//PCRE
#ifdef REG_ENABLE_UTF8
    {__LINE__, {"a\ra \t　c"},  {"a\\h+"},          {{"a \t　"}},           1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"a aあ\tc"},    {"\\h\\H+\\h"},     {{" aあ\t"}},           1, 0, REG_PCRE2},//PCRE
#else
    {__LINE__, {"a\ra \tc"},    {"a\\h+"},          {{"a \t"}},             1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"a a\tc"},      {"\\h\\H+\\h"},     {{" a\t"}},             1, 0, REG_PCRE2},//PCRE
#endif
    {__LINE__, {"ab\v"},        {"\\V*"},           {{"ab"}},               1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"a\r\n b"},     {"\\R+"},           {{"\r\n"}},             1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"\n ab"},       {"\\N+"},           {{" ab"}},              1, 0, REG_PCRE2},//PCRE

    {__LINE__, {"x123ABCdefx"}, {"x[[:xdigit:]]*"}, {{"x123ABCdef"}},       1, 0, REG_ALL},
    {__LINE__, {"[:-+a]"},      {"[[:punct:]]*"},   {{"[:-+"}},             1, 0, REG_ALL},
    {__LINE__, {" \tz"},        {"[[:blank:]]*"},   {{" \t"}},              1, 0, REG_ALL},
    {__LINE__, {" \t\n\r\f\vz"},{"[[:space:]]*"},   {{" \t\n\r\f\v"}},      1, 0, REG_ALL},
    {__LINE__, {" \t\n\r\f\vz"},{"\\s*"},           {{" \t\n\r\f\v"}},      1, 0, REG_ALL}, //GNU,PCER
    {__LINE__, {"abc123 "},     {"\\S*"},           {{"abc123"}},           1, 0, REG_ALL}, //GNU,PCER
    {__LINE__, {"\01\177"},     {"[[:cntrl:]]*"},   {{"\01\177"}},          1, 0, REG_ALL},
    {__LINE__, {"abc \t"},      {"[[:graph:]]*"},   {{"abc"}},              1, 0, REG_ALL},
    {__LINE__, {"abc \t"},      {"[[:print:]]*"},   {{"abc "}},             1, 0, REG_ALL},
    {__LINE__, {"abc"},         {"[:alpha:]*"},     {{"a"}},                1, 0, REG_GNU},
//  {__LINE__, {"abc"},         {"[:alpha:]*"},     {{"a"}},                1, 0, REG_PCRE2},   //PCRE2ではエラー
    {__LINE__, {"abc"},         {"[[:Alpha:]]*"},   {{""}},                 1,-1, REG_ALL|REG_ICASE},
    {__LINE__, {"abc"},         {"[[:alpha]]"},     {{""}},                 1,-1, REG_GNU},     //GNUでは  "[:" + class_name + ":]"で判断
//  {__LINE__, {"a]bc"},        {"[[:alpha]]"},     {{"a]"}},               1, 0, REG_PCRE2},   //PCRE2では"[" + :class_name: + "]"で判断？
    {__LINE__, {"abc"},         {"[[:alpha:]"},     {{""}},                 1,-1, REG_ALL},
    {__LINE__, {"abc"},         {"[[:alpha:]-z]"},  {{""}},                 1,-1, REG_GNU},
//  {__LINE__, {"abc"},         {"[[:alpha:]-z]"},  {{""}},                 1,-1, REG_PCRE2},   //PCPR2では[class-]はエラー。[a-b-c]がエラーではないのに対して一貫性なし。
    {__LINE__, {"a\177c"},      {"[^[:alpha:]]"},   {{"\177"}},             1, 0, REG_ALL},

//  REG_ICASE
    {__LINE__, {"aBc"},         {"aBc"},            {{"aBc"}},              1, 0, REG_ALL|REG_ICASE},
    {__LINE__, {"aBc"},         {"Abc"},            {{"aBc"}},              1, 0, REG_ALL|REG_ICASE},
    {__LINE__, {"aBc"},         {"[a-z]*"},         {{"aBc"}},              1, 0, REG_ALL|REG_ICASE},
    {__LINE__, {"aBc"},         {"[^a-z]+"},        {{""}},                 1, 1, REG_ALL|REG_ICASE},
    {__LINE__, {"aBc"},         {"[^A-Z]+"},        {{""}},                 1, 1, REG_ALL|REG_ICASE},
    {__LINE__, {"aBc"},         {"[[:lower:]]*"},   {{"aBc"}},              1, 0, REG_GNU|REG_ICASE},
//  {__LINE__, {"aBc"},         {"[[:lower:]]*"},   {{"a"}},                1, 0, REG_PCRE2|REG_ICASE}, //PCRE2_UCPを指定すると挙動が変わる
    {__LINE__, {"aBc"},         {"[[:alpha:]]*"},   {{"aBc"}},              1, 0, REG_ALL|REG_ICASE},

//  REG_NEWLINE
    {__LINE__, {"abc\n"},       {"c\n"},            {{"c\n"}},              1, 0, REG_ALL},
    {__LINE__, {"abc\n"},       {"c\n"},            {{"c\n"}},              1, 0, REG_ALL|REG_NEWLINE},
    {__LINE__, {"ab\nc"},       {"b\nc"},           {{"b\nc"}},             1, 0, REG_ALL|REG_NEWLINE},
    {__LINE__, {"\nabc"},       {"\na"},            {{"\na"}},              1, 0, REG_ALL|REG_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"^x"},             {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"ab\nxy"},      {"^x"},             {{"x"}},                1, 0, REG_ALL|REG_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"b^x"},            {{""}},                 1, 1, REG_ALL|REG_NEWLINE},
    {__LINE__, {"\nxy"},        {"^x"},             {{"x"}},                1, 0, REG_ALL|REG_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"b$"},             {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"ab\nxy"},      {"b$"},             {{"b"}},                1, 0, REG_ALL|REG_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"b$x"},            {{""}},                 1, 1, REG_ALL|REG_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"b$^x"},           {{""}},                 1, 1, REG_ALL|REG_NEWLINE},
    {__LINE__, {"a\nb"},        {"a$\n^b"},         {{""}},                 1, 1, REG_BRE|REG_NEWLINE},
    {__LINE__, {"a\nb"},        {"a$\n^b"},         {{"a\nb"}},             1, 0, REG_ERE_PCRE2|REG_NEWLINE},
    {__LINE__, {"a\nb"},        {"a$^b"},           {{""}},                 1, 1, REG_ERE_PCRE2|REG_NEWLINE},
    {__LINE__, {"ab\n"},        {"b$"},             {{""}},                 1, 1, REG_GNU},     //GNUの$は行末の\nにマッチしない
    {__LINE__, {"ab\n"},        {"b$"},             {{"b"}},                1, 0, REG_PCRE2},   //PCREの$は行末の\nにマッチする
    {__LINE__, {"ab\n\n"},      {"b$"},             {{""}},                 1, 1, REG_PCRE2},   //         行末の\n\nにはマッチしない
    {__LINE__, {"ab\n"},        {"b\\Z"},           {{"b"}},                1, 0, REG_PCRE2},   //PCREの\Zは行末の\nにマッチする
    {__LINE__, {"ab\n"},        {"b\\z"},           {{""}},                 1, 1, REG_PCRE2},   //PCREの\zは行末の\nにマッチしない
    {__LINE__, {"ab\n"},        {"b$"},             {{"b"}},                1, 0, REG_ALL|REG_NEWLINE},
    {__LINE__, {"ab\n"},        {"b\\Z"},           {{"b"}},                1, 0, REG_PCRE2|REG_NEWLINE},
    {__LINE__, {"ab\n"},        {"b\\z"},           {{""}},                 1, 1, REG_PCRE2|REG_NEWLINE},

    {__LINE__, {"ab\nxy"},      {"\\(^x\\)y"},      {{"xy"},{"x"}},         2, 0, REG_BRE   |REG_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"(^x)y"},          {{"xy"},{"x"}},         2, 0, REG_ERE_PCRE2|REG_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"\\(b$\\)\nx"},    {{"b\nx"},{"b"}},       2, 0, REG_BRE   |REG_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"(b$)\nx"},        {{"b\nx"},{"b"}},       2, 0, REG_ERE_PCRE2|REG_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"^(x)y"},          {{"xy"},{"x"}},         2, 0, REG_ERE_PCRE2|REG_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"(b)$"},           {{"b"},{"b"}},          2, 0, REG_ERE_PCRE2|REG_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"^^xy"},           {{"xy"}},               1, 0, REG_ERE_PCRE2|REG_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"ab$$"},           {{"ab"}},               1, 0, REG_ERE_PCRE2|REG_NEWLINE},

    {__LINE__, {"xy"},          {"((^x)y)"},        {{"xy"},{"xy"},{"x"}},  3, 0, REG_ERE_PCRE2|REG_NEWLINE},
    {__LINE__, {"\nxy"},        {"(^x)y"},          {{"xy"},{"x"}},         2, 0, REG_ERE_PCRE2|REG_NEWLINE},
    {__LINE__, {"\nxy"},        {"((^x)y)"},        {{"xy"},{"xy"},{"x"}},  3, 0, REG_ERE_PCRE2|REG_NEWLINE},

    {__LINE__, {"ab\nxy"},      {"^xz|^xy"},        {{"xy"}},               1, 0, REG_ERE_PCRE2|REG_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"^(xz|xy)"},       {{"xy"},{"xy"}},        2, 0, REG_ERE_PCRE2|REG_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"(^xy|z)"},        {{"xy"},{"xy"}},        2, 0, REG_ERE_PCRE2|REG_NEWLINE},
    {__LINE__, {"ab\nxyz"},     {"((^xy|z)z)$"},    {{"xyz"},{"xyz"},{"xy"}},3, 0, REG_ERE_PCRE2|REG_NEWLINE},

    {__LINE__, {"ab\nxy"},      {"ab$|az$"},        {{"ab"}},               1, 0, REG_ERE_PCRE2|REG_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"(ab|az)$"},       {{"ab"},{"ab"}},        2, 0, REG_ERE_PCRE2|REG_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"(ab$|az$)"},      {{"ab"},{"ab"}},        2, 0, REG_ERE_PCRE2|REG_NEWLINE},
    {__LINE__, {"0ab\nxy"},     {"^(0(ab$|az$))"},  {{"0ab"},{"0ab"},{"ab"}},3, 0, REG_ERE_PCRE2|REG_NEWLINE},

    {__LINE__, {"ab\nxy"},      {"ab."},            {{"ab\n"}},             1, 0, REG_GNU},             //GNUでは.は\nにマッチする
    {__LINE__, {"ab\nxy"},      {"ab."},            {{""}},                 1, 1, REG_PCRE2,0,0,RE_DOT_NEWLINE},    //PCRE2では.は\nにマッチしない
    {__LINE__, {"ab\nxy"},      {"ab."},            {{"ab\n"}},             1, 0, REG_PCRE2,0,RE_DOT_NEWLINE},
    {__LINE__, {"ab\nxy"},      {"ab."},            {{""}},                 1, 1, REG_ALL|REG_NEWLINE}, //.は\nにマッチしない
    {__LINE__, {"ab\nxy"},      {"ab[^a-z]"},       {{"ab\n"}},             1, 0, REG_ALL},
    {__LINE__, {"ab\nxy"},      {"ab[^a-z]"},       {{""}},                 1, 1, REG_GNU|REG_NEWLINE}, //[^...]は\nにマッチしない
//  {__LINE__, {"ab\nxy"},      {"ab[^a-z]"},       {{""}},                 1, 1, REG_PCRE2|REG_NEWLINE},//[^...]は\nにマッチする
    {__LINE__, {"ab\nxy"},      {"ab[\n]"},         {{"ab\n"}},             1, 0, REG_ALL|REG_NEWLINE},

//  REG_NOTBOL,REG_NOTEOL
    {__LINE__, {"ab\nxy"},      {"^ab"},            {{"ab"}},               1, 0, REG_ALL},
    {__LINE__, {"ab\nxy"},      {"^ab"},            {{""}},                 1, 1, REG_ALL,REG_NOTBOL},  //文字列の開始を^にマッチさせない
    {__LINE__, {"ab\nxy"},      {"^xy"},            {{"xy"}},               1, 0, REG_ALL|REG_NEWLINE,REG_NOTBOL},
    {__LINE__, {"ab\nxy"},      {"xy$"},            {{"xy"}},               1, 0, REG_ALL},
    {__LINE__, {"ab\nxy"},      {"xy$"},            {{""}},                 1, 1, REG_ALL,REG_NOTEOL},  //文字列の最後を$にマッチさせない
    {__LINE__, {"ab\nxy"},      {"ab$"},            {{"ab"}},               1, 0, REG_ALL|REG_NEWLINE,REG_NOTEOL},

//   no.        text,tlen,       regexp,rlen,        {bstr,len},...         nmatch,expect,cflags,eflags,on_syntax,off_syntax,{start,end}
//  REG_STARTEND
    {__LINE__, {"aabbcc"},      {"bb"},             {{"bb"}},               1, 0, REG_ALL},
    {__LINE__, {"aabbcc"},      {"bb"},             {{"bb"}},               1, 0, REG_ALL,REG_STARTEND},
    {__LINE__, {"aabbcc"},      {"b*"},             {{""}},                 1, 0, REG_ALL},
    {__LINE__, {"aabbcc"},      {"b*"},             {{"bb"}},               1, 0, REG_ALL,REG_STARTEND,0,0,{2}},
    {__LINE__, {"aabbcc"},      {"b*"},             {{"b"}},                1, 0, REG_ALL,REG_STARTEND,0,0,{2,3}},

//  \0を含む処理
    {__LINE__, {"ab\0cd",5},    {"b\0c",3},         {{"b\0c",3}},           1, 0, REG_ALL},
    {__LINE__, {"ab\0cd",5},    {"b.c",3},          {{""}},                 1, 1, REG_GNU},     //GNUEはデフォルトでは.は\0にはマッチしない
    {__LINE__, {"ab\0cd",5},    {"b.c",3},          {{"b\0c",3}},           1, 0, REG_ALL,0,0,RE_DOT_NOT_NULL}, //.を\0にマッチさせる
    {__LINE__, {"ab\0\0cd",6},  {"b\0*c",4},        {{"b\0\0c",4}},         1, 0, REG_ALL},
    {__LINE__, {"ab\0cd",5},    {"(b\0c)",5},       {{"b\0c",3},{"b\0c",3}},2, 0, REG_ERE_PCRE2},
    {__LINE__, {"ab\0cd",5},    {"b(\0)c",5},       {{"b\0c",3},{"\0",1}},  2, 0, REG_ERE_PCRE2},
    {__LINE__, {"\0\01a",3},    {"[[:cntrl:]]*",0}, {{"\0\01",2}},          1, 0, REG_ALL},
//text_endのオーバーランチェック
    {__LINE__, {"abc",2},       {"abc"},            {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc",2},       {"ab."},            {{""}},                 1, 1, REG_ALL},
#ifdef REG_ENABLE_UTF8
    {__LINE__, {"あい",3},      {"あい"},           {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"あ",1},        {"."},              {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"あ"},          {"あ",1},           {{"あ",1}},             1, 0, REG_GNU},
//  {__LINE__, {"あ"},          {"あ",1},           {{"}},                  1,-1, REG_PCRE2},
#endif //REG_ENABLE_UTF8
    {__LINE__, {"abc",2},       {"ab(c)"},          {{""}},                 1, 1, REG_ERE_PCRE2},
    {__LINE__, {"aba",2},       {"(a)b\\1"},        {{""}},                 1, 1, REG_ERE_PCRE2},
    {__LINE__, {"a\n",1},       {"^"},              {{""}},                 1, 1, REG_ALL|REG_NEWLINE,REG_NOTBOL},
    {__LINE__, {"a\n",1},       {"a$"},             {{""}},                 1, 1, REG_ALL|REG_NEWLINE,REG_NOTEOL},
    {__LINE__, {"a,c",2},       {",\\<"},           {{""}},                 1, 1, REG_GNU},
    {__LINE__, {"abc",2},       {"ab\\>"},          {{"ab"}},               1, 0, REG_GNU},
    {__LINE__, {"a,c",2},       {",\\b"},           {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"abc",2},       {"ab\\b"},          {{"ab"}},               1, 0, REG_ALL},
    {__LINE__, {"abc",2},       {"ab\\b"},          {{"ab"}},               1, 0, REG_ALL},
    {__LINE__, {"abc",2},       {"ab\\B"},          {{""}},                 1, 1, REG_ALL},
    {__LINE__, {"aaa",2},       {"a{3}"},           {{""}},                 1, 1, REG_ERE_PCRE2},

//  Word
    {__LINE__, {"scan can."},   {"\\<can"},         {{"can"}},              1, 0, REG_GNU}, //GNU
    {__LINE__, {"scan can."},   {"\\<scan"},        {{"scan"}},             1, 0, REG_GNU}, //GNU
    {__LINE__, {"12s _9"},      {"\\<12"},          {{"12"}},               1, 0, REG_GNU}, //GNU
    {__LINE__, {"12s _9"},      {"\\<_"},           {{"_"}},                1, 0, REG_GNU}, //GNU
    {__LINE__, {"begin end"},   {"\\<\\w{3}\\>"},   {{"end"}},              1, 0, REG_ERE}, //GNU
    {__LINE__, {"begin end"},   {"\\b\\w{3}\\b"},   {{"end"}},              1, 0, REG_ERE_PCRE2},
    {__LINE__, {"canCan"},      {"\\Bcan"},         {{"Can"}},              1, 0, REG_ALL|REG_ICASE},
    {__LINE__, {"canCan"},      {"can\\Bcan"},      {{"canCan"}},           1, 0, REG_ALL|REG_ICASE},

    {__LINE__, {"A\ncan\nZ"} ,  {"\\<can\\>"},      {{"can"}},              1, 0, REG_GNU}, //GNU
    {__LINE__, {"A\nscan\nZ"} , {"\\<can\\>"},      {{""}},                 1, 1, REG_GNU}, //GNU
    {__LINE__, {"A\ncan\nZ"} ,  {"\\<(can)\\>"},    {{"can"},{"can"}},      2, 0, REG_ERE}, //GNU
    {__LINE__, {"A\ncan\nZ"} ,  {"(\\<can\\>)"},    {{"can"},{"can"}},      2, 0, REG_ERE}, //GNU

    {__LINE__, {"begin end"},   {"\\`\\w+"},        {{"begin"}},            1, 0, REG_ERE},     //GNU
    {__LINE__, {"can Can"},     {"\\`Can"},         {{""}},                 1, 1, REG_GNU},     //GNU
    {__LINE__, {"can\nCan"},    {"\\`Can"},         {{""}},                 1, 1, REG_GNU},     //GNU
    {__LINE__, {"begin end"},   {"\\w+\\'"},        {{"end"}},              1, 0, REG_ERE},     //GNU
    {__LINE__, {"begin end"},   {"\\A\\w+"},        {{"begin"}},            1, 0, REG_PCRE2},   //PCRE2
    {__LINE__, {"can Can"},     {"\\ACan"},         {{""}},                 1, 1, REG_PCRE2},   //PCRE2
    {__LINE__, {"can\nCan"},    {"\\ACan"},         {{""}},                 1, 1, REG_PCRE2},   //PCRE2
    {__LINE__, {"begin end"},   {"\\w+\\z"},        {{"end"}},              1, 0, REG_PCRE2},   //PCRE2

//  UTF-8
#ifdef REG_ENABLE_UTF8
    {__LINE__, {"あいう"},      {"あ"},             {{"あ"}},               1, 0, REG_ERE_PCRE2},
    {__LINE__, {"ああう"},      {"あ*"},            {{"ああ"}},             1, 0, REG_ERE_PCRE2},
    {__LINE__, {"ああう"},      {"(あ)*"},          {{"ああ"}},             1, 0, REG_ERE_PCRE2},
    {__LINE__, {"あいう"},      {"^あ"},            {{"あ"}},               1, 0, REG_ERE_PCRE2},
    {__LINE__, {"あいあ"},      {"あ$"},            {{"あ"}},               1, 0, REG_ERE_PCRE2},
    {__LINE__, {"あいう"},      {"(あ|い)*"},       {{"あい"}},             1, 0, REG_ERE_PCRE2},
    {__LINE__, {"あいう"},      {".."},             {{"あい"}},             1, 0, REG_ERE_PCRE2},
    {__LINE__, {"あいbう"},     {"[あaい]*"},       {{"あい"}},             1, 0, REG_ERE_PCRE2},
    {__LINE__, {"あbいう"},     {"[^aい]*"},        {{"あb"}},              1, 0, REG_ERE_PCRE2},
    {__LINE__, {"あ１０"},      {"[０-９]+"},       {{"１０"}},             1, 0, REG_ERE_PCRE2},
    {__LINE__, {"１０あ"},      {"[^０-９]+"},      {{"あ"}},               1, 0, REG_ERE_PCRE2},

    {__LINE__, {"aβｆ１三あ1"},{"[[:alpha:]]+"},   {{"aβｆ１三あ"}},      1, 0, REG_ERE},
//  {__LINE__, {"aβｆ１三あ1"},{"[[:alpha:]]+"},   {{"aβｆ１三あ"}},      1, 0, REG_PCRE2},     //PCRE2_UCPを指定すると挙動が変わる
    {__LINE__, {"a1βｆ１三あ"},{"[[:alnum:]]+"},   {{"a1βｆ１三あ"}},     1, 0, REG_ERE_PCRE2},
    {__LINE__, {"a1_βｆ１三あ"},{"\\w+"},          {{"a1_βｆ１三あ"}},    1, 0, REG_ERE_PCRE2},
    {__LINE__, {"a1_βｆ１三あ"},{"[[:word:]]+"},   {{"a1_βｆ１三あ"}},    1, 0, REG_PCRE2},
    {__LINE__, {"aβ1１三あ"},  {"[[:digit:]]+"},   {{"1"}},                1, 0, REG_ERE},
//  {__LINE__, {"aβ1１三あ"},  {"[[:digit:]]+"},   {{"1１"}},              1, 0, REG_PCRE2},     //PCRE2_UCPを指定すると全角もマッチ
    {__LINE__, {"a1ｆ１三あ"},  {"[[:xdigit:]]+"},  {{"a1"}},               1, 0, REG_ERE_PCRE2},
    {__LINE__, {"aβｆ１三あ"}, {"[[:lower:]]+"},   {{"aβｆ"}},            1, 0, REG_ERE_PCRE2},
    {__LINE__, {"aβｆ１三あ"}, {"[[:upper:]]+"},   {{""}},                 1, 1, REG_ERE_PCRE2},
    {__LINE__, {"ZΣＦ１三あ"}, {"[[:upper:]]+"},   {{"ZΣＦ"}},            1, 0, REG_ERE_PCRE2},

    {__LINE__, {"aあ　 \t\n"},  {"[[:blank:]]+"},   {{"　 \t"}},            1, 0, REG_ERE_PCRE2},
    {__LINE__, {"aあ　 \t\n"},  {"[[:space:]]+"},   {{"　 \t\n"}},          1, 0, REG_ERE_PCRE2},
    {__LINE__, {"aあ　 \t\n"},  {"\\s+"},           {{"　 \t\n"}},          1, 0, REG_ERE_PCRE2},
    {__LINE__, {"aあ　 \t\n"},  {"\\S+"},           {{"aあ"}},              1, 0, REG_ERE_PCRE2},
    {__LINE__, {"aβ１三あ1"},  {"[[:print:]]+"},   {{"aβ１三あ1"}},       1, 0, REG_ERE_PCRE2},
    {__LINE__, {"aβ１三あ1"},  {"[[:graph:]]+"},   {{"aβ１三あ1"}},       1, 0, REG_ERE_PCRE2},
    {__LINE__, {"\01\177"},     {"[[:cntrl:]]*"},   {{"\01\177"}},          1, 0, REG_ERE_PCRE2},
    {__LINE__, {"[:＆＋￥a]"},  {"[[:punct:]]*"},   {{"[:＆＋￥"}},         1, 0, REG_ERE},
//  {__LINE__, {"[:＆＋￥a]"},  {"[[:punct:]]*"},   {{"[:＆"}},             1, 0, REG_PCRE2},     //PCRE2_UCPを指定すると挙動が変わる
    {__LINE__, {"[:＆＋￥a]"},  {"\\W*"},           {{"[:＆＋￥"}},         1, 0, REG_ERE_PCRE2},
    {__LINE__, {"a1βｆ１三あ"},{"[[:alpha:][:digit:]]+"},
                                                    {{"a1βｆ１三あ"}},     1, 0, REG_ERE_PCRE2},
    {__LINE__, {"\01@\177"},    {"[[:ascii:]]*"},   {{"\01@\177"}},         1, 0, REG_PCRE2},

    {__LINE__, {"ZΣＡあ"},     {"zσａ"},          {{"ZΣＡ"}},            1, 0, REG_ERE_PCRE2|REG_ICASE},
    {__LINE__, {"zσａあ"},     {"ZΣＡ"},          {{"zσａ"}},            1, 0, REG_ERE_PCRE2|REG_ICASE},
    {__LINE__, {"ZΣＡあ"},     {"[zσａ]+"},       {{"ZΣＡ"}},            1, 0, REG_ERE_PCRE2|REG_ICASE},
    {__LINE__, {"zσａあ"},     {"[ZΣＡ]+"},       {{"zσａ"}},            1, 0, REG_ERE_PCRE2|REG_ICASE},
    {__LINE__, {"ZΣＡあ"},     {"[a-zα-σａ-ｚ]+"},{{"ZΣＡ"}},           1, 0, REG_ERE_PCRE2|REG_ICASE},
    {__LINE__, {"zσａあ"},     {"[A-ZΑ-ΣＡ-Ｚ]+"},{{"zσａ"}},           1, 0, REG_ERE_PCRE2|REG_ICASE},
    {__LINE__, {"ZΣＡあ"},     {"[[:lower:]]+"},   {{"ZΣＡあ"}},          1, 0, REG_ERE      |REG_ICASE},
//  {__LINE__, {"ZΣＡあ"},     {"[[:lower:]]+"},   {{""}},                 1, 1, REG_PCRE2    |REG_ICASE},
    {__LINE__, {"zσａあ"},     {"[[:upper:]]+"},   {{"zσａあ"}},          1, 0, REG_ERE      |REG_ICASE},
//  {__LINE__, {"zσａあ"},     {"[[:upper:]]+"},   {{"zσａあ"}},          1, 0, REG_PCRE2    |REG_ICASE},
    {__LINE__, {"aBβΣｂＺっ"},{"[[:lower:]]+"},   {{"aBβΣｂＺっ"}},     1, 0, REG_ERE      |REG_ICASE},
//  {__LINE__, {"aBβΣｂＺっ"},{"[[:lower:]]+"},   {{"a"}},                1, 0, REG_PCRE2    |REG_ICASE},

    {__LINE__, {"x12３ABＣdefx"}, {"x\\d*"},        {{"x12３"}},            1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"x12３ABＣdefx"}, {"\\D{3}"},       {{"ABＣ"}},             1, 0, REG_PCRE2},//PCRE
#endif

//escaped character
    {__LINE__, {"\a\x1b\f\n\r\t\v"},{"\\a\\e\\f\\n\\r\\t\\v"},{{"\a\x1b\f\n\r\t\v"}},1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"\a\f0"},       {"\\07\\0140"},     {{"\a\f0"}},            1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"\0\a\f0",4},   {"\\0\\07\\0140"},  {{"\0\a\f0",4}},        1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"\0\a\f0",4},   {"\\x0\\x7\\x0C0"}, {{"\0\a\f0",4}},        1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"\0\a\f0",4},   {"\\x\\x07\\x0c0"}, {{"\0\a\f0",4}},        1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"\0\a\f0",4},   {"\\o{0}\\o{07}\\o{014}0"}, {{"\0\a\f0",4}},1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"\0\a\f0",4},   {"\\x{0}\\x{07}\\x{0C}0"}, {{"\0\a\f0",4}}, 1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"\0\a\1770",4}, {"\\c@\\cG\\c?0"},  {{"\0\a\1770",4}},      1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"\0\a\1770",4}, {"\\c@\\cg\\c?0"},  {{"\0\a\1770",4}},      1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"`a~"},         {"\\c \\c!\\c>"},   {{"`a~"}},              1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\o"},            {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\o{"},           {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\o{0"},          {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\o{}"},          {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\o{8}"},         {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\o{0A}"},        {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\oA"},           {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\x{"},           {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\x{0"},          {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\x{}"},          {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\x{G}"},         {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\x{0Z}"},        {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\xZ"},           {{""}},                 1, 1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\c"},            {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\c\n"},          {{""}},                 1,-1, REG_PCRE2},//PCRE
#ifdef REG_ENABLE_UTF8
    {__LINE__, {"abc"},         {"\\N{"},           {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\N{U"},          {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\N{UX"},         {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\N{U+"},         {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\N{U+}"},        {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\N{U+G}"},       {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\N{U+0Z}"},      {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {u8"\u3042"},    {"\\o{30102}"},     {{"あ"}},               1, 0, REG_PCRE2},//PCRE
    {__LINE__, {u8"\u3042"},    {"\\x{3042}"},      {{"あ"}},               1, 0, REG_PCRE2},//PCRE
    {__LINE__, {u8"\u3042"},    {"\\N{U+3042}"},    {{"あ"}},               1, 0, REG_PCRE2},//PCRE
    {__LINE__, {u8"\U0001f600"},{"\\o{373000}"},    {{"😀"}},              1, 0, REG_PCRE2},//PCRE
    {__LINE__, {u8"\U0001f600"},{"\\x{1f600}"},     {{"😀"}},              1, 0, REG_PCRE2},//PCRE
    {__LINE__, {u8"\U0001f600"},{"\\N{U+1f600}"},   {{"😀"}},              1, 0, REG_PCRE2},//PCRE
    {__LINE__, {u8"\u3042"},    {"\\o{3Z0102}"},    {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {u8"\u3042"},    {"\\x{3Z042}"},     {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {u8"\u3042"},    {"\\N{U+3Z042}"},   {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\o{777777777}"}, {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\x{123456789}"}, {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"あz"},         {"[\\N{U+3042}]z"}, {{"あz"}},              1, 0, REG_PCRE2},//PCRE
#else
    {__LINE__, {"abc"},         {"\\o{30102}"},     {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"\\x{3042}"},      {{""}},                 1,-1, REG_PCRE2},//PCRE
#endif
    {__LINE__, {"\t\a\f"},      {"[\\t\\07\\x{c}]*"},{{"\t\a\f"}},          1, 0, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"[\\o]"},          {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"(\\o)"},          {{""}},                 1,-1, REG_PCRE2},//PCRE
    {__LINE__, {"abc"},         {"(\\o|a)"},        {{""}},                 1,-1, REG_PCRE2},//PCRE

//RE_COMMENT
    {__LINE__, {"abc"},         {"ab \t\r\n\f\vc"}, {{"abc"}},              1, 0, REG_PCRE2,0,RE_COMMENT},
    {__LINE__, {"abc \t\n"},    {"abc\\ \\\t\\\n"}, {{"abc \t\n"}},         1, 0, REG_PCRE2,0,RE_COMMENT},
    {__LINE__, {"abc "},        {"( a # comment #\r\n\
                                    b\tc)"},        {{"abc"},{"abc"}},      2, 0, REG_PCRE2,0,RE_COMMENT},
    {__LINE__, {"abc"},         {"ab \t\r\n\f\vc"}, {{"abc"}},              1, 0, REG_PCRE2,0,RE_COMMENT_EXT},
    {__LINE__, {" \tabc\r"},    {"[abc \t\r]+"},    {{"abc\r"}},            1, 0, REG_PCRE2,0,RE_COMMENT_EXT},


//  {__LINE__, {"a"},           {"[^a]"},           {{""}},                 1, 1, REG_ERE_PCRE2|REG_DUMP},   //dumpテスト
//}
//   no.        text,tlen,       regexp,rlen,        {bstr,len},...         nmatch,expect,cflags,eflags,on_syntax,off_syntax,{start,end}
