#include "ast/Node.hpp"
#include "OperatorManager.hpp"

namespace ast {

// BodyNode

BodyNode::BodyNode(const Node* node)
{
    body_.push_back(node);
}

BodyNode::~BodyNode()
{
    for (auto& node: body_) {
        delete node;
    }
}

void BodyNode::append(const Node* node)
{
    body_.push_back(node);
}

Token BodyNode::eval() const
{
    // Return value from the last expression in body (ruby-like)
    size_t loop_length = body_.size() - 1;
    for (size_t i = 0; i < loop_length; ++i) {
        body_[i]->eval();
    }
    return body_.back()->eval();
}

void BodyNode::repr(int depth) const
{
    std::cout << std::string(depth, ' ') << '[' << std::endl;
    for (const auto& node: body_) {
        node->repr(depth + 1);
    }
    std::cout << std::string(depth, ' ') << ']' << std::endl;
}

// IfNode

IfNode::IfNode(const Node* test, const Node* if_block):
    test_(test),
    if_block_(if_block),
    else_block_(nullptr)
{
}

IfNode::~IfNode()
{
    delete test_;
    delete if_block_;
    if (else_block_ != nullptr) {
        delete else_block_;
    }
}

Token IfNode::eval() const
{
    if (test_->eval().as_bool()) {
        return if_block_->eval();
    }
    else if (else_block_ != nullptr) {
        return else_block_->eval();
    }
    return Token(Token::NULL_VALUE);
}

void IfNode::repr(int depth) const
{
    std::cout << std::string(depth, ' ') << "(if" << std::endl;

    test_->repr(depth + 1);
    if_block_->repr(depth + 1);
    if (else_block_ != nullptr) {
        else_block_->repr(depth + 1);
    }

    std::cout << std::string(depth, ' ') << ")" << std::endl;
}

void IfNode::set_else_block(const Node* node)
{
    else_block_ = node;
}

// LoopNode

LoopNode::LoopNode(const Node* test, const Node* body):
    test_(test),
    body_(body)
{
}

LoopNode::~LoopNode()
{
    delete test_;
    delete body_;
}


Token LoopNode::eval() const
{
    while (test_->eval().as_bool()) {
        body_->eval();
    }
    return Token(Token::NULL_VALUE);
}

void LoopNode::repr(int depth) const
{
    std::cout << std::string(depth, ' ') << "(loop" << std::endl;
    test_->repr(depth + 1);
    body_->repr(depth + 1);
    std::cout << std::string(depth, ' ') << ")" << std::endl;
}

// UnaryOpNode

UnaryOpNode::UnaryOpNode(Token::OperatorType op, const Node* operand):
    op_(op),
    operand_(operand)
{
}

UnaryOpNode::~UnaryOpNode()
{
    delete operand_;
}

Token UnaryOpNode::eval() const
{
    return operand_->eval().apply_unary_operator(op_);
}

void UnaryOpNode::repr(int depth) const
{
    std::cout << std::string(depth, ' ') << "(" << OperatorManager::to_str(op_) << std::endl;
    operand_->repr(depth + 1);
    std::cout << std::string(depth, ' ') << ")" << std::endl;
}

// BinaryOpNode

BinaryOpNode::BinaryOpNode(Token::OperatorType op, const Node* first, const Node* second):
    op_(op),
    first_(first),
    second_(second)
{
}

BinaryOpNode::~BinaryOpNode()
{
    delete first_;
    delete second_;
}

Token BinaryOpNode::eval() const
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

void BinaryOpNode::repr(int depth) const
{
    std::cout << std::string(depth, ' ') << "(" << OperatorManager::to_str(op_) << std::endl;
    first_->repr(depth + 1);
    second_->repr(depth + 1);
    std::cout << std::string(depth, ' ') << ")" << std::endl;
}

// FuncCallNode

FuncCallNode::FuncCallNode(const Node* func):
    func_(func)
{
}

FuncCallNode::~FuncCallNode()
{
    delete func_;
    for (auto& node: arguments_) {
        delete node;
    }
}

Token FuncCallNode::eval() const
{
    // Fetch function token, then invoke built-in function with arguments vector
    return func_->eval().get_function()(arguments_);
}

void FuncCallNode::repr(int depth) const
{
    std::cout << std::string(depth, ' ') << "(func_call" << std::endl;
    func_->repr(depth + 1);
    for (auto& node: arguments_) {
        node->repr(depth + 1);
    }
    std::cout << std::string(depth, ' ') << ")" << std::endl;
}

void FuncCallNode::push_arg(const Node* node)
{
    arguments_.push_back(node);
}

// ValueNode

ValueNode::ValueNode(const Token& t):
    token(t)
{
}

Token ValueNode::eval() const
{
    return token;
}

void ValueNode::repr(int depth) const
{
    std::cout << std::string(depth, ' ') << "(";
    token.debug(std::cout);
    std::cout << ")" << std::endl;
}

}
