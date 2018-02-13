%{
#include "while.h"
#include "parser.h"
#include "lexer.h"

int yyerror(Statement **statement, yyscan_t scanner, const char *msg) {
    printf("Error: %s\n", msg);
	return 0;
}
%}

%code requires {

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

}

%output  "parser.c"
%defines "parser.h"

%define api.pure

%error-verbose

%lex-param   { yyscan_t scanner }
%parse-param { Statement **program }
%parse-param { yyscan_t scanner }

%union {
    Statement *stmt;
    ArithmeticExpression *AExp;
    BooleanExpression *BExp;
    char* sval;
    int ival;
}


%type <stmt> block enclosedblock statements statement
%type <AExp> expression
%type <BExp> boolean

%token SKIP 
%token IF THEN ELSE WHILE DO
%token TRUE FALSE
%token TOKEN_ASSIGN TOKEN_SEMICOLON
%token TOKEN_LPAREN TOKEN_RPAREN
%token <ival> TOKEN_NUMBER
%token <sval> TOKEN_IDENTIFIER

%left TOKEN_EQ TOKEN_LEQ TOKEN_NOT TOKEN_AND
%left TOKEN_PLUS TOKEN_MINUS
%left TOKEN_MULTIPLY

%%

/* A program is a "block" of executable logic */
program: block { *program = $1; }

/* A block is simply a (possibly empty) statement sequence which may or may not end with a semicolon */
block: /* empty */ { $$ = NULL; }
    | statements { $$ = $1; }
    | statements TOKEN_SEMICOLON { $$ = $1; }

enclosedblock: statement[S] { $$ = $S; }
    | TOKEN_LPAREN block[B] TOKEN_RPAREN { $$ = $B; }

/* A statement sequence is either a single statement, or a sequence followed by a statement */
statements: statement { $$ = $1; }
    | statements TOKEN_SEMICOLON statement { $$ = BuildStatementSequence($1, $3); }
    ;

statement: TOKEN_IDENTIFIER[V] TOKEN_ASSIGN expression[E] { $$ = BuildAssignment($V, $E); } /* [V]ariable := [E]xpression */
    | SKIP { $$ = BuildSkip(); } /* do nothing */
    | IF boolean[C] THEN enclosedblock[T] ELSE enclosedblock[F] { $$ = BuildConditional($T, $F, $C); } /* if [C]ondition then (block if [T]rue) else (block if [F]alse) */
    | WHILE boolean[C] DO enclosedblock[B] { $$ = BuildLoop($B, $C); } /* while [C]ondition execute [B]ody */
    ;

expression: TOKEN_NUMBER[N] { $$ = BuildNumber($N); } /* [N]umber literal */ 
    | TOKEN_IDENTIFIER[V] { $$ = BuildVariable($V); } /* [V]ariable */
    | expression[L] TOKEN_PLUS expression[R] { $$ = BuildSum($L, $R); } /* [L]eft + [R]ight */
    | expression[L] TOKEN_MINUS expression[R] { $$ = BuildDifference($L, $R); } /* [L]eft - [R]ight */
    | expression[L] TOKEN_MULTIPLY expression[R] { $$ = BuildProduct($L, $R); } /* [L]eft * [R]ight */
    | TOKEN_LPAREN expression[E] TOKEN_RPAREN { $$ = $E; } /* ([E]xpression) */
    ;

boolean: TRUE { $$ = BuildBooleanLiteral(1); } /* literal true */
    | FALSE { $$ = BuildBooleanLiteral(0); } /* literal false */
    | expression[L] TOKEN_EQ expression[R] { $$ = BuildEquals($L, $R); } /* [L]eft eq [R]ight */
    | expression[L] TOKEN_LEQ expression[R] { $$ = BuildLessThanOrEqualTo($L, $R); } /* [L]eft leq [R]ight */
    | TOKEN_NOT boolean[B] { $$ = BuildNot($B); }  /* not [B]oolean */
    | boolean TOKEN_AND boolean { $$ = BuildAnd($1, $3); } /* [B]oolean and [B]oolean */
    | TOKEN_LPAREN boolean TOKEN_RPAREN { $$ = $2; } /* ([B]oolean) */
    ;

%%
