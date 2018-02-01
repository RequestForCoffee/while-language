%{
#include "while.h"
#include "parser.h"
#include "lexer.h"

int yyerror(Statement **expression, ArithmeticExpression **AExp, BooleanExpression **BExp, yyscan_t scanner, const char *msg) {
    printf("%s", msg);
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
%parse-param { Statement **statement }
%parse-param { ArithmeticExpression **AExp }
%parse-param { BooleanExpression **BExp }
%parse-param { yyscan_t scanner }

%union {
    Statement *stmt;
    ArithmeticExpression *AExp;
    BooleanExpression *BExp;
    char* sval;
    int ival;
}

%start statements

%type <stmt> statements statement 
%type <AExp> expression
%type <BExp> boolean

%token SKIP ASSIGN IF THEN ELSE WHILE DO
%token <ival> NUMBER
%token <sval> IDENTIFIER

%left EQ LEQ NOT AND
%left '+' '-' '*'

%%

statements: /* empty */ { $$ = NULL; }
    | statements statement ';' { $$ = BuildStatementSequence($1, $2); }

statement: IDENTIFIER ASSIGN expression { $$ = BuildAssignment($1, $3); }
    | SKIP { $$ = BuildSkip(); }
    | IF boolean THEN '{' statements '}' ELSE '{' statements '}' { $$ = BuildConditional($5, $9, $2); }
    | WHILE boolean DO '{' statements '}' { $$ = BuildLoop($5, $2); }

expression: NUMBER { $$ = BuildNumber($1); }
    | IDENTIFIER { $$ = BuildVariable($1); }
    | expression '+' expression { $$ = BuildSum($1, $3); }
    | expression '-' expression { $$ = BuildDifference($1, $3); }
    | expression '*' expression { $$ = BuildProduct($1, $3); }
    | '(' expression ')' { $$ = $2; }

boolean: 'true' { $$ = BuildBooleanLiteral(1); }
    | 'false' { $$ = BuildBooleanLiteral(0); }
    | expression EQ expression { $$ = BuildEquals($1, $3); }
    | expression LEQ expression { $$ = BuildLessThanOrEqualTo($1, $3); }
    | NOT boolean { $$ = BuildNot($2); }
    | boolean AND boolean { $$ = BuildAnd($1, $3); }
    | '(' boolean ')' { $$ = $2; }

%%
