#ifndef ASPIC_FUNCTION_WRAPPER_HPP
#define ASPIC_FUNCTION_WRAPPER_HPP

class Object;

namespace ast {
    class NodeVector;
}

typedef Object (*FunctionWrapper)(const ast::NodeVector&);

#endif
