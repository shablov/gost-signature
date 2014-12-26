/*****************************************************************************

    cmp.hpp -- Comparisions.

    This file is a part of Arageli library.

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

/**
    \file cmp.hpp
    \brief A lot of functions for making comparison.

    Generic cmp function and several particular case testing functions.
*/

#ifndef _ARAGELI_cmp_hpp_
#define _ARAGELI_cmp_hpp_

#include <complex>

#include "config.hpp"
#include "exception.hpp"
#include "factory.hpp"

#include "std_import.hpp"


namespace Arageli
{


/// Performs Arageli like comparision.
/** There is specialization of this function (or an overloaded version) for each
    main Arageli object.
    Returns
    -  0  if a == b,
    -  -1 if a < b,
    -  1  if a > b. */
template <typename T1, typename T2>
inline int cmp (const T1& a, const T2& b)
{
    if(a < b)
        return -1;
    else if(a > b)
        return +1;
    else
    {
        ARAGELI_ASSERT_0(a == b);
        return 0;
    }
}


/// Lexicografically compares two complexes as pairs (Re, Im).
template <typename T1, typename T2>
inline int cmp (const std::complex<T1>& a, const std::complex<T2>& b)
{
    if(int reres = cmp(a.real(), b.real()))
        return reres;
    else
        return cmp(a.imag(), b.imag());
}


/// Sign of element.
/** Returns +1, 0 or -1. */
template <typename T>
inline int sign (const T& x)
{
    return cmp(x, null(x));
}


/// Returns true if x is even.
template <typename T>
inline bool is_even (const T& x)
{
    return is_null(x & unit(x));
}


/// Returns true if x is odd.
template <typename T>
inline bool is_odd (const T& x)
{
    return !is_null(x & unit(x));
}


/// Returns true if a is divisible by b.
template <typename T1, typename T2>
inline bool is_divisible (const T1& a, const T2& b)
{
    return is_null(a%b);
}


/// Returns true if x is greater then null.
template <typename T>
inline bool is_positive (const T& x)
{
    return x > null(x);
}

/// Returns true if x is less then null.
template <typename T>
inline bool is_negative (const T& x)
{
    return x < null(x);
}


/// Returns true if x is integer
template <typename T>
inline bool is_integer (const T& x)
{
    return is_null(frac(x));
}


} // namespace Arageli


//#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
//    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_CMP
//    #include "cmp.cpp"
//    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_CMP
//#endif


#endif  //  #ifndef _ARAGELI_cmp_hpp_
