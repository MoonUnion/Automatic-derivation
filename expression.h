#ifndef AUTOMATIC_DERIVATION_EXPRESSION_H
#define AUTOMATIC_DERIVATION_EXPRESSION_H

#include <memory>
#include <string>
#include <string_view>
#include <typeinfo>
#include <utility>
#include <vector>

namespace expression {
  class Expr {
    public:
      using ptr = std::shared_ptr<Expr>;
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

  template<int args>
  class Operator : public Expr {
    public:
      Operator() = delete;
      ~Operator() override = default;
  };

  template<>
  class Operator<1> : public Expr {
    public:
      explicit Operator(ptr expr) : _expr(std::move(expr)) {}
      ~Operator() override = default;

      decltype(auto) expr() { return _expr; }
      decltype(auto) expr() const { return _expr; }
      std::string to_string() const override {
          return "?(" + expr()->to_string() + ")";
      }

    private:
      ptr _expr;
  };

  template<>
  class Operator<2> : public Expr {
    public:
      Operator(ptr expr1, ptr expr2) : _expr1(std::move(expr1)), _expr2(std::move(expr2)) {}
      ~Operator() override = default;

      decltype(auto) expr_left() { return _expr1; }
      decltype(auto) expr_right() { return _expr2; }
      decltype(auto) expr_left() const { return _expr1; }
      decltype(auto) expr_right() const { return _expr2; }
      std::string to_string() const override {
          return expr_left()->to_string() + " ? " + expr_right()->to_string();
      }

    private:
      ptr _expr1;
      ptr _expr2;
  };

  template<>
  class Operator<-1> : public Expr {
    public:
      explicit Operator(std::vector<ptr> expr_list) : _expr_list(std::move(expr_list)) {}
      explicit Operator(std::vector<ptr> &&expr_list) : _expr_list(std::move(expr_list)) {}
      explicit Operator(ptr expr) : _expr_list{std::move(expr)} {}
      Operator(ptr expr1, ptr expr2) : _expr_list{std::move(expr1), std::move(expr2)} {}

      ~Operator() override = default;

      void add(ptr expr) { _expr_list.push_back(std::move(expr)); }
      decltype(auto) expr_list() { return _expr_list; }
      decltype(auto) expr_list() const { return _expr_list; }
      std::string to_string() const override {
          std::string temp{"("};
          auto s = _expr_list.size() - 1;
          for (size_t i = 0; i < s; ++i) {
              temp += _expr_list[i]->to_string() + " ? ";
          }
          return temp + _expr_list[s]->to_string() + ")";
      }

    private:
      std::vector<ptr> _expr_list;
  };

  class Negative : public Operator<1> {
    public:
      explicit Negative(ptr expr) : Operator(std::move(expr)) {}
      ~Negative() override = default;
      std::string to_string() const override { return "-" + expr()->to_string(); }
  };

  class Sine : public Operator<1> {
    public:
      explicit Sine(ptr expr) : Operator(std::move(expr)) {}
      ~Sine() override = default;
      std::string to_string() const override {
          return "sin(" + expr()->to_string() + ")";
      }
  };

  class Cosine : public Operator<1> {
    public:
      explicit Cosine(ptr expr) : Operator(std::move(expr)) {}
      ~Cosine() override = default;
      std::string to_string() const override {
          return "cos(" + expr()->to_string() + ")";
      }
  };

  class Tangent : public Operator<1> {
    public:
      explicit Tangent(ptr expr) : Operator(std::move(expr)) {}
      ~Tangent() override = default;
      std::string to_string() const override {
          return "tan(" + expr()->to_string() + ")";
      }
  };

  class Add2 : public Operator<2> {
    public:
      Add2(ptr expr1, ptr expr2) : Operator(std::move(expr1), std::move(expr2)) {}
      ~Add2() override = default;
      std::string to_string() const override {
          return expr_left()->to_string() + " + " + expr_right()->to_string();
      }
  };

  class Add : public Operator<-1> {
    public:
      explicit Add(const std::vector<ptr> &exprList) : Operator(exprList) {}
      Add(ptr expr1, ptr expr2) : Operator(std::move(expr1), std::move(expr2)) {}
      ~Add() override = default;
      std::string to_string() const override {
          std::string temp;
          const auto &e = expr_list();
          auto s = e.size() - 1;
          for (size_t i = 0; i < s; ++i) {
              temp += e[i]->to_string() + " + ";
          }
          return temp + e[s]->to_string();
      }
  };

  class Minus2 : public Operator<2> {
    public:
      Minus2(ptr expr1, ptr expr2) : Operator(std::move(expr1), std::move(expr2)) {}
      ~Minus2() override = default;
      std::string to_string() const override {
          return expr_left()->to_string() + " - " + expr_right()->to_string();
      }
  };

  class Minus : public Operator<-1> {
    public:
      explicit Minus(const std::vector<ptr> &exprList) : Operator(exprList) {}
      Minus(ptr expr1, ptr expr2) : Operator(std::move(expr1), std::move(expr2)) {}
      ~Minus() override = default;
      std::string to_string() const override {
          std::string temp;
          const auto &e = expr_list();
          auto s = e.size() - 1;
          for (size_t i = 0; i < s; ++i) {
              temp += e[i]->to_string() + " + ";
          }
          return temp + e[s]->to_string();
      }
  };

  class Multiply2 : public Operator<2> {
    public:
      Multiply2(ptr expr1, ptr expr2) : Operator(std::move(expr1), std::move(expr2)) {}
      ~Multiply2() override = default;
      std::string to_string() const override {
          return expr_left()->to_string() + " * " + expr_right()->to_string();
      }
  };
    class Multiply : public Operator<-1> {
    public:
        explicit Multiply(const std::vector<ptr> &exprList) : Operator(exprList) {}
        Multiply(ptr expr1, ptr expr2) : Operator(std::move(expr1), std::move(expr2)) {}
        ~Multiply() override = default;
        std::string to_string() const override {
            std::string temp;
            const auto &e = expr_list();
            auto s = e.size() - 1;
            for (size_t i = 0; i < s; ++i) {
                temp += e[i]->to_string() + " * ";
            }
            return temp + e[s]->to_string();
        }
    };

  class Divide : public Operator<2> {
    public:
      Divide(ptr expr1, ptr expr2) : Operator(std::move(expr1), std::move(expr2)) {}
      ~Divide() override = default;
      std::string to_string() const override {
          return expr_left()->to_string() + " / " + expr_right()->to_string();
      }
  };

  class Power : public Operator<2> {
    public:
      Power(ptr expr1, ptr expr2) : Operator(std::move(expr1), std::move(expr2)) {}
      ~Power() override = default;
      std::string to_string() const override {
          return expr_left()->to_string() + "^" + expr_right()->to_string();
      }
  };

  class Logarithmic : public Operator<2> {
    public:
      Logarithmic(ptr expr1, ptr expr2) : Operator(std::move(expr1), std::move(expr2)) {}
      ~Logarithmic() override = default;
      std::string to_string() const override {
          return "log " + expr_left()->to_string() + "(" + expr_right()->to_string() + ")";
      }
  };

  enum class ExprType {
      negative, sin, cos, tan, add, minus, add2, minus2, mult, mult2, div, power, log, null
  };

  template<ExprType t>
  inline Expr::ptr make(const Expr::ptr &) {
      return nullptr;
  }
  template<ExprType t>
  inline Expr::ptr make(const Expr::ptr &, const Expr::ptr &) {
      return nullptr;
  }
  template<class T>
  inline Expr::ptr make_const(T value) {
      return std::make_shared<Constant<T>>(value);
  }
  template<class T>
  inline Expr::ptr make_var(std::string_view name) {
      return std::make_shared<Variable<T>>(name);
  }
  template<>
  inline Expr::ptr make<ExprType::negative>(const Expr::ptr &expr) {
      return std::make_shared<Negative>(expr);
  }
  template<>
  inline Expr::ptr make<ExprType::sin>(const Expr::ptr &expr) {
      return std::make_shared<Sine>(expr);
  }
  template<>
  inline Expr::ptr make<ExprType::cos>(const Expr::ptr &expr) {
      return std::make_shared<Cosine>(expr);
  }
  template<>
  inline Expr::ptr make<ExprType::tan>(const Expr::ptr &expr) {
      return std::make_shared<Tangent>(expr);
  }
  template<>
  inline Expr::ptr make<ExprType::add2>(const Expr::ptr &left, const Expr::ptr &right) {
      return std::make_shared<Add2>(left, right);
  }
  template<>
  inline Expr::ptr make<ExprType::minus2>(const Expr::ptr &left, const Expr::ptr &right) {
      return std::make_shared<Minus2>(left, right);
  }
  template<>
  inline Expr::ptr make<ExprType::add>(const Expr::ptr &left, const Expr::ptr &right) {
      return std::make_shared<Add>(left, right);
  }
  template<>
  inline Expr::ptr make<ExprType::minus>(const Expr::ptr &left, const Expr::ptr &right) {
      return std::make_shared<Minus>(left, right);
  }
    template<>
    inline Expr::ptr make<ExprType::mult>(const Expr::ptr &left, const Expr::ptr &right) {
        return std::make_shared<Multiply>(left, right);
    }
  template<>
  inline Expr::ptr make<ExprType::mult2>(const Expr::ptr &left, const Expr::ptr &right) {
      return std::make_shared<Multiply2>(left, right);
  }
  template<>
  inline Expr::ptr make<ExprType::div>(const Expr::ptr &left, const Expr::ptr &right) {
      return std::make_shared<Divide>(left, right);
  }
  template<>
  inline Expr::ptr make<ExprType::power>(const Expr::ptr &left, const Expr::ptr &right) {
      return std::make_shared<Power>(left, right);
  }
  template<>
  inline Expr::ptr make<ExprType::log>(const Expr::ptr &left, const Expr::ptr &right) {
      return std::make_shared<Logarithmic>(left, right);
  }

}  // namespace expression
#endif  // AUTOMATIC_DERIVATION_EXPRESSION_H
