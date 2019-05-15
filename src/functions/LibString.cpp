#include "LibString.hpp"
#include "ast/Node.hpp"
#include "ast/NodeVector.hpp"

/**
 * Compute number of characters
 * @param 0: target string
 * @return string length
 */
Object str_len(const ast::NodeVector& args)
{
    args.check(1);
    int length = args[0]->eval().get_string().size();
    return Object::create_int(length);
}

/**
 * Count occurences in a string
 * @param 0: string
 * @param 1: occurrence
 * @return occurrence count
 */
Object str_count(const ast::NodeVector& args)
{
    args.check(2);
    std::string str =      args[0]->eval().get_string();
    std::string look_for = args[1]->eval().get_string();

    size_t pos = str.find(look_for);
    int count = 0;
    while (pos != std::string::npos)
    {
        pos = str.find(look_for, pos + 1);
        ++count;
    }
    return Object::create_int(count);
}

/**
 * @param 0: target string
 * @param 1: sous-chaîne rechercher
 * @param 2: sous-chaîne à caser
 * @return chaîne remplacée
 */
Object str_replace(const ast::NodeVector& args)
{
    args.check(3);
    std::string target  =    args[0]->eval().get_string();
    std::string look_for =   args[1]->eval().get_string();
    std::string replace_by = args[2]->eval().get_string();

    size_t step = replace_by.size();
    size_t offset = look_for.size();
    size_t pos = target.find(look_for);
    while (pos != std::string::npos)
    {
        target.replace(pos, offset, replace_by);
        pos = target.find(look_for, pos + step);
    }
    return Object::create_string(target);
}

/**
 * @param 0: string
 * @param 1: position of a character to be used as starting character for the substring.
 * @param 2: length of the substring
 * @return substr
 */
Object str_substr(const ast::NodeVector& args)
{
    args.check(3);
    std::string str = args[0]->eval().get_string();
    int from        = args[1]->eval().get_int();
    int size        = args[2]->eval().get_int();
    return Object::create_string(str.substr(from, size));
}

/**
 * Remove leading and trailing whitespaces
 * @param 0: string
 * @return trimmed string
 */
Object str_trim(const ast::NodeVector& args)
{
    args.check(1);
    std::string str = args[0]->eval().get_string();
    const char* WHITESPACES = " \t\n\r\0xb";
    std::string::size_type first = str.find_first_not_of(WHITESPACES);
    if (first != std::string::npos)
    {
        std::string::size_type last = str.find_last_not_of(WHITESPACES);
        return Object::create_string(str.substr(first, last - first + 1));
    }
    return Object::create_string(str);
}

/**
 * Convert to lowercase
 * @param 0: string
 * @return lowercase string
 */
Object str_lower(const ast::NodeVector& args)
{
    args.check(1);
    std::string str = args[0]->eval().get_string();
    for (size_t i = 0; i < str.length(); ++i)
    {
        str[i] = tolower(str[i]);
    }
    return Object::create_string(str);
}

/**
 * Convert to uppercase
 * @param 0: string
 * @return uppercase string
 */
Object str_upper(const ast::NodeVector& args)
{
    args.check(1);
    std::string str = args[0]->eval().get_string();
    for (size_t i = 0; i < str.length(); ++i)
    {
        str[i] = toupper(str[i]);
    }
    return Object::create_string(str);
}
