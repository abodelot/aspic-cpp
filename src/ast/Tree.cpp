#include "ast/Tree.hpp"
#include "ast/Node.hpp"

namespace ast {

Tree::Tree():
    root_(nullptr)
{
}

Tree::~Tree()
{
    clear();
}

void Tree::setRoot(const BodyNode* node)
{
    clear();
    root_ = node;
}

void Tree::clear()
{
    if (root_ != nullptr) {
        delete root_;
    }
    root_ = nullptr;
}

Token Tree::eval() const
{
    return root_->eval();
}

void Tree::print() const
{
    if (root_ != nullptr) {
        root_->repr(0);
    }
}

}
