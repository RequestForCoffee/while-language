/*
MIT License

Copyright (c) 2018 Robertas Maleckas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "while.h"
#include "parser.h"
#include "lexer.h"

#include <stdio.h>

/*----------------------------------------------------------------------------*/
// Constants and configuration
/*----------------------------------------------------------------------------*/
const char* VersionString = "1.0";

/*----------------------------------------------------------------------------*/
// Type declarations
/*----------------------------------------------------------------------------*/
typedef struct options
{
    char* FileName;
    FILE* File;
    int PrintAST;
} CLIOptions;

/*----------------------------------------------------------------------------*/
// Function declarations
/*----------------------------------------------------------------------------*/
int yyparse(Statement **program, yyscan_t scanner);
Statement* ParseFile(FILE* file);
int ParseCLIArgs(CLIOptions *opts, int count, char* args[]);
void PrintHelp();

/*----------------------------------------------------------------------------*/
// Main
/*----------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
    CLIOptions opts;
    Statement *program = NULL;

    printf("while language interpreter for COMP11212 version %s\n", VersionString);
    printf("Copyright (c) 2018 Robertas Maleckas\n\n");

    if (0 != ParseCLIArgs(&opts, argc, argv))
        exit(0);
    
    printf("Parsing '%s'...\n", opts.FileName);
    program = ParseFile(opts.File);
    fclose(opts.File);

    if (opts.PrintAST)
    {
        printf("Printing the AST for '%s':\n\n", opts.FileName);
        PrintAST(stdout, program);
    }
    else
    {
        printf("Executing '%s'...\n", opts.FileName);
        EvaluateStatement(program);
        printf("Execution complete.\n");
        printf("Resulting state:\n\n");
        PrintState(stdout);
        FreeMemory();
    }

    DeleteStatement(program);
    return 0;
}

/*----------------------------------------------------------------------------*/
// Function definitions
/*----------------------------------------------------------------------------*/
Statement* ParseFile(FILE* file)
{
    Statement *statement;
    yyscan_t scanner;
    YY_BUFFER_STATE state;

    if (yylex_init(&scanner)) {
        // could not initialize
        printf("Error encountered while initializing the scanner.\n");
        return NULL;
    }

    state = yy_create_buffer(file, YY_BUF_SIZE, scanner);
    yy_switch_to_buffer(state, scanner);

    if (yyparse(&statement, scanner)) {
        // error parsing
        printf("Error encountered while parsing.\n");
        return NULL;
    }

    yy_delete_buffer(state, scanner);

    yylex_destroy(scanner);

    return statement;
}

int ParseCLIArgs(CLIOptions* opts, int count, char* args[])
{
    opts->File = NULL;
    opts->PrintAST = 0;

    if (1 == count)
    {
        PrintHelp();
        return 100;
    }

    int i;
    for (i = 1; i < count; i++)
    {
        if (0 == strcmp(args[i], "-h") || 0 == strcmp(args[i], "--help"))
        {
            PrintHelp();
            return 100;
        }
        else if (0 == strcmp(args[i], "-p") || 0 == strcmp(args[i], "--print-ast"))
        {
            if (0 != opts->PrintAST)
            {
                printf("Error: the AST print option may only be specified once.\n");
                return 1;
            }
            opts->PrintAST = 1;
        }
        else
        {
            // Naively assume anything other than predefined args is a file name
            if (NULL != opts->File)
            {
                printf("Error: only one source file may be specified.\n");
            }
            opts->FileName = args[i];
            opts->File = fopen(args[i], "r");
            if (NULL == opts->File)
            {
                printf("Error: could not open file '%s' for reading.\n", args[i]);
                return -1;
            }
        }
    }

    return 0;
}

void PrintHelp()
{
    printf("Usage: while [options] <file>\n\n");
    printf("Options:");
    printf("    -h --help: print this information\n\n");
    printf("    -p --print-ast: do not execute; only print out the parsed AST\n\n");
}
