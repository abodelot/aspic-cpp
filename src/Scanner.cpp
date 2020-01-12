#include "Scanner.hpp"
#include "Error.hpp"

#include <cctype>
#include <cstring>

#define COMMENT_SYMBOL '#'

Scanner::Scanner():
    opened_blocks_(0)
{
    // Define mapping for operator symbols
    // Multi-part operators () and [] are scanned separately in the tokenize method
    operators_["!"]  = Operator::OP_NOT;

    operators_["**"] = Operator::OP_POW;
    operators_["*"]  = Operator::OP_MULTIPLICATION;
    operators_["/"]  = Operator::OP_DIVISION;
    operators_["%"]  = Operator::OP_MODULO;
    operators_["+"]  = Operator::OP_ADDITION; // see parse_operator for OP_UNARY_PLUS
    operators_["-"]  = Operator::OP_SUBTRACTION; // see parse_operator for OP_UNARY_MINUS

    operators_["<"]  = Operator::OP_LESS_THAN;
    operators_["<="] = Operator::OP_LESS_THAN_OR_EQUAL;
    operators_[">"]  = Operator::OP_GREATER_THAN;
    operators_[">="] = Operator::OP_GREATER_THAN_OR_EQUAL;
    operators_["=="] = Operator::OP_EQUAL;
    operators_["!="] = Operator::OP_NOT_EQUAL;
    operators_["&&"] = Operator::OP_LOGICAL_AND;
    operators_["||"] = Operator::OP_LOGICAL_OR;

    operators_["="]  = Operator::OP_ASSIGNMENT;
    operators_["*="] = Operator::OP_MULTIPLY_AND_ASSIGN;
    operators_["/="] = Operator::OP_DIVIDE_AND_ASSIGN;
    operators_["%="] = Operator::OP_MODULO_AND_ASSIGN;
    operators_["+="] = Operator::OP_ADD_AND_ASSIGN;
    operators_["-="] = Operator::OP_SUBTRACT_AND_ASSIGN;
}

bool Scanner::tokenize(const std::string& line)
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
            Operator op_type;
            if (parse_operator(buffer, op_type, previous)) {
                tokens_.push_back(Token::create_operator(op_type));
            }
            else {
                throw Error::UnknownOperator(buffer);
            }
        }
        // Left paren?
        else if (current == '(') {
            // Can be either grouping or a function call operator, depending on previous token
            if (previous == nullptr
                || previous->get_type() == Token::OPERATOR
                || previous->get_type() == Token::LEFT_PAREN
                || previous->get_type() == Token::ARG_SEPARATOR
                || previous->get_type() == Token::KW_IF) {
                tokens_.push_back(Token(Token::LEFT_PAREN));
            }
            else {
                tokens_.push_back(Token::create_operator(Operator::OP_FUNC_CALL));
            }
            ++unmatched_parentheses;
        }
        // Right paren?
        else if (current == ')') {
            tokens_.push_back(Token(Token::RIGHT_PAREN));
            --unmatched_parentheses;
        }
        // Left bracket?
        else if (current == '[') {
            if (previous == nullptr
                || previous->get_type() == Token::OPERATOR
                || previous->get_type() == Token::LEFT_PAREN
                || previous->get_type() == Token::ARRAY_LITERAL
                || previous->get_type() == Token::MAP_LITERAL
                || previous->get_type() == Token::ARG_SEPARATOR
                || previous->get_type() == Token::COLON
                || previous->get_type() == Token::KW_IF) {
                tokens_.push_back(Token(Token::ARRAY_LITERAL));
            }
            else {
                tokens_.push_back(Token::create_operator(Operator::OP_INDEX));
            }
        }
        // Right bracket?
        else if (current == ']') {
            tokens_.push_back(Token(Token::RIGHT_BRACKET));
        }
        // Left brace?
        else if (current == '{') {
            tokens_.push_back(Token(Token::MAP_LITERAL));
        }
        // Right brace?
        else if (current == '}') {
            tokens_.push_back(Token(Token::RIGHT_BRACE));
        }
        // Arg separator?
        else if (current == ',') {
            tokens_.push_back(Token(Token::ARG_SEPARATOR));
        }
        // Colon?
        else if (current == ':') {
            tokens_.push_back(Token(Token::COLON));
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
            tokens_.push_back(dot_found
                ? Token::create_float(atof(buffer.c_str()))
                : Token::create_int(atoi(buffer.c_str()))
            );
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
                tokens_.push_back(Token::create_null());
            }
            else if (buffer == "if") {
                ++opened_blocks_;
                tokens_.push_back(Token(Token::KW_IF));
            }
            else if (buffer == "elif") {
                tokens_.push_back(Token(Token::KW_ELIF));
            }
            else if (buffer == "else") {
                tokens_.push_back(Token(Token::KW_ELSE));
            }
            else if (buffer == "while") {
                ++opened_blocks_;
                tokens_.push_back(Token(Token::KW_WHILE));
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

void Scanner::clear()
{
    tokens_.clear();
    opened_blocks_ = 0;
}

const std::vector<Token>& Scanner::get_tokens() const
{
    return tokens_;
}

bool Scanner::is_valid_identifier_char(char c) const
{
    return (c >= 'a' && c <= 'z')
        || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9')
        || (c == '_');
}

bool Scanner::is_valid_operator_char(char c) const
{
    const char* ALPHABET = "+-*/%=!<>|&";
    for (const char* p = ALPHABET; *p != '\0'; ++p) {
        if (*p == c) {
            return true;
        }
    }
    return false;
}

bool Scanner::parse_operator(const std::string& str, Operator& op_type, const Token* previous) const
{
    OperatorTable::const_iterator it = operators_.find(str);
    if (it == operators_.end()) {
        return false;
    }
    op_type = it->second;
    // Handle special cases for +/- operators and their unary counterparts
    if (op_type == Operator::OP_ADDITION && precedes_unary_operator(previous)) {
        op_type = Operator::OP_UNARY_PLUS;
    }
    else if (op_type == Operator::OP_SUBTRACTION && precedes_unary_operator(previous)) {
        op_type = Operator::OP_UNARY_MINUS;
    }
    return true;
}

bool Scanner::precedes_unary_operator(const Token* previous) const
{
    /* "+" and "-" are unary operator if they are:
        - preceded by nothing
        - preceded by another operator
        - preceded by a left bracket
        - preceded by an argument separator
    */
    return previous == nullptr
        || previous->get_type() == Token::OPERATOR
        || previous->get_type() == Token::LEFT_PAREN
        || previous->get_type() == Token::ARG_SEPARATOR;
}
