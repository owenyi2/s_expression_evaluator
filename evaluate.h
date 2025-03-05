#ifndef EVALUATE_H
#define EVALUATE_H

#include <math.h>

#include "atom.h"
#include "snode.h"

int evaluate(SNode* snode, double* result) {
    if (snode->type == LIST) {
        EvalAtom* first_atom = (EvalAtom*) snode->list->atom;
        switch (first_atom->type) {
            case EVAL_ATOM_NUMBER:
                if (sn_list_len(snode) != 1) {
                    fprintf(stderr, "evaluate: 0 arguments expected after number\n");
                    return 1;
                } else {
                    fprintf(stderr, "NUMBER\n");
                    *result = first_atom->number;
                }
                break;
            case EVAL_ATOM_IDENTIFIER:
                if (sn_list_len(snode) != 1) {
                    fprintf(stderr, "evaluate: 0 arguments expected after identifier\n");
                    return 1;
                } else {
                    fprintf(stderr, "evaluate: NOT IMPLEMENTED YET\n");
                }
                break;
            case EVAL_ATOM_FUNC:
                if (sn_next_count(snode->list) != first_atom->num_args) {
                    fprintf(stderr, "evaluate: %d arguments expected after identifier\n", first_atom->num_args);
                    return 1;

                } else {
                    switch (first_atom->func) {
                        case FUNC_PLUS: {
                            double a, b;
                            int err_a = evaluate(snode->list->next, &a);
                            int err_b = evaluate(snode->list->next->next, &b);
                            if (err_a | err_b) {
                                return 1;
                            } else {
                                *result = a + b;
                            }
                            break;
                        }
                        case FUNC_MINUS: {
                            double a, b;
                            int err_a = evaluate(snode->list->next, &a);
                            int err_b = evaluate(snode->list->next->next, &b);
                            if (err_a | err_b) {
                                return 1;
                            } else {
                                *result = a - b;
                            }
                            break;
                        }
                        case FUNC_TIMES: {
                            double a, b;
                            int err_a = evaluate(snode->list->next, &a);
                            int err_b = evaluate(snode->list->next->next, &b);
                            if (err_a | err_b) {
                                return 1;
                            } else {
                                *result = a * b;
                            }
                            break;
                        }
                        case FUNC_DIVIDE: {
                            double a, b;
                            int err_a = evaluate(snode->list->next, &a);
                            int err_b = evaluate(snode->list->next->next, &b);
                            if (err_a | err_b) {
                                return 1;
                            } else {
                                *result = a / b;
                            }
                            break;
                        }
                        case FUNC_POW: {
                            double a, b;
                            int err_a = evaluate(snode->list->next, &a);
                            int err_b = evaluate(snode->list->next->next, &b);
                            if (err_a | err_b) {
                                return 1;
                            } else {
                                *result = pow(a, b);
                            }
                            break;
                        }
                        case FUNC_SETQ: {
                            if ((snode->list->next->type != ATOM) || 
                            (((EvalAtom*)(snode->list->next->atom))->type != EVAL_ATOM_IDENTIFIER)) {
                                fprintf(stderr, "evaluate: first argument of setq must be identifier\n");
                                return 1;
                            }
                            fprintf(stderr, "Not Implemented Yet\n");
                            return 1;
                            break;
                        }
                        case FUNC_SIN: {
                            double a;
                            int err_a = evaluate(snode->list->next, &a);
                            if (err_a) {
                                return 1;
                            } else {
                                *result = sin(a);
                            }
                            break;
                        }
                        case FUNC_COS: { 
                            double a;
                            int err_a = evaluate(snode->list->next, &a);
                            if (err_a) {
                                return 1;
                            } else {
                                *result = cos(a);
                            }
                            break;
                        }
                        case FUNC_TAN: {
                            double a;
                            int err_a = evaluate(snode->list->next, &a);
                            if (err_a) {
                                return 1;
                            } else {
                                *result = tan(a);
                            }
                            break;
                        }
                        case FUNC_EXP: {
                            double a;
                            int err_a = evaluate(snode->list->next, &a);
                            if (err_a) {
                                return 1;
                            } else {
                                *result = exp(a);
                            }
                            break;
                        }
                        case FUNC_LOG: {
                            double a;
                            int err_a = evaluate(snode->list->next, &a);
                            if (err_a) {
                                return 1;
                            } else {
                                *result = log(a);
                            }
                            break;
                        }
                    }
                }
                break;
        }
    } else {
        EvalAtom* atom = (EvalAtom*)snode->atom;
        switch (atom->type) {
            case EVAL_ATOM_NUMBER:
                *result = atom->number;
                break;
            case EVAL_ATOM_IDENTIFIER:
                fprintf(stderr, "evaluate: NOT IMPLEMENTED YET\n");
                return 1;
                break;
            case EVAL_ATOM_FUNC:
                fprintf(stderr, "evaluate: function in unexpected place\n");
                return 1;
                break;

        }
    }
    return 0;
}



#endif
