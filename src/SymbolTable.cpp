#include <iostream>

#include "SymbolTable.hpp"
#include "Error.hpp"
#include "functions/LibCore.hpp"
#include "functions/LibString.hpp"

// Init static attribute (singleton)
SymbolTable::IdentifierMap SymbolTable::identifiers_;

void SymbolTable::register_stdlib()
{
    // Load core library (no prefix)
    add("print", core_print);
    add("input", core_input);
    add("typeof", core_typeof);
    add("round", core_round);
    add("min", core_min);
    add("max", core_max);

    // Load string library
    add("str_len", str_len);
    add("str_count", str_count);
    add("str_replace", str_replace);
    add("str_substr", str_substr);
    add("str_trim", str_trim);
    add("str_lower", str_lower);
    add("str_upper", str_upper);
}

Token& SymbolTable::get(const std::string& name)
{
    return identifiers_[name];
}

void SymbolTable::set(const std::string& name, Token& token)
{
    if (token.is_literal())
    {
        identifiers_[name] = token;
    }
    else
    {
        throw Error::InternalError("symbol table can only store literals");
    }
}

void SymbolTable::add(const std::string& name, const FunctionWrapper& function)
{
    identifiers_[name] = Token::create_function(function);
}

void SymbolTable::print_all_symbols()
{
    for (IdentifierMap::iterator it = identifiers_.begin(); it != identifiers_.end(); ++it)
    {
        std::cout << it->first.c_str() << ": ";
        try
        {
            it->second.print_value(std::cout);
        }
        catch (Error& error) // TODO: NameError
        {
            std::cout << "[not initialized yet]";
        }
        std::cout << "\n";
    }
    std::cout << "****** total: " << identifiers_.size() << " ******" << std::endl;
}
