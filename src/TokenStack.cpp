#include <sstream>

#include "TokenStack.hpp"
#include "Error.hpp"


std::string TokenStack::pop_string()
{
	std::string value = back().as_string();
	pop_back();
	return value;
}


int TokenStack::pop_int()
{
	int value = back().as_int();
	pop_back();
	return value;
}


double TokenStack::pop_float()
{
	double value = back().as_float();
	pop_back();
	return value;
}


bool TokenStack::pop_bool()
{
	bool value = back().as_bool();
	pop_back();
	return value;
}


void TokenStack::check(size_t count) const
{
	if (size() < count)
	{
		// "function takes [exactaly N|no] argument[s] (Z given)"
		std::ostringstream oss;
		oss << "function takes ";
		if (count == 0)
			oss << "no";
		else
			oss << "exactly " << count;

		oss << " argument";
		if (count != 1)
			oss << "s";

		oss << " (" << size() << " given)";
		throw Error::TypeError(oss.str());
	}
}
