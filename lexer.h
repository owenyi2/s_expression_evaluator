#ifndef LEXER_H
#define LEXER_H

#include <string.h>

/* TODO: sed all the enums to add TOKEN or TOKEN_TYPE or LEXER prefix to avoid clashes with */

enum TokenType {
    /* Paren */
    LPAREN = 1,
    RPAREN = 2,
    /* Binop */
    PLUS = 11,
    MINUS = 12,
    TIMES = 13,
    DIVIDE = 14,
    POW = 15,
    SETQ = 16,
    /* Unop */
    SIN = 21,
    COS = 22,
    TAN = 23,
    EXP= 24,
    LOG = 25,
    /* Other */
    NUMBER = 31,
    IDENTIFIER = 32,
    END = -1,
};

typedef struct Token {
    enum TokenType type;
    char* identifier;
    double number;
} Token;

void debug_token(Token token) {
    switch (token.type) {
        case LPAREN:
            fprintf(stderr, "debug_token: LPAREN\n");
            break;
        case RPAREN:
            fprintf(stderr, "debug_token: RPAREN\n");
            break;
        case PLUS:
            fprintf(stderr, "debug_token: PLUS\n");
            break;
        case MINUS:
            fprintf(stderr, "debug_token: MINUS\n");
            break;
        case TIMES:
            fprintf(stderr, "debug_token: TIMES\n");
            break;
        case DIVIDE:
            fprintf(stderr, "debug_token: DIVIDE\n");
            break;
        case POW:
            fprintf(stderr, "debug_token: POW\n");
            break;
        case SETQ:
            fprintf(stderr, "debug_token: SETQ\n");
            break;
        case SIN:
            fprintf(stderr, "debug_token: SIN\n");
            break;
        case COS:
            fprintf(stderr, "debug_token: COS\n");
            break;
        case TAN:
            fprintf(stderr, "debug_token: TAN\n");
            break;
        case EXP:
            fprintf(stderr, "debug_token: EXP\n");
            break;
        case LOG:
            fprintf(stderr, "debug_token: LOG\n");
            break;
        case NUMBER:
            fprintf(stderr, "debug_token: NUMBER = %f\n", token.number);
            break;
        case IDENTIFIER:
            fprintf(stderr, "debug_token: IDENTIFIER = %s\n", token.identifier);
            break;
        case -1:
            fprintf(stderr, "debug_token: END\n");
            break;
    }
}

typedef struct Lexer {
    char* input;
    size_t input_size;
    size_t curr_pos;
    char curr_char;
} Lexer;

void lx_next_char(Lexer* lx) {
    lx->curr_pos += 1;
    if (lx->curr_pos >= lx->input_size) {
        lx->curr_char = '\0';
    }
    lx->curr_char = lx->input[lx->curr_pos];
}

char lx_peek(Lexer* lx) {
    lx_next_char(lx);
    char peek = lx->curr_char;
    lx->curr_pos -= 1;
    return peek;
}

Lexer* lx_new(char* input, size_t input_size) {
    Lexer* lx = malloc(sizeof(Lexer));
    *lx = (Lexer) {.input = input, .input_size = input_size, .curr_pos = -1, .curr_char = '\0' };
    lx_next_char(lx);
    return lx;
}

void lx_skip_whitespace(Lexer* lx) {
    int is_white_space = 1;
    while (is_white_space) {
        switch(lx->curr_char) {
            case ' ':
            case '\n':
            case '\r':
            case '\t':
                lx_next_char(lx);
                break;
            default:
                is_white_space = 0;
        }
    }
}

int is_digit(char c) {
    /* [0-9] */
    return (48 <= c) & (c <= 57);
}

int is_alpha(char c) {
    /* [a-zA-Z] */
    return ((65 <= c) & (c <= 90)) | ((96 <= c) & (c <= 122));
}

int is_alphnum(char c) {
    /* [a-zA-Z0-9] */
    return is_alpha(c) | is_digit(c); 
}

int is_keyword(char* input, enum TokenType* keyword) {
    if (!(strcmp(input, "setq"))) {
        *keyword = SETQ;
    } else if (!(strcmp(input, "sin"))) {
        *keyword = SIN;
    } else if (!(strcmp(input, "cos"))) {
        *keyword = COS;
    } else if (!(strcmp(input, "tan"))) {
        *keyword = TAN;
    } else if (!(strcmp(input, "exp"))) {
        *keyword = EXP;
    } else if (!(strcmp(input, "log"))) {
        *keyword = LOG;
    } else {
        return 0;
    }
    return 1;
}

int lx_next_token(Lexer* lx, Token* token) {
    lx_skip_whitespace(lx);
 
    switch (lx->curr_char) {
        case '(':
            token->type = LPAREN;
            break;
        case ')':
            token->type = RPAREN;
            break;
        case '+':
            token->type = PLUS;
            break;
        case '-':
            token->type = MINUS;
            break;
        case '*':
            token->type = TIMES;
            break;
        case '/':
            token->type = DIVIDE;
            break;
        case '^':
            token->type = POW;
            break;
        case '\0':
            token->type = END;
            break;
        default: {
            if (is_digit(lx->curr_char)) {
                long start = lx->curr_pos;
                while (is_digit(lx_peek(lx))) {
                    lx_next_char(lx);       
                }
                if (lx_peek(lx) == '.') {
                    lx_next_char(lx);       
                    while (is_digit(lx_peek(lx))) {
                        lx_next_char(lx);       
                    }
                }
                size_t len = lx->curr_pos - start + 1;
                char temp[len+1];
                strncpy(temp, lx->input + start, len);
                token->type = NUMBER;
                token->number = atof(temp);
            } else if (is_alpha(lx->curr_char)) {
                long start = lx->curr_pos;
                while (is_alphnum(lx_peek(lx))) {
                    lx_next_char(lx);
                }
                size_t len = lx->curr_pos - start + 1;
                char temp[len+1];
                strncpy(temp, lx->input + start, len);
                if (is_keyword(temp, &token->type)) {

                } else {
                    token->type = IDENTIFIER;
                    token->identifier = malloc(sizeof(temp));
                    strncpy(token->identifier, temp, sizeof(temp));
                }
            } else {
                fprintf(stderr, "Lexer Error: Unknown symbol, %c\n", lx->curr_char);     
                return 1;
            }
        }
    }
    fprintf(stderr, "lexer: ");
    debug_token(*token);
    lx_next_char(lx);
    return 0;
}

#endif
