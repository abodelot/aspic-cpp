#ifndef ASPIC_LIBCORE_HPP
#define ASPIC_LIBCORE_HPP

#include "Object.hpp"

namespace ast {
    class NodeVector;
}

/**
 * Core library: essential and utility functions
 */

Object array_push(const ast::NodeVector& args);
Object array_find(const ast::NodeVector& args);
Object array_count(const ast::NodeVector& args);

Object hash_push(const ast::NodeVector& args);
Object hash_keys(const ast::NodeVector& args);

Object core_len(const ast::NodeVector& args);

Object core_rand(const ast::NodeVector& args);

// raise AssertionError if argument != true
Object core_assert(const ast::NodeVector& args);

// print string representation to std::out
Object core_print(const ast::NodeVector& args);

// ask user input
Object core_input(const ast::NodeVector& args);

// get type as string
Object core_type(const ast::NodeVector& args);

// round to floating number to x decimals
Object core_round(const ast::NodeVector& args);

// a < b ? a : b
Object core_min(const ast::NodeVector& args);

// a > b ? a : b
Object core_max(const ast::NodeVector& args);

#endif
