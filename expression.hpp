#ifndef HEADER_GUARD_EXPRESSION_HPP_INCLUDED
#define HEADER_GUARD_EXPRESSION_HPP_INCLUDED

#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <complex>
#include <vector>
#include <memory>

namespace Expressions {

template <typename T>
class ExpressionNode{
public:
    virtual ~ExpressionNode() = default;

    virtual std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values) const = 0;
    virtual T resolve() const = 0;
    virtual std::shared_ptr<ExpressionNode<T>> diff(const std::string &var) const = 0;
    virtual std::string to_string() const = 0;
};

template <typename T>
class NumberNode : public ExpressionNode<T>{
private:
    T val;
public:
    explicit NumberNode(T num);
    ~NumberNode() = default;
    virtual std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values) const override;
    virtual T resolve() const override;
    virtual std::shared_ptr<ExpressionNode<T>> diff(const std::string &var) const override;
    virtual std::string to_string() const override;
};

template <typename T>
class VariableNode : public ExpressionNode<T>{
private:
    std::string name;
public:
    VariableNode(std::string name);
    ~VariableNode() = default;
    virtual std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values) const override;
    virtual T resolve() const override;
    virtual std::shared_ptr<ExpressionNode<T>> diff(const std::string &var) const override;
    virtual std::string to_string() const override;
};

template <typename T>
class PlusNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> left;
    std::shared_ptr<ExpressionNode<T>> right;
public:
    explicit PlusNode(const std::shared_ptr<ExpressionNode<T>> &left, const std::shared_ptr<ExpressionNode<T>> &right);
    ~PlusNode() = default;
    virtual std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values) const override;
    virtual  T resolve() const override;
    virtual std::shared_ptr<ExpressionNode<T>> diff(const std::string &var) const override;
    virtual std::string to_string() const override;
};

template <typename T>
class MinusNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> left;
    std::shared_ptr<ExpressionNode<T>> right;
public:
    explicit MinusNode(const std::shared_ptr<ExpressionNode<T>> &left, const std::shared_ptr<ExpressionNode<T>> &right);
    ~MinusNode() = default;
    virtual std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values) const override;
    virtual T resolve() const override;
    virtual std::shared_ptr<ExpressionNode<T>> diff(const std::string &var) const override;
    virtual std::string to_string() const override;
};

template <typename T>
class MultNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> left;
    std::shared_ptr<ExpressionNode<T>> right;
public:
    explicit MultNode(const std::shared_ptr<ExpressionNode<T>> &left, const std::shared_ptr<ExpressionNode<T>> &right);
    ~MultNode() = default;
    virtual std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values) const override;
    virtual T resolve() const override;
    virtual std::shared_ptr<ExpressionNode<T>> diff(const std::string &var) const override;
    virtual std::string to_string() const override;
};

template <typename T>
class DivNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> left;
    std::shared_ptr<ExpressionNode<T>> right;
public:
    explicit DivNode(const std::shared_ptr<ExpressionNode<T>> &left, const std::shared_ptr<ExpressionNode<T>> &right);
    ~DivNode() = default;
    virtual std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values) const override;
    virtual T resolve() const override;
    virtual std::shared_ptr<ExpressionNode<T>> diff(const std::string &var) const override;
    virtual std::string to_string() const override;
};

template <typename T>
class PowNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> left;
    std::shared_ptr<ExpressionNode<T>> right;
public:
    explicit PowNode(const std::shared_ptr<ExpressionNode<T>> &left, const std::shared_ptr<ExpressionNode<T>> &right);
    ~PowNode() = default;
    virtual std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values) const override;
    virtual T resolve() const override;
    virtual std::shared_ptr<ExpressionNode<T>> diff(const std::string &var) const override;
    virtual std::string to_string() const override;
};

template <typename T>
class SinNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> arg;
public:
    explicit SinNode(std::shared_ptr<ExpressionNode<T>> arg);
    ~SinNode() = default;
    virtual std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values) const override;
    virtual T resolve() const override;
    virtual std::shared_ptr<ExpressionNode<T>> diff(const std::string &var) const override;
    virtual std::string to_string() const override;
};

template <typename T>
class CosNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> arg;
public:
    explicit CosNode(std::shared_ptr<ExpressionNode<T>> arg);
    ~CosNode() = default;
    virtual std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values) const override;
    virtual T resolve() const override;
    virtual std::shared_ptr<ExpressionNode<T>> diff(const std::string &var) const override;
    virtual std::string to_string() const override;
};

template <typename T>
class LnNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> arg;
public:
    explicit LnNode(std::shared_ptr<ExpressionNode<T>> arg);
    ~LnNode() = default;
    virtual std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values) const override;
    virtual T resolve() const override;
    virtual std::shared_ptr<ExpressionNode<T>> diff(const std::string &var) const override;
    virtual std::string to_string() const override;
};

template <typename T>
class ExpNode : public ExpressionNode<T>{
private:
    std::shared_ptr<ExpressionNode<T>> arg;
public:
    explicit ExpNode(std::shared_ptr<ExpressionNode<T>> arg);
    ~ExpNode() = default;
    virtual std::shared_ptr<ExpressionNode<T>> evaluate(std::vector<std::string> variables, std::vector<T> values) const override;
    virtual T resolve() const override;
    virtual std::shared_ptr<ExpressionNode<T>> diff(const std::string &var) const override;
    virtual std::string to_string() const override;
};


template <typename T> class Expression{
private:
    std::shared_ptr<ExpressionNode<T>> expr; // root of expression tree
public:
    Expression(T num);
    Expression(const std::string& expression);
    Expression(std::shared_ptr<ExpressionNode<T>> expression);
    Expression(const Expression<T>& other);
    Expression<T> operator = (const Expression<T>& other);
    Expression<T> operator = (const Expression<T>&& other);
    ~Expression() = default;

    Expression<T> diff(const std::string var) const;
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