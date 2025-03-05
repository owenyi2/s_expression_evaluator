#ifndef ATOM_H
#define ATOM_H

#include "lexer.h"

enum EvalAtomType {
    EVAL_ATOM_NUMBER, /* aka double */
    EVAL_ATOM_IDENTIFIER, 
    EVAL_ATOM_FUNC,
};

enum Func {
    FUNC_PLUS,
    FUNC_MINUS,
    FUNC_TIMES,
    FUNC_DIVIDE,
    FUNC_POW,
    FUNC_SETQ,
    FUNC_SIN,
    FUNC_COS, 
    FUNC_TAN,
    FUNC_EXP,
    FUNC_LOG,
};

typedef struct EvalAtom {
    enum EvalAtomType type;
    union {
        enum Func func;
        double number;
        char* identifier;
    };
    int num_args;
} EvalAtom;

void ea_free(void* atom) {
    EvalAtom* eval_atom = (EvalAtom*) atom;
    if (eval_atom->type == EVAL_ATOM_IDENTIFIER) {
        free(eval_atom->identifier);
    }
    free(eval_atom);
}

EvalAtom* ea_from_token(Token* token) {
    EvalAtom* eval_atom = malloc(sizeof(EvalAtom));
    switch (token->type) {
        case PLUS:
            eval_atom->type = EVAL_ATOM_FUNC;
            eval_atom->func = FUNC_PLUS;
            eval_atom->num_args = 2;
            break;
        case MINUS:
            eval_atom->type = EVAL_ATOM_FUNC;
            eval_atom->func = FUNC_MINUS;
            eval_atom->num_args = 2;
            break;
        case TIMES:
            eval_atom->type = EVAL_ATOM_FUNC;
            eval_atom->func = FUNC_TIMES;
            eval_atom->num_args = 2;
            break;
        case DIVIDE:
            eval_atom->type = EVAL_ATOM_FUNC;
            eval_atom->func = FUNC_DIVIDE;
            eval_atom->num_args = 2;
            break;
        case POW:
            eval_atom->type = EVAL_ATOM_FUNC;
            eval_atom->func = FUNC_POW;
            eval_atom->num_args = 2;
            break;
        case SETQ:
            eval_atom->type = EVAL_ATOM_FUNC;
            eval_atom->func = FUNC_SETQ;
            eval_atom->num_args = 2;
            break;
        case SIN:
            eval_atom->type = EVAL_ATOM_FUNC;
            eval_atom->func = FUNC_SIN;
            eval_atom->num_args = 1;
            break;
        case COS:
            eval_atom->type = EVAL_ATOM_FUNC;
            eval_atom->func = FUNC_COS;
            eval_atom->num_args = 1;
            break;
        case TAN:
            eval_atom->type = EVAL_ATOM_FUNC;
            eval_atom->func = FUNC_TAN;
            eval_atom->num_args = 1;
            break;
        case EXP:
            eval_atom->type = EVAL_ATOM_FUNC;
            eval_atom->func = FUNC_EXP;
            eval_atom->num_args = 1;
            break;
        case LOG:
            eval_atom->type = EVAL_ATOM_FUNC;
            eval_atom->func = FUNC_LOG;
            eval_atom->num_args = 1;
            break;
        case NUMBER:
            eval_atom->type = EVAL_ATOM_NUMBER;
            eval_atom->number = token->number;
            break;
        case IDENTIFIER:
            eval_atom->type = EVAL_ATOM_IDENTIFIER;
            eval_atom->identifier = token->identifier;
            break;
        default:
            fprintf(stderr, "ea_from_token: unable to convert token to atom\n");
            free(eval_atom);
            return NULL;
    }
    return eval_atom; 
}

void debug_eval_atom(void* atom) {
    EvalAtom* eval_atom = (EvalAtom*) atom;
    switch (eval_atom->type) {
        case EVAL_ATOM_NUMBER:
            printf("number[%f]", eval_atom->number);
            break;
        case EVAL_ATOM_IDENTIFIER:
            printf("identifier[%s]", eval_atom->identifier);
            break;
        case EVAL_ATOM_FUNC: {
            char func_name[10];
            switch (eval_atom->func) {
                case FUNC_PLUS:
                    strcpy(func_name, "+");
                    break;
                case FUNC_MINUS:
                    strcpy(func_name, "-");
                    break;
                case FUNC_TIMES:
                    strcpy(func_name, "*");
                    break;
                case FUNC_DIVIDE:
                    strcpy(func_name, "/");
                    break;
                case FUNC_POW:
                    strcpy(func_name, "^");
                    break;
                case FUNC_SETQ:
                    strcpy(func_name, "setq");
                    break;
                case FUNC_SIN:
                    strcpy(func_name, "sin");
                    break;
                case FUNC_COS:
                    strcpy(func_name, "cos");
                    break;
                case FUNC_TAN:
                    strcpy(func_name, "tan");
                    break;
                case FUNC_EXP:
                    strcpy(func_name, "exp");
                    break;
                case FUNC_LOG:
                    strcpy(func_name, "log");
                    break;
                
            }
            printf("function[%s, %d]", func_name, eval_atom->num_args);
        }
    }
}

#endif
