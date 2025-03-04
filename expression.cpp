#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <complex>
#include <vector>
#include "expression.hpp"
#include <algorithm>
#include <map>

namespace Expressions{
    
    /*AUXILARY FUNCTIONS*/

    bool is_number(std::string s) {
        int ind = 0;
        if (s[0] == '-') {
            ind++;
        }
        bool found_point = false;
        while (ind < s.size()) {
            if (s[ind] == '.') {
                if (found_point) { // aka 2 points found
                    return false;
                }
                ind++;
                found_point = true;
                continue;
            }
            if (!std::isdigit(s[ind])) {
                return false;
            }
            ind++;
        }
        return true;
    }

    bool is_operation(char c) {
        if (c == '+' || c == '-' || 
        c == '/' || c == '*' || c == '^') {
            return true;
        }
        return false;
    }

    void remove_spaces(std::string& s) {
        while (s.back() == ' ') {
            s.pop_back();
        }
        std::reverse(s.begin(), s.end());

        while (s.back() == ' ') {
            s.pop_back();
        }
        std::reverse(s.begin(), s.end());
    }
    
    /*ARGUMENTS*/

    // only numbers and variables
    Argument::Argument(const std::string s) {
        if (is_number(s)) {
            is_var = false;
            value = strtold(s.c_str());
        } else {
            is_var = true;
            var_name = s;
        }
    }

    // sets variable value
    void evaluate(T val) {
        value = val;
    }

    /*EXPRESSIONS*/

    // number constructor
    Expression::Expression(T num){
        args.push_back(Argument(num));
        str_expr = std::to_string(num);
    }

    //string constructor
    Expression::Expression(const std::string expr){
        str_expr = expr;
        std::string temp = "";

        for (size_t i = 0; i < expr.size(); i++) {
            if (is_operation(s[i])) {
                remove_spaces(&temp);

                args.push_back(Argument(temp));
                operations.push_back(s[i]);
                temp = ""; 
            } else if (i == expr.size() - 1) {
                remove_spaces(&temp);

                args.push_back(Argument(temp));
                temp = "";
            } else {
                temp += s[i];
            }
        }
    }

    // copy constructor
    Expression::Expression(const Expression& other){
        args = other->getArgs();
        operations = other->getOps();
    }

    // copy operator
    Expression& Expression::operator = (const Expression& other){
        args = other->getArgs();
        operations = other->getOps();
        return this*;
    }

    // move operator
    Expression& Expression::operator = (const Expression&& other){
        memcpy(this->args, other->args, sizeof(other->args));
        memcpy(this->operations, other->operations, sizeof(other->operations));
        return this*;
    }

    // calculate expression with 1 given variable value
    T Expression::evaluate_expr(std::string name, T value){
        for (Argument arg : args) {
            if (arg.is_var) {
                if(arg.var_name == name){
                    arg.evaluate(value);
                }
            }
        }

        T left = args[0].value, right;
        T res = 0;
        for (auto i = 0; i < operations.size(); i++) {
            right = args[i + 1].value;
            switch(operations[i]){
            case '+': res += left + right;
            case '-': res += left - right;
            case '/': res += left / right;
            case '*': res += left * right;
            case '^': res += pow(left, right);
            }
            left = res;
            }
        if (operations.size() == 0) {
            res = left;
        }

        return res;
    }

    // calculate expression with multiple given variable values
    T Expression::mult_evaluate_expr(std::map<std::string, T> mp) {
        for (Argument arg : args) {
            if (arg.is_var) {
                arg.evaluate(mp[arg.var_name]);
            }
        }
        T left = args[0].value, right;
        T res = 0;
        for (auto i = 0; i < operations.size(); i++) {
            right = args[i + 1].value;
            switch(operations[i]){
            case '+': res += left + right;
            case '-': res += left - right;
            case '/': res += left / right;
            case '*': res += left * right;
            case '^': res += pow(left, right);
            }
            left = res;
        }
        if (operations.size() == 0) {
            res = left;
        }
        return res;
    }

    /*operators*/

    Expression& Expression::operator + (const Expression& other) const{
        Expression res(*this);
        res.args.insert(res.args.end(), other.args.begin());
        res.operations.push_back('+');
        return *res;
    }

    Expression& Expression::operator - (const Expression& other) const{
        Expression res(*this);
        res.args.insert(res.args.end(), other.args.begin());
        res.operations.push_back('-');
        return *res;
    }
    Expression& Expression::operator * (const Expression& other) const;
    Expression& Expression::operator / (const Expression& other) const;
    Expression& Expression::operator ^ (const Expression& other) const;

    /*functions*/

    Expression Expression::sin(const Expression& expr);
    Expression Expression::cos(const Expression& expr);
    Expression Expression::ln(const Expression& expr);
    Expression Expression::exp(const Expression& expr);

    std::string Expression::toString() const{
        return str_expr;
    };
}