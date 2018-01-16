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

main (int argc, char *argv[])
{
    extern FILE *yyin;
    ++argv; −−argc;
    yyin = fopen(argv[0], ”r”);
    yydebug = 1;
    errors = 0;
    yyparse ();
}
yyerror (char *s) /* Called by yyparse on error */
{
    printf (”%s\n”, s);
}
