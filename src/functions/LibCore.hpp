#ifndef LIBCORE_HPP
#define LIBCORE_HPP

#include "../SymbolTable.hpp"

Token std_print(TokenStack& args);
Token std_input(TokenStack& args);

Token std_typeof(TokenStack& args);


#endif // LIBCORE_HPP
