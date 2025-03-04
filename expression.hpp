#ifndef HEADER_GUARD_EXPRESSION_HPP_INCLUDED
#define HEADER_GUARD_EXPRESSION_HPP_INCLUDED

#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <complex>
#include <vector>

namespace Expressions {
    bool is_number(std::string s);
    bool is_operation(char c);
    void remove_spaces(std::string& s);

    template<typename T> struct Argument{
        bool is_var;
        T value;
        std::string var_name = "";
        std::string func = "none";

        Argument(const std::string s);
    };

    template <typename T> class Expression{
    private:
        std::vector<Argument> args;
        std::vector<char> operations;
        std::string str_expr;

    public:
        Expression(T expr);
        Expression(const std::string expr);
        Expression(const Expression& other);
        Expression operator = (const Expression& other);
        Expression operator = (const Expression&& other);
        ~Expression() = default;

        Expression evaluate_expr(T value);
        T mult_evaluate_expr(std::map<std::string, T> mp);

        Expression& operator + (const Expression& other) const;
        Expression& operator - (const Expression& other) const;
        Expression& operator * (const Expression& other) const;
        Expression& operator / (const Expression& other) const;
        Expression& operator ^ (const Expression& other) const;

        Expression sin(const Expression& expr);
        Expression cos(const Expression& expr);
        Expression ln(const Expression& expr);
        Expression exp(const Expression& expr);

        std::string toString() const;
    };
}

#endif // HEADER_GUARD_EXPRESSION_HPP_INCLUDED