#include "parser.hpp"
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

double __pow(std::vector<double> &args) {
  return std::pow(args[0], args[1]);
}

void load_functions() {
  functions.insert(
      std::make_pair("pow", CalcyFunction{.ptr = __pow, .argcount = 2}));
}
