#ifndef ASPIC_OPERATORS_HPP
#define ASPIC_OPERATORS_HPP

/**
 * List of available operator types
 */
enum class Operator
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

/**
 * Helper functions for operators
 * See Token::OperatorType enum for list of supported operators
 * See Scanner class for mapping symbols to operators
 */
class Operators
{
public:
    static bool is_right_associative(Operator op);

    static int get_binding_power(Operator op);

    static const char* to_str(Operator op);

private:
    Operators() = delete;
};

#endif
