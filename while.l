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
