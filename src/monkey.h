#ifndef _MONKEY_H_
#define _MONKEY_H_

#include "logger.h"

#include "arena.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define UNUSED(x) (void)x
// TODO: implement a better logging system with error and fatal
#define UNIMPLEMENTED(message)                                                                     \
    do {                                                                                           \
        fprintf(stderr, "%s:%d: TODO: %s\n", __FILE__, __LINE__, message);                         \
        abort();                                                                                   \
    } while (0)

extern Arena global_arena;

// token.c
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
    TT_COUNT,
} TokenType;

static_assert(TT_COUNT == 27);
#define TOKEN_TYPE_COUNT 27

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
    Token token;
    char *value;
    int value_size;
} Identifier;

typedef enum {
    ET_IDENT,
} ExprType;

typedef struct {
    ExprType type;
    union {
        Identifier ident;
    };
} Expression;

typedef struct {
    Token token;
    Identifier *name;
    // TODO: refactor this at some point
    Expression *value;
} LetStmt;

typedef struct {
    Token token;
    // TODO: refactor this at some point
    Expression *value;
} ReturnStmt;

typedef struct {
    Token token;
    Expression expr;
} ExprStmt;

typedef enum {
    ST_LET,
    ST_RETURN,
    ST_EXPR,
} StmtType;

typedef struct {
    StmtType type;
    void *data;
} StmtV2;

typedef struct {
    StmtV2 *items;
    int count;
    int capacity;
} Program;

char *program_token_literal(Program *prog);
void program_append_stmt(Program *prog, StmtV2 st);
char *program_string(Program *prog);
void letstmt_stmt_node(LetStmt *ls);
char *letstmt_token_lit(LetStmt *ls);
char *letstmt_string(LetStmt *ls);
void ident_stmt_node(Identifier *ident);
char *ident_token_lit(Identifier *ident);
const char *st_type_to_str(StmtType st_type);
const char *et_type_to_str(ExprType et_type);
void returnstmt_stmt_node(ReturnStmt *rs);
char *returnstmt_token_lit(ReturnStmt *rs);
char *returnstmt_string(ReturnStmt *rs);
void exprstmt_stmt_node(ExprStmt *es);
char *exprstmt_token_lit(ExprStmt *es);
char *expr_string(Expression *expr);

// parser.c
typedef struct {
    char **items;
    int count;
    int capacity;
} ErrorList;

// Forward declaration
typedef struct Parser Parser;
typedef Expression (*PrefixParseFuncPtr)(Parser*);
typedef Expression (*InfixParseFuncPtr)(Parser*, Expression);

typedef struct Parser {
    Lexer *lexer;
    Token curr_token;
    Token peek_token;
    ErrorList errors;
    PrefixParseFuncPtr prefix_parse_func[TOKEN_TYPE_COUNT];
    InfixParseFuncPtr infix_parse_func[TOKEN_TYPE_COUNT];
} Parser;

typedef enum {
    EPO_LOWEST = 1,
    EPO_EQUALS,       // ==
    EPO_LESSGREATER,  // > or <
    EPO_SUM,          // +
    EPO_PRODUCT,      // *
    EPO_PREFIX,       // -X or !X
    EPO_CALL,         // myFunction(X)
} ExprParseOrder;

void parser_init(Parser *p, Lexer *l);
void parser_next_token(Parser *p);
bool parser_parse_program(Parser *p, Program *progs);
void *parser_parse_stmt(Parser *p, StmtType *st_type);
bool parser_parse_let_stmt(Parser *p, LetStmt *ls);
bool parser_parse_return_stmt(Parser *p, ReturnStmt *rs);
bool parser_parse_expr_stmt(Parser *p, ExprStmt *es);
void parser_parse_expr(Parser *p, ExprParseOrder epo, Expression *expr);
Expression parser_parse_identifier(Parser *p);
bool parser_curr_token_is(Parser *p, TokenType tt);
bool parser_peek_token_is(Parser *p, TokenType tt);
bool parser_expect_peek(Parser *p, TokenType tt);
void parser_add_error(Parser *p, char *err);
void parser_peek_error(Parser *p, TokenType tt);
void parser_register_prefix(Parser *p, TokenType tt, PrefixParseFuncPtr pre_fn);
void parser_register_infix(Parser *p, TokenType tt, InfixParseFuncPtr in_fn);

#endif // _MONKEY_H_
