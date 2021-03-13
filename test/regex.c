#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

int main(int argc, char **argv) {
    regex_t preg;
    int errcode;

    if (argc<3) {
        fprintf(stderr, "Usage: regex regexp text ...\n");
        return 1;
    }

    printf("regex = '%s'\n", argv[1]);
    errcode = regcomp(&preg, argv[1], REG_NOSUB);
    if (errcode !=0) {
        char buf[256];
        regerror(errcode, &preg, buf, sizeof(buf));
        fprintf(stderr, "Error: %s\n", buf);
        return 1;
      }

    for (int i = 2; i < argc; i++) {
        if (regexec(&preg, argv[i], 0, NULL, 0)==0) {
            printf("'%s' : match\n", argv[i]);
        } else {
            printf("'%s' : not match!\n", argv[i]);
        }
    }

    regfree(&preg);
    return 0;
}
