#ifndef ASPIC_PARSER_HPP
#define ASPIC_PARSER_HPP

#include "Scanner.hpp"
#include "Token.hpp"
#include "ast/Tree.hpp"

#include <string>
#include <vector>

namespace ast { class Node; }

/**
 * Aspic entry-point:
 * 1) tokenize input (see tokenize)
 * 2) build AST from tokens (see build_ast)
 * 3) evaluate AST (see eval_ast)
 */
class Parser
{
public:
    Parser();

    /**
     * Tokenize input
     */
    bool tokenize(const std::string& line);

    /**
     * Generate AST from tokens.
     */
    void build_ast();

    /**
     * Print internal AST to stdout (debug)
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
     * Parse a single expression
     * @return AST root node of expression
     */
    ast::Node* parse(int rbp);

    /**
     * Parse expressions until end of block is met
     * @return AST body node containing a list of expressions
     */
    ast::BodyNode* parse_block();

    void advance(Token::Type type);

    /**
     * Parse method when token appears at the beginning of a language construct
     */
    ast::Node* null_denotation(const Token& current);

    /**
     * Parse method when token appears inside the construct
     */
    ast::Node* left_denotation(const Token& current, const ast::Node* left);

    Scanner scanner_;
    const std::vector<Token>& tokens_;
    ast::Tree ast_;
    size_t index_;
};

#endif
