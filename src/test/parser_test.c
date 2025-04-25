#include "../monkey.h"

static bool _test_let_stmt(StmtV2 *stmt, const char *name)
{
    int name_len = strlen(name);
    // Confirm type is LET statement
    if (stmt->type != ST_LET) {
        fprintf(stderr, "ERROR: not LET stmt. got='%s'\n", st_type_to_str(stmt->type));
        return false;
    }

    // Check token literal is "let"
    LetStmt *ls = stmt->data;
    const char *token_lit = letstmt_token_lit(ls);
    if (strncmp(token_lit, "let", 3)) {
        fprintf(stderr, "ERROR: token literal not 'let'. got='%s'\n", token_lit);
        return false;
    }

    // Check if identifier is correct
    if (strncmp(ls->name->value, name, name_len)) {
        fprintf(stderr, "ERROR: let_stmt.name.value not '%s'. got='%s'\n", name, ls->name->value);
        return false;
    }

    token_lit = ident_token_lit(ls->name);
    if (strncmp(token_lit, name, name_len)) {
        fprintf(stderr, "ERROR: let ident token name not '%s'. got='%s'\n", name, token_lit);
        return false;
    }

    return true;
}

static void check_parser_errors(Parser *p)
{
    const int N = p->errors.count;
    if (N == 0) return;
    for (int i = 0; i < N; i++) {
        char *err = p->errors.items[i];
        fprintf(stderr, "Parser error: %s\n", err);
    }
    fprintf(stderr, "ERROR: parser had %d errors.\n", N);

    // Fail now due to the error(s)
    exit(EXIT_FAILURE);
}

bool test_let_stmts(void)
{
    char *input = NULL;
    {
        // Get file size
        const char *filepath = "src/test/parser_let_test.monkey";
        FILE *fp = fopen(filepath, "r");
        if (fp == NULL) {
            fprintf(stderr, "ERROR: Could not find '%s'\n", filepath);
            return false;
        }
        fseek(fp, 0L, SEEK_END);
        int file_size = (int)ftell(fp);
        rewind(fp);

        // Read file content
        input = arena_alloc(&global_arena, file_size);
        fread(input, file_size, 1, fp);
        fclose(fp);
    }

    Lexer l = {0};
    lexer_init(&l, input, strlen(input));
    Parser p = {0};
    parser_init(&p, &l);

    Program prog = {0};
    parser_parse_program(&p, &prog);
    check_parser_errors(&p);

    if (prog.count != 3) {
        fprintf(stderr, "ERROR: prog.Stmts does not contain 3 statements. got=%d\n", prog.count);
        return false;
    }

    const char *expectedIdent[] = {"x", "y", "foobar"};
    const int N = sizeof(expectedIdent) / sizeof(*expectedIdent);

    for (int i = 0; i < N; i++) {
        if (!_test_let_stmt(&prog.items[i], expectedIdent[i]))
            return false;
    }

    return true;
}

bool test_return_stmts()
{
    char *input = NULL;
    {
        // Get file size
        const char *filepath = "src/test/parser_return_test.monkey";
        FILE *fp = fopen(filepath, "r");
        if (fp == NULL) {
            fprintf(stderr, "ERROR: Could not find '%s'\n", filepath);
            return false;
        }
        fseek(fp, 0L, SEEK_END);
        int file_size = (int)ftell(fp);
        rewind(fp);

        // Read file content
        input = arena_alloc(&global_arena, file_size);
        fread(input, file_size, 1, fp);
        fclose(fp);
    }

    Lexer l = {0};
    lexer_init(&l, input, strlen(input));
    Parser p = {0};
    parser_init(&p, &l);

    Program prog = {0};
    parser_parse_program(&p, &prog);
    check_parser_errors(&p);

    if (prog.count != 3) {
        fprintf(stderr, "ERROR: prog.Stmts does not contain 3 statements. got=%d\n", prog.count);
        return false;
    }

    char *token_lit = NULL;
    for (int i = 0; i < prog.count; i++) {
        StmtV2 st = prog.items[i];
        if (st.type != ST_RETURN) {
            fprintf(stderr, "ERROR: stmt not return statement, got=%s\n", st_type_to_str(st.type));
            continue;
        }
        token_lit = returnstmt_token_lit(st.data);
        if (strncmp(token_lit, "return", 6)) {
            fprintf(stderr, "ERROR: retstmt.literal not 'return', got=%s\n", token_lit);
        }
    }

    return true;
}

int main(void)
{
    bool success = test_let_stmts() & test_return_stmts();
    if (success) {
        printf("All tests passed!\n");
        return 0;
    } else {
        printf("Test failed.\n");
        return 1;
    }
}
