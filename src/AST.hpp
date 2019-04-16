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

    /**
     * Handle one operator, one operand
     */
    class UnaryOpNode: public Node
    {
    public:
        UnaryOpNode(Token::OperatorType op, Node* operand_);

        ~UnaryOpNode();

        Token eval() const override;

        void repr(int depth) const override;

    private:
        Token::OperatorType op_;
        Node* operand_;
    };

    /**
     * Handle one operator and two operands
     */
    class BinaryOpNode: public Node
    {
    public:
        BinaryOpNode(Token::OperatorType op, Node* first, Node* second);

        ~BinaryOpNode();

        Token eval() const override;

        void repr(int depth) const override;

    private:
        Token::OperatorType op_;
        Node* first_;
        Node* second_;
    };

    /**
     * Handle a value, represented by a token
     */
    class ValueNode: public Node
    {
    public:
        ValueNode(const Token& t);

        Token eval() const override;

        void repr(int depth) const override;

    private:
        Token token;
    };

    /**
     * Handle one operator, one left operand, and N right operands (arguments)
     * Currently, this is only used for representing function calls
     */
    class FuncCallNode: public Node
    {
    public:
        FuncCallNode(Token::OperatorType op, Node* operand);

        ~FuncCallNode();

        Token eval() const override;

        void repr(int depth) const override;

        void push_arg(const Node* node);

    private:
        Token::OperatorType op_;
        Node* operand_;
        std::vector<const Node*> arguments_;
    };

    AST();

    ~AST();

    // Define root of the AST
    void build(const Node* node);

    // Deallocate all nodes
    void clear();

    // Evaluate the AST recursively
    Token eval() const;

    // Print a representation of the AST to stdout
    void print() const;

private:
    const AST::Node* root_;
};

#endif
