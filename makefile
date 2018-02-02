# Makefile

CC	= g++
CFLAGS	= -g -ansi
FILES	= lexer.c parser.c while.c main.c
BISONFLAGS	= -vd --debug

while:	$(FILES)
		$(CC) $(CFLAGS) $(FILES) -o whilec

lexer.c:	while.l 
		win_flex while.l

parser.c:	while.y lexer.c
		win_bison $(BISONFLAGS) --debug while.y

clean:
		del lexer.h lexer.c parser.c parser.h whilec.exe

# Disable default .c targets
%.c: %.y

%.c: %.l
