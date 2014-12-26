/*****************************************************************************

    type_pair_traits.hpp -- additional information about pair of types.

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

    This file contains stucture type_pair_traits to represent additional
    information about pair of types. That infrormation is used by some parts of
    the library. Note that here is only general implementation of
    the structures, it is not specialized for concrete types and makes some
    assumptions for specialized types. If your types don't fit to this
    implementaion, you need to implement own specialization of type_pair_traits.
*/


#ifndef _ARAGELI_type_pair_traits_hpp_
#define _ARAGELI_type_pair_traits_hpp_

#include "config.hpp"

#include <limits>

#include "type_traits.hpp"


//****************************************************************************


namespace Arageli
{

/// Prototype of minimal specialization for type_pair_traits.
template <typename T1, typename T2>
struct type_pair_traits_default
{
    /// True iff there is available information about these type pair.
    static const bool is_specialized = false;

    /// True iff an object of the first type can be converted to the second type.
    /** This is related to both initialization and assignment. */
    static const bool is_convertible = false;

    /// True iff any object of the first type can be converted to the second type without loss information.
    /** This is related to both initialization and assignment. */
    static const bool is_safe_convertible = false;

    /// True iff an object of the first type can be converted to the second type.
    /** This is related only to assignment. */
    static const bool is_assignable = false;

    /// True iff any object of the first type can be converted to the second type without loss information.
    /** This is related only to assignment. */
    static const bool is_safe_assignable = false;

    /// True iff an object of the first type can be converted to the second type.
    /** This is related only to initialization (by constructor). */
    static const bool is_initializable = false;

    /// True iff any object of the first type can be converted to the second type without loss information.
    /** This is related only to initialization (by constructor). */
    static const bool is_safe_initializable = false;
};


/// The root source for information about ralations of two types.
template <typename T1, typename T2>
struct type_pair_traits;


template <typename T1, typename T2>
struct type_pair_traits<const T1, T2> :
    public type_pair_traits<T1, T2>
{};

template <typename T1, typename T2>
struct type_pair_traits<T1, const T2> :
    public type_pair_traits<T1, T2>
{};

template <typename T1, typename T2>
struct type_pair_traits<const T1, const T2> :
    public type_pair_traits<T1, T2>
{};

template <typename T1, typename T2>
struct type_pair_traits<T1&, T2> :
    public type_pair_traits<T1, T2>
{};

template <typename T1, typename T2>
struct type_pair_traits<T1, T2&> :
    public type_pair_traits<T1, T2>
{};

template <typename T1, typename T2>
struct type_pair_traits<T1&, T2&> :
    public type_pair_traits<T1, T2>
{};


template <typename T>
struct type_pair_traits<const T, T> :
    public type_pair_traits<T, T>
{};

template <typename T>
struct type_pair_traits<T, const T> :
    public type_pair_traits<T, T>
{};

template <typename T>
struct type_pair_traits<const T, const T> :
    public type_pair_traits<T, T>
{};

template <typename T>
struct type_pair_traits<T&, T> :
    public type_pair_traits<T, T>
{};

template <typename T>
struct type_pair_traits<T, T&> :
    public type_pair_traits<T, T>
{};

template <typename T>
struct type_pair_traits<T&, T&> :
    public type_pair_traits<T, T>
{};


template <typename T1, typename T2, bool ISAGR1, bool ISAGR2>
struct type_pair_traits_helper;

template <typename T1, typename T2>
struct type_pair_traits_helper<T1, T2, true, true>
{
private:

    typedef type_traits<T1> TT1;
    typedef type_traits<T2> TT2;
    typedef typename type_traits<T1>::element_type TE1;
    typedef typename type_traits<T2>::element_type TE2;

public:

    static const bool is_specialized =
        TT1::is_specialized && TT2::is_specialized;

    static const bool is_convertible =
        type_pair_traits<T1, TE2>::is_convertible
        ||
            type_pair_traits<TE1, TE2>::is_convertible &&
            !type_pair_traits<T1, TE2>::is_convertible &&
            TT1::is_rational == TT2::is_rational &&
            TT1::is_vector == TT2::is_vector &&
            TT1::is_matrix == TT2::is_matrix &&
            TT1::is_complex_number == TT2::is_complex_number &&
            TT1::is_polynom == TT2::is_polynom
        ||
        TT1::is_rational &&
            type_pair_traits<TE1, T2>::is_convertible;

    static const bool is_safe_convertible =
        type_pair_traits<T1, TE2>::is_safe_convertible
        ||
            type_pair_traits<TE1, TE2>::is_safe_convertible &&
            !type_pair_traits<T1, TE2>::is_safe_convertible &&
            TT1::is_rational == TT2::is_rational &&
            TT1::is_vector == TT2::is_vector &&
            TT1::is_matrix == TT2::is_matrix &&
            TT1::is_complex_number == TT2::is_complex_number &&
            TT1::is_polynom == TT2::is_polynom;

    static const bool is_assignable =
        type_pair_traits<T1, TE2>::is_assignable
        ||
            type_pair_traits<TE1, TE2>::is_assignable &&
            !type_pair_traits<T1, TE2>::is_assignable &&
            TT1::is_rational == TT2::is_rational &&
            TT1::is_vector == TT2::is_vector &&
            TT1::is_matrix == TT2::is_matrix &&
            TT1::is_complex_number == TT2::is_complex_number &&
            TT1::is_polynom == TT2::is_polynom
        ||
        TT1::is_rational &&
            type_pair_traits<TE1, T2>::is_assignable;

    static const bool is_safe_assignable =
        type_pair_traits<T1, TE2>::is_safe_assignable
        ||
            type_pair_traits<TE1, TE2>::is_safe_assignable &&
            !type_pair_traits<T1, TE2>::is_safe_assignable &&
            TT1::is_rational == TT2::is_rational &&
            TT1::is_vector == TT2::is_vector &&
            TT1::is_matrix == TT2::is_matrix &&
            TT1::is_complex_number == TT2::is_complex_number &&
            TT1::is_polynom == TT2::is_polynom;

    static const bool is_initializable =
        type_pair_traits<T1, TE2>::is_initializable
        ||
            type_pair_traits<TE1, TE2>::is_initializable &&
            !type_pair_traits<T1, TE2>::is_initializable &&
            TT1::is_rational == TT2::is_rational &&
            TT1::is_vector == TT2::is_vector &&
            TT1::is_matrix == TT2::is_matrix &&
            TT1::is_complex_number == TT2::is_complex_number &&
            TT1::is_polynom == TT2::is_polynom
        ||
        TT1::is_rational &&
            type_pair_traits<TE1, T2>::is_initializable;

    static const bool is_safe_initializable =
        type_pair_traits<T1, TE2>::is_safe_initializable
        ||
            type_pair_traits<TE1, TE2>::is_safe_initializable &&
            !type_pair_traits<T1, TE2>::is_safe_initializable &&
            TT1::is_rational == TT2::is_rational &&
            TT1::is_vector == TT2::is_vector &&
            TT1::is_matrix == TT2::is_matrix &&
            TT1::is_complex_number == TT2::is_complex_number &&
            TT1::is_polynom == TT2::is_polynom;
};


template <typename T1, typename T2>
struct type_pair_traits_helper<T1, T2, true, false>
{
private:

    typedef type_traits<T1> TT1;
    typedef type_traits<T2> TT2;
    typedef typename type_traits<T1>::element_type TE1;

public:

    static const bool is_specialized =
        TT1::is_specialized && TT2::is_specialized;

    static const bool is_convertible =
        TT1::is_rational &&
        type_pair_traits<TE1, T2>::is_convertible;

    static const bool is_safe_convertible = false;

    static const bool is_assignable =
        TT1::is_rational &&
        type_pair_traits<TE1, T2>::is_assignable;

    static const bool is_safe_assignable = false;

    static const bool is_initializable =
        TT1::is_rational &&
        type_pair_traits<TE1, T2>::is_initializable;

    static const bool is_safe_initializable = false;
};


template <typename T1, typename T2>
struct type_pair_traits_helper<T1, T2, false, true>
{
private:

    typedef type_traits<T1> TT1;
    typedef type_traits<T2> TT2;
    typedef typename type_traits<T2>::element_type TE2;

public:

    static const bool is_specialized =
        TT1::is_specialized && TT2::is_specialized;

    static const bool is_convertible =
        type_pair_traits<T1, TE2>::is_convertible;

    static const bool is_safe_convertible =
        type_pair_traits<T1, TE2>::is_safe_convertible;

    static const bool is_assignable =
        type_pair_traits<T1, TE2>::is_assignable;

    static const bool is_safe_assignable =
        type_pair_traits<T1, TE2>::is_safe_assignable;

    static const bool is_initializable =
        type_pair_traits<T1, TE2>::is_initializable;

    static const bool is_safe_initializable =
        type_pair_traits<T1, TE2>::is_safe_initializable;
};


template <typename T1, typename T2>
struct type_pair_traits_helper<T1, T2, false, false>
{
private:

    typedef type_traits<T1> TT1;
    typedef type_traits<T2> TT2;
    typedef std::numeric_limits<T1> NL1;
    typedef std::numeric_limits<T2> NL2;

public:

    static const bool is_specialized =
        TT1::is_specialized && TT2::is_specialized;

    static const bool is_convertible =
        TT1::is_number && TT2::is_number;

    static const bool is_safe_convertible =
        is_convertible &&
        (!(NL1::is_signed && !NL2::is_signed)) &&
        (!(!NL1::is_integer && NL2::is_integer)) &&
        (
            NL1::is_bounded && NL2::is_bounded && (NL1::digits <= NL2::digits) ||
            NL1::is_bounded && !NL2::is_bounded
        );

    static const bool is_assignable = is_convertible;
    static const bool is_safe_assignable = is_safe_convertible;
    static const bool is_initializable = is_convertible;
    static const bool is_safe_initializable = is_safe_convertible;
};


/// Generic implementation of type_pair_traits --- information about two types.
template <typename T1, typename T2>
struct type_pair_traits :
    public type_pair_traits_helper
    <
        T1,
        T2,
        type_traits<T1>::is_aggregate,
        type_traits<T2>::is_aggregate
    >
{};


/// Pattern for type_pair_traits in case two identical types.
template <typename T>
struct type_pair_traits_for_the_same
{
    static const bool is_specialized = true;
    static const bool is_convertible = true;
    static const bool is_safe_convertible = true;
    static const bool is_assignable = true;
    static const bool is_safe_assignable = true;
    static const bool is_initializable = true;
    static const bool is_safe_initializable = true;
};


/// Specialization of type_pair_traits for two identical types.
template <typename T>
struct type_pair_traits<T, T> :
    public type_pair_traits_for_the_same<T>
{};


#if 0

/// Pattern for type_pair_traits for two similar aggregates of similar elements.
template <typename T1, typename T2>
struct type_pair_traits_for_similar_aggregates
{
protected:

    typedef typename type_traits<T1>::element_type TE1;
    typedef typename type_traits<T2>::element_type TE2;

public:

    static const bool is_specialized =
        type_traits<T1>::is_specialized &&
        type_traits<T2>::is_specialized &&
        type_pair_traits<TE1, TE2>::is_specialized;

    static const bool is_convertible =
        type_pair_traits<TE1, TE2>::is_convertible;

    static const bool is_safe_convertible =
        type_pair_traits<TE1, TE2>::is_safe_convertible;

    static const bool is_assignable =
        type_pair_traits<TE1, TE2>::is_assignable;

    static const bool is_safe_assignable =
        type_pair_traits<TE1, TE2>::is_safe_assignable;

    static const bool is_initializable =
        type_pair_traits<TE1, TE2>::is_initializable;

    static const bool is_safe_initializable =
        type_pair_traits<TE1, TE2>::is_safe_initializable;
};


/// Pattern for type_pair_traits for two builting numeric types.
template <typename T1, typename T2>
struct type_pair_traits_for_builtin_numeric
{
private:

    typedef std::numeric_limits<T1> NL1;
    typedef std::numeric_limits<T2> NL2;

public:

    static const bool is_specialized =
        NL1::is_specialized && NL2::is_specialized;

    static const bool is_convertible = true;

    static const bool is_safe_convertible =
        (!(NL1::is_signed && !NL2::is_signed)) &&
        (!(!NL1::is_integer && NL2::is_integer)) &&
        (NL1::digits <= NL2::digits || NL1::is_bounded && !NL1::is_bounded);

    static const bool is_assignable = is_convertible;
    static const bool is_safe_assignable = is_safe_convertible;
    static const bool is_initializable = is_convertible;
    static const bool is_safe_initializable = is_safe_convertible;
};


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/// @name Specialization of type_pair_traits for buitin numeric types.
/** The following macros and its instantation need to generate specializations
    for all pairs of builtin types. */
//@{

#define ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, T2)    \
    template <> struct type_pair_traits<T1, T2> :    \
        public type_pair_traits_for_builtin_numeric<T1, T2>    \
    {};

#define ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(T1)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, bool)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, char)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, signed char)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, unsigned char)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, signed short int)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, unsigned short int)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, signed int)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, unsigned int)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, signed long int)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, unsigned long int)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, float)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, double)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, long double)

#define ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_SECOND_STD_DEF(T2)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(bool, T2)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(char, T2)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(signed char, T2)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(unsigned char, T2)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(signed short int, T2)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(unsigned short int, T2)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(signed int, T2)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(unsigned int, T2)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(signed long int, T2)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(unsigned long int, T2)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(float, T2)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(double, T2)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(long double, T2)

#define ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_STD_DEF    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(bool)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(char)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(signed char)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(unsigned char)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(signed short int)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(unsigned short int)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(signed int)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(unsigned int)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(signed long int)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(unsigned long int)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(float)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(double)    \
    ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(long double)

#if defined(ARAGELI_INT64_SUPPORT) && defined(ARAGELI_LONG_LONG_SUPPORT)

    #define ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STDEX_DEF(T1)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(T1)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_SECOND_STD_DEF(T1)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, signed __int64)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, unsigned __int64)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, signed long long)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, unsigned long long)

    #define ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_STDEX_DEF    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_STD_DEF    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STDEX_DEF(signed __int64)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STDEX_DEF(unsigned __int64)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STDEX_DEF(signed long long)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STDEX_DEF(unsigned long long)

#elif defined(ARAGELI_INT64_SUPPORT) && !defined(ARAGELI_LONG_LONG_SUPPORT)

    #define ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STDEX_DEF(T1)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(T1)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_SECOND_STD_DEF(T1)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, signed __int64)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, unsigned __int64)

    #define ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_STDEX_DEF    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_STD_DEF    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STDEX_DEF(signed __int64)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STDEX_DEF(unsigned __int64)

#elif !defined(ARAGELI_INT64_SUPPORT) && defined(ARAGELI_LONG_LONG_SUPPORT)

    #define ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STDEX_DEF(T1)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(T1)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_SECOND_STD_DEF(T1)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, signed long long)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_ONE_DEF(T1, unsigned long long)

    #define ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_STDEX_DEF    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_STD_DEF    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STDEX_DEF(signed long long)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STDEX_DEF(unsigned long long)

#elif !defined(ARAGELI_INT64_SUPPORT) && !defined(ARAGELI_LONG_LONG_SUPPORT)

    #define ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STDEX_DEF(T1)    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_FIRST_STD_DEF(T1)

    #define ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_STDEX_DEF    \
        ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_STD_DEF

#endif


ARAGELI_TYPE_PAIR_TRAITS_BUILTIN_STDEX_DEF;

//@}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/// Pattern for type_pair_traits in case if T2 is aggregate.
template <typename T1, typename T2>
struct type_pair_traits_for_unknown_and_aggregate
{
protected:

    typedef typename type_traits<T2>::element_type TE2;

public:

    static const bool is_specialized =
        type_traits<T1>::is_specialized &&
        type_traits<T2>::is_specialized &&
        type_traits<T2>::is_aggregate &&
        type_traits<TE2>::is_specialized;

    static const bool is_convertible =
        type_pair_traits<T1, TE2>::is_convertible;

    static const bool is_safe_convertible =
        type_pair_traits<T1, TE2>::is_safe_convertible;

    static const bool is_assignable =
        type_pair_traits<T1, TE2>::is_assignable;

    static const bool is_safe_assignable =
        type_pair_traits<T1, TE2>::is_safe_assignable;

    static const bool is_initializable =
        type_pair_traits<T1, TE2>::is_initializable;

    static const bool is_safe_initializable =
        type_pair_traits<T1, TE2>::is_safe_initializable;
};


/// Pattern for type_pair_traits in case if T2 likes our vector or matrix.
/** The difference from type_pair_traits_for_unknown_and_aggregate is
    an impossibility to initialize vector or matrix by a scalar type as
    usual way. */
template <typename T1, typename T2>
struct type_pair_traits_for_unknown_and_vector_matrix :
    public type_pair_traits_for_unknown_and_aggregate<T1, T2>
{
    static const bool is_initializable = false;
    static const bool is_safe_initializable = false;
};


/// Pattern for type_pair_traits in case if T2 is rational.
template <typename T1, typename T2>
struct type_pair_traits_for_unknown_and_rational :
    public type_pair_traits_for_unknown_and_aggregate<T1, T2>
{
protected:

    typedef typename type_traits<T2>::element_type TE2;

public:

    static const bool is_convertible =
        type_pair_traits_for_unknown_and_aggregate<T1, T2>::is_convertible &&
        type_traits<TE2>::has_unit;

    static const bool is_safe_convertible =
        type_pair_traits_for_unknown_and_aggregate<T1, T2>::is_safe_convertible &&
        type_traits<TE2>::has_unit;

    static const bool is_assignable =
        type_pair_traits_for_unknown_and_aggregate<T1, T2>::is_assignable &&
        type_traits<TE2>::has_unit;

    static const bool is_safe_assignable =
        type_pair_traits_for_unknown_and_aggregate<T1, T2>::is_safe_assignable &&
        type_traits<TE2>::has_unit;

    static const bool is_initializable =
        type_pair_traits_for_unknown_and_aggregate<T1, T2>::is_initializable &&
        type_traits<TE2>::has_unit;

    static const bool is_safe_initializable =
        type_pair_traits_for_unknown_and_aggregate<T1, T2>::is_safe_initializable &&
        type_traits<TE2>::has_unit;
};


/// Pattern for type_pair_traits for rational and unknown type.
template <typename T1, typename T2>
struct type_pair_traits_for_rational_and_unknown :
    public type_pair_traits_for_aggregate_and_unknown<T1, T2>
{
    static const bool is_specialized =
        type_traits<T1>::is_specialized &&
        type_traits<T2>::is_specialized;

    static const bool is_convertible =
        type_pair_traits<T1, T2>::is_convertible;

    static const bool is_safe_convertible =
        type_traits<T2>::is_rational &&
        type_traits<T2>::is_aggregate &&
        type_pair_traits<TE1, typename type_traits<T2>::element_type>::is_specialized &&
        type_pair_traits<TE1, typename type_traits<T2>::element_type>::is_safe_convertible;

    static const bool is_assignable = is_convertible;
    static const bool is_safe_assignable = is_safe_convertible;
    static const bool is_initializable = is_convertible;
    static const bool is_safe_initializable = is_safe_convertible;
};



/// Pattern for type_pair_traits in case if T2 is sparse polynomial.
template <typename T1, typename T2>
struct type_pair_traits_for_unknown_and_polynom :
    public type_pair_traits_for_unknown_and_aggregate<T1, T2>
{
protected:

    typedef typename T2::degree_type TD2;

public:

    static const bool is_convertible =
        type_pair_traits_for_unknown_and_aggregate<T1, T2>::is_convertible &&
        type_traits<TD2>::has_null;

    static const bool is_safe_convertible =
        type_pair_traits_for_unknown_and_aggregate<T1, T2>::is_safe_convertible &&
        type_traits<TD2>::has_null;

    static const bool is_assignable =
        type_pair_traits_for_unknown_and_aggregate<T1, T2>::is_assignable &&
        type_traits<TD2>::has_null;

    static const bool is_safe_assignable =
        type_pair_traits_for_unknown_and_aggregate<T1, T2>::is_safe_assignable &&
        type_traits<TD2>::has_null;

    static const bool is_initializable =
        type_pair_traits_for_unknown_and_aggregate<T1, T2>::is_initializable &&
        type_traits<TD2>::has_null;

    static const bool is_safe_initializable =
        type_pair_traits_for_unknown_and_aggregate<T1, T2>::is_safe_initializable &&
        type_traits<TD2>::has_null;
};

#endif


namespace _Internal
{

// WARNING! It's better if we make this case unrealizable.
template <typename T1, typename T2, bool ISSPEC, bool TheSecond>
struct power_union_type_helper
{
    typedef T1 type;
};

// WARNING! It's better if we make this case unrealizable.
template <typename T1, typename T2, bool TheSecond>
struct power_union_type_helper<T1, T2, false, TheSecond>
{
    typedef T1 type;
};

template <typename T1, typename T2>
struct power_union_type_helper<T1, T2, true, false>
{
    typedef T1 type;
};

template <typename T1, typename T2>
struct power_union_type_helper<T1, T2, true, true>
{
    typedef T2 type;
};

} // namespace _Internal


/// Choose a type that can represent any values of both T1 and T2.
/** WARNING! It's temporary limited implementation. */
template <typename T1, typename T2>
struct power_union_type :
    public _Internal::power_union_type_helper
    <
        T1,
        T2,
        type_pair_traits<T1, T2>::is_specialized &&
            type_pair_traits<T2, T1>::is_specialized,
        !type_pair_traits<T2, T1>::is_safe_convertible &&    // T1 dominates
        (
            type_pair_traits<T1, T2>::is_safe_convertible ||
                std::numeric_limits<T1>::is_specialized &&
                std::numeric_limits<T2>::is_specialized &&
                (
                    std::numeric_limits<T1>::is_integer &&
                    !std::numeric_limits<T2>::is_integer
                    ||
                    std::numeric_limits<T1>::is_integer &&
                    std::numeric_limits<T2>::is_integer &&
                    !std::numeric_limits<T1>::is_signed &&
                    std::numeric_limits<T2>::is_signed
                    ||
                    std::numeric_limits<T1>::is_integer &&
                    std::numeric_limits<T2>::is_integer &&
                    std::numeric_limits<T1>::is_signed &&
                    std::numeric_limits<T2>::is_signed &&
                    std::numeric_limits<T2>::digits >
                        std::numeric_limits<T1>::digits
                    ||
                    std::numeric_limits<T1>::is_integer &&
                    std::numeric_limits<T2>::is_integer &&
                    !std::numeric_limits<T1>::is_signed &&
                    !std::numeric_limits<T2>::is_signed &&
                    std::numeric_limits<T2>::digits >
                        std::numeric_limits<T1>::digits
                    ||
                    !std::numeric_limits<T1>::is_integer &&
                    !std::numeric_limits<T2>::is_integer &&
                    std::numeric_limits<T2>::digits >
                        std::numeric_limits<T1>::digits
                )
        )
    >
{};


/// Choose a type that can represent any product of values T1 and T2.
/** WARNING! It's temporary limited implementation. */
template <typename T1, typename T2>
struct power_product_type :
    public power_union_type<T1, T2>
{};


class big_int;

/// Choose a type that can represent any power T1^T2.
/** WARNING! It's temporary limited implementation. */
template <typename T1, typename T2>
struct power_power_type :
    public power_product_type<T1, T2>
{
    typedef big_int type;
};


} // namespace Arageli


#if 0
#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_TYPE_PAIR_TRAITS
    #include "type_pair_traits.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_TYPE_PAIR_TRAITS
#endif
#endif

#endif  //  #ifndef _ARAGELI_type_pair_traits_hpp_
