#ifndef ASPIC_ERROR_HPP
#define ASPIC_ERROR_HPP

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
    static Error UnknownOperator(const std::string& str);
    static Error UnexpectedTokenType(Token::Type expected, Token::Type got);

    // Name
    static Error NameError(const std::string& var_name);

    // Type
    static Error TypeError(const std::string& str);
    static Error UnsupportedUnaryOperator(Token::Type operand, Token::OperatorType op);
    static Error UnsupportedBinaryOperator(Token::Type a, Token::Type b, Token::OperatorType op);

    // IndexError: bad index for operator []
    static Error IndexError(int index);

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
        Internal,
        Value,
    };

    Error(ID id, const std::string& message = "");

    std::string message_;
};

#endif
