#include "LibTypes.hpp"
#include "Error.hpp"
#include "ast/Node.hpp"
#include "ast/NodeVector.hpp"

#include <sstream>


Object core_str(const ast::NodeVector& args)
{
    args.check(1);
    Object value = args[0]->eval();
    return value.get_type() == Object::STRING
        ? value
        : Object::create_string(value.to_string());
}

Object core_int(const ast::NodeVector& args)
{
    args.check(1);
    Object value = args[0]->eval().get_value();
    switch (value.get_type()) {
    case Object::INT:
        return value;
    case Object::FLOAT:
        return Object::create_int(value.get_int());
    case Object::STRING:
    {
        int base = 10;
        // 2nd argument: base
        if (args.size() == 2) {
            base = args[1]->eval().get_int();
        }
        char* tmp = nullptr;
        int result = std::strtol(value.get_string().c_str(), &tmp, base);
        if (*tmp != '\0') {
            std::ostringstream oss;
            oss << "invalid string literal '" << value.get_string()
                << "' for int() with base " << base;
            throw Error::ValueError(oss.str());
        }
        return Object::create_int(result);
    }
    default:
        throw Error::TypeConvertError(value.get_type(), Object::INT);
    }
}

