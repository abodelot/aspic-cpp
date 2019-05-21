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
        OPERATOR,
        IDENTIFIER,
        ARG_SEPARATOR,
        LEFT_PARENTHESIS,
        RIGHT_PARENTHESIS,
        RIGHT_BRACKET, // Note: left bracket is OP_INDEX
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

    // Methods for operator tokens
    // -------------------------------------------------------------------------

    /**
     * Operator helpers
     */
    bool is_operator() const;

    /**
     * Getters, according to type
     */
    Operator get_operator_type() const;
    size_t get_id_hash() const;
    const Object& get_object() const;

    // Methods for value tokens
    // -------------------------------------------------------------------------

    /**
     * If token is a value, return reference to self.
     * If token is an identifier, return reference to token in the symbol table
     */
    const Token& get_value() const;

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

    Object          object_;

    // Cannot store it into TokenData union because std::string is an object
    TokenData  data_;
    Type       type_;
};

std::ostream& operator<<(std::ostream&, const Token& token);

#endif
