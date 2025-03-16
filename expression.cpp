#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <complex>
#include <vector>
#include "expression.hpp"
#include <algorithm>
#include <map>
#include <memory>

namespace Expressions {

/*NODES*/

// NUMBER NODE
template <typename T> NumberNode<T>::NumberNode(T num) : val(num) {}

template <typename T>
std::shared_ptr<ExpressionNode<T>> NumberNode<T>::evaluate(std::vector<std::string> variables, std::vector<T> values){
    return std::make_shared<NumberNode<T>>(val);
}

template <typename T>
T NumberNode<T>::resolve() { return val; }

template <typename T>
std::shared_ptr<ExpressionNode<T>> NumberNode<T>::diff(const std::string &var){
    return std::make_shared<NumberNode<T>>(0);
}

template <typename T> 
std::string NumberNode<T>::to_string() const { return std::to_string(val); }


// VARIABLE NODE
template <typename T> VariableNode<T>::VariableNode(std::string name) : name(name) {}

template <typename T>
std::shared_ptr<ExpressionNode<T>> VariableNode<T>::diff(const std::string &var){
    if (name == var){ return std::make_shared<NumberNode<T>>(1); }
    return std::make_shared<NumberNode<T>>(0);
}

template <typename T>
std::shared_ptr<ExpressionNode<T>> VariableNode<T>::evaluate(std::vector<std::string> variables, std::vector<T> values){
    int i = 0;
    for(auto var : variables){
        if (var == name){ return std::make_shared<NumberNode<T>>(values[i]); }
        i++;
    }
    // variable not found
    return std::make_shared<NumberNode<T>>(0);
}

template <typename T>
T VariableNode<T>::resolve() { return 0; }

template <typename T>
std::string VariableNode<T>::to_string() const { return name; }


// PLUS NODE
template <typename T>
PlusNode<T>::PlusNode(std::shared_ptr<ExpressionNode<T>> left, std::shared_ptr<ExpressionNode<T>> right) :
left(left), right(right) {}

template <typename T>
std::shared_ptr<ExpressionNode<T>> PlusNode<T>::diff(const std::string &var){
    return std::make_shared<PlusNode<T>>(left->diff(var), right->diff(var));
}

template <typename T>
std::shared_ptr<ExpressionNode<T>> PlusNode<T>::evaluate(std::vector<std::string> variables, std::vector<T> values){
    return std::make_shared<PlusNode<T>>(left->evaluate(variables, values), right->evaluate(variables, values));
}

template <typename T>
T PlusNode<T>::resolve() { return left->resolve() + right->resolve(); }

template <typename T> std::string PlusNode<T>::to_string() const { return left->to_string() + " + " + right->to_string(); }


// MINUS NODE
template <typename T>
MinusNode<T>::MinusNode(std::shared_ptr<ExpressionNode<T>> left, std::shared_ptr<ExpressionNode<T>> right) :
left(left), right(right) {}

template <typename T>
std::shared_ptr<ExpressionNode<T>> MinusNode<T>::diff(const std::string &var){
    return std::make_shared<MinusNode<T>>(MinusNode<T>(left->diff(var), right->diff(var)));
}

template <typename T>
std::shared_ptr<ExpressionNode<T>> MinusNode<T>::evaluate(std::vector<std::string> variables, std::vector<T> values){
    return std::make_shared<MinusNode<T>>(MinusNode<T>(left->evaluate(variables, values), right->evaluate(variables, values)));
}

template <typename T>
T MinusNode<T>::resolve() { return left->resolve() - right->resolve(); }

template <typename T> std::string MinusNode<T>::to_string() const { return left->to_string() + " - "  + right->to_string(); }


// MULTIPLICATION NODE
template <typename T>
MultNode<T>::MultNode(std::shared_ptr<ExpressionNode<T>> left, std::shared_ptr<ExpressionNode<T>> right) :
left(left), right(right) {}

template <typename T>
std::shared_ptr<ExpressionNode<T>> MultNode<T>::diff(const std::string &var){
    // f'g' = f'g + fg'
    return std::make_shared<PlusNode<T>>(MultNode<T>(left->diff(var), right), MultNode<T>(left, right->diff(var)));
}

template <typename T>
std::shared_ptr<ExpressionNode<T>> MultNode<T>::evaluate(std::vector<std::string> variables, std::vector<T> values){
    return std::make_shared<MultNode<T>>(MultNode<T>(left->evaluate(variables, values), right->evaluate(variables, values)));
}

template <typename T>
T MultNode<T>::resolve() { return left->resolve() * right->resolve(); }

template <typename T> std::string MultNode<T>::to_string() const { return left->to_string() + " * " + right->to_string(); }


// DIVISION NODE
template <typename T>
DivNode<T>::DivNode(std::shared_ptr<ExpressionNode<T>> left, std::shared_ptr<ExpressionNode<T>> right) :
left(left), right(right) {}

template <typename T>
std::shared_ptr<ExpressionNode<T>> DivNode<T>::diff(const std::string &var){
    // (f/g)' = (f'g - fg') / g^2
    MinusNode<T> numerator = MinusNode<T>(MultNode<T>(left->diff(var), right), MultNode<T>(left, right->diff(var)));
    PowNode<T> denominator = PowNode<T>(right, NumberNode<T>(2));
    return std::make_shared<DivNode<T>>(DivNode<T>(numerator, denominator));
}

template <typename T>
std::shared_ptr<ExpressionNode<T>> DivNode<T>::evaluate(std::vector<std::string> variables, std::vector<T> values){
    return std::make_shared<DivNode<T>>(DivNode<T>(left->evaluate(variables, values), right->evaluate(variables, values)));
}

template <typename T>
T DivNode<T>::resolve() { return left->resolve() / right->resolve(); }

template <typename T> std::string DivNode<T>::to_string() const { return left->to_string() + " / " + right->to_string(); }


// POWER NODE
template <typename T>
PowNode<T>::PowNode(std::shared_ptr<ExpressionNode<T>> left, std::shared_ptr<ExpressionNode<T>> right) :
left(left), right(right) {}

template <typename T>
std::shared_ptr<ExpressionNode<T>> PowNode<T>::diff(const std::string &var){
    // (f^g)' = (g * f^(g - 1) * f') + (f^(g) * ln(f) * g')
    //                  left_p       +      right_p  
    // f = left, g = right

    // f^(g - 1)
    PowNode<T> f_pow_g = PowNode<T>(left, MinusNode<T>(right, NumberNode<T>(1)));
    // g * f^(g - 1) * f'
    MultNode<T> left_p = MultNode<T>(MultNode<T>(right, left->diff(var)), f_pow_g);
    // f^(g) * ln(f) * g'
    MultNode<T> right_p = MultNode<T>(MultNode<T>(PowNode<T>(left, right), right->diff(var)), LnNode<T>(left));

    return std::make_shared<PlusNode<T>>(PlusNode<T>(left_p, right_p));
}

template <typename T>
std::shared_ptr<ExpressionNode<T>> PowNode<T>::evaluate(std::vector<std::string> variables, std::vector<T> values){
    return std::make_shared<PowNode<T>>(PowNode(left->evaluate(variables, values), right->evaluate(variables, values)));
}

template <typename T>
T PowNode<T>::resolve() { return std::pow(left->resolve(), right->resolve()); }

template <typename T> std::string PowNode<T>::to_string() const { return left->to_string() + '^' + right->to_string(); }


// SIN NODE
template <typename T>
SinNode<T>::SinNode(std::shared_ptr<ExpressionNode<T>> arg) : arg(arg) {}

template <typename T>
std::shared_ptr<ExpressionNode<T>> SinNode<T>::diff(const std::string &var){
    // (sin f(x))' = (cos f(x)) * f'(x)
    return std::make_shared<MultNode<T>>(CosNode<T>(arg), arg->diff(var));
}

template <typename T>
std::shared_ptr<ExpressionNode<T>> SinNode<T>::evaluate(std::vector<std::string> variables, std::vector<T> values){
    return std::make_shared<SinNode<T>>(SinNode(arg->evaluate(variables, values)));
}

template <typename T>
T SinNode<T>::resolve() { return std::sin(arg->resolve()); }

template <typename T> std::string SinNode<T>::to_string() const { return "sin(" + arg->to_string() + ")"; }


// COS NODE
template <typename T>
CosNode<T>::CosNode(std::shared_ptr<ExpressionNode<T>> arg) : arg(arg) {}

template <typename T>
std::shared_ptr<ExpressionNode<T>> CosNode<T>::diff(const std::string &var){
    // (cos f(x))' = (-sin f(x)) * f'(x)
    return std::make_shared<MultNode<T>>(MultNode<T>(SinNode<T>(arg), MultNode<T>(NumberNode<T>(-1), arg->diff(var))));
}

template <typename T>
std::shared_ptr<ExpressionNode<T>> CosNode<T>::evaluate(std::vector<std::string> variables, std::vector<T> values){
    return std::make_shared<CosNode<T>>(CosNode<T>(arg->evaluate(variables, values)));
}

template <typename T>
T CosNode<T>::resolve() { return std::cos(arg->resolve()); }

template <typename T> std::string CosNode<T>::to_string() const { return "cos(" + arg->to_string() + ")"; }


// LN NODE
template <typename T>
LnNode<T>::LnNode(std::shared_ptr<ExpressionNode<T>> arg) : arg(arg) {}

template <typename T>
std::shared_ptr<ExpressionNode<T>> LnNode<T>::diff(const std::string &var){
    // (ln f(x))' = f'(x) / f(x)
    return std::make_shared<DivNode<T>>(DivNode<T>(arg->diff(var), arg));
}

template <typename T>
std::shared_ptr<ExpressionNode<T>> LnNode<T>::evaluate(std::vector<std::string> variables, std::vector<T> values){
    return std::make_shared<LnNode<T>>(LnNode<T>(arg->evaluate(variables, values)));
}

template <typename T>
T LnNode<T>::resolve() { return std::log(arg->resolve()); }

template <typename T> std::string LnNode<T>::to_string() const { return "ln(" + arg->to_string() + ")"; }


// EXP NODE
template <typename T>
ExpNode<T>::ExpNode(std::shared_ptr<ExpressionNode<T>> arg) : arg(arg) {}

template <typename T>
std::shared_ptr<ExpressionNode<T>> ExpNode<T>::diff(const std::string &var){
    // (exp f(x))' = (exp f(x)) * f'(x)
    return std::make_shared<MultNode<T>>(MultNode<T>(ExpNode<T>(arg), arg->diff(var)));
}

template <typename T>
std::shared_ptr<ExpressionNode<T>> ExpNode<T>::evaluate(std::vector<std::string> variables, std::vector<T> values){
    return std::make_shared<ExpNode<T>>(ExpNode<T>(arg->evaluate(variables, values)));
}

template <typename T>
T ExpNode<T>::resolve() { return std::exp(arg->resolve()); }

template <typename T> std::string ExpNode<T>::to_string() const { return "exp(" + arg->to_string() + ")"; }



/*EXPRESSIONS*/



// number constructor
template <typename T>
Expression<T>::Expression(T num) : expr(std::make_shared<NumberNode<T>>(num)) {}

// string constructor
//UNFIN
template <typename T>
Expression<T>::Expression(const std::string expression){
    expr = std::make_shared<ExpressionNode>(Parser<T>::parseExpression(expresssion));
}

// expression node constructor
template <typename T>
Expression<T>::Expression(std::shared_ptr<ExpressionNode<T>> expression) : expr(expression) {}

// copy constructor
template <typename T>
Expression<T>::Expression(const Expression<T>& other) : expr(other.expr) {}

// copy operator
template <typename T>
Expression<T> Expression<T>::operator = (const Expression<T>& other){
    if (this != &other) {
        expr = other.expr;
    }
    return *this;
}

// move operator
template <typename T>
Expression<T> Expression<T>::operator = (const Expression<T>&& other){
    if (this != &other) {
        expr = std::move(other.expr);
    }
    return *this;
}

// differantiates expression by given variable
template <typename T>
Expression<T> diff(std::string var){
    return Expression<T>(expr->diff());
}

// evaluates expression with given variable values
// not all variables may be evaluated
// returns expression
template <typename T>
Expression<T> Expression<T>::evaluate(std::vector<std::string> variables, std::vector<T> values){
    return Expression<T>(expr->evaluate(variables, values));
}

// resolves current expression
// !!guaranteed that no variables are unevaluated!!
// returns type T value
template <typename T>
T Expression<T>::resolve(){
    return expr->resolve();
}

// calculates expression with given variable values
// all variables evaluated
// returns type T value
template <typename T>
T Expression<T>::eval_and_resolve(std::vector<std::string> variables, std::vector<T> values){
    return expr->evaluate(variables, values)->resolve();
}


/*operators*/

template <typename T>
Expression<T> Expression<T>::operator + (const Expression<T>& other) const{
    return Expression<T>(std::make_shared<PlusNode<T>>(PlusNode<T>(expr, other.expr)));
}

template <typename T>
Expression<T> Expression<T>::operator - (const Expression<T>& other) const{
    return Expression<T>(std::make_shared<MinusNode<T>>(MinusNode<T>(expr, other.expr)));
}

template <typename T>
Expression<T> Expression<T>::operator * (const Expression<T>& other) const{
    return Expression<T>(std::make_shared<MultNode<T>>(MultNode<T>(expr, other.expr)));
}

template <typename T>
Expression<T> Expression<T>::operator / (const Expression<T>& other) const{
    return Expression<T>(std::make_shared<DivNode<T>>(expr, other.expr));
}

template <typename T>
Expression<T> Expression<T>::operator ^ (const Expression<T>& other) const{
    return Expression<T>(std::make_shared<PowNode<T>>(expr, other.expr));
}


/*functions*/

template <typename T>
Expression<T> Expression<T>::sin(){
    return Expression<T>(std::make_shared<SinNode<T>>(expr));
}

template <typename T>
Expression<T> Expression<T>::cos(){
    return Expression<T>(std::make_shared<CosNode<T>>(expr));
}

template <typename T>
Expression<T> Expression<T>::ln(){
    return Expression<T>(std::make_shared<LnNode<T>>(expr));
}

template <typename T>
Expression<T> Expression<T>::exp(){
    return Expression<T>(std::make_shared<ExpNode<T>>(expr));
}

template <typename T>
std::string Expression<T>::to_string() const{
    return expr->to_string();
}

} // namespace Expressions