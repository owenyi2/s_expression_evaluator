#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "snode.h"
#include "parser.h"
#include "evaluate.h"

void readline(char** buffer, int* buffer_size) {
/* based on https://brennan.io/2015/01/16/write-a-shell-in-c */
    const int resize_factor = 2;
    int c;
    int position = 0;
    *buffer_size = 4;
    
    if ((*buffer = realloc(*buffer, sizeof(char) * *buffer_size)) == NULL) {
        fprintf(stderr, "readline: realloc error\n");
        exit(1);
    }

    while (1) {
        c = getchar();

        if (c == EOF || c == '\n') {
            (*buffer)[position] = '\0';
            break;
        } else {
            (*buffer)[position] = c;
        }
        position++;

        if (position >= *buffer_size) {
            *buffer_size *= resize_factor;
            if ((*buffer = realloc(*buffer, *buffer_size)) == NULL) {
                fprintf(stderr, "readline: realloc error\n");
                exit(1);
            }
        }
    }
}

int parse_userline(char* input_buffer, int input_size, SNode** snode) {  
    Lexer* lx = lx_new(input_buffer, input_size);
    Parser* ps = ps_new(lx);
    ps_parse(ps, snode);

    int error = ps->error;
    free(lx);
    free(ps);
    return error;
}

int main() {
    char* input_buffer = calloc(4, 1);
    int input_size = -1;
    double result;

    SNode* snode = NULL;
    while (1) {
        /* Prompt */
        printf("> ");

        /* Get/Parse input */
        readline(&input_buffer, &input_size);
        if (strcmp("exit", input_buffer) == 0) {
            break;
        }

        if (parse_userline(input_buffer, input_size, &snode) != 0) {
            if (snode != NULL) {
                sn_free_recursive(snode);
            }

            fprintf(stderr, "Syntax Error\n");
            snode = NULL;
            continue;
        }
        
        /* Debug Info */
        printf("DEBUG: ");
        sn_debug(snode, debug_eval_atom);
        printf("\n");

        if (snode != NULL) {
            if (evaluate(snode, &result) == 0) {
                printf("%f\n", result);
            } else {
                fprintf(stderr, "Evaluation Error\n");
            }
        }

        /* Clean up */
        if (snode != NULL) {
            sn_free_recursive(snode);
	    snode = NULL;
        }
    }
    if (input_buffer != NULL) {
        free(input_buffer);
    }
}

