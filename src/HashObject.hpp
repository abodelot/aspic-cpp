#ifndef ASPIC_HASH_OBJECT_HPP
#define ASPIC_HASH_OBJECT_HPP

#include "BaseObject.hpp"
#include "Object.hpp"

#include <unordered_map>

class ArrayObject;

/**
 * A hashmap of 'Object', for both keys and values
 * Implemented as a wrapper around std::unordered_map
 */
class HashObject: public BaseObject
{
public:
    typedef std::unordered_map<Object, Object> InternalHash;

    HashObject();
    ~HashObject();

    const char* class_name() const override;

    /**
     * Get number of key-value pairs
     */
    size_t size() const;

    /**
     * Compare two hash objects
     */
    bool eq(const HashObject& hash) const;

    /**
     * Add a key value pair
     */
    void push(const Object& key, const Object& value);

    /**
     * Get value at given index
     */
    Object& at(const Object& key);

    /**
     * Get list of keys
     */
    ArrayObject* get_keys() const;

    InternalHash::const_iterator begin() {
        return values_.begin();
    }

    InternalHash::const_iterator end() {
        return values_.end();
    }

private:
    HashObject(const HashObject&) = delete;
    HashObject& operator=(const HashObject&) = delete;

    void gc_visit();

    InternalHash values_;
};

#endif
