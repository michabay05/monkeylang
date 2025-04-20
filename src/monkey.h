#ifndef _MONKEY_H_
#define _MONKEY_H_

#include "arena.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// token.c
typedef enum
{
    // Special
    TT_ILLEGAL,
    TT_EOF,

    // Identifiers + literals
    TT_IDENT, // add, foobar, x, y, ...
    TT_INT,

    // Operators
    TT_ASSIGN,
    TT_PLUS,
    TT_MINUS,
    TT_BANG,
    TT_ASTERISK,
    TT_SLASH,
    TT_LT,
    TT_GT,
    TT_EQ,
    TT_NOT_EQ,

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
    TT_TRUE,
    TT_FALSE,
    TT_IF,
    TT_ELSE,
    TT_RETURN,
} TokenType;

typedef struct {
    TokenType type;
    int lit_size;
    char *literal;
} Token;

Token token_new(TokenType type, char *ch);
const char *tt_to_str(TokenType tt);
TokenType token_lookup_ident(const char *ident);

// lexer.c
typedef struct {
    char *input;
    int input_size;
    // Current position in input (points to current char)
    int position;
    // Current reading position in input (after current char)
    int read_position;
    // Current char under examination
    char ch;
    Arena arena;
} Lexer;

void lexer_init(Lexer *lex, char *input, int input_size);
void lexer_read_char(Lexer *lex);
Token lexer_next_token(Lexer *lex);
char *lexer_read_identifier(Lexer *lex, int *lit_size);
void lexer_skip_whitespace(Lexer *lex);
char *lexer_read_number(Lexer *lex, int *lit_size);
char lexer_peek_char(Lexer *lex);

// ast.c
typedef struct {
    char* (*token_lit_func_ptr)(void);
} NodeVT;

typedef struct {
    NodeVT *nvt;
    void (*stmt_node_func_ptr)(void);
} StatementVT;

typedef struct {
    NodeVT *nvt;
    void (*expr_node_func_ptr)(void);
} ExpressionVT;

typedef struct {
    Arena arena;
    StatementVT *stmts;
    int stmt_count;
    int stmt_cap;
} Program;

typedef struct {
    Token token;
    char *value;
    int value_size;
} Identifier;

typedef struct {
    Token token;
    Identifier *name;
    ExpressionVT value;
} LetStmt;

char *program_token_literal(Program *prog);

// parser.c
typedef struct {
    Lexer *lexer;
    Token curr_token;
    Token peek_token;
} Parser;

void parser_init(Parser *p, Lexer *l);
void parser_next_token(Parser *p);
bool parser_parse_program(Parser *p, Program *progs);
StatementVT parser_parse_stmt(Parser *p);
bool parser_parse_let_stmt(Parser *p, LetStmt *ls);
bool parser_curr_token_is(Parser *p, TokenType tt);
bool parser_peek_token_is(Parser *p, TokenType tt);
bool parser_expect_peek(Parser *p, TokenType tt);

#endif // _MONKEY_H_
