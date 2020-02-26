#include "ast/Node.hpp"
#include "Operators.hpp"
#include "SymbolTable.hpp"
#include "ArrayObject.hpp"
#include "HashObject.hpp"

#define SPACES(X) std::string((X) * 4, ' ')

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

Object BodyNode::eval() const
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
    std::cout << SPACES(depth) << "[[" << std::endl;
    for (const auto& node: body_) {
        node->repr(depth + 1);
    }
    std::cout << SPACES(depth) << "]]" << std::endl;
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

Object IfNode::eval() const
{
    if (test_->eval().truthy()) {
        return if_block_->eval();
    }
    else if (else_block_ != nullptr) {
        return else_block_->eval();
    }
    return Object::create_null();
}

void IfNode::repr(int depth) const
{
    std::cout << SPACES(depth) << "(if" << std::endl;

    test_->repr(depth + 1);
    if_block_->repr(depth + 1);
    if (else_block_ != nullptr) {
        else_block_->repr(depth + 1);
    }

    std::cout << SPACES(depth) << ")" << std::endl;
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

Object LoopNode::eval() const
{
    while (test_->eval().truthy()) {
        body_->eval();
    }
    return Object::create_null();
}

void LoopNode::repr(int depth) const
{
    std::cout << SPACES(depth) << "(loop" << std::endl;
    test_->repr(depth + 1);
    body_->repr(depth + 1);
    std::cout << SPACES(depth) << ")" << std::endl;
}

// UnaryOpNode

UnaryOpNode::UnaryOpNode(Operator op, const Node* operand):
    op_(op),
    operand_(operand)
{
}

UnaryOpNode::~UnaryOpNode()
{
    delete operand_;
}

Object UnaryOpNode::eval() const
{
    return operand_->eval().apply_unary_operator(op_);
}

void UnaryOpNode::repr(int depth) const
{
    std::cout << SPACES(depth) << "(unary_op " << Operators::to_str(op_) << std::endl;
    operand_->repr(depth + 1);
    std::cout << SPACES(depth) << ")" << std::endl;
}

// BinaryOpNode

BinaryOpNode::BinaryOpNode(Operator op, const Node* first, const Node* second):
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

Object BinaryOpNode::eval() const
{
    // ==, !=, ||, &&: operator implementation is not type-dependant
    // eval() is called as late as possible to implement lazy evaluation
    switch (op_) {
        case Operator::OP_EQUAL:
            return Object::create_bool(first_->eval().get_value().equal(second_->eval().get_value()));
        case Operator::OP_NOT_EQUAL:
            return Object::create_bool(!first_->eval().get_value().equal(second_->eval().get_value()));
        case Operator::OP_LOGICAL_AND:
        {
            const Object& left = first_->eval();
            if (left.truthy()) {
                return second_->eval();
            }
            return left;
        }
        case Operator::OP_LOGICAL_OR:
        {
            const Object& left = first_->eval();
            if (left.truthy()) {
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
    std::cout << SPACES(depth) << "(binary_op " << Operators::to_str(op_) << std::endl;
    first_->repr(depth + 1);
    second_->repr(depth + 1);
    std::cout << SPACES(depth) << ")" << std::endl;
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

Object FuncCallNode::eval() const
{
    // Fetch function object, then invoke built-in function with arguments vector
    return func_->eval().get_function()(arguments_);
}

void FuncCallNode::repr(int depth) const
{
    std::cout << SPACES(depth) << "(func_call" << std::endl;
    func_->repr(depth + 1);
    for (auto& node: arguments_) {
        node->repr(depth + 1);
    }
    std::cout << SPACES(depth) << ")" << std::endl;
}

void FuncCallNode::add_arg(const Node* node)
{
    arguments_.push_back(node);
}

// ValueNode

ValueNode::ValueNode(const Object& object):
    object_(object)
{
}

Object ValueNode::eval() const
{
    return object_;
}

void ValueNode::repr(int depth) const
{
    std::cout << SPACES(depth) << object_ << std::endl;
}

// ArrayExprNode

ArrayExprNode::ArrayExprNode()
{
}

ArrayExprNode::~ArrayExprNode()
{
    for (auto& node: values_) {
        delete node;
    }
}

Object ArrayExprNode::eval() const
{
    ArrayObject* array = new ArrayObject(values_.size());
    for (auto& node: values_) {
        array->push(node->eval().get_value());
    }
    return Object::create_array(array);
}

void ArrayExprNode::repr(int depth) const
{
    std::cout << SPACES(depth) << "(array_expr" << std::endl;
    for (size_t i = 0; i < values_.size(); ++i) {
        std::cout << SPACES(depth + 1) << "[" << i << "] ->" << std::endl;
        values_[i]->repr(depth + 2);
    }
    std::cout << SPACES(depth) << ')' << std::endl;
}

void ArrayExprNode::add_value(const Node* node)
{
    values_.push_back(node);
}

// HashmapExprNode

HashmapExprNode::HashmapExprNode()
{
}

HashmapExprNode::~HashmapExprNode()
{
    for (auto& node_pair: values_) {
        delete node_pair.first;
        delete node_pair.second;
    }
}

Object HashmapExprNode::eval() const
{
    HashObject* hash = new HashObject();
    for (auto& kv: values_) {
        hash->push(kv.first->eval().get_value(), kv.second->eval().get_value());
    }
    return Object::create_hash(hash);
}

void HashmapExprNode::repr(int depth) const
{
    std::cout << SPACES(depth) << "(hash_expr" << std::endl;
    for (size_t i = 0; i < values_.size(); ++i) {
        values_[i].first->repr(depth + 1);
        values_[i].second->repr(depth + 2);
    }
    std::cout << SPACES(depth) << ')' << std::endl;
}

void HashmapExprNode::add_pair(const Node* key, const Node* value)
{
    values_.push_back(std::make_pair(key, value));
}

}
