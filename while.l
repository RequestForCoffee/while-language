%{
#include "while.h"
#include "parser.h"

%}

%option outfile="lexer.c" header-file="lexer.h"

%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

DIGIT [0-9]
ID [a-z][a-z0-9]*

%%

skip                    { return(SKIP);       }
":="                    { return(ASSIGN);     }
if                      { return(IF);         }
then                    { return(THEN);       }
else                    { return(ELSE);       }
while                   { return(WHILE);      }
do                      { return(DO);         }
{DIGIT}+                { sscanf(yytext, "%d", &yylval->AExp->NumberLiteral); return(NUMBER); }
{ID}                    { sscanf(yytext, "%s", &yylval->stmt->Variable); return(IDENTIFIER); }
"="                     { return(EQ);         }
"<="                    { return(LEQ);        }
"!"                     { return(NOT);        }
"&"                     { return(AND);        }
[ \r\n\t]+
.                       { return(yytext[0]);  }

%%

int yyerror(const char *msg) {
    fprintf(stderr,"Error:%s\n",msg); return 0;
}
