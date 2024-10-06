#pragma once

#include <cstddef>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
struct Expr;

enum class ExprType { REF, VAL, BIN, CALL };

enum class BinaryExprType { ADD, SUB, MUL, DIV };

struct BinaryExpr {
  Expr *a, *b;
  BinaryExprType type;
};

struct CallExpr {
  char *name;
  Expr **args;
};

struct Expr {
  union {
    double val;
    char *ref;
    BinaryExpr *bin;
    CallExpr *call;
  };
  ExprType kind;
};

struct Assign {
  Expr *expr;
  int at;
};

struct CalcyFunction {
  double (*ptr)(std::vector<double> &);
  size_t argcount;
};

Expr *make_call_expr(char *name, Expr **args);
Expr *make_val_expr(double val);
Expr *make_ref_expr(char *varname);
Expr *make_binary_expr(Expr *a, Expr *b, BinaryExprType type);
char **defargs_create(char *name);
char **defargs_join(char **a, char *b);
Expr **exprargs_create(Expr *name);
Expr **exprargs_join(Expr **a, Expr *b);
void set_def(char **varnames);
void set_out(Expr *expr);
int set_assign(char *varname, Expr *expr);
bool variable_exists(char *varname);
int function_exists(char *name, Expr **args);

extern int lineno;
extern std::vector<std::string> def_varnames;
extern std::vector<std::pair<std::string, Assign *>> assign_pairs;
extern Expr *out_expr;
extern std::unordered_map<std::string, CalcyFunction> functions;
