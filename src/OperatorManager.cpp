#include <cstring>

#include "OperatorManager.hpp"


/*
http://publib.boulder.ibm.com/infocenter/comphelp/v8v101/index.jsp?topic=/com.ibm.xlcpp8a.doc/language/ref/preeval.htm

Precedence and associativity of postfix operators

Rank 	Right associative? 	Operator function 							Usage
1 		yes 				C++ global scope resolution 				:: name_or_qualified name
1 							C++ class or namespace scope resolution 	class_or_namespace :: member
2 							member selection 							object . member
2 							member selection 							pointer -> member
2 							subscripting 								pointer [ expr ]
2 							function call 								expr ( expr_list )
2 							value construction 							type ( expr_list )
2 							postfix increment 							lvalue ++
2 							postfix decrement 							lvalue --
2 		yes 				C++ type identification 					typeid ( type )
2 		yes 				C++ type identification at run time 		typeid ( expr )
2 		yes 				C++ conversion checked at compile time 		static_cast < type > ( expr )
2 		yes 				C++ conversion checked at run time 			dynamic_cast < type > ( expr )
2 		yes 				C++ unchecked conversion 					reinterpret_cast < type > ( expr )
2 		yes 				C++ const conversion 						const_cast < type > ( expr )

Precedence and associativity of unary operators

Rank 	Right associative? 	Operator function 							Usage
3 		yes 				size of object in bytes 					sizeof expr
3 		yes 	 			size of type in bytes 						sizeof ( type )
3 		yes 	 			prefix increment 							++ lvalue
3 		yes 	 			prefix decrement 							-- lvalue
3 		yes 	 			bitwise negation 							~ expr
*3 		yes 	 			not 										! expr
*3 		yes 	 			unary minus 								- expr
*3 		yes 	 			unary plus 									+ expr
3 		yes 	 			address of 									& lvalue
3 		yes 	 			indirection or dereference 					* expr
3 		yes 	 			C++ create (allocate memory) 				new type
3 		yes 	 			C++ create (allocate and initialize memory)	new type ( expr_list ) type
3 		yes 	 			C++ create (placement) 						new type ( expr_list ) type ( expr_list )
3 		yes 	 			C++ destroy (deallocate memory) 			delete pointer
3 		yes 	 			C++ destroy array 							delete [ ] pointer
3 		yes 	 			type conversion (cast) 						( type ) expr

Precedence and associativity of binary operators

Rank 	Right associative? 	Operator function 							Usage
4 							C++ member selection 						object .* ptr_to_member
4 							C++ member selection 						object ->* ptr_to_member
*5 							multiplication 								expr * expr
*5 							division 									expr / expr
*5 							modulo (remainder) 							expr % expr
*6 							binary addition 							expr + expr
*6 							binary subtraction 							expr - expr
7 							bitwise shift left 							expr << expr
7 							bitwise shift right 						expr >> expr
*8 							less than 									expr < expr
*8 							less than or equal to 						expr <= expr
*8 							greater than 								expr > expr
*8 							greater than or equal to 					expr >= expr
*9 							equal 										expr == expr
*9 							not equal 									expr != expr
10 							bitwise AND 								expr & expr
11 							bitwise exclusive OR 						expr ^ expr
12 							bitwise inclusive OR 						expr | expr
*13 						logical AND 								expr && expr
*14 						logical inclusive OR 						expr || expr
15 							conditional expression 						expr ? expr : expr
*16 	yes 				simple assignment 							lvalue = expr
*16		yes 				multiply and assign 						lvalue *= expr
*16		yes 				divide and assign 							lvalue /= expr
*16	 	yes 				modulo and assign 							lvalue %= expr
*16		yes 				add and assign 								lvalue += expr
*16		yes 				subtract and assign 						lvalue -= expr
16 		yes 				shift left and assign 						lvalue <<= expr
16 		yes 				shift right and assign 						lvalue >>= expr
16 		yes 				bitwise AND and assign 						lvalue &= expr
16 		yes 				bitwise exclusive OR and assign 			lvalue ^= expr
16 		yes 				bitwise inclusive OR and assign 			lvalue |= expr
17 		yes 				C++ throw expression 						throw expr
18 							comma (sequencing) 							expr , expr

*/


OperatorManager& OperatorManager::get_instance()
{
    static OperatorManager self;
    return self;
}


OperatorManager::OperatorManager()
{
	// init operators
	operators_["!"] = Token::OP_NOT;

	operators_["**"] = Token::OP_POW;
	operators_["*"]  = Token::OP_MULTIPLICATION;
	operators_["/"]  = Token::OP_DIVISION;
	operators_["%"]  = Token::OP_MODULO;
	operators_["+"]  = Token::OP_ADDITION; // see eval for OP_UNARY_PLUS
	operators_["-"]  = Token::OP_SUBTRACTION; // see eval for OP_UNARY_MINUS

	operators_["<"]  = Token::OP_LESS_THAN;
	operators_["<="] = Token::OP_LESS_THAN_OR_EQUAL;
	operators_[">"]  = Token::OP_GREATER_THAN;
	operators_[">="] = Token::OP_GREATER_THAN_OR_EQUAL;
	operators_["=="] = Token::OP_EQUAL;
	operators_["!="] = Token::OP_NOT_EQUAL;
	operators_["&&"] = Token::OP_LOGICAL_AND;
	operators_["||"] = Token::OP_LOGICAL_OR;

	operators_["="]  = Token::OP_ASSIGNMENT;
	operators_["*="] = Token::OP_MULTIPLY_AND_ASSIGN;
	operators_["/="] = Token::OP_DIVIDE_AND_ASSIGN;
	operators_["%="] = Token::OP_MODULO_AND_ASSIGN;
	operators_["+="] = Token::OP_ADD_AND_ASSIGN;
	operators_["-="] = Token::OP_SUBTRACT_AND_ASSIGN;

	// init precedences
	// see http://en.cppreference.com/w/cpp/language/operator_precedence for details
	precedences_[Token::OP_NOT]            = 3;
	precedences_[Token::OP_UNARY_PLUS]     = 3;
	precedences_[Token::OP_UNARY_MINUS]    = 3;

	precedences_[Token::OP_POW]            = 4;
	precedences_[Token::OP_MULTIPLICATION] = 5;
	precedences_[Token::OP_DIVISION]       = 5;
	precedences_[Token::OP_MODULO]         = 5;
	precedences_[Token::OP_ADDITION]       = 6;
	precedences_[Token::OP_SUBTRACTION]    = 6;

	precedences_[Token::OP_LESS_THAN]             = 8;
	precedences_[Token::OP_LESS_THAN_OR_EQUAL]    = 8;
	precedences_[Token::OP_GREATER_THAN]          = 8;
	precedences_[Token::OP_GREATER_THAN_OR_EQUAL] = 8;
	precedences_[Token::OP_EQUAL]                 = 9;
	precedences_[Token::OP_NOT_EQUAL]             = 9;
	precedences_[Token::OP_LOGICAL_AND]           = 13;
	precedences_[Token::OP_LOGICAL_OR]            = 14;

	precedences_[Token::OP_ASSIGNMENT]            = 16;
	precedences_[Token::OP_MULTIPLY_AND_ASSIGN]   = 16;
	precedences_[Token::OP_DIVIDE_AND_ASSIGN]     = 16;
	precedences_[Token::OP_MODULO_AND_ASSIGN]     = 16;
	precedences_[Token::OP_ADD_AND_ASSIGN]        = 16;
	precedences_[Token::OP_SUBTRACT_AND_ASSIGN]   = 16;
}


bool OperatorManager::eval(const std::string& str, Token::OperatorType& op_type, const Token* previous) const
{
	OperatorMap::const_iterator it = operators_.find(str);
	if (it == operators_.end())
	{
		return false;
	}
	op_type = it->second;
	// check if op_type is an unary operator
	if (op_type == Token::OP_ADDITION || op_type == Token::OP_SUBTRACTION)
	{
		/* "+" and "-" are unary operator if they are:
		 - preceded by nothing
		 - preceded by another operator
		 - preceded by a left bracket
		 - preceded by an argument separator
		*/
		if (previous == NULL
			|| previous->is_operator()
			|| previous->get_type() == Token::LEFT_BRACKET
			|| previous->get_type() == Token::ARG_SEPARATOR)
		{
			op_type = to_unary(op_type);
		}
	}
	return true;
}


int OperatorManager::compare(const Token& op1, const Token& op2)
{
	int a = op1.get_operator_type();
	int b = op2.get_operator_type();
	return precedences_[b] - precedences_[a];
}


Token::OperatorType OperatorManager::to_unary(Token::OperatorType op)
{
	switch (op)
	{
		case Token::OP_ADDITION:
			return Token::OP_UNARY_PLUS;
		case Token::OP_SUBTRACTION:
			return Token::OP_UNARY_MINUS;
		default:
			return op;
	}
}


const char* OperatorManager::to_str(Token::OperatorType op)
{
	switch (op)
	{
		case Token::OP_NOT:
			return "!";
		case Token::OP_UNARY_PLUS:
			return "unary+";
		case Token::OP_UNARY_MINUS:
			return "unary-";

		case Token::OP_POW:
			return "**";
		case Token::OP_ADDITION:
			return "+";
		case Token::OP_SUBTRACTION:
			return "-";
		case Token::OP_MULTIPLICATION:
			return "*";
		case Token::OP_DIVISION:
			return "/";
		case Token::OP_MODULO:
			return "%";

		case Token::OP_LESS_THAN:
			return "<";
		case Token::OP_LESS_THAN_OR_EQUAL:
			return "<=";
		case Token::OP_GREATER_THAN:
			return ">";
		case Token::OP_GREATER_THAN_OR_EQUAL:
			return ">=";
		case Token::OP_EQUAL:
			return "==";
		case Token::OP_NOT_EQUAL:
			return "!=";
		case Token::OP_LOGICAL_AND:
			return "&&";
		case Token::OP_LOGICAL_OR:
			return "||";

		case Token::OP_ASSIGNMENT:
			return "=";
		case Token::OP_MULTIPLY_AND_ASSIGN:
			return "*=";
		case Token::OP_DIVIDE_AND_ASSIGN:
			return "/=";
		case Token::OP_MODULO_AND_ASSIGN:
			return "%=";
		case Token::OP_ADD_AND_ASSIGN:
			return "+=";
		case Token::OP_SUBTRACT_AND_ASSIGN:
			return "-=";
	}
	return NULL;
}
