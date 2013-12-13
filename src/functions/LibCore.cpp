#include <iostream>
#include <cmath>

#include "LibCore.hpp"
#include "../Token.hpp"


Token std_print(TokenStack& args)
{
	SymbolTable::check_args(args, 1);
	Token arg = args.top(); args.pop();
	arg.print_value(std::cout);
	std::cout << std::endl;
	return Token::create_int(0); // TODO: return Token::NONE
}


Token std_input(TokenStack& args)
{
	SymbolTable::check_args(args, 1);
	std::string prompt = args.top().as_string(); args.pop();
	std::cout << prompt;
	std::string input;
	std::cin >> input; // TODO: do a proper getline and purge the buffer, this will break on whitespaces
	return Token::create_string(input);
}


Token std_typeof(TokenStack& args)
{
	SymbolTable::check_args(args, 1);
	Token arg = args.top(); args.pop();
	return Token::create_string(Token::type_to_str(arg.get_type()));
}


Token std_round(TokenStack& args)
{
	SymbolTable::check_args(args, 2);
	int x = args.top().as_int(); args.pop();
	double num = args.top().as_float(); args.pop();

	return Token::create_float(ceil( ( num * pow( 10, x ) ) - 0.49 ) / pow( 10, x ));
}

