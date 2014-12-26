/*****************************************************************************

    hermite/storjohann.cpp -- See declarations in hermite_storjohann.hpp.

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
    See description for hermite_storjohann.hpp file.
*/


#include "../config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_hermite_storjohann)

#include <cstddef>

#include "../exception.hpp"
#include "../std_import.hpp"
#include "../factory.hpp"
#include "../cmp.hpp"
#include "../intalg.hpp"
#include "../powerest.hpp"
#include "../gcd.hpp"

#include "storjohann.hpp"


namespace Arageli
{

namespace _Internal
{

template
<
    typename MB,
    typename Basis
>
void find_next_basis_items
(
    MB &B,
    Basis &basis,
    std::size_t j1,
    std::size_t &j2,
    std::size_t r1,
    std::size_t &r2
)
{
    typedef typename MB::element_type T_B;

    T_B N1 = B(r1, j1);
    for(std::size_t j = j1 + 1; j < B.ncols() - 1; j ++)
    {
        T_B N2 = B(r1, j);
        for(std::size_t i = r1 + 1; i < B.nrows() - 1; i ++)
            if(N1*B(i, j) != N2*B(i, j1))
            {
                basis.push_back(j - 1);
                j2 = j;
                r2 = i;
                return;
            }
    }
    j2 = B.ncols() - 1;
    r2 = r1 + 1;
}

} // namespace _Internal


template
<
    typename MA,
    typename MB,
    typename MQ,
    typename Basis,
    typename T_det
>
void hermite_storjohann
(
    const MA &A,
    MB &B,
    MQ &Q,
    Basis& basis,
    T_det& det
)
{
    typedef typename MB::element_type T_B;

    std::size_t
        n = A.ncols(),
        m = A.nrows(),
        j1 = 0,
        r1 = 0,
        j2,
        r2;
    T_det d = unit(det);
    Q.assign_eye(m + 2);
    basis.resize(0);
    det = unit(det);

    B = A;
    B.insert_col(n, null(B(0, 0)));
    B.insert_row(m, null(B(0, 0)));
    B.insert_col(0, null(B(0, 0)));
    B.insert_row(0, null(B(0, 0)));
    B(0, 0) = unit(B(0, 0));
    B(m + 1, n + 1) = unit(B(0, 0));

    for(;;)
    {
        _Internal::find_next_basis_items(B, basis, j1, j2, r1, r2);
        for(std::size_t k = r1 + 1; k < B.nrows(); k ++)
            if(k != r2 && !is_null(B(k, j1)))
            {
                T_B g = gcd(B(r2, j1), B(k, j1));
                T_B a = mod(B(r2, j1)/g, B(r1, j1));
                T_B b = mod(B(k, j1)/g, B(r1, j1));
                T_B c = stab(a, b, B(r1, j1));

                if
                (
                    is_null
                    (
                        B(r1, j1)*(B(r2, j2) + c*B(k, j2)) -
                        B(r1, j2)*(B(r2, j1) + c*B(k, j1))
                    )
                )
                    c = -stab(a, -b, B(r1, j1));

                B.addmult_rows(r2, k, c);
                Q.addmult_rows(r2, k, c);
            }


        T_B
            a = B(r1, j1),
            b = B(r2, j1),
            u,
            v,
            g = euclid_bezout(a, b, u, v),
            s = unit(s);
        //  if(is_negative(g)){opposite(&g); opposite(&u); opposite(&v);}
        if(a*B(r2, j2)/g < b*B(r1, j2)/g)
            opposite(&s);

        B.addmult_rows(r2, s*a/g, r1, -s*b/g);
        B.addmult_rows(r1, s, r2, v);
        B.div_row(r1, s*a/g);

        Q.addmult_rows(r2, s*a/g, r1, -s*b/g);
        Q.addmult_rows(r1, s, r2, v);
        Q.div_row(r1, s*a/g);

        for(std::size_t i = 0; i < B.nrows(); i ++)
            if(i != r1 && !is_null(B(i, j1)))
            {
                T_B q = prquot(B(i, j1), B(r1, j1));
                B.addmult_rows(i, r1, -q);
                Q.addmult_rows(i, r1, -q);
            }

        if(j2 == B.ncols() - 1)
            break;

        for(std::size_t i = 0; i < B.nrows(); i ++)
            if(i != r2 && !is_null(B(i, j2)))
            {
                T_B q = prquot(B(i, j2), B(r2, j2));
                B.addmult_rows(i, r2, -q);
                Q.addmult_rows(i, r2, -q);
            }
        j1 = j2;
        r1 ++;
        if(r2 != r1)
        {
            B.swap_rows(r2, r1);
            Q.swap_rows(r2, r1);
            opposite(&d);
        }
    }

    B.erase_col(0);
    B.erase_row(0);
    B.erase_col(n);
    B.erase_row(m);
    Q.erase_col(0);
    Q.erase_row(0);
    Q.erase_col(m);
    Q.erase_row(m);

    for(std::size_t i = 0; i < basis.size(); i ++)
        det *= B(i, basis[i]);
    det *= d;

    ARAGELI_ASSERT_1(is_unit(abs(det_brs(Q))));
    ARAGELI_ASSERT_1(Q*A == B);
}


}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
