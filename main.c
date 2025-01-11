#include <stdlib.h>
#include <stdio.h>

#include "snode.h"
#include "atom.h"

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

SNode* parse_userline(char* input_buffer, int input_size, int* parse_error_flag) {
    FILE* input_as_file = fmemopen(input_buffer, input_size, "r");

    SNode* snode = sn_parse(input_as_file, parse_error_flag); 
    
    if (*parse_error_flag != 0) {
        fclose(input_as_file);
        return NULL;
    } 
    *parse_error_flag = type_check_snode(snode);
    if (*parse_error_flag != 0) {
        fclose(input_as_file);
        return NULL;
    } 
    fclose(input_as_file);
    return snode;
        
}

int main() {
    char* input_buffer = NULL;
    int* input_size;
    int parse_error_flag = 0;
    SNode* snode;
    while (1) {
        printf("> ");
        readline(&input_buffer, input_size);
        snode = parse_userline(input_buffer, *input_size, &parse_error_flag);
        if (parse_error_flag != 0) {
            parse_error_flag = 0;
            continue;
        }
        if (snode != NULL) {
            sn_free_recursive(snode);
        }
    }
    if (input_buffer != NULL) {
        free(input_buffer);
    }
}
