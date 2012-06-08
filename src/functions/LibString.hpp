#ifndef LIBSTRING_HPP
#define LIBSTRING_HPP

#include "../SymbolTable.hpp"

/**
 * String library: string management functions
 */


// string length
Token str_len(TokenStack& args);

// find occurrences
Token str_count(TokenStack& args);

// replace occurrences
Token str_replace(TokenStack& args);

// extract substring
Token str_substr(TokenStack& args);

// trim whitespace
Token str_trim(TokenStack& args);

// convert to lowercase
Token str_lower(TokenStack& args);

// convert to uppercase
Token str_upper(TokenStack& args);

#endif // LIBSTRING_HPP
