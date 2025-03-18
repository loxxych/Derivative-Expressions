#include "expression.hpp"
#include "parser.hpp"
#include <string>
#include <vector>
#include <iostream>

void run_tests(){
    // expression constructors

    std::cout << "Test 1: ";
    Expressions::Expression<long double> expr1(10.5);
    if (expr1.to_string() == "10.5"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n"; }

    std::cout << "Test 2: ";
    Expressions::Expression<long double> expr2_1("x");
    if (expr2_1.to_string() == "x"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n"; }

    std::cout << "Test 3: ";
    Expressions::Expression<std::complex<double>> expr2_2("1 + i");
    if (expr2_2.to_string() == "(x + i)"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n"; }


    std::cout << "Test 4: ";
    Expressions::Expression<long double> expr3("x * 5 + 3.5");
    if (expr3.to_string() == "((x * 5) + 3.5)"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n"; }

    // operations

    // +
    std::cout << "Test 5: ";
    Expressions::Expression<long double> expr44("x * 5");
    Expressions::Expression<long double> expr55("y");
    Expressions::Expression<long double> expr66 = expr44 + expr55;
    if (expr66.to_string() == "((x * 5) + y)"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n";}

    // -
    std::cout << "Test 6: ";
    Expressions::Expression<long double> expr4("x * 5");
    Expressions::Expression<long double> expr5("y");
    Expressions::Expression<long double> expr6 = expr4 - expr5;
    if (expr6.to_string() == "((x * 5) - y)"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n";}

    // *
    std::cout << "Test 7: ";
    Expressions::Expression<long double> expr7("6.6 - x");
    Expressions::Expression<long double> expr8("2");
    Expressions::Expression<long double> expr9 = expr7 * expr8;
    if (expr9.to_string() == "((6.6 - x) * 2)"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n";}

    // div
    std::cout << "Test 8: ";
    Expressions::Expression<long double> expr10("x * 5");
    Expressions::Expression<long double> expr11("y");
    Expressions::Expression<long double> expr12 = expr10 / expr11;
    if (expr12.to_string() == "((x * 5) / y)"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n";}

    // ^
    std::cout << "Test 9: ";
    Expressions::Expression<long double> expr13("x * 5");
    Expressions::Expression<long double> expr14("y");
    Expressions::Expression<long double> expr15 = expr13 ^ expr14;
    if (expr15.to_string() == "((x * 5) ^ y)"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n"; }

    // functions
    std::cout << "Test 10: ";
    Expressions::Expression<long double> expr16("x + 5");
    if (expr16.sin().to_string() == "sin((x + 5))"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n"; }

    std::cout << "Test 11: ";
    Expressions::Expression<long double> expr17("x + 5");
    if (expr17.cos().to_string() == "cos((x + 5))"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n"; }

    std::cout << "Test 12: ";
    Expressions::Expression<long double> expr18("x + 5");
    if (expr18.ln().to_string() == "ln((x + 5))"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n"; }

    std::cout << "Test 13: ";
    Expressions::Expression<long double> expr19("x + 5");
    if (expr3.exp().to_string() == "exp((x + 5))"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n"; }

    // evaluate
    std::cout << "Test 14: ";
    Expressions::Expression<long double> expr20("x + 5 * y");
    Expressions::Expression<long double> expr21 = expr20.evaluate(std::vector<std::string> {"x"}, std::vector<long double> {10});
    if (expr21.to_string() == "(10 + (5 * y))"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n"; }

    // resolve
    std::cout << "Test 15: ";
    Expressions::Expression<long double> expr22("5 * 10");
    Expressions::Expression<long double> expr23 = expr22.resolve();
    if (expr23.to_string() == "50"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n"; }

    // evaluate and resolve
    std::cout << "Test 16: ";
    Expressions::Expression<long double> expr24("x + 5 * y");
    Expressions::Expression<long double> expr25 = expr24.evaluate(std::vector<std::string> {"x", "y"}, std::vector<long double> {10, 2});
    if (expr25.to_string() == "20"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n"; }

    // diff
    std::cout << "Test 17: ";
    Expressions::Expression<long double> expr26("ln(x)");
    Expressions::Expression<long double> expr27 = expr26.diff("x");
    if (expr27.to_string() == "1 / x"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n"; }

    std::cout << "Test 18: ";
    Expressions::Expression<long double> expr28("x + 5 * y");
    Expressions::Expression<long double> expr29 = expr28.diff("y");
    if (expr29.to_string() == "0 + 5 * 1"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n"; }

    std::cout << "Test 19: ";
    Expressions::Expression<long double> expr30("x ^ y");
    Expressions::Expression<long double> expr31 = expr30.diff("y");
    Expressions::Expression<long double> expr32 = expr30.diff("x");
    if (expr31.to_string() == "((x ^ y) * ln(x))" && expr31.to_string() == "()"){
        std::cout << "OK\n";
    } else { std::cout << "FAIL\n"; }
    
}

int main(){
    run_tests();
    return 0;
}