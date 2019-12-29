#include "Parser.hpp"
#include "SymbolTable.hpp"
#include "Error.hpp"
#include "Operators.hpp"
#include "ast/Node.hpp"

#include <iostream>


Parser::Parser():
    tokens_(scanner_.get_tokens()),
    index_(0)
{
}

bool Parser::tokenize(const std::string& line)
{
    return scanner_.tokenize(line);
}

void Parser::build_ast()
{
    index_ = 0;
    if (tokens_.size() > 0) {
        ast_.setRoot(parse_block());
    }
}

void Parser::print_ast() const
{
    ast_.print();
}

Object Parser::eval_ast() const
{
    return ast_.eval();
}

void Parser::reset()
{
    scanner_.clear();
    ast_.clear();
    index_ = 0;
}

ast::Node* Parser::parse(int rbp)
{
    const Token* t = &tokens_[index_++];
    ast::Node* left = null_denotation(*t);
    while (rbp < tokens_[index_].lbp) {
        t = &tokens_[index_++];
        left = left_denotation(*t, left);
    }
    return left;
}

ast::BodyNode* Parser::parse_block()
{
    ast::BodyNode* body = new ast::BodyNode(parse(0));
    advance(Token::END_EXPR);
    // Parse expressions until an end-of-block token is found
    while ((index_ + 1) < tokens_.size() &&
           (tokens_[index_].get_type() != Token::KW_END &&
            tokens_[index_].get_type() != Token::KW_ELIF &&
            tokens_[index_].get_type() != Token::KW_ELSE)) {
        body->append(parse(0));
        advance(Token::END_EXPR);
    }
    return body;
}

ast::Node* Parser::null_denotation(const Token& token)
{
    switch (token.get_type()) {
        case Token::VALUE:
            return new ast::ValueNode(token.get_object());

        case Token::ARRAY_LITERAL:
        {
            ast::ArrayLiteralNode* node = new ast::ArrayLiteralNode();
            if (tokens_[index_].get_type() != Token::RIGHT_BRACKET) {
                while (true) {
                    node->add_value(parse(0));
                    if (tokens_[index_].get_type() != Token::ARG_SEPARATOR) {
                        break;
                    }
                    advance(Token::ARG_SEPARATOR);
                }
            }
            advance(Token::RIGHT_BRACKET);
            return node;
        }

        case Token::IDENTIFIER:
            return new ast::ValueNode(Object::create_reference(token.get_id_hash()));

        case Token::KW_IF:
        {
            // Parse test expression
            const ast::Node* test = parse(0);
            advance(Token::END_EXPR);
            // Parse "if" block
            const ast::Node* if_block = parse_block();
            ast::IfNode* if_node = new ast::IfNode(test, if_block);
            ast::IfNode* last_if = if_node;

            // Parse optional "elif" blocks
            while (tokens_[index_].get_type() == Token::KW_ELIF) {
                ++index_;
                const ast::Node* elif_test = parse(0);
                advance(Token::END_EXPR);
                const ast::Node* elif_body = parse_block();

                // Chain new "elif" node to previous "if" or "elif" node
                ast::IfNode* elif = new ast::IfNode(elif_test, elif_body);
                last_if->set_else_block(elif);
                last_if = elif;
            }

            // Parse optional else block
            if (tokens_[index_].get_type() == Token::KW_ELSE) {
                ++index_;
                last_if->set_else_block(parse_block());
            }
            advance(Token::KW_END);
            return if_node;
        }
        case Token::KW_WHILE:
        {
            // Parse test expression
            const ast::Node* test = parse(0);
            advance(Token::END_EXPR);
            // Parse body
            const ast::Node* body = parse_block();
            advance(Token::KW_END);
            return new ast::LoopNode(test, body);
        }
        case Token::LEFT_PARENTHESIS:
        {
            ast::Node* next = parse(0);
            advance(Token::RIGHT_PARENTHESIS);
            return next;
        }
        case Token::OPERATOR:
        {
            Operator op = token.get_operator_type();
            ast::Node* right = parse(Operators::is_right_associative(op) ? token.lbp - 1 : token.lbp);
            return new ast::UnaryOpNode(op, right);
        }
        default:
            break;
    }
    throw Error::UnexpectedToken(token.get_type());
}

ast::Node* Parser::left_denotation(const Token& token, const ast::Node* left)
{
    // Current token MUST be an operator
    if (token.get_type() == Token::OPERATOR) {
        Operator op = token.get_operator_type();
        if (op == Operator::OP_FUNC_CALL) {
            ast::FuncCallNode* node = new ast::FuncCallNode(left);
            // Find arguments until matching right parenthesis
            if (tokens_[index_].get_type() != Token::RIGHT_PARENTHESIS) {
                while (true) {
                    node->add_arg(parse(0));
                    if (tokens_[index_].get_type() != Token::ARG_SEPARATOR) {
                        break;
                    }
                    advance(Token::ARG_SEPARATOR);
                }
            }
            advance(Token::RIGHT_PARENTHESIS);
            return node;
        }
        else if (op == Operator::OP_INDEX) {
            ast::Node* right = parse(0);
            advance(Token::RIGHT_BRACKET);
            return new ast::BinaryOpNode(Operator::OP_INDEX, left, right);
        }
        else {
            ast::Node* right = parse(Operators::is_right_associative(op) ? token.lbp - 1 : token.lbp);
            return new ast::BinaryOpNode(op, left, right);
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
