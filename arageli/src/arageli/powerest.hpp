/*****************************************************************************

    powerest.hpp -- Ќабор полезных математических утилит.

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

#ifndef _ARAGELI_powerest_hpp_
#define _ARAGELI_powerest_hpp_

#include "config.hpp"
#include "exception.hpp"
#include "factory.hpp"
#include "cmp.hpp"

#include "std_import.hpp"


namespace Arageli
{


/// Exponentiates a in to positive integer power n via squaring and adding.
/** Requirements: n != 0 or a != 0. */
template <typename T, typename I, typename T_factory>
T power (T a, I n, const T_factory& tfctr);

/// Exponentiates a in to positive integer power n via squaring and adding.
template <typename T, typename I>
inline T power (const T& a, const I& n)
{
    return power(a, n, factory<T>());
}


/// Exponentiates 2 in to positive integer power n via left shift operator.
template <typename T, typename I, typename T_factory>
inline T pow2 (I n, const T_factory& tfctr)
{
    T res = tfctr.unit();
    return res <<= n;
}

/// Exponentiates 2 in to positive integer power n via left shift operator.
template <typename T, typename I>
inline T pow2 (I n)
{
    T res = unit<T>();
    return res <<= n;
}


/// Returns the base-2 logarithm of x. The default version.
template <typename T>
inline T log2(T x)
{
    return log(x)/log(2.0l);
}


/// Performs division produced quotient and remainder.
/**    @param a dividend
    @param b divisor
    @param q the resulting quotient
    @param r the resulting remainder    */
template <typename T1, typename T2, typename Q, typename R>
inline void divide (const T1& a, const T2& b, Q& q, R& r)
{
    ARAGELI_ASSERT_0(!is_null(b));
    q = a/b;
    r = a%b;
    ARAGELI_ASSERT_1(a == b*q + r);
}

#if 0
/// Performs division produced quotient and remainder.
/**    @param a dividend
    @param b divisor
    @param q the resulting quotient
    @param r the resulting remainder    */
template <typename T1, typename T2, typename Q, typename R>
inline void divide (const T1& a, const T2& b, Q& q, R& r)
{
    divide
    (
        a,
        b,
        q,
        r,
        type_traits<T1>(),
        type_traits<T2>(),
        type_traits<Q>(),
        type_traits<R>()
    );
}
#endif

/// Performs division produced quotient and POSITIVE remainder (pr).
/**    @param a dividend
    @param b divisor
    @param q the resulting quotient
    @param r the resulting remainder, 0 <= r < |b|    */
template <typename T1, typename T2, typename Q, typename R>
inline void prdivide (const T1& a, const T2& b, Q& q, R& r)
{
    divide(a, b, q, r);

    if(is_negative(r))
    {
        if(is_positive(b))
        {
            --q;
            r += b;
        }
        else
        {
            ++q;
            r -= b;
        }
    }

    ARAGELI_ASSERT_1(a == b*q + r);
    ARAGELI_ASSERT_1(sign(r) >= 0);
    ARAGELI_ASSERT_1(is_positive(b) ? r < b : r < -b);
}

#if 0
/// Performs division produced quotient and POSITIVE remainder (pr-division).
/**    @param a dividend
    @param b divisor
    @param q the resulting quotient
    @param r the resulting remainder, 0 <= r < |b|    */
template <typename T1, typename T2, typename Q, typename R>
inline void prdivide (const T1& a, const T2& b, Q& q, R& r)
{
    prdivide
    (
        a,
        b,
        q,
        r,
        type_traits<T1>(),
        type_traits<T2>(),
        type_traits<Q>(),
        type_traits<R>()
    );
}
#endif

/// Returns quotient of pr-division of a by b.
template <typename T1, typename T2>
inline T1 prquot (const T1& a, const T2& b)
{
    T1 q;
    T2 r;
    prdivide(a, b, q, r);
    return q;
}

#if 0
/// Returns quotient of pr-division of a by b.
template <typename T1, typename T2>
inline T1 prquot (const T1& a, const T2& b)
{
    return prquot(a, b, type_traits<T1>(), type_traits<T2>());
}
#endif

/// Returns remainder of pr-division of a by b.
template <typename T1, typename T2>
inline T2 prrem (const T1& a, const T2& b)
{
    T1 q; T2 r;
    prdivide(a, b, q, r);
    return r;
}

#if 0
/// Returns remainder of pr-division of a by b.
template <typename T1, typename T2>
inline T2 prrem (const T1& a, const T2& b)
{
    return prrem(a, b, type_traits<T1>(), type_traits<T2>());
}
#endif

/// Returns square of a.
template <typename T>
inline T square (const T& a)
{
    return a*a;
}


inline int pow (int x, int y)
{
    return power(x, y);
}

#if 0
/// Returns square of a.
template <typename T>
inline T square (const T& a)
{
    return square(a, type_traits<T>());
}
#endif

template <typename T>
inline T round (const T& x)
{
    T floorx = floor(x);
    return (x - floorx < unit<T>()/2) ? floorx : ceil(x);
}


} // namespace Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_POWEREST
    #include "powerest.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_POWEREST
#endif


#endif  //  #ifndef _ARAGELI_powerest_hpp_
