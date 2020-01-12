#ifndef ASPIC_ERROR_HPP
#define ASPIC_ERROR_HPP

#include "Operators.hpp"
#include "Token.hpp"

#include <exception>
#include <string>

/**
 * Error management
 */
class Error: public std::exception
{
public:
    static Error AssertionError();

    // Syntax
    static Error SyntaxError(const std::string& str);
    static Error UnexpectedToken(Token::Type token_type);
    static Error UnknownOperator(const std::string& str);
    static Error UnexpectedTokenType(Token::Type expected, Token::Type got);

    // Name
    static Error NameError(const std::string& var_name);

    // Type
    static Error TypeError(const std::string& str);
    static Error TypeConvertError(Object::Type from, Object::Type to);
    static Error UnsupportedUnaryOperator(Object::Type operand, Operator op);
    static Error UnsupportedBinaryOperator(Object::Type a, Object::Type b, Operator op);

    // Invalid array index
    static Error IndexError(int index);

    // Invalid hashmap key
    static Error KeyError(const std::string& index);

    // Internal
    static Error InternalError(const std::string& str);

    // ValueError
    static Error ValueError(const std::string& str);
    static Error DivideByZero();

    const char* what() const noexcept override;

private:
    enum ID
    {
        Assertion,
        Syntax,
        Name,
        Type,
        Index,
        Key,
        Internal,
        Value,
    };

    Error(ID id, const std::string& message = "");

    std::string message_;
};

#endif
