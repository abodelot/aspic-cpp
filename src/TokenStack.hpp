#ifndef TOKENSTACK_HPP
#define TOKENSTACK_HPP

#include <vector>
#include "Token.hpp"

/**
 * A stack with helper methods for tokens
 */
class TokenStack: public std::vector<Token>
{
public:
    inline void push(const Token& token) { push_back(token); }
    inline void pop() { pop_back(); }
    inline Token& top(){ return back(); }

    /**
     * Pop the last token and return its value casted as a literal
     */
    std::string pop_string();
    int         pop_int();
    double      pop_float();
    bool        pop_bool();

    /**
     * Raise an exception if arguments count is insufficient
     * @param count: expected arguments count
     */
    void check(size_t count) const;

private:
};

#endif // TOKENSTACK_HPP
