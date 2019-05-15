#include "Object.hpp"
#include "SymbolTable.hpp"
#include "Error.hpp"

#include <cmath>

// constructors

Object::Object():
    type_(NULL_VALUE)
{
}

Object::Object(Type type):
    type_(type)
{
}

Object Object::create_int(int value)
{
    Object self(INT);
    self.data_.int_ = value;
    return self;
}

Object Object::create_float(double value)
{
    Object self(FLOAT);
    self.data_.float_ = value;
    return self;
}

Object Object::create_bool(bool value)
{
    Object self(BOOL);
    self.data_.bool_ = value;
    return self;
}

Object Object::create_string(const std::string& string)
{
    Object self(STRING);
    self.string_ = string;
    return self;
}

Object Object::create_function(FunctionWrapper function_ptr)
{
    Object self(BUILTIN_FUNCTION);
    self.data_.function_ptr_ = function_ptr;
    return self;
}

Object Object::create_reference(size_t id_hash)
{
    Object self(REFERENCE);
    self.data_.id_hash_ = id_hash;
    return self;
}

Object Object::create_null()
{
    return Object(NULL_VALUE);
}

// types
// -----------------------------------------------------------------------------

const char* Object::type_to_str(Type type)
{
    switch (type) {
        case INT:
            return "int";
        case FLOAT:
            return "float";
        case BOOL:
            return "bool";
        case STRING:
            return "string";
        case BUILTIN_FUNCTION:
            return "function";
        case REFERENCE:
            return "reference";
        case NULL_VALUE:
            return "null";
    }
    return nullptr;
}

Object::Type Object::get_type() const
{
    return type_;
}

const Object& Object::get_value() const
{
    return type_ == REFERENCE ? SymbolTable::get(data_.id_hash_) : *this;
}

Object::Type Object::get_value_type() const
{
    return type_ == REFERENCE ? SymbolTable::get(data_.id_hash_).get_value_type() : type_;
}

bool Object::contains(Type type) const
{
    return type_ == type || (type_ == REFERENCE && SymbolTable::get(data_.id_hash_).type_ == type);
}

const std::string& Object::get_string() const
{
    if (type_ == STRING) {
        return string_;
    }
    if (type_ == REFERENCE) {
        return SymbolTable::get(data_.id_hash_).get_string();
    }
    throw Error::TypeError("a string is required");
}

int Object::get_int() const
{
    if (type_ == INT) {
        return data_.int_;
    }
    if (type_ == REFERENCE) {
        return SymbolTable::get(data_.id_hash_).get_int();
    }
    throw Error::TypeError("an integer is required");
}

double Object::get_float() const
{
    if (type_ == FLOAT) {
        return data_.float_;
    }
    if (type_ == INT) {
        return static_cast<double>(data_.int_);
    }
    if (type_ == REFERENCE) {
        return SymbolTable::get(data_.id_hash_).get_float();
    }
    throw Error::TypeError("a float is required");
}

FunctionWrapper Object::get_function() const
{
    if (type_ == BUILTIN_FUNCTION) {
        return data_.function_ptr_;
    }
    if (type_ == REFERENCE) {
        return SymbolTable::get(data_.id_hash_).data_.function_ptr_;
    }
    throw Error::TypeError("a function is required");
}

bool Object::truthy() const
{
    switch (type_) {
        case INT:
            return data_.int_ != 0;
        case FLOAT:
            return data_.float_ != 0.f;
        case STRING:
            return string_.size() > 0;
        case BOOL:
            return data_.bool_;
        case NULL_VALUE:
            // null is always evaluated as false
            return false;
        case BUILTIN_FUNCTION:
            // a built-in function is always evaluated as true
            return true;
        case REFERENCE:
            return SymbolTable::get(data_.id_hash_).truthy();
    }
    return false; // Unreachable, fix -Wreturn-type
}

// Operations

bool Object::equal(const Object& object) const
{
    if (type_ != object.type_) {
        // Allow casting for int/float
        if (is_numeric() && object.is_numeric()) {
            // Different object types, but both are numeric => at least one of
            // them is a float, so both operands are cast to float
            return get_float() == object.get_float();
        }
        return false;
    }
    switch (type_) {
        case BOOL:
            return data_.bool_ == object.data_.bool_;
        case INT:
            return data_.int_ == object.data_.int_;
        case FLOAT:
            return data_.float_ == object.data_.float_;
        case STRING:
            return string_ == object.string_;
        case NULL_VALUE:
            return true; // null == null
        case BUILTIN_FUNCTION:
            return data_.function_ptr_ == object.data_.function_ptr_;
        default:
            return false;
    }
}

Object Object::apply_unary_operator(Operator op) const
{
    switch (type_) {
        case INT:
        case BOOL:
            switch (op) {
                case Operator::OP_UNARY_PLUS:
                    return Object::create_int(+get_int());
                case Operator::OP_UNARY_MINUS:
                    return Object::create_int(-get_int());
                case Operator::OP_NOT:
                    return Object::create_bool(!truthy());
                default: break;
            }
            break;
        case FLOAT:
            switch (op) {
                case Operator::OP_UNARY_PLUS:
                    return Object::create_float(+get_float());
                case Operator::OP_UNARY_MINUS:
                    return Object::create_float(-get_float());
                case Operator::OP_NOT:
                    return Object::create_bool(!truthy());
                default: break;
            }
            break;
        case STRING:
            if (op == Operator::OP_NOT) {
                return Object::create_bool(!truthy());
            }
            break;
        case REFERENCE:
            return SymbolTable::get(data_.id_hash_).apply_unary_operator(op);
        default:
            break;

    }
    throw Error::UnsupportedUnaryOperator(type_, op);
}

Object Object::apply_binary_operator(Operator op, const Object& operand) const
{
    switch (type_) {
    case INT:
        if (operand.contains(FLOAT)) {
            // If the other operand is float typed, the result will be also float typed
            Object cast_to_float = Object::create_float(get_float());
            return cast_to_float.apply_binary_operator(op, operand);
        }

        switch (op) {
            case Operator::OP_POW:
                return Object::create_int(std::pow(get_int(), operand.get_int()));

            case Operator::OP_MULTIPLICATION:
                if (operand.contains(STRING)) {
                    return multiply_string(operand.get_string(), get_int());
                }
                return Object::create_int(get_int() * operand.get_int());

            case Operator::OP_DIVISION:
            {
                int denominator = operand.get_int();
                if (denominator == 0) {
                    throw Error::DivideByZero();
                }
                return Object::create_int(get_int() / denominator);
            }
            case Operator::OP_MODULO:
            {
                int denominator = operand.get_int();
                if (denominator == 0) {
                    throw Error::DivideByZero();
                }
                return Object::create_int(get_int() % denominator);
            }
            case Operator::OP_ADDITION:
                return Object::create_int(get_int() + operand.get_int());

            case Operator::OP_SUBTRACTION:
                return Object::create_int(get_int() - operand.get_int());

            case Operator::OP_LESS_THAN:
                return Object::create_bool(get_int() < operand.get_int());

            case Operator::OP_LESS_THAN_OR_EQUAL:
                return Object::create_bool(get_int() <= operand.get_int());

            case Operator::OP_GREATER_THAN:
                return Object::create_bool(get_int() > operand.get_int());

            case Operator::OP_GREATER_THAN_OR_EQUAL:
                return Object::create_bool(get_int() >= operand.get_int());

            default: break;
        }
        break;

    case FLOAT:
        switch (op) {
            case Operator::OP_POW:
                return Object::create_float(std::pow(get_float(), operand.get_float()));

            case Operator::OP_MULTIPLICATION:
                return Object::create_float(get_float() * operand.get_float());

            case Operator::OP_DIVISION:
            {
                double denominator = operand.get_float();
                if (denominator == 0.f) {
                    throw Error::DivideByZero();
                }
                return Object::create_float(get_float() / denominator);
            }
            case Operator::OP_MODULO:
            {
                double denominator = operand.get_float();
                if (denominator == 0.f) {
                    throw Error::DivideByZero();
                }
                return Object::create_float(fmod(get_float(), denominator));
            }
            case Operator::OP_ADDITION:
                return Object::create_float(get_float() + operand.get_float());

            case Operator::OP_SUBTRACTION:
                return Object::create_float(get_float() - operand.get_float());

            case Operator::OP_LESS_THAN:
                return Object::create_bool(get_float() < operand.get_float());

            case Operator::OP_LESS_THAN_OR_EQUAL:
                return Object::create_bool(get_float() <= operand.get_float());

            case Operator::OP_GREATER_THAN:
                return Object::create_bool(get_float() > operand.get_float());

            case Operator::OP_GREATER_THAN_OR_EQUAL:
                return Object::create_bool(get_float() >= operand.get_float());

            default: break;
        }
        break;

    case STRING:
        switch (op) {
            case Operator::OP_INDEX:
                if (operand.contains(INT)) {
                    int index = operand.get_int();
                    int length = string_.size();
                    if (index < -length || index >= length) {
                        throw Error::IndexError(index);
                    }
                    if (index < 0) {
                        index = string_.size() + index;
                    }
                    // Return char located at index as a new string token
                    return Object::create_string(std::string(1, string_[index]));
                }
                else {
                    throw Error::UnsupportedBinaryOperator(type_, operand.get_value_type(), op);
                }

            case Operator::OP_ADDITION:
                if (operand.contains(STRING)) {
                    return Object::create_string(string_ + operand.get_string());
                }
                else {
                    throw Error::UnsupportedBinaryOperator(type_, operand.get_value_type(), op);
                }

            case Operator::OP_MULTIPLICATION:
                return multiply_string(string_, operand.get_int());

            case Operator::OP_LESS_THAN:
                return Object::create_bool(string_ < operand.get_string());

            case Operator::OP_LESS_THAN_OR_EQUAL:
                return Object::create_bool(string_ <= operand.get_string());

            case Operator::OP_GREATER_THAN:
                return Object::create_bool(string_ > operand.get_string());

            case Operator::OP_GREATER_THAN_OR_EQUAL:
                return Object::create_bool(string_ >= operand.get_string());

            default: break;
        }
        break;

    case REFERENCE:
        switch (op) {
            // Handle operators which update the variable value, operand is the assigned lvalue
            case Operator::OP_ASSIGNMENT:
            {
                //  copy by value, otherwise copying the operand name will create a reference
                if (operand.type_ == REFERENCE) {
                    const Object& value = SymbolTable::get(operand.data_.id_hash_);
                    SymbolTable::set(data_.id_hash_, value);
                    return value;
                }
                SymbolTable::set(data_.id_hash_, operand);
                return operand;
            }
            case Operator::OP_MULTIPLY_AND_ASSIGN:
            {
                Object& value = SymbolTable::get(data_.id_hash_);
                value = value.apply_binary_operator(Operator::OP_MULTIPLICATION, operand);
                return value;
            }
            case Operator::OP_DIVIDE_AND_ASSIGN:
            {
                Object& value = SymbolTable::get(data_.id_hash_);
                value = value.apply_binary_operator(Operator::OP_DIVISION, operand);
                return value;
            }
            case Operator::OP_MODULO_AND_ASSIGN:
            {
                Object& value = SymbolTable::get(data_.id_hash_);
                value = value.apply_binary_operator(Operator::OP_MODULO, operand);
                return value;
            }
            case Operator::OP_ADD_AND_ASSIGN:
            {
                Object& value = SymbolTable::get(data_.id_hash_);
                value = value.apply_binary_operator(Operator::OP_ADDITION, operand);
                return value;
            }
            case Operator::OP_SUBTRACT_AND_ASSIGN:
            {
                Object& value = SymbolTable::get(data_.id_hash_);
                value = value.apply_binary_operator(Operator::OP_SUBTRACTION, operand);
                return value;
            }
            default:
                // Reference is not modified, extract value and apply operator on it
                return SymbolTable::get(data_.id_hash_).apply_binary_operator(op, operand);
        }
        break;
    default:
        break;
    }

    throw Error::UnsupportedBinaryOperator(type_, operand.get_value_type(), op);
}

Object Object::multiply_string(const std::string& source, int count)
{
    if (count < 0) {
        throw Error::ValueError("Cannot construct string with negative length");
    }
    Object result = Object::create_string("");
    result.string_.reserve(count * source.size());
    for (int i = 0; i < count; ++i) {
        result.string_ += source;
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Object& object)
{
    switch (object.type_) {
        case Object::INT:
            os << object.data_.int_;
            break;
        case Object::FLOAT:
            os << object.data_.float_;
            break;
        case Object::STRING:
            os << object.string_;
            break;
        case Object::BOOL:
            os << (object.data_.bool_ ? "true" : "false");
            break;
        case Object::NULL_VALUE:
            os << "null";
            break;
        case Object::BUILTIN_FUNCTION:
            // Function pointer needs to be casted to void* to be displayed on std::cout
            os << "<function at " << reinterpret_cast<void *>(object.data_.function_ptr_) << ">";
            break;
        case Object::REFERENCE:
            os << SymbolTable::get(object.data_.id_hash_);
            break;
    }
    return os;
}
