%{
#include <cstdio> // unneeded?
#include <iostream>

using namespace std; // unneeded?

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

void yyerror(char *s);
%}

%start statements

%token SKIP ASSIGN IF THEN ELSE WHILE DO
%token NUMBER
%token IDENTIFIER
%left EQ LEQ NOT AND
%left '+' '-' '*'

%%

statements: /* empty */
    | statements statement ';'

statement: IDENTIFIER ASSIGN expression
    | SKIP
    | IF boolean THEN '{' statements '}' ELSE '{' statements '}'
    | WHILE boolean DO '{' statements '}'

expression: NUMBER
    | IDENTIFIER
    | expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | '(' expression ')'

boolean: 'true'
    | 'false'
    | expression EQ expression
    | expression LEQ expression
    | NOT boolean
    | boolean AND boolean
    | '(' boolean ')'

%%

int main(int argc, char *argv[]) {

    FILE *myfile = fopen(argv[0], "r");
    // make sure it's valid:
    if (!myfile) {
        return -1;
    }

    yyin = myfile;

    yylex();
}

void yyerror(char *s)
{
    printf("%s\n", s);
}
