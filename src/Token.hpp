#ifndef ASPIC_TOKEN_HPP
#define ASPIC_TOKEN_HPP

#include <string>
#include <iostream>

#include "FunctionWrapper.hpp"


/**
 * Token: holds an atomic element in an expression
 */
class Token
{
public:
    enum Type
    {
        INT_LITERAL,
        FLOAT_LITERAL,
        STRING_LITERAL,
        BOOL_LITERAL,
        NULL_VALUE,
        OPERATOR,
        FUNCTION,
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

    /**
     * Operator type for token typed as OPERATOR
     */
    enum OperatorType
    {
        OP_INDEX,       // []
        OP_FUNC_CALL,   // ()

        OP_NOT,         // !
        OP_UNARY_PLUS,  // +
        OP_UNARY_MINUS, // -

        OP_POW,            // **
        OP_MULTIPLICATION, // *
        OP_DIVISION,       // /
        OP_MODULO,         // %
        OP_ADDITION,       // +
        OP_SUBTRACTION,    // -

        OP_LESS_THAN,             // <
        OP_LESS_THAN_OR_EQUAL,    // <=
        OP_GREATER_THAN,          // >
        OP_GREATER_THAN_OR_EQUAL, // >=
        OP_EQUAL,                 // ==
        OP_NOT_EQUAL,             // !=
        OP_LOGICAL_AND,           // &&
        OP_LOGICAL_OR,            // ||

        OP_ASSIGNMENT,          // =
        OP_MULTIPLY_AND_ASSIGN, // *=
        OP_DIVIDE_AND_ASSIGN,   // /=
        OP_MODULO_AND_ASSIGN,   // %=
        OP_ADD_AND_ASSIGN,      // +=
        OP_SUBTRACT_AND_ASSIGN, // -=
    };

    Token();
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

    /**
     * Create an operator token
     */
    static Token create_operator(OperatorType op_type);

    /**
     * Create a token bound to a symbol
     */
    static Token create_identifier(const std::string& identifier_name);
    static Token create_function(const FunctionWrapper& function);

    // Methods for operator tokens
    // -------------------------------------------------------------------------

    /**
     * Operator helpers
     */
    bool is_operator() const;
    bool is_right_associative_operator() const;

    /**
     * Getters, according to type
     */
    OperatorType get_operator_type() const;

    /**
     * Apply an operation on an operand
     * @param op: applied operator
     * @param operand: second operand (for binary operators)
     * @return operation result, encapsuled in a new token
     */
    Token apply_unary_operator(Token::OperatorType op) const;
    Token apply_binary_operator(Token::OperatorType op, const Token& operand) const;

    // Methods for value tokens
    // -------------------------------------------------------------------------

    /**
     * @return true if token holds a value, otherwise false
     */
    bool is_value() const;

    /**
     * If token is a value, return reference to self.
     * If token is an identifier, return reference to token in the symbol table
     */
    const Token& get_value() const;

    Type get_value_type() const;

    bool is_numeric() const
    {
        return type_ == INT_LITERAL || type_ == FLOAT_LITERAL;
    }

    /**
     * Compare two value tokens
     */
    bool equal(const Token& token) const;

    // Value getters
    // -------------------------------------------------------------------------

    /**
     * Get literal value from an operand token, value is casted if necesary
     * If token cannot be represented as the requested type, TypeError is raised
     */
    const std::string& as_string() const;
    int                as_int() const;
    double             as_float() const;
    bool               as_bool() const;

    const FunctionWrapper& get_function() const;

    /**
     * Print string representation
     */
    void debug(std::ostream& os) const;

    int lbp;

private:
    friend std::ostream& operator<<(std::ostream&, const Token& token);

    /**
     * Check if token is typed with given type, or if token is an identifier
     * pointing to a literal with given type
     */
    bool contains(Type type) const;

    // helper function for str * int operation
    static Token multiply_string(const std::string& source, int count);

    /**
     * Holds the token value, according to Token::Type
     */
    union TokenData
    {
        // Define empty ctor to allow non-POD union members
        TokenData()
        {
        }

        OperatorType    op_type;     // OPERATOR
        int             int_value;   // INT_LITERAL
        bool            bool_value;  // BOOL_LITERAL
        double          float_value; // FOAT_LITERAL
        FunctionWrapper function;    // FUNCTION
    };
    // Cannot store it into TokenData union because std::string is an object
    std::string str_;              // STRING_LITERAL

    TokenData  data_;
    Type       type_;
};

std::ostream& operator<<(std::ostream&, const Token& token);

#endif
