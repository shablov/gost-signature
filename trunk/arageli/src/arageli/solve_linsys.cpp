/*****************************************************************************

    solve_linsys.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2006--2007 Sergey S. Lyalin
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
    \file
    See description for solve_linsys.hpp file.
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_solve_linsys)

#include <cstddef>

#include "std_import.hpp"
#include "factory.hpp"
#include "cmp.hpp"
#include "gauss.hpp"
#include "gcd.hpp"
#include "cmp.hpp"
#include "misc.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "smith.hpp"
#include "intalg.hpp"

#include "solve_linsys.hpp"


namespace Arageli
{


namespace _Internal
{

template <typename MA, typename VB, typename MB>
void rational_to_integer
(
    const MA &A,
    const VB &b,
    MB &B
)
{
    // WARNING! Member function 'at' is used!

    typedef typename MA::element_type T_A;

    std::size_t n = A.ncols(), m = A.nrows();

    for(std::size_t i = 0; i < m; i ++)
    {
        T_A alpha = unit(alpha);

        B(i, 0) = b.at(i).numerator();
        alpha = lcm(alpha, noncopy_cast<T_A>(b.at(i).denominator()));

        for(std::size_t j = 0; j < n; j++)
        {
            B(i, j + 1) = -A.at(i, j).numerator();
            alpha = lcm(alpha, noncopy_cast<T_A>(A.at(i, j).denominator()));
        }

        B(i, 0) *= alpha/b.at(i).denominator();

        for(std::size_t j = 0; j < n; j++)
            B(i, j + 1) *= alpha/A.at(i, j).denominator();
    }
}


//    общий случай
//    базисные переменные выражены через свободные
//    basis    -    набор индексов базисных переменных
//    X.col(0)    -    столбец свободных членов
//    X.col(1)            |
//    ...                    |    коэффициенты при свободных
//    X.col(n-basis.size)    |    переменных
//
//    функция возвращает
//        true    - если система совместна в Q
//        false    - в противном случае
template
<
    typename MA,
    typename VB,
    typename MX,
    typename Basis
>
bool solve_rational_linsystem
(
    const MA &A,
    const VB &b,
    MX &X,
    Basis &basis
)
{
    typedef typename MA::element_type T_A;

    std::size_t n = A.ncols(), m = A.nrows();
    matrix<T_A, false> B(m, n + 1);

    rational_to_integer(A, b, B);

    T_A d = unit(d);
    basis.resize(0);

    std::size_t r = 0;

    for(std::size_t j = 1; r < m && r <= n - j;)
    {
        T_A max = null(B(0, 0));
        std::size_t i_max = r;

        for(std::size_t ii = r; ii < m; ii++)
            if(abs(B(ii, j)) > max)
            {
                max = abs(B(ii, j));
                i_max = ii;
            }

        if(is_null(max))
        {
            ++j;
            continue;
        }

        if(r != i_max)
            B.swap_rows(r, i_max);
        if(is_negative(B(r, j)))
            B.mult_row(r, -1);

        T_A pivot = B(r, j);

        for(std::size_t ii = 0; ii < m; ii++)
            if(ii != r)
            {
                T_A alpha = B(ii, j);
                B.mult_row(ii, pivot);
                B.addmult_rows(ii, r,  -alpha);
                B.div_row(ii, d);
            }
        d = B(r, j);
        B.erase_col(j);
        basis.push_back(r + j - 1);
        r ++;
    }

    for(std::size_t i = r; i < m; i++)
        if(!is_null(B(i, 0)))
            return false;
    X = -B;
    if(m > r)
        X.erase_rows(r, m - r);
    X /= d;
    return true;
}


} // namespace _Internal


template <typename A, typename B, typename GX, typename Offset>
solve_linsys_result solve_linsys_integer
(const A& a, const B& b, Offset& offset, GX& gx)
{
    // алгоритм из Василенко

    typedef typename A::element_type T_A;

    std::size_t m = a.nrows(), n = a.ncols(), I = 0, J = 0, R = 0;
    matrix<T_A, false> tmp = a;
    tmp.insert_col(n, -b);
    gx.assign_eye(n);

    for(;;)
    {
        T_A min = null(min);
        for(std::size_t i = R; i < m; i++)
        {
            for(std::size_t j = R; j < n; j++)
                if(!is_null(tmp(i, j)) && (is_null(min) || abs(tmp(i, j)) < min))
                {
                    min = abs(tmp(i, j));
                    I = i;
                    J = j;
                }

            if(!is_null(min))
                break;
        }

        if(is_null(min))
            break;

        if(is_negative(tmp(I, J)))
        {
            tmp.mult_col(J, -1);
            gx.mult_col(J, -1);
        }
        std::size_t min_j = J;
        do
        {
            min = null(min);

            for(std::size_t j = R; j < n; j++)
                if(j != J)
                {
                    T_A q = prquot(tmp(I, j), tmp(I, J));
                    tmp.addmult_cols(j, J, -q);
                    gx.addmult_cols(j, J, -q);

                    if(!is_null(tmp(I, j)) && (is_null(min) || abs(tmp(I, j)) < min))
                    {
                        min = abs(tmp(I, j));
                        min_j = j;
                    }
                }
            if(is_null(min))
                break;
            J = min_j;
        }while(1);

        if(J != R)
        {
            tmp.swap_cols(R, J);
            gx.swap_cols(R, J);
        }

        if(I != R)
            tmp.swap_rows(R, I);
        R++;
    }

    gx.insert_col(0);

    for(std::size_t i = 0; i < R; i++)
        if(!is_null(tmp(i, n)%tmp(i, i)))
        {
            gx.resize(n, 0);
            return SLR_EMPTY;
        }
        else
        {
            T_A alpha = tmp(i, n)/tmp(i, i);
            tmp.addmult_cols(n, i, -alpha);
            gx.addmult_cols(0, 1, -alpha);
            gx.erase_col(1);
        }

    offset = gx.take_col(0);

    return gx.ncols() ? SLR_MULTIPLE : SLR_UNIQUE;
}


template <typename A, typename D, typename GX, typename Order>
solve_linsys_result solve_linsys_integer_mod
(const A& a, const D& d, GX& gx, Order& order)
{
    ARAGELI_ASSERT_0(a.is_square());    // temporary limitation
    ARAGELI_ASSERT_0(is_positive(d));

    typedef typename A::element_type TA;
    typedef typename A::size_type size_type;

    matrix<TA, false> q, b;

    {
        matrix<TA, false> p;
        TA det;
        size_type rank;
        smith(a, b, p, q, rank, det);

        ARAGELI_ASSERT_0(!is_null(det));
    }

    size_type k = 0;
    vector<size_type, false> erased;
    for(; k < b.nrows() && is_unit(b(k, k)); ++k)
        erased.push_back(k);

    if(k < b.nrows())
    {
        order.resize(b.nrows() - k);
        for(size_type i = k; i < b.nrows(); ++i)
        {
            ARAGELI_ASSERT_0(is_divisible(d, b(i, i)));
            q.mult_col(i, d/b(i, i));
            q.mod_col(i, d);
            order[i - k] = b(i, i);
        }

        q.erase_cols(erased);
        gx = q;
        return SLR_MULTIPLE;
    }
    else
    {
        // only one point --- zero
        gx.assign_fromsize(a.ncols(), 1);    // zeros
        order.assign_fromval(1, 1);
        return SLR_UNIQUE;
    }
}


template <typename A, typename B, typename GX, typename Offset>
solve_linsys_result solve_linsys_rational
(const A& a, const B& b, Offset& offset, GX& gx)
{
    vector<std::size_t, false> basis;

    if(!_Internal::solve_rational_linsystem(a, b, gx, basis))
        return SLR_EMPTY;

    typename GX::element_type one = unit(gx(0, 0));

    for(std::size_t i = 0, j = 0, k = 0; i < a.ncols(); i++)
    {
        if(k < basis.size() && i == basis[k])
            k++;
        else
        {
            gx.insert_row(i);
            gx(i, ++j) = one;
        }
    }

    ARAGELI_ASSERT_1(gx.ncols());

    offset = gx.take_col(0);

    return gx.ncols() ? SLR_MULTIPLE : SLR_UNIQUE;
}


template <typename BA, typename GX, typename Offset>
solve_linsys_result solve_linsys_rational_ba
(const BA& ba, Offset& offset, GX& gx)
{
    typedef typename BA::value_type T;
    matrix<T, false> a = ba;
    vector<T, false> b = -a.take_col(0);
    return solve_linsys_rational(a, b, offset, gx);
}


template <typename BA, typename X>
solve_linsys_result solve_linsys_rational_ba (const BA& ba, X& x)
{
    typedef typename BA::value_type T;
    matrix<T, false> a = ba;
    vector<T, false> b = -a.take_col(0);
    return solve_linsys_rational(a, b, x);
}


template <typename A, typename B, typename X>
solve_linsys_result solve_linsys_field (const A& a, const B& b, X& x)
{
    // WARNING! Slow and temporary implementation.

    ARAGELI_ASSERT_0(a.is_square());    // temporary restriction

    try
    {
        x = inverse(a)*b;
        return SLR_UNIQUE;
    }
    catch(const matrix_is_singular&)
    {
        return SLR_MULTIPLE;
    }
}


template <typename A, typename B>
B solve_linsys_field (const A& a, const B& b)
{
    // WARNING! Slow and temporary implementation.

    ARAGELI_ASSERT_0(a.is_square());    // temporary restriction

    try
    {
        return inverse(a)*b;
    }
    catch(const matrix_is_singular&)
    {
        throw no_unique_solution();
    }
}


}


#if ARAGELI_DEBUG_LEVEL > 3

#include "big_int.hpp"
#include "residue.hpp"
#include "rational.hpp"

namespace Arageli
{

template solve_linsys_result solve_linsys_integer
(
    const matrix<big_int>& a,
    const vector<big_int>& b,
    vector<big_int>& offset,
    matrix<big_int>& gx
);

template solve_linsys_result solve_linsys_integer
(
    const matrix<int>& a,
    const vector<int>& b,
    vector<int>& offset,
    matrix<int>& gx
);

template solve_linsys_result solve_linsys_integer
(
    const matrix<int>& a,
    const vector<int>& b,
    vector<big_int>& offset,
    matrix<big_int>& gx
);


template solve_linsys_result solve_linsys_integer_mod
(
    const matrix<big_int>& a,
    const big_int& d,
    matrix<big_int>& gx,
    vector<big_int>& order
);


template solve_linsys_result solve_linsys_integer_mod
(
    const matrix<int>& a,
    const int& d,
    matrix<int>& gx,
    vector<int>& order
);


template solve_linsys_result solve_linsys_integer_mod
(
    const matrix<big_int>& a,
    const int& d,
    matrix<big_int>& gx,
    vector<int>& order
);


template solve_linsys_result solve_linsys_rational
(
    const matrix<rational<big_int> >& a,
    const vector<rational<big_int> >& b,
    vector<rational<big_int> >& offset,
    matrix<rational<big_int> >& gx
);


template solve_linsys_result solve_linsys_rational
(
    const matrix<rational<int> >& a,
    const vector<rational<int> >& b,
    vector<rational<int> >& offset,
    matrix<rational<int> >& gx
);

template solve_linsys_result solve_linsys_rational
(
    const matrix<rational<int> >& a,
    const vector<rational<int> >& b,
    vector<rational<big_int> >& offset,
    matrix<rational<big_int> >& gx
);


template solve_linsys_result solve_linsys_field
(
    const matrix<rational<big_int> >& a,
    const vector<rational<big_int> >& b,
    vector<rational<big_int> >& x
);

template solve_linsys_result solve_linsys_field
(
    const matrix<rational<int> >& a,
    const vector<rational<int> >& b,
    vector<rational<int> >& x
);

template solve_linsys_result solve_linsys_field
(
    const matrix<residue<int> >& a,
    const vector<residue<int> >& b,
    vector<residue<int> >& x
);

template solve_linsys_result solve_linsys_field
(
    const matrix<residue<big_int> >& a,
    const vector<residue<big_int> >& b,
    vector<residue<big_int> >& x
);


template vector<rational<big_int> > solve_linsys_field
(
    const matrix<rational<big_int> >& a,
    const vector<rational<big_int> >& b
);

template vector<rational<int> > solve_linsys_field
(
    const matrix<rational<int> >& a,
    const vector<rational<int> >& b
);

template vector<residue<int> > solve_linsys_field
(
    const matrix<residue<int> >& a,
    const vector<residue<int> >& b
);

template vector<residue<big_int> > solve_linsys_field
(
    const matrix<residue<big_int> >& a,
    const vector<residue<big_int> >& b
);


}

#endif

#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
