%{
#include <string>
#include "while.tab.h"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
extern "C" int yywrap() { }
%}

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
{DIGIT}+                { return(NUMBER);     }
{ID}                    { return(IDENTIFIER); }
"="                     { return(EQ);         }
"<="                    { return(LEQ);        }
"!"                     { return(NOT);        }
"&"                     { return(AND);        }
[ \t\n]+
.                       { return(yytext[0]);  }

%%
