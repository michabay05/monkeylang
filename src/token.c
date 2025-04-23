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
        case TT_ASSIGN: return "ASSIGN";
        case TT_PLUS: return "PLUS";
        case TT_MINUS: return "MINUS";
        case TT_BANG: return "BANG";
        case TT_ASTERISK: return "ASTERISK";
        case TT_SLASH: return "SLASH";
        case TT_LT: return "LT";
        case TT_GT: return "GT";
        case TT_EQ: return "EQ";
        case TT_NOT_EQ: return "NOT_EQ";
        case TT_COMMA: return "COMMA";
        case TT_SEMICOLON: return "SEMICOLON";
        case TT_LPAREN: return "LPAREN";
        case TT_RPAREN: return "RPAREN";
        case TT_LBRACE: return "LBRACE";
        case TT_RBRACE: return "RBRACE";
        case TT_FUNCTION: return "FUNCTION";
        case TT_LET: return "LET";
        case TT_TRUE: return "TRUE";
        case TT_FALSE: return "FALSE";
        case TT_IF: return "IF";
        case TT_ELSE: return "ELSE";
        case TT_RETURN: return "RETURN";
        default:
            fprintf(stderr, "ERROR: Unknown token\n");
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
    } else if (!strncmp(ident, "true", 3)) {
        return TT_TRUE;
    } else if (!strncmp(ident, "false", 3)) {
        return TT_FALSE;
    } else if (!strncmp(ident, "if", 3)) {
        return TT_IF;
    } else if (!strncmp(ident, "else", 3)) {
        return TT_ELSE;
    } else if (!strncmp(ident, "return", 3)) {
        return TT_RETURN;
    }
    return TT_IDENT;
}
