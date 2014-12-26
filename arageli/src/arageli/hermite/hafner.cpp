/*****************************************************************************

    hermite/hafner.cpp -- See declarations in hermite_hafner.hpp.

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
    See description for hermite_hafner.hpp file.
*/


#include "../config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_hermite_hafner)

#include <cstddef>

#include "../exception.hpp"
#include "../factory.hpp"
#include "../cmp.hpp"
#include "../intalg.hpp"
#include "../gcd.hpp"
#include "../vector.hpp"

#include "hafner.hpp"


namespace Arageli
{


template
<
    typename MA,
    typename MH,
    typename MU,
    typename Basis,
    typename T_N
>
void hermite_hafner
(
    const MA &A,
    MH &H,
    MU &U,
    Basis &basis,
    T_N N
)
{
    typedef typename MH::element_type T_H;
    typedef typename MU::element_type T_U;

    std::size_t
        n = A.ncols(),
        m = A.nrows();
    U.assign_eye(m);
    basis.resize(0);
    H = A;

    for(std::size_t j = 0, r = 0; j < n && r < m; j ++)
    {
        //H(r, j) = mod(H(r, j), N);

        for(std::size_t i = r + 1; i < m; i ++)
        {
            //  H.mod_row(i, N);
            //if(is_null(H(i, j))) continue;

            T_H
                a = H(r, j),
                b = H(i, j);
            T_H u, v, w, z;
            gcdex(a, b, N, u, v, w, z);

            vector<T_H, false> H_row_r = H.copy_row(r);
            H.addmult_rows(r, u, i, v);
            H.mod_row(r, N);
            H.addmult_rows(i, z, H_row_r, w);
            H.mod_row(i, N);

            vector<T_U, false> U_row_r = U.copy_row(r);
            U.addmult_rows(r, u, i, v);
            U.mod_row(r, N);
            U.addmult_rows(i, z, U_row_r, w);
            U.mod_row(i, N);
        }

        if(!is_null(H(r, j)))
            r++;
    }

    for(std::size_t j = 0, r = 0; j < n && r < m; j ++)
        if(!is_null(H(r, j)))
        {
            T_H c = unit(H(r, j), N);
            H.mult_row(r, c);
            H.mod_row(r, N);
            U.mult_row(r, c);
            U.mod_row(r, N);

            for(std::size_t i = 0; i < r; i++)
            {
                T_H q = quo_mod(H(i, j), H(r, j), N);
                H.addmult_rows(i, r, -q);
                H.mod_row(i, N);
                U.addmult_rows(i, r, -q);
                U.mod_row(i, N);
            }

            basis.push_back(j);
            r++;
        }
}


}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
