#include "parser.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include <utility>
#include <vector>

double __pow(std::vector<double> &args) { return std::pow(args[0], args[1]); }

double __random(std::vector<double> &args) {
  std::random_device device;
  std::uniform_real_distribution<double> dist;
  return dist(device);
}

void load_functions() {
  functions.insert(
      std::make_pair("pow", CalcyFunction{.ptr = __pow, .argcount = 2}));
  functions.insert(
      std::make_pair("random", CalcyFunction{.ptr = __random, .argcount = 0}));
}
