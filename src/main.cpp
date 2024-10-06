#include "parser.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <utility>

extern FILE *yyin;
extern int yyparse();
void load_functions();

double execute_expr(Expr *expr,
                    std::unordered_map<std::string, double> &resolved) {
  switch (expr->kind) {
  case ExprType::CALL: {
    auto func = functions[expr->call->name];
    std::vector<double> funcargs;
    while (*expr->call->args) {
      funcargs.push_back(execute_expr(*expr->call->args, resolved));
      expr->call->args++;
    }
    return func.ptr(funcargs);
  }
  case ExprType::REF:
    return resolved[expr->ref];
  case ExprType::VAL:
    return expr->val;
  case ExprType::BIN:
    switch (expr->bin->type) {
    case BinaryExprType::ADD:
      return execute_expr(expr->bin->a, resolved) +
             execute_expr(expr->bin->b, resolved);
    case BinaryExprType::SUB:
      return execute_expr(expr->bin->a, resolved) -
             execute_expr(expr->bin->b, resolved);
    case BinaryExprType::MUL:
      return execute_expr(expr->bin->a, resolved) *
             execute_expr(expr->bin->b, resolved);
    case BinaryExprType::DIV:
      auto a = execute_expr(expr->bin->a, resolved);
      auto b = execute_expr(expr->bin->b, resolved);
      if (b == 0) {
        std::cerr << "Division by zero";
        exit(1);
      }
      return a / b;
    };
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Required calcy [inputfile]\n";
    return 1;
  }
  yyin = fopen(argv[1], "r");
  if (!yyin) {
    std::cerr << "Cannot open input file\n";
    return 1;
  }
  load_functions();
  yyparse();
  std::unordered_map<std::string, double> resolved_def_varnames;
  for (auto it = def_varnames.begin(); it != def_varnames.end(); it++) {
    std::cout << "Enter '" << *it << "': ";
    double buf;
    std::cin >> buf;
    resolved_def_varnames.insert(std::make_pair(*it, buf));
  }
  for (auto it = assign_pairs.begin(); it != assign_pairs.end(); it++) {
    auto result = execute_expr(it->second->expr, resolved_def_varnames);
    resolved_def_varnames.insert(std::make_pair(it->first, result));
  }
  auto result = execute_expr(out_expr, resolved_def_varnames);
  std::cout << "Result: " << result << "\n";
}
