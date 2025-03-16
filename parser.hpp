#ifndef HEADER_GUARD_PARSER_HPP_INCLUDED
#define HEADER_GUARD_PARSER_HPP_INCLUDED

#include <iostream>
#include <string>
#include <regex>
#include <set>
#include "expression.hpp"

namespace Expressions{

enum TokenType
{
    Number,         // Number e.g. "10"
    Variable,       // Variable e.g. "x"
    Plus,           // "+"
    Minus,          // "-"
    Mult,           // "*"
    Div,            // "/"
    Pow,            // "^"
    Sin,            // "sin"
    Cos,            // "cos"
    Ln,             // "ln"
    Exp,            // "exp"
    Left_bracket,   // "("
    Right_bracket,  // ")"
    Eof,            // "/n"
};

// regular expressions for some token types
constexpr const char* REGEXP_SPACE_SEQUENCE = "[ \t]+";
constexpr const char* REGEXP_VARIABLE       = "[a-zA-Z_]+";
constexpr const char* REGEXP_NUMBER        = "(\\+|-)?(0|[1-9][0-9]*)(\\.[0-9]+)?";

struct Token
{
    TokenType type;
    std::string lexeme;
    size_t column;
};

class Lexer
{
private:
    // string to be analyzed
    std::string input_;
    // current position in analyzed string
    const char* pos_;
    // final position in analyzed string
    const char* end_;
    // current index of analyzed string
    size_t column_;

    char peek() const;
    char get();

    Token getTokenByPattern(const std::regex& regexp, TokenType expectedTokenType);
    void skipTokenByPattern(const std::regex& regexp);

    void skipSpaceSequence();
    Token getVariable();
    Token getNumber();

public:
    Lexer(const std::string& input);
    ~Lexer() = default;

    Token getNextToken();
};

template<typename T>
class Parser
{
private:
    Lexer& lexer_;
    // current lexem
    Token currentToken_;
    // previous lexem
    Token previousToken_;

    // move to next lexem
    void advance();
    // move to next expected lexem, exception if token types dont match
    void expect(std::set<TokenType> types);
    // move to next lexem if token types match
    bool match(TokenType type);

    Expression<T> parseExpr();
    Expression<T> parseTerm();
    Expression<T> parseFactor();
public:
    Parser(Lexer& lexer);
    Expression<T> parseExpression();
};
} // namespace Expressions

#endif // HEADER_GUARD_PARSER_HPP_INCLUDED