#include <iostream>

#include "LibCore.hpp"
#include "../Token.hpp"

Token std_print(TokenStack& args)
{
	SymbolTable::check_args(args, 1);
	Token arg = args.top(); args.pop();
	arg.print(std::cout);
	std::cout << std::endl;
	return Token::create_int(0);
}


Token std_input(TokenStack& args)
{
	SymbolTable::check_args(args, 1);
	std::string prompt = args.top().string_value(); args.pop();
	std::cout << prompt;
	std::string input;
	std::cin >> input;
	return Token::create_string(input);
}


Token std_typeof(TokenStack& args)
{
	SymbolTable::check_args(args, 1);
	Token arg = args.top(); args.pop();
	return Token::create_string(Token::to_str(arg.get_type()));
}
