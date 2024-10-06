%{

#include "parser.hpp"
#include "parser.gen.hpp"
#include <string>

%}

%%

"def"                  return DEFINE;
"out"                  return OUTPUT;
[0-9]+(\.[0-9]+)? { yylval.numval = std::stod(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9]*  { yylval.strval = strdup(yytext); return IDENT; }
"+"                    return '+';
"-"                    return '-';
"*"                    return '*';
"/"                    return '/';
"("                    return '(';
")"                    return ')';
","                    return ',';
"="                    return '=';
"\n"+                  return NEWLINE;
[\t ]+

%%

int yywrap() {
    return 1;
}
