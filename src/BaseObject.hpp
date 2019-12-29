#ifndef ASPIC_BASE_OBJECT_HPP
#define ASPIC_BASE_OBJECT_HPP

/**
 * Base class for objects which are handled using reference
 */
class BaseObject
{
public:
    virtual ~BaseObject();

    virtual const char* class_name() const = 0;

    /**
     * Mark object as still active
     */
    void mark();

    /**
     * Reset the active mark as 'false'
     */
    void clear_mark();

    /**
     * Get the object marked status
     */
    bool is_marked() const;

protected:
    BaseObject();

    /**
     * Callback when object has been marked as still active by GC
     */
    virtual void gc_visit() = 0;

    bool marked_;
};

#endif
