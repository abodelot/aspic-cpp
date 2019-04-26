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

void AST::setRoot(const BodyNode* node)
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
    if (root_ != nullptr) {
        root_->repr(0);
    }
}

// AST::BodyNode

AST::BodyNode::BodyNode(const Node* node)
{
    body_.push_back(node);
}

AST::BodyNode::~BodyNode()
{
    for (auto& node: body_) {
        delete node;
    }
}

void AST::BodyNode::append(const Node* node)
{
    body_.push_back(node);
}

Token AST::BodyNode::eval() const
{
    // Return value from the last expression in body (ruby-like)
    size_t loop_length = body_.size() - 1;
    for (size_t i = 0; i < loop_length; ++i) {
        body_[i]->eval();
    }
    return body_.back()->eval();
}

void AST::BodyNode::repr(int depth) const
{
    std::cout << std::string(depth, ' ') << '[' << std::endl;
    for (const auto& node: body_) {
        node->repr(depth + 1);
    }
    std::cout << std::string(depth, ' ') << ']' << std::endl;
}

// AST::IfNode

AST::IfNode::IfNode(const Node* test, const Node* body_true, const Node* body_false):
    test_(test),
    body_true_(body_true),
    body_false_(body_false)
{
}

AST::IfNode::~IfNode()
{
    delete test_;
    delete body_true_;
    if (body_false_ != nullptr) {
        delete body_false_;
    }
}

Token AST::IfNode::eval() const
{
    if (test_->eval().as_bool()) {
        return body_true_->eval();
    }
    else if (body_false_) {
        return body_false_->eval();
    }
    return Token(Token::NULL_VALUE);
}

void AST::IfNode::repr(int depth) const
{
    std::cout << std::string(depth, ' ') << "(if" << std::endl;

    test_->repr(depth + 1);
    body_true_->repr(depth + 1);
    if (body_false_ != nullptr) {
        body_false_->repr(depth + 1);
    }

    std::cout << std::string(depth, ' ') << ")" << std::endl;
}

// AST::LoopNode

AST::LoopNode::LoopNode(const Node* test, const Node* body):
    test_(test),
    body_(body)
{
}

AST::LoopNode::~LoopNode()
{
    delete test_;
    delete body_;
}


Token AST::LoopNode::eval() const
{
    while (test_->eval().as_bool()) {
        body_->eval();
    }
    return Token(Token::NULL_VALUE);
}

void AST::LoopNode::repr(int depth) const
{
    std::cout << std::string(depth, ' ') << "(loop" << std::endl;
    test_->repr(depth + 1);
    body_->repr(depth + 1);
    std::cout << std::string(depth, ' ') << ")" << std::endl;
}

// AST::UnaryOpNode

AST::UnaryOpNode::UnaryOpNode(Token::OperatorType op, const AST::Node* operand):
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
    std::cout << std::string(depth, ' ') << "(" << OperatorManager::to_str(op_) << std::endl;
    operand_->repr(depth + 1);
    std::cout << std::string(depth, ' ') << ")" << std::endl;
}

// AST::BinaryOpNode

AST::BinaryOpNode::BinaryOpNode(Token::OperatorType op, const Node* first, const Node* second):
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
    std::cout << std::string(depth, ' ') << "(" << OperatorManager::to_str(op_) << std::endl;
    first_->repr(depth + 1);
    second_->repr(depth + 1);
    std::cout << std::string(depth, ' ') << ")" << std::endl;
}

// AST::FuncCallNode

AST::FuncCallNode::FuncCallNode(const Node* func):
    func_(func)
{
}

AST::FuncCallNode::~FuncCallNode()
{
    delete func_;
    for (auto& node: arguments_) {
        delete node;
    }
}

Token AST::FuncCallNode::eval() const
{
    Token fn = func_->eval();
    TokenStack stack;
    for (auto& node: arguments_) {
        stack.push(node->eval());
    }
    return fn.get_function()(stack);
}

void AST::FuncCallNode::repr(int depth) const
{
    std::cout << std::string(depth, ' ') << "(func_call" << std::endl;
    func_->repr(depth + 1);
    for (auto& node: arguments_) {
        node->repr(depth + 1);
    }
    std::cout << std::string(depth, ' ') << ")" << std::endl;
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
    std::cout << std::string(depth, ' ') << "(";
    token.debug(std::cout);
    std::cout << ")" << std::endl;
}
