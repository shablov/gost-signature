/*****************************************************************************

    intalg.cpp

    This file is a part of Arageli library.

    Some functions in this file are part of Anna Bestaeva degree work 2006.
    They have been integrated into Arageli by Sergey Lyalin.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2005--2007 Sergey S. Lyalin
    Copyright (C) 2005--2006 Aleksey A. Bader
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
    \file intalg.cpp
    \brief The intalg.hpp file stuff implementation.
*/

#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_INTALG)

#include <utility>
#include <stack>
#include <vector>

#include "intalg.hpp"
#include "powerest.hpp"
#include "vector.hpp"


namespace Arageli
{


template <typename T, typename T_factory>
std::size_t nbits (T a, const T_factory& tfctr)
{
    std::size_t res = tfctr.null(a);
    std::size_t n = a;
    while(n)
    {
        ++res;
        n >>= 1;
    }
    return res;
}


template <typename T, typename I, typename T_factory>
T power_mod (T a, I n, const T& m, const T_factory& tfctr)
{
    ARAGELI_ASSERT_0(!is_null(a) || is_positive(n));

    T res = tfctr.unit(a);
    while(!is_null(n))
    {
        if(is_odd(n))
            (res *= a) %= m;
        (a *= a) %= m;
        n >>= 1;
    }

    return res;
}


namespace _Internal
{

template <typename T, typename V>
T conditioner (const T& _a, const T& _b, const T& N, V &d)
{
    typedef unsigned long index;

    typename V::template other_element_type<T>::type a(d.size()), b(d.size());
    index t, K = 2*std::pow(nbits(N), 1.5);
    typename V::template other_element_type<bool>::type B(K + 1, true);

    for(std::size_t i = 0; i < d.size(); i ++)
    {
        a[i] = prrem(_a, d[i]);
        b[i] = prrem(_b, d[i]);
        T gi = gcd(b[i], d[i]);

        if(gi > unit<T>() && gi < d[i])
        {
            d[i] /= gi;
            d.insert(d.begin() + i, gi);
            return opposite_unit<T>();//-1;
        }
    }

    for(std::size_t i = 0; i < d.size(); i ++)
        if(!is_null(b[i]))
        {
            T si = mod(-a[i], b[i], d[i]);
            if(K < si)
                continue;
            index M = index(si);

            for(;;)
            {
                B[M] = false;
                if(M + d[i] > K)
                    break;
                M += index(d[i]);
            }
        }

    for(t = 0; t < B.size(); t++)
        if(B[t])break;

    for(std::size_t i = 0; i < d.size(); i ++)
    {
        T gi = gcd(a[i] + t*b[i], d[i]);

        if(gi > unit<T>())
        {
            d[i] /= gi;
            d.insert(d.begin() + i, gi);
            return opposite_unit<T>();//-1;
        }
    }

    return t;
}


template <typename T>
T conditioner (const T& a, const T& b, const T& N)
{
    ARAGELI_ASSERT_0(is_unit(gcd(a, b)));
    ARAGELI_ASSERT_0(is_positive(N));

    vector<T> d(1, N);
    T c;

    do
    {
        c = conditioner(a, b, N, d);
    }while(is_opposite_unit(c));

    ARAGELI_ASSERT_1(is_unit(gcd3(a, b, N) == gcd(a + c*b, N)));

    return c;
}


}    // namespace _Internal


template <typename T1, typename T2, typename T3>
T3 div_mod (const T1& a, const T2& b, const T3& d)
{
    // WARNING! Are all variables of type T3.
    T3 u, v;
    T3 g = euclid_bezout(b, d, u, v);
    return mod(u*a/g, d);
}


template <typename T1, typename T2, typename T3>
T3 quo_mod (const T1& a, const T2& b, const T3& d)
{
    T3 r = rem_mod(a, b, d);
    T3 c = div_mod(a - r, b, d);
    return mod(c, d);
}


template <typename T>
T split (const T& a, const T& d)
{
    T x = a, t = d;

    while(!is_unit(x))
    {
        x = gcd(x, t);
        t /= x;
    }

    return t;
}


template <typename T>
T stab (const T& A, const T& B, const T& N)
{
    ARAGELI_ASSERT_0(is_positive(N));

    if(is_null(B))
        return null(A);
    if(is_null(A))
        return unit(A);

    T
        c,
        g1 = gcd3(A, B, N),
        g2 = gcd(A/g1, B/g1),
        a = A/(g1*g2),
        b = B/(g1*g2),
        d = N/g1;

    if(is_unit(gcd(a, d)))
        return null(A);
    if(is_unit(gcd(a + b, d)))
        return unit(A);

    c = _Internal::conditioner(a, b, d);

    ARAGELI_ASSERT_1(gcd3(A, B, N) == gcd(A + c*B, N));

    return c;
}


template <typename T>
T unit (const T& a, const T& N)
{
    ARAGELI_ASSERT_0(is_positive(N));

    T u, v, w, z;
    T
        g = gcdex(a, N, u, v, w, z),
        c = mod(u + stab(u, w, N)*w, N);

    ARAGELI_ASSERT_1(is_unit(gcd(c, N)));
    ARAGELI_ASSERT_1(mod(c*a, N) == mod(gcd(a, N), N));

    return c;
}


template <typename T>
T split_stab (const T& A, const T& B, const T& N)
{
    ARAGELI_ASSERT_0(is_positive(N));

    if(is_null(B))
        return null(A);
    if(is_null(A))
        return unit(A);

    T
        c,
        g1 = gcd(A, B, N),
        g2 = gcd(A/g1, B/g1),
        a = A/(g1*g2),
        b = B/(g1*g2),
        d = N/g1;

    if(is_unit(gcd(a, d)))
        return null(A);
    if(is_unit(gcd(a + b, d)))
        return unit(A);

    c = split(a, d);

    ARAGELI_ASSERT_1(gcd3(A, B, N) == gcd(A + c*B, N));

    return c;
}


template <typename T>
T split_mod (const T& a, const T& d)
{
    // WARNING! Need to create nnbits(x) function to compute nbits(nbits(x)).

    std::size_t N = nbits(nbits(a))/*(big_int(a.length())).length()*/;
    T x = a;
    for(std::size_t i = 0; i <= N + 1; i++) //??? i <= loglogN ???// WARNING!
        x = mod(x*x, d);
    return d/gcd(x, d);
}


template <typename T>
T stab_mod (const T& a, const T& b, const T& N)
{
    ARAGELI_ASSERT_0(is_positive(N));

    if(is_null(b))
        return null<T>();
    if(is_null(a))
        return unit<T>();
    T g = gcd3(a, b, N),
    c = split_mod(a/g, N/g);

    ARAGELI_ASSERT_1(gcd3(a, b, N) == gcd(a + c*b, N));

    return c;
}


template<typename T>
bool is_invertible_mod (const T& a, const T& N)
{
    return is_unit(gcd(mod(a, N), N));
}


template <typename T, typename T_factory>
T factorial_successive_multiplication (T a, const T_factory& tfctr)
{
    ARAGELI_ASSERT_0(sign(a) >= 0);

    if(is_null(a) || is_unit(a))
        return tfctr.unit(a);
    T res = a;

    for(--a; !is_unit(a); --a)
        res *= a;

    return res;
}


template <typename T, typename T_factory>
T factorial_even_odd_multiplication (T a, const T_factory& tfctr)
{
    //return factorial_successive_multiplication(a, tt);

    ARAGELI_ASSERT_0(sign(a) >= 0);

    if(is_null(a) || is_unit(a))
        return tfctr.unit(a);
    T
        unit = tfctr.unit(a),
        two = unit;
    ++two;
    if(a == two)
        return two;

    T res = unit;
    T n2q, n2r, n4q, n4r;
    divide(a - unit, two, n2q, n2r);
    divide(n2q - unit, two, n4q, n4r);

    if(!is_null(n2r))
        res *= a;
    if(!is_null(n4r))
        res *= n2q;

    res *= factorial_even_odd_multiplication(n4q, tfctr);

    T t = unit;
    T iend = (n4q << 1u) + unit;
    T i = (n2q << 1u) + unit;

    std::vector<std::pair<T, int> > pairer;
    std::size_t reserve_size = 2 + nbits(((i - iend) >> 1u) + unit);
    pairer.reserve(reserve_size);
    pairer.push_back(std::make_pair(t, 0));

    for(; i != iend; ----i)
    {
        ARAGELI_ASSERT_1(!pairer.empty());
        ARAGELI_ASSERT_1(pairer.size() <= reserve_size);

        T cur = i;  // the current value
        int level = 0;  // the level of the current value

        while(pairer.back().second == level)
        {
            // make a pair
            cur *= pairer.back().first;
            ++level;
            pairer.pop_back();
            if(pairer.empty())
                break;
        }

        ARAGELI_ASSERT_1(pairer.empty() || pairer.back().second > level);
        pairer.push_back(std::make_pair(cur, level));
    }

    ARAGELI_ASSERT_1(pairer.size() <= reserve_size);
    ARAGELI_ASSERT_1(!pairer.empty());
    ARAGELI_ASSERT_2(is_unit(t));

    while(!pairer.empty())
    {
        t *= pairer.back().first;
        pairer.pop_back();
    }

#if 0
    for(; i != iend; ----i)
        t *= i;
#endif

    T t1 = unit;

    reserve_size = 2 + nbits((i >> 1u) + unit);
    pairer.reserve(reserve_size);
    ARAGELI_ASSERT_1(pairer.empty());
    pairer.push_back(std::make_pair(t1, 0));

    for(; !is_unit(i); ----i)
    {
        ARAGELI_ASSERT_1(!pairer.empty());
        ARAGELI_ASSERT_1(pairer.size() <= reserve_size);

        T cur = i;  // the current value
        int level = 0;  // the level of the current value

        while(pairer.back().second == level)
        {
            // make a pair
            cur *= pairer.back().first;
            ++level;
            pairer.pop_back();
            if(pairer.empty())
                break;
        }

        ARAGELI_ASSERT_1(pairer.empty() || pairer.back().second > level);
        pairer.push_back(std::make_pair(cur, level));
    }

    ARAGELI_ASSERT_1(pairer.size() <= reserve_size);
    ARAGELI_ASSERT_1(!pairer.empty());
    ARAGELI_ASSERT_2(is_unit(t1));

    while(!pairer.empty())
    {
        t1 *= pairer.back().first;
        pairer.pop_back();
    }

#if 0
    for(; !is_unit(i); ----i)
        t1 *= i;
#endif

    return (res*t)*(t1*t1) << (n2q + n4q);
}


template <typename T, typename T_factory>
int jacobi (T n, T p, const T_factory& tfctr)
{
    ARAGELI_ASSERT_0(is_positive(p));
    ARAGELI_ASSERT_0(is_odd(p));

    /////////////   TEMPORARY RECURSIVE VERSION   ///////////////

    T g;
    T a = n;
    T b = p;

    //if (!(b % 2)) return 2;
    if (a >= b)
        a = a%b;
    if (a == 0)
        return 0;
    if (a == 1)
        return 1;

    if (a < 0)
        if ((b-1)/2 % 2 == 0)
            return jacobi(-a,b);
        else
            return -jacobi(-a,b);

    if (a % 2 == 0)
        if (((b*b - 1)/8) % 2 == 0)
            return +jacobi(a/2,b);
        else
            return -jacobi(a/2,b);

    g = gcd(a, b);
    //if (!(a % 2)) return 2;

    if (g == a)
        return 0;
    else
        if (g != 1)
            return jacobi(g,b)*jacobi(a/g,b);
        else
            if (((a-1)*(b-1)/4) % 2 == 0)
                return +jacobi(b,a);
            else
                return -jacobi(b,a);

#if 0

    ////////  THE FOLLOWING CODE IS INCORRECT,  BUG HAS NO FOUND  //////////

    ARAGELI_ASSERT_0(is_positive(n));
    ARAGELI_ASSERT_0(is_odd(n));

    if(is_divisible(a, n))return tfctr.null(a);

    int res = 1;

    if(is_negative(a))
    {
        if(is_odd((n - 1) >> 1))res = -res;
        opposite(&a);
    }

    if(a > n)a %= n;

    T a1, t;

    for(;;)
    {
        factorize_multiplier(a, 2, a1, t);

        if(is_odd((t*t - 1) >> 3))res = -res;

        if(is_unit(a1))break;

        ARAGELI_ASSERT_1(a1 < n);
        ARAGELI_ASSERT_1(a1 > tfctr.unit(a1));

        if(is_odd(((a1 - 1) >> 1) * ((n - 1) >> 1)))res = -res;

        a = n; n = a1;
        ARAGELI_ASSERT_1(a > n);
        a %= n;
    }

    return res;

#endif
}


template <typename T>
T intsqrt (const T& a)
{
    ARAGELI_ASSERT_1(sign(a) >= 0);
    if(is_null(a) || is_unit(a))return a;

    T r = a >> 1;
    T q;
    for(;;)
    {
        q = a / r;
        if(q >= r)
            break;
        else
            r = (r + q) >> 1;
    }

    ARAGELI_ASSERT_1(r*r <= a && (r+1)*(r+1) > a);
    return r;
}


template <typename T, typename TT>
T sqrt_mod_shenks (const T& a, const T& n, const TT& tt)
{
    ARAGELI_ASSERT_0(is_positive(a));
    ARAGELI_ASSERT_0(is_positive(n));
    ARAGELI_ASSERT_0(a < n);
    ARAGELI_ASSERT_0(is_prime(n));
    ARAGELI_ASSERT_0(jacobi(a, n) == 1);
    ARAGELI_ASSERT_0(is_unit(n%4));

    /////////  WARNING! THERE IS NO IMPLEMENTATION!  /////////

//    ARAGELI_ASSERT_ALWAYS(0);
    T b, j, r;
    T p = 2;
    for(;jacobi(p, n)>-1;p+=1);
    T temp = (n-1)/2;
    unsigned alpha = 1;
    while (!(temp % 2))
    {
        temp = temp /2;
        ++alpha;
    }
    T s = temp;
    r = power_mod(a, (s+1)/2, n);
    b = power_mod(p, s, n);
//      find inverse element a^(-1)
    T inva = inverse_mod(a, n);
    T temp1 = (r*r*inva) % n;
    j = "0";    // WARNING!
    const T two = "2";  // WARNING!
    T t1 = power(two, alpha - two);
    if (is_null(power_mod(temp1, t1, n) - n-1))
        j=1;
    int i;
    T t2;
    for (i = 1; i <= alpha - 2; ++i)
    {
        j*=2;
        t2 = power(two, alpha-i-two);
        if (is_null(power_mod(r*r*inva*b*b, t2, n) - n-1))
        {
            j++;
        }
    }
    return power(b,j)*r%n;
}


///////////////////////  FOR THE FUTURE IMPLEMENTATION  //////////////////////
#if 0
template <typename T>
T sqrt_mod (const T& a, const T& n, const TT& tt)
{
    ARAGELI_ASSERT_0(is_positive(a, tt));
    ARAGELI_ASSERT_0(is_positive(n, tt));
    ARAGELI_ASSERT_0(a < n);
    ARAGELI_ASSERT_0(is_prime(n, tt));
    AEAGELI_ASSERT_0(jacobi(a, n, tt) == 1);

    if(n == 2)
    {
        AEAGELI_ASSERT_1(tt.is_unit(a) || tt.is_null(a));
        return a;
    }

    /////////  WARNING! PARTIAL IMPLEMENTATION!  //////////
    ARAGELI_ASSERT_ALWAYS(n%4 == 3);

    AEAGELI_ASSERT_1(is_divisible(n+1, 4, tt, type_traits<int>()));
    return power_mod(a, (n+1)/4, tt);
}
#endif
///////////////////////////////////////////////////////////////////////////////


} // namespace Arageli

#endif
