#include <iostream>
#include <iomanip>

#include "SymbolTable.hpp"
#include "Error.hpp"
#include "functions/LibCore.hpp"
#include "functions/LibString.hpp"

// Init static attribute (singleton)
SymbolTable::IdentifierMap SymbolTable::identifiers_;

void SymbolTable::register_stdlib()
{
    // Load core library (no prefix)
    add("assert", core_assert);
    add("print", core_print);
    add("input", core_input);
    add("type", core_type);
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

bool SymbolTable::contains(const std::string& name)
{
    return identifiers_.find(name) != identifiers_.end();
}

Token& SymbolTable::get(const std::string& name)
{
    IdentifierMap::iterator it(identifiers_.find(name));
    if (it != identifiers_.end()) {
        return it->second;
    }
    throw Error::NameError(name);
}


void SymbolTable::set(const std::string& name, Token& token)
{
    if (token.is_literal()) {
        identifiers_[name] = token;
    }
    else {
        throw Error::InternalError("symbol table can only store literals");
    }
}

void SymbolTable::add(const std::string& name, const FunctionWrapper& function)
{
    identifiers_[name] = Token::create_function(function);
}

void SymbolTable::print_all_symbols()
{
    // Get max identifier length
    size_t max_length = 0;
    for (const auto& kv: identifiers_) {
        if (kv.first.size() > max_length) {
            max_length = kv.first.size();
        }
    }

    for (const auto& kv: identifiers_) {
        std::cout << std::setw(max_length) << std::left << kv.first << " | ";
        kv.second.debug(std::cout);
        std::cout << std::endl;
    }
    std::cout << "Symbol table size: " << identifiers_.size() << std::endl;
}
