#include <iostream>
#include <cmath>

#include "Token.hpp"
#include "FunctionWrapper.hpp"
#include "Error.hpp"
#include "OperatorManager.hpp"
#include "SymbolTable.hpp"

// helpers ---------------------------------------------------------------------

const char* Token::type_to_str(Type type)
{
    switch (type) {
        case INT_LITERAL:
            return "int";
        case FLOAT_LITERAL:
            return "float";
        case BOOL_LITERAL:
            return "bool";
        case STRING_LITERAL:
            return "string";
        case NULL_VALUE:
            return "null";
        case FUNCTION:
            return "function";
        case IDENTIFIER:
            return "identifier";
        case ARG_SEPARATOR:
            return ",";
        case LEFT_PARENTHESIS:
            return "(";
        case RIGHT_PARENTHESIS:
            return ")";
        case RIGHT_BRACKET:
            return "]";
        case OPERATOR:
            return "operator";
        case END:
            return "<end>";
    }
    return nullptr;
}

// constructors ----------------------------------------------------------------

Token::Token():
    lbp(0),
    type_(INT_LITERAL)
{
    data_.int_value = 0;
}

Token::Token(Type type):
    lbp(0),
    type_(type)
{
}

Token Token::create_int(int value)
{
    Token self(INT_LITERAL);
    self.data_.int_value = value;
    return self;
}

Token Token::create_string(const std::string& value)
{
    Token self(STRING_LITERAL);
    self.str_ = value;
    return self;
}

Token Token::create_float(double value)
{
    Token self(FLOAT_LITERAL);
    self.data_.float_value = value;
    return self;
}

Token Token::create_bool(bool value)
{
    Token self(BOOL_LITERAL);
    self.data_.bool_value = value;
    return self;
}

Token Token::create_operator(OperatorType op_type)
{
    Token self(OPERATOR);
    self.lbp = OperatorManager::get_instance().get_binding_power(op_type);
    self.data_.op_type = op_type;
    return self;
}

Token Token::create_function(const FunctionWrapper& function)
{
    Token self(FUNCTION);
    self.data_.function = function;
    return self;
}

Token Token::create_identifier(const std::string& identifier_name)
{
    Token self(IDENTIFIER);
    self.str_ = identifier_name;
    return self;
}

// getters by type -------------------------------------------------------------

Token::Type Token::get_type() const
{
    return type_;
}

Token::Type Token::get_contained_type() const
{
    return type_== IDENTIFIER ? SymbolTable::get(str_).type_ : type_;
}

Token::OperatorType Token::get_operator_type() const
{
    return data_.op_type;
}

const FunctionWrapper& Token::get_function() const
{
    if (type_ == FUNCTION) {
        return data_.function;
    }
    if (type_ == IDENTIFIER) {
        return SymbolTable::get(str_).get_function();
    }
    throw Error::TypeError("token is not a function");
}

// operator helpers ------------------------------------------------------------

bool Token::is_operator() const
{
    return type_ == OPERATOR;
}

bool Token::is_right_associative_operator() const
{
    // must be an operator
    if (!is_operator()) {
        return false;
    }
    // switch on remaining right associative operators
    switch (data_.op_type) {
        case OP_NOT:
        case OP_UNARY_PLUS:
        case OP_UNARY_MINUS:
        case OP_POW:
        case OP_ASSIGNMENT:
        case OP_MULTIPLY_AND_ASSIGN:
        case OP_DIVIDE_AND_ASSIGN:
        case OP_MODULO_AND_ASSIGN:
        case OP_ADD_AND_ASSIGN:
        case OP_SUBTRACT_AND_ASSIGN:
            return true;
        default:
            return false;
    }
}

// operator implementation -----------------------------------------------------

Token Token::apply_unary_operator(Token::OperatorType op)
{
    switch (type_) {
        case INT_LITERAL:
        case BOOL_LITERAL:
            switch (op) {
                case OP_UNARY_PLUS:
                    return Token::create_int(+ as_int());
                case OP_UNARY_MINUS:
                    return Token::create_int(- as_int());
                case OP_NOT:
                    return Token::create_bool(! as_bool());
                default: break;
            }
            break;
        case FLOAT_LITERAL:
            switch (op) {
                case OP_UNARY_PLUS:
                    return Token::create_float(+ as_float());
                case OP_UNARY_MINUS:
                    return Token::create_float(- as_float());
                case OP_NOT:
                    return Token::create_bool(! as_bool());
                default: break;
            }
            break;
        case STRING_LITERAL:
            if (op == OP_NOT) {
                return Token::create_bool(! as_bool());
            }
            break;
        case IDENTIFIER:
            return SymbolTable::get(str_).apply_unary_operator(op);
        default:
            break;

    }
    throw Error::UnsupportedUnaryOperator(type_, op);
}

Token Token::apply_binary_operator(Token::OperatorType op, Token& operand)
{
    switch (type_)
    {
    case INT_LITERAL:
    case BOOL_LITERAL: // false is equivalent to 0, true is equivalent to 1
        if (operand.contains(FLOAT_LITERAL)) {
            // if the other operand is float typed, the result will be also float typed
            Token cast_to_float = Token::create_float(as_float());
            return cast_to_float.apply_binary_operator(op, operand);
        }

        switch (op) {
            case OP_POW:
                return Token::create_int(std::pow(as_int(), operand.as_int()));

            case OP_MULTIPLICATION:
                // 2 * "hello" => "hellohello"
                if (operand.contains(STRING_LITERAL)) {
                    return operand.apply_binary_operator(op, *this);
                }
                return Token::create_int(as_int() * operand.as_int());

            case OP_DIVISION:
            {
                int denominator = operand.as_int();
                if (denominator == 0) {
                    throw Error::DivideByZero();
                }
                return Token::create_int(as_int() / denominator);
            }
            case OP_MODULO:
            {
                int denominator = operand.as_int();
                if (denominator == 0) {
                    throw Error::DivideByZero();
                }
                return Token::create_int(as_int() % denominator);
            }
            case OP_ADDITION:
                return Token::create_int(as_int() + operand.as_int());

            case OP_SUBTRACTION:
                return Token::create_int(as_int() - operand.as_int());

            case OP_LESS_THAN:
                return Token::create_bool(as_int() < operand.as_int());

            case OP_LESS_THAN_OR_EQUAL:
                return Token::create_bool(as_int() <= operand.as_int());

            case OP_GREATER_THAN:
                return Token::create_bool(as_int() > operand.as_int());

            case OP_GREATER_THAN_OR_EQUAL:
                return Token::create_bool(as_int() >= operand.as_int());

            case OP_EQUAL:
                return Token::create_bool(as_int() == operand.as_int());

            case OP_NOT_EQUAL:
                return Token::create_bool(as_int() != operand.as_int());

            case OP_LOGICAL_AND:
                return as_bool() ? operand : *this;

            case OP_LOGICAL_OR:
                return as_bool() ? *this : operand;

            default: break;
        }
        break;

    case FLOAT_LITERAL:
        switch (op)
        {
            case OP_POW:
                return Token::create_float(std::pow(as_float(), operand.as_float()));

            case OP_MULTIPLICATION:
                return Token::create_float(as_float() * operand.as_float());

            case OP_DIVISION:
            {
                double denominator = operand.as_float();
                if (denominator == 0.f) {
                    throw Error::DivideByZero();
                }
                return Token::create_float(as_float() / denominator);
            }
            case OP_MODULO:
            {
                double denominator = operand.as_float();
                if (denominator == 0.f) {
                    throw Error::DivideByZero();
                }
                return Token::create_float(fmod(as_float(), denominator));
            }
            case OP_ADDITION:
                return Token::create_float(as_float() + operand.as_float());

            case OP_SUBTRACTION:
                return Token::create_float(as_float() - operand.as_float());

            case OP_LESS_THAN:
                return Token::create_bool(as_float() < operand.as_float());

            case OP_LESS_THAN_OR_EQUAL:
                return Token::create_bool(as_float() <= operand.as_float());

            case OP_GREATER_THAN:
                return Token::create_bool(as_float() > operand.as_float());

            case OP_GREATER_THAN_OR_EQUAL:
                return Token::create_bool(as_float() >= operand.as_float());

            case OP_EQUAL:
                return Token::create_bool(as_float() == operand.as_float());

            case OP_NOT_EQUAL:
                return Token::create_bool(as_float() != operand.as_float());

            case OP_LOGICAL_AND:
                return as_bool() ? operand : *this;

            case OP_LOGICAL_OR:
                return as_bool() ? *this : operand;

            default: break;
        }
        break;

    case STRING_LITERAL:
        switch (op) {
            case OP_INDEX:
                if (operand.contains(Token::INT_LITERAL)) {
                    int index = operand.as_int();
                    int length = str_.size();
                    if (index < -length || index >= length) {
                        throw Error::IndexError(index);
                    }
                    if (index < 0) {
                        index = str_.size() + index;
                    }
                    std::string str(1, str_[index]);
                    return Token::create_string(str);
                }
                else {
                    throw Error::UnsupportedBinaryOperator(type_, operand.get_contained_type(), op);
                }

            case OP_ADDITION:
                if (operand.contains(STRING_LITERAL)) {
                    return Token::create_string(str_ + operand.as_string());
                }
                else {
                    throw Error::UnsupportedBinaryOperator(type_, operand.get_contained_type(), op);
                }

            case OP_MULTIPLICATION:
            {
                int count = operand.as_int();
                std::string result;
                result.reserve(count * str_.size());
                for (int i = 0; i < count; ++i) {
                    result += str_;
                }
                return Token::create_string(result);
            }

            case OP_LESS_THAN:
                return Token::create_bool(str_ < operand.as_string());

            case OP_LESS_THAN_OR_EQUAL:
                return Token::create_bool(str_ <= operand.as_string());

            case OP_GREATER_THAN:
                return Token::create_bool(str_ > operand.as_string());

            case OP_GREATER_THAN_OR_EQUAL:
                return Token::create_bool(str_ >= operand.as_string());

            case OP_EQUAL:
                return Token::create_bool(str_ == operand.as_string());

            case OP_NOT_EQUAL:
                return Token::create_bool(str_ != operand.as_string());

            case OP_LOGICAL_AND:
                return as_bool() ? operand : *this;

            case OP_LOGICAL_OR:
                return as_bool() ? *this : operand;

            default: break;
        }
        break;

    case IDENTIFIER:
        switch (op)
        {
            // Handle operators which update the variable value, operand is the assigned lvalue
            case OP_ASSIGNMENT:
            {
                // Always copy by value, otherwise copying the operand name will create a reference
                if (operand.type_ == IDENTIFIER)
                {
                    Token& lvalue = SymbolTable::get(operand.str_);
                    SymbolTable::set(str_, lvalue);
                    return lvalue;
                }
                SymbolTable::set(str_, operand);
                return operand;
            }
            case OP_MULTIPLY_AND_ASSIGN:
            {
                Token& rvalue = SymbolTable::get(str_);
                rvalue = rvalue.apply_binary_operator(OP_MULTIPLICATION, operand);
                return rvalue;
            }
            case OP_DIVIDE_AND_ASSIGN:
            {
                Token& rvalue = SymbolTable::get(str_);
                rvalue = rvalue.apply_binary_operator(OP_DIVISION, operand);
                return rvalue;
            }
            case OP_MODULO_AND_ASSIGN:
            {
                Token& rvalue = SymbolTable::get(str_);
                rvalue = rvalue.apply_binary_operator(OP_MODULO, operand);
                return rvalue;
            }
            case OP_ADD_AND_ASSIGN:
            {
                Token& rvalue = SymbolTable::get(str_);
                rvalue = rvalue.apply_binary_operator(OP_ADDITION, operand);
                return rvalue;
            }
            case OP_SUBTRACT_AND_ASSIGN:
            {
                Token& rvalue = SymbolTable::get(str_);
                rvalue = rvalue.apply_binary_operator(OP_SUBTRACTION, operand);
                return rvalue;
            }
            default:
                // Variable is not modified, extract value and apply operator on it
                return SymbolTable::get(str_).apply_binary_operator(op, operand);
        }
        break;
    default:
        break;
    }

    throw Error::UnsupportedBinaryOperator(type_, operand.get_contained_type(), op);
}

bool Token::is_literal() const
{
    return type_ == STRING_LITERAL
        || type_ == INT_LITERAL
        || type_ == FLOAT_LITERAL
        || type_ == BOOL_LITERAL
        || type_ == NULL_VALUE
        || type_ == FUNCTION;
}

bool Token::contains(Type type) const
{
    return type_ == type || (type_ == IDENTIFIER && SymbolTable::get(str_).get_type() == type);
}

// cast ------------------------------------------------------------------------

const std::string& Token::as_string() const
{
    if (type_ == STRING_LITERAL) {
        return str_;
    }
    if (type_ == IDENTIFIER) {
        return SymbolTable::get(str_).as_string();
    }
    throw Error::TypeError("a string is required");
}

double Token::as_float() const
{
    switch (type_) {
        case FLOAT_LITERAL:
            return data_.float_value;
        case INT_LITERAL:
            return static_cast<double>(data_.int_value);
        case BOOL_LITERAL:
            return static_cast<double>(data_.bool_value);
        case IDENTIFIER:
            return SymbolTable::get(str_).as_float();
        default:
            throw Error::TypeError("a float is required");
    }
}

int Token::as_int() const
{
    switch (type_) {
        case INT_LITERAL:
            return data_.int_value;
        case BOOL_LITERAL:
            return data_.bool_value ? 1 : 0;
        case IDENTIFIER:
            return SymbolTable::get(str_).as_int();
        default:
            throw Error::TypeError("an integer is required");
    }
}

bool Token::as_bool() const
{
    switch (type_) {
        case INT_LITERAL:
            return data_.int_value != 0;
        case FLOAT_LITERAL:
            return data_.float_value != 0.f;
        case STRING_LITERAL:
            return str_.size() > 0;
        case BOOL_LITERAL:
            return data_.bool_value;
        case NULL_VALUE:
            return false;
        case IDENTIFIER:
            return SymbolTable::get(str_).as_bool();
        default:
            throw Error::TypeError("a boolean is required");
    }
}

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    switch (token.type_) {
        case Token::INT_LITERAL:
            os << token.data_.int_value;
            break;
        case Token::FLOAT_LITERAL:
            os << token.data_.float_value;
            break;
        case Token::STRING_LITERAL:
            os << token.str_;
            break;
        case Token::BOOL_LITERAL:
            os << (token.data_.bool_value ? "true" : "false");
            break;
        case Token::NULL_VALUE:
            os << "null";
            break;
        case Token::FUNCTION:
            // Function pointer needs to be casted to void* to be displayed
            os << "<function at " << reinterpret_cast<void *>(token.data_.function) << ">";
            break;
        case Token::IDENTIFIER:
            os << SymbolTable::get(token.str_);
            break;
        case Token::OPERATOR:
            os << OperatorManager::to_str(token.data_.op_type);
            break;
        case Token::ARG_SEPARATOR:
            os << ',';
            break;
        case Token::LEFT_PARENTHESIS:
            os << '(';
            break;
        case Token::RIGHT_PARENTHESIS:
            os << ')';
            break;
        case Token::RIGHT_BRACKET:
            os << ']';
            break;
        case Token::END:
            os << "<end>";
            break;
        default:
            break;
    }
    return os;
}

// debug -----------------------------------------------------------------------

void Token::debug(std::ostream& os) const
{
    switch (type_)
    {
        case IDENTIFIER:
            os << str_;
            break;
        case STRING_LITERAL:
            os << '"' << str_ << '"';
            break;
        case BOOL_LITERAL:
            os << "bool<" << (data_.bool_value ? "true" : "false") << ">";
            break;
        default:
            os << *this;
    }
}
