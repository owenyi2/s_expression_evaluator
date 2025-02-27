#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "snode.h"
#include "atom.h"

// Bug is in lexer
// when there is multiple wihtespace between e.g. (1  1)
// lexer returns 1 on unknown symbol
// because lexer returns error code and ps_next_token does not handle the error, then it returnsbut leaves the Parser->curr_token unmodified. 
// This causes the parser to parse a duplicate token
// This was because we forgot to put a break; 

typedef struct Parser {
    Lexer* lx;   
    Token curr_token;
} Parser;

void ps_next_token(Parser* ps) {
    fprintf(stderr, "%d\n", lx_next_token(ps->lx, &(ps->curr_token)));
    fprintf(stderr, "parser: ");
    debug_token(ps->curr_token);  
}

Parser* ps_new(Lexer* lx) {
    Parser* ps = malloc(sizeof(Parser));
    ps->lx = lx;
    ps_next_token(ps);
    return ps;
}
int ps_accept(Parser* ps, enum TokenType token_type, Token* return_token) {
    if (token_type == ps->curr_token.type) {
        ps_next_token(ps);
        return 1;
    }
    return 0;
    // call accept when we can accept multiple possible syntactically correct tokens following 
    // call expect when there is only one syntactically correct token following
}

int ps_accept_atom(Parser* ps, Token* return_token) { 
    switch (ps->curr_token.type) {
        case PLUS:
        case MINUS:
        case TIMES:
        case DIVIDE:
        case POW:
        case SETQ:
        case SIN:
        case COS:
        case TAN:
        case EXP:
        case LOG:
        case NUMBER:
        case IDENTIFIER:
            fprintf(stderr, "ps_accept_atom\n");
            *return_token = ps->curr_token;
            ps_next_token(ps);
            return 1;
        default:
            return 0;
    }
}

int ps_expect(Parser* ps, enum TokenType token_type, Token* return_token) {
    if (ps_accept(ps, token_type, return_token)) {
        return 1;
    }
    fprintf(stderr, "ps_expect error\n");
    return 0;
}

void ps_atom(Parser* ps, SNode** snode) {
    Token token;
    if (ps_accept_atom(ps, &token)) {
        // THIS IS WHERE THE BUG IS maybe?
        // We call ps_accept_atom idk
        *snode = sn_new_atom(ea_from_token(&token));
    } else {
        fprintf(stderr, "ps_atom: syntax error\n");
        // ps_next_token(ps); 
    }
}

void ps_sexpr(Parser* ps, SNode** snode) {
    // fprintf(stderr, "%d\n", ps->curr_token.type);
    if (ps_accept(ps, LPAREN, NULL)) {
        *snode = sn_new_empty();
        SNode** current = &((*snode)->list);
        while ((ps->curr_token.type != RPAREN) && (ps->curr_token.type != END)) {
            ps_sexpr(ps, current);
            current = &((*current)->next);
        }
        ps_expect(ps, RPAREN, NULL);
    } else {
        ps_atom(ps, snode);
    }
}

void ps_parse(Parser* ps, SNode** snode) {
    ps_sexpr(ps, snode);
    ps_expect(ps, END, NULL);
}

#endif
