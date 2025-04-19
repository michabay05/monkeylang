#include "monkey.h"

void lexer_new(Lexer *l, char *input, int input_size)
{
    *l = (Lexer) {0};
    l->input = input;
    l->input_size = input_size;
    lexer_read_char(l);
}

void lexer_read_char(Lexer *l)
{
    if (l->read_position >= l->input_size) {
        l->ch = 0;
    } else {
        l->ch = l->input[l->read_position];
    }
    l->position = l->read_position;
    l->read_position += 1;
}

Token lexer_next_token(Lexer *l)
{
    Token tok = {0};

    switch (l->ch) {
        case '=':
            tok = token_new(TT_ASSIGN, l->ch);
            break;
        case ';':
            tok = token_new(TT_SEMICOLON, l->ch);
            break;
        case '(':
            tok = token_new(TT_LPAREN, l->ch);
            break;
        case ')':
            tok = token_new(TT_RPAREN, l->ch);
            break;
        case ',':
            tok = token_new(TT_COMMA, l->ch);
            break;
        case '+':
            tok = token_new(TT_PLUS, l->ch);
            break;
        case '{':
            tok = token_new(TT_LBRACE, l->ch);
            break;
        case '}':
            tok = token_new(TT_RBRACE, l->ch);
            break;
        case 0:
            tok = (Token) {
                .literal = 0,
                .type = TT_EOF,
            };
            break;
        default:
            fprintf(stderr, "ERROR: Unknown char: '%c'", l->ch);
            break;
    }

    lexer_read_char(l);
    return tok;
}
