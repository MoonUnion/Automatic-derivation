#ifndef AUTOMATIC_DERIVATION_EXPRESSION_H
#define AUTOMATIC_DERIVATION_EXPRESSION_H

namespace expression {
  class Expr {
    public:
      virtual ~Expr() = default;
  };

  template<class T>
  class Constant : public Expr {
    public:
      explicit Constant(T value) : _value(value) {
      }
      ~Constant() override = default;

      decltype(auto) expr() const {
          return _value;
      }

    private:
      const T _value;
  };

  template<size_t args>
  class Operator : public Expr {
    public:
      Operator() = delete;
      ~Operator() override = default;
  };

  template<>
  class Operator<1> : public Expr {
    public:
      explicit Operator(Expr *expr) : _expr(expr) {
      }
      ~Operator() override = default;

      decltype(auto) expr() {
          return _expr;
      }

    private:
      Expr *_expr;
  };

  template<>
  class Operator<2> : public Expr {
    public:
      Operator(Expr *expr1, Expr *expr2) : _expr1(expr1), _expr2(expr2) {
      }
      ~Operator() override = default;

      decltype(auto) expr_left() {
          return _expr1;
      }
      decltype(auto) expr_right() {
          return _expr2;
      }

    private:
      Expr *_expr1;
      Expr *_expr2;
  };

  class Negative : public Operator<1> {
    public:
      explicit Negative(Expr *expr) : Operator(expr) {
      }
      ~Negative() override = default;
  };

  class Sine : public Operator<1> {
     public:
      explicit Sine(Expr *expr) : Operator(expr) {}
      ~Sine() override = default;
  };

  class Cosine : public Operator<1> {
     public:
      explicit Cosine(Expr *expr) : Operator(expr) {}
      ~Cosine() override = default;
  };

  class Tangent : public Operator<1> {
     public:
      explicit Tangent(Expr *expr) : Operator(expr) {}
      ~Tangent() override = default;
  };

  class Add : public Operator<2> {
    public:
      Add(Expr *expr1, Expr *expr2) : Operator(expr1, expr2) {
      }
      ~Add() override = default;
  };

  class Minus : public Operator<2> {
    public:
      Minus(Expr *expr1, Expr *expr2) : Operator(expr1, expr2) {
      }
      ~Minus() override = default;
  };

  class Multiply : public Operator<2> {
    public:
      Multiply(Expr *expr1, Expr *expr2) : Operator(expr1, expr2) {
      }
      ~Multiply() override = default;
  };

  class Divide : public Operator<2> {
    public:
      Divide(Expr *expr1, Expr *expr2) : Operator(expr1, expr2) {
      }
      ~Divide() override = default;
  };

}  // namespace expression
#endif  // AUTOMATIC_DERIVATION_EXPRESSION_H
