#include "parser.hpp"
#include <algorithm>

int lineno = 1;
std::vector<std::string> def_varnames;
std::vector<std::pair<std::string, Assign *>> assign_pairs;
Expr *out_expr;

Expr *make_val_expr(double val) {
  Expr *e = new Expr;
  e->kind = ExprType::VAL;
  e->val = val;
  return e;
}

Expr *make_ref_expr(char *varname) {
  Expr *e = new Expr;
  e->kind = ExprType::REF;
  e->ref = varname;
  return e;
}

Expr *make_binary_expr(Expr *a, Expr *b, BinaryExprType type) {
  Expr *e = new Expr;
  e->kind = ExprType::BIN;
  e->bin = new BinaryExpr{.a = a, .b = b, .type = type};
  return e;
}

char **defargs_create(char *name) {
  char **r = new char *[2];
  r[0] = name;
  r[1] = NULL;
  return r;
}

char **defargs_join(char **a, char *b) {
  size_t len = 0;
  char **a1 = a;
  while (*a1) {
    len++;
    a1++;
  }
  a = (char **)std::realloc(a, sizeof(char *) * (len + 2));
  a[len] = b;
  a[len + 1] = NULL;
  return a;
}

void set_def(char **varnames) {
  while (*varnames) {
    def_varnames.push_back(*varnames);
    varnames++;
  }
}

void set_out(Expr *expr) { out_expr = expr; }

int set_assign(char *varname, Expr *expr) {
  if (std::find(def_varnames.begin(), def_varnames.end(), varname) !=
      def_varnames.end()) {
    return 1;
  }
  auto found = std::find_if(assign_pairs.begin(), assign_pairs.end(),
                            [=](auto p) { return p.first == varname; });
  if (found != assign_pairs.end()) {
    return found->second->at;
  }
  assign_pairs.push_back(
      std::make_pair(varname, new Assign{.expr = expr, .at = 234}));
  return 0;
}

bool variable_exists(char *varname) {
  return (std::find(def_varnames.begin(), def_varnames.end(), varname) !=
          def_varnames.end()) ||
         (std::find_if(assign_pairs.begin(), assign_pairs.end(), [=](auto p) {
            return p.first == varname;
          }) != assign_pairs.end());
}
