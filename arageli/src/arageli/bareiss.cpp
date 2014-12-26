/*****************************************************************************

    bareiss.cpp -- See declarations in bareiss.hpp.

    This file is a part of the Arageli library.

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
    \file bareiss.cpp
    \brief The bareiss.hpp file stuff implementation.
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_bareiss)

#include <cstddef>

#include "factory.hpp"
#include "cmp.hpp"
#include "vector.hpp"

#include "bareiss.hpp"

#include "std_import.hpp"


namespace Arageli
{


template
<
    typename MA,
    typename MB,
    typename MQ,
    typename Basis,
    typename T_det
>
void bareiss
(
    const MA& A, MB& B, MQ& Q,
    Basis& basis, T_det& det
)
{
    typedef typename MB::element_type T_B;

    std::size_t m = A.nrows(), n = A.ncols();
    Q.assign_eye(m);
    basis.resize(0);
    det = unit(det);
    T_B d = unit(d);
    B = A;

    for(std::size_t i = 0, j = 0; i < m && j < n; j++)
    {
        T_B max = null(B(0, 0));
        std::size_t i_max = i;

        for(std::size_t ii = i; ii < m; ii++)
            if(abs(B(ii, j)) > max)
            {
                max = abs(B(ii, j));
                i_max = ii;
            }

        if(is_null(max))
        {
            /*det = null(det);*/
            continue;
        }

        if(i != i_max)
        {
            B.swap_rows(i, i_max);
            Q.swap_rows(i, i_max);
            opposite(&det);
        }

        if(is_negative(B(i, j)))
        {
            B.mult_row(i, -1);
            Q.mult_row(i, -1);
            opposite(&det);
        }

        T_B pivot = B(i, j);

        for(std::size_t ii = 0; ii < m; ii++)
            if(ii != i)
            {
                T_B alpha = B(ii, j);
                B.mult_row(ii, pivot);
                B.addmult_rows(ii, i,  -alpha);
                B.div_row(ii, d);

                Q.mult_row(ii, pivot);
                Q.addmult_rows(ii, i,  -alpha);
                Q.div_row(ii, d);
            }

        d = B(i, j);
        basis.push_back(j);
        i++;
    }
    det *= d;
}


template
<
    typename MA,
    typename MP,
    typename MQ,
    typename Rank,
    typename T_det
>
void bareiss_pq
(
    const MA& A,
    MP& P,
    MQ& Q,
    Rank& rank,
    T_det& det
)
{
    typedef typename MA::element_type T_A;

    std::size_t m = A.nrows(), n = A.ncols();
    MA B = A;//matrix<T_A, false> B = A;
    P.assign_eye(m);
    Q.assign_eye(n);
    rank = null(rank);
    det = unit(det);
    T_A d = unit(d);

    for(std::size_t r = 0; r < std::min(m, n); r++)
    {
        T_A max = null(B(0, 0));
        std::size_t i_max = r, j_max = r, j = r;

        while(is_null(max) && j < n)
        {
            for (std::size_t i = r; i < m; i++)
                if (abs(B(i, j)) > max)
                {
                    max = abs(B(i, j));
                    i_max = i;
                    j_max = j;
                }
            j++;
        }

        if(is_null(max)) break;

        if(r != i_max)
        {
            B.swap_rows(r, i_max);
            P.swap_rows(r, i_max);
            opposite(&det);
        }

        if(r != j_max)
        {
            B.swap_cols(r, j_max);
            Q.swap_cols(r, j_max);
            opposite(&det);
        }

        if(is_negative(B(r, r)))
        {
            B.mult_row(r, -1);
            P.mult_row(r, -1);
            opposite(&det);
        }

        T_A pivot = B(r, r);

        for(std::size_t i = r + 1; i < m; i++)
        {
            T_A alpha = B(i, r);
            B.mult_row(i, pivot);
            B.addmult_rows(i, r, -alpha);
            B.div_row(i, d);
        }

        d = B(r, r);
        rank++;
    }
    det *= d;
}


template <typename MA> MA adjoint (const MA& A)
{
    // Why false/true:
    //matrix<T, false> B;
    //matrix<T, true> Q;

    MA B, Q;
    vector<std::size_t, false> basis;
    typename MA::element_type det;

    bareiss(A, B, Q, basis, det);

    return Q;
}


template
<
    typename MA,
    typename MP,
    typename T_det
>
void adjoint (const MA& A, MP& P, T_det& det)
{
    //matrix<T_A, false> B;
    MA B;
    vector<std::size_t, false> basis;
    bareiss(A, B, P, basis, det);
}


template <typename MA>
typename MA::element_type det_brs (const MA& A)
{
    typename MA::size_type rank;
    MA P, Q;//matrix<T, false>  P, Q;
    typename MA::element_type det;

    bareiss_pq(A, P, Q, rank, det);

    return det;
}


template <typename MA>
typename MA::size_type rank_brs (const MA& A)
{
    typename MA::size_type rank;
    MA P, Q;
    typename MA::element_type det;

    bariess(A, P, Q, rank, det);

    return rank;
}


}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
