#include <stdio.h>
#include "regexp.h"

typedef struct {
    const char *text;
    const char *regexp;
    const int expect;
} test_t;
test_t data[] = {
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
 };

int main(int argc, char **argv) {
    int result = 0;
    for (int i=0; i<sizeof(data)/sizeof(test_t); i++) {
        test_t *t = &data[i];
        int ret = match(t->regexp, t->text);
        if (ret != t->expect) {
            printf("ERROR: %d: text='%s', regexp='%s', expect=%d\n", i, t->text, t->regexp, t->expect);
            result = 1;
        }
    }
    return result;
}