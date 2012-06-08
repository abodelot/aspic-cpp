#ifndef OPERATORMANAGER_HPP
#define OPERATORMANAGER_HPP

#include <map>
#include <string>

#include "Token.hpp"

/**
 * Singleton which handles operator recognition
 */
class OperatorManager
{
public:
    static OperatorManager& get_instance();

    /**
     * Eval a string representing an operator
     * @param str: string to be evaluated
     * @param op_type: returned operator type
     * @param previous: token preceding the operator in the expression (or NULL if operator is the first token)
     * @return true if a token was parsed
     */
    bool eval(const std::string& str, Token::OperatorType& op_type, const Token* previous) const;

    /**
     * Alphabet whose operators are made
     * @return string containing all characters which compose operators
     */
    static const char* alphabet();

	/**
	 * Test if a character belongs to operator's alphabet
	 */
    static bool in_alphabet(char c);

	/**
	 * Compare priority of two operators
	 * @return < 0 if op1 < op2, 0 if op1 == op2, > 0 if op1 > op2
	 */
	int compare(const Token& op1, const Token& op2);

	static const char* to_str(Token::OperatorType op);

private:
    OperatorManager();


    /**
     * Convert a binary operator to its unary version
     * @param op: operator to convert
     * @return unary operator, or original op if conversion failed
     */
    static Token::OperatorType to_unary(Token::OperatorType op);

    typedef std::map<std::string, Token::OperatorType> OperatorMap;
    OperatorMap operators_;

    int precedences_[Token::OPERATOR_COUNT];

    typedef std::map<std::string, Token::Keyword> KeywordMap;
    KeywordMap keywords_;
};

#endif // OPERATORMANAGER_HPP
