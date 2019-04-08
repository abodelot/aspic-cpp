#ifndef ASPIC_FUNCITON_WRAPPER_HPP
#define ASPIC_FUNCITON_WRAPPER_HPP

class Token;
class TokenStack;

typedef Token (*FunctionWrapper)(TokenStack&);

#endif
