/*****************************************************************************

    type_traits.hpp -- additional information about types.

    This file is part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2005--2007 Sergey S. Lyalin

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

/** \file

    This file contains stucture type_traits to represent additional
    information about types. That infrormation is used by some parts of
    the library. Note that here is only general implementation of
    the structures, it is not specialized for concrete types and makes some
    assumptions for specialized types. If your type doesn't fit to this
    implementaion you need to implement own specialization of type_traits.
*/


#ifndef _ARAGELI_type_traits_hpp_
#define _ARAGELI_type_traits_hpp_


#include "config.hpp"

#include <cstddef>
#include <limits>
#include <complex>    // for specialization of type_traits
#include <iostream>

#include "type_opers.hpp"


//****************************************************************************


namespace std
{

#define _ARAGELI_STCONS(TYPE, NAME, VALUE) static const TYPE NAME = VALUE

/// Specialization of numeric_limits for arrays.
/** It is needed for additional type info system. Note that actually
    there is NO factual numeric_limits for arrays (is_specialized = false). */
template <typename T, std::size_t Size>
class numeric_limits<T[Size]>
{
public:

    _ARAGELI_STCONS(float_denorm_style, has_denorm, denorm_absent);
    _ARAGELI_STCONS(bool, has_denorm_loss, false);
    _ARAGELI_STCONS(bool, has_infinity, false);
    _ARAGELI_STCONS(bool, has_quiet_NaN, false);
    _ARAGELI_STCONS(bool, has_signaling_NaN, false);
    _ARAGELI_STCONS(bool, is_bounded, false);
    _ARAGELI_STCONS(bool, is_exact, false);
    _ARAGELI_STCONS(bool, is_iec559, false);
    _ARAGELI_STCONS(bool, is_integer, false);
    _ARAGELI_STCONS(bool, is_modulo, false);
    _ARAGELI_STCONS(bool, is_signed, false);
    _ARAGELI_STCONS(bool, is_specialized, false);
    _ARAGELI_STCONS(bool, tinyness_before, false);
    _ARAGELI_STCONS(bool, traps, false);
    _ARAGELI_STCONS(float_round_style, round_style, round_toward_zero);
    _ARAGELI_STCONS(int, digits, 0);
    _ARAGELI_STCONS(int, digits10, 0);
    _ARAGELI_STCONS(int, max_exponent, 0);
    _ARAGELI_STCONS(int, max_exponent10, 0);
    _ARAGELI_STCONS(int, min_exponent, 0);
    _ARAGELI_STCONS(int, min_exponent10, 0);
    _ARAGELI_STCONS(int, radix, 0);

};

#undef _ARAGELI_STCONS

}


//****************************************************************************


namespace Arageli
{


/// Category of type.
/** This namespace contains the hierarchy of types each of those
    is a tag for particular data structure.  This will be used to help
    automatic selection of reloaded operators.
    WARNING!  This mechanism hasn't completed yet.
    Don't use it for the moment. */
namespace type_category
{
    /// Generic type.
    class type
    {};

    class matrix :
        public type
    {};

    class dense_matrix :
        public matrix
    {};

    class sparse_matrix :
        public matrix
    {};

    class vector :
        public type
    {};

    class dense_vector :
        public vector
    {};

    class sparse_vector :
        public vector
    {};

    class polynom :
        public type
    {};

    class dense_polynom :
        public polynom
    {};

    class sparse_polynom :
        public polynom
    {};

    class number :
        public type
    {};

    class rational :
        public number
    {};

    class integer :
        public number
    {};

    class real :
        public number
    {};

    class complex :
        public number
    {};

    class algebraic :
        public number
    {};

    class factory :
        public type
    {};

    class type_traits :
        public type
    {};

    class iomanip :
        public type
    {};

    class iostream :
        public type
    {};

    class iterator :
        public type
    {};

    class string :
        public type
    {};
}


namespace _Internal
{

// Select from interger and real category based on some properties.
template <bool is_specialized, bool is_integer>
struct auto_type_category_by_numeric_limits_helper
{
    typedef type_category::type value_type;
};

template <>
struct auto_type_category_by_numeric_limits_helper<true, false>
{
    typedef type_category::real value_type;
};

template <>
struct auto_type_category_by_numeric_limits_helper<true, true>
{
    typedef type_category::integer value_type;
};

}

/// Determines type category for particular type.
/** This is default bihaviour for type_traits. If type T have a specialization
    of std::numeric_limits and it indicates T is integer type, than value_type
    will be integer type category tag, otherwise real type. */
template <typename T>
struct auto_type_category_by_numeric_limits
{
    typedef
        typename _Internal::auto_type_category_by_numeric_limits_helper
        <
            std::numeric_limits<T>::is_specialized,
            std::numeric_limits<T>::is_integer
        >::value_type
            value_type;
};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/// Minimal implementation for each specialization of type_traits.
template <typename T>
struct type_traits_default
{
    static const bool is_specialized = false;

    /// True iff T is fraction (but not necessary a rational number, for last see below).
    static const bool is_rational = false;

    static const bool is_number = false;
    static const bool is_integer_number = false;
    static const bool is_polynom = false;
    static const bool is_real_number = false;
    static const bool is_rational_number = false;
    static const bool is_complex_number = false;
    static const bool is_ring = false;    // +, -, *, null, unit
    static const bool is_field = false;    // +, -, *, /, null, unit
    static const bool is_finite = false;
    static const bool is_additive_group = false;    // +, -, null
    static const bool is_multiplicative_group = false;    // *, /, unit
    static const bool is_integer_modulo_ring = false;
    static const bool is_matrix = false;
    static const bool is_vector = false;

    static const bool has_zero_divisor = true;
    static const bool has_commutative_multiplication = false;
    static const bool has_commutative_addition = false;
    static const bool has_associative_multiplication = false;
    static const bool has_associative_addition = false;
    static const bool has_distributive_multiplication = false;
    static const bool has_distributive_addition = false;
    static const bool has_null = false;
    static const bool has_unit = false;
    static const bool has_opposite_unit = false;
    static const bool is_entire_ring = false;

    /// True iff type is composite type consists another elements.
    static const bool is_aggregate = false;

    /// Type of each element if T is composite type.
    typedef T element_type;

    template <typename T1, bool REFCNT2>
    struct other_element_type_refcnt;

    typedef type_category::type category_type;
    static const category_type category_value;
};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/// General specialization of root of additional information service about type.
/**    This default implementation only appropriate for types that have
    specialized std::numeric_limits template. */
template <typename T>
struct type_traits :
    public type_traits_default<T>
{
private:

    typedef std::numeric_limits<T> numeric_limits;

public:

    static const bool is_specialized = numeric_limits::is_specialized;
    static const bool is_rational = false;
    static const bool is_number = numeric_limits::is_specialized;
    static const bool is_integer_number = numeric_limits::is_integer;
    static const bool is_polynom = false;
    static const bool is_real_number = !numeric_limits::is_integer;
    static const bool is_rational_number = false;
    static const bool is_complex_number = false;
    static const bool is_ring = numeric_limits::is_integer;

    static const bool is_field =
                        !numeric_limits::is_integer ||
                        equal_types<bool, T>::bvalue;

    static const bool is_finite = equal_types<bool, T>::bvalue;
    static const bool is_additive_group = numeric_limits::is_signed;

    static const bool is_multiplicative_group =
                        !numeric_limits::is_integer ||
                        equal_types<bool, T>::bvalue;

    static const bool has_zero_divisor = false;
    static const bool is_integer_modulo_ring = equal_types<bool, T>::bvalue;
    static const bool is_matrix = false;
    static const bool is_vector = false;
    static const bool has_commutative_multiplication = true;
    static const bool has_commutative_addition = true;
    static const bool has_associative_multiplication = true;
    static const bool has_associative_addition = true;
    static const bool has_distributive_multiplication = true;
    static const bool has_distributive_addition = true;
    static const bool has_null = true;
    static const bool has_unit = true;
    static const bool has_opposite_unit = true;

    static const bool is_entire_ring =
                        is_ring &&
                        has_associative_multiplication &&
                        has_commutative_multiplication;

    static const bool is_aggregate = false;
    typedef T element_type;
    typedef typename auto_type_category_by_numeric_limits<T>::value_type
        category_type;
    static const category_type category_value;
};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/// Specialization of type_traits template class for std::complex.
template <typename T>
struct type_traits<std::complex<T> > :
    public type_traits_default<std::complex<T> >
{
    static const bool is_specialized =
        type_traits<T>::is_specialized;
    static const bool is_number =
        type_traits<T>::is_number;
    static const bool is_integer_number = false;
    static const bool is_polynom = false;
    static const bool is_real_number = false;
    static const bool is_rational_number = false;
    static const bool is_complex_number =
        type_traits<T>::is_number;
    static const bool is_ring = type_traits<T>::is_ring;
    static const bool is_field = type_traits<T>::is_field;
    static const bool is_finite =
        type_traits<T>::is_finite;
    static const bool is_additive_group =
        type_traits<T>::is_additive_group;
    static const bool is_multiplicative_group =
        type_traits<T>::is_multiplicative_group;
    static const bool has_zero_divisor =
        type_traits<T>::has_zero_divisor;
    static const bool is_integer_modulo_ring = false;
    static const bool is_matrix = false;
    static const bool is_vector = false;

    static const bool has_commutative_multiplication =
        type_traits<T>::has_commutative_multiplication;

    static const bool has_commutative_addition =
        type_traits<T>::has_commutative_addition;

    static const bool has_null = type_traits<T>::has_null;
    static const bool has_unit = type_traits<T>::has_unit;
    static const bool has_opposite_unit =
        type_traits<T>::has_opposite_unit;
    typedef type_category::complex category_type;
    static const category_type category_value;
};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template <typename Iter, typename Val>
struct type_traits_iterator :
    public type_traits_default<Iter>
{
    static const bool is_specialized = true;
    static const bool is_aggregate = true;
    typedef Val element_type;
    typedef type_category::iterator category_type;
    static const category_type category_value;
};

template <typename T>
struct type_traits<T*> :
    public type_traits_iterator<T*, T>
{};

template <typename T, std::size_t Size>
struct type_traits<T[Size]> :
    public type_traits_iterator<T[Size], T>
{};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template <typename Str, typename Char>
struct type_traits_string :
    public type_traits_default<Str>
{
    static const bool is_specialized = true;
    static const bool is_aggregate = true;
    typedef char element_type;
    typedef type_category::string category_type;
    static const category_type category_value;
};


template <> struct type_traits<const char*> :
    public type_traits_string<const char*, char> {};

template <> struct type_traits<char*> :
    public type_traits_string<char*, char> {};

template <std::size_t Size> struct type_traits<const char[Size]> :
    public type_traits_string<const char[Size], char> {};

template <std::size_t Size> struct type_traits<char[Size]> :
    public type_traits_string<char[Size], char> {};

template <> struct type_traits<const wchar_t*> :
    public type_traits_string<const wchar_t*, wchar_t> {};

template <> struct type_traits<wchar_t*> :
    public type_traits_string<wchar_t*, wchar_t> {};

template <std::size_t Size> struct type_traits<const wchar_t[Size]> :
    public type_traits_string<const wchar_t[Size], wchar_t>
    {};

template <std::size_t Size> struct type_traits<wchar_t[Size]> :
    public type_traits_string<wchar_t[Size], wchar_t>
    {};

template <typename Char, typename CharTr, typename A>
struct type_traits<std::basic_string<Char, CharTr, A> > :
    public type_traits_string<std::basic_string<Char, CharTr, A>, Char>
{};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template <typename Str>
struct type_traits_iostream :
    public type_traits_default<Str>
{
    static const bool is_specialized = true;
    static const bool is_aggregate = false;
    typedef type_category::iostream category_type;
    static const category_type category_value;
};


template <typename Ch, typename ChT>
struct type_traits<std::basic_istream<Ch, ChT> > :
    public type_traits_iostream<std::basic_istream<Ch, ChT> >
{};

template <typename Ch, typename ChT>
struct type_traits<std::basic_ostream<Ch, ChT> > :
    public type_traits_iostream<std::basic_ostream<Ch, ChT> >
{};

template <typename Ch, typename ChT>
struct type_traits<std::basic_ofstream<Ch, ChT> > :
    public type_traits_iostream<std::basic_ifstream<Ch, ChT> >
{};

template <typename Ch, typename ChT>
struct type_traits<std::basic_ifstream<Ch, ChT> > :
    public type_traits_iostream<std::basic_ofstream<Ch, ChT> >
{};

template <typename Ch, typename ChT>
struct type_traits<std::basic_fstream<Ch, ChT> > :
    public type_traits_iostream<std::basic_fstream<Ch, ChT> >
{};

template <typename Ch, typename ChT>
struct type_traits<std::basic_istringstream<Ch, ChT> > :
    public type_traits_iostream<std::basic_istringstream<Ch, ChT> >
{};

template <typename Ch, typename ChT>
struct type_traits<std::basic_ostringstream<Ch, ChT> > :
    public type_traits_iostream<std::basic_ostringstream<Ch, ChT> >
{};

template <typename Ch, typename ChT>
struct type_traits<std::basic_stringstream<Ch, ChT> > :
    public type_traits_iostream<std::basic_stringstream<Ch, ChT> >
{};


} // namespace Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_TYPE_TRAITS
    #include "type_traits.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_TYPE_TRAITS
#endif


#endif  //  #ifndef _ARAGELI_typetraits_hpp_
