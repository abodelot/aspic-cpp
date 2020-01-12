#include "HashObject.hpp"
#include "ArrayObject.hpp"
#include "Error.hpp"


HashObject::HashObject():
    BaseObject()
{
}

HashObject::~HashObject()
{
}

const char* HashObject::class_name() const
{
    return "hashmap";
}

size_t HashObject::size() const
{
    return values_.size();
}

bool HashObject::eq(const HashObject& hash) const
{
    if (this == &hash) {
        return true;
    }
    if (size() != hash.size()) {
        return false;
    }
    return this->values_ == hash.values_;
}

void HashObject::push(const Object& key, const Object& value)
{
    values_[key] = value;
}

Object& HashObject::at(const Object& key)
{
    try {
        return values_.at(key);
    }
    catch (const std::out_of_range& e) {
        throw Error::KeyError(key.to_string());
    }
}

ArrayObject* HashObject::get_keys() const
{
    ArrayObject* array = new ArrayObject(values_.size());
    for (InternalHash::const_iterator it = values_.begin(); it != values_.end(); ++it) {
        array->push(it->first);
    }
    return array;
}

void HashObject::gc_visit()
{
    for (InternalHash::const_iterator it = values_.begin(); it != values_.end(); ++it) {
        it->first.gc_visit();
        it->second.gc_visit();
    }
}
