#include "../monkey.h"

static bool _test_let_stmt(StatementVT *stmt, const char *name)
{
    if (!strncmp(stmt->nvt->token_lit_func_ptr(), "let", 3)) {
        fprintf(stderr, "ERROR: stmt.token_lit not 'let'. got='%s'", stmt->nvt->token_lit_func_ptr());
        return false;
    }

    fprintf(stderr, "TODO: finish this test (pg. 39)");
    exit(123);
}

static bool test_let_stmts(void)
{
    Arena a = {0};
    char *input = NULL;
    {
        // Get file size
        const char *filepath = "src/test/parser_test.monkey";
        FILE *fp = fopen(filepath, "r");
        if (fp == NULL) {
            fprintf(stderr, "ERROR: Could not find '%s'\n", filepath);
            return false;
        }
        fseek(fp, 0L, SEEK_END);
        int file_size = (int)ftell(fp);
        rewind(fp);

        // Read file content
        input = arena_alloc(&a, file_size);
        fread(input, file_size, 1, fp);
        fclose(fp);
    }

    Lexer l = {0};
    lexer_init(&l, input, strlen(input));
    Parser p = {0};
    parser_init(&p, &l);

    Program prog = {0};
    if (!parser_parse_program(&p, &prog)) {
        fprintf(stderr, "ERROR: failed to parser program");
        return false;
    }

    if (prog.stmt_count != 3) {
        fprintf(stderr, "ERROR: prog.Stmts does not contain 3 statements. got=%d\n",
                prog.stmt_count);
        return false;
    }

    const char *expectedIdent[] = {"x", "y", "foobar"};
    const int N = sizeof(expectedIdent) / sizeof(*expectedIdent);

    for (int i = 0; i < N; i++) {
        StatementVT *stmt = &prog.stmts[i];
        if (_test_let_stmt(stmt, expectedIdent[i])) return false;
    }

    return true;
}

int main(void)
{
    return !test_let_stmts();
}
