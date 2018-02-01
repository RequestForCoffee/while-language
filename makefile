# Makefile

CC	= g++
CFLAGS	= -g -ansi
FILES	= lexer.c parser.c while.c main.c

while:	$(FILES)
		$(CC) $(CFLAGS) $(FILES) -o whilec

lexer.c:	while.l 
		win_flex while.l

parser.c:	while.y lexer.c
		win_bison -vd while.y

# Disable default .c targets
%.c: %.y

%.c: %.l
