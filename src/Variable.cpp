#include "Variable.hpp"
#include "Error.hpp"


Variable::Variable():
	literal_(Token::create_int(0)),
	unset_(true)
{
}


void Variable::set(const Token& literal)
{
	if (!literal.is_literal())
	{
		throw Error::InternalError("invalid right-value for variable");
	}
	unset_ = false;
	literal_ = literal;
}


Token& Variable::get()
{
	if (unset_)
	{
		throw Error::NameError("referencing a variable before assignment");
	}
	return literal_;
}
