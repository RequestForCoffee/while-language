while.tab.c while.tab.h: while.y
	bison -vd while.y

lex.yy.c: while.l while.tab.h
	flex while.l

while: lex.yy.c while.tab.c while.tab.h
	g++ while.tab.c lex.yy.c -o whilec

