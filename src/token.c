#include "monkey.h"

Token token_new(TokenType type, char *ch)
{
    return (Token) {
        .type = type,
        .lit_size = 1,
        .literal = ch
    };
}

const char *tt_to_str(TokenType tt)
{
    switch (tt) {
        case TT_ILLEGAL: return "ILLEGAL";
        case TT_EOF: return "EOF";
        case TT_IDENT: return "IDENT";
        case TT_INT: return "INT";
        case TT_ASSIGN: return "=";
        case TT_PLUS: return "+";
        case TT_COMMA: return ",";
        case TT_SEMICOLON: return ";";
        case TT_LPAREN: return "(";
        case TT_RPAREN: return ")";
        case TT_LBRACE: return "{";
        case TT_RBRACE: return "}";
        case TT_FUNCTION: return "FUNCTION";
        case TT_LET: return "LET";
        default:
            fprintf(stderr, "ERROR: Unknown token");
            exit(EXIT_FAILURE);
            break;
    }
}

TokenType token_lookup_ident(const char *ident)
{
    if (!strncmp(ident, "fn", 2)) {
        return TT_FUNCTION;
    } else if (!strncmp(ident, "let", 3)) {
        return TT_LET;
    }
    return TT_IDENT;
}
