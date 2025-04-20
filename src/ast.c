#include "monkey.h"

char *program_token_literal(Program *prog)
{
    if (prog->stmt_count > 0) {
        return prog->stmts[0].nvt->token_lit_func_ptr();
    } else {
        return NULL;
    }
}

void letstmt_stmt_node(LetStmt *ls) {}
char *letstmt_token_lit(LetStmt *ls)
{
    return ls->token.literal;
}

void ident_stmt_node(Identifier *ident) {}
char *ident_token_lit(Identifier *ident)
{
    return ident->token.literal;
}
