#ifndef ASPIC_LIBCORE_HPP
#define ASPIC_LIBCORE_HPP

#include "../TokenStack.hpp"

/**
 * Core library: essential and utility functions
 */

// raise AssertionError if argument != true
Token core_assert(TokenStack& args);

// print string representation to std::out
Token core_print(TokenStack& args);

// ask user input
Token core_input(TokenStack& args);

// get type as string
Token core_typeof(TokenStack& args);

// round to floating number to x decimals
Token core_round(TokenStack& args);

// a < b ? a : b
Token core_min(TokenStack& args);

// a > b ? a : b
Token core_max(TokenStack& args);

#endif
