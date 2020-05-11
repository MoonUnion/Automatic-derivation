#include <iostream>

#include "expression.h"

using namespace std;

int main() {
  using namespace expression;
  using t = ExprType;
  auto a = make_const(1);
  auto b = make_var<int>("x");
  auto expr = make<t::add>(a, make<t::power>(make<t::sin>(make_const(2)), b));
  std::cout << dynamic_pointer_cast<Add>(expr)->to_string() << std::endl;
  return 0;
}
