#include <cstdlib>

#include "Error.hpp"
#include "OperatorManager.hpp"



Error Error::SyntaxError(const std::string& str)
{
	return Error(str);
}

Error Error::NameError(const std::string& str)
{
	return Error(str);
}

Error Error::DivideByZero()
{
	return Error("division or modulo by 0");
}

Error Error::MissingLeftBracket()
{
	return Error("a left bracket is missing");
}

Error Error::MissingRightBracket()
{
	return Error("a right bracket is missing");
}

Error Error::InvalidToken(char token)
{
	Error e;
	e.message_ += "token '";
	e.message_ += token;
	e.message_ += "' is invalid";
	return e;
}

Error Error::UnknownOperator(const std::string& str)
{
	return Error("operator '" + str + "' does not exist");
}

Error Error::UnsupportedOperator(Token::Type operand, Token::OperatorType op)
{
	Error e;
	e.message_ += "type '";
	e.message_ += Token::to_str(operand);
	e.message_ += "' doesn't support operator '";
	e.message_ += OperatorManager::to_str(op);
	e.message_ += "'";
	return e;
}

Error Error::UnsupportedBinaryOperator(Token::Type a, Token::Type b, Token::OperatorType op)
{
	Error e;
	e.message_ += "unsupported binary operator '";
	e.message_ += OperatorManager::to_str(op);
	e.message_ += "' for operands '";
	e.message_ += Token::to_str(a);
	e.message_ += "' and '";
	e.message_ += Token::to_str(b);
	e.message_ += "'";
	return e;
}


Error::Error(const std::string& message)
{
	message_ = message;
}


Error::~Error() throw()
{
}


const char* Error::what() const throw()
{
	return message_.c_str();
}
