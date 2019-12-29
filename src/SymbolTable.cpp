#include "SymbolTable.hpp"
#include "Error.hpp"
#include "BaseObject.hpp"
#include "functions/LibCore.hpp"
#include "functions/LibString.hpp"

#include <iostream>
#include <iomanip>
#include <functional>

// Init static attributes
SymbolTable::IdentifierTable SymbolTable::identifiers_;
SymbolTable::NameTable       SymbolTable::names_;
SymbolTable::ObjectList      SymbolTable::shared_objects_;


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
    add("count", array_count);
    add("find", array_find);
    add("push", array_push);
    add("len", core_len);

    // Load string library
    add("str_len", str_len);
    add("str_count", str_count);
    add("str_replace", str_replace);
    add("str_substr", str_substr);
    add("str_trim", str_trim);
    add("str_lower", str_lower);
    add("str_upper", str_upper);
}

Object& SymbolTable::get(size_t hash)
{
    IdentifierTable::iterator it = identifiers_.find(hash);
    if (it == identifiers_.end()) {
        throw Error::NameError(names_.at(hash));
    }
    return it->second;
}

void SymbolTable::set(size_t hash, const Object& object)
{
    identifiers_[hash].assign(object);
}

void SymbolTable::add(const std::string& name, const FunctionWrapper& function)
{
    size_t hash = hash_identifier_name(name);
    identifiers_.emplace(hash, Object::create_function(function));
}

void SymbolTable::inspect_symbols()
{
    // Get max identifier length
    size_t max_length = 0;
    for (const auto& kv: names_) {
        if (kv.second.size() > max_length) {
            max_length = kv.second.size();
        }
    }

    for (const auto& kv: identifiers_) {
        std::cout
            << std::setw(max_length) << std::left << names_.at(kv.first)
            << " | " << kv.second << std::endl;
    }
    std::cout << "Symbol table size: " << identifiers_.size() << std::endl;
}

void SymbolTable::inspect_gc()
{
    for (ObjectList::const_iterator it = shared_objects_.begin(); it != shared_objects_.end(); ++it) {
        std::cout << (**it).class_name() << "@" <<  *it << std::endl;
    }
}

void SymbolTable::track_object(BaseObject* ptr)
{
    shared_objects_.push_back(ptr);
}

void SymbolTable::mark_and_sweep()
{
    // Visit all objects associated to an identifier
    for (auto& kv: identifiers_) {
        kv.second.gc_visit();
    }

    ObjectList::iterator it = shared_objects_.begin();
    while (it != shared_objects_.end())  {
        // Unmark visited objects, remove the other ones
        if ((**it).is_marked()) {
            (**it).clear_mark();
        }
        else {
            delete *it;
            it = shared_objects_.erase(it);
        }
    }
}

void SymbolTable::destroy()
{
    names_.clear();
    // Clear all identifiers first, so the mark_and_sweep method won't
    // mark any objects. This ensures all allocated objects will be deleted.
    identifiers_.clear();
    mark_and_sweep();
    shared_objects_.clear();
}
