#ifndef LIBSTRING_HPP
#define LIBSTRING_HPP

#include "../TokenStack.hpp"

/**
 * String library: string management functions
 */

// Get string length
Token str_len(TokenStack& args);

// Find occurrences
Token str_count(TokenStack& args);

// Replace occurrences
Token str_replace(TokenStack& args);

// Extract substring
Token str_substr(TokenStack& args);

// Trim whitespaces
Token str_trim(TokenStack& args);

// Transform to lowercase
Token str_lower(TokenStack& args);

// Transform to uppercase
Token str_upper(TokenStack& args);

#endif // LIBSTRING_HPP
