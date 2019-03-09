#ifndef OPERATORMANAGER_HPP
#define OPERATORMANAGER_HPP

#include <map>
#include <string>

#include "Token.hpp"

/**
 * Helper class for operators
 */
class OperatorManager
{
public:
    static OperatorManager& get_instance();

    /**
     * Eval a string representing an operator
     * @param str: string to be evaluated
     * @param op_type: returned operator type
     * @param previous: token preceding the operator in the expression (or nullptr if operator is the first token)
     * @return true if a token was parsed
     */
    bool eval(const std::string& str, Token::OperatorType& op_type, const Token* previous) const;

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

    int precedences_[Token::_OPERATOR_COUNT];
};

#endif // OPERATORMANAGER_HPP
