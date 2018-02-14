# Makefile

CC	= g++
CFLAGS	= -g -ansi
FILES	= lexer.c parser.c while.c main.c
BISONFLAGS	= -vd --debug
# Change to flex & bison if building on UNIX
FLEX = win_flex
BISON = win_bison

while:	$(FILES)
		$(CC) $(CFLAGS) $(FILES) -o while

lexer.c:	while.l 
		$(FLEX) while.l

parser.c:	while.y lexer.c
		$(BISON) $(BISONFLAGS) --debug while.y

clean:
		del lexer.h lexer.c parser.c parser.h while.exe

# Disable default .c targets
%.c: %.y

%.c: %.l
