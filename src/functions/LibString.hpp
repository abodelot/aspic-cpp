#ifndef ASPIC_LIBSTRING_HPP
#define ASPIC_LIBSTRING_HPP

#include "Token.hpp"

namespace ast {
    class NodeVector;
}

/**
 * String library: string management functions
 */

// Get string length
Token str_len(const ast::NodeVector& args);

// Find occurrences
Token str_count(const ast::NodeVector& args);

// Replace occurrences
Token str_replace(const ast::NodeVector& args);

// Extract substring
Token str_substr(const ast::NodeVector& args);

// Trim whitespaces
Token str_trim(const ast::NodeVector& args);

// Transform to lowercase
Token str_lower(const ast::NodeVector& args);

// Transform to uppercase
Token str_upper(const ast::NodeVector& args);

#endif
