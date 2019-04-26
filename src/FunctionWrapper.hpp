#ifndef ASPIC_FUNCTION_WRAPPER_HPP
#define ASPIC_FUNCTION_WRAPPER_HPP

class Token;

namespace ast {
    class NodeVector;
}

typedef Token (*FunctionWrapper)(const ast::NodeVector&);

#endif
