#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <string>
#include <vector>

#include "Token.hpp"

class SymbolTable;
class OperatorManager;

/**
 * Parseur d'expressions mathématiques
 */
class Instruction
{
public:
	Instruction(SymbolTable& symbols);

	/**
	 * Évaluer une expression
	 */
	Token eval(const std::string& expression);

	/**
	 * Afficher la notation postfixe de la dernière expression évaluée
	 */
	void debug_postfix() const;
	void debug_tokens() const;

private:
	/**
	 * Transformation de l'expression en suite de tokens
	 */
	void tokenization(const std::string& expression);

	/**
	 * Conversion d'une expression infixe vers une notation postfixe
	 */
	void infix_to_postfix();

	/**
	 * Évaluation de l'expression postfixée
	 */
	Token eval_postfix();

	/**
	 * @return true si c est un chiffre
	 */
	bool is_digit(char c) const;

	/**
	 * @return true si 'c' est un composant valide d'un identifiant
	 */
	bool is_valid_identifier_symbol(char c) const;

	typedef std::vector<Token> TokenList;

	void print_tokens(const TokenList& t) const;

	TokenList postfix_;
	TokenList tokens_;
	SymbolTable& symbols_;;
	OperatorManager& operators_;
};

#endif // INSTRUCTION_HPP
