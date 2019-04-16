#include "AST.hpp"
#include "OperatorManager.hpp"
#include "TokenStack.hpp"

// AST

AST::AST():
    root_(nullptr)
{
}

AST::~AST()
{
    clear();
}

void AST::build(const Node* node)
{
   clear();
   root_ = node;
}

void AST::clear()
{
    if (root_ != nullptr) {
        delete root_;
    }
    root_ = nullptr;
}

Token AST::eval() const
{
    return root_->eval();
}

void AST::print() const
{
    root_->repr(0);
}

// AST::UnaryOpNode

AST::UnaryOpNode::UnaryOpNode(Token::OperatorType op, AST::Node* operand):
    op_(op),
    operand_(operand)
{
}

AST::UnaryOpNode::~UnaryOpNode()
{
    delete operand_;
}

Token AST::UnaryOpNode::eval() const
{
    return operand_->eval().apply_unary_operator(op_);
}

void AST::UnaryOpNode::repr(int depth) const
{
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }
    std::cout << "(" << OperatorManager::to_str(op_) << std::endl;

    operand_->repr(depth + 1);

    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }

    std::cout << ")" << std::endl;
}

// AST::BinaryOpNode

AST::BinaryOpNode::BinaryOpNode(Token::OperatorType op, Node* first, Node* second):
    op_(op),
    first_(first),
    second_(second)
{
}

AST::BinaryOpNode::~BinaryOpNode()
{
    delete first_;
    delete second_;
}

Token AST::BinaryOpNode::eval() const
{
    // ==, !=, ||, &&: operator implementation is not type-dependant
    // eval() is called as late as possible to implement lazy evaluation
    switch (op_) {
        case Token::OP_EQUAL:
            return Token::create_bool(first_->eval().get_value().equal(second_->eval().get_value()));
        case Token::OP_NOT_EQUAL:
            return Token::create_bool(!first_->eval().get_value().equal(second_->eval().get_value()));
        case Token::OP_LOGICAL_AND:
        {
            Token left = first_->eval();
            if (left.as_bool()) {
                return second_->eval();
            }
            return left;
        }
        case Token::OP_LOGICAL_OR:
        {
            Token left = first_->eval();
            if (left.as_bool()) {
                return left;
            }
            return second_->eval();
        }
        default: break;
    }
    return first_->eval().apply_binary_operator(op_, second_->eval());
}

void AST::BinaryOpNode::repr(int depth) const
{
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }
    std::cout << "(" << OperatorManager::to_str(op_) << std::endl;

    first_->repr(depth + 1);
    second_->repr(depth + 1);

    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }
    std::cout << ")" << std::endl;
}

// AST::FuncCallNode

AST::FuncCallNode::FuncCallNode(Token::OperatorType op, Node* operand):
    op_(op),
    operand_(operand)
{
}

AST::FuncCallNode::~FuncCallNode()
{
    delete operand_;
    for (auto& node: arguments_) {
        delete node;
    }
}

Token AST::FuncCallNode::eval() const
{
    Token fn = operand_->eval();
    TokenStack stack;
    for (auto& node: arguments_) {
        stack.push(node->eval());
    }
    return fn.get_function()(stack);
}

void AST::FuncCallNode::repr(int depth) const
{
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }
    std::cout << "(" << OperatorManager::to_str(op_) << std::endl;
    operand_->repr(depth + 1);

    for (auto& node: arguments_) {
        node->repr(depth + 1);
    }

    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }
    std::cout << ")" << std::endl;
}

void AST::FuncCallNode::push_arg(const Node* node)
{
    arguments_.push_back(node);
}

// AST::ValueNode

AST::ValueNode::ValueNode(const Token& t):
    token(t)
{
}

Token AST::ValueNode::eval() const
{
    return token;
}

void AST::ValueNode::repr(int depth) const
{
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }
    std::cout << "(";
    token.debug(std::cout);
    std::cout <<  ")" << std::endl;
}
