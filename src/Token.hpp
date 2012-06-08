#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include "SymbolTable.hpp"

class Variable;

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

	enum Keyword
	{
		KW_IF,
		KW_ELIF,
		KW_ELSE,
		KW_WHILE,
		KW_END
	};

	static const int OPERATOR_COUNT = 23;

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
	static Token create_variable(Variable* var);
	static Token create_function(SymbolTable::Function func);


	/**
	 * Exécuter une fonction
	 * @param args: arguments la fonction, sous forme de tokens
	 * @return résultat de la fonction, sous forme de token
	 */
	Token exec_function(std::stack<Token>& args) const
	{
		return (data_.function)(args);
	}

	/**
	 * Get token type and string representation
	 */
	Type get_type() const;
	static const char* to_str(Type type);
	OperatorType get_operator_type() const;


    /**
     * Print string representation
     */
	void print(std::ostream& os) const;

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
	 * Operand cast
	 */
	std::string string_value() const;
	int         int_value() const;
	double      float_value() const;
	bool        bool_value() const;

private:
	/**
	 * Holds token value, according to its type
	 */
	union TokenData
	{
		OperatorType          op_type;     // OPERATOR
		int                   int_value;   // INT_LITERAL
		bool                  bool_value;  // BOOL_LITERAL
		double                float_value; // FOAT_LITERAL
		Variable*             variable;    // VARIABLE
		SymbolTable::Function function;    // FUNCTION
		Keyword               keyword;     // KEYWORD
	};
	// STRING_LITERAL
	std::string str_; // HACK: cannot store it into TokenData union because std::string is an object

	TokenData data_;
	Type type_;
};

#endif // TOKEN_HPP
