#include "while.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>

int yyparse(Statement **program, yyscan_t scanner);


Statement* ParseText(const char* text)
{
    Statement *statement;
    yyscan_t scanner;
    YY_BUFFER_STATE state;
 
    if (yylex_init(&scanner)) {
        // could not initialize
        return NULL;
    }
 
    state = yy_scan_string(text, scanner);
 
    if (yyparse(&statement, scanner)) {
        // error parsing
        return NULL;
    }
 
    yy_delete_buffer(state, scanner);
 
    yylex_destroy(scanner);
 
    return statement;
}


int main()
{
    Statement *program = NULL;
    char test[] = "variable := 7;";

    program = ParseText(test);



    DeleteStatement(program);
    return 0;
}