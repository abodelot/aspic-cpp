#include <iostream>
#include <cassert>
#include <cmath>

#include "Token.hpp"
#include "Error.hpp"
#include "OperatorManager.hpp"
#include "Variable.hpp"

// constructors ----------------------------------------------------------------

Token::Token(Type type)
{
	type_ = type;
}


Token Token::create_int(int value)
{
	Token self(INT_LITERAL);
	self.data_.int_value = value;
	return self;
}


Token Token::create_string(const std::string& value)
{
	Token self(STRING_LITERAL);
	self.str_ = value;
	return self;
}


Token Token::create_float(double value)
{
	Token self(FLOAT_LITERAL);
	self.data_.float_value = value;
	return self;
}


Token Token::create_bool(bool value)
{
	Token self(BOOL_LITERAL);
	self.data_.bool_value = value;
	return self;
}


Token Token::create_operator(OperatorType op_type)
{
	Token self(OPERATOR);
	self.data_.op_type = op_type;
	return self;
}


Token Token::create_variable(Variable* variable)
{
	Token self(VARIABLE);
	self.data_.variable = variable;
	return self;
}


Token Token::create_function(SymbolTable::Function func)
{
	Token self(FUNCTION);
	self.data_.function = func;
	return self;
}


// getters by type -------------------------------------------------------------

Token::Type Token::get_type() const
{
	return type_;
}


Token::OperatorType Token::get_operator_type() const
{
	assert(type_ == OPERATOR);
	return data_.op_type;
}


const char* Token::to_str(Type type)
{
	switch (type)
	{
		case INT_LITERAL:
			return "int";
		case FLOAT_LITERAL:
			return "float";
		case BOOL_LITERAL:
			return "bool";
		case STRING_LITERAL:
			return "string";
		default: break;
	}
	return "";
}


// operator helpers ------------------------------------------------------------

bool Token::is_operator() const
{
	return type_ == OPERATOR;
}


bool Token::is_unary_operator() const
{
	// must be an operator
	if (!is_operator())
	{
		return false;
	}
	// switch on unary operators
	switch (data_.op_type)
	{
		case OP_NOT:
		case OP_UNARY_PLUS:
		case OP_UNARY_MINUS:
			return true;
		default:
			return false;
	}
}


bool Token::is_binary_operator() const
{
	return !is_unary_operator(); // thx captain obvious
}


bool Token::is_right_associative_operator() const
{
	// must be an operator
	if (!is_operator())
	{
		return false;
	}
	// all unary operators are right associative
	if (is_unary_operator())
	{
		return true;
	}
	// switch on remaining right associative operators
	switch (data_.op_type)
	{
		case OP_ASSIGNMENT:
		case OP_MULTIPLY_AND_ASSIGN:
		case OP_DIVIDE_AND_ASSIGN:
		case OP_MODULO_AND_ASSIGN:
		case OP_ADD_AND_ASSIGN:
		case OP_SUBTRACT_AND_ASSIGN:
			return true;
		default:
			return false;
	}
}

// operator implementation -----------------------------------------------------

Token Token::apply_unary_operator(Token::OperatorType op)
{
	/*if (type_ == VARIABLE)
	{
		return data_.variable->to_token().apply_unary_operator(op);
	}

	switch (op)
	{
		case OP_UNARY_PLUS:
			switch (type_)
			{
				case INT_LITERAL:
				case BOOL_LITERAL:
					return Token::create_int(+ int_value());
				case FLOAT_LITERAL:
					return Token::create_float(+ float_value());
				default: break;
			}
			break;
		case OP_UNARY_MINUS:
			switch (type_)
			{
				case INT_LITERAL:
				case BOOL_LITERAL:
					return Token::create_int(- int_value());
				case FLOAT_LITERAL:
					return Token::create_float(- float_value());
				default: break;
			}
			break;
		case OP_NOT:
			switch (type_)
			{
				case INT_LITERAL:
				case BOOL_LITERAL:
				case FLOAT_LITERAL:
				case STRING_LITERAL:
					return Token::create_bool(! bool_value());
				default: break;

			}
	}*/

	switch (type_)
	{
		case INT_LITERAL:
		case BOOL_LITERAL:
			switch (op)
			{
				case OP_UNARY_PLUS:
					return Token::create_int(+ int_value());
				case OP_UNARY_MINUS:
					return Token::create_int(- int_value());
				case OP_NOT:
					return Token::create_bool(! bool_value());
				default: break;
			}
			break;
		case FLOAT_LITERAL:
			switch (op)
			{
				case OP_UNARY_PLUS:
					return Token::create_float(+ float_value());
				case OP_UNARY_MINUS:
					return Token::create_float(- float_value());
				case OP_NOT:
					return Token::create_bool(! bool_value());
				default: break;

			}
			break;
		case STRING_LITERAL:
			if (op == OP_NOT)
			{
				return Token::create_bool(! bool_value());
			}
			break;
		case VARIABLE:
			return data_.variable->value().apply_unary_operator(op);
		default:
			break;

	}
	throw Error::UnsupportedOperator(type_, op);
}


Token Token::apply_binary_operator(Token::OperatorType op, Token& operand)
{
	switch (type_)
	{
	case INT_LITERAL:
	case BOOL_LITERAL: // false is equivalent to 0, true is equivalent to 1
		switch (op)
		{
			case OP_POW:
				return Token::create_int(std::pow(int_value(), operand.int_value()));

			case OP_MULTIPLICATION:
				if (operand.is_typed(INT_LITERAL))
					return Token::create_int(int_value() * operand.int_value());

				else if (operand.is_typed(STRING_LITERAL))
					// 2 * "hello" => "hellohello"
					return operand.apply_binary_operator(op, *this);

				break;
			case OP_DIVISION:
			{
				int denominator = operand.int_value();
				if (denominator == 0)
					throw Error::DivideByZero();

				return Token::create_int(int_value() / denominator);
			}
			case OP_MODULO:
			{
				int value = operand.int_value();
				if (value == 0)
					throw Error::DivideByZero();

				return Token::create_int(int_value() % value);
			}
			case OP_ADDITION:
				return Token::create_int(int_value() + operand.int_value());

			case OP_SUBTRACTION:
				return Token::create_int(int_value() - operand.int_value());

			case OP_LESS_THAN:
				return Token::create_bool(int_value() < operand.int_value());

			case OP_LESS_THAN_OR_EQUAL:
				return Token::create_bool(int_value() <= operand.int_value());

			case OP_GREATER_THAN:
				return Token::create_bool(int_value() > operand.int_value());

			case OP_GREATER_THAN_OR_EQUAL:
				return Token::create_bool(int_value() >= operand.int_value());

			case OP_EQUAL:
				return Token::create_bool(int_value() == operand.int_value());

			case OP_NOT_EQUAL:
				return Token::create_bool(int_value() != operand.int_value());

			case OP_LOGICAL_AND:
				return Token::create_bool(bool_value() && operand.bool_value());

			case OP_LOGICAL_OR:
				return Token::create_bool(bool_value() || operand.bool_value());

			default: break;
		}
		break;

	case FLOAT_LITERAL:
		break;

	case STRING_LITERAL:
		switch (op)
		{
			case OP_ADDITION:
				return Token::create_string(str_ + operand.string_value());

			case OP_MULTIPLICATION:
			{
				std::string result;
				int count = operand.int_value();
				for (int i = 0; i < count; ++i)
				{
					result += str_;
				}
				return Token::create_string(result);
			}

			case OP_LESS_THAN:
				return Token::create_bool(str_ < operand.string_value());

			case OP_LESS_THAN_OR_EQUAL:
				return Token::create_bool(str_ <= operand.string_value());

			case OP_GREATER_THAN:
				return Token::create_bool(str_ > operand.string_value());

			case OP_GREATER_THAN_OR_EQUAL:
				return Token::create_bool(str_ >= operand.string_value());

			case OP_EQUAL:
				return Token::create_bool(str_ == operand.string_value());

			case OP_NOT_EQUAL:
				return Token::create_bool(str_ != operand.string_value());

			case OP_LOGICAL_AND:
				return Token::create_bool(bool_value() && operand.bool_value());

			case OP_LOGICAL_OR:
				return Token::create_bool(bool_value() || operand.bool_value());
			default: break;
		}
		break;

	case VARIABLE:
		switch (op)
		{
			// handles operators which update the variable value
			case OP_ASSIGNMENT:
			{
				// do NOT put a variable token in a variable token :)
				if (operand.type_ == VARIABLE)
					data_.variable->set(operand.data_.variable->value());
				else
					data_.variable->set(operand);
				return operand;
			}
			case OP_MULTIPLY_AND_ASSIGN:
			{
				Token result = data_.variable->value().apply_binary_operator(OP_MULTIPLICATION, operand);
				data_.variable->set(result);
				return result;
			}
			case OP_DIVIDE_AND_ASSIGN:
			{
				Token result = data_.variable->value().apply_binary_operator(OP_DIVISION, operand);
				data_.variable->set(result);
				return result;
			}
			case OP_MODULO_AND_ASSIGN:
			{
				Token result = data_.variable->value().apply_binary_operator(OP_MODULO, operand);
				data_.variable->set(result);
				return result;
			}
			case OP_ADD_AND_ASSIGN:
			{
				Token result = data_.variable->value().apply_binary_operator(OP_ADDITION, operand);
				data_.variable->set(result);
				return result;
			}
			case OP_SUBTRACT_AND_ASSIGN:
			{
				Token result = data_.variable->value().apply_binary_operator(OP_SUBTRACTION, operand);
				data_.variable->set(result);
				return result;
			}
			default:
				// variable is not modified, extract value and apply operator on it
				return data_.variable->value().apply_binary_operator(op, operand);
		}
		break;
	default:
		break;
	}

	throw Error::UnsupportedBinaryOperator(type_, operand.type_, op);
}


bool Token::is_literal() const
{
	return type_ == STRING_LITERAL || type_ == INT_LITERAL || type_ || FLOAT_LITERAL || type_ == BOOL_LITERAL;
}

bool Token::is_typed(Type type) const
{
	return type_ == type || (type_ == VARIABLE && data_.variable->value().get_type() == type);
}


// cast ------------------------------------------------------------------------

std::string Token::string_value() const
{
	if (type_ == STRING_LITERAL)
		return str_;

	if (type_ == VARIABLE)
		return data_.variable->value().string_value();

	throw Error::SyntaxError("expected string token");
}



double Token::float_value() const
{
	switch (type_)
	{
		case FLOAT_LITERAL:
			return data_.float_value;
		case VARIABLE:
			return data_.variable->value().float_value();
		default:
			return (double) int_value();
	}
}


int Token::int_value() const
{
	switch (type_)
	{
		case INT_LITERAL:
			return data_.int_value;
		// allow implicit casting for float???
		/*case FLOAT_LITERAL:
			return (int) data_.float_value;*/
		case BOOL_LITERAL:
			return data_.bool_value ? 1 : 0;
		case VARIABLE:
			return data_.variable->value().int_value();
		default:
			throw Error::SyntaxError("expected int token");
	}
}


bool Token::bool_value() const
{
	switch (type_)
	{
		case INT_LITERAL:
			return data_.int_value != 0;
		case FLOAT_LITERAL:
			return data_.float_value != 0.f;
		case STRING_LITERAL:
			return str_.size() > 0;
		case BOOL_LITERAL:
			return data_.bool_value;
		case VARIABLE:
			return data_.variable->value().bool_value();
		default:
			throw Error::SyntaxError("token must be a value");
	}
}

// debug -----------------------------------------------------------------------

void Token::print(std::ostream& os) const
{
	switch (type_)
	{
		case INT_LITERAL:
			os << data_.int_value;
			break;
		case FLOAT_LITERAL:
			os << data_.float_value << "f";
			break;
		case STRING_LITERAL:
			os << '"' << str_ << '"';
			break;
		case BOOL_LITERAL:
			os << (data_.bool_value ? "true" : "false");
			break;
		case VARIABLE:
			// print encapsulated token
			data_.variable->value().print(os);
			break;
		case FUNCTION:
			os << str_ << "()";
			break;
		case OPERATOR:
			os << OperatorManager::to_str(data_.op_type);
			break;
		case LEFT_BRACKET:
			os << '(';
			break;
		case RIGHT_BRACKET:
			os << ')';
			break;
		case ARG_SEPARATOR:
			os << ", ";
			break;
	}
}

