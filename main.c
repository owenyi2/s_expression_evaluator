#include <stdlib.h>
#include <stdio.h>

#include "snode.h"

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

int main() {
    char* input_buffer = NULL;
    int* input_size;
    int parse_error_flag = 0;
    FILE* input_as_file = NULL;
    SNode* snode;
    while (1) {
        printf("> ");
        readline(&input_buffer, input_size);
        input_as_file = fmemopen(input_buffer, *input_size, "r");
        snode = sn_parse(input_as_file, &parse_error_flag);
        if (parse_error_flag != 0) {
            parse_error_flag = 0;
            continue;
        } 
        sn_debug(snode, debug_char_atom);
        printf("\n");
        sn_free_recursive(snode);
        fclose(input_as_file);
    }
    if (input_buffer != NULL) {
        free(input_buffer);
    }
}
