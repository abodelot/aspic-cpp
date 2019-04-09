#include <iostream>
#include <cmath>

#include "LibCore.hpp"
#include "Error.hpp"


Token core_assert(TokenStack& args)
{
    args.check(1);
    bool result= args.pop_bool();
    if (!result) {
        throw Error::AssertionError();
    }
    return Token(Token::NULL_VALUE);
}

Token core_print(TokenStack& args)
{
    args.check(1);
    Token arg = args.top(); args.pop();
    std::cout << arg << std::endl;
    return Token(Token::NULL_VALUE);
}

Token core_input(TokenStack& args)
{
    args.check(1);
    std::string prompt = args.pop_string();
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return Token::create_string(input);
}

Token core_type(TokenStack& args)
{
    args.check(1);
    Token arg = args.top(); args.pop();
    return Token::create_string(Token::type_to_str(arg.get_contained_type()));
}

Token core_round(TokenStack& args)
{
    args.check(2);
    int x = args.pop_int();
    double num = args.pop_float();

    return Token::create_float(ceil( ( num * pow( 10, x ) ) - 0.49 ) / pow( 10, x ));
}

Token core_min(TokenStack& args)
{
    args.check(2);
    Token tok2 = args.top(); args.pop();
    Token tok1 = args.top(); args.pop();
    return tok1.apply_binary_operator(Token::OP_LESS_THAN, tok2).as_bool() ? tok1 : tok2;
}

Token core_max(TokenStack& args)
{
    args.check(2);
    Token tok2 = args.top(); args.pop();
    Token tok1 = args.top(); args.pop();
    return tok1.apply_binary_operator(Token::OP_GREATER_THAN, tok2).as_bool() ? tok1 : tok2;
}
