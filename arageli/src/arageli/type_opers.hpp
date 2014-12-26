/*****************************************************************************

    type_opers.hpp -- Operation with types.

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

    Some static mechanisms that deal with types: type comparision and
    type modification (result type is built by another type).

*/


#ifndef _ARAGELI_type_opers_hpp_
#define _ARAGELI_type_opers_hpp_

#include <cstddef>

#include "config.hpp"


//****************************************************************************


namespace Arageli
{


/// @name Boolean as type.

//@{

struct true_type
{
    static const bool value = true;
};

struct false_type
{
    static const bool value = false;
};

template <bool V> struct bool_type;

template <> struct bool_type<false>
{
    typedef false_type type;
    typedef false_type value_type;
    static const false_type value;
};


template <> struct bool_type<true>
{
    typedef true_type type;
    typedef true_type value_type;
    static const true_type value;
};


template <typename Out>
inline Out& operator<< (Out& out, const true_type&)
{
    out << true; return out;
}

template <typename Out>
inline Out& operator<< (Out& out, const false_type&)
{
    out << false; return out;
}


//@}


/// @name Type comparision for equality.
/** Make a comparision between two types (the same or not). */

//@{

/// General form for defferent types.
template <typename T1, typename T2>
struct equal_types
{
    static const bool bvalue = false;
    typedef false_type value_type;
    static const false_type value;
};

/// Specialized form for the same type.
template <typename T>
struct equal_types<T, T>
{
    static const bool bvalue = true;
    typedef true_type value_type;
    static const true_type value;
};

//@}


/// @name The 'const' modificator ommiting.
/** Make a type from the particular type with omitted
    const modificator (if present). */

//@{

/// General form (for types without exterior const).
template <typename T>
struct omit_const
{
    typedef T type;
};

/// Specialized form (for types with exterior const).
template <typename T>
struct omit_const<const T>
{
    typedef T type;
};

//@}


/// @name Omitting exterior asterisk '*' from the type.
/** Make a type from the particular type with omitted '*' (if present).
    For example, omit_asterisk<int*>::type is int and
    omit_asterisk<int**>::type is int*, but omit_asterisk<int>::type is
    just int. */

//@{

/// Omitting exterior: general form (for types without exterior '*').
template <typename T>
struct omit_asterisk
{
    typedef T type;
};

/// Omitting exterior: Specialized form (for types with exterior '*').
template <typename T>
struct omit_asterisk<T*>
{
    typedef T type;
};

//@}



/// @name Omitting reference symbol '&'.
/** Make a type from the particular type with omitted '&' (if present). */

//@{

/// General form (for types without exterior *).
template <typename T>
struct omit_ref
{
    typedef T type;
};

/// Specialized form (for types with exterior const).
template <typename T>
struct omit_ref<T&>
{
    typedef T type;
};

//@}


/// Omitting 'const &'.
/** Make a type from the particular type with omitted 'const &' (if present). */
template <typename T>
struct omit_const_ref
{
    typedef typename omit_const<typename omit_ref<T>::type>::type type;
};


/// @name Extract element type T from T* or T[].

//@{

template <typename T>
struct array_element
{
    typedef T type;
};

template <typename T, std::size_t I>
struct array_element<T[I]>
{
    typedef T type;
};

template <typename T>
struct array_element<T*>
{
    typedef T type;
};

//@}


/// Signed equivalent of type T. By the default it's the same type.
template <typename T> struct signed_type
{
    typedef T type;
};

template <> struct signed_type<char>
{
    typedef signed char type;
};

template <> struct signed_type<unsigned char>
{
    typedef signed char type;
};

template <> struct signed_type<unsigned short>
{
    typedef signed short type;
};

template <> struct signed_type<unsigned int>
{
    typedef signed int type;
};

template <> struct signed_type<unsigned long>
{
    typedef signed long type;
};

#ifdef ARAGELI_INT64_SUPPORT

    template <> struct signed_type<unsigned __int64>
    {
        typedef signed __int64 type;
    };

#endif

#ifdef ARAGELI_LONG_LONG_SUPPORT

    template <> struct signed_type<unsigned long long>
    {
        typedef signed long long type;
    };

#endif

/// Unsigned equivalent of type T. By the default it's the same type.
template <typename T> struct unsigned_type
{
    typedef T type;
};

template <> struct unsigned_type<char>
{
    typedef unsigned char type;
};

template <> struct unsigned_type<signed char>
{
    typedef unsigned char type;
};

template <> struct unsigned_type<signed short>
{
    typedef unsigned short type;
};

template <> struct unsigned_type<signed int>
{
    typedef unsigned int type;
};

template <> struct unsigned_type<signed long>
{
    typedef unsigned long type;
};

#ifdef ARAGELI_INT64_SUPPORT

    template <> struct unsigned_type<signed __int64>
    {
        typedef unsigned __int64 type;
    };

#endif

#ifdef ARAGELI_LONG_LONG_SUPPORT

    template <> struct unsigned_type<signed long long>
    {
        typedef unsigned long long type;
    };

#endif


} // namespace Arageli



#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_TYPE_OPERS
    #include "type_opers.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_TYPE_OPERS
#endif


#endif  //  #ifndef _ARAGELI_type_opers_hpp_
