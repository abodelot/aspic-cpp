#ifndef ASPIC_LIBCORE_HPP
#define ASPIC_LIBCORE_HPP

#include "Token.hpp"

namespace ast {
    class NodeVector;
}

/**
 * Core library: essential and utility functions
 */

// raise AssertionError if argument != true
Token core_assert(const ast::NodeVector& args);

// print string representation to std::out
Token core_print(const ast::NodeVector& args);

// ask user input
Token core_input(const ast::NodeVector& args);

// get type as string
Token core_type(const ast::NodeVector& args);

// round to floating number to x decimals
Token core_round(const ast::NodeVector& args);

// a < b ? a : b
Token core_min(const ast::NodeVector& args);

// a > b ? a : b
Token core_max(const ast::NodeVector& args);

#endif
