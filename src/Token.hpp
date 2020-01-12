#ifndef ASPIC_TOKEN_HPP
#define ASPIC_TOKEN_HPP

#include "Object.hpp"

#include <string>

/**
 * Token: holds an atomic element in an expression
 */
class Token
{
public:
    enum Type
    {
        VALUE,
        MAP_LITERAL,
        ARRAY_LITERAL,
        OPERATOR,
        IDENTIFIER,
        ARG_SEPARATOR,
        LEFT_PAREN,
        RIGHT_PAREN,
        RIGHT_BRACKET, // Note: left bracket is OP_INDEX or ARRAY_LITERAL
        RIGHT_BRACE, // Note: right brace is MAP_LITERAL
        COLON,
        KW_IF,
        KW_ELIF,
        KW_ELSE,
        KW_WHILE,
        KW_END,
        END_EXPR,
    };

    static const char* type_to_str(Type type);

    Token(Type);

    Type get_type() const;

    // Factory methods
    // -------------------------------------------------------------------------

    /**
     * Create an operand token (literal value)
     */
    static Token create_int(int value);
    static Token create_float(double value);
    static Token create_string(const std::string& value);
    static Token create_bool(bool value);
    static Token create_null();

    /**
     * Create an operator token
     */
    static Token create_operator(Operator op_type);

    /**
     * Create a token bound to a symbol
     */
    static Token create_identifier(const std::string& identifier_name);

    /**
     * Getters, according to type
     */
    Operator get_operator_type() const;
    size_t get_id_hash() const;
    const Object& get_object() const;

    int lbp;

private:
    Token(const Object& object);
    Token() = delete;

    /**
     * Holds the token value, according to Token::Type
     */
    union TokenData
    {
        Operator    op_type;     // OPERATOR
        size_t      id_hash;     // IDENTIFIER

    };

    // Cannot store it into TokenData union
    Object          object_;

    TokenData  data_;
    Type       type_;
};

std::ostream& operator<<(std::ostream&, const Token& token);

#endif
