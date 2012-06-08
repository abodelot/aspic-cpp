#include <iostream>
#include <cassert>

#include "Variable.hpp"
#include "Error.hpp"

Variable::Variable() :
	token_(Token::create_int(0))
{
	unset_ = true;
}


void Variable::set(const Token& token)
{
	if (!token.is_literal())
	{
		throw Error::SyntaxError("invalid right-value");
	}
	token_ = token;
	unset_ = false;
}


bool Variable::is_null() const
{
	return unset_;
}


Token& Variable::value()
{
	if (unset_)
	{
		throw Error::NameError("referencing a variable before assignment");
	}
	return token_;
}

