#ifndef HEADER_GUARD_EXPRESSION_HPP_INCLUDED
#define HEADER_GUARD_EXPRESSION_HPP_INCLUDED

#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <complex>
#include <vector>

namespace Expressions {

template <typename T>
class ExpressionNode{
public:
    virtual ~ExpressionNode() = default;
    virtual std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values) = 0;
    virtual T resolve() = 0;
    virtual std::shared_ptr<ExpressionNode<T>> diff(const std::string &var) = 0;
    virtual std::string to_string() const = 0;
};

template <typename T>
class NumberNode : public ExpressionNode<T>{
private:
    T val;
public:
    NumberNode(T num);
    ~NumberNode() = default;
    std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values) override;
    T resolve() override;
    std::shared_ptr<ExpressionNode<T>> diff(const std::string &var) override;
    std::string to_string() const override;
};

template <typename T>
class VariableNode : public ExpressionNode<T>{
private:
    std::string name;
public:
    VariableNode(std::string name);
    ~VariableNode() = default;
    std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values);
    T resolve();
    std::shared_ptr<ExpressionNode<T>> diff(const std::string &var);
    std::string to_string() const;
};

template <typename T>
class PlusNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> left;
    std::shared_ptr<ExpressionNode<T>> right;
public:
    PlusNode(std::shared_ptr<ExpressionNode<T>> left, std::shared_ptr<ExpressionNode<T>> right);
    ~PlusNode() = default;
    std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values);
    T resolve();
    std::shared_ptr<ExpressionNode<T>> diff(const std::string &var);
    std::string to_string() const;
};

template <typename T>
class MinusNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> left;
    std::shared_ptr<ExpressionNode<T>> right;
public:
    MinusNode(std::shared_ptr<ExpressionNode<T>> left, std::shared_ptr<ExpressionNode<T>> right);
    ~MinusNode() = default;
    std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values);
    T resolve();
    std::shared_ptr<ExpressionNode<T>> diff(const std::string &var);
    std::string to_string() const;
};

template <typename T>
class MultNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> left;
    std::shared_ptr<ExpressionNode<T>> right;
public:
    MultNode(std::shared_ptr<ExpressionNode<T>> left, std::shared_ptr<ExpressionNode<T>> right);
    ~MultNode() = default;
    std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values);
    T resolve();
    std::shared_ptr<ExpressionNode<T>> diff(const std::string &var);
    std::string to_string() const;
};

template <typename T>
class DivNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> left;
    std::shared_ptr<ExpressionNode<T>> right;
public:
    DivNode(std::shared_ptr<ExpressionNode<T>> left, std::shared_ptr<ExpressionNode<T>> right);
    ~DivNode() = default;
    std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values);
    T resolve();
    std::shared_ptr<ExpressionNode<T>> diff(const std::string &var);
    std::string to_string() const;
};

template <typename T>
class PowNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> left;
    std::shared_ptr<ExpressionNode<T>> right;
public:
    PowNode(std::shared_ptr<ExpressionNode<T>> left, std::shared_ptr<ExpressionNode<T>> right);
    ~PowNode() = default;
    std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values);
    T resolve();
    std::shared_ptr<ExpressionNode<T>> diff(const std::string &var);
    std::string to_string() const;
};

template <typename T>
class SinNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> arg;
public:
    SinNode(std::shared_ptr<ExpressionNode<T>> arg);
    ~SinNode() = default;
    std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values);
    T resolve();
    std::shared_ptr<ExpressionNode<T>> diff(const std::string &var);
    std::string to_string() const;
};

template <typename T>
class CosNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> arg;
public:
    CosNode(std::shared_ptr<ExpressionNode<T>> arg);
    ~CosNode() = default;
    std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values);
    T resolve();
    std::shared_ptr<ExpressionNode<T>> diff(const std::string &var);
    std::string to_string() const;
};

template <typename T>
class LnNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> arg;
public:
    LnNode(std::shared_ptr<ExpressionNode<T>> arg);
    ~LnNode() = default;
    std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values);
    T resolve();
    std::shared_ptr<ExpressionNode<T>> diff(const std::string &var);
    std::string to_string() const;
};

template <typename T>
class ExpNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> arg;
public:
    ExpNode(std::shared_ptr<ExpressionNode<T>> arg);
    ~ExpNode() = default;
    std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values);
    T resolve();
    std::shared_ptr<ExpressionNode<T>> diff(const std::string &var);
    std::string to_string() const;
};


template <typename T> class Expression{
private:
    std::shared_ptr<ExpressionNode<T>> expr; // root of expression tree
public:
    Expression<T>(T num);
    Expression<T>(const std::string expression);
    Expression<T>(std::shared_ptr<ExpressionNode<T>> expression);
    Expression<T>(const Expression<T>& other);
    Expression<T> operator = (const Expression<T>& other);
    Expression<T> operator = (const Expression<T>&& other);
    ~Expression() = default;

    Expression<T> diff(std::string var);
    Expression<T> evaluate(std::vector<std::string> variables, std::vector<T> values);
    T resolve();
    T eval_and_resolve(std::vector<std::string> variables, std::vector<T> values);

    Expression<T> operator + (const Expression<T>& other) const;
    Expression<T> operator - (const Expression<T>& other) const;
    Expression<T> operator * (const Expression<T>& other) const;
    Expression<T> operator / (const Expression<T>& other) const;
    Expression<T> operator ^ (const Expression<T>& other) const;

    Expression<T> sin();
    Expression<T> cos();
    Expression<T> ln();
    Expression<T> exp();

    std::string to_string() const;
};
} // namespace Expressions

#endif // HEADER_GUARD_EXPRESSION_HPP_INCLUDED