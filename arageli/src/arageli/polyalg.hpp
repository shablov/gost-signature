/*****************************************************************************

    polyalg.hpp

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
    \file polyalg.hpp
    \brief Some algorithms on polynomials (division, primpart, content etc.).
*/


#ifndef _ARAGELI_polyalg_hpp_
#define _ARAGELI_polyalg_hpp_

#include "config.hpp"

#include <cstddef>

#include "type_traits.hpp"
#include "factory.hpp"
#include "_utility.hpp"
#include "misc.hpp"


#include "std_import.hpp"

//****************************************************************************

namespace Arageli
{


/// Division of two univariate polynomials with quotient and remainder.
/** Appropriate for polynomials over rings. */
template <typename P1, typename P2, typename PQ, typename PR>
void polynom_divide_simple
(
    const P1& p1,
    const P2& p2,
    PQ& pq,
    PR& pr
)
{
    ARAGELI_ASSERT_0(!is_null(p2));

    pr = p1;
    pq = null(pq);
    const typename P2::monom& lmp2 = p2.leading_monom();
    typename PR::degree_type
        oldprdeg = pr.degree(),
        newprdeg = oldprdeg;
    const typename P2::degree_type p2deg = lmp2.degree();

    for(;;)
    {
        if(newprdeg < p2deg)break;
        ARAGELI_ASSERT_1(!is_null(pr));
        typename PR::monom mpr = pr.leading_monom();
        mpr /= lmp2;    // maybe integer division
        P2 tp2 = p2;
        oldprdeg = newprdeg;
        pr -= (tp2 *= mpr);
        newprdeg = pr.degree();
        ARAGELI_ASSERT_1(newprdeg <= oldprdeg);
        pq += mpr;
        if(newprdeg == oldprdeg)break;
    }
}


/// Polynomial pseudo-division.
template <typename P1, typename P2, typename PQ, typename PR, typename T>
void polynom_pseudodivide_simple
(
    const P1& p1,
    const P2& p2,
    PQ& pq,
    PR& pr,
    T& multiplier
)
{
    ARAGELI_ASSERT_0(!is_null(p2));

    if(p1.degree() < p2.degree())
    {
        pq = null(pq);
        pr = p1;
        multiplier = unit(multiplier);
    }
    else
    {
        multiplier = power(p2.leading_coef(), p1.degree() - p2.degree() + 1);
        polynom_divide_simple(p1*multiplier, p2, pq, pr);
        ////COUTCOM std::cout << "\npq = " << pq;
    }
}


/// Computes constant res for polynomial p such, that res*p has only integer coefs.
template <typename P, typename T>
T& polynom_rational_to_int_value (const P& p, T& res)
{
    res = unit(res);

    for
    (
        typename P::coef_const_iterator i = p.coefs_begin();
        i != p.coefs_end();
        ++i
    )
        res = lcm(res, i->denominator());

    return res;
}


/// Multiplies pr by some constant to obtain a polynomial with only integer coefs.
template <typename Pr, typename Pi, typename X>
void polynom_rational_to_int (const Pr& pr, Pi& pi, const X& x)
{
    pi = null(pi);

    typedef typename Pr::monom_const_iterator Pr_citer;
    typedef typename Pr::coef_type Pr_coef;
    typedef typename Pi::monom Pi_monom;
    Pr_citer pr_monoms_end = pr.monoms_end();

    // WARNING.  P1 and P1int, P2 and P2int should have the same monom order.

    for(Pr_citer i = pr.monoms_begin(); i != pr_monoms_end; ++i)
    {
        const Pr_coef& ic = i->coef();
        ARAGELI_ASSERT_0(is_divisible(x, ic.denominator()));

        pi.insert
        (
            pi.monoms_end(),
            Pi_monom(ic.numerator() * (x/ic.denominator()), i->degree())
        );
    }
}


/// Simple division of polynomials over rationals through pseudo-division.
template <typename P1, typename P2, typename PQ, typename PR>
void polynom_divide_rational_simple
(P1 p1, P2 p2, PQ& pq, PR& pr)
{
    if(p1.degree() + p2.degree() <= 2 || p2.size() == 1)
    {
        polynom_divide_simple(p1, p2, pq, pr);
        /*_my_counter_1++*/;
        return;
    }
    else
        /*_my_counter_2++*/;

    typedef typename P1::coef_type P1coef;
    typedef typename P1coef::value_type P1int;

    // WARNING! Should other types (coef_type for P2, PQ and PR) be a rational too?

    typename P1::template other_coef<P1int>::type p1i;
    typename P2::template other_coef<P1int>::type p2i;
    typename PQ::template other_coef<P1int>::type pqi;
    typename PR::template other_coef<P1int>::type pri;

    // Compute normalization values.  After multiplication by them all coefficients
    // become integer.

    P1int m1, m2, multiplier;
    polynom_rational_to_int(p1, p1i, polynom_rational_to_int_value(p1, m1));
    polynom_rational_to_int(p2, p2i, polynom_rational_to_int_value(p2, m2));

    // perform pseudo division
    polynom_pseudodivide_simple(p1i, p2i, pqi, pri, multiplier);

    // restore correct quotient and remainder

    multiplier *= m1;
    pq = pqi;
    pr = pri;
    (pq *= m2) /= multiplier;
    pr /= multiplier;
}


/// One of the specializations of polynom_divide. For not rational coefs.
template <typename P1, typename P2, typename PQ, typename PR>
inline void polynom_divide
(
    const P1& p1,
    const P2& p2,
    PQ& pq,
    PR& pr,
    const type_category::type&
)
{
    polynom_divide_simple(p1, p2, pq, pr);
}


/// One of the specializations of polynom_divide. For rational coefs.
template <typename P1, typename P2, typename PQ, typename PR>
inline void polynom_divide
(
    const P1& p1,
    const P2& p2,
    PQ& pq,
    PR& pr,
    const type_category::rational&
)
{
    polynom_divide_rational_simple(p1, p2, pq, pr);
}


/// Divides two polynomials by algorithm that is determined by type of the coefficients.
template <typename P1, typename P2, typename PQ, typename PR>
inline void polynom_divide
(
    const P1& p1,
    const P2& p2,
    PQ& pq, PR&
    pr
)
{
    polynom_divide
    (
        p1, p2, pq, pr,
        type_traits<typename P1::coef_type>::category_value
    );
}


/// Polynomial coefficients reduction modulo m.
/** Reduce all coefficients of p by modulo m. If zero coefficients
    occure, they will be eliminated. */
template <typename P, typename M>
void sparse_polynom_reduction_mod (P& p, const M& m)
{
    typedef typename P::coef_iterator Piter;
    Piter pend = p.coefs_end();
    for(Piter i = p.coefs_begin(); i != pend;)
        if(is_null(*i = prrem(*i, m)))
            i = Piter(p.erase(i.base()));
        else
            ++i;
}


/// Computes maximum of absolute values of coefficients of polynom p.
template <typename P>
typename P::coef_type max_abs_coef (const P& p)
{
    ARAGELI_ASSERT_0(!p.is_null());
    typename P::coef_type res = null<typename P::coef_type>();

    for(typename P::coef_const_iterator i = p.coefs_begin(); i != p.coefs_end(); ++i)
    {
        typename P::coef_type absval = abs(*i);
        if(absval > res)
            res = absval;
    }

    return res;
}


/// Computes maximum of absolute values of coefficients of polynom p without leading coef.
template <typename P>
typename P::coef_type max_abs_coef_wo_leading (const P& p)
{
    ARAGELI_ASSERT_0(p.degree() > 0);
    typename P::coef_const_iterator i = p.coefs_end();
    --i;
    typename P::coef_type res = null(*i);
    if(i == p.coefs_begin())
        return res;

    do
    {
        --i;
        typename P::coef_type absval = abs(*i);
        if(absval > res)
            res = absval;
    }while(i != p.coefs_begin());

    return res;
}


/// Content of polynomial.
/** WARNING! Now it works only for coefs that are compatible with
    big_int type. */
template <typename P>
typename P::coef_type polynom_content (const P& x)
{
    typedef big_int T;
    T res = null<T>();
    for(typename P::coef_const_iterator i = x.coefs_begin(); i != x.coefs_end(); ++i)
        res = gcd(res, big_int(*i));
    return res;
}


/// Primitive part of polynomial.
template <typename P>
inline P polynom_primpart (const P& x)
{
    return x/polynom_content(x);
}


/// Cauchy's estimation on maximum root absolute value for polynomial.
template <typename T, typename P>
T root_upper_bound_cauchy (const P& p)
{
    ARAGELI_ASSERT_0(p.degree() > 0);

    if(p.size() == 1)
        return null(p.leading_coef());
    else
        return
            unit<T>() +
            T(max_abs_coef_wo_leading(p))/abs(T(p.leading_coef()));
}


/// Reciprocal polynomial.
template <typename P>
P reciprocal_poly (const P& p)
{
    P res;
    typedef typename P::monom monom;
    for(typename P::monom_const_iterator i = p.monoms_begin(); i != p.monoms_end(); ++i)
        res += monom(i->coef(), p.degree() - i->degree());

    ////COUTCOM std::cout << "\nreciprocal_poly(" << p << ") = " << res;

    return res;
}


template <typename In, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& polynom_output_list
(
    std::basic_ostream<Ch, ChT>& out,
    In begin,
    In end,
    const char* first_bracket,
    const char* second_bracket,
    const char* separator
)
{
    out << first_bracket;

    if(begin != end)
    {
        In i = begin;
        output_list(out, *i, first_bracket, second_bracket, separator);

        for(++i; i != end; ++i)
        {
            out << separator;
            output_list(out, *i, first_bracket, second_bracket, separator);
        }
    }

    out << second_bracket;
    return out;
}


enum monoms_order
{
    mo_inc,
    mo_dec
};


template <typename In, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& polynom_output_var
(
    std::basic_ostream<Ch, ChT>& out,
    In begin,
    In end,
    monoms_order mo,
    const char* var,
    const char* mul,
    const char* pow
)
{
    if(begin == end)
        out << "0";
    else
    {
        In i = begin;
        output_var(out, *i, true, var, mul, pow);

        for(++i; i != end; ++i)
            output_var(out, *i, false, var, mul, pow);
    }

    return out;
}


} // namespace Arageli


#if 0
#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_POLYALG
    #include "polyalg.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_POLYALG
#endif
#endif

#endif  //  #ifndef _ARAGELI_polyalg_hpp_
