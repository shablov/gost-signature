/*****************************************************************************

    smith/storjohann.cpp -- See declarations in smith_storjohann.hpp.

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
    \file
    See description for smith_storjohann.hpp file.
*/


#include "../config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_smith_storjohann)

#include <cstddef>

#include "../exception.hpp"
#include "../std_import.hpp"
#include "../factory.hpp"
#include "../cmp.hpp"
#include "../intalg.hpp"
#include "../powerest.hpp"
#include "../gcd.hpp"
#include "../bareiss.hpp"
#include "../hermite.hpp"
#include "../matrix.hpp"

#include "storjohann.hpp"


namespace Arageli
{


template
<
    typename MA,
    typename MP,
    typename MQ,
    typename MU,
    typename ME,
    typename MF,
    typename Rank,
    typename T_det
>
void smith_storjohann
(
    const MA &A,
    MP &P,
    MQ &Q,
    MU& U,
    ME& E,
    MF& F,
    Rank &rank,
    T_det &det
)
{
    typedef typename MA::element_type T_A;

    std::size_t
        n = A.ncols(),
        m = A.nrows(),
        corner = 0;
    matrix<T_A, false> PAQ, H, S;
    bareiss_pq(A, P, Q, rank, det);
    PAQ = P*A*Q;

    hermite_upper(PAQ, H, U);
    E.assign_eye(m);
    F.assign_eye(n);

    for(std::size_t i = rank; i > 0; i--)
        for(std::size_t j = i; j < n; j++)
        {
            T_A q = prquot(H(i - 1, j), H(i - 1, i - 1));
            H.addmult_cols(j, i - 1, -q);
            F.addmult_cols(j, i - 1, -q);
        }

    for(std::size_t i = 0; i < rank; i++)
        if(!is_unit(H(i, i)))
            break;
        else
            corner++;

    // Phase1.

    for(std::size_t k = corner + 1; k < rank; k++)
    {
        for(std::size_t i = k; i > corner; i --)
        {
            T_A c = stab(H(i - 1, k), H(i, k), H(i - 1, i - 1));
            H.addmult_rows(i - 1, i, c);
            E.addmult_rows(i - 1, i, c);
            if(i < k)
            {
                T_A alpha = H(i - 1, i)/H(i - 1, i - 1);
                H(i - 1, i) = null(H(i - 1, i));
                F.addmult_cols(i, i - 1, -alpha);
            }
            for(std::size_t j = k + 1; j < n; j ++)
                if(!is_null(H(i - 1, j)))
                {
                    T_A q, r; prdivide(H(i - 1, j), H(i - 1, i - 1), q, r);
                    F.addmult_cols(j, i - 1, -q);
                    H(i - 1, j) = r;
                }
        }

        for(std::size_t i = corner; i < k; i++)
            if(!is_null(H(i, k)))
            {
                T_A a = H(i, i), b = H(i, k), u, v, w, z,
                g = gcdex(a, b, u, v, w, z);
                H.addmult_cols(i, k, v);
                H.mult_col(k, z);
                H(i, i) = g;
                H(i, k) = null(H(i, k));

                F.addmult_cols(k, z, i, w);
                F.addmult_cols(i, k, v);
                F.div_col(i, z);

                for(std::size_t ii = i + 1; ii < k + 1; ii++)
                {
                    T_A alpha = H(ii, i)/H(i, i);
                    H.addmult_rows(ii, i, -alpha);
                    E.addmult_rows(ii, i, -alpha);
                }

                for(std::size_t j = k + 1; j < n; j++)
                {
                    T_A q, r;
                    prdivide(H(i, j), H(i, i), q, r);
                    F.addmult_cols(j, i, -q);
                    H(i, j) = r;
                }
            }

        for(std::size_t j = k + 1; j < n; j++)
        {
            T_A q, r;
            prdivide(H(k, j), H(k, k), q, r);
            F.addmult_cols(j, k, -q);
            H(k, j) = r;
        }
    }

    // Phase2.

    corner = 0;
    for(std::size_t i = 0; i < rank; i++)
        if(!is_unit(H(i, i)))
            break;
        else
            corner++;

    for(std::size_t i = corner; i < rank; i++)
    {
        for(std::size_t j = rank; j < n; j++)
            if(!is_null(H(i, j)))
            {
                T_A a = H(i, i), b = H(i, j), u, v, w, z,
                g = gcdex(a, b, u, v, w, z);

                H.addmult_cols(j, z, i, w);
                H.addmult_cols(i, j, v);
                H.div_col(i, z);

                F.addmult_cols(j, z, i, w);
                F.addmult_cols(i, j, v);
                F.div_col(i, z);

                for(std::size_t ii = i + 1; ii < rank; ii++)
                {
                    T_A q, r;
                    prdivide(H(ii, i), H(ii, ii), q, r);
                    F.addmult_cols(i, ii, -q);
                    H(ii, i) = r;

                    prdivide(H(ii, j), H(ii, ii), q, r);
                    F.addmult_cols(j, ii, -q);
                    H(ii, j) = r;
                }
            }
    }

    // Phase3.

    if(rank == n)
    {
        for(std::size_t j = corner; j < rank; j++)
            for(std::size_t i = j + 1; i < rank; i++)
            {
                T_A q = prquot(H(i, j), H(j, j));
                E.addmult_rows(i, j, -q);
                H(i, j) = null(H(i, j));
            }
    }
    else
    {
        for(std::size_t k = corner + 1; k < rank; k++)
        {
            for(std::size_t j = k; j > corner; j --)
            {
                T_A c = stab(H(k, j - 1), H(k, j), H(j - 1, j - 1));
                if(!is_null(c))
                {
                    H.addmult_cols(j - 1, j, c);
                    F.addmult_cols(j - 1, j, c);
                    if(j < k)
                    {
                        T_A alpha = prquot(H(j, j - 1), H(j - 1, j - 1));
                        H.addmult_rows(j, j - 1, - alpha);
                        E.addmult_rows(j, j - 1, - alpha);
                    }
                }
            }

            for(std::size_t j = corner; j < k; j++)
                if(!is_null(H(k, j)))
                {
                    T_A a = H(j, j), b = H(k, j), u, v, w, z,
                    g = gcdex(a, b, u, v, w, z);
                    H.addmult_rows(j, k, v);
                    H.mult_row(k, z);
                    H(j, j) = g;
                    H(k, j) = null(H(k, j));

                    E.addmult_rows(k, z, j, w);
                    E.addmult_rows(j, k, v);
                    E.div_row(j, z);

                    for(std::size_t jj = j + 1; jj <= k; jj ++)
                        if(!is_null(H(j, jj)))
                        {
                            T_A alpha = prquot(H(j, jj), H(j, j));
                            H.addmult_cols(jj, j, -alpha);
                            F.addmult_cols(jj, j, -alpha);
                        }

                    for(std::size_t ii = j + 1; ii < rank; ii ++)
                        if(!is_null(H(ii, j)))
                        {
                            T_A r, q; prdivide(H(ii, j), H(j, j), q, r);
                            E.addmult_rows(ii, j, -q);
                            H(ii, j) = r;
                        }
                }

            for(std::size_t ii = k + 1; ii < rank; ii ++)if(!is_null(H(ii, k)))
            {
                T_A r, q; prdivide(H(ii, k), H(k, k), q, r);
                E.addmult_rows(ii, k, -q);
                H(ii, k) = r;
            }
        }
    }

    ARAGELI_ASSERT_1(E*U*P*A*Q*F == H);
    ARAGELI_ASSERT_1(is_unit(abs(det_brs(U))));
    ARAGELI_ASSERT_1(is_unit(abs(det_brs(E))));
    ARAGELI_ASSERT_1(is_unit(abs(det_brs(F))));
}


}


#if ARAGELI_DEBUG_LEVEL > 3

#include "../big_int.hpp"

namespace Arageli
{

template void smith_storjohann
(
    const matrix<big_int> &A,
    matrix<big_int> &P,
    matrix<big_int> &Q,
    matrix<big_int>& U,
    matrix<big_int>& E,
    matrix<big_int>& F,
    big_int &rank,
    big_int &det
);

template void smith_storjohann
(
    const matrix<int> &A,
    matrix<int> &P,
    matrix<int> &Q,
    matrix<int>& U,
    matrix<int>& E,
    matrix<int>& F,
    int &rank,
    int &det
);

}

#endif


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
