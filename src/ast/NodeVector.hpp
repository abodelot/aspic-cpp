#ifndef ASPIC_NODEVECTOR_HPP
#define ASPIC_NODEVECTOR_HPP

#include <vector>
#include "Token.hpp"

namespace ast
{

class Node;

/**
 * A stack with helper methods for tokens
 */
class NodeVector: public std::vector<const Node*>
{
public:
    /**
     * Raise an exception if arguments count is insufficient
     * @param count: expected arguments count
     */
    void check(size_t count) const;
};

}

#endif
