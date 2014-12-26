/*****************************************************************************

    hermite/classic.cpp -- See declarations in hermite_classic.hpp.

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
    See description for hermite_classic.hpp file.
*/


#include "../config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_hermite_classic)

#include <cstddef>

#include "../exception.hpp"
#include "../std_import.hpp"
#include "../factory.hpp"
#include "../cmp.hpp"

#include "classic.hpp"



namespace Arageli
{

template
<
    typename MA,
    typename MB,
    typename MP,
    typename Basis,
    typename T_det
>
void hermite_classic
(
    const MA &A,
    MB &B,
    MP &P,
    Basis &basis,
    T_det &det
)
{
    typedef typename MB::element_type T_B;

    std::size_t
        m = A.nrows(),
        n = A.ncols(),
        I,
        J = 0,
        R = 0;

    det = unit(det);

    P.assign_eye(m);
    basis.resize(0);
    B = A;

    for(;;)
    {
        T_B min = null(min);
        for(std::size_t j = J; j < n; j++)
        {
            for(std::size_t i = R; i < m; i++)

                if(!is_null(B(i, j)) && (is_null(min) || abs(B(i, j)) < min))
                {
                    min = abs(B(i, j));
                    I = i;
                    J = j;
                }

            if(!is_null(min))break;
        }

        if(is_null(min))break;

        if(is_negative(B(I, J)))
        {
            B.mult_row(I, -1);
            P.mult_row(I, -1);
            opposite(&det);
        }

        std::size_t min_i = I;

        for(;;)
        {
            min = null(min);

            for(std::size_t i = R; i < m; i++)if(i != I)
            {
                T_B q = prquot(B(i, J), B(I, J));
                B.addmult_rows(i, I, -q);
                P.addmult_rows(i, I, -q);

                if(!is_null(B(i, J)) && (is_null(min) || abs(B(i, J)) < min))
                {
                    min = abs(B(i, J));
                    min_i = i;
                }
            }

            if(is_null(min))break;
            I = min_i;
        }

        if(I != R)
        {
            B.swap_rows(R, I);
            P.swap_rows(R, I);
            opposite(&det);
        }

        for(std::size_t i = 0; i < R; i++)
            if(!is_null(B(i, J)))
            {
                T_B q = prquot(B(i, J), B(R, J));
                B.addmult_rows(i, R, -q);
                P.addmult_rows(i, R, -q);
            }

        basis.push_back(J);
        R++;
    }

    for(std::size_t i = 0; i < basis.size(); i++)
        det *= B(i, basis[i]);

    ARAGELI_ASSERT_1(P*A == B);
    ARAGELI_ASSERT_1(is_unit(abs(det_brs(P))));
}


}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
