#define ARENA_IMPLEMENTATION
#include "../monkey.h"

bool test_next_token()
{
    Arena a = {0};
    char *input = NULL;
    {
        // Get file size
        const char *filepath = "src/test/lexer_test.monkey";
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

    // clang-format off
    const TokenType expected_tt[] = {
        // "let five = 5;"
        TT_LET, TT_IDENT, TT_ASSIGN, TT_INT, TT_SEMICOLON,

        // "let ten = 10;"
        TT_LET, TT_IDENT, TT_ASSIGN, TT_INT, TT_SEMICOLON,

        // "let add = fn(x, y) {"
        TT_LET, TT_IDENT, TT_ASSIGN, TT_FUNCTION, TT_LPAREN,
        TT_IDENT, TT_COMMA, TT_IDENT, TT_RPAREN, TT_LBRACE,

        // "x + y;"
        TT_IDENT, TT_PLUS, TT_IDENT, TT_SEMICOLON,

        // "};"
        TT_RBRACE, TT_SEMICOLON,

        // "let result = add(five, ten);"
        TT_LET, TT_IDENT, TT_ASSIGN, TT_IDENT, TT_LPAREN,
        TT_IDENT, TT_COMMA, TT_IDENT, TT_RPAREN, TT_SEMICOLON,

        // "!-/*5;"
        TT_BANG, TT_MINUS, TT_SLASH, TT_ASTERISK, TT_INT, TT_SEMICOLON,

        // "5 < 10 > 5;"
        TT_INT, TT_LT, TT_INT, TT_GT, TT_INT, TT_SEMICOLON,

        // if (5 < 10) {
        TT_IF, TT_LPAREN, TT_INT, TT_LT, TT_INT, TT_RPAREN, TT_LBRACE,

        // "return true;"
        TT_RETURN, TT_TRUE, TT_SEMICOLON,

        // "} else {"
        TT_RBRACE, TT_ELSE, TT_LBRACE,

        // "return false;"
        TT_RETURN, TT_FALSE, TT_SEMICOLON,

        // "}"
        TT_RBRACE,

        // "10 == 10"
        TT_INT, TT_EQ, TT_INT,

        // "10 != 9"
        TT_INT, TT_NOT_EQ, TT_INT,

        TT_EOF
    };

    const char *expected_lit[] = {
        "let", "five", "=", "5", ";",

        "let", "ten", "=", "10", ";",

        "let", "add", "=", "fn", "(",
        "x", ",", "y", ")", "{",

        "x", "+", "y", ";",

        "}", ";",

        "let", "result", "=", "add", "(",
        "five", ",", "ten", ")", ";",

        "!", "-", "/", "*", "5", ";",

        "5", "<", "10", ">", "5", ";",

        "if", "(", "5", "<", "10", ")", "{",

        "return", "true", ";",

        "}", "else", "{",

        "return", "false", ";",

        "}",

        "10", "==", "10",

        "10", "!=", "9",

        NULL
    };
    // clang-format on
    const int N = sizeof(expected_lit) / sizeof(*expected_lit);

    Lexer l = {0};
    lexer_init(&l, input, strlen(input));
    bool success = true;
    for (int i = 0; i < N; i++) {
        Token tok = lexer_next_token(&l);

        if (tok.type != expected_tt[i]) {
            fprintf(stderr, "tests[%d] - tokentype wrong. expected='%s', got='%s'\n", i,
                    tt_to_str(expected_tt[i]), tt_to_str(tok.type));
            return false;
        }

        if (tok.literal != NULL && strncmp(tok.literal, expected_lit[i], tok.lit_size)) {
            fprintf(stderr, "tests[%d] - literal wrong. expected='%s', got='%s'\n", i,
                    expected_lit[i], tok.literal);
            return false;
        }
    }

    arena_free(&a);
    return success;
}

int main(void)
{
    return test_next_token();
}
