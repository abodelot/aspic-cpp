#ifndef FUNCITON_WRAPPER_HPP
#define FUNCITON_WRAPPER_HPP

class Token;
class TokenStack;

/**
 * Define the FunctionPtr type
 * The token stack holds all function arguments
 */
struct FunctionWrapper
{
    typedef Token (*Ptr)(TokenStack&);

    FunctionWrapper(const Ptr& p = nullptr):
        ptr(p)
    {
    }

    Ptr ptr;
};

#endif
