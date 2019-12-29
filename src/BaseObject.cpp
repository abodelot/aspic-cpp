#include "BaseObject.hpp"
#include "SymbolTable.hpp"


BaseObject::BaseObject():
    marked_(0)
{
    SymbolTable::track_object(this);
}

BaseObject::~BaseObject()
{
}

bool BaseObject::is_marked() const
{
    return marked_;
}

void BaseObject::clear_mark()
{
    marked_ = false;
}

void BaseObject::mark()
{
    if (!marked_) {
        marked_ = true;
        gc_visit();
    }
}
