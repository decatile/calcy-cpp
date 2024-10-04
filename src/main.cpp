#include <cstdio>

extern FILE *yyin;
extern int yyparse();

int main() {
  yyin = fopen("file.calc", "r");
  yyparse();
}
