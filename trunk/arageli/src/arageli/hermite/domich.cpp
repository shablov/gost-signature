/*****************************************************************************

    hermite/domich.cpp -- See declarations in hermite_domich.hpp.

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
    See description for hermite_domich.hpp file.
*/


#include "../config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_hermite_domich)

#include <cstddef>

#include "../exception.hpp"
#include "../std_import.hpp"
#include "../factory.hpp"
#include "../cmp.hpp"
#include "../intalg.hpp"
#include "../gcd.hpp"
#include "../vector.hpp"
#include "../matrix.hpp"
#include "../bareiss.hpp"

#include "domich.hpp"


namespace Arageli
{


template
<
    typename MA,
    typename MH,
    typename MU,
    typename Basis,
    typename T_det
>
void hermite_domich
(
    const MA &A,
    MH &H,
    MU &U,
    Basis &basis,
    T_det &det
)
{
    typedef typename MA::element_type T_A;
    typedef typename MH::element_type T_H;

    matrix<T_A, false> P, Q, PA;
    std::size_t
        n = A.ncols(),
        m = A.nrows(),
        rank;
    bareiss_pq(A, P, Q, rank, det);

    ARAGELI_ASSERT_0(n == rank);

    //if(n != rank) return;

    H = P*A;

    if(m > rank)
        H.insert_cols(rank, m - rank, null(H(0, 0)));
    for(std::size_t i = rank; i < m; i++)
        H(i, i) = unit(H(i, i));

    PA = H;

    T_H N = abs(det)*abs(det);


    for(std::size_t r = 0; r < m; r ++)
    {
        H(r, r) = mod(H(r, r), N);

        for(std::size_t i = r + 1; i < m; i ++)
        {
            H.mod_row(i, N);
            if(is_null(H(i, r)))
                continue;

            T_A
                a = H(r, r),
                b = H(i, r);
            T_A u, v, w, z;
            gcdex(a, b, N, u, v, w, z);

            vector<T_H, false> row_r = H.copy_row(r);
            H.addmult_rows(r, u, i, v);
            H.mod_row(r, N);
            H.addmult_rows(i, z, row_r, w);
            H.mod_row(i, N);
        }
    }

    for(std::size_t r = 0; r < m; r ++)
    {
        T_H c = unit(H(r, r), N);
        H.mult_row(r, c);
        H.mod_row(r, N);

        for(std::size_t i = 0; i < r; i++)
        {
            T_H q = quo_mod(H(i, r), H(r, r), N);
            H.addmult_rows(i, r, -q);
            H.mod_row(i, N);
        }
    }

    T_A detPA;
    adjoint(PA, U, detPA);
    U = H*U;
    U /= detPA;

    if(m > rank)
        H.erase_cols(rank, m - rank);
    U = U*P;

    ARAGELI_ASSERT_1(U*A == H);
    ARAGELI_ASSERT_1(is_unit(abs(det_brs(U))));
}


}


#if ARAGELI_DEBUG_LEVEL > 3

#include "../big_int.hpp"

namespace Arageli
{

template void hermite_domich
(
    const matrix<big_int> &A,
    matrix<big_int> &H,
    matrix<big_int> &U,
    vector<std::size_t>& basis,
    big_int &det
);

template void hermite_domich
(
    const matrix<int> &A,
    matrix<int> &H,
    matrix<int> &U,
    vector<std::size_t>& basis,
    int &det
);

}

#endif


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
