#include "LibCore.hpp"
#include "ast/NodeVector.hpp"
#include "ast/Node.hpp"
#include "Error.hpp"

#include <iostream>
#include <cmath>


Object core_assert(const ast::NodeVector& args)
{
    args.check(1);
    if (!args[0]->eval().truthy()) {
        throw Error::AssertionError();
    }
    return Object::create_null();
}

Object core_print(const ast::NodeVector& args)
{
    for (size_t i = 0; i < args.size(); ++i) {
        if (i > 0) {
            std::cout << ' ';
        }
        std::cout << args[i]->eval();
    }
    std::cout << std::endl;
    return Object::create_null();
}

Object core_input(const ast::NodeVector& args)
{
    args.check(1);
    std::string prompt = args[0]->eval().get_string();
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return Object::create_string(input);
}

Object core_type(const ast::NodeVector& args)
{
    args.check(1);
    return Object::create_string(Object::type_to_str(args[0]->eval().get_value_type()));
}

Object core_round(const ast::NodeVector& args)
{
    args.check(2);
    double num = args[0]->eval().get_float();
    int x = args[1]->eval().get_int();

    return Object::create_float(ceil( ( num * pow( 10, x ) ) - 0.49 ) / pow( 10, x ));
}

Object core_min(const ast::NodeVector& args)
{
    args.check(2);
    Object arg1 = args[0]->eval();
    Object arg2 = args[1]->eval();
    return arg1.apply_binary_operator(Operator::OP_LESS_THAN, arg2).truthy() ? arg1 : arg2;
}

Object core_max(const ast::NodeVector& args)
{
    args.check(2);
    Object arg1 = args[0]->eval();
    Object arg2 = args[1]->eval();
    return arg1.apply_binary_operator(Operator::OP_GREATER_THAN, arg2).truthy() ? arg1 : arg2;
}
