#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include <stack>
#include "Token.hpp"

typedef std::stack<Token> TokenStack;

/**
 * Define the Function type
 * The token stack holds all function arguments
 */
struct Function
{
	typedef Token (*Ptr)(TokenStack&);
	Function(Ptr p=NULL): ptr(p){}
	Ptr ptr;
};

/**
 * Holds a dynamic literal value
 * The token holds the literal value assigned to the variable
 */
class Variable
{
public:
	Variable();

	/**
	 * Initialize
	 */
	void set(const Token& token);

	Token& get();

private:
	Token literal_;
	bool  unset_;
};

#endif // VARIABLE_HPP
