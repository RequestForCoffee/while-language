%{
#include "while.h"
#include "parser.h"
#include "lexer.h"

int yyerror(Statement **statement, yyscan_t scanner, const char *msg) {
    printf("%s", msg);
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


%type <stmt> block statements statement
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

/* A statement sequence is either a single statement, or a sequence followed by a statement */
statements: statement { $$ = $1; }
    | statements TOKEN_SEMICOLON statement { $$ = BuildStatementSequence($1, $3); }
    ;

statement: TOKEN_IDENTIFIER TOKEN_ASSIGN expression { $$ = BuildAssignment($1, $3); }
    | SKIP { $$ = BuildSkip(); }
    | IF boolean THEN TOKEN_LPAREN block TOKEN_RPAREN ELSE TOKEN_LPAREN block TOKEN_RPAREN { $$ = BuildConditional($5, $9, $2); }
    | WHILE boolean DO TOKEN_LPAREN block TOKEN_RPAREN { $$ = BuildLoop($5, $2); }
    ;

expression: TOKEN_NUMBER { $$ = BuildNumber($1); }
    | TOKEN_IDENTIFIER { $$ = BuildVariable($1); }
    | expression TOKEN_PLUS expression { $$ = BuildSum($1, $3); }
    | expression TOKEN_MINUS expression { $$ = BuildDifference($1, $3); }
    | expression TOKEN_MULTIPLY expression { $$ = BuildProduct($1, $3); }
    | TOKEN_LPAREN expression TOKEN_RPAREN { $$ = $2; }
    ;

boolean: TRUE { $$ = BuildBooleanLiteral(1); }
    | FALSE { $$ = BuildBooleanLiteral(0); }
    | expression TOKEN_EQ expression { $$ = BuildEquals($1, $3); }
    | expression TOKEN_LEQ expression { $$ = BuildLessThanOrEqualTo($1, $3); }
    | TOKEN_NOT boolean { $$ = BuildNot($2); }
    | boolean TOKEN_AND boolean { $$ = BuildAnd($1, $3); }
    | TOKEN_LPAREN boolean TOKEN_RPAREN { $$ = $2; }
    ;

%%
