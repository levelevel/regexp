

typedef struct regcomp reg_compile_t;

reg_compile_t* reg_compile(const char *regexp);
int reg_exec(reg_compile_t *preg_compile, const char *text);
void reg_compile_free(reg_compile_t* preg_compile);
int match(const char *regexp, const char *text);
