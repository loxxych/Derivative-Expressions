#include "parser.hpp"
#include <stdexcept>

namespace Expressions{

/*LEXER*/

// see next symbol
char Lexer::peek() const { return *pos_; }

// get next symbol
char Lexer::get(){
    char c = peek();
    pos_++;
    return c;
}

// gets token from analyzed string based on a regular expression
Token Lexer::getTokenByPattern(const std::regex& regexp, TokenType expectedTokenType){
    std::cmatch matchResult{};

    // searching for pattern in string
    bool matchStatus = std::regex_search(pos_, end_, matchResult, regexp, std::regex_constants::match_continuous);
    
    if (!matchStatus) // pattern not found
    {
        throw std::runtime_error(
            std::string("Unexpected subexpression on pos ") + std::to_string(column_));
    }

    // moving current position in string
    pos_ = matchResult[0].second;

    // the found matching regexp string
    std::string lexemeStr = std::string(matchResult[0].first, matchResult[0].second);

    // return found token
    return Token{expectedTokenType, std::move(lexemeStr), column_};
}

// skips given token (interpreted as a regexp) in analyzed string
void Lexer::skipTokenByPattern(const std::regex& regexp){
    std::cmatch matchResult{};

    // searching for pattern in string
    bool matchStatus = std::regex_search(pos_, end_, matchResult, regexp, std::regex_constants::match_continuous);
    if (matchStatus){
        // moving current position
        pos_ = matchResult[0].second;
    }
}

// skips unnecessary spaces
void Lexer::skipSpaceSequence(){
    static const std::regex spacePattern{REGEXP_SPACE_SEQUENCE};
    skipTokenByPattern(spacePattern);
}

// gets next variable in string
Token Lexer::getVariable(){
    static const std::regex variablePattern{REGEXP_VARIABLE};
    return getTokenByPattern(variablePattern, Variable);
}

// gets next number in string
Token Lexer::getNumber(){
    static const std::regex numberPattern{REGEXP_NUMBER};
    return getTokenByPattern(numberPattern, Number);
}

// constructor
Lexer::Lexer(const std::string& input) : input_(input), pos_(), end_(), column_(0){
    pos_ = input_.c_str();
    end_ = pos_ + input_.size();
}

// gets next token of the string
Token Lexer::getNextToken()
{
    skipSpaceSequence();

    // reached EOF
    if (pos_ >= end_)
    {
        return Token{Eof, "", column_};
    }

    // getting next symbol to identify the lexem
    char currentChar = peek();

    if (isalpha(currentChar) || currentChar == '_')
    {
        Token token = getVariable();
        if (token.lexeme == "sin") {
            return Token{Sin, "sin", column_};
        } else if (token.lexeme == "cos") {
            return Token{Cos, "cos", column_};
        } else if (token.lexeme == "ln") {
            return Token{Ln, "ln", column_};
        } else if (token.lexeme == "exp") {
            return Token{Exp, "exp", column_};
        } else if (token.lexeme == "i") {
            return Token{Number, "i", column_};
        } else {
            // variable
            return token;
        }
    }
    else if (isdigit(currentChar) || currentChar == '+' || currentChar == '-'){
        Token token = getNumber();
        // check if complex
        if (peek() == 'i') {
            get(); // skip 'i'
            token.lexeme += "i";
            token.type = Number;
        }
        return token;
    }
    else if (currentChar == '+'){
        get();
        return Token{Plus, "+", column_};
    }
    else if (currentChar == '-'){
        get();
        return Token{Minus, "-", column_};
    }
    else if (currentChar == '*'){
        get();
        return Token{Mult, "*", column_};
    }
    else if (currentChar == '/'){
        get();
        return Token{Div, "/", column_};
    }
    else if (currentChar == '^'){
        get();
        return Token{Pow, "^", column_};
    }
    else if (currentChar == '('){
        get();
        return Token{Left_bracket, "(", column_};
    }
    else if (currentChar == ')'){
        get();
        return Token{Right_bracket, ")", column_};
    }
    else{
        throw std::runtime_error(
            std::string("Unexpected subexpression on pos ") + std::to_string(column_));
    }
}



/*PARSER*/



// parser constructor based on a lexer
template<typename T>
Parser<T>::Parser(Lexer& lexer) : lexer_(lexer), currentToken_(), previousToken_(){
    // read first lexeme
    advance();
}

// moves to next lexeme
template<typename T>
void Parser<T>::advance(){
    previousToken_ = currentToken_;
    currentToken_ = lexer_.getNextToken();
}

// advances to next lexeme if the token type is in the given set
template<typename T>
void Parser<T>::expect(std::set<TokenType> types){
    if (!types.contains(currentToken_.type)){
        // expected token not found in given set
        throw std::runtime_error(
            "Got unexpected token \"" + currentToken_.lexeme +
            "\" of type " + std::to_string(currentToken_.type));
    }

    // token found, we move to next lexem
    advance();
}

// advances to next lexeme if the token is of the given type
// returns true = success, false = fail
template<typename T>
bool Parser<T>::match(TokenType type){
    if (currentToken_.type == type){
        advance();
        return true;
    }
    return false;
}

template<typename T>
Expression<T> Parser<T>::parseExpr(){
    Expression<T> expr = parseTerm();
    while (currentToken_.type == Plus){
        // getting '+'
        advance();

        // getting next operand
        Expression term = parseTerm();

        // updating expression
        expr = expr + term;
    }

    return expr;
}

// template<typename T>
// Expression<T> Parser<T>::parseFactor()
// {
//     if (match(Left_bracket)){
//         Expression<T> expr = parseExpr();
//         expect({Right_bracket});
//         return expr;
//     }

//     if (match(Number))
//     {
//         std::string lexeme = previousToken_.lexeme;
//         if (lexeme == "i") {
//             // single 'i'
//             return Expression<T>(std::complex<double>(0, 1)); // 0 + 1i
//         } else if (lexeme.find('i') != std::string::npos) {
//             // complex with coef
//             lexeme.pop_back(); // deleting 'i'
//             double imagPart = std::stod(lexeme);
//             return Expression<T>(std::complex<double>(0, imagPart)); // 0 + bi
//         } else {
//             // not complex
//             return Expression<T>(std::stold(lexeme));
//         }
//     }

//     if (match(Variable)){
//         return Expression<T>(previousToken_.lexeme);
//     }

//     if (match(Sin)){
//         expect({Left_bracket});
//         Expression<T> arg = parseExpr();
//         expect({Right_bracket});
//         return arg.sin();
//     }

//     if (match(Cos)){
//         expect({Left_bracket});
//         Expression<T> arg = parseExpr();
//         expect({Right_bracket});
//         return arg.cos();
//     }

//     if (match(Ln)){
//         expect({Left_bracket});
//         Expression<T> arg = parseExpr();
//         expect({Right_bracket});
//         return arg.ln();
//     }

//     if (match(Exp)){
//         expect({Left_bracket});
//         Expression<T> arg = parseExpr();
//         expect({Right_bracket});
//         return arg.exp();
//     }


//     throw std::runtime_error(
//         "Got unexpected token \"" + currentToken_.lexeme +
//         "\" of type " + std::to_string(currentToken_.type));
// }

// template<>
// Expression<std::complex<double>> Parser<std::complex<double>>::parseFactor()
// {
//     if (match(Left_bracket)){
//         Expression<std::complex<double>> expr = parseExpr();
//         expect({Right_bracket});
//         return expr;
//     }

//     if (match(Number))
//     {
//         std::string lexeme = previousToken_.lexeme;
//         if (lexeme == "i") {
//             // single 'i'
//             return Expression<std::complex<double>>(std::complex<double>(0, 1)); // 0 + 1i
//         } else if (lexeme.find('i') != std::string::npos) {
//             // complex with coef
//             lexeme.pop_back(); // deleting 'i'
//             double imagPart = std::stod(lexeme);
//             return Expression<std::complex<double>>(std::complex<double>(0, imagPart)); // 0 + bi
//         } else {
//             // not complex
//             return Expression<std::complex<double>>(static_cast<std::complex<double>>(std::stod(lexeme), 0));
//         }
//     }

//     if (match(Variable)){
//         return Expression<std::complex<double>>(std::complex<double>(std::stod(previousToken_.lexeme), 0));
//     }

//     if (match(Sin)){
//         expect({Left_bracket});
//         Expression<std::complex<double>> arg = parseExpr();
//         expect({Right_bracket});
//         return arg.sin();
//     }

//     if (match(Cos)){
//         expect({Left_bracket});
//         Expression<std::complex<double>> arg = parseExpr();
//         expect({Right_bracket});
//         return arg.cos();
//     }

//     if (match(Ln)){
//         expect({Left_bracket});
//         Expression<std::complex<double>> arg = parseExpr();
//         expect({Right_bracket});
//         return arg.ln();
//     }

//     if (match(Exp)){
//         expect({Left_bracket});
//         Expression<std::complex<double>> arg = parseExpr();
//         expect({Right_bracket});
//         return arg.exp();
//     }


//     throw std::runtime_error(
//         "Got unexpected token \"" + currentToken_.lexeme +
//         "\" of type " + std::to_string(currentToken_.type));
// }

template<>
Expression<long double> Parser<long double>::parseFactor()
{
    if (match(Left_bracket)){
        Expression<long double> expr = parseExpr();
        expect({Right_bracket});
        return expr;
    }

    if (match(Number)){
        return Expression<long double>(std::stold(previousToken_.lexeme));
    }

    if (match(Variable)){
        return Expression<long double>(previousToken_.lexeme);
    }

    if (match(Sin)){
        expect({Left_bracket});
        Expression<long double> arg = parseExpr();
        expect({Right_bracket});
        return arg.sin();
    }

    if (match(Cos)){
        expect({Left_bracket});
        Expression<long double> arg = parseExpr();
        expect({Right_bracket});
        return arg.cos();
    }

    if (match(Ln)){
        expect({Left_bracket});
        Expression<long double> arg = parseExpr();
        expect({Right_bracket});
        return arg.ln();
    }

    if (match(Exp)){
        expect({Left_bracket});
        Expression<long double> arg = parseExpr();
        expect({Right_bracket});
        return arg.exp();
    }

    throw std::runtime_error(
        "Got unexpected token \"" + currentToken_.lexeme +
        "\" of type " + std::to_string(currentToken_.type));
}

template<typename T>
Expression<T> Parser<T>::parseTerm(){
    Expression<T> term = parseFactor();

    while (currentToken_.type == Mult){
        // reading "*"
        advance();

        // getting next multiplicand
        Expression factor = parseFactor();

        // updating expression
        term = term * factor;
    }

    return term;
}


// parses full expression
template<typename T>
Expression<T> Parser<T>::parseExpression(){
    Expression<T> expr = parseExpr();

    expect({Eof});

    return expr;
}

template class Parser<long double>;
//template class Parser<std::complex<long double>>;

} // namespace Expressions