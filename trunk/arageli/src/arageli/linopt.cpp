/*****************************************************************************

    linopt.cpp -- See declarations in linopt.hpp.

    This file is a part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2006--2007 Sergey S. Lyalin

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
    \file
    See description for linopt.hpp file.
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_linopt)

#include "factory.hpp"
#include "vector.hpp"
#include "simplex_method.hpp"

#include "linopt.hpp"


namespace Arageli
{


namespace _Internal
{

inline linopt_result simplex_method_to_linop_result
(Arageli::simplex_method::result_kind x)
{
    switch(x)
    {
        case simplex_method::rk_found:
            return lor_found;
        case simplex_method::rk_empty:
            return lor_empty;
        case simplex_method::rk_infinite:
            return lor_infinite;
        default:
            ARAGELI_ASSERT_1(!"It's impossible!");
    }
}

}


template
<
    typename A,
    typename B,
    typename C,
    typename Res,
    typename BasisX,
    typename Basis
>
linopt_result linear_maximize_canonical
(
    const A& a,
    const B& b,
    const C& c,
    Res& res,
    BasisX& basis_x,
    Basis& basis
)
{
    ARAGELI_ASSERT_0(a.nrows() == b.size());

    A allow_a = a;
    B allow_b = b;

    for(typename B::size_type i = 0; i < allow_b.size(); ++i)
        if(is_negative(allow_b[i]))
        {
            opposite(&allow_b[i]);
            allow_a.mult_row(i, opposite_unit<typename A::element_type>());
        }

    using namespace simplex_method;

    result_kind rk = primal_row_with_artificial_basis
    (
        allow_a,
        allow_b,
        c,
        basis_x,
        basis,
        res
    );

    if(rk == rk_found)
        basis -= 1;

    return _Internal::simplex_method_to_linop_result(rk);
}


template
<
    typename A,
    typename B,
    typename C,
    typename Res,
    typename X
>
linopt_result linear_maximize_canonical
(
    const A& a,
    const B& b,
    const C& c,
    Res& res,
    X& x
)
{
    X basis_x;
    vector<typename C::size_type, false> basis;
    linopt_result ret = linear_maximize_canonical(a, b, c, res, basis_x, basis);

    if(ret == lor_found)
    {
        x.resize(c.size());
        x[basis] = basis_x;
    }

    return ret;
}


template
<
    typename A,
    typename B,
    typename C,
    typename Res,
    typename X
>
linopt_result intlinear_maximize_canonical
(
    A a,
    B b,
    const C& c,
    Res& res,
    X& x
)
{
    ARAGELI_ASSERT_0(a.nrows() == b.size());

    for(typename B::size_type i = 0; i < b.size(); ++i)
        if(is_negative(b[i]))
        {
            opposite(&b[i]);
            a.mult_row(i, opposite_unit<typename A::element_type>());
        }

    using namespace simplex_method;

    typedef typename A::element_type T;
    typedef vector<typename C::size_type, false> Basis;

    // Introduce artificial basis.

    matrix<T, false> q;    // a simplex table
    row_table_create(a, b, q);
    Basis basis;
    result_kind rk = basis_artificial(q, basis);
    ARAGELI_ASSERT_1(rk == rk_found)

    if(is_negative(q(0, 0)))
        return lor_empty;

    // Solve noninteger problem.

    row_table_place_c(c, q);
    row_table_pivot_basis_c(q, basis);
    rk = primal_row_iters(q, basis);

    if(rk != rk_found)
        return _Internal::simplex_method_to_linop_result(rk);

    //X x_opt;
    //row_table_extract_solution(q, basis, x_opt);

    // Solve integer problem.

    matrix<T, false> t;
    Basis nonbasis;
    row_to_col_table(q, basis, t, nonbasis);
    rk = gomory1_iters(t, nonbasis);

    if(rk == rk_found)
    {
        col_table_extract_solution(t, a.ncols(), x);
        res = t(0, 0);
        return lor_found;
    }
    else
        return _Internal::simplex_method_to_linop_result(rk);
}


template
<
    typename A,
    typename B,
    typename C,
    typename Res,
    typename BasisX,
    typename Basis
>
linopt_result linear_minimize_canonical
(
    const A& a,
    const B& b,
    const C& c,
    Res& res,
    BasisX& basis_x,
    Basis& basis
)
{
    C invc = c;
    opposite(&invc);

    linopt_result ret =
        linear_maximize_canonical(a, b, invc, res, basis_x, basis);

    if(ret == lor_found)
        opposite(&res);
    return ret;
}


template
<
    typename A,
    typename B,
    typename C,
    typename Res,
    typename X
>
linopt_result linear_minimize_canonical
(
    const A& a,
    const B& b,
    const C& c,
    Res& res,
    X& x
)
{
    C invc = c;
    opposite(&invc);

    linopt_result ret =
        linear_maximize_canonical(a, b, invc, res, x);

    if(ret == lor_found)
        opposite(&res);
    return ret;
}


template
<
    typename A,
    typename B,
    typename C,
    typename Res,
    typename X
>
linopt_result linear_minimize_dual_canonical
(
    const A& a,
    const B& b,
    const C& c,
    Res& res,
    X& x
)
{
    A da;
    B db;
    C dc;
    vector<typename C::size_type, false> basis;
    simplex_method::primal_to_dual_canonical(a, b, c, da, db, dc, basis);
    X xe;
    linopt_result ret = linear_maximize_canonical(da, db, dc, res, xe);
    //std::cout << xe;

    if(ret == lor_found)
    {
        simplex_method::extract_solution_from_extended_canonical(xe, b.size(), x);
        opposite(&res);
    }

    return ret;
}



template
<
    typename A,
    typename B,
    typename C,
    typename Res,
    typename X
>
linopt_result intlinear_minimize_canonical
(
    const A& a,
    const B& b,
    const C& c,
    Res& res,
    X& x
)
{
    C invc = c;
    opposite(&invc);

    linopt_result ret =
        linear_maximize_canonical(a, b, invc, res, x);

    if(ret == lor_found)
        opposite(&res);
    return ret;
}


template
<
    typename A,
    typename B,
    typename C,
    typename Res,
    typename X
>
linopt_result intlinear_minimize_dual_canonical
(
    const A& a,
    const B& b,
    const C& c,
    Res& res,
    X& x
)
{
    A da;
    B db;
    C dc;
    vector<typename C::size_type, false> basis;
    simplex_method::primal_to_dual_canonical(a, b, c, da, db, dc, basis);
    X xe;
    linopt_result ret = intlinear_maximize_canonical(da, db, dc, res, xe);
    //std::cout << xe;

    if(ret == lor_found)
    {
        simplex_method::extract_solution_from_extended_canonical(xe, b.size(), x);
        opposite(&res);
    }

    return ret;
}


}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
