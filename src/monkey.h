#ifndef _MONKEY_H_
#define _MONKEY_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
    // Special
    TT_ILLEGAL,
    TT_EOF,

    // Identifiers + literals
    TT_IDENT, // add, foobar, x, y, ...
    TT_INT,

    // Operators
    TT_ASSIGN,
    TT_PLUS,

    // Delimiters
    TT_COMMA,
    TT_SEMICOLON,

    TT_LPAREN,
    TT_RPAREN,
    TT_LBRACE,
    TT_RBRACE,

    // Keywords
    TT_FUNCTION,
    TT_LET,
} TokenType;

typedef struct {
    TokenType type;
    char literal;
} Token;

extern const char *TT_STR[14];

Token token_new(TokenType type, char ch);
const char *tt_to_str(TokenType tt);

typedef struct {
    char *input;
    int input_size;
    // Current position in input (points to current char)
    int position;
    // Current reading position in input (after current char)
    int read_position;
    // Current char under examination
    char ch;
} Lexer;

void lexer_new(Lexer *l, char *input, int input_size);
void lexer_read_char(Lexer *l);
Token lexer_next_token(Lexer *l);

#endif // _MONKEY_H_
