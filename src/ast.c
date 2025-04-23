#include "monkey.h"

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

void letstmt_stmt_node(LetStmt *ls)
{
    UNUSED(ls);
}

char *letstmt_token_lit(LetStmt *ls)
{
    return ls->token.literal;
}

void ident_stmt_node(Identifier *ident)
{
    UNUSED(ident);
}
char *ident_token_lit(Identifier *ident)
{
    return ident->token.literal;
}

const char *st_type_to_str(StmtType st_type)
{
    switch (st_type) {
        case ST_LET:
            return "LET";
        default: {
            // TODO: add proper handling for unknown type
            return NULL;
        }
    }
}
