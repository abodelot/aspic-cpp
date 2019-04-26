#include <iostream>
#include <cmath>

#include "LibCore.hpp"
#include "ast/NodeVector.hpp"
#include "ast/Node.hpp"
#include "Error.hpp"


Token core_assert(const ast::NodeVector& args)
{
    args.check(1);
    if (!args[0]->eval().as_bool()) {
        throw Error::AssertionError();
    }
    return Token(Token::NULL_VALUE);
}

Token core_print(const ast::NodeVector& args)
{
    for (size_t i = 0; i < args.size(); ++i) {
        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << args[i]->eval();
    }
    std::cout << std::endl;
    return Token(Token::NULL_VALUE);
}

Token core_input(const ast::NodeVector& args)
{
    args.check(1);
    std::string prompt = args[0]->eval().as_string();
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return Token::create_string(input);
}

Token core_type(const ast::NodeVector& args)
{
    args.check(1);
    return Token::create_string(Token::type_to_str(args[0]->eval().get_value_type()));
}

Token core_round(const ast::NodeVector& args)
{
    args.check(2);
    double num = args[0]->eval().as_float();
    int x = args[1]->eval().as_int();

    return Token::create_float(ceil( ( num * pow( 10, x ) ) - 0.49 ) / pow( 10, x ));
}

Token core_min(const ast::NodeVector& args)
{
    args.check(2);
    Token tok1 = args[0]->eval();
    Token tok2 = args[1]->eval();
    return tok1.apply_binary_operator(Token::OP_LESS_THAN, tok2).as_bool() ? tok1 : tok2;
}

Token core_max(const ast::NodeVector& args)
{
    args.check(2);
    Token tok1 = args[0]->eval();
    Token tok2 = args[1]->eval();
    return tok1.apply_binary_operator(Token::OP_GREATER_THAN, tok2).as_bool() ? tok1 : tok2;
}
