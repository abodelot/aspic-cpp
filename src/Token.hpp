#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>

class Variable;
class Function;

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
		OPERATOR,
		VARIABLE,
		FUNCTION,
		ARG_SEPARATOR,
		LEFT_BRACKET,
		RIGHT_BRACKET,
		KEYWORD
	};

	static const char* type_to_str(Type type);

	/**
	 * Types d'opérateur (token de type OPERATOR uniquement)
	 * dans l'ordre croissant de priorité
	 */
	enum OperatorType
	{
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
		OP_SUBTRACT_AND_ASSIGN  // -=
	};

	static const int OPERATOR_COUNT = 23;

	enum Keyword
	{
		KW_IF,
		KW_ELIF,
		KW_ELSE,
		KW_WHILE,
		KW_END
	};

    Token(Type);

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
	static Token create_variable(Variable* value);
	static Token create_function(const Function* func);

	/**
	 * Getters, according to type
	 */
	Type get_type() const;
	OperatorType    get_operator_type() const;
	const Function* get_function() const;

    /**
     * Print string representation
     */
	void debug(std::ostream& os) const;
	void print_value(std::ostream& os) const;

	/**
	 * Operator helpers
	 */
	bool is_operator() const;
	bool is_unary_operator() const;
	bool is_binary_operator() const;
	bool is_right_associative_operator() const;

	/**
	 * Apply an operation on an operand
	 * @param op: applied operator
	 * @param operand: second operand (for binary operators)
	 * @return operation result, encapsuled in a new token
	 */
	Token apply_unary_operator(Token::OperatorType op);
	Token apply_binary_operator(Token::OperatorType op, Token& operand);

	/**
	 * Operand helpers
	 */
	bool is_literal() const;
	bool is_typed(Type type) const;

	/**
	 * Get literal value from an operand token, value is casted if necesary
	 * If token cannot be represented as the requested type, TypeError is raised
	 */
	const std::string& as_string() const;
	int                as_int() const;
	double             as_float() const;
	bool               as_bool() const;

private:
	/**
	 * Holds the token value, according to Token::Type
	 */
	union TokenData
	{
		OperatorType    op_type;     // OPERATOR
		int             int_value;   // INT_LITERAL
		bool            bool_value;  // BOOL_LITERAL
		double          float_value; // FOAT_LITERAL
		Variable*       variable;    // VARIABLE
		const Function* function;    // FUNCTION
		Keyword         keyword;     // KEYWORD
	};
	// Cannot store it into TokenData union because std::string is an object
	std::string str_;              // STRING_LITERAL

	TokenData  data_;
	Type       type_;
};

#endif // TOKEN_HPP
