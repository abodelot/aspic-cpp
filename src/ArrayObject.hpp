#ifndef ASPIC_ARRAY_OBJECT_HPP
#define ASPIC_ARRAY_OBJECT_HPP

#include "BaseObject.hpp"

#include <vector>

class Object;

/**
 * A vector of Object. Implemented as a wrapper around std::vector
 */
class ArrayObject: public BaseObject
{
public:
    ArrayObject(size_t size = 0);
    ~ArrayObject();

    const char* class_name() const override;

    /**
     * Merge two arrays into a new one
     */
    static ArrayObject* concat(const ArrayObject& a, const ArrayObject& b);

    /**
     * Get array length
     */
    size_t size() const;

    /**
     * Compare two arrays
     */
    bool eq(const ArrayObject& array) const;

    /**
     * Append value to end of array
     */
    void push(const Object& object);

    /**
     * Return first index of given value, -1 otherwise
     */
    int find(const Object& object) const;

    /**
     * Return number of occurrences of fiven value
     */
    int count(const Object& object) const;

    /**
     * Get value at given index
     */
    Object& at(int index);

private:
    ArrayObject(const ArrayObject&) = delete;
    ArrayObject& operator=(const ArrayObject&) = delete;

    void gc_visit() override;

    std::vector<Object> values_;
};

#endif
