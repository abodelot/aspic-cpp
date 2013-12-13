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
	static Error SyntaxError(const std::string& str);
	static Error NameError(const std::string& str);
	static Error TypeError(const std::string& str);
	static Error InternalError(const std::string& str);
	static Error DivideByZero();
	static Error MissingLeftBracket();
	static Error MissingRightBracket();
	static Error UnknownOperator(const std::string& str);
	static Error UnsupportedOperator(Token::Type operand, Token::OperatorType op);
	static Error UnsupportedBinaryOperator(Token::Type a, Token::Type b, Token::OperatorType op);

	~Error() throw();

	const char* what() const throw();

private:
	Error(const std::string& message = "");
	std::string message_;
};

#endif // ERROR_HPP
