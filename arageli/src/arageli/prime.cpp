/*****************************************************************************

    prime.cpp -- see prime.hpp.

    This file is a part of Arageli library.

    Copyright (C) 2005, 2010 Nikolai Yu. Zolotykh
    Copyright (C) 2006 Aleksey Bader
    Copyright (C) 2008 Sergey S. Lyalin

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

#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_PRIME)

#include <list>
#include <iterator>

#include "prime.hpp"
#include "exception.hpp"
#include "powerest.hpp"
#include "setenumrnd/grid.hpp"


namespace Arageli
{

template <typename Out, typename T>
Out small_primes (Out primes, const T& N)
{
    // WARNING! Why do we need to generate the same primes every time?

    typedef typename std::iterator_traits<Out>::value_type Val;
    typedef typename std::iterator_traits<Out>::reference Ref;

    primes[0] = 2;
    Val n = 3;

    for (T j = 1; j < N; j++)
    {
        primes[j] = n;
        for(;;)
        {
            n += 2;
            for(T k = 1; ; k++)
            {
                Val q, r;
                Ref pk = primes[k];
                divide(n, pk, q, r);
                if(is_null(r))
                    break;
                if(q <= pk)
                    goto next_prime;
            }
        }
next_prime: ;
    }

    return primes + N;
}

template <typename T, typename T_factory>
bool is_prime_division (const T& x, const T_factory& tfctr)
{
    T d = tfctr.unit(x);    // d = 1
    if(x <= d)
        return false;
    ++d;    // d = 2
    if(x == d)
        return true;
    if(is_null(x % d))
        return false;
    ++d;    // d = 3
    if(x == d)
        return true;
    if(is_null(x % d))
        return false;

    d += 2;

    T q, r;

    for(;;)
        for(unsigned s = 2; s <= 4; s += 2)
        {
            divide(x, d, q, r);
            if(q < d)
                return true;
            if(is_null(r))
                return false;
            d += s;
        }
}

template <typename T, typename N, typename T_factory>
bool is_pseudoprime_solovay_strassen (const T& x, const N& n, const T_factory& tfctr)
{
    if (x<=unit(x))
        return false;
    if (x == T(2))
        return true;
    T jac;
    T a;

    // We need integer random numbers from [2, x-1].
    typedef set::grid1<T, T> int_grid;
    int_grid int_2_xm1(2, x-1); // integer set from [2, x-1]
    rnd::equiprob<int_grid> gen_2_xm1(int_2_xm1);   // random generator from [2, x-1]

    for (N i = null(n); i < n; ++i)
    {
        a = gen_2_xm1();

        if (!is_unit(gcd(a,x)))
            return false;
        jac = jacobi(a, x);
        if (is_opposite_unit(jac))
            jac += x;
        if (power_mod(a, (x-1)/2, x) != jac )
            return false;
    }
    return true;
}

template <typename T, typename N, typename T_factory>
bool is_pseudoprime_miller_rabin (const T& x, const N& n, const T_factory& tfctr)
{
    ARAGELI_ASSERT_0(is_positive(x));
    if(is_unit(x))
        return false;
    if(x == 2)
        return true;
    if(is_even(x))
        return false;
    T a, q, c;
    N k;
    const T unt = unit(x);
    const T start_q = (x - 1) >> 1;

    // We need integer random numbers from [2, x-1].
    typedef set::grid1<T, T> int_grid;
    int_grid int_2_xm1(2, x-1); // integer set from [2, x-1]
    rnd::equiprob<int_grid> gen_2_xm1(int_2_xm1);   // random generator from [2, x-1]

    for (N i = null(n); i < n; ++i)
    {
        a = gen_2_xm1();

        if(!is_unit(gcd(a,x)))
            return false;
        q = start_q;
        k = unit(n);
        while (is_even(q))
        {
            q = q >> 1;
            ++k;
        }
        c = power_mod(a, q, x);
        if (is_unit(c) || c == x - unt)
            continue;

        N j = unit(k);
        for (; j < k; ++j)
        {
            (c *= c) %= x;
            if (c == x - unt)
                break;
            if (is_unit(c))
                return false;
        }
        if (j == k)
            return false;
    }
    return true;
}

template <typename T>
T LPF(T p)
{
    T s = p;
    T i = 2;
    T j = intsqrt(s);
    while (i <= j)
    {
        if (s%i == 0)
        {
            s=s/i;
            j = intsqrt(s);
        }
        else
            ++i;
    }
    return s;
}

template <typename T>
bool is_power_of_prime(T n, long b)
{
    T p = square(nbits(n)/b + 1);
    T q;
    while (true)
    {
        q = (p*(b-1) + n/(power(p,b-1)))/b;
        if (q>=p)
            break;
        p=q;
    }
    if ( n == power(p,b))
        return true;
    return false;
}

template <typename T>
bool is_prime_AKS_classic (const T& n)
{
    T N = intsqrt(n);
    if (square(N) == n)
        return false;
    int i;
    int len = n.length();
    for (i = 3; i < len; i+=2)
        if (is_power_of_prime(n,i))
            return false;

    T r = 3;
    T q, s;
    int len4 = 4*len;
    while (r<N)
    {
        if (is_prime(r))
        {
            if (gcd(r,n) != 1)
                return false;
            q = LPF(r-1);
            if
            (
                (q >= 4*intsqrt(r)+len4) &&
                (power_mod((n%r),((r-1)/q),r) <= 1)
            )
                break;
        }
        r+=2;
    }
    if (r >= N)
        return true;
    sparse_polynom<T> mod_part;
    sparse_polynom<T> right_part;
    mod_part = power(sparse_polynom<T>("x"), r)-1;
    right_part = power_mod(sparse_polynom<T>("x"), n, mod_part);
    s = 2*N*len;
    for(i = 1; i <= s; ++i)
    {
        sparse_polynom<T> left_part("x");
        left_part -= i;
        left_part = power_mod(left_part, n, mod_part);
        left_part += i;
        sparse_polynom_reduction_mod(left_part, n);
        if (left_part != right_part)
            return false;
    }
    return true;
}

template<typename T>
long primitive_root(T n, T r)
{
    long i;
    long r_len = r.length();
    for (i = 1; i <= r_len; ++i)
    {
        if( is_unit(power_mod(n%r,i,r)) )
            return i;
    }
    return 0;
}

template <typename T>
bool is_prime_AKS (const T& n)
{
    T N = intsqrt(n);
    if (square(N) == n)
        return false;
    long i;
    long len = n.length();
    for (i = 3; i < len; i+=2)
        if (is_power_of_prime(n,i))
            return false;
    T r = next_prime(power(len,2));
    T q, s;

    for (;primitive_root(n,r+2);r=next_prime(r+2));
    for (q = 3; (is_unit(gcd(q,n)) || (gcd(q,n) == n)) && (q < r); ++q);
    if (q<r)
    {
        return false;
    }
    else if (n<=r)
    {
        return false;
    }

    sparse_polynom<T> mod_part;
    sparse_polynom<T> right_part;
    mod_part = power(sparse_polynom<T>("x"), r)-1;
    right_part = power_mod(sparse_polynom<T>("x"), n, mod_part);
    s = intsqrt(r)*len;
    for(i = 1; i <= s; ++i)
    {
        sparse_polynom<T> left_part("x");
        left_part -= i;
        left_part = power_mod(left_part, n, mod_part);
        left_part += i;
        sparse_polynom_reduction_mod(left_part, n);
        if (left_part != right_part)
            return false;
    }
    return true;
}

template <typename T, typename N>
int is_prime_small_primes_division (const T& n, const N& np)
{
    ARAGELI_ASSERT_0(n > unit(n));

    vector<int, false> small_primes;
    small_primes(small_primes, np);    // WARNING! Why is this every time generated?

    for(int j = 0; j < np; j++)
    {
        if(n == small_primes[j])
            return 1;
        if(is_divisible(n, small_primes[j]))
            return 0;
    }

    // n doesn't have prime divisors less or equal than np-th prime number
    return -1;
}

template <typename T1, typename T2, bool REFCNT2, typename T_factory>
vector<T2, REFCNT2>& factorize_division
(
    T1 x,
    vector<T2, REFCNT2>& res,
    const T_factory& tfctr
)
{
    ARAGELI_DEBUG_EXEC_2(T1 xx = x);    // save original value
    ARAGELI_ASSERT_0(is_positive(x));

    if(is_unit(x))
    {
        res.resize(0);
        return res;
    }

    T1 d = tfctr.unit(x);   // d = 1
    ARAGELI_ASSERT_0(x >= d);
    ++d;    // d = 2

    std::list<T2> resbuf;

    while(is_null(x % 2))
    {
        x >>= 1;
        resbuf.push_back(d);
    }

    T1 q, r;

    ++d;    // d = 3

    for(;;)
    {
        divide(x, d, q, r);
        if(is_null(r))
        {
            x = q;
            resbuf.push_back(d);
        }
        else
            break;
    }

    d += 2; // d = 5

    unsigned s = 2;

    while(!is_unit(x))
    {
        ARAGELI_ASSERT_1(x >= d);

        divide(x, d, q, r);
        if(is_null(r))
        {
            x = q;
            resbuf.push_back(d);
        }
        else if(q < d)
        {
            ARAGELI_ASSERT_1(is_prime(x, tfctr));
            //std::cout << "\nARAGELI DEBUG OUTPUT: d = " << d << std::endl;
            resbuf.push_back(x);
            break;
        }
        else
        {
            //d = next_prime(d);
            d += s;
            if(s == 2)
                s = 4;
            else
                s = 2;
        }
    }

    res.assign(resbuf.size(), resbuf.begin());

    ARAGELI_DEBUG_EXEC_2(T1 tt);
    ARAGELI_ASSERT_2(product(res, tt) == xx);

    return res;
}

namespace _Internal
{
    template<typename T>
    T random_function(T x, T n)
    {
        return (x*x+5)%n;
    }
}

template<typename T, typename T_factory>
T rho_pollard(const T& n, const T_factory& tfctr)
{
    //// Zolotykh - Arageli 1.03
    /*
    T x = 2, y = 2, d = unit(n);
    while(is_unit(d))
    {
        x = _Internal::random_function(x, n);
        y = _Internal::random_function(_Internal::random_function(y, n), n);
        d = gcd(std::abs(x-y), n);
        if ((1 < d) && (d < n))
        {
            return d;
        }
        if (d == n)
            return 1; // failure
    }
    return d;
    */

    // Zolotykh - Computer Algebra
    ARAGELI_ASSERT_1(n >= 4);

    // We need integer random numbers from [0, n-1].
    typedef set::grid1<T, T> int_grid;
    int_grid int_0_nm1(0, n-1); // integer set from [0, n-1]
    rnd::equiprob<int_grid> gen_0_nm1(int_0_nm1);   // random generator from [0, n-1]

    T x0  = gen_0_nm1();
    T x = x0;
    T d;
    unsigned int i, j = 1;
    while (true)
    {
        x0 = x;
        for (i = 1; i <= j; ++i)
        {
            x = _Internal::random_function(x, n);
            d = gcd(std::abs(x-x0), n);
            if ((d > 1) && (d <= n))
                return d;
        }
        j*=2;
    }
}

template<typename T, typename N, typename T_factory>
T brent(const T& n, N no_of_iter, const T_factory& tfctr)
{
    N r = 1;
    N m = 10;
    N iter = 0;

    T y = 0;
    T q = 1;
    T x, ys, g;

    do
    {
        x = y;
        for (N j = 1; j <= r; j++)
            y = _Internal::random_function(y, n);

        N k = 0;

        do
        {
            iter++;
            if (iter > no_of_iter)
                return 0; // failure
            ys = y;
            T mn = r - k;
            if (m < mn)
                mn = m;
            for (N j = 1; j <= mn; j++)
            {
                y = _Internal::random_function(y, n);
                q = (q * std::abs(x - y)) % n;
            }
            g = gcd(q, n);
            k = k + m;

        }while(k < r && g <= 1);

        r = 2 * r;
    }while (g <= 1);

    if (g == n)
    {
        do
        {
            ys = _Internal::random_function(ys, n);
            g = gcd(x - ys, n);
        }while (g <= 1);
    }

    if (g == n)
        return 0;
    else
        return g;
}

template<typename T, typename N, typename T_factory>
T pollard_pm1(const T& n, const N& no_of_iter, const T_factory& tfctr)
{
    T b = 2, t = 2, p = unit(n);
    if (!no_of_iter)
    {
        int i = 2;
    while(true)
    {
        //  if (i % 10 == 0) std::cout << " i = " << i << std::endl;
        t = power_mod(t, i, n); // now t = b^(i!)
        p = gcd(n, t - 1);
        if (p > unit(n) && p < n)
            return p;
        if (p == n)
        {
            i = 2;
            t = ++b;
            if (b == 5)
                return null(n);
        }
        ++i;
    }
    }
    else
    {
    for(N i = 2; i <= no_of_iter; i++)
    {
        //  if (i % 10 == 0) std::cout << " i = " << i << std::endl;
        t = power_mod(t, i, n); // now t = b^(i!)
        p = gcd(n, t - 1);
        if (p > unit(n) && p < n)
            return p;
        if (p == n)
        {
            i = 2;
            t = ++b;
            if (b == 5)
                return null(n);
        }
    }
    }
    std::cout << "\nIncrease iterations number!\n";
    return null(n);
}

template
<
    typename T1,
    typename T2,
    bool REFCNT2,
    typename T3,
    typename T4
>
vector<T2, REFCNT2>& partial_factorize_division
(
    T1 x,
    vector<T2, REFCNT2>& res,
    const T3& max,
    T4& rest
)
{
    ARAGELI_DEBUG_EXEC_2(T1 xx = x);    // save original value

    if(is_unit(x) || max < 2)
    {
        res.resize(0);
        rest = x;

        ARAGELI_DEBUG_EXEC_2(T1 tt);
        ARAGELI_ASSERT_2(product(res, tt)*rest == xx);

        return res;
    }

    T1 d = unit(x); // d = 1
    ARAGELI_ASSERT_0(x >= d);
    ++d;    // d = 2

    std::list<T2> resbuf;

    while(is_null(x % 2))
    {
        x >>= 1;
        resbuf.push_back(d);
    }

    if(max < 3)
    {
        res.assign(resbuf.size(), resbuf.begin());
        rest = x;

        ARAGELI_DEBUG_EXEC_2(T1 tt);
        ARAGELI_ASSERT_2(product(res, tt)*rest == xx);

        return res;
    }

    T1 q, r;

    ++d;    // d = 3

    for(;;)
    {
        divide(x, d, q, r);
        if(is_null(r))
        {
            x = q;
            resbuf.push_back(d);
        }
        else
            break;
    }

    d += 2; // d = 5

    unsigned s = 2;
    rest = x;
    while(!is_unit(x))
    {
        ARAGELI_ASSERT_1(x >= d);

        if(max < d)
        {
            if (is_prime(x))
            {
                resbuf.push_back(x);
                rest = unit(rest);
            }
            else
            {
                rest = x;
            }
            res.assign(resbuf.size(), resbuf.begin());

            ARAGELI_DEBUG_EXEC_2(T1 tt);
            ARAGELI_ASSERT_2(product(res, tt)*rest == xx);

            return res;
        }

        divide(x, d, q, r);
        if(is_null(r))
        {
            x = q;
            resbuf.push_back(d);
        }
        else if(q < d)
        {
            ARAGELI_ASSERT_1(is_prime(x));
            rest = 1;
            resbuf.push_back(x);
            break;
        }
        else
        {
            //d = next_prime(d);
            d += s;
            if(s == 2)
                s = 4;
            else s = 2;
        }
    }

    res.assign(resbuf.size(), resbuf.begin());

    ARAGELI_DEBUG_EXEC_2(T1 tt);
    ARAGELI_ASSERT_2(product(res, tt)*rest == xx);

    return res;
}


template <typename T1, typename T2, typename T3, typename T4>
const T4& factorize_multiplier (T1 x, const T2& m, T3& remx, T4& nm)
{
    ARAGELI_ASSERT_0(!is_null(m));
    ARAGELI_ASSERT_0(!is_unit(m));
    ARAGELI_ASSERT_0(!is_opposite_unit(m));

    T1 q = x;
    T2 r;
    nm = null(nm);

    for(;;)
    {
        remx = x;
        divide(remx, m, x, r);
        if(!is_null(r))break;
        ++nm;
    }

    return nm;
}


template <typename T>
T next_prime_successive_checking (T x)
{
    for(++x; !is_prime(x); ++x);
    return x;
}


template <typename T>
T prev_prime_successive_checking (T x)
{
    ARAGELI_ASSERT_0(x > 2);
    for(--x; !is_prime(x); --x);
    return x;
}

template <typename T>
T next_probably_prime (T x)
{
    for(++x; !is_probably_prime(x); ++x);
    return x;
}


template <typename T>
T prev_probably_prime (T x)
{
    ARAGELI_ASSERT_0(x > 2);
    for(--x; !is_probably_prime(x); --x);
    return x;
}


template <typename T>
T next_mersen_prime_degree (const T& n)
{
    static const unsigned mersen_degrees[] =
    {
        2,3,5,7,13,17,19,31,61,89,107,127,521,607,1279,
        2203,2281,3217,4253,4423,9689,9941,11213,19937,
        21701,23209,44497,86243,110503,132049,216091,
        756839,859433,1257787,1398269,2976221,3021377,
        6972593,13466917
    };

    const unsigned* res = std::upper_bound
    (
        &mersen_degrees[0],
        &mersen_degrees[sizeof(mersen_degrees)/sizeof(unsigned)],
        unsigned(n)
    );

    if(res == &mersen_degrees[sizeof(mersen_degrees)/sizeof(unsigned)])
        throw invalid_argument();

    return *res;
}



} // namespace Arageli

#endif

