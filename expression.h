#ifndef AUTOMATIC_DERIVATION_EXPRESSION_H
#define AUTOMATIC_DERIVATION_EXPRESSION_H

#include <string>
#include <string_view>
#include <typeinfo>

namespace expression {
  class Expr {
    public:
      virtual ~Expr() = default;
      virtual std::string to_string() const { return ""; }
  };

  template<class T>
  class Constant : public Expr {
    public:
      explicit Constant(T value) : _value(value) {}
      ~Constant() override = default;

      decltype(auto) expr() const { return _value; }
      std::string to_string() const override { return std::to_string(_value); }

    private:
      const T _value;
  };

  template<class T>
  class Variable : public Expr {
    public:
      explicit Variable(std::string_view name) : _name(name) {}
      ~Variable() override = default;

      decltype(auto) name() const { return _name; }
      std::string to_string() const override { return _name; }

    private:
      std::string _name;
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
      explicit Operator(Expr *expr) : _expr(expr) {}
      ~Operator() override = default;

      decltype(auto) expr() { return _expr; }
      decltype(auto) expr() const { return _expr; }
      std::string to_string() const override {
          return "?(" + expr()->to_string() + ")";
      }

    private:
      Expr *_expr;
  };

  template<>
  class Operator<2> : public Expr {
    public:
      Operator(Expr *expr1, Expr *expr2) : _expr1(expr1), _expr2(expr2) {}
      ~Operator() override = default;

      decltype(auto) expr_left() { return _expr1; }
      decltype(auto) expr_right() { return _expr2; }
      decltype(auto) expr_left() const { return _expr1; }
      decltype(auto) expr_right() const { return _expr2; }
      std::string to_string() const override {
          return expr_left()->to_string() + " ? " + expr_right()->to_string();
      }

    private:
      Expr *_expr1;
      Expr *_expr2;
  };

  class Negative : public Operator<1> {
    public:
      explicit Negative(Expr *expr) : Operator(expr) {}
      ~Negative() override = default;
      std::string to_string() const override { return "-" + expr()->to_string(); }
  };

  class Sine : public Operator<1> {
    public:
      explicit Sine(Expr *expr) : Operator(expr) {}
      ~Sine() override = default;
      std::string to_string() const override {
          return "sin(" + expr()->to_string() + ")";
      }
  };

  class Cosine : public Operator<1> {
    public:
      explicit Cosine(Expr *expr) : Operator(expr) {}
      ~Cosine() override = default;
      std::string to_string() const override {
          return "cos(" + expr()->to_string() + ")";
      }
  };

  class Tangent : public Operator<1> {
    public:
      explicit Tangent(Expr *expr) : Operator(expr) {}
      ~Tangent() override = default;
      std::string to_string() const override {
          return "tan(" + expr()->to_string() + ")";
      }
  };

  class Add : public Operator<2> {
    public:
      Add(Expr *expr1, Expr *expr2) : Operator(expr1, expr2) {}
      ~Add() override = default;
      std::string to_string() const override {
          return expr_left()->to_string() + " + " + expr_right()->to_string();
      }
  };

  class Minus : public Operator<2> {
    public:
      Minus(Expr *expr1, Expr *expr2) : Operator(expr1, expr2) {}
      ~Minus() override = default;
      std::string to_string() const override {
          return expr_left()->to_string() + " - " + expr_right()->to_string();
      }
  };

  class Multiply : public Operator<2> {
    public:
      Multiply(Expr *expr1, Expr *expr2) : Operator(expr1, expr2) {}
      ~Multiply() override = default;
      std::string to_string() const override {
          return expr_left()->to_string() + " * " + expr_right()->to_string();
      }
  };

  class Divide : public Operator<2> {
    public:
      Divide(Expr *expr1, Expr *expr2) : Operator(expr1, expr2) {}
      ~Divide() override = default;
      std::string to_string() const override {
          return expr_left()->to_string() + " / " + expr_right()->to_string();
      }
  };

}  // namespace expression
#endif  // AUTOMATIC_DERIVATION_EXPRESSION_H
