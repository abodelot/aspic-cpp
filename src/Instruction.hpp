#ifndef ASPIC_INSTRUCTION_HPP
#define ASPIC_INSTRUCTION_HPP

#include <string>
#include <vector>

#include "Token.hpp"
#include "AST.hpp"

class OperatorManager;

class Instruction
{
public:
    Instruction();

    /**
     * Eval an expresion.
     * @param expresion: expression to be parsed
     * @return result value as a literal-type token
     */
    Token eval(const std::string& expression);

    /**
     * Print scanned tokens
     */
    void print_tokens() const;

    void print_ast() const;

private:
    /**
     * Parse input into a token list
     * @return true if at least one token was parsed
     */
    bool tokenize(const std::string& expression);

    /**
     * Test if a character is a valid part of an identifier
     */
    bool is_valid_identifier_char(char c) const;

    /**
     * Test if a character is a valid part of an operator
     */
    bool is_valid_operator_char(char c) const;

    AST::Node* parse(int rbp);

    void advance(Token::Type type);

    /**
     * Parse method when token appears at the beginning of a language construct
     */
    AST::Node* null_denotation(Token& current);

    /**
     * Parse method when token appears inside the construct
     */
    AST::Node* left_denotation(Token& current, AST::Node* left);


    typedef std::vector<Token> TokenList;

    void print_tokens(const TokenList& t) const;

    TokenList tokens_;
    AST ast_;
    OperatorManager& operators_;
    size_t index_;
};

#endif
