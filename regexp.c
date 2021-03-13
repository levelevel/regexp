// c
// \c
// ^
// $
// .
// [s]
// [^s]
// *

// https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html
// A Regular Expression Matcher
// Code by Rob Pike 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "regexp.h"

#define SPECIALCHARS "\\^$.[*"

typedef struct {
    int reverse;
    char *chars;
} char_class_t;

static int matchhere(const char *regexp, const char *text);
static char_class_t *get_char_class(const char *regexp, int *len);
static int matchchar(const char *regexp, const char *text, int *len);
static int matchstar(int c, const char *regexp, const char *text);

/* match: search for regexp anywhere in text */
int match(const char *regexp, const char *text)
{
    if (regexp[0] == '^')
        return matchhere(regexp+1, text);
    do {    /* must look even if string is empty */
        if (matchhere(regexp, text))
            return 1;
    } while (*text++ != '\0');
    return 0;
}

/* matchhere: search for regexp at beginning of text */
static int matchhere(const char *regexp, const char *text)
{
    int len;
    if (regexp[0] == '\0')
        return 1;
    if (regexp[0] == '\\' && strchr(SPECIALCHARS, regexp[1])) {
        if (regexp[1] == text[0]) return matchhere(regexp+2, text+1);
        else return 0;
    }
    if (regexp[1] == '*')
        return matchstar(regexp[0], regexp+2, text);
    if (regexp[0] == '$' && regexp[1] == '\0')
        return *text == '\0';
    if (*text!='\0' && matchchar(regexp, text, &len))
        return matchhere(regexp+len, text+1);
    return 0;
}

static char_class_t *get_char_class(const char *regexp, int *len)
{
    assert(*regexp == '[');
    int chars_len = 128;
    int pos = 0;
    char_class_t *char_class = calloc(1, sizeof(char_class_t));
    char_class->chars = calloc(chars_len, sizeof(char));
    const char *org = regexp;

    regexp++;
    if (*regexp == '^') {
        char_class->reverse = 1;
        regexp++;
    }
    if (*regexp == ']' || *regexp == '-') {
        char_class->chars[pos++] = *regexp;
        regexp++;
    }
    for (; *regexp != '\0'; regexp++) {
        if (*regexp == '-' && regexp[1] != '\0' && regexp[1] != ']') {
            //rangeの逆転はチェックしていない。[z-a]は[za]と同じ。
            for (int c=regexp[-1]+1; c<regexp[1]; c++) {
                char_class->chars[pos++] = c;
            }
            char_class->chars[pos++] = regexp[1];
            regexp++;
        } else if (*regexp != ']') {
            char_class->chars[pos++] = *regexp;
        } else {
            regexp++;
            break;
        }
    }
    *len = regexp-org;
    //printf("regexp=%s, len=%d\n", org, *len);
    //printf("[%s]\n", char_class->chars);
    return char_class;
}

static int matchchar(const char *regexp, const char *text, int *len) {
    *len = 1;
    if (*regexp == '.')
        return 1;
    if (*regexp == '[') {
        char_class_t *char_class = get_char_class(regexp, len);
        if (strchr(char_class->chars, *text))
            return !char_class->reverse;
        return char_class->reverse;
    }
    if (*regexp == *text)
        return 1;
    return 0;
}

/* matchstar: search for c*regexp at beginning of text */
static int matchstar(int c, const char *regexp, const char *text)
{
    do {    /* a * matches zero or more instances */
        if (matchhere(regexp, text))
            return 1;
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}
