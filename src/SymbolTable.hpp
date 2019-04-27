#ifndef ASPIC_SYMBOLTABLE_HPP
#define ASPIC_SYMBOLTABLE_HPP

#include <string>
#include <unordered_map>

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
     * Check if a given identifier exists in the symbol table
     * @return true if there is such an identifier, otherwise false
     */
    static bool contains(const std::string& name);

    /**
     * Get value of given identifier. Raise NameError exception if identifier is not declared.
     * @param name: identifier name
     */
    static Token& get(const std::string& name);

    /**
     * Set value for a given identifier
     */
    static void set(const std::string& name, const Token& token);

    /**
     * Initialize the symbol table with the functions from the standard library
     */
    static void register_stdlib();

    /**
     * Print symbol table content to stdout (for debugging purpose)
     */
    static void print_all_symbols();

private:
    SymbolTable();

   /**
     * Declare a C++ function
     * @param name: function public identifier
     * @param function: wrapper with the C++ function to store
     */
    static void add(const std::string& name, const FunctionWrapper& function);

    typedef std::unordered_map<std::string, Token> IdentifierMap;
    static IdentifierMap identifiers_;
};

#endif
