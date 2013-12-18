#include "Error.hpp"
#include "OperatorManager.hpp"


Error Error::SyntaxError(const std::string& str)
{
	return Error(Syntax, str);
}


Error Error::UnknownOperator(const std::string& str)
{
	return Error(Syntax, "operator '" + str + "' does not exist");
}


Error Error::NameError(const std::string& identifier)
{
	return Error(Name, "name '" + identifier + "' is not defined");
}


Error Error::TypeError(const std::string& str)
{
	return Error(Type, str);
}


Error Error::UnsupportedOperator(Token::Type operand, Token::OperatorType op)
{
	Error e(Type);
	e.message_ += "type '";
	e.message_ += Token::type_to_str(operand);
	e.message_ += "' doesn't support operator '";
	e.message_ += OperatorManager::to_str(op);
	e.message_ += "'";
	return e;
}


Error Error::UnsupportedBinaryOperator(Token::Type a, Token::Type b, Token::OperatorType op)
{
	Error e(Type);
	e.message_ += "unsupported binary operator '";
	e.message_ += OperatorManager::to_str(op);
	e.message_ += "' for operands '";
	e.message_ += Token::type_to_str(a);
	e.message_ += "' and '";
	e.message_ += Token::type_to_str(b);
	e.message_ += "'";
	return e;
}


Error Error::InternalError(const std::string& str)
{
	return Error(Internal, str);
}


Error Error::DivideByZero()
{
	return Error(Runtime, "division or modulo by 0");
}


Error::Error(ID id, const std::string& message)
{
	switch (id)
	{
		case Syntax:   message_ = "[SyntaxError] ";   break;
		case Name:     message_ = "[NameError] ";     break;
		case Type:     message_ = "[TypeError] ";     break;
		case Internal: message_ = "[InternalError] "; break;
		case Runtime:  message_ = "[RuntimeError] ";  break;
	}
	message_ += message;
}


Error::~Error() throw()
{
}


const char* Error::what() const throw()
{
	return message_.c_str();
}
