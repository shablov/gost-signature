/*****************************************************************************

    gcd.cpp

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
    \file gcd.cpp
    \brief The gcd.hpp file stuff implementation.
*/

#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_GCD)

#include <cstddef>
#include <algorithm>

#include "gcd.hpp"
#include "_utility.hpp"


namespace Arageli
{


namespace _Internal
{

template <typename T>
inline T& euclid_norm (T& x)
{
    return x;
}

template <typename T, typename Config>
inline polynom<T, Config>& euclid_norm (polynom<T, Config>& x)
{
    if(!x.is_null())
    {
        const T lc = x.leading_coef();
        if(!is_unit(lc))
        {
            typedef typename polynom<T, Config>::size_type size_type;
            const size_type degree = x.degree();
            for(size_type i = factory<size_type>::null(); i <= degree; i++)
                x[i] /= lc;
        }
        else
            return x;
    }
    return x;
}

template <typename T, typename P, bool REFCNT>
inline sparse_polynom<T, P, REFCNT>& euclid_norm (sparse_polynom<T, P, REFCNT>& x)
{
    if(!is_null(x))
        x /= x.leading_coef_cpy();
    return x;
}

template <typename T, typename D, typename MD, typename Config>
inline sparse_multipolynom<T, D, MD, Config>& euclid_norm
(sparse_multipolynom<T, D, MD, Config>& x)
{
    typedef typename sparse_multipolynom<T, D, MD, Config>::monom M;
    if(!is_null(x))
        x /=
            sparse_multipolynom<T, D, MD, Config>
                (M(x.leading_coef(), MD(x.leading_monom().nvars())));
    return x;
}

template <typename T1, typename T2, typename T3>
inline void euclid_norm (T1& x1, T2& x2, T3& x3) {}

template
<
    typename T1,
    typename P1,
    bool REFCNT1,
    typename T2,
    typename P2,
    bool REFCNT2,
    typename T3,
    typename P3,
    bool REFCNT3
>
inline void euclid_norm
(
    sparse_polynom<T1, P1, REFCNT1>& x1,
    sparse_polynom<T2, P2, REFCNT2>& x2,
    sparse_polynom<T3, P3, REFCNT3>& x3
)
{
    if(!is_null(x1))
    {
        x2 /= x1.leading_coef();
        x3 /= x1.leading_coef();
        x1 /= x1.leading_coef_cpy();
    }
}

}


template <typename T, typename T_factory>
T euclid (T a, T b, const T_factory& tfctr)
{
    ARAGELI_TRACE_BEGIN_AUTO
    (
        tracing_common
            << tracing_indent() << "euclid: a = "
            << a << ", b = " << b << '\n', 0
    );

    if(is_negative(a))
        opposite(&a);
    if(is_negative(b))
        opposite(&b);

    if(is_null(a))
        if(is_null(b))
            return tfctr.unit(a);
        else
            return _Internal::euclid_norm(b);
    if(is_null(b))
        return _Internal::euclid_norm(a);

    while(!is_null(a %= b))
    {
        std::swap(a, b);
    }
    return _Internal::euclid_norm(b);
}


template <typename T, typename T_factory>
T euclid_binary (T a, T b, const T_factory& tfctr)
{
    if(is_negative(a))
        opposite(&a);
    if(is_negative(b))
        opposite(&b);

    if(is_null(a))
        if(is_null(b))
            return tfctr.unit(a);
        else
            return _Internal::euclid_norm(b);
    if(is_null(b))
        return _Internal::euclid_norm(a);

    T res = tfctr.unit(a);

    while(!is_unit(a) && !is_unit(b))
    {
        ARAGELI_ASSERT_1(!is_null(a));
        ARAGELI_ASSERT_1(!is_null(b));

        if(is_even(a))
            if(is_even(b))
            {
                a >>= 1; b >>= 1;
                res <<= 1;
            }
            else
            {
                a >>= 1;
            }
        else
            if(is_even(b))
            {
                b >>= 1;
            }
            else
            {
                T t = a - b;
                switch(sign(t))
                {
                    case +1:
                    {
                        a = t;
                        a >>= 1;
                        break;
                    }
                    case -1:
                    {
                        b = -t;
                        b >>= 1;
                        break;
                    }
                    case 0:
                        return _Internal::euclid_norm(res *= a);
                }
            }
    }

    return _Internal::euclid_norm(res);
}


template <typename T>
T gcdex (const T& a, const T& b, T&u, T& v, T& w, T& z)
{
    T g = euclid_bezout(a, b, u, v);

    if(is_null(g))
    {
        u = unit(u);
        v = null(v);
        w = null(w);
        z = unit(z);
        return g;
    }

    z = a/g;
    w = -b/g;

    ARAGELI_ASSERT_1(is_unit(abs(u*z - w*v)));
    ARAGELI_ASSERT_1(is_null(a*w + b*z));

    return g;
}


template <typename T>
T gcdex (const T& a, const T& b, const T& N, T&u, T& v, T& w, T& z)
{
    ARAGELI_ASSERT_0(is_positive(N));

    T
        a_mod_N = mod(a, N),
        b_mod_N = mod(b, N),
        g = euclid_bezout(a_mod_N, b_mod_N, u, v);

    if(is_null(g))
    {
        u = unit(u);
        v = null(v);
        w = null(w);
        z = unit(z);

        return g;
    }

    u = mod(u, N);
    v = mod(v, N);
    w = N - b_mod_N/g;
    z = a_mod_N/g;

    ARAGELI_ASSERT_1(is_unit(gcd(mod(u*z - w*v, N), N)));
    ARAGELI_ASSERT_1(is_null(mod(a*w + b*z,N)));

    return g;
}


template <typename T, bool REFCNT, typename T_factory>
T gcd (const vector<T, REFCNT>& v, const T_factory& tfctr)
{
    if(v.is_empty())
        return tfctr.null();    // null as in Maple 9

    T d = v[v.size() - 1];
    typedef typename vector<T, REFCNT>::difference_type difftype;
    difftype j = difftype(v.size()) - 2;

    while(!is_unit(d) && j >= 0)
    {
        if(!is_null(v[j]))
            d = gcd(v[j], d, tfctr);
        --j;
    }

    return is_null(d) ? tfctr.unit() : d;
}


template <typename T, bool REFCNT, typename T_factory>
T lcm (const vector<T, REFCNT>& v, const T_factory& tfctr)
{
    if(v.is_empty())
        return tfctr.unit();    // unit as in Maple 9

    T d = v[v.size() - 1];
    typedef typename vector<T, REFCNT>::difference_type difftype;
    difftype j = difftype(v.size()) - 2;

    while(j >= 0)
    {
        if(!is_null(v[j]))
            d = lcm(v[j], d, tfctr);
        --j;
    }

    return is_null(d) ? tfctr.unit() : d;
}


template
<
    typename T,
    bool REFCNT,
    typename T_factory,
    typename T1,
    bool REFCNT1
>
vector<T, REFCNT> gcd_vec
(const vector<T, REFCNT>& a, const vector<T1, REFCNT1>& b, const T_factory& tfctr)
{
    ARAGELI_ASSERT_0(a.size() == b.size());
    std::size_t size = a.size();
    vector<T, REFCNT> res(size, T());

    for(std::size_t i = 0; i < size; ++i)
        res[i] = gcd(a[i], b[i]);

    return res;
}


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
)
{
    ARAGELI_ASSERT_0(a.size() == b.size());

    ARAGELI_ASSERT_0(a.size() == b.size());
    std::size_t size = a.size();
    vector<T, REFCNT> res(size, T());

    for(std::size_t i = 0; i < size; ++i)
        res[i] = lcm(a[i], b[i]);

    return res;
}


template
<
    typename T,
    bool REFCNT,
    typename T_factory,
    typename T1,
    bool REFCNT1
>
vector<T, REFCNT> euclid
(
    const vector<T, REFCNT>& a,
    const vector<T1, REFCNT1>& b,
    const T_factory& tfctr
)
{
    ARAGELI_ASSERT_0(a.size() == b.size());

    std::size_t size = a.size();
    vector<T, REFCNT> res(size, T());

    for(std::size_t i = 0; i < size; ++i)
        res[i] = euclid(a[i], noncopy_cast(b[i]), tfctr);

    return res;
}


template <typename T, typename T_factory>
T euclid_bezout
(
    const T& a,
    const T& b,
    T& u,
    T& v,
    const T_factory& tfctr
)
{
    u = tfctr.unit(a);
    v = tfctr.null(a);
    T u_new = v;
    T v_new = u;
    T r    = abs(a);
    T r_new = abs(b);

    while(!is_null(r_new))
    {
        T q = prquot(r, r_new);

        T aux = u_new;
        u_new = u - q * u_new;
        u     = aux;

        aux   = v_new;
        v_new = v - q * v_new;
        v     = aux;

        aux   = r_new;
        r_new = r - q * r_new;
        r     = aux;
    }

    if(is_negative(a))
        opposite(&u);
    if(is_negative(b))
        opposite(&v);

    _Internal::euclid_norm(r, u, v);

    ARAGELI_ASSERT_1(a*u + b*v == r);

    return r;
}


} // namespace Arageli

#endif
