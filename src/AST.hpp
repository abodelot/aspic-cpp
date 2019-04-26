#ifndef ASPIC_AST_HPP
#define ASPIC_AST_HPP

#include "Token.hpp"
#include <vector>

/**
 * Abstract syntax tree
 */
class AST
{
public:
    /**
     * Abstract base class
     */
    class Node
    {
    public:
        virtual ~Node() {};
        virtual Token eval() const = 0;
        virtual void repr(int depth) const = 0;
    };

    typedef std::vector<const Node*> NodeVector;

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
        Token eval() const override;

        void repr(int depth) const;

    private:
        NodeVector body_;
    };

    /**
     * A node with a test expression, a body if test is true and an
     * optional body if test is false
     */
    class IfNode: public Node
    {
    public:
        // Set body_false to nullptr if no else block
        IfNode(const Node* test, const Node* body_true, const Node* body_false);
        ~IfNode();

        Token eval() const override;

        void repr(int depth) const;

    private:
        const Node* test_;
        const Node* body_true_;
        const Node* body_false_;
    };

    class LoopNode: public Node
    {
    public:
        LoopNode(const Node* test, const Node* body);
        ~LoopNode();

        Token eval() const override;

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
        UnaryOpNode(Token::OperatorType op, const Node* operand_);

        ~UnaryOpNode();

        // Return operation result
        Token eval() const override;

        void repr(int depth) const override;

    private:
        Token::OperatorType op_;
        const Node* operand_;
    };

    /**
     * Handle one operator and two operands
     */
    class BinaryOpNode: public Node
    {
    public:
        BinaryOpNode(Token::OperatorType op, const Node* first, const Node* second);

        ~BinaryOpNode();

        // Return operation result
        Token eval() const override;

        void repr(int depth) const override;

    private:
        Token::OperatorType op_;
        const Node* first_;
        const Node* second_;
    };

    /**
     * Handle a value, represented by a token
     */
    class ValueNode: public Node
    {
    public:
        ValueNode(const Token& t);

        // Return value
        Token eval() const override;

        void repr(int depth) const override;

    private:
        Token token;
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
        Token eval() const override;

        void repr(int depth) const override;

        void push_arg(const Node* node);

    private:
        const Node* func_;
        NodeVector arguments_;
    };

    AST();

    ~AST();

    // Add a node at the top-level of the AST
    void setRoot(const BodyNode* node);

    // Deallocate all nodes
    void clear();

    // Evaluate the AST recursively
    Token eval() const;

    // Print a representation of the AST to stdout
    void print() const;

private:
    const AST::BodyNode* root_;
};

#endif
