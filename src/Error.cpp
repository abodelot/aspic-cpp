#include "Error.hpp"
#include "Token.hpp"
#include "Operators.hpp"

#include <sstream>


Error Error::AssertionError()
{
    return Error(Assertion);
}

// Syntax

Error Error::SyntaxError(const std::string& str)
{
    return Error(Syntax, str);
}

Error Error::UnknownOperator(const std::string& str)
{
    return Error(Syntax, "operator '" + str + "' does not exist");
}

Error Error::UnexpectedToken(const Token& unexpected)
{
    std::ostringstream oss;
    oss << "Unexpected token '" << unexpected << "'";
    return Error(Syntax, oss.str());
}

Error Error::UnexpectedToken(const Token& expected, const Token& got)
{
    std::ostringstream oss;
    oss << "unexpected token '" << got << "', expecting '" << expected << "'";
    return Error(Syntax, oss.str());
}

Error Error::NameError(const std::string& identifier)
{
    return Error(Name, "name '" + identifier + "' is not defined");
}

// Type Error

Error Error::TypeError(const std::string& str)
{
    return Error(Type, str);
}

Error Error::TypeConvertError(Object::Type from, Object::Type to)
{
    std::ostringstream oss;
    oss << "Cannot convert '" << Object::type_to_str(from)
        << "' to '" << Object::type_to_str(to) << "'";
    return Error(Type, oss.str());
}

Error Error::UnsupportedUnaryOperator(Object::Type operand, Operator op)
{
    Error e(Type);
    e.message_ += ": type '";
    e.message_ += Object::type_to_str(operand);
    e.message_ += "' doesn't support unary operator '";
    e.message_ += Operators::to_str(op);
    e.message_ += "'";
    return e;
}

Error Error::UnsupportedBinaryOperator(Object::Type a, Object::Type b, Operator op)
{
    Error e(Type);
    e.message_ += ": unsupported binary operator '";
    e.message_ += Operators::to_str(op);
    e.message_ += "' for operands '";
    e.message_ += Object::type_to_str(a);
    e.message_ += "' and '";
    e.message_ += Object::type_to_str(b);
    e.message_ += "'";
    return e;
}

Error Error::IndexError(int index)
{
    std::ostringstream oss;
    oss << "index " << index << " is out of range";
    return Error(Index, oss.str());
}

Error Error::KeyError(const std::string& str)
{
    return Error(Key, str);
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
        case Key:       message_ = "KeyError";       break;
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
