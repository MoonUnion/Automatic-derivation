#include <iostream>

#include "expression.h"

using namespace std;

int main() {
    namespace e = expression;
    auto a = make_shared<e::Constant<int>>(1);
    auto b = make_shared<e::Variable<int>>("x");
    auto expr = make_shared<e::Add>(a, b);
    std::cout << dynamic_pointer_cast<e::Add>(expr)->to_string() << std::endl;
    return 0;
}
