#include <iostream>
#include <cctype>
#include <cstring>
#include <cstdlib>

#include "Instruction.hpp"
#include "SymbolTable.hpp"
#include "Error.hpp"
#include "TokenStack.hpp"
#include "OperatorManager.hpp"

#define COMMENT_SYMBOL '#'


Instruction::Instruction() :
    operators_(OperatorManager::get_instance()),
    index_(0)
{
}

bool Instruction::tokenize(const std::string& expression)
{
    // Utility buffer for parsing multi-chars tokens
    static std::string buffer;

    tokens_.clear();
    for (size_t i = 0; i < expression.size(); ++i) {
        char current = expression[i];

        // operator?
        if (is_valid_operator_char(current)) {
            size_t start = i++;
            while (i < expression.size() && is_valid_operator_char(expression[i])) {
                ++i;
            }
            buffer = expression.substr(start, i - start);
            --i;
            Token::OperatorType op_type;
            const Token* previous = tokens_.empty() ? nullptr : &(tokens_.back());
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
            const Token* previous = tokens_.empty() ? nullptr : &(tokens_.back());
            if (previous == nullptr
                || previous->get_type() == Token::OPERATOR
                || previous->get_type() == Token::LEFT_PARENTHESIS) {
                tokens_.push_back(Token(Token::LEFT_PARENTHESIS));
            }
            else {
                tokens_.push_back(Token::create_operator(Token::OP_FUNC_CALL));
            }
        }
        // Right parenthesis?
        else if (current == ')') {
            tokens_.push_back(Token(Token::RIGHT_PARENTHESIS));
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
            while (i < expression.size() && (isdigit(expression[i]) || (!dot_found && expression[i] == '.'))) {
                dot_found |= expression[i] == '.';
                ++i;
            }
            buffer = expression.substr(start, i - start);
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
            for (++i; i < expression.size(); ++i) {
                if (!escape_next_char) {
                    if (expression[i] == closure_char) {
                        break;
                    }

                    if (expression[i] == '\\') {
                        escape_next_char = true;
                    }
                    else {
                        buffer += expression[i];
                    }
                }
                else {
                    switch (expression[i]) {
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
                            buffer += expression[i];
                            break;
                    }
                    escape_next_char = false;
                }
            }

            // Ensure string is correctly enclosed
            if (i == expression.size()) {
                throw Error::SyntaxError("EOL while scanning string literal");
            }

            tokens_.push_back(Token::create_string(buffer));
        }
        // Scanning identifiers (functions, variables, and reserved literal keywords)
        // We already know that current char is not a digit, so no need to check that
        else if (is_valid_identifier_char(current)) {
            // Increment i until the end of the identifier is reached
            size_t start = i++;
            while (i < expression.size() && is_valid_identifier_char(expression[i])) {
                ++i;
            }
            buffer = expression.substr(start, i - start);
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
            else {
                // Not a keyword, create an identifier
                tokens_.push_back(Token::create_identifier(buffer));
            }
        }
        // Ignore everything after comment symbol
        else if (current == COMMENT_SYMBOL) {
            i = expression.size();
        }
        // Ignore whitespaces
        else if (current != ' ' && current != '\t') {
            throw Error::SyntaxError(std::string("illegal character encountered: ") + current);
        }
    }

    // At least one token should be parsed for tokenization to be successfull
    return tokens_.size() > 0;
}

Token Instruction::eval(const std::string& input)
{
    if (tokenize(input)) {
        tokens_.push_back(Token(Token::END));
        index_ = 0;
        return parse(0);
    }
    else {
        return Token(Token::NULL_VALUE);
    }
}

Token Instruction::parse(int rbp)
{
    // See Pratt parser algorithm
    Token* t = &tokens_[index_++];
    Token left = null_denotation(*t);
    while (rbp < tokens_[index_].lbp) {
        t = &tokens_[index_++];
        left = left_denotation(*t, left);
    }
    return left;
}

Token Instruction::null_denotation(Token& token)
{
    if (token.is_literal() || token.get_type() == Token::IDENTIFIER) {
        return token;
    }
    if (token.get_type() == Token::LEFT_PARENTHESIS) {
        Token next = parse(0);
        advance(Token::RIGHT_PARENTHESIS);
        return next;
    }
    if (token.get_type() == Token::OPERATOR) {
        Token right = parse(token.is_right_associative_operator() ? token.lbp - 1 : token.lbp);
        return right.apply_unary_operator(token.get_operator_type());
    }
    throw Error::SyntaxError("missing value");
}

Token Instruction::left_denotation(Token& token, Token& left)
{
    if (token.get_type() == Token::OPERATOR) {
        if (token.get_operator_type() == Token::OP_FUNC_CALL) {
            TokenStack args;
            if (tokens_[index_].get_type() != Token::RIGHT_PARENTHESIS) {
                while (true) {
                    args.push(parse(0));
                    if (tokens_[index_].get_type() != Token::ARG_SEPARATOR) {
                        break;
                    }
                    advance(Token::ARG_SEPARATOR);
                }
            }
            advance(Token::RIGHT_PARENTHESIS);
            return left.get_function()(args);
        }
        else if (token.get_operator_type() == Token::OP_INDEX) {
            Token right = parse(0);
            advance(Token::RIGHT_BRACKET);
            return left.apply_binary_operator(Token::OP_INDEX, right);
        }
        else {
            Token right = parse(token.is_right_associative_operator() ? token.lbp - 1 : token.lbp);
            return left.apply_binary_operator(token.get_operator_type(), right);
        }
    }
    else {
        throw Error::InternalError("expected operator");
    }
}

void Instruction::advance(Token::Type type)
{
    if (tokens_[index_].get_type() != type) {
        throw Error::UnexpectedTokenType(type, tokens_[index_].get_type());
    }
    ++index_;
}

bool Instruction::is_valid_identifier_char(char c) const
{
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9')
        || (c == '_');
}

bool Instruction::is_valid_operator_char(char c) const
{
    const char* ALPHABET = "+-*/%=!<>|&";
    for (const char* p = ALPHABET; *p != '\0'; ++p) {
        if (*p == c) {
            return true;
        }
    }
    return false;
}

void Instruction::debug() const
{
    std::cout << "lexer:   ";
    print_tokens(tokens_);
}

void Instruction::print_tokens(const TokenList& t) const
{
    for (TokenList::const_iterator it = t.begin(); it != t.end(); ++it) {
        it->debug(std::cout);
        std::cout << "  ";
    }
    std::cout << std::endl;
}
