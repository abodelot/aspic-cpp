#include <sstream>

#include "Error.hpp"
#include "OperatorManager.hpp"


Error Error::AssertionError()
{
    return Error(Assertion);
}

// Syntax

Error Error::SyntaxError(const std::string& str)
{
    return Error(Syntax, str);
}

Error Error::UnexpectedToken(const Token& token)
{
    std::ostringstream oss;
    oss << "Unexpected token " << token;
    return Error(Syntax, oss.str());
}

Error Error::UnknownOperator(const std::string& str)
{
    return Error(Syntax, "operator '" + str + "' does not exist");
}

Error Error::UnexpectedTokenType(Token::Type expected, Token::Type got)
{
    Error e(Syntax);
    e.message_ += ": expected ";
    e.message_ += Token::type_to_str(expected);
    e.message_ += ", got ";
    e.message_ += Token::type_to_str(got);
    return e;
}

Error Error::NameError(const std::string& identifier)
{
    return Error(Name, "name '" + identifier + "' is not defined");
}

Error Error::TypeError(const std::string& str)
{
    return Error(Type, str);
}

Error Error::UnsupportedUnaryOperator(Token::Type operand, Token::OperatorType op)
{
    Error e(Type);
    e.message_ += ": type '";
    e.message_ += Token::type_to_str(operand);
    e.message_ += "' doesn't support unary operator '";
    e.message_ += OperatorManager::to_str(op);
    e.message_ += "'";
    return e;
}

Error Error::UnsupportedBinaryOperator(Token::Type a, Token::Type b, Token::OperatorType op)
{
    Error e(Type);
    e.message_ += ": unsupported binary operator '";
    e.message_ += OperatorManager::to_str(op);
    e.message_ += "' for operands '";
    e.message_ += Token::type_to_str(a);
    e.message_ += "' and '";
    e.message_ += Token::type_to_str(b);
    e.message_ += "'";
    return e;
}

Error Error::IndexError(int index)
{
    std::ostringstream oss;
    oss << "index " << index << " is out of range";
    return Error(Index, oss.str());

}
Error Error::InternalError(const std::string& str)
{
    return Error(Internal, str);
}

Error Error::ValueError(const std::string& str)
{
    return Error(Value, str);
}

Error Error::DivideByZero()
{
    return Error(Value, "division or modulo by 0");
}

Error::Error(ID id, const std::string& message)
{
    switch (id) {
        case Assertion: message_ = "AssertionError"; break;
        case Syntax:    message_ = "SyntaxError";    break;
        case Name:      message_ = "NameError";      break;
        case Type:      message_ = "TypeError";      break;
        case Index:     message_ = "IndexError";     break;
        case Internal:  message_ = "InternalError";  break;
        case Value:     message_ = "ValueError";     break;
    }
    if (message.size()) {
        message_ += ": " + message;
    }
}

const char* Error::what() const noexcept
{
    return message_.c_str();
}
