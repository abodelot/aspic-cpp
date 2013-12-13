#include <iostream>
#include <cassert>
#include <cmath>

#include "Token.hpp"
#include "Variable.hpp"
#include "Error.hpp"
#include "OperatorManager.hpp"
#include "SymbolTable.hpp"

// helpers ---------------------------------------------------------------------

const char* Token::type_to_str(Type type)
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

// constructors ----------------------------------------------------------------

Token::Token(Type type):
	type_(type)
{
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


Token Token::create_function(const Function* func)
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


Token Token::exec_function(std::stack<Token>& args) const
{
	return (data_.function->ptr)(args);
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
		case OP_POW:
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
	switch (type_)
	{
		case INT_LITERAL:
		case BOOL_LITERAL:
			switch (op)
			{
				case OP_UNARY_PLUS:
					return Token::create_int(+ as_int());
				case OP_UNARY_MINUS:
					return Token::create_int(- as_int());
				case OP_NOT:
					return Token::create_bool(! as_bool());
				default: break;
			}
			break;
		case FLOAT_LITERAL:
			switch (op)
			{
				case OP_UNARY_PLUS:
					return Token::create_float(+ as_float());
				case OP_UNARY_MINUS:
					return Token::create_float(- as_float());
				case OP_NOT:
					return Token::create_bool(! as_bool());
				default: break;

			}
			break;
		case STRING_LITERAL:
			if (op == OP_NOT)
			{
				return Token::create_bool(! as_bool());
			}
			break;
		case VARIABLE:
			return data_.variable->get().apply_unary_operator(op);
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

		if (operand.is_typed(FLOAT_LITERAL))
		{
			// if the other operand is float typed, the result will be also float typed
			Token cast_to_float = Token::create_float(as_float());
			return cast_to_float.apply_binary_operator(op, operand);
		}

		switch (op)
		{
			case OP_POW:
				return Token::create_int(std::pow(as_int(), operand.as_int()));

			case OP_MULTIPLICATION:
				// 2 * "hello" => "hellohello"
				if (operand.is_typed(STRING_LITERAL))
					return operand.apply_binary_operator(op, *this);

				return Token::create_int(as_int() * operand.as_int());

			case OP_DIVISION:
			{
				int denominator = operand.as_int();
				if (denominator == 0)
					throw Error::DivideByZero();

				return Token::create_int(as_int() / denominator);
			}
			case OP_MODULO:
			{
				int denominator = operand.as_int();
				if (denominator == 0)
					throw Error::DivideByZero();

				return Token::create_int(as_int() % denominator);
			}
			case OP_ADDITION:
				return Token::create_int(as_int() + operand.as_int());

			case OP_SUBTRACTION:
				return Token::create_int(as_int() - operand.as_int());

			case OP_LESS_THAN:
				return Token::create_bool(as_int() < operand.as_int());

			case OP_LESS_THAN_OR_EQUAL:
				return Token::create_bool(as_int() <= operand.as_int());

			case OP_GREATER_THAN:
				return Token::create_bool(as_int() > operand.as_int());

			case OP_GREATER_THAN_OR_EQUAL:
				return Token::create_bool(as_int() >= operand.as_int());

			case OP_EQUAL:
				return Token::create_bool(as_int() == operand.as_int());

			case OP_NOT_EQUAL:
				return Token::create_bool(as_int() != operand.as_int());

			case OP_LOGICAL_AND:
				return Token::create_bool(as_bool() && operand.as_bool());

			case OP_LOGICAL_OR:
				return Token::create_bool(as_bool() || operand.as_bool());

			default: break;
		}
		break;

	case FLOAT_LITERAL:
		switch (op)
		{
			case OP_POW:
				return Token::create_float(std::pow(as_float(), operand.as_float()));

			case OP_MULTIPLICATION:
				return Token::create_float(as_float() * operand.as_float());

			case OP_DIVISION:
			{
				float denominator = operand.as_float();
				if (denominator == 0.f)
					throw Error::DivideByZero();

				return Token::create_float(as_float() / denominator);
			}
			case OP_MODULO:
			{
				double denominator = operand.as_float();
				if (denominator == 0.f)
					throw Error::DivideByZero();

				return Token::create_float(fmod(as_float(), denominator));
			}
			case OP_ADDITION:
				return Token::create_float(as_float() + operand.as_float());

			case OP_SUBTRACTION:
				return Token::create_float(as_float() - operand.as_float());

			case OP_LESS_THAN:
				return Token::create_bool(as_float() < operand.as_float());

			case OP_LESS_THAN_OR_EQUAL:
				return Token::create_bool(as_float() <= operand.as_float());

			case OP_GREATER_THAN:
				return Token::create_bool(as_float() > operand.as_float());

			case OP_GREATER_THAN_OR_EQUAL:
				return Token::create_bool(as_float() >= operand.as_float());

			case OP_EQUAL:
				return Token::create_bool(as_float() == operand.as_float());

			case OP_NOT_EQUAL:
				return Token::create_bool(as_float() != operand.as_float());

			case OP_LOGICAL_AND:
				return Token::create_bool(as_bool() && operand.as_bool());

			case OP_LOGICAL_OR:
				return Token::create_bool(as_bool() || operand.as_bool());

			default: break;
		}
		break;

	case STRING_LITERAL:
		switch (op)
		{
			case OP_ADDITION:
				return Token::create_string(str_ + operand.as_string());

			case OP_MULTIPLICATION:
			{
				std::string result;
				int count = operand.as_int();
				for (int i = 0; i < count; ++i)
				{
					result += str_;
				}
				return Token::create_string(result);
			}

			case OP_LESS_THAN:
				return Token::create_bool(str_ < operand.as_string());

			case OP_LESS_THAN_OR_EQUAL:
				return Token::create_bool(str_ <= operand.as_string());

			case OP_GREATER_THAN:
				return Token::create_bool(str_ > operand.as_string());

			case OP_GREATER_THAN_OR_EQUAL:
				return Token::create_bool(str_ >= operand.as_string());

			case OP_EQUAL:
				return Token::create_bool(str_ == operand.as_string());

			case OP_NOT_EQUAL:
				return Token::create_bool(str_ != operand.as_string());

			case OP_LOGICAL_AND:
				return Token::create_bool(as_bool() && operand.as_bool());

			case OP_LOGICAL_OR:
				return Token::create_bool(as_bool() || operand.as_bool());
			default: break;
		}
		break;

	case VARIABLE:
		switch (op)
		{
			// Handle operators which update the variable value, operand is the assigned lvalue
			case OP_ASSIGNMENT:
			{
				// Always copy by value, otherwise copying the operand name will create a reference
				if (operand.type_ == VARIABLE)
				{
					Token& lvalue = operand.data_.variable->get();
					data_.variable->set(lvalue);
					return lvalue;
				}
				data_.variable->set(operand);
				return operand;
			}
			case OP_MULTIPLY_AND_ASSIGN:
			{
				Token& rvalue = data_.variable->get();
				rvalue = rvalue.apply_binary_operator(OP_MULTIPLICATION, operand);
				return rvalue;
			}
			case OP_DIVIDE_AND_ASSIGN:
			{
				Token& rvalue = data_.variable->get();
				rvalue = rvalue.apply_binary_operator(OP_DIVISION, operand);
				return rvalue;
			}
			case OP_MODULO_AND_ASSIGN:
			{
				Token& rvalue = data_.variable->get();
				rvalue = rvalue.apply_binary_operator(OP_MODULO, operand);
				return rvalue;
			}
			case OP_ADD_AND_ASSIGN:
			{
				Token& rvalue = data_.variable->get();
				rvalue = rvalue.apply_binary_operator(OP_ADDITION, operand);
				return rvalue;
			}
			case OP_SUBTRACT_AND_ASSIGN:
			{
				Token& rvalue = data_.variable->get();
				rvalue = rvalue.apply_binary_operator(OP_SUBTRACTION, operand);
				return rvalue;
			}
			default:
				// Variable is not modified, extract value and apply operator on it
				return data_.variable->get().apply_binary_operator(op, operand);
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
	return type_ == type || (type_ == VARIABLE && data_.variable->get().get_type() == type);
}


// cast ------------------------------------------------------------------------

const std::string& Token::as_string() const
{
	if (type_ == STRING_LITERAL)
		return str_;

	if (type_ == VARIABLE)
		return data_.variable->get().as_string();

	throw Error::TypeError("a string is required");
}



double Token::as_float() const
{
	switch (type_)
	{
		case FLOAT_LITERAL:
			return data_.float_value;
		case INT_LITERAL:
			return (double) data_.int_value;
		case BOOL_LITERAL:
			return (double) data_.bool_value;
		case VARIABLE:
			return data_.variable->get().as_float();
		default:
			throw Error::TypeError("a float is required");
	}
}


int Token::as_int() const
{
	switch (type_)
	{
		case INT_LITERAL:
			return data_.int_value;
		case BOOL_LITERAL:
			return data_.bool_value ? 1 : 0;
		case VARIABLE:
			return data_.variable->get().as_int();
		default:
			throw Error::TypeError("an integer is required");
	}
}


bool Token::as_bool() const
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
			return data_.variable->get().as_bool();
		default:
			throw Error::TypeError("a boolean is required");
	}
}


void Token::print_value(std::ostream& os) const
{
	switch (type_)
	{
		case INT_LITERAL:
			os << data_.int_value;
			break;
		case FLOAT_LITERAL:
			os << data_.float_value;
			break;
		case STRING_LITERAL:
			os << str_;
			break;
		case BOOL_LITERAL:
			os << (data_.bool_value ? "true" : "false");
			break;
		case VARIABLE:
			// print encapsulated token
			data_.variable->get().print_value(std::cout);
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
		case FUNCTION:
		case KEYWORD:
			os << "<not implemented>";
			break;
	}
}

// debug -----------------------------------------------------------------------

void Token::debug(std::ostream& os) const
{
	if (type_ == VARIABLE)
	{
		os << SymbolTable::find_variable_name(data_.variable);
	}
	else if (type_ == FUNCTION)
	{
		os << SymbolTable::find_function_name(data_.function);
	}
	else
	{
		print_value(os);
	}
}
