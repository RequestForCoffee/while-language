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


%type <stmt> statements statement 
%type <AExp> expression
%type <BExp> boolean

%token SKIP IF THEN ELSE WHILE DO
%token TOKEN_ASSIGN TOKEN_SEMICOLON
%token <ival> TOKEN_NUMBER
%token <sval> TOKEN_IDENTIFIER

%left TOKEN_EQ TOKEN_LEQ TOKEN_NOT TOKEN_AND
%left TOKEN_PLUS TOKEN_MINUS
%left TOKEN_MULTIPLY

%%

input: statements { *program = $1; }

statements: /* empty */ { $$ = NULL; }
    | statements statement TOKEN_SEMICOLON { $$ = BuildStatementSequence($1, $2); }
    ;

statement: TOKEN_IDENTIFIER TOKEN_ASSIGN expression { $$ = BuildAssignment($1, $3); }
    | SKIP { $$ = BuildSkip(); }
    | IF boolean THEN '{' statements '}' ELSE '{' statements '}' { $$ = BuildConditional($5, $9, $2); }
    | WHILE boolean DO '{' statements '}' { $$ = BuildLoop($5, $2); }
    ;

expression: TOKEN_NUMBER { printf("%s", "no"); $$ = BuildNumber($1); }
    | TOKEN_IDENTIFIER { printf("%s", "var"); $$ = BuildVariable($1); }
    | expression TOKEN_PLUS expression { $$ = BuildSum($1, $3); }
    | expression TOKEN_MINUS expression { $$ = BuildDifference($1, $3); }
    | expression TOKEN_MULTIPLY expression { $$ = BuildProduct($1, $3); }
    | '(' expression ')' { $$ = $2; }
    ;

boolean: 'true' { $$ = BuildBooleanLiteral(1); }
    | 'false' { $$ = BuildBooleanLiteral(0); }
    | expression TOKEN_EQ expression { $$ = BuildEquals($1, $3); }
    | expression TOKEN_LEQ expression { $$ = BuildLessThanOrEqualTo($1, $3); }
    | TOKEN_NOT boolean { $$ = BuildNot($2); }
    | boolean TOKEN_AND boolean { $$ = BuildAnd($1, $3); }
    | '(' boolean ')' { $$ = $2; }
    ;

%%
