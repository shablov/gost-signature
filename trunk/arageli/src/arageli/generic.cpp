/*****************************************************************************

    generic.cpp

    This file is a part of the Arageli library.

    Copyright (C) 2006--2007 Sergey S. Lyalin

    The Arageli Library is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.

    The Arageli Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.

    We are also open for dual licensing for the whole library or
    for its particular part. If you are interested to get the library
    in this way, i.e. not under the GNU General Public License,
    please contact Arageli Support Service support.arageli@gmail.com.

*****************************************************************************/

/**
    \file generic.cpp
    \brief The generic.hpp file stuff implementation.
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_generic)

// REFERENCE ADDITIONAL HEADERS HERE


namespace Arageli
{

// PLACE ALL TEMPLATE NOT INLINE IMPLEMENTATIONS HERE

}


#else    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...


#include "generic.hpp"
#include "gauss.hpp"


namespace Arageli
{


generic_type::operator big_int () const
{
    if(const generic_big_int* a = dynamic_cast<const generic_big_int*>(value_m))
        return a->get_value();
    else
        throw generic_internal::unsupported_operation();
}

generic_type::operator vector<generic_type> () const
{
    if(const generic_vector* a = dynamic_cast<const generic_vector*>(value_m))
        return a->get_value();
    else
        throw generic_internal::unsupported_operation();
}

generic_type::operator matrix<generic_type> () const
{
    if(const generic_matrix* a = dynamic_cast<const generic_matrix*>(value_m))
        return a->get_value();
    else
        throw generic_internal::unsupported_operation();
}


generic_big_int* generic_big_int::clone () const
{
    return new generic_big_int(value);
}

std::string generic_big_int::type () const
{
    return "integer";
}

bool generic_big_int::operator_bool () const
{
    return bool(value);
}

generic_big_int* generic_big_int::logical_not () const
{
    return new generic_big_int(!value);
}

generic_big_int* generic_big_int::unary_plus () const
{
    return new generic_big_int(+value);
}

generic_big_int* generic_big_int::unary_minus () const
{
    return new generic_big_int(-value);
}

generic_internal* generic_big_int::postfix_increment ()
{
    return new generic_big_int(value++);
}

generic_internal* generic_big_int::postfix_decrement ()
{
    return new generic_big_int(value--);
}

generic_internal* generic_big_int::prefix_increment ()
{
    ++value;
    return this;
}

generic_internal* generic_big_int::prefix_decrement ()
{
    --value;
    return this;
}

generic_internal* generic_big_int::assign_plus (const generic_internal* x)
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(x))
    {
        value += p->value;
        return this;
    }
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::assign_minus (const generic_internal* x)
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(x))
    {
        value -= p->value;
        return this;
    }
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::assign_multiplies (const generic_internal* x)
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(x))
    {
        value *= p->value;
        return this;
    }
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::assign_divides (const generic_internal* x)
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(x))
    {
        value /= p->value;
        return this;
    }
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::assign_modulus (const generic_internal* x)
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(x))
    {
        value %= p->value;
        return this;
    }
    else
        throw unsupported_operation();
}


generic_internal* generic_big_int::assign_bitwise_or (const generic_internal* b)
{
    /*if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
    {
        value |= p->value;
        return this;
    }
    else */
        throw unsupported_operation();
}

generic_internal* generic_big_int::assign_bitwise_and (const generic_internal* b)
{
    /*if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
    {
        value &= p->value;
        return this;
    }
    else */
        throw unsupported_operation();
}

generic_internal* generic_big_int::assign_bitwise_xor (const generic_internal* b)
{
    /*if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
    {
        value ^= p->value;
        return this;
    }
    else */
        throw unsupported_operation();
}

generic_internal* generic_big_int::assign_shift_left (const generic_internal* b)
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
    {
        value <<= p->value;
        return this;
    }
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::assign_shift_right (const generic_internal* b)
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
    {
        value >>= p->value;
        return this;
    }
    else
        throw unsupported_operation();
}


generic_internal* generic_big_int::plus (const generic_internal* b) const
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_big_int(value + p->value);
    else if(const generic_vector* p = dynamic_cast<const generic_vector*>(b))
        return new generic_vector(generic_type(value) + p->value);
    else if(const generic_matrix* p = dynamic_cast<const generic_matrix*>(b))
        return new generic_matrix(generic_type(value) + p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::minus (const generic_internal* b) const
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_big_int(value - p->value);
    else if(const generic_vector* p = dynamic_cast<const generic_vector*>(b))
        return new generic_vector(generic_type(value) - p->value);
    else if(const generic_matrix* p = dynamic_cast<const generic_matrix*>(b))
        return new generic_matrix(generic_type(value) - p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::multiplies (const generic_internal* b) const
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_big_int(value * p->value);
    else if(const generic_vector* p = dynamic_cast<const generic_vector*>(b))
        return new generic_vector(generic_type(value) * p->value);
    else if(const generic_matrix* p = dynamic_cast<const generic_matrix*>(b))
        return new generic_matrix(generic_type(value) * p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::divides (const generic_internal* b) const
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_big_int(value / p->value);
    else if(const generic_vector* p = dynamic_cast<const generic_vector*>(b))
        return new generic_vector(generic_type(value) / p->value);
    else if(const generic_matrix* p = dynamic_cast<const generic_matrix*>(b))
        return new generic_matrix(generic_type(value) / p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::modulus (const generic_internal* b) const
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_big_int(value % p->value);
    else if(const generic_vector* p = dynamic_cast<const generic_vector*>(b))
        return new generic_vector(generic_type(value) % p->value);
    else if(const generic_matrix* p = dynamic_cast<const generic_matrix*>(b))
        return new generic_matrix(generic_type(value) % p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::equal_to (const generic_internal* b) const
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_big_int(value == p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::not_equal_to (const generic_internal* b) const
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_big_int(value != p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::greater (const generic_internal* b) const
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_big_int(value > p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::less (const generic_internal* b) const
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_big_int(value < p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::greater_equal (const generic_internal* b) const
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_big_int(value >= p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::less_equal (const generic_internal* b) const
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_big_int(value <= p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::bitwise_or (const generic_internal* b) const
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_big_int(value | p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::bitwise_and (const generic_internal* b) const
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_big_int(value & p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::bitwise_xor (const generic_internal* b) const
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_big_int(value ^ p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::shift_left (const generic_internal* b) const
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_big_int(value << p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_big_int::shift_right (const generic_internal* b) const
{
    if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_big_int(value >> p->value);
    else
        throw unsupported_operation();
}

void generic_big_int::output (std::ostream& out) const
{
    out << value;
}

generic_vector* generic_vector::clone () const
{
    return new generic_vector(value);
}

std::string generic_vector::type () const { return "vector"; }

generic_internal* generic_vector::plus (const generic_internal* b) const
{
    if(const generic_vector* p = dynamic_cast<const generic_vector*>(b))
        return new generic_vector(value + p->value);
    else if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_vector(value + generic_type(p->value));
    //else if(const generic_matrix* p = dynamic_cast<const generic_matrix*>(b))
    //    return new generic_matrix(value + p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_vector::minus (const generic_internal* b) const
{
    if(const generic_vector* p = dynamic_cast<const generic_vector*>(b))
        return new generic_vector(value - p->value);
    else if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_vector(value - generic_type(p->value));
    //else if(const generic_matrix* p = dynamic_cast<const generic_matrix*>(b))
    //    return new generic_matrix(value - p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_vector::multiplies (const generic_internal* b) const
{
    if(const generic_vector* p = dynamic_cast<const generic_vector*>(b))
        return new generic_vector(value * p->value);
    else if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_vector(value * generic_type(p->value));
    else if(const generic_matrix* p = dynamic_cast<const generic_matrix*>(b))
        return new generic_matrix(value * p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_vector::divides (const generic_internal* b) const
{
    if(const generic_vector* p = dynamic_cast<const generic_vector*>(b))
        return new generic_vector(value / p->value);
    else if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_vector(value / generic_type(p->value));
    //else if(const generic_matrix* p = dynamic_cast<const generic_matrix*>(b))
    //    return new generic_matrix(value / p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_vector::modulus (const generic_internal* b) const
{
    if(const generic_vector* p = dynamic_cast<const generic_vector*>(b))
        return new generic_vector(value % p->value);
    else if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_vector(value % generic_type(p->value));
    else
        throw unsupported_operation();
}

void generic_vector::output (std::ostream& out) const
{
    out << "vector" << value;
}

generic_matrix* generic_matrix::clone () const
{
    return new generic_matrix(value);
}

std::string generic_matrix::type () const
{
    return "matrix";
}

generic_internal* generic_matrix::plus (const generic_internal* b) const
{
    /*if(const generic_vector* p = dynamic_cast<const generic_vector*>(b))
        return new generic_vector(value + p->value);
    else */if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_matrix(value + generic_type(p->value));
    else if(const generic_matrix* p = dynamic_cast<const generic_matrix*>(b))
        return new generic_matrix(value + p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_matrix::minus (const generic_internal* b) const
{
    /*if(const generic_vector* p = dynamic_cast<const generic_vector*>(b))
        return new generic_vector(value - p->value);
    else */if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_matrix(value - generic_type(p->value));
    else if(const generic_matrix* p = dynamic_cast<const generic_matrix*>(b))
        return new generic_matrix(value - p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_matrix::multiplies (const generic_internal* b) const
{
    if(const generic_vector* p = dynamic_cast<const generic_vector*>(b))
        return new generic_matrix(value * p->value);
    else if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_matrix(value * generic_type(p->value));
    else if(const generic_matrix* p = dynamic_cast<const generic_matrix*>(b))
        return new generic_matrix(value * p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_matrix::divides (const generic_internal* b) const
{
    /*if(const generic_vector* p = dynamic_cast<const generic_vector*>(b))
        return new generic_vector(value / p->value);
    else */if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_matrix(value / generic_type(p->value));
    else if(const generic_matrix* p = dynamic_cast<const generic_matrix*>(b))
        return new generic_matrix(value / p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_matrix::modulus (const generic_internal* b) const
{
    /*if(const generic_vector* p = dynamic_cast<const generic_vector*>(b))
        return new generic_vector(value % p->value);
    else */if(const generic_big_int* p = dynamic_cast<const generic_big_int*>(b))
        return new generic_matrix(value % generic_type(p->value));
    else if(const generic_matrix* p = dynamic_cast<const generic_matrix*>(b))
        return new generic_matrix(value % p->value);
    else
        throw unsupported_operation();
}

void generic_matrix::output (std::ostream& out) const
{
    out << "matrix" << value;
}


generic_sparse_multipolynom* generic_sparse_multipolynom::clone () const
{
    return new generic_sparse_multipolynom(value);
}

generic_internal* generic_sparse_multipolynom::plus (const generic_internal* b) const
{
    if
    (
        const generic_sparse_multipolynom* p =
            dynamic_cast<const generic_sparse_multipolynom*>(b)
    )
        return new generic_sparse_multipolynom(value + p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_sparse_multipolynom::minus (const generic_internal* b) const
{
    if
    (
        const generic_sparse_multipolynom* p =
            dynamic_cast<const generic_sparse_multipolynom*>(b)
    )
        return new generic_sparse_multipolynom(value - p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_sparse_multipolynom::multiplies (const generic_internal* b) const
{
    if
    (
        const generic_sparse_multipolynom* p =
            dynamic_cast<const generic_sparse_multipolynom*>(b)
    )
        return new generic_sparse_multipolynom(value * p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_sparse_multipolynom::divides (const generic_internal* b) const
{
    if
    (
        const generic_sparse_multipolynom* p =
            dynamic_cast<const generic_sparse_multipolynom*>(b)
    )
        return new generic_sparse_multipolynom(value / p->value);
    else
        throw unsupported_operation();
}

generic_internal* generic_sparse_multipolynom::modulus (const generic_internal* b) const
{
    if
    (
        const generic_sparse_multipolynom* p =
            dynamic_cast<const generic_sparse_multipolynom*>(b)
    )
        return new generic_sparse_multipolynom(value % p->value);
    else
        throw unsupported_operation();
}


void generic_sparse_multipolynom::output (std::ostream& out) const
{
    out << "sparse_multipolynom(" << value << ")";
}


generic_internal* generic_internal::create_from_input (std::istream& in)
{
    // Temporary limited implementation.

    char ch;
    in >> ch;
    in.putback(ch);
    in.clear();

    if(std::isdigit(ch) || ch == '+' || ch == '-')
    {
        in.clear();
        big_int x;
        in >> x;
        if(in)
            return new generic_big_int(x);
        else
            return 0;
    }

    if(_Internal::read_literal(in, "vector"))
    {
        vector<generic_type> x;
        in >> x;
        if(in)
            return new generic_vector(x);
        else
            return 0;
    }

    if(_Internal::read_literal(in, "matrix"))
    {
        matrix<generic_type> x;
        in >> x;
        if(in)
            return new generic_matrix(x);
        else
            return 0;
    }

    if(_Internal::read_literal(in, "sparse_multipolynom"))
    {
        sparse_multipolynom<generic_type> x;
        in >> x;
        if(in)
            return new generic_sparse_multipolynom(x);
        else
            return 0;
    }

    return 0;
}


}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
