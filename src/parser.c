#include "monkey.h"
#include <string.h>

void parser_init(Parser *p, Lexer *l)
{
    *p = (Parser){0};
    p->lexer = l;

    // Read two tokens, so curToken and peekToken are both set
    parser_next_token(p);
    parser_next_token(p);
}

void parser_next_token(Parser *p)
{
    p->curr_token = p->peek_token;
    p->peek_token = lexer_next_token(p->lexer);
}

bool parser_parse_program(Parser *p, Program *progs)
{
    *progs = (Program){0};

    while (p->curr_token.type != TT_EOF) {
        StmtType st_type;
        void *stmt = parser_parse_stmt(p, &st_type);
        if (stmt != NULL) {
            program_append_stmt(progs, (StmtV2){st_type, stmt});
        } else {
            fprintf(stderr, "ERROR: stmt parser returned NULL\n");
        }
        parser_next_token(p);
    }

    return true;
}

void *parser_parse_stmt(Parser *p, StmtType *st_type)
{
    // Send a void ptr with an enum of what type the data in the void ptr is
    switch (p->curr_token.type) {
        case TT_LET: {
            LetStmt *ls = arena_alloc(&global_arena, sizeof(LetStmt));
            if (parser_parse_let_stmt(p, ls)) {
                *st_type = ST_LET;
                return ls;
            } else {
                fprintf(stderr, "ERROR: failed to parse let stmt\n");
                return NULL;
            }
        }
        default:
            fprintf(stderr, "TODO: implement parsing for other token types (pg. 41)\n");
            exit(123);
    }
}

bool parser_parse_let_stmt(Parser *p, LetStmt *ls)
{
    *ls = (LetStmt){0};
    ls->token = p->curr_token;

    if (!parser_expect_peek(p, TT_IDENT)) {
        return false;
    }

    ls->name = arena_alloc(&global_arena, sizeof(Identifier));
    *ls->name = (Identifier){
        .token = p->curr_token,
        .value = p->curr_token.literal,
        .value_size = p->curr_token.lit_size,
    };

    if (!parser_expect_peek(p, TT_ASSIGN)) {
        return false;
    }

    // TODO: We're skipping the expressions until we
    // encounter a semicolon
    while (!parser_curr_token_is(p, TT_SEMICOLON)) {
        parser_next_token(p);
    }

    return true;
}

bool parser_curr_token_is(Parser *p, TokenType tt)
{
    return p->curr_token.type == tt;
}

bool parser_peek_token_is(Parser *p, TokenType tt)
{
    return p->peek_token.type == tt;
}

bool parser_expect_peek(Parser *p, TokenType tt)
{
    if (parser_peek_token_is(p, tt)) {
        parser_next_token(p);
        return true;
    } else {
        return false;
    }
}
