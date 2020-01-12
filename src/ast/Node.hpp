#ifndef ASPIC_AST_NODE_HPP
#define ASPIC_AST_NODE_HPP

#include "Operators.hpp"
#include "Object.hpp"
#include "ast/NodeVector.hpp"

namespace ast {

/**
 * Abstract base class
 */
class Node
{
public:
    virtual ~Node() {};
    virtual Object eval() const = 0;
    virtual void repr(int depth) const = 0;
};

/**
 * Sequence of expressions
 */
class BodyNode: public Node
{
public:
    BodyNode(const Node* node);

    ~BodyNode();

    void append(const Node* node);

    // Return last expression result
    Object eval() const override;

    void repr(int depth) const;

private:
    NodeVector body_;
};

/**
 * A node with a "test" expression, a block if "test" is true and an
 * optional block if "test" is false
 */
class IfNode: public Node
{
public:
    // Set body_false to nullptr if no else block
    IfNode(const Node* test, const Node* if_block);
    ~IfNode();

    Object eval() const override;

    void repr(int depth) const;

    void set_else_block(const Node* node);

private:
    const Node* test_;
    const Node* if_block_;
    const Node* else_block_;
};

class LoopNode: public Node
{
public:
    LoopNode(const Node* test, const Node* body);
    ~LoopNode();

    Object eval() const override;

    void repr(int depth) const;


private:
    const Node* test_;
    const Node* body_;
};

/**
 * Handle one operator, one operand
 */
class UnaryOpNode: public Node
{
public:
    UnaryOpNode(Operator op, const Node* operand_);

    ~UnaryOpNode();

    // Return operation result
    Object eval() const override;

    void repr(int depth) const override;

private:
    Operator op_;
    const Node* operand_;
};

/**
 * Handle one operator and two operands
 */
class BinaryOpNode: public Node
{
public:
    BinaryOpNode(Operator op, const Node* first, const Node* second);

    ~BinaryOpNode();

    // Return operation result
    Object eval() const override;

    void repr(int depth) const override;

private:
    Operator op_;
    const Node* first_;
    const Node* second_;
};

/**
 * Handle a value, represented by a token
 */
class ValueNode: public Node
{
public:
    ValueNode(const Object& object);

    // Return value
    Object eval() const override;

    void repr(int depth) const override;

private:
    Object object_;
};

/**
 * Handle one operator, one left operand, and N right operands (arguments)
 * Used for representing function calls
 */
class FuncCallNode: public Node
{
public:
    FuncCallNode(const Node* func);

    ~FuncCallNode();

    // Return function call result
    Object eval() const override;

    void repr(int depth) const override;

    void add_arg(const Node* node);

private:
    const Node* func_;
    NodeVector arguments_;
};

/**
 * Array expression: store a list of values
 * Return an Array object on eval
 */
class ArrayExprNode: public Node
{
public:
    ArrayExprNode();
    ~ArrayExprNode();

    Object eval() const override;

    void repr(int depth) const override;

    void add_value(const Node* node);

private:
    NodeVector values_;
};

/**
 * Hashmap expression: stores a list of key-value pairs
 * Return an Hashmap object on eval
 */
class HashmapExprNode: public Node
{
public:
    HashmapExprNode();
    ~HashmapExprNode();

    Object eval() const override;

    void repr(int depth) const override;

    void add_pair(const Node* key, const Node* value);

private:
    std::vector<std::pair<const Node*, const Node*>> values_;
};

}

#endif
