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


bool Operators::is_right_associative(Operator op)
{
    // Right associative operators are
    // - unary operators
    // - assignment operators
    // - pow
    switch (op) {
        case Operator::OP_NOT:
        case Operator::OP_UNARY_PLUS:
        case Operator::OP_UNARY_MINUS:
        case Operator::OP_POW:
        case Operator::OP_ASSIGNMENT:
        case Operator::OP_MULTIPLY_AND_ASSIGN:
        case Operator::OP_DIVIDE_AND_ASSIGN:
        case Operator::OP_MODULO_AND_ASSIGN:
        case Operator::OP_ADD_AND_ASSIGN:
        case Operator::OP_SUBTRACT_AND_ASSIGN:
            return true;
        default:
            return false;
    }
}

int Operators::get_binding_power(Operator operator_type)
{
    switch (operator_type) {
        case Operator::OP_INDEX:
        case Operator::OP_FUNC_CALL:
            return 100;

        case Operator::OP_NOT:
        case Operator::OP_UNARY_PLUS:
        case Operator::OP_UNARY_MINUS:
            return 90;

        case Operator::OP_POW:
            return 80;

        case Operator::OP_MULTIPLICATION:
        case Operator::OP_DIVISION:
        case Operator::OP_MODULO:
            return 70;

        case Operator::OP_ADDITION:
        case Operator::OP_SUBTRACTION:
            return 60;

        case Operator::OP_LESS_THAN:
        case Operator::OP_LESS_THAN_OR_EQUAL:
        case Operator::OP_GREATER_THAN:
        case Operator::OP_GREATER_THAN_OR_EQUAL:
            return 50;

        case Operator::OP_EQUAL:
        case Operator::OP_NOT_EQUAL:
            return 40;

        case Operator::OP_LOGICAL_AND:
            return 30;

        case Operator::OP_LOGICAL_OR:
            return 20;

        case Operator::OP_ASSIGNMENT:
        case Operator::OP_MULTIPLY_AND_ASSIGN:
        case Operator::OP_DIVIDE_AND_ASSIGN:
        case Operator::OP_MODULO_AND_ASSIGN:
        case Operator::OP_ADD_AND_ASSIGN:
        case Operator::OP_SUBTRACT_AND_ASSIGN:
            return 10;
    }
    return 0;
}

const char* Operators::to_str(Operator op)
{
    switch (op)
    {
        case Operator::OP_INDEX:
            return "op[";
        case Operator::OP_FUNC_CALL:
            return "op(";

        case Operator::OP_NOT:
            return "!";
        case Operator::OP_UNARY_PLUS:
            return "unary+";
        case Operator::OP_UNARY_MINUS:
            return "unary-";

        case Operator::OP_POW:
            return "**";
        case Operator::OP_ADDITION:
            return "+";
        case Operator::OP_SUBTRACTION:
            return "-";
        case Operator::OP_MULTIPLICATION:
            return "*";
        case Operator::OP_DIVISION:
            return "/";
        case Operator::OP_MODULO:
            return "%";

        case Operator::OP_LESS_THAN:
            return "<";
        case Operator::OP_LESS_THAN_OR_EQUAL:
            return "<=";
        case Operator::OP_GREATER_THAN:
            return ">";
        case Operator::OP_GREATER_THAN_OR_EQUAL:
            return ">=";
        case Operator::OP_EQUAL:
            return "==";
        case Operator::OP_NOT_EQUAL:
            return "!=";
        case Operator::OP_LOGICAL_AND:
            return "&&";
        case Operator::OP_LOGICAL_OR:
            return "||";

        case Operator::OP_ASSIGNMENT:
            return "=";
        case Operator::OP_MULTIPLY_AND_ASSIGN:
            return "*=";
        case Operator::OP_DIVIDE_AND_ASSIGN:
            return "/=";
        case Operator::OP_MODULO_AND_ASSIGN:
            return "%=";
        case Operator::OP_ADD_AND_ASSIGN:
            return "+=";
        case Operator::OP_SUBTRACT_AND_ASSIGN:
            return "-=";
    }
    return nullptr;
}
