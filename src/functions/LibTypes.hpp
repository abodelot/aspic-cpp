#ifndef ASPIC_LIBTYPES_HPP
#define ASPIC_LIBTYPES_HPP

#include "Object.hpp"

namespace ast {
    class NodeVector;
}

/**
 * Convert to int
 * @param 1: value to convert
 * @param 2: base (default is 10)
 */
Object core_int(const ast::NodeVector& args);

/**
 * Convert to string
 * @param 1: value to convert
 */
Object core_str(const ast::NodeVector& args);

#endif
