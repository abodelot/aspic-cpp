#include "ArrayObject.hpp"
#include "Object.hpp"


ArrayObject::ArrayObject(size_t size):
    BaseObject()
{
    if (size > 0) {
        values_.reserve(size);
    }
}

ArrayObject::~ArrayObject()
{
}

const char* ArrayObject::class_name() const
{
    return "array";
}

void ArrayObject::gc_visit()
{
    for (auto& value: values_) {
        value.gc_visit();
    }
}

ArrayObject* ArrayObject::concat(const ArrayObject& a, const ArrayObject& b)
{
    ArrayObject* array = new ArrayObject(a.size() + b.size());
    array->values_.insert(array->values_.begin(), a.values_.begin(), a.values_.end());
    array->values_.insert(array->values_.end(), b.values_.begin(), b.values_.end());
    return array;
}

size_t ArrayObject::size() const
{
    return values_.size();
}

Object& ArrayObject::at(int index)
{
    return values_.at(index);
}

void ArrayObject::push(const Object& object)
{
    // get_value() ensures an identifier reference isn't pushed to the array
    values_.push_back(object.get_value());
}

int ArrayObject::find(const Object& object) const
{
    for (size_t i = 0; i < values_.size(); ++i) {
        if (values_[i].equal(object)) {
            return i;
        }
    }
    return -1;
}

int ArrayObject::count(const Object& object) const
{
    int count = 0;
    for (auto& value: values_) {
        if (value.equal(object)) {
            ++count;
        }
    }
    return count;
}

bool ArrayObject::eq(const ArrayObject& array) const
{
    if (this == &array) {
        return true;
    }
    if (size() != array.size()) {
        return false;
    }
    return std::equal(values_.begin(), values_.end(), array.values_.begin(),
        [](const Object& left, const Object& right) {
            return left.equal(right);
        });
}
