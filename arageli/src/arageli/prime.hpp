/*****************************************************************************

    prime.hpp -- tests for prime number and factorization foutine.

    This file is a part of Arageli library.

    Copyright (C) 1999--2006, 2010 Nikolai Yu. Zolotykh
    Copyright (C) 2006 Aleksey Bader

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

#ifndef _ARAGELI_prime_hpp_
#define _ARAGELI_prime_hpp_

#include "config.hpp"
#include "factory.hpp"
#include "cmp.hpp"
#include "vector.hpp"

#include "std_import.hpp"


namespace Arageli
{

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// @name A test for primality.

//@{

/// Determines if x is prime via consecutive division by sample divisors.
/** Very simple and slow algorithm for big x. */
template <typename T, typename T_factory>
bool is_prime_division (const T& x, const T_factory& tfctr);

/// Determines if x is prime via consecutive division by sample divisors.
/** Very simple and slow algorithm for big x. */
template <typename T>
inline bool is_prime_division (const T& x)
{
    return is_prime_division(x, factory<T>());
}

/// Determines if x is prime via Miller-Rabin algorithm.
/** Very fast algorithm for big x. Returns true if x is pseudo-prime to n bases
    and false if x is composite number.*/
template <typename T, typename N, typename T_factory>
bool is_pseudoprime_miller_rabin (const T& x, const N& n, const T_factory& tfctr);

/// Determines if x is prime.
/** Very slow algorithm for any prime x. */
template <typename T>
bool is_prime_AKS_classic (const T& n);

/// Determines if x is prime.
/** Very slow algorithm for any prime x. */
template <typename T>
bool is_prime_AKS (const T& n);

/// Determines if x is prime via Miller-Rabin algorithm.
/** Very fast algorithm for big x. Returns true if x is pseudo-prime to n bases
    and false if x is composite number.*/
template <typename T, typename N>
inline bool is_pseudoprime_miller_rabin (const T& x, const N& n)
{
    return is_pseudoprime_miller_rabin(x, n, factory<T>());
}

/// Determines if x is prime via Solovey-Strassen algorithm.
/** More fast algorithm for big x, than simple division. */
template <typename T, typename N, typename T_factory>
bool is_pseudoprime_solovay_strassen
(const T& x, const N& n, const T_factory& tfctr);

/// Determines if x is prime via Solovey-Strassen algorithm.
/** More fast algorithm for big x, than simple division. */
template <typename T, typename N>
inline bool is_pseudoprime_solovay_strassen (const T& x, const N& n)
{
    return is_pseudoprime_solovay_strassen(x, n, factory<T>());
}


/// Determines if x is prime via appropriate algorithm for T.
/** By default it is just call is_prime_division function. */
template <typename T, typename T_factory>
inline bool is_prime (const T& x, const T_factory& tfctr)
{
    return is_prime_division(x, tfctr);
}


/// Determines if x is prime via appropriate algorithm for T.
template <typename T>
inline bool is_prime (const T& x)
{
    return is_prime(x, factory<T>());
}

/// Determines if x is probably prime via appropriate algorithm for T.
/** By default it is just call is_pseudoprime_solovay_strassen function. */
template <typename T, typename T_factory>
inline bool is_probably_prime (const T& x, const T_factory& tfctr)
{
    return is_pseudoprime_solovay_strassen(x, 10, tfctr);    // WARNING! 10
}


/// Determines if x is probably prime via appropriate algorithm for T.
template <typename T>
inline bool is_probably_prime (const T& x)
{
    return is_probably_prime(x, factory<T>());
}

/// Fills sequence with first N primes.
/** Fills sequence that begin on 'primes' with first prime numbers, i.g. 2, 3, ...
Number of generated primes is N. The function returns iterator on end of
generated sequence. */
template <typename Out, typename T>
Out small_primes (Out primes, const T& N);

/**
Return 1 if n is prime
Return 0 if n is composite
Return -1 if n has no small factors
*/
template <typename T, typename N>
int is_prime_small_primes_division (const T& n, const N& np);

/**
Return 1 if n is prime
Return 0 if n is composite
Return -1 if n has no small factors
*/
template <typename T>
int is_prime_small_primes_division(const T& n)
{
    return
        is_prime_small_primes_division
        (
            n,
            ARAGELI_IS_PRIME_SMALL_PRIMES_DIVISION_NUMBER
        );
}

/// Determines if x is composite.
template <typename T, typename T_factory>
inline bool is_composite (const T& x, const T_factory& tfctr)
{
    return
        !is_null(x) &&
        !is_unit(x) &&
        !is_opposite_unit(x) &&
        !is_prime(x, tfctr);
}


/// Determines if x is composite.
template <typename T>
inline bool is_composite (const T& x)
{
    return is_composite(x, factory<T>());
}

// @}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// @name Factorization.

//@{

/// Factorizes x into a set of prime factors via consecutive division.
/** Very simple and slow algorithm for big x. The resulting prime multipliers
    in res are ascending sorted. Number of copies of each multiplier is equal
    to its repetition factor, i.e. after the function call the expression
    (product(res) == x) is true. */
template <typename T1, typename T2, bool REFCNT2, typename T_factory>
vector<T2, REFCNT2>& factorize_division
(
    T1 x,
    vector<T2, REFCNT2>& res,
    const T_factory& tfctr
);


/// Factorizes x into a set of prime factors via consecutive division.
template <typename T1, typename T2, bool REFCNT2>
inline vector<T2, REFCNT2>& factorize_division
(const T1& x, vector<T2, REFCNT2>& res)
{
    return factorize_division(x, res, factory<T1>());
}


/// Factorizes x into a set of prime factors via consecutive division.
template <typename T, typename T_factory>
inline vector<T, true> factorize_division
(const T& x, const T_factory& tfctr)
{
    vector<T, true> res;
    return factorize_division(x, res, tfctr);
}


/// Factorizes x into a set of prime factors via consecutive division.
template <typename T>
inline vector<T, true> factorize_division (const T& x)
{
    return factorize_division(x, factory<T>());
}


/// Factorizes x into a set of prime factors via appropriate algorithm for T.
/** By default it is just call factorize_division function. */
template <typename T1, typename T2, bool REFCNT2, typename T_factory>
inline vector<T2, REFCNT2>& factorize
(
    const T1& x,
    vector<T2, REFCNT2>& res,
    const T_factory& tfctr
)
{
    return factorize_division(x, res, tfctr);
}


/// Factorizes x into a set of prime factors via appropriate algorithm for T.
template <typename T1, typename T2, bool REFCNT2>
inline vector<T2, REFCNT2>& factorize
(const T1& x, vector<T2, REFCNT2>& res)
{
    return factorize(x, res, factory<T1>());
}


/// Factorizes x into a set of prime factors via appropriate algorithm for T.
template <typename T, typename T_factory>
inline vector<T, true> factorize
(const T& x, const T_factory& tfctr)
{
    vector<T, true> res;
    return factorize(x, res, tfctr);
}


/// Factorizes x into a set of prime factors via appropriate algorithm for T.
template <typename T>
inline vector<T, true> factorize (const T& x)
{
    return factorize(x, factory<T>());
}

//@}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// @name Partial factorization.

//@{

/// Partialy factorizes x into a set of prime factors via test division.
template
<
    typename T1,
    typename T2, bool REFCNT2,
    typename T3, typename T4
>
vector<T2, REFCNT2>& partial_factorize_division
(
    T1 x,
    vector<T2, REFCNT2>& res,
    const T3& max,
    T4& rest
);

/// Pollard p-1 method (Simple variant).
/**
    Pollard p-1 method (Simple variant)
*/
template<typename T, typename N, typename T_factory>
T pollard_pm1
(
    const T& n,
    const N& no_of_iter,
    const T_factory& tfctr
);

/// Pollard p-1 method (Simple variant).
/**
    Pollard p-1 method (Simple variant)
*/
template<typename T, typename N>
inline T pollard_pm1
(
    const T& n,
    const N& no_of_iter = 10000
)
{
    return pollard_pm1(n, no_of_iter, factory<T>());
}


/// Pollard's rho algorithm.
/**
    Return a non-trivial factor of n or 1 if the algorithm failed.
*/
template<typename T, typename T_factory>
T rho_pollard (const T& n, const T_factory& tfctr);


/// Pollard's rho algorithm.
/**
    Return a non-trivial factor of n or 1 if the algorithm failed.
*/
template<typename T>
inline T rho_pollard (const T& n)
{
    return rho_pollard(n, factory<T>());
}

/// Pollard's rho algorithm.
/**
    Brent's improovement of Pollard's rho algorithm.
    Return a non-trivial factor of n or 1 if the algorithm failed.
    This algorithm can fail for composite n.
*/
template<typename T, typename N, typename T_factory>
T brent (const T& n, N no_of_iter, const T_factory& tfctr);

/// Pollard's rho algorithm.
/**
    Brent's improovement of Pollard's rho algorithm.
    Return a non-trivial factor of n or 1 if the algorithm failed.
    This algorithm can fail for composite n.
*/
template<typename T, typename N>
inline T brent (const T& n, N no_of_iter)
{
    return brent(n, no_of_iter, factory<T>());
}

/// Partialy factorizes x into a set of prime factors via test division.
template <typename T, typename T3, typename T4>
inline vector<T, true> partial_factorize_division
(const T& x, const T3& max, T4& rest)
{
    vector<T, true> res;
    return partial_factorize_division(x, res, max, rest);
}


/// Partialy factorizes x into a set of prime factors via test division.
template
<
    typename T1,
    typename TT1, typename T2, bool REFCNT2,
    typename T3
>
inline vector<T2, REFCNT2>& partial_factorize_division
(
    const T1& x,
    vector<T2, REFCNT2>& res,
    const T3& max
)
{
    T1 rest;    // not used
    return partial_factorize_division(x, res, max, rest);
}


/// Partialy factorizes x into a set of prime factors via test division.
template <typename T, typename T3>
inline vector<T, true> partial_factorize_division
(const T& x, const T3& max)
{
    T rest;    // not used
    return partial_factorize_division(x, max, rest);
}


/// Partialy factorizes x into a set of prime factors via appropriate algorithm for T.
template
<
    typename T1,
    typename T2, bool REFCNT2,
    typename T3, typename T4
>
inline vector<T2, REFCNT2>& partial_factorize
(
    const T1& x,
    vector<T2, REFCNT2>& res,
    const T3& max,
    T4& rest
)
{
    return partial_factorize_division(x, res, max, rest);
}


/// Partialy factorizes x into a set of prime factors via appropriate algorithm for T.
template <typename T, typename T3, typename T4>
inline vector<T, true> partial_factorize
(const T& x, const T3& max, T4& rest)
{
    vector<T, true> res;
    return partial_factorize(x, res, max, rest);
}


/// Partialy factorizes x into a set of prime factors via appropriate algorithm for T.
template
<
    typename T1,
    typename TT1, typename T2, bool REFCNT2,
    typename T3
>
inline vector<T2, REFCNT2>& partial_factorize
(
    const T1& x,
    vector<T2, REFCNT2>& res,
    const T3& max
)
{
    T1 rest;    // not used
    return partial_factorize(x, res, max, rest);
}


/// Partialy factorizes x into a set of prime factors via appropriate algorithm for T.
template <typename T, typename T3>
inline vector<T, true> partial_factorize
(const T& x, const T3& max)
{
    T rest;    // not used
    return partial_factorize(x, max, rest);
}


/// Finds rest and nm such that x = rest * m^nm and rest is not divisible by m; returns reference to nm.
template <typename T1, typename T2, typename T3, typename T4>
const T4& factorize_multiplier (T1 x, const T2& m, T3& rest, T4& nm);


/// Finds rest and nm such that x = rest * m^nm and rest is not divisible by m; returns nm.
template <typename T1, typename T2, typename T3>
T1 factorize_multiplier (const T1& x, const T2& m, T3& rest)
{
    T1 nm;
    factorize_multiplier(x, m, rest, nm);
    return nm;
}


/// Finds rest and nm such that x = rest * m^nm and rest is not divisible by m; returns nm.
template <typename T1, typename T2>
T1 factorize_multiplier (const T1& x, const T2& m)
{
    T1 rest;
    return factorize_multiplier(x, m, rest);
}

// @}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// @name Prime number generators.

//@{

/// Finds the next after x prime number via successive test for each number.
/** Calls is_prime for each number greater then x and returns
    when is_prime returns true. */
template <typename T>
T next_prime_successive_checking (T x);


/// Finds the previous before x prime number via successive test for each number.
/** Calls is_prime for each number less then x and returns
    when is_prime returns true. */
template <typename T>
T prev_prime_successive_checking (T x);


/// Finds the next after x prime number via appropriate algorithm.
template <typename T>
inline T next_prime (const T& x)
{
    return next_prime_successive_checking(x);
}


/// Finds the previous before x prime number via appropriate algorithm.
template <typename T>
inline T prev_prime (const T& x)
{
    return prev_prime_successive_checking(x);
}

/// Finds the next after x probably prime number via appropriate algorithm.
template <typename T>
inline T next_probably_prime (T x);


/// Finds the previous before x probably prime number via appropriate algorithm.
template <typename T>
inline T prev_probably_prime (T x);


//@}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// @Generation of big prime numbers.

//@{


/// Determines if 2^n-1 is Mersenn prime number.
/** Determination doesn't go by Luka-Lemer criteria (1890, 1930).
    Be careful: the determination is time expensive for large n. */
template <typename T>
inline bool is_mersen_prime_degree (const T& n)
{
    return n == next_mersen_prime_degree(n-1); /* WARNING! TEMPORARY! */
}


/// Gives such k > n that 2^k-1 is Mersenn prime number, and for all k > m > n, 2^m-1 is not.
/** Determination by a table with all known at the moment Mersenn numbers.
    If the next number is unknown, the function throws an exception. */
template <typename T>
T next_mersen_prime_degree (const T& n);


//@}


} // namespace Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_PRIME
    #include "prime.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_PRIME
#endif


#endif  //  #ifndef _ARAGELI_prime_hpp_
