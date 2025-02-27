#include <stdlib.h>
#include <stdio.h>

#include "snode.h"
#include "parser.h"

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
    Token token;
    ps_parse(ps, snode);
    return ps->error;
}

int main() {
    char* input_buffer = NULL;
    int input_size = -1;

    SNode* snode;
    while (1) {
        /* Prompt */
        printf("> ");

        /* Get/Parse input */
        readline(&input_buffer, &input_size);
        if (parse_userline(input_buffer, input_size, &snode) != 0) {
            sn_free_recursive(snode);
            fprintf(stderr, "Syntax Error\n");
            continue;
        }
        
        /* Debug Info */
        printf("DEBUG: ");
        sn_debug(snode, debug_eval_atom);
        printf("\n");

        /* Clean up */
        if (snode != NULL) {
            sn_free_recursive(snode);
        }
    }
    if (input_buffer != NULL) {
        free(input_buffer);
    }
}

