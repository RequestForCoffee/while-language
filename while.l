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

%{
#include "while.h"
#include "parser.h"

%}

%option outfile="lexer.c" header-file="lexer.h"
%option warn nodefault

%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

ASSIGN ":="

EQ "="
LEQ "<="
NOT "!"
AND "&&"

PLUS "+"
MINUS "-"
MULTIPLY "*"

LPAREN "("
RPAREN ")"

SEMICOLON ";"

WS [ \r\n\t]+

DIGIT [0-9]
ID [a-z][a-z0-9]*

%%

skip                    { return(SKIP);  }
if                      { return(IF);    }
then                    { return(THEN);  }
else                    { return(ELSE);  }
while                   { return(WHILE); }
do                      { return(DO);    }

true                    { return(TRUE);  }
false                   { return(FALSE); }

{DIGIT}+                { sscanf(yytext, "%d", &yylval->ival); return(TOKEN_NUMBER);   }
{ID}                    { ReadString(yytext, &yylval->sval); return(TOKEN_IDENTIFIER); }

{ASSIGN}                { return(TOKEN_ASSIGN); }

{EQ}                    { return(TOKEN_EQ);  }
{LEQ}                   { return(TOKEN_LEQ); }
{NOT}                   { return(TOKEN_NOT); }
{AND}                   { return(TOKEN_AND); }

{WS}                    { }

{PLUS}                  { return(TOKEN_PLUS);      }
{MINUS}                 { return(TOKEN_MINUS);     }
{MULTIPLY}              { return(TOKEN_MULTIPLY);  }

{LPAREN}                { return(TOKEN_LPAREN);    }
{RPAREN}                { return(TOKEN_RPAREN);    }

{SEMICOLON}             { return(TOKEN_SEMICOLON); }

.                       { return(yytext[0]); }

%%
