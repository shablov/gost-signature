/*****************************************************************************

    gcd.hpp

    This file is a part of Arageli library.

    Some functions in this file are part of Anna Bestaeva degree work 2006.
    They have been integrated into Arageli by Sergey Lyalin.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2006 Anna Bestaeva

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
    \file gcd.hpp
    \brief GCD, LCM and relative routines.
*/

#ifndef _ARAGELI_gcd_hpp_
#define _ARAGELI_gcd_hpp_

#include "config.hpp"
#include "type_traits.hpp"
#include "frwrddecl.hpp"
#include "factory.hpp"
#include "cmp.hpp"
#include "vector.hpp"

#include "std_import.hpp"


namespace Arageli
{

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// @name The Euclidean algorithm.

//@{

/// Processes the Euclidean algorithm on a and b.
/**    If is_null(a) and is_null(b) then the function returns tfctr.unit(a). */
template <typename T, typename T_factory>
T euclid (T a, T b, const T_factory& tfctr);


/// Processes the Euclidean algorithm on a and b.
template <typename T>
inline T euclid (const T& a, const T& b)
{
    return euclid(a, b, factory<T>());
}


/// Processes the Euclidean algorithm on a and b.
/**    If is_null(a) and is_null(b) then the function returns tfctr.unit(a). */
template <typename T, typename T_factory>
T euclid_binary (T a, T b, const T_factory& tfctr);


/// Processes the Euclidean algorithm on a and b.
template <typename T>
inline T euclid_binary (const T& a, const T& b)
{
    return euclid_binary(a, b, factory<T>());
}


/// Computes Euclidean algorithm for each corresponding pair of a and b.
/**    The factory of tfctr of type T_factory is a factory for type compatible
    with T or T1. */
template <typename T, bool REFCNT, typename T_factory, typename T1, bool REFCNT1>
vector<T, REFCNT> euclid
(
    const vector<T, REFCNT>& a,
    const vector<T1, REFCNT1>& b,
    const T_factory& tfctr
);


/// Computes Euclidean algorithm for each corresponding pair of a and b.
template <typename T, bool REFCNT, typename T1, bool REFCNT1>
inline vector<T, REFCNT> euclid
(
    const vector<T, REFCNT>& a,
    const vector<T1, REFCNT1>& b
)
{
    return euclid(a, b, factory<T>());
}


/// The greatest common divisor and Bezout's coefficients.
/*    Function returns the greatest common divisor
    of two integers a and b and returns Bezout's coefficients
    u and v such that gcd = a * u + b * v. */
template <typename T, typename T_factory>
T euclid_bezout
(
    const T& a,
    const T& b,
    T& u,
    T& v,
    const T_factory& tfctr
);


/// The greatest common divisor and Bezout's coefficients.
/*    Function returns the greatest common divisor
    of two integers a and b and returns Bezout's coefficients
    u and v such that gcd = a * u + b * v. */
template <typename T>
inline T euclid_bezout (const T& a, const T& b, T& u, T& v)
{
    return euclid_bezout(a, b, u, v, factory<T>());
}

//@}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// @name The GCD and LCM algorithms.

//@{

template <typename T, typename T_factory>
inline T gcd
(
    const T& a,
    const T& b,
    const T_factory& tfctr,
    const type_category::type&
)
{
    return euclid(a, b, tfctr);
}

template <typename T, typename T_factory>
inline T gcd
(
    const T& a,
    const T& b,
    const T_factory& tfctr,
    const type_category::real&
)
{
    return unit(a);
}

template <typename T, typename T_factory>
inline T gcd
(
    const T& a,
    const T& b,
    const T_factory& tfctr,
    const type_category::integer&
)
{
    return euclid/*_binary*/(a, b, tfctr);
}

/// Computes GCD for a and b.
template <typename T, typename T_factory>
inline T gcd (const T& a, const T& b, const T_factory& tfctr)
{
    return gcd(a, b, tfctr, type_traits<T>::category_value);
}

/// Computes GCD for a and b.
template <typename T>
inline T gcd (const T& a, const T& b)
{
    return gcd(a, b, factory<T>());
}

/// Computes GCD for a and b.
template <typename T1, typename T2>
inline T1 gcd (const T1& a, const T2& b)
{
    return gcd(a, T1(b));
}

/// Computes GCD for all items of vector 'x'.
template <typename T, bool REFCNT, typename T_factory>
T gcd (const vector<T, REFCNT>& x, const T_factory& tfctr);

/// Computes GCD for all items of vector 'x'.
template <typename T, bool REFCNT>
inline T gcd (const vector<T, REFCNT>& x)
{
    return gcd(x, factory<T>());
}


/// Compute GCD(a, b, c)
template <typename T>
inline T gcd3 (const T& a, const T& b, const T& c)
{
    return gcd(gcd(a, b), c);
}


/// Extended GCD; abs(u*z - w*v) == 1; a*w + b*z == 0.
template <typename T>
T gcdex
(
    const T& a,
    const T& b,
    T&u,
    T& v,
    T& w,
    T& z
);


// TODO: Document it!
template <typename T>
T gcdex
(
    const T& a,
    const T& b,
    const T& N,
    T&u,
    T& v,
    T& w,
    T& z
);


/// Computes GCD for each corresponding pair of a and b.
template
<
    typename T,
    bool REFCNT,
    typename T_factory,
    typename T1,
    bool REFCNT1
>
vector<T, REFCNT> gcd_vec
(
    const vector<T, REFCNT>& a,
    const vector<T1, REFCNT1>& b,
    const T_factory& tfctr
);


/// Computes GCD for each corresponding pair of a and b.
template
<
    typename T,
    bool REFCNT,
    typename T_factory,
    typename T1,
    bool REFCNT1
>
inline vector<T, REFCNT> gcd
(
    const vector<T, REFCNT>& a,
    const vector<T1, REFCNT1>& b,
    const T_factory& tfctr
)
{
    return gcd_vec(a, b, tfctr);
}


/// Computes GCD for each corresponding pair of a and b.
template <typename T, bool REFCNT, typename T_factory>
inline vector<T, REFCNT> gcd
(
    const vector<T, REFCNT>& a,
    const vector<T, REFCNT>& b,
    const T_factory& tfctr
)
{
    return gcd_vec(a, b, tfctr);
}


/// Computes GCD for each corresponding pair of a and b.
template <typename T, bool REFCNT, typename T1, bool REFCNT1>
inline vector<T, REFCNT> gcd
(
    const vector<T, REFCNT>& a,
    const vector<T1, REFCNT1>& b
)
{
    return gcd(a, b, factory<T>());
}


/// Computes GCD for each corresponding pair of a and b.
template <typename T, bool REFCNT>
inline vector<T, REFCNT> gcd
(
    const vector<T, REFCNT>& a,
    const vector<T, REFCNT>& b
)
{
    return gcd(a, b, factory<T>());
}


/// Computes LCM for a and b.
template <typename T, typename T_factory>
inline T lcm (const T& a, const T& b, const T_factory& tfctr)
{
    return a*b/gcd(a, b, tfctr);
}

/// Computes LCM for a and b.
template <typename T>
inline T lcm (const T& a, const T& b)
{
    return lcm(a, b, factory<T>());
}

/// Computes LCM for all items of vector 'x'.
template <typename T, bool REFCNT, typename T_factory>
T lcm (const vector<T, REFCNT>& x, const T_factory& tfctr);

/// Computes LCM for all items of vector 'x'.
template <typename T, bool REFCNT>
inline T lcm (const vector<T, REFCNT>& x)
{
    return lcm(x, factory<T>());
}


/// Computes LCM for each corresponding pair of a and b.
template
<
    typename T,
    bool REFCNT,
    typename T_factory,
    typename T1,
    bool REFCNT1
>
vector<T, REFCNT> lcm_vec
(
    const vector<T, REFCNT>& a,
    const vector<T1, REFCNT1>& b,
    const T_factory& tfctr
);


/// Computes LCM for each corresponding pair of a and b.
template
<
    typename T,
    bool REFCNT,
    typename T_factory,
    typename T1,
    bool REFCNT1
>
inline vector<T, REFCNT> lcm
(
    const vector<T, REFCNT>& a,
    const vector<T1, REFCNT1>& b,
    const T_factory& tfctr
)
{
    return lcm_vec(a, b, tfctr);
}


/// Computes LCM for each corresponding pair of a and b.
template <typename T, bool REFCNT, typename T_factory>
inline vector<T, REFCNT> lcm
(
    const vector<T, REFCNT>& a,
    const vector<T, REFCNT>& b,
    const T_factory& tfctr
)
{
    return lcm_vec(a, b, tfctr);
}


/// Computes LCM for each corresponding pair of a and b.
template <typename T, bool REFCNT, typename T1, bool REFCNT1>
inline vector<T, REFCNT> lcm
(
    const vector<T, REFCNT>& a,
    const vector<T1, REFCNT1>& b
)
{
    return lcm(a, b, factory<T>());
}


template <typename T, bool REFCNT>
inline vector<T, REFCNT> lcm
(
    const vector<T, REFCNT>& a,
    const vector<T, REFCNT>& b
)
{
    return lcm(a, b, factory<T>());
}

//@}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// @name A test for relative primality.

//@{

/// Returns true if two objects are coprime.
template <typename T, typename T_factory>
inline bool is_coprime (const T& a, const T& b, const T_factory& tfctr)
{
    return is_unit(gcd(a, b, tfctr));
}

/// Returns true if two objects are coprime.
template <typename T>
inline bool is_coprime (const T& a, const T& b)
{
    return is_coprime(a, b, factory<T>());
}

/// Returns true if all objects in the vector are coprime.
template <typename T, bool REFCNT, typename T_factory>
inline bool is_coprime (const vector<T, REFCNT>& x, const T_factory& tfctr)
{
    return is_unit(gcd(x, tfctr));
}

/// Returns true if all objects in the vector are coprime.
template <typename T, bool REFCNT>
inline bool is_coprime (const vector<T, REFCNT>& x)
{
    return is_coprime(x, factory<T>());
}

//@}


} // namespace Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_GCD
    #include "gcd.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_GCD
#endif


#endif  //  #ifndef _ARAGELI_gcd_hpp_
