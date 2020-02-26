#include "Token.hpp"
#include "Error.hpp"
#include "Operators.hpp"
#include "SymbolTable.hpp"

#include <iostream>


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

Operator Token::get_operator() const
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

bool Token::end_of_expression() const
{
    switch (type_)
    {
    case VALUE:
    case IDENTIFIER:
    case RIGHT_PAREN:
    case RIGHT_BRACKET:
    case RIGHT_BRACE:
    case KW_END:
        return true;
    default:
        return false;
    }
}

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    switch (token.type_) {
    case Token::VALUE:
        os << token.object_;
        break;
    case Token::ARRAY_LITERAL:
        os << "[";
        break;
    case Token::MAP_LITERAL:
        os << "{";
        break;
    case Token::IDENTIFIER:
        os << SymbolTable::get_name(token.data_.id_hash);
        break;
    case Token::ARG_SEPARATOR:
        os << ",";
        break;
    case Token::LEFT_PAREN:
        os << "(";
        break;
    case Token::RIGHT_PAREN:
        os << ")";
        break;
    case Token::RIGHT_BRACKET:
        os << "]";
        break;
    case Token::RIGHT_BRACE:
        os << "}";
        break;
    case Token::COLON:
        os << ":";
        break;
    case Token::OPERATOR:
        os << Operators::to_str(token.data_.op_type);
        break;
    case Token::KW_IF:
        os << "kw:if";
        break;
    case Token::KW_ELIF:
        os << "kw:elif";
        break;
    case Token::KW_ELSE:
        os << "kw:else";
        break;
    case Token::KW_WHILE:
        os << "kw:while";
        break;
    case Token::KW_END:
        os << "kw:end";
        break;
    case Token::END_EXPR:
        os << "<end-expr>";
        break;
    }
    return os;
}
