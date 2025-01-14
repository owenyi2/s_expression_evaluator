#include <ctype.h>
#include <math.h>

#include "hashmap.h"

/* we honestly should do the ifndef thing to include snode.h. sigh */

enum EvalAtomType {
    NUMBER, /* aka double */
    IDENTIFIER, 
    BINOP, /* e.g. +, -, *, / */
    UNOP, /* e.g. sin, cos, tan, exp, log */
};

enum Binop {
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
    POW,
    SETQ,
};

enum Unop {
    SIN,
    COS, 
    TAN,
    EXP,
    LOG,
};

typedef struct EvalAtom {
    enum EvalAtomType type;
    union {
        enum Binop binop;
        enum Unop unop;
        double number;
        char* identifier;
    };
} EvalAtom;

int is_double(const char* s, double *dest) {
    if (s == NULL) {
        return 0;
    } 
    char* endptr;
    *dest = (double) strtod(s, &endptr);
    if (s == endptr) {
        return 0; 
    }

    while (isspace((char) *endptr)) {
        endptr++;
    }
    return *endptr == '\0';
}

EvalAtom* ea_from_string(char* input) {
    /* replace with a hash and verify strategy later */ 
    EvalAtom* eval_atom;
    if (!(strcmp(input, "+"))) {
        eval_atom = malloc(sizeof(EvalAtom));
        *eval_atom = (EvalAtom) { .type = BINOP, .binop = PLUS }; 
    } else if (!(strcmp(input, "-"))) {
        eval_atom = malloc(sizeof(EvalAtom));
        *eval_atom = (EvalAtom) { .type = BINOP, .binop = MINUS }; 
    } else if (!(strcmp(input, "*"))) {
        eval_atom = malloc(sizeof(EvalAtom));
        *eval_atom = (EvalAtom) { .type = BINOP, .binop = TIMES }; 
    } else if (!(strcmp(input, "/"))) {
        eval_atom = malloc(sizeof(EvalAtom));
        *eval_atom = (EvalAtom) { .type = BINOP, .binop = DIVIDE }; 
    } else if (!(strcmp(input, "^"))) {
        eval_atom = malloc(sizeof(EvalAtom));
        *eval_atom = (EvalAtom) { .type = BINOP, .binop = POW }; 
    } else if (!(strcmp(input, "setq"))) {
        eval_atom = malloc(sizeof(EvalAtom));
        *eval_atom = (EvalAtom) { .type = BINOP, .binop = SETQ }; 
    } else if (!(strcmp(input, "sin"))) {
        eval_atom = malloc(sizeof(EvalAtom));
        *eval_atom = (EvalAtom) { .type = UNOP, .unop = SIN }; 
    } else if (!(strcmp(input, "cos"))) {
        eval_atom = malloc(sizeof(EvalAtom));
        *eval_atom = (EvalAtom) { .type = UNOP, .unop = COS }; 
    } else if (!(strcmp(input, "tan"))) {
        eval_atom = malloc(sizeof(EvalAtom));
        *eval_atom = (EvalAtom) { .type = UNOP, .unop = TAN }; 
    } else if (!(strcmp(input, "exp"))) {
        eval_atom = malloc(sizeof(EvalAtom));
        *eval_atom = (EvalAtom) { .type = UNOP, .unop = EXP }; 
    } else if (!(strcmp(input, "log"))) {
        eval_atom = malloc(sizeof(EvalAtom));
        *eval_atom = (EvalAtom) { .type = UNOP, .unop = LOG }; 
    } else {
        double number;
        if (is_double(input, &number)) {
            eval_atom = malloc(sizeof(EvalAtom));
            *eval_atom = (EvalAtom) { .type = NUMBER, .number = number };
        } else {
            eval_atom = malloc(sizeof(EvalAtom));
            char* identifier = malloc(sizeof(input));
            strcpy(identifier, input); 
            *eval_atom = (EvalAtom) { .type = IDENTIFIER, .identifier = identifier };

            // fprintf(stderr, "ea_from_string: Unrecognised input\n");
            // eval_atom = NULL;
        }
    }
    return eval_atom;
}

void debug_eval_atom(void* atom) {
    EvalAtom* eval_atom = (EvalAtom*) atom;
    switch (eval_atom->type) {
        case NUMBER:
            printf("number[%f]", eval_atom->number);
            break;
        case IDENTIFIER:
            printf("identifier[%s]", eval_atom->identifier);
            break;
        case BINOP:
            switch (eval_atom->binop) {
                case PLUS:
                    printf("binop[+]");
                    break;
                case MINUS:
                    printf("binop[-]");
                    break;
                case TIMES:
                    printf("binop[*]");
                    break;
                case DIVIDE:
                    printf("binop[/]");
                    break;
                case POW:
                    printf("binop[^]");
                    break;
                case SETQ:
                    printf("binop[setq]");
                    break;
                default:
                    fprintf(stderr, "debug_eval_atom: Unhandled case");
                    exit(1);
            }
            break;
        case UNOP:
            switch (eval_atom->unop) {
                case SIN:
                    printf("unop[sin]");
                    break;
                case COS:
                    printf("unop[cos]");
                    break;
                case TAN:
                    printf("unop[tan]");
                    break;
                case EXP:
                    printf("unop[exp]");
                    break;
                case LOG:
                    printf("unop[log]");
                    break;
                default:
                    fprintf(stderr, "debug_eval_atom: Unhandled case");
                    exit(1);
            }
            break; 
    }
}

void _type_check_snode_recursive(SNode* snode, int first_element, int* _type_check_flag) {
    /* 
    Converts char* atoms into EvalAtom* atoms 
    Checks number of operands is correct

    Further type checks performed at runtime 
    */
    if (*_type_check_flag != 0) {
        return;
    }

    EvalAtom* eval_atom;
    if (snode->type == ATOM) {
        eval_atom = ea_from_string(snode->atom);
        if (eval_atom == NULL) {
            *_type_check_flag = 1;
            return; 
        }
        /* TODO: (ref 1) type check against cases e.g. (1 1) where list has no operator but allow (1) which evaluates to 1 */
        if (eval_atom->type == BINOP) {
            if (sn_next_count(snode) != 2) {
                fprintf(stderr, "_type_check_snode_recursive: Binop expects 2 operands\n");
                *_type_check_flag = 1;
                return;
            }
            if (first_element != 1) {
                fprintf(stderr, "_type_check_snode_recursive: Operator must be first element of list\n");
                *_type_check_flag = 1;
                return;
            }
        } else if (eval_atom->type == UNOP) {
            if (sn_next_count(snode) != 1) {
                fprintf(stderr, "_type_check_snode_recursive: Binop expects 2 operands\n");
                *_type_check_flag = 1;
                return;
            }
            if (first_element != 1) {
                fprintf(stderr, "_type_check_snode_recursive: Operator must be first element of list\n");
                *_type_check_flag = 1;
                return;
            }
        } else if (eval_atom->type == NUMBER) {
            if ((first_element == 1) && (sn_next_count(snode) > 1)) {
                fprintf(stderr, "_type_check_snode_recursive: Tuples of Numbers e.g. (1 1) are not allowed\n");
                *_type_check_flag = 1;
                return;
            }

        }
        free(snode->atom);
        snode->atom = eval_atom;
    } else {
        if (snode->list != NULL) {
            _type_check_snode_recursive(snode->list, 1, _type_check_flag);
        }
    }
    if (snode->next != NULL) {
        _type_check_snode_recursive(snode->next, 0, _type_check_flag);
    }
}

int type_check_snode(SNode* snode) {
    int type_check_flag = 0;
    _type_check_snode_recursive(snode, 1, &type_check_flag);
    return type_check_flag;
}

double _evaluate_snode_recursive(SNode* snode, int* _evaluate_flag, HashMap* environment) {
    if (*_evaluate_flag != 0) {
        return 0; /* This value is meaningless if _evaluate_flag triggered */
    }
    if (snode->type == LIST) {
        EvalAtom* first_atom = (EvalAtom*) snode->list->atom;

        switch (first_atom->type) {
            case BINOP:
                switch (first_atom->binop) {
                    case PLUS:
                        return _evaluate_snode_recursive(snode->list->next, _evaluate_flag, environment) + _evaluate_snode_recursive(snode->list->next->next, _evaluate_flag, environment); 
                    case MINUS:
                        return _evaluate_snode_recursive(snode->list->next, _evaluate_flag, environment) - _evaluate_snode_recursive(snode->list->next->next, _evaluate_flag, environment); 
                    case TIMES:
                        return _evaluate_snode_recursive(snode->list->next, _evaluate_flag, environment) * _evaluate_snode_recursive(snode->list->next->next, _evaluate_flag, environment); 
                    case DIVIDE:
                        return _evaluate_snode_recursive(snode->list->next, _evaluate_flag, environment) / _evaluate_snode_recursive(snode->list->next->next, _evaluate_flag, environment); 
                    case POW:
                        return pow(_evaluate_snode_recursive(snode->list->next, _evaluate_flag, environment), _evaluate_snode_recursive(snode->list->next->next, _evaluate_flag, environment)); 
                    case SETQ: {
                        EvalAtom* identifier = ((EvalAtom*)snode->list->next->atom);
                        if (identifier->type != IDENTIFIER) {
                            fprintf(stderr, "_evaluate_snode_recursive: SETQ expects IDENTIFIER as 1st argument)\n");
                            *_evaluate_flag = 1; 
                        }
                        double return_value = _evaluate_snode_recursive(snode->list->next->next, _evaluate_flag, environment); 
                        hm_insert(environment, identifier->identifier, &return_value); 
                        return return_value;
                    }
                    default:
                        fprintf(stderr, "_evaluate_snode_recursive: unhandled case\n");
                        *_evaluate_flag = 1;
                        return 0; /* This value is meaningless if _evaluate_flag triggered */
                }
            case UNOP:
                switch (first_atom->unop) {
                    case SIN:
                        return sin(_evaluate_snode_recursive(snode->list->next, _evaluate_flag, environment));
                    case COS:
                        return cos(_evaluate_snode_recursive(snode->list->next, _evaluate_flag, environment));
                    case TAN:
                        return tan(_evaluate_snode_recursive(snode->list->next, _evaluate_flag, environment));
                    case EXP:
                        return exp(_evaluate_snode_recursive(snode->list->next, _evaluate_flag, environment));
                    case LOG:
                        return log(_evaluate_snode_recursive(snode->list->next, _evaluate_flag, environment));
                }
            case NUMBER:
                return first_atom->number;
            case IDENTIFIER: {
                void* value = hm_get(environment, first_atom->identifier);
                if (value == NULL) {
                    fprintf(stderr, "unknown identifier\n");
                    *_evaluate_flag = 1;
                    return 0; /* This value is meaningless if _evaluate_flag triggered */
                }
                return *(double*) value; 
            }
        }
    } else {
        EvalAtom* atom = (EvalAtom*)snode->atom;
        if (atom->type == NUMBER) {
            return atom->number;
        } else if (atom->type == IDENTIFIER) {
            void* value = hm_get(environment, atom->identifier);
            if (value == NULL) {
                fprintf(stderr, "unknown identifier\n");
                *_evaluate_flag = 1;
                return 0; /* This value is meaningless if _evaluate_flag triggered */
            }
            return *(double*) value; 
        } else {
            /* how the fuck did an Operator get evaluated wtf */
            fprintf(stderr, "unknown identifier\n");
            *_evaluate_flag = 1;
            return 0;
        }
    }
}

int evaluate_snode(SNode* snode, double* result, HashMap* environment) {
    int evaluate_flag = 0;
    *result = _evaluate_snode_recursive(snode, &evaluate_flag, environment);
    return evaluate_flag;
}

/* Don't know if I like modifying in place rather than propagating up doubles */
// void _evaluate_snode_recursive(SNode* snode) {
//     if (snode->type == LIST) {
//         if (sn_all_atomic(snode)) {
//             EvalAtom* eval_atom = (EvalAtom*) snode->list->atom;
//             switch (eval_atom->type) {
//                 case BINOP:
//                     break; 
//                 case UNOP:
//                     break;
//                 case NUMBER:
//                 /* See TODO: (ref 1). For now we assume that NUMBER are followed by nothing */
//                     assert(sn_list_len(snode) == 1);
//                     break;
//             }
//         }
//     } 
// }
