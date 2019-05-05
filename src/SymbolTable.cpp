#include "SymbolTable.hpp"
#include "Error.hpp"
#include "functions/LibCore.hpp"
#include "functions/LibString.hpp"

#include <iostream>
#include <iomanip>
#include <functional>

// Init static attributes
SymbolTable::IdentifierTable SymbolTable::identifiers_;
SymbolTable::NameTable       SymbolTable::names_;


size_t SymbolTable::hash_identifier_name(const std::string& name)
{
    size_t hash = std::hash<std::string>{}(name);
    names_.emplace(hash, name);
    return hash;
}

const std::string& SymbolTable::get_name(size_t hash)
{
    return names_.at(hash);
}

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

Token& SymbolTable::get(size_t hash)
{
    IdentifierTable::iterator it = identifiers_.find(hash);
    if (it == identifiers_.end()) {
        throw Error::NameError(names_.at(hash));
    }
    return it->second;
}

void SymbolTable::set(size_t hash, const Token& token)
{
    if (token.is_value()) {
        identifiers_[hash] = token;
    }
    else {
        throw Error::InternalError("symbol table can only store literals");
    }
}

void SymbolTable::add(const std::string& name, const FunctionWrapper& function)
{
    size_t hash = hash_identifier_name(name);
    set(hash, Token::create_function(function));
}

void SymbolTable::print_all_symbols()
{
    // Get max identifier length
    size_t max_length = 0;
    for (const auto& kv: names_) {
        if (kv.second.size() > max_length) {
            max_length = kv.second.size();
        }
    }

    for (const auto& kv: identifiers_) {
        std::cout << std::setw(max_length) << std::left << names_.at(kv.first) << " | ";
        kv.second.debug(std::cout);
        std::cout << std::endl;
    }
    std::cout << "Symbol table size: " << identifiers_.size() << std::endl;
}
