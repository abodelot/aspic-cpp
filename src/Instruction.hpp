#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <string>
#include <vector>

#include "Token.hpp"
#include "TokenStack.hpp"

class OperatorManager;

class Instruction
{
public:
	Instruction();

	/**
	 * Eval an expresion
	 * @return result value as a literal-type token
	 */
	Token eval(const std::string& expression);

	/**
	 * Print infix and postifx notation of the last evalued expression
	 */
	void debug() const;

private:
	/**
	 * Parse input into a token list
	 */
	void tokenization(const std::string& expression);

	/**
	 * Transform infix expression to a postfix expression (Reverse Polish Notation)
	 */
	void infix_to_postfix();

	/**
	 * Eval postfix expression and return result as a literal-type token
	 */
	Token eval_postfix();

	/**
	 * Test if a character is a valid part of an identifier
	 */
	bool is_valid_identifier_char(char c) const;

	/**
	 * Test if a character is a valid part of an operator
	 */
	bool is_valid_operator_char(char c) const;

	typedef std::vector<Token> TokenList;

	void print_tokens(const TokenList& t) const;

	TokenList postfix_;
	TokenList tokens_;
	TokenStack stack_;
	OperatorManager& operators_;
};

#endif // INSTRUCTION_HPP
