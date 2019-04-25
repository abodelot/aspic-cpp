#include <iostream>
#include <cctype>
#include <cstring>
#include <cstdlib>

#include "Parser.hpp"
#include "SymbolTable.hpp"
#include "Error.hpp"
#include "TokenStack.hpp"
#include "OperatorManager.hpp"

#define COMMENT_SYMBOL '#'


Parser::Parser() :
    operators_(OperatorManager::get_instance()),
    index_(0),
    opened_blocks_(0)
{
}

bool Parser::tokenize(const std::string& line)
{
    // Utility buffer for parsing multi-chars tokens
    static std::string buffer;

    const Token* previous = nullptr;
    int unmatched_parentheses = 0;
    for (size_t i = 0; i < line.size(); ++i) {
        char current = line[i];

        // operator?
        if (is_valid_operator_char(current)) {
            size_t start = i++;
            while (i < line.size() && is_valid_operator_char(line[i])) {
                ++i;
            }
            buffer = line.substr(start, i - start);
            --i;
            Token::OperatorType op_type;
            if (operators_.eval(buffer, op_type, previous)) {
                tokens_.push_back(Token::create_operator(op_type));
            }
            else {
                throw Error::UnknownOperator(buffer);
            }
        }
        // Left parenthesis?
        else if (current == '(') {
            // Can be either grouping or a function call operator, depending on previous token
            if (previous == nullptr
                || previous->get_type() == Token::OPERATOR
                || previous->get_type() == Token::LEFT_PARENTHESIS
                || previous->get_type() == Token::KW_IF) {
                tokens_.push_back(Token(Token::LEFT_PARENTHESIS));
            }
            else {
                tokens_.push_back(Token::create_operator(Token::OP_FUNC_CALL));
            }
            ++unmatched_parentheses;
        }
        // Right parenthesis?
        else if (current == ')') {
            tokens_.push_back(Token(Token::RIGHT_PARENTHESIS));
            --unmatched_parentheses;
        }
        // Left bracket?
        else if (current == '[') {
            tokens_.push_back(Token::create_operator(Token::OP_INDEX));
        }
        // Right bracket?
        else if (current == ']') {
            tokens_.push_back(Token(Token::RIGHT_BRACKET));
        }
        // Arg separator?
        else if (current == ',') {
            tokens_.push_back(Token(Token::ARG_SEPARATOR));
        }
        // Scanning int or float literal
        else if (isdigit(current) || current == '.') {
            size_t start = i++;
            bool dot_found = current == '.';
            // While char is a digit, and no more than one dot '.' has been found
            while (i < line.size() && (isdigit(line[i]) || (!dot_found && line[i] == '.'))) {
                dot_found |= line[i] == '.';
                ++i;
            }
            buffer = line.substr(start, i - start);
            // ASCII to int, or to float if we've found a dot
            Token t = dot_found
                ? Token::create_float(atof(buffer.c_str()))
                : Token::create_int(atoi(buffer.c_str()));
            tokens_.push_back(t);
            --i;
        }
        // Scanning string literal
        else if (current == '"' || current == '\'') {
            // The closing quote must match the opening quote (single or double)
            char closure_char = current;
            bool escape_next_char = false;
            buffer.clear();
            // Swallow chars into buffer until end of string is reached
            for (++i; i < line.size(); ++i) {
                if (!escape_next_char) {
                    if (line[i] == closure_char) {
                        break;
                    }

                    if (line[i] == '\\') {
                        escape_next_char = true;
                    }
                    else {
                        buffer += line[i];
                    }
                }
                else {
                    switch (line[i]) {
                        case '\\': buffer += '\\'; break; // Backslash (\)
                        case '\'': buffer += '\''; break; // Single quote (')
                        case '\"': buffer += '\"'; break; // Double quote (")
                        case 'a':  buffer += '\a'; break; // ASCII Bell (BEL)
                        case 'b':  buffer += '\b'; break; // ASCII Backspace (BS)
                        case 'f':  buffer += '\f'; break; // ASCII Formfeed (FF)
                        case 'n':  buffer += '\n'; break; // ASCII Linefeed (LF)
                        case 'r':  buffer += '\r'; break; // ASCII Carriage Return (CR)
                        case 't':  buffer += '\t'; break; // ASCII Horizontal Tab (TAB)
                        case 'v':  buffer += '\v'; break; // ASCII Vertical Tab (VT)
                        default:
                            // Not a special character, the previous backslash is kept
                            buffer += '\\';
                            buffer += line[i];
                            break;
                    }
                    escape_next_char = false;
                }
            }

            // Ensure string is correctly enclosed
            if (i == line.size()) {
                throw Error::SyntaxError("EOL while scanning string literal");
            }

            tokens_.push_back(Token::create_string(buffer));
        }
        // Scanning identifiers (functions, variables, and reserved literal keywords)
        // We already know that current char is not a digit, so no need to check that
        else if (is_valid_identifier_char(current)) {
            // Increment i until the end of the identifier is reached
            size_t start = i++;
            while (i < line.size() && is_valid_identifier_char(line[i])) {
                ++i;
            }
            buffer = line.substr(start, i - start);
            --i;

            // Check if identifier is a reserved keyword
            if (buffer == "true") {
                tokens_.push_back(Token::create_bool(true));
            }
            else if (buffer == "false") {
                tokens_.push_back(Token::create_bool(false));
            }
            else if (buffer == "null") {
                tokens_.push_back(Token(Token::NULL_VALUE));
            }
            else if (buffer == "if") {
                ++opened_blocks_;
                tokens_.push_back(Token(Token::KW_IF));
            }
            else if (buffer == "else") {
                tokens_.push_back(Token(Token::KW_ELSE));
            }
            else if (buffer == "end") {
                --opened_blocks_;
                tokens_.push_back(Token(Token::KW_END));
            }
            else {
                // Not a keyword, create an identifier
                tokens_.push_back(Token::create_identifier(buffer));
            }
        }
        // Ignore everything after comment symbol
        else if (current == COMMENT_SYMBOL) {
            break;
        }
        // Ignore whitespaces
        else if (current != ' ' && current != '\t') {
            throw Error::SyntaxError(std::string("illegal character encountered: ") + current);
        }

        // Imporant: do no use previous after tokens_.push_back!
        previous = &(tokens_.back());
    }

    // Each expression must end with special END_EXPR token
    if (tokens_.size() > 0) {
        if (tokens_.back().get_type() != Token::END_EXPR && tokens_.back().get_type() != Token::KW_ELSE) {
            tokens_.push_back(Token::Token::END_EXPR);
        }
    }
    return opened_blocks_ == 0;
}

void Parser::build_ast()
{
    index_ = 0;
    ast_.setRoot(parse_block());
}

void Parser::print_tokens() const
{
    std::cout << "lexer: ";
    for (const auto& token: tokens_) {
        token.debug(std::cout);
        std::cout << ' ';
    }
    std::cout << std::endl;
}

void Parser::print_ast() const
{
    ast_.print();
}

Token Parser::eval_ast() const
{
    return ast_.eval();
}

void Parser::reset()
{
    tokens_.clear();
    ast_.clear();
    index_ = 0;
    opened_blocks_ = 0;
}

AST::Node* Parser::parse(int rbp)
{
    // See Pratt parser algorithm
    Token* t = &tokens_[index_++];
    AST::Node* left = null_denotation(*t);
    while (rbp < tokens_[index_].lbp) {
        t = &tokens_[index_++];
        left = left_denotation(*t, left);
    }
    return left;
}

AST::BodyNode* Parser::parse_block()
{
    AST::BodyNode* body = new AST::BodyNode(parse(0));
    advance(Token::END_EXPR);
    // Parse expressions until an end-of-block token is found
    while ((index_ + 1) < tokens_.size() &&
           (tokens_[index_].get_type() != Token::KW_END &&
            tokens_[index_].get_type() != Token::KW_ELSE)) {
        body->append(parse(0));
        advance(Token::END_EXPR);
    }
    return body;
}

AST::Node* Parser::null_denotation(Token& token)
{
    if (token.is_value() || token.get_type() == Token::IDENTIFIER) {
        return new AST::ValueNode(token);
    }
    if (token.get_type() == Token::KW_IF) {
        // Parse test expression
        const AST::Node* test = parse(0);
        advance(Token::END_EXPR);
        // Parse if body
        const AST::Node* body_true = parse_block();
        const AST::Node* body_false = nullptr;

        // Check for optional else block
        if (tokens_[index_].get_type() == Token::KW_ELSE) {
            ++index_;
            body_false = parse_block();
        }
        advance(Token::KW_END);
        return new AST::IfNode(test, body_true, body_false);
    }
    if (token.get_type() == Token::LEFT_PARENTHESIS) {
        AST::Node* next = parse(0);
        advance(Token::RIGHT_PARENTHESIS);
        return next;
    }
    if (token.get_type() == Token::OPERATOR) {
        AST::Node* right = parse(token.is_right_associative_operator() ? token.lbp - 1 : token.lbp);
        return new AST::UnaryOpNode(token.get_operator_type(), right);
    }
    throw Error::UnexpectedToken(token);
}

AST::Node* Parser::left_denotation(Token& token, AST::Node* left)
{
    if (token.get_type() == Token::OPERATOR) {
        if (token.get_operator_type() == Token::OP_FUNC_CALL) {
            AST::FuncCallNode* node = new AST::FuncCallNode(left);
            // Find arguments until matching right parenthesis
            if (tokens_[index_].get_type() != Token::RIGHT_PARENTHESIS) {
                while (true) {
                    node->push_arg(parse(0));
                    if (tokens_[index_].get_type() != Token::ARG_SEPARATOR) {
                        break;
                    }
                    advance(Token::ARG_SEPARATOR);
                }
            }
            advance(Token::RIGHT_PARENTHESIS);
            return node;
        }
        else if (token.get_operator_type() == Token::OP_INDEX) {
            AST::Node* right = parse(0);
            advance(Token::RIGHT_BRACKET);
            return new AST::BinaryOpNode(Token::OP_INDEX, left, right);
        }
        else {
            AST::Node* right = parse(token.is_right_associative_operator() ? token.lbp - 1 : token.lbp);
            return new AST::BinaryOpNode(token.get_operator_type(), left, right);
        }
    }
    else {
        throw Error::InternalError("expected operator");
    }
}

void Parser::advance(Token::Type type)
{
    if (index_ >= tokens_.size()) {
        throw Error::InternalError("unexpected end of input");
    }
    if (tokens_[index_].get_type() != type) {
        throw Error::UnexpectedTokenType(type, tokens_[index_].get_type());
    }
    ++index_;
}

bool Parser::is_valid_identifier_char(char c) const
{
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9')
        || (c == '_');
}

bool Parser::is_valid_operator_char(char c) const
{
    const char* ALPHABET = "+-*/%=!<>|&";
    for (const char* p = ALPHABET; *p != '\0'; ++p) {
        if (*p == c) {
            return true;
        }
    }
    return false;
}
