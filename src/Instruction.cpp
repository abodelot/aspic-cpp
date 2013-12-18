#include <iostream>
#include <cctype>
#include <cstring>
#include <cstdlib>

#include "Instruction.hpp"
#include "SymbolTable.hpp"
#include "Error.hpp"
#include "SymbolTable.hpp"
#include "OperatorManager.hpp"


Instruction::Instruction() :
	operators_(OperatorManager::get_instance())
{
}


Token Instruction::eval(const std::string& expression)
{
	// 1. Split string into a token list
	tokenization(expression);

	// 2. Convert tokens from infix notation to postfix notation
	infix_to_postfix();

	// 3. Eval postfixed expression
	return eval_postfix();
}


void Instruction::tokenization(const std::string& expression)
{
	// tokenization: transform an expression string into a token vector
	tokens_.clear();
	static std::string buffer; // buffer is the string representation of the currently parsed token
	int previous_index = -1;
	for (size_t i = 0; i < expression.size(); ++i)
	{
		buffer.clear();
		char current = expression[i];

		// operator?
		if (is_valid_operator_char(current))
		{
			buffer += current;
			for (++i; i < expression.size() && is_valid_operator_char(expression[i]); ++i)
			{
				buffer += expression[i];
			}
			--i;
			Token::OperatorType op_type;
			const Token* previous = previous_index != -1 ? &tokens_[previous_index] : NULL;
			if (operators_.eval(buffer, op_type, previous))
			{
				tokens_.push_back(Token::create_operator(op_type));
			}
			else
			{
				throw Error::UnknownOperator(buffer);
			}
		}
		// left bracket?
		else if (current == '(')
		{
			tokens_.push_back(Token(Token::LEFT_BRACKET));
		}
		// right bracket?
		else if (current == ')')
		{
			tokens_.push_back(Token(Token::RIGHT_BRACKET));
		}
		// arg separator?
		else if (current == ',')
		{
			tokens_.push_back(Token(Token::ARG_SEPARATOR));
		}
		// Scanning int or float literal
		else if (isdigit(current) || current == '.')
		{
			bool dot_found = current == '.';
			buffer += current;
			// tant que c est un chiffre ou un un seul et unique point
			for (++i; i < expression.size() && (isdigit(expression[i]) || (!dot_found && expression[i] == '.')); ++i)
			{
				dot_found |= expression[i] == '.';
				buffer += expression[i];
			}
			--i;
            // ASCII to int, or to float if we've found a dot
			Token t = dot_found ?
				Token::create_float(atof(buffer.c_str())) :
				Token::create_int(atoi(buffer.c_str()));
            tokens_.push_back(t);
		}
		// Scanning string literal
		else if (current == '"' || current == '\'')
		{
			char closure_char = current; // "...", '...'
			bool escape_next_char = false;
			for (++i; i < expression.size(); ++i)
			{
				if (!escape_next_char)
				{
					if (expression[i] == closure_char)
						break;

					if (expression[i] == '\\')
						escape_next_char = true;
					else
						buffer += expression[i];
				}
				else if (escape_next_char)
				{
					switch (expression[i])
					{
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
							// Not a special character, the prevous backslash is kept
							buffer += '\\';
							buffer += expression[i];
							break;
					}
					escape_next_char = false;
				}
			}

			// Ensure string is correctly enclosed
			if (i == expression.size())
			{
				throw Error::SyntaxError("EOL while scanning string literal");
			}

			tokens_.push_back(Token::create_string(buffer));
		}
		// Scanning identifier (function or variable)
		// We already know that current char is not a digit, so no need to check that
		else if (is_valid_identifier_char(current))
		{
			// on lit tous les caractères jusqu'à recomposer l'identifiant
			buffer += current;
			for (++i; i < expression.size() && is_valid_identifier_char(expression[i]); ++i)
			{
				buffer += expression[i];
			}
			--i;

			// TODO: allow variables to shadow function defintions?
			const Function* func = SymbolTable::get_function(buffer);
			if (func != NULL)
			{
				tokens_.push_back(Token::create_function(func));
			}
			else
			{
				// If a variable named after the buffer value doesn't exist, SymbolTable will create it
				Variable* var = SymbolTable::get_variable(buffer);
				tokens_.push_back(Token::create_variable(var));
			}
        }
        // Ignore whitespaces
        else if (current != ' ' && current != '\t')
        {
        	throw Error::SyntaxError(std::string("illegal character encountered: ") + current);
        }

		previous_index = tokens_.size() - 1;
	}
}


void Instruction::infix_to_postfix()
{
	postfix_.clear();
	stack_.clear();

	// transform infix to postfix notation, using shunting-yard algorithm
	for (size_t i = 0; i < tokens_.size(); ++i)
	{
		Token& current = tokens_[i];

		switch (current.get_type())
		{
		case Token::OPERATOR:
			/*
			while there is an operator token 'op' at the top of the stack, and
				either current is left-associative and its precedence is less than or equal to that of 'op'
				or current is right-associative and its precedence is less than that of 'op'
			*/
			while (!stack_.empty() && stack_.top().is_operator() &&
				((!current.is_right_associative_operator() && operators_.compare(current, stack_.top()) <= 0)
				 || (current.is_right_associative_operator() && operators_.compare(current, stack_.top()) < 0)))
			{
				Token& op = stack_.top();
				stack_.pop();
				postfix_.push_back(op);
			}
			stack_.push(current);
			break;

		case Token::LEFT_BRACKET:
			stack_.push(current);
			break;

		case Token::RIGHT_BRACKET:
		{
			bool bracket_found = false;
			// Until the token at the top of the stack is a left parenthesis,
			// pop operators off the stack onto the output queue
			while (!stack_.empty())
			{
				if (stack_.top().get_type() == Token::LEFT_BRACKET)
				{
					bracket_found = true;
					break;
				}
				else
				{
					Token& t = stack_.top();
					postfix_.push_back(t);
					stack_.pop();
				}
			}
			// If the stack runs out without finding a left parenthesis, then there are mismatched parentheses.
			if (!bracket_found)
			{
				throw Error::SyntaxError("a left bracket is missing");
			}
			// Pop the left parenthesis from the stack, but not onto the output queue.
			stack_.pop();
			// If the token at the top of the stack is a function token, pop it onto the output queue.
			if (!stack_.empty() && stack_.top().get_type() == Token::FUNCTION)
			{
				Token& t = stack_.top();
				postfix_.push_back(t);
				stack_.pop();
			}
			break;
		}

		case Token::INT_LITERAL:
		case Token::FLOAT_LITERAL:
		case Token::STRING_LITERAL:
		case Token::BOOL_LITERAL:
		case Token::VARIABLE:
			postfix_.push_back(current);
			break;

		case Token::FUNCTION:
			stack_.push(current);
			break;

        case Token::ARG_SEPARATOR:
		{
			bool bracket_found = false;
			// Until the token at the top of the stack is a left parenthesis,
			// pop operators off the stack onto the output queue
			while (!stack_.empty())
			{
				if (stack_.top().get_type() == Token::LEFT_BRACKET)
				{
					bracket_found = true;
					break;
				}
				else
				{
					Token& t = stack_.top();
					postfix_.push_back(t);
					stack_.pop();
				}
			}
			// If no left parentheses are encountered, either the separator
			// was misplaced or parentheses were mismatched.
			if (!bracket_found)
			{
				throw Error::SyntaxError("a left bracket is missing");
			}
			break;
		}
		default:
			break;
		}
	}

	while (!stack_.empty())
	{
		// la pile ne doit contenir plus que des opérateurs
		Token& t = stack_.top();
		if (t.is_operator())
		{
			postfix_.push_back(t);
		}
		else
		{
			throw Error::SyntaxError("a right bracket is missing");
		}
		stack_.pop();
	}
}


Token Instruction::eval_postfix()
{
	stack_.clear();

	for (TokenList::const_iterator it = postfix_.begin(); it != postfix_.end(); ++it)
	{
		const Token& tok = *it;
		switch (tok.get_type())
		{
			case Token::INT_LITERAL:
			case Token::FLOAT_LITERAL:
			case Token::STRING_LITERAL:
			case Token::VARIABLE:
				stack_.push(tok);
				break;

			case Token::OPERATOR:
				if (tok.is_binary_operator())
				{
					// binary operator -> 2 operands
					if (stack_.size() < 2)
					{
						throw Error::SyntaxError("binary operator needs two operands");
					}
					Token operand2 = stack_.top(); stack_.pop();
					Token operand1 = stack_.top(); stack_.pop();
					// on applique l'opérateur binaire sur les deux opérandes et on rempile le résultat
					stack_.push(operand1.apply_binary_operator(tok.get_operator_type(), operand2));
				}
				else
				{
					// unary operator -> 1 operand
					if (stack_.size() < 1)
					{
						throw Error::SyntaxError("unary operator needs one operand");
					}
					Token operand = stack_.top(); stack_.pop();
					// on applique l'opérateur unaire sur l'opérande et on rempile le résultat
					stack_.push(operand.apply_unary_operator(tok.get_operator_type()));
				}
				break;

			case Token::FUNCTION:
				// Execute function with stack as arguments, and push back the result on the stack
				stack_.push(tok.get_function()->ptr(stack_));
				break;

			default:
				throw Error::InternalError("illegal token in postfix expression");
				break;
		}
	}
	if (stack_.empty())
	{
		throw Error::InternalError("postfixed expression didn't yield any result");
	}
	else if (stack_.size() > 1)
	{
		throw Error::SyntaxError("too many arguments or operands");
	}
	return stack_.top();
}


bool Instruction::is_valid_identifier_char(char c) const
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_');
}


bool Instruction::is_valid_operator_char(char c) const
{
	const char* ALPHABET = "+-*/%=!<>|&";
	for (const char* p = ALPHABET; *p != '\0'; ++p)
		if (*p == c)
			return true;

	return false;
}


void Instruction::debug() const
{
	std::cout << "infix:   ";
	print_tokens(tokens_);

	std::cout << "postfix: ";
	print_tokens(postfix_);
}


void Instruction::print_tokens(const TokenList& t) const
{
    for (TokenList::const_iterator it = t.begin(); it != t.end(); ++it)
    {
		it->debug(std::cout);
		std::cout << "  ";
    }
	std::cout << std::endl;
}
