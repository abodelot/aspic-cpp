#ifndef ASPIC_PARSER_HPP
#define ASPIC_PARSER_HPP

#include <string>
#include <vector>

#include "Token.hpp"
#include "AST.hpp"

class OperatorManager;

/**
 * Evaluation entry-point:
 * Tokenize input stream and build AST
 */
class Parser
{
public:
    Parser();

    /**
     * Feed an expresion to the parser. Result will be appended to AST.
     * @param expresion: expression to be parsed
     * @return result value as a literal-type token
     */
    bool feed(const std::string& expression);

    /**
     * Print scanned tokens (debug)
     */
    void print_tokens() const;

    /**
     * Print internal AST to stdout
     */
    void print_ast() const;

    /**
     * Evaluate internal AST.
     * @return result value, wrapped in a token
     */
    Token eval_ast() const;

    /**
     * Clear parsed tokens and AST.
     */
    void reset();

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


    typedef std::vector<Token> TokenVector;

    TokenVector tokens_;
    AST ast_;
    OperatorManager& operators_;
    size_t index_;
};

#endif
