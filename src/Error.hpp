#ifndef ERROR_HPP
#define ERROR_HPP

#include "Token.hpp"

#include <exception>
#include <string>

/**
 * Error management
 */
class Error: public std::exception
{
public:
	enum ID
	{
		Syntax,
		Name,
		Type,
		Internal,
		Runtime
	};
	// Syntax
	static Error SyntaxError(const std::string& str);
	static Error UnknownOperator(const std::string& str);
	// Name
	static Error NameError(const std::string& var_name);
	// Type
	static Error TypeError(const std::string& str);
	static Error UnsupportedOperator(Token::Type operand, Token::OperatorType op);
	static Error UnsupportedBinaryOperator(Token::Type a, Token::Type b, Token::OperatorType op);
	// Internal
	static Error InternalError(const std::string& str);
	// Runtime
	static Error DivideByZero();

	~Error() throw();

	const char* what() const throw();

private:
	Error(ID id, const std::string& message = "");
	std::string message_;
};

#endif // ERROR_HPP
