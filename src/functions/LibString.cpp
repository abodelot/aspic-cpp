#include "LibString.hpp"
#include "ast/Node.hpp"
#include "ast/NodeVector.hpp"

/**
 * Compute number of characters
 * @param 0: target string
 * @return string length
 */
Token str_len(const ast::NodeVector& args)
{
    args.check(1);
    int length = args[0]->eval().as_string().size();
    return Token::create_int(length);
}

/**
 * Count occurences in a string
 * @param 0: string
 * @param 1: occurrence
 * @return occurrence count
 */
Token str_count(const ast::NodeVector& args)
{
    args.check(2);
    std::string str =      args[0]->eval().as_string();
    std::string look_for = args[1]->eval().as_string();

    size_t pos = str.find(look_for);
    int count = 0;
    while (pos != std::string::npos)
    {
        pos = str.find(look_for, pos + 1);
        ++count;
    }
    return Token::create_int(count);
}

/**
 * @param 0: target string
 * @param 1: sous-chaîne rechercher
 * @param 2: sous-chaîne à caser
 * @return chaîne remplacée
 */
Token str_replace(const ast::NodeVector& args)
{
    args.check(3);
    std::string target  =    args[0]->eval().as_string();
    std::string look_for =   args[1]->eval().as_string();
    std::string replace_by = args[2]->eval().as_string();

    size_t step = replace_by.size();
    size_t offset = look_for.size();
    size_t pos = target.find(look_for);
    while (pos != std::string::npos)
    {
        target.replace(pos, offset, replace_by);
        pos = target.find(look_for, pos + step);
    }
    return Token::create_string(target);
}

/**
 * @param 0: string
 * @param 1: position of a character to be used as starting character for the substring.
 * @param 2: length of the substring
 * @return substr
 */
Token str_substr(const ast::NodeVector& args)
{
    args.check(3);
    std::string str = args[0]->eval().as_string();
    int from        = args[1]->eval().as_int();
    int size        = args[2]->eval().as_int();
    return Token::create_string(str.substr(from, size));
}

/**
 * Remove leading and trailing whitespaces
 * @param 0: string
 * @return trimmed string
 */
Token str_trim(const ast::NodeVector& args)
{
    args.check(1);
    std::string str = args[0]->eval().as_string();
    const char* WHITESPACES = " \t\n\r\0xb";
    std::string::size_type first = str.find_first_not_of(WHITESPACES);
    if (first != std::string::npos)
    {
        std::string::size_type last = str.find_last_not_of(WHITESPACES);
        return Token::create_string(str.substr(first, last - first + 1));
    }
    return Token::create_string(str);
}

/**
 * Convert to lowercase
 * @param 0: string
 * @return lowercase string
 */
Token str_lower(const ast::NodeVector& args)
{
    args.check(1);
    std::string str = args[0]->eval().as_string();
    for (size_t i = 0; i < str.length(); ++i)
    {
        str[i] = tolower(str[i]);
    }
    return Token::create_string(str);
}

/**
 * Convert to uppercase
 * @param 0: string
 * @return uppercase string
 */
Token str_upper(const ast::NodeVector& args)
{
    args.check(1);
    std::string str = args[0]->eval().as_string();
    for (size_t i = 0; i < str.length(); ++i)
    {
        str[i] = toupper(str[i]);
    }
    return Token::create_string(str);
}
