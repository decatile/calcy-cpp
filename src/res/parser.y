%{

#include "parser.hpp"
#include <cstdio>
#include <iostream>
#include <sstream>
#include <cstring>

int yylex(void);
void yyerror(const char *s);

%}

%union {
	double numval;
	char* strval;
	Expr* exprval;
	char** strsval;
	Expr** exprsval;
}

%token <strval> IDENT
%token <numval> NUMBER
%token DEFINE
%token OUTPUT
%token NEWLINE
%left '+' '-'
%left '*' '/'

%type <strsval> def_args
%type <exprval> expression
%type <exprsval> expression_comma

%%

program:
	def_stmt out_stmt
	|
	def_stmt assign_stmts out_stmt
;

def_stmt:
	DEFINE def_args NEWLINE { set_def($2); lineno++; }
;

def_args:
	IDENT { $$ = defargs_create($1); }
	|
	def_args ',' IDENT { $$ = defargs_join($1, $3); }
;

assign_stmts:
	assign_stmt
	|
	assign_stmts assign_stmt
;

assign_stmt:
	IDENT '=' expression NEWLINE {
		if (set_assign($1, $3) != 0) {
			std::ostringstream s;
			s << "variable '" << $1 << "' already defined";
			yyerror(s.str().c_str());
		}
		lineno++;
	}
;

out_stmt:
	OUTPUT expression { set_out($2); }
	|
	OUTPUT expression NEWLINE { set_out($2); }

expression_comma:
	expression { $$ = exprargs_create($1); }
	|
	expression_comma ',' expression { $$ = exprargs_join($1, $3); }

expression:
	expression '+' expression { $$ = make_binary_expr($1, $3, BinaryExprType::ADD); }
  |
	expression '-' expression { $$ = make_binary_expr($1, $3, BinaryExprType::SUB); }
  |
	expression '*' expression { $$ = make_binary_expr($1, $3, BinaryExprType::MUL); }
  |
	expression '/' expression { $$ = make_binary_expr($1, $3, BinaryExprType::DIV); }
  |
	'(' expression ')' { $$ = $2; }
  |
	NUMBER { $$ = make_val_expr($1); }
	|
	IDENT {
		if (!variable_exists($1)) {
			std::ostringstream s;
			s << "variable '" << $1 << "' is not defined";
			yyerror(s.str().c_str());
		}
		$$ = make_ref_expr($1);
	}
	|
	IDENT '(' expression_comma ')' {
		auto exists = function_exists($1, $3);
		if (exists < 1) {
			std::ostringstream s;
			s << "function '" << $1 << "' requires " << -exists << " arguments";
			yyerror(s.str().c_str());
		}
		if (exists == 1) {
			std::ostringstream s;
			s << "function '" << $1 << "' not found";
			yyerror(s.str().c_str());
		}
		$$ = make_call_expr($1, $3);
	}
;

%%

void yyerror(const char *s)
{
  fprintf(stderr, "Compilation failed at %d line: %s\n", lineno, s);
	exit(1);
}
