#include <sstream>

#include "ast/NodeVector.hpp"
#include "ast/Node.hpp"
#include "Error.hpp"

namespace ast {

void NodeVector::check(size_t count) const
{
    if (size() < count)
    {
        // "function takes [exactaly N|no] argument[s] (Z given)"
        std::ostringstream oss;
        oss << "function takes ";
        if (count == 0)
            oss << "no";
        else
            oss << "exactly " << count;

        oss << " argument";
        if (count != 1)
            oss << "s";

        oss << " (" << size() << " given)";
        throw Error::TypeError(oss.str());
    }
}

}
