#ifndef ASPIC_LIBSTRING_HPP
#define ASPIC_LIBSTRING_HPP

#include "Object.hpp"

namespace ast {
    class NodeVector;
}

/**
 * String library: string management functions
 */

// Get string length
Object str_len(const ast::NodeVector& args);

// Find occurrences
Object str_count(const ast::NodeVector& args);

// Replace occurrences
Object str_replace(const ast::NodeVector& args);

// Extract substring
Object str_substr(const ast::NodeVector& args);

// Trim whitespaces
Object str_trim(const ast::NodeVector& args);

// Transform to lowercase
Object str_lower(const ast::NodeVector& args);

// Transform to uppercase
Object str_upper(const ast::NodeVector& args);

#endif
