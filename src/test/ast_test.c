#include "../monkey.h"

bool test_string()
{
    Program prog = {0};
    LetStmt ls = {
        .token = (Token){
            .type = TT_LET,
            .literal = "let",
            .lit_size = 3,
        },
        .name = &((Identifier) {
            .token = (Token) {
                .type = TT_IDENT,
                .literal = "myVar",
                .lit_size = 5
            },
            .value = "myVar",
            .value_size = 5,
        }),
        .value = &((Identifier){
            .token = (Token) {
                .type = TT_IDENT,
                .literal = "anotherVar",
                .lit_size = 10,
            },
            .value = "anotherVar",
            .value_size = 10,
        }),
    };
    program_append_stmt(&prog, (StmtV2){.data = &ls, .type = ST_LET});
    char *str = program_string(&prog);
    char *expected = "let myVar = anotherVar";
    if (strncmp(str, expected, strlen(expected))) {
        fprintf(stderr, "program_string() wrong. got='%s'\n", str);
        return false;
    }
    return true;
}

int main(void)
{
    if (test_string()) {
        printf("All tests passed!\n");
        return 0;
    } else {
        printf("Test failed!\n");
        return 1;
    }
}
