#ifndef ASPIC_AST_TREE_HPP
#define ASPIC_AST_TREE_HPP

#include "Object.hpp"

namespace ast {

class BodyNode;

/**
 * Abstract syntax tree
 */
class Tree
{
public:

    Tree();

    ~Tree();

    // Add a node at the top-level of the AST
    void setRoot(const BodyNode* node);

    // Deallocate all nodes
    void clear();

    // Evaluate the AST recursively
    Object eval() const;

    // Print a representation of the AST to stdout
    void print() const;

private:
    const BodyNode* root_;
};

}

#endif
