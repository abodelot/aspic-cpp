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
     * Feed a line to the parser and generate tokens
     * @param expresion: line to be scanned into tokens
     * @return true if no end-of-block keyword is expected after having tokenized
     *   the line, otherwise false (used for Shell mode)
     */
    bool tokenize(const std::string& expression);

    /**
     * Print scanned tokens to stdout (debug)
     */
    void print_tokens() const;

    /**
     * Print internal AST to stdout (debug)
     */
    void print_ast() const;

    /**
     * Generate AST from tokens.
     */
    void build_ast();

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
     * Test if a character is a valid part of an identifier
     */
    bool is_valid_identifier_char(char c) const;

    /**
     * Test if a character is a valid part of an operator
     */
    bool is_valid_operator_char(char c) const;

    /**
     * Parse a single expression
     * @return AST root node of expression
     */
    AST::Node* parse(int rbp);

    /**
     * Parse expressions until end of block is met
     * @return AST body node containing a list of expressions
     */
    AST::BodyNode* parse_block();

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
    int opened_blocks_;
};

#endif
