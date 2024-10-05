#pragma once

#include <string>
#include <utility>
#include <vector>
struct Expr;

enum class ExprType { REF, VAL, BIN };

enum class BinaryExprType { ADD, SUB, MUL, DIV };

struct BinaryExpr {
  Expr *a, *b;
  BinaryExprType type;
};

struct Expr {
  union {
    double val;
    char *ref;
    BinaryExpr *bin;
  };
  ExprType kind;
};

struct Assign {
  Expr *expr;
  int at;
};

Expr *make_val_expr(double val);
Expr *make_ref_expr(char *varname);
Expr *make_binary_expr(Expr *a, Expr *b, BinaryExprType type);
char **defargs_create(char *name);
char **defargs_join(char **a, char *b);
void set_def(char **varnames);
void set_out(Expr *expr);
int set_assign(char *varname, Expr *expr);
bool variable_exists(char *varname);

extern int lineno;
extern std::vector<std::string> def_varnames;
extern std::vector<std::pair<std::string, Assign *>> assign_pairs;
extern Expr *out_expr;
