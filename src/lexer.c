#include "arena.h"
#include "monkey.h"
#include <string.h>

static bool _is_letter(char ch);
static bool _is_digit(char ch);

void lexer_init(Lexer *lex, char *input, int input_size)
{
    *lex = (Lexer){0};
    lex->input = input;
    lex->input_size = input_size;
    lex->arena = (Arena){0};
    lexer_read_char(lex);
}

void lexer_read_char(Lexer *lex)
{
    if (lex->read_position >= lex->input_size) {
        lex->ch = 0;
    } else {
        lex->ch = lex->input[lex->read_position];
    }
    lex->position = lex->read_position;
    lex->read_position += 1;
}

Token lexer_next_token(Lexer *lex)
{
    Token tok = {0};

    lexer_skip_whitespace(lex);
    char *lit_ch = arena_memdup(&lex->arena, lex->input + lex->position, 1);

    switch (lex->ch) {
        case '=': {
            if (lexer_peek_char(lex) == '=') {
                int pos = lex->position;
                lexer_read_char(lex);
                char *lit = arena_memdup(&lex->arena, lex->input + pos, 2);
                tok = token_new(TT_EQ, lit);
            } else {
                tok = token_new(TT_ASSIGN, lit_ch);
            }
        } break;
        case '!': {
            if (lexer_peek_char(lex) == '=') {
                int pos = lex->position;
                lexer_read_char(lex);
                char *lit = arena_memdup(&lex->arena, lex->input + pos, 2);
                tok = token_new(TT_NOT_EQ, lit);
            } else {
                tok = token_new(TT_BANG, lit_ch);
            }
        } break;
        case '+':
            tok = token_new(TT_PLUS, lit_ch);
            break;
        case '-':
            tok = token_new(TT_MINUS, lit_ch);
            break;
        case '*':
            tok = token_new(TT_ASTERISK, lit_ch);
            break;
        case '/':
            tok = token_new(TT_SLASH, lit_ch);
            break;
        case '<':
            tok = token_new(TT_LT, lit_ch);
            break;
        case '>':
            tok = token_new(TT_GT, lit_ch);
            break;
        case ';':
            tok = token_new(TT_SEMICOLON, lit_ch);
            break;
        case ',':
            tok = token_new(TT_COMMA, lit_ch);
            break;
        case '(':
            tok = token_new(TT_LPAREN, lit_ch);
            break;
        case ')':
            tok = token_new(TT_RPAREN, lit_ch);
            break;
        case '{':
            tok = token_new(TT_LBRACE, lit_ch);
            break;
        case '}':
            tok = token_new(TT_RBRACE, lit_ch);
            break;
        case 0:
            tok = (Token){.type = TT_EOF, .lit_size = 0, .literal = NULL};
            break;
        default:
            if (_is_letter(lex->ch)) {
                tok.literal = lexer_read_identifier(lex, &tok.lit_size);
                tok.type = token_lookup_ident(tok.literal);
                return tok;
            } else if (_is_digit(lex->ch)) {
                tok.literal = lexer_read_number(lex, &tok.lit_size);
                tok.type = TT_INT;
                return tok;
            } else {
                tok = token_new(TT_ILLEGAL, lit_ch);
            }
            break;
    }

    lexer_read_char(lex);
    return tok;
}

char *lexer_read_identifier(Lexer *lex, int *lit_size)
{
    int position = lex->position;
    while (_is_letter(lex->ch)) {
        lexer_read_char(lex);
        *lit_size += 1;
    }
    int size = lex->position - position;
    return arena_memdup(&lex->arena, lex->input + position, size);
}

char *lexer_read_number(Lexer *lex, int *lit_size)
{
    int position = lex->position;
    while (_is_digit(lex->ch)) {
        lexer_read_char(lex);
        *lit_size += 1;
    }
    int size = lex->position - position;
    return arena_memdup(&lex->arena, lex->input + position, size);
}

void lexer_skip_whitespace(Lexer *lex)
{
    while (lex->ch == ' ' || lex->ch == '\t' || lex->ch == '\n' || lex->ch == '\r') {
        lexer_read_char(lex);
    }
}

char lexer_peek_char(Lexer *lex)
{
    if (lex->read_position >= lex->input_size) {
        return 0;
    } else {
        return lex->input[lex->read_position];
    }
}

static bool _is_letter(char ch)
{
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || (ch == '_');
}

static bool _is_digit(char ch)
{
    return '0' <= ch && ch <= '9';
}
