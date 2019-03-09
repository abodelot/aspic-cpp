#ifndef SYMBOLTABLE_HPP
#define SYMBOLTABLE_HPP

#include <string>
#include <map>

#include "FunctionWrapper.hpp"
#include "Token.hpp"

/**
 * Interpreter symbol table
 * Store each declared identifier (variables or functions)
 */
class SymbolTable
{
public:
    /**
     * Get identifier value.
     * Identifier will be initialized and added to the symbol table it doesn't already exist.
     * @param name: variable identifier
     */
    static Token& get(const std::string& name);

    /**
     * Set value for a given identifier
     */
    static void set(const std::string& name, Token& token);

    /**
     * Initialize the symbol table with the function from the standard library
     */
    static void register_stdlib();

    /**
     * Print symbol table content
     */
    static void debug();

private:
    SymbolTable();

   /**
     * Declare a C++ function
     * @param name: function public identifier
     * @param function: wrapper with the C++ function to store
     */
    static void add(const std::string& name, const FunctionWrapper& function);

    typedef std::map<std::string, Token> IdentifierMap;
    static IdentifierMap identifiers_;
};

#endif // SYMBOLTABLE_HPP
