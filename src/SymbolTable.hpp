#ifndef ASPIC_SYMBOLTABLE_HPP
#define ASPIC_SYMBOLTABLE_HPP

#include "FunctionWrapper.hpp"
#include "Object.hpp"

#include <string>
#include <unordered_map>
#include <list>

class BaseObject;

/**
 * The symbol table stores all declared identifiers, such as variables and
 * built-in functions.
 *
 * Variable names must be hashed first (with hash_identifier_name function),
 * then the returned hash can be used as the variable ID to read and update
 * the associated Object value (see get function).
 *
 * Built-in functions are automatically loaded in the symbol table when the
 * interpreter is started (see register_stdlib)
 *
 * The symbol table also stores shared objects which are passed by reference,
 * such as ArrayObject. A shared object is defined by:
 *   1. at least one identifier entry (identifiers_)
 *   2. a pointer to the allocated object (shared_objects_)
 */
class SymbolTable
{
public:
    /**
     * Declare an identifier name
     * @return hash ID for the given name
     */
    static size_t hash_identifier_name(const std::string& name);

    /**
     * Find an identifier name from its hash. Name must be have been registered first.
     */
    static const std::string& get_name(size_t hash);

    /**
     * Get value of given identifier.
     * A NameError exception is raised is no value has been set first.
     * @param id_hash: hash of the identifier name
     * @return associated value
     */
    static Object& get(size_t id_hash);

    /**
     * Associate the given object to the given identifier hash
     */
    static void set(size_t hash, const Object& object);

    /**
     * Load the built-in functions from the standard library into the  symbol table
     */
    static void register_stdlib();

    /**
     * Print symbol table content to stdout (for debugging purpose)
     */
    static void inspect_symbols();

    /**
     * Print allocated object list to stdout (for debugging purpose)
     */
    static void inspect_memory();

    /**
     * Add object to the list of allocated objects
     */
    static void track_object(BaseObject* ptr);

    /**
     * Mark and sweep garbage collection: visit all objects and delete the
     * unreachable ones
     */
    static void mark_and_sweep();

    static void destroy();

private:
    SymbolTable() = delete;

   /**
     * Register a built-in C++ function
     * @param name: function public identifier name
     * @param function: wrapper with the C++ function to store
     */
    static void add(const std::string& name, const FunctionWrapper& function);

    typedef std::unordered_map<size_t, Object> IdentifierTable;
    static IdentifierTable identifiers_;

    typedef std::unordered_map<size_t, std::string> NameTable;
    static NameTable names_;

    typedef std::list<BaseObject*> ObjectList;
    static ObjectList shared_objects_;
};

#endif
