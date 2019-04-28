#include "Operators.hpp"

/*
Operator precedence and associativity are the same than C++:
http://publib.boulder.ibm.com/infocenter/comphelp/v8v101/index.jsp?topic=/com.ibm.xlcpp8a.doc/language/ref/preeval.htm
Supported operators are marked with '*'.

Precedence and associativity of postfix operators

  Rank   Right associative?   Operator function                             Usage
  1      yes                  C++ global scope resolution                   :: name_or_qualified name
  1                           C++ class or namespace scope resolution       class_or_namespace :: member
  2                           member selection                              object . member
  2                           member selection                              pointer -> member
* 2                           subscripting                                  pointer [ expr ]
  2                           function call                                 expr ( expr_list )
  2                           value construction                            type ( expr_list )
  2                           postfix increment                             lvalue ++
  2                           postfix decrement                             lvalue --
  2      yes                  C++ type identification                       typeid ( type )
  2      yes                  C++ type identification at run time           typeid ( expr )
  2      yes                  C++ conversion checked at compile time        static_cast < type > ( expr )
  2      yes                  C++ conversion checked at run time            dynamic_cast < type > ( expr )
  2      yes                  C++ unchecked conversion                      reinterpret_cast < type > ( expr )
  2      yes                  C++ const conversion                          const_cast < type > ( expr )

Precedence and associativity of unary operators

  Rank   Right associative?   Operator function                             Usage
  3      yes                  size of object in bytes                       sizeof expr
  3      yes                  size of type in bytes                         sizeof ( type )
  3      yes                  prefix increment                              ++ lvalue
  3      yes                  prefix decrement                              -- lvalue
  3      yes                  bitwise negation                              ~ expr
* 3      yes                  not                                           ! expr
* 3      yes                  unary minus                                   - expr
* 3      yes                  unary plus                                    + expr
  3      yes                  address of                                    & lvalue
  3      yes                  indirection or dereference                    * expr
  3      yes                  C++ create (allocate memory)                  new type
  3      yes                  C++ create (allocate and initialize memory)   new type ( expr_list ) type
  3      yes                  C++ create (placement)                        new type ( expr_list ) type ( expr_list )
  3      yes                  C++ destroy (deallocate memory)               delete pointer
  3      yes                  C++ destroy array                             delete [ ] pointer
  3      yes                  type conversion (cast)                        ( type ) expr

Precedence and associativity of binary operators

  Rank   Right associative?   Operator function                             Usage
  4                           C++ member selection                          object .* ptr_to_member
  4                           C++ member selection                          object ->* ptr_to_member
* 5                           multiplication                                expr * expr
* 5                           division                                      expr / expr
* 5                           modulo (remainder)                            expr % expr
* 6                           binary addition                               expr + expr
* 6                           binary subtraction                            expr - expr
  7                           bitwise shift left                            expr << expr
  7                           bitwise shift right                           expr >> expr
* 8                           less than                                     expr < expr
* 8                           less than or equal to                         expr <= expr
* 8                           greater than                                  expr > expr
* 8                           greater than or equal to                      expr >= expr
* 9                           equal                                         expr == expr
* 9                           not equal                                     expr != expr
  10                          bitwise AND                                   expr & expr
  11                          bitwise exclusive OR                          expr ^ expr
  12                          bitwise inclusive OR                          expr | expr
* 13                          logical AND                                   expr && expr
* 14                          logical inclusive OR                          expr || expr
  15                          conditional expression                        expr ? expr : expr
* 16     yes                  simple assignment                             lvalue = expr
* 16     yes                  multiply and assign                           lvalue *= expr
* 16     yes                  divide and assign                             lvalue /= expr
* 16     yes                  modulo and assign                             lvalue %= expr
* 16     yes                  add and assign                                lvalue += expr
* 16     yes                  subtract and assign                           lvalue -= expr
  16     yes                  shift left and assign                         lvalue <<= expr
  16     yes                  shift right and assign                        lvalue >>= expr
  16     yes                  bitwise AND and assign                        lvalue &= expr
  16     yes                  bitwise exclusive OR and assign               lvalue ^= expr
  16     yes                  bitwise inclusive OR and assign               lvalue |= expr
  17     yes                  C++ throw expression                          throw expr
  18                          comma (sequencing)                            expr , expr
*/

int Operators::get_binding_power(Token::OperatorType operator_type)
{
    switch (operator_type) {
        case Token::OP_INDEX:
        case Token::OP_FUNC_CALL:
            return 100;

        case Token::OP_NOT:
        case Token::OP_UNARY_PLUS:
        case Token::OP_UNARY_MINUS:
            return 90;

        case Token::OP_POW:
            return 80;

        case Token::OP_MULTIPLICATION:
        case Token::OP_DIVISION:
        case Token::OP_MODULO:
            return 70;

        case Token::OP_ADDITION:
        case Token::OP_SUBTRACTION:
            return 60;

        case Token::OP_LESS_THAN:
        case Token::OP_LESS_THAN_OR_EQUAL:
        case Token::OP_GREATER_THAN:
        case Token::OP_GREATER_THAN_OR_EQUAL:
            return 50;

        case Token::OP_EQUAL:
        case Token::OP_NOT_EQUAL:
            return 40;

        case Token::OP_LOGICAL_AND:
            return 30;

        case Token::OP_LOGICAL_OR:
            return 20;

        case Token::OP_ASSIGNMENT:
        case Token::OP_MULTIPLY_AND_ASSIGN:
        case Token::OP_DIVIDE_AND_ASSIGN:
        case Token::OP_MODULO_AND_ASSIGN:
        case Token::OP_ADD_AND_ASSIGN:
        case Token::OP_SUBTRACT_AND_ASSIGN:
            return 10;
    }
    return 0;
}

const char* Operators::to_str(Token::OperatorType op)
{
    switch (op)
    {
        case Token::OP_INDEX:
            return "op[";
        case Token::OP_FUNC_CALL:
            return "op(";

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
    return nullptr;
}
