#include <stdio.h>
#include <stdlib.h>
#define ARENA_IMPLEMENTATION
#include "monkey.h"
#include <string.h>

#define BUF_SIZE 8 * 1024

static void repl_start(FILE *in, FILE *out)
{
    char buf[BUF_SIZE] = {0};
    Lexer lex = {0};
    while (true) {
        printf(">> ");
        if (fgets(buf, BUF_SIZE, in) == NULL)
            break;
        if (!strncmp(buf, "quit", 4))
            break;

        int n = (int)strlen(buf);
        lexer_init(&lex, buf, n);

        // Iterate through all tokens
        Token tok = lexer_next_token(&lex);
        while (tok.type != TT_EOF) {
            fprintf(out, "{ type: %s, lit_size: %d, literal: '%s' }\n", tt_to_str(tok.type),
                    tok.lit_size, tok.literal);
            tok = lexer_next_token(&lex);
        }
        memset(buf, 0, BUF_SIZE);
    }
}

int main(void)
{
    const char *user_name = getenv("USER");
    printf("Hello %s! This is the Monkey programming language!\n", user_name);
    printf("Feel free to type in commands\n");
    repl_start(stdin, stdout);
    return 0;
}
