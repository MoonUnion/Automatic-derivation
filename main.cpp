#include <iostream>

#include "expression.h"


int main() {
  namespace e = expression;
  auto a = e::Constant(1);
  auto b = e::Variable<int>("x");
  auto expr = e::Add(&a, &b);
  std::cout << "Hello, World!" << std::endl;
  return 0;
}
