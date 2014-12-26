/*****************************************************************************

    intalg.hpp

    This file is a part of Arageli library.

    Some functions in this file are part of Anna Bestaeva degree work 2006.
    They have been integrated into Arageli by Sergey Lyalin.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2005--2007 Sergey S. Lyalin
    Copyright (C) 2005--2006 Aleksey Bader
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
    \file intalg.hpp
    \brief Miscellaneous algorithm for integers.
*/

#ifndef _ARAGELI_intalg_hpp_
#define _ARAGELI_intalg_hpp_

#include "config.hpp"

#include <cmath>

#include "exception.hpp"
#include "type_traits.hpp"
#include "factory.hpp"
#include "cmp.hpp"
#include "gcd.hpp"
#include "powerest.hpp"

#include "std_import.hpp"


namespace Arageli
{


/// Returns inversion of a modulo n.
/** Requirement: n must be prime. */
template <typename T, typename T_factory>
inline T inverse_mod (const T& a, const T& m, const T_factory& tfctr)
{
    ARAGELI_ASSERT_0(is_positive(a));
    ARAGELI_ASSERT_0(m >= 2);

    T u, v;
    T r = euclid_bezout(a, m, u, v, tfctr);
    ARAGELI_ASSERT_0(is_unit(r));
    return is_negative(u) ? u + m : u;
}


/// Returns inversion of a modulo n.
/** Requirement: n must be prime. */
template <typename T>
inline T inverse_mod (const T& a, const T& n)
{
    return inverse_mod(a, n, factory<T>());
}


/// Exponentiates a in to power n modulo m via squaring and adding.
/** Requirements: n and m must be a positive. */
template <typename T, typename I, typename T_factory>
T power_mod (T a, I n, const T& m, const T_factory& tfctr);

/// Exponentiates a in to power n modulo m via squaring and adding.
/** Requirements: n and m must be a positive. */
template <typename T, typename I>
inline T power_mod (const T& a, const I& n, const T& m)
{
    return power_mod(a, n, m, factory<T>());
}


namespace _Internal
{

// WARNING! The following functions are here temporary
// tail their functionality will be clear.
// TODO: Understand what these functions do and give more complicated names.
// Then move into Arageli namespace directly.

/// TODO: remove old comments in russian
/**
* Requirements: a, b, N --- integers, N > 0, gcd(a, b) = 1
* \return gcd(a + cb, N) = gcd(a, b, N), 0 <= c <= 2(logN)^1.5
*/
template <typename T>
T conditioner (const T& a, const T& b, const T& N);

/// TODO: remove old comments in russian
/**
* \return either ñ: gcd(a + cb, N) = gcd(a, b, N), 0 <= c <= 2(logN)^1.5
*             or -1, if N divisors amount were increased.
* d --- N number decomposition into relatively prime numbers.
*/
template <typename T, typename V>
T conditioner (const T& _a, const T& _b, const T& N, V &d);

}


// WARNING! STRANGE FUNCTION
// TODO: Document!
template <typename T1, typename T2, typename T3>
T3 mod (const T1& a, const T2& b, const T3& d)
{
    // WARNING! div_mod(unit(a), b, d) is inverse of b modulo d
    return mod(a*div_mod(unit(a), b, d), d);
}


/// Compute a modulo b.
template <typename T1, typename T2>
inline T2 mod (const T1& a, const T2& b)
{
    return prrem(a, b);
}


/// Compute a/b modulo d.
template <typename T1, typename T2, typename T3>
T3 div_mod (const T1& a, const T2& b, const T3& d);


/// Compute residue of division a by b modulo d.
/** If d is a prime, the function returns 0. */
template <typename T1, typename T2, typename T3>
inline T3 rem_mod (const T1& a, const T2& b, const T3& d)
{
    return prrem(a, gcd(b, d));
}


/// Compute n/gcd(a, n).
template <typename T1, typename T2>
inline T2 ann (const T1& a, const T2& n)
{
    return n/gcd(a, n);
}


// TODO: Document it!
template <typename T1, typename T2, typename T3>
T3 quo_mod (const T1& a, const T2& b, const T3& d);

// TODO: Document it!
template <typename T>
T split (const T& a, const T& d);


// TODO: Document it!
template <typename T>
T stab (const T& a, const T& b, const T& N);


// TODO: Document it!
template <typename T>
T split_stab (const T& a, const T& b, const T& N);


// TODO: Document it and check name conflicts!
// Maybe more complicated name will be better.
template <typename T>
T unit (const T& a, const T& N);


// TODO: Document it!
template <typename T>
inline T stab (const T& a, const T& b, const T& N, const T& d)
{
    return stab(a, b, gcd(d, N));
}


// TODO: Document it!
template <typename T>
inline T split_stab (const T& a, const T& b, const T& N, const T& d)
{
    return split_stab(a, b, gcd(d, N));
}


// TODO: Document it!
template <typename T>
T split_mod (const T& a, const T& d);


// TODO: Document it!
template <typename T>
T stab_mod (const T& a, const T& b, const T& N);


// TODO: Document it!
template <typename T>
inline T stab_mod (const T& a, const T& b, const T& N, const T& d)
{
    return stab_mod(a, b, gcd(d, N));
}


/// Returns true iff a is invertible element modulo N.
template<typename T>
bool is_invertible_mod (const T& a, const T& N);


/// Compute number of bit in binary notation of a
template <typename T>
inline std::size_t nbits (const T& a)
{
    return nbits(a, factory<T>());
}


/// Compute number of bit in binary notation of a
template <typename T, typename T_factory>
std::size_t nbits(T a, const T_factory& tfctr);


/// Computes factorial via just successive multiplication (by definition).
/** Computes a*(a-1)*(a-2)* ... *3*2. */
template <typename T, typename T_factory>
T factorial_successive_multiplication (T a, const T_factory& tfctr);

/// Computes factorial via just successive multiplication (by definition).
template <typename T>
inline T factorial_successive_multiplication (const T& a)
{
    return factorial_successive_multiplication(a, factory<T>());
}


/// Computes factorial via even and odd multiplier separation.
template <typename T, typename T_factory>
T factorial_even_odd_multiplication (T a, const T_factory& tfctr);

/// Computes factorial via even and odd multiplier separation.
template <typename T>
inline T factorial_even_odd_multiplication (const T& a)
{
    return factorial_even_odd_multiplication(a, factory<T>());
}


/// Computes factorial of a via apropriate algorithm.
template <typename T, typename T_factory>
inline T factorial (const T& a, const T_factory& tfctr)
{
    return factorial_even_odd_multiplication(a, tfctr);
}

/// Computes factorial of a via apropriate algorithm.
template <typename T>
inline T factorial (const T& a)
{
    return factorial(a, factory<T>());
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// @name Jacobi symbol.

//@{

///    Computes the Jacobi symbol (a/n).
/**    Requirements: m > 1, m is odd. */
template <typename T, typename T_factory>
int jacobi (T a, T n, const T_factory& tfctr);


///    Computes the Jacobi symbol (a/n).
/**    Requirements: m > 1, m is odd. */
template <typename T>
inline int jacobi (const T& a, const T& n)
{
    return jacobi(a, n, factory<T>());
}


//@}

/// Integer square root of a.
/** Returns the biggest x such that x^2 <= a.
    Requirement: a is positive. */
template <typename T>
T intsqrt (const T& a);

/// The Shenks algorithm (square root of a modulo n, for n = 1 (mod 4)).
/**    Returns x such that x^2 = a (mod n).
    Requirement: 0 <= a < n, n is prime and n = 1 (mod 4). */
template <typename T, typename TT>
T sqrt_mod_shenks (const T& a, const T& n, const TT& tt);


/// The Shenks algorithm (square root of a modulo n, for n = 1 (mod 4)).
/**    Returns x such that x^2 = a (mod n).
    Requirement: 0 <= a < n, n is prime and n = 1 (mod 4). */
template <typename T>
inline T sqrt_mod_shenks (const T& a, const T& n)
{
    return sqrt_mod_shenks(a, n, type_traits<T>());
}


#if 0  //////////////  FOR THE FUTURE IMPLEMENTATION  ////////////////

/// Integer square root of a.
/** Returns the biggest x such that x^2 <= a.
    Requirement: a is positive. */
template <typename T>
inline T intsqrt (const T& a)
{
    return intsqrt(a, type_traits<T>());
}

/// The Shenks algorithm (square root of a modulo n, for n = 1 (mod 4)).
/** Returns x such that x^2 = a (mod n).
    Requirement: 0 <= a < n, n is prime and n = 1 (mod 4). */
template <typename T>
T sqrt_mod_shenks (const T& a, const T& n, const TT& tt);


/// The Shenks algorithm (square root of a modulo n, for n = 1 (mod 4)).
/** Returns x such that x^2 = a (mod n).
    Requirement: 0 <= a < n, n is prime and n = 1 (mod 4). */
template <typename T>
inline T sqrt_mod_shenks (const T& a, const T& n)
{
    return sqrt_mod_shenks(a, n, type_traits<T>());
}


/// Square root of a modulo n.
/** Returns x such that x^2 = a (mod n).
    Requirement: 0 <= a < n, n is prime. */
template <typename T>
T sqrt_mod (const T& a, const T& n, const TT& tt);


/// Square root of a modulo n.
/** Returns x such that x^2 = a (mod n).
    Requirement: 0 <= a < n, n is prime. */
template <typename T>
inline T sqrt_mod (const T& a, const T& n)
{
    return sqrt_mod(a, n, type_traits<T>());
}

#endif /////////////////////////////////////////////////////////////

} // namespace Arageli


namespace std
{

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// @name Specialization of std::sqrt for all integer type as Arageli::intsqrt.

//@{

inline char sqrt (char a)
{
    return Arageli::intsqrt(a);
}

inline unsigned char sqrt (unsigned char a)
{
    return Arageli::intsqrt(a);
}

inline signed char sqrt (signed char a)
{
    return Arageli::intsqrt(a);
}

inline unsigned short sqrt (unsigned short a)
{
    return Arageli::intsqrt(a);
}

inline signed short sqrt (signed short a)
{
    return Arageli::intsqrt(a);
}

inline unsigned int sqrt (unsigned int a)
{
    return Arageli::intsqrt(a);
}

inline signed int sqrt (signed int a)
{
    return Arageli::intsqrt(a);
}

inline unsigned long sqrt (unsigned long a)
{
    return Arageli::intsqrt(a);
}

inline signed long sqrt (signed long a)
{
    return Arageli::intsqrt(a);
}

#ifdef ARAGELI_INT64_SUPPORT

    inline unsigned __int64 sqrt (unsigned __int64 a)
    {
        return Arageli::intsqrt(a);
    }

    inline signed __int64 sqrt (signed __int64 a)
    {
        return Arageli::intsqrt(a);
    }

#endif

#ifdef ARAGELI_LONG_LONG_SUPPORT

    inline unsigned long long sqrt (unsigned long long a)
    {
        return Arageli::intsqrt(a);
    }

    inline signed long long sqrt (signed long long a)
    {
        return Arageli::intsqrt(a);
    }

#endif

//@}


}


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_INTALG
    #include "intalg.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_INTALG
#endif


#endif  //  #ifndef _ARAGELI_intalg_hpp_
