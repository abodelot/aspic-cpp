#include "Token.hpp"
#include "Error.hpp"
#include "Operators.hpp"
#include "SymbolTable.hpp"

#include <iostream>


const char* Token::type_to_str(Type type)
{
    switch (type) {
        case VALUE:
            return "value";
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
        case KW_IF:
            return "<if>";
        case KW_ELIF:
            return "<elif>";
        case KW_ELSE:
            return "<else>";
        case KW_WHILE:
            return "<while>";
        case KW_END:
            return "<end>";
        case END_EXPR:
            return "<end-expr>";
    }
    return nullptr;
}

Token::Type Token::get_type() const
{
    return type_;
}

Token::Token(Type type):
    lbp(0),
    type_(type)
{
}

Token::Token(const Object& object):
    lbp(0),
    object_(object),
    type_(VALUE)
{
}

Token Token::create_int(int value)
{
    return Token(Object::create_int(value));
}

Token Token::create_string(const std::string& value)
{
    return Token(Object::create_string(value));
}

Token Token::create_float(double value)
{
    return Token(Object::create_float(value));
}

Token Token::create_bool(bool value)
{
    return Token(Object::create_bool(value));
}

Token Token::create_null()
{
    return Token(Object::create_null());
}

Token Token::create_operator(Operator op_type)
{
    Token self(OPERATOR);
    self.lbp = Operators::get_binding_power(op_type);
    self.data_.op_type = op_type;
    return self;
}

Token Token::create_identifier(const std::string& identifier_name)
{
    Token self(IDENTIFIER);
    self.data_.id_hash = SymbolTable::hash_identifier_name(identifier_name);
    return self;
}

bool Token::is_operator() const
{
    return type_ == OPERATOR;
}

Operator Token::get_operator_type() const
{
    return data_.op_type;
}

size_t Token::get_id_hash() const
{
    return data_.id_hash;
}

const Object& Token::get_object() const
{
    return object_;
}
