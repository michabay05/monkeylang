#include "../monkey.h"

bool TestNextToken() {
    char *input = "=+(){},;";
    const TokenType expected_tt[] = {
        TT_ASSIGN,
        TT_PLUS,
        TT_LPAREN,
        TT_RPAREN,
        TT_LBRACE,
        TT_RBRACE,
        TT_COMMA,
        TT_SEMICOLON,
        TT_EOF,
    };
    const char expected_lit[] = {
        '=', '+', '(', ')', '{', '}', ',', ';', 0
    };
    const int N = 9;

    Lexer l = {0};
    lexer_new(&l, input, strlen(input));
    bool success = true;
    for (int i = 0; i < N; i++) {
        Token tok = lexer_next_token(&l);

        if (tok.type != expected_tt[i]) {
            fprintf(
                stderr,
                "tests[%d] - tokentype wrong. expected=\"%s\", got=\"%s\"\n",
                i, tt_to_str(expected_tt[i]), tt_to_str(tok.type)
            );
            success = false;
        }

        if (tok.literal != expected_lit[i]) {
            fprintf(
                stderr,
                "tests[%d] - literal wrong. expected='%c', got='%c'\n",
                i, expected_lit[i], tok.literal
            );
            success = false;
        }
    }

    return success;
}

int main(void)
{
    if (TestNextToken()) {
        printf("All tests passed!\n");
    } else {
        return EXIT_FAILURE;
    }
}
