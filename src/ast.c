#include "monkey.h"
#include <stdio.h>
#include <string.h>

char *program_token_literal(Program *prog)
{
    if (prog->count > 0) {
        UNIMPLEMENTED("finish program_token_literal() (pg. 34)");
        // return prog->stmts[0].nvt->token_lit_func_ptr();
        // return ((StmtVT *)prog->items[0].data)->nvt->token_lit_func_ptr();
    } else {
        return NULL;
    }
}

#define PROG_INIT_CAP 20
void program_append_stmt(Program *prog, StmtV2 st)
{
    // TODO: maybe replace this with arena's da_append macro

    // Ensure enough mem is available to append
    int expected_cap = prog->count + 1;
    int old_cap = prog->capacity;
    if (expected_cap > prog->capacity) {
        if (prog->capacity == 0) {
            prog->capacity = PROG_INIT_CAP;
        }
        while (expected_cap > prog->capacity) {
            prog->capacity *= 2;
        }
        prog->items = arena_realloc(&global_arena, prog->items, old_cap * sizeof(*prog->items),
                                    prog->capacity * sizeof(*prog->items));
        if (prog->items == NULL) {
            fprintf(stderr, "ERROR: Unable to allocate new statements\n");
            exit(EXIT_FAILURE);
        }
    }
    // Append stmt
    prog->items[prog->count] = st;
    prog->count += 1;
}

char *program_string(Program *prog)
{
    char buf[8*1024] = {0};
    for (int i = 0; i < prog->count; i++) {
        StmtV2 stmt = prog->items[i];
        char *stmt_str = NULL;
        switch (stmt.type) {
            case ST_LET:
                stmt_str = letstmt_string(stmt.data);
                break;
            case ST_RETURN:
                stmt_str = returnstmt_string(stmt.data);
                break;
            default:
                UNIMPLEMENTED("finish implementing program_string()");
        }
        strncat(buf, stmt_str, strlen(stmt_str));
    }

    // Copy string to heap
    return arena_memdup(&global_arena, buf, strlen(buf));
}

void letstmt_stmt_node(LetStmt *ls)
{
    UNUSED(ls);
}

char *letstmt_token_lit(LetStmt *ls)
{
    return ls->token.literal;
}

char *letstmt_string(LetStmt *ls)
{
    char buf[2*1024] = {0};
    snprintf(
        buf, 2*1024, "%s %s = %s;",
        letstmt_token_lit(ls),
        ls->name->value,
        expr_string(ls->value)
    );
    return arena_memdup(&global_arena, buf, strlen(buf));
}

void ident_stmt_node(Identifier *ident)
{
    UNUSED(ident);
}

char *ident_token_lit(Identifier *ident)
{
    return ident->token.literal;
}

char *ident_string(Identifier *ident)
{
    return ident_token_lit(ident);
}

void returnstmt_stmt_node(ReturnStmt *rs)
{
    UNUSED(rs);
}

char *returnstmt_token_lit(ReturnStmt *rs)
{
    return rs->token.literal;
}

char *returnstmt_string(ReturnStmt *rs)
{
    char buf[2*1024] = {0};
    snprintf(
        buf, 2*1024, "%s %s;",
        returnstmt_token_lit(rs),
        expr_string(rs->value)
    );
    return arena_memdup(&global_arena, buf, strlen(buf));
}

void exprstmt_stmt_node(ExprStmt *es)
{
    UNUSED(es);
}

char *exprstmt_token_lit(ExprStmt *es)
{
    return es->token.literal;
}

char *expr_string(Expression *expr)
{
    if (expr != NULL) {
        switch (expr->type) {
            case ET_IDENT:
                return expr->ident.value;
            default:
                return NULL;
        }
    }
    return NULL;
}

const char *st_type_to_str(StmtType st_type)
{
    switch (st_type) {
        case ST_LET:
            return "ST_LET";
        case ST_RETURN:
            return "ST_RETURN";
        case ST_EXPR:
            return "ST_EXPR";
        default: {
            fprintf(stderr, "Unknown stmt type: %d\n", st_type);
            exit(1);
        }
    }
}

const char *et_type_to_str(ExprType et_type)
{
    switch (et_type) {
        case ET_IDENT:
            return "ET_IDENT";
        default: {
            fprintf(stderr, "Unknown expr type: %d\n", et_type);
            exit(1);
        }
    }
}
