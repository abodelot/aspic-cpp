#ifndef ASPIC_FUNCTION_WRAPPER_HPP
#define ASPIC_FUNCTION_WRAPPER_HPP

class Token;
class TokenStack;

typedef Token (*FunctionWrapper)(TokenStack&);

#endif
