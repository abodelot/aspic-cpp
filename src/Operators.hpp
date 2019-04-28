#ifndef ASPIC_OPERATORS_HPP
#define ASPIC_OPERATORS_HPP

#include "Token.hpp"

/**
 * Helper functions for operators
 * See Token::OperatorType enum for list of supported operators
 * See Scanner class for mapping symbols to operators
 */
class Operators
{
public:
    static int get_binding_power(Token::OperatorType op);

    static const char* to_str(Token::OperatorType op);

private:
    Operators() = delete;
};

#endif
