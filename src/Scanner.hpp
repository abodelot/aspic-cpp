#ifndef ASPIC_SCANNER_HPP
#define ASPIC_SCANNER_HPP

#include "Operators.hpp"
#include "Token.hpp"

#include <unordered_map>
#include <vector>

/**
 * Scanner class transforms input stream into a list of tokens
 * Input is fed to the scanner line by line (see tokenize)
 */
class Scanner
{
public:
    Scanner();

    /**
     * Feed a line to the parser and generate tokens
     * @param expresion: line to be scanned into tokens
     * @return true if no end-of-block keyword is expected after having tokenized
     *   the line, otherwise false (used for Shell mode)
     */
    bool tokenize(const std::string& line);

    /**
     * Clear all tokens
     */
    void clear();

    /**
     * Get a reference to the internal token vector
     */
    const std::vector<Token>& get_tokens() const;

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
     * Eval a string representing an operator
     * @param str: string to be evaluated
     * @param op_type: returned operator type
     * @param previous: token preceding the operator in the expression (or nullptr if operator is the first token)
     * @return true if a token was parsed
     */
    bool parse_operator(const std::string& str, Operator& op_type, const Token* previous) const;

    /**
     * Test if given token precedes a unary operator
     * @param previous: previous tokenkized token (null if none)
     */
    bool precedes_unary_operator(const Token* previous) const;

    // Map operator representation
    typedef std::unordered_map<std::string, Operator> OperatorTable;
    OperatorTable operators_;
    std::vector<Token> tokens_;
    int opened_blocks_;
};

#endif
