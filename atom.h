#include <ctype.h>

/* we honestly should do the ifndef thing to include snode.h. sigh */

enum EvalAtomType {
    NUMBER, /* aka double */
    BINOP, /* e.g. +, -, *, / */
    UNOP, /* e.g. sin, cos, tan, exp, log */
};

enum Binop {
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
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
            *eval_atom = (EvalAtom) { .type = NUMBER, number = number };
        } else {
            fprintf(stderr, "ea_from_string: Unrecognised input\n");
            eval_atom = NULL;
        }
    }
    return eval_atom;
}

void _type_check_snode_recursive(SNode* snode, int first_element, int* _type_check_flag) {
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
        /* TODO: How to check that an operator is the first element of its parent list */
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
