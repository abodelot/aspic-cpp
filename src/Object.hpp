#ifndef ASPIC_OBJECT_HPP
#define ASPIC_OBJECT_HPP

#include "Operators.hpp"
#include "FunctionWrapper.hpp"

#include <string>
#include <iostream>

class Object
{
public:
    enum Type
    {
        INT,
        FLOAT,
        BOOL,
        STRING,
        BUILTIN_FUNCTION,
        REFERENCE,
        NULL_VALUE
    };

    // Constructors

    Object();
    static Object create_int(int value);
    static Object create_float(double value);
    static Object create_bool(bool value);
    static Object create_string(const std::string& string);
    static Object create_function(FunctionWrapper function_ptr);
    static Object create_reference(size_t id_hash);
    static Object create_null();

    // Types

    static const char* type_to_str(Type type);

    Type get_type() const;

    const Object& get_value() const;

    Type get_value_type() const;

    /**
     * Get literal value from an operand token, value is casted if necesary
     * If token cannot be represented as the requested type, TypeError is raised
     */
    int get_int() const;
    double get_float() const;
    const std::string& get_string() const;
    FunctionWrapper get_function() const;
    bool truthy() const;

    // Operations

    bool equal(const Object& object) const;

    /**
     * Apply an operation on an operand
     * @param op: applied operator
     * @param operand: second operand (for binary operators)
     * @return operation result, stored in a new object
     */
    Object apply_unary_operator(Operator op) const;

    Object apply_binary_operator(Operator op, const Object& operand) const;

private:
    friend std::ostream& operator<<(std::ostream&, const Object& object);

    /**
     * Check if token is typed with given type, or if token is an identifier
     * pointing to a literal with given type
     */
    bool contains(Type type) const;

    inline bool is_numeric() const
    {
        return type_ == INT || type_ == FLOAT;
    }

    // helper function for str * int operation
    static Object  multiply_string(const std::string& source, int count);

    Object(Type type);

    Type type_;

    union Data
    {
        int int_;
        double float_;
        bool bool_;
        FunctionWrapper function_ptr_;
        size_t id_hash_;
    };

    Data data_;
    std::string string_;
};

#endif
