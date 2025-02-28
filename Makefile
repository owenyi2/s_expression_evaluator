CC = gcc 
CFLAGS = -std=c99 -Wall -Werror -g -fsanitize=address -fsanitize=undefined -fsanitize=leak

main: main.c lexer.h parser.h atom.h snode.h  
	$(CC) $(CFLAGS) main.c -o main
