#ifndef VARIABLE_HPP
#define VARIABLE_HPP

#include "Token.hpp"

/**
 * Holds a dynamic literal value (in a literal token)
 * The variable itself being encapsuled in a variable token
 */
class Variable
{
public:
	Variable();

	void set(const Token& token);

	bool is_null() const;

	Token& value();

private:
	Token token_;
	bool unset_;
};

#endif // VARIABLE_HPP
