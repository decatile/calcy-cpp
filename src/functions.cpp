#include "parser.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include <utility>
#include <vector>

using CalcyArgs = std::vector<double> &;

double __pow(CalcyArgs args) { return std::pow(args[0], args[1]); }

double __floor(CalcyArgs args) { return std::floor(args[0]); }

double __ceil(CalcyArgs args) { return std::ceil(args[0]); }

double __min(CalcyArgs args) { return std::min(args[0], args[1]); }

double __max(CalcyArgs args) { return std::max(args[0], args[1]); }

double __random(CalcyArgs args) {
  std::random_device device;
  std::uniform_real_distribution<double> dist;
  return dist(device);
}

#define INSERT(name, args)                                                     \
  functions.insert(                                                            \
      std::make_pair(#name, CalcyFunction{.ptr = __##name, .argcount = args}))

void load_functions() {
  INSERT(pow, 2);
  INSERT(floor, 1);
  INSERT(ceil, 1);
  INSERT(min, 2);
  INSERT(max, 2);
  INSERT(random, 0);
}
