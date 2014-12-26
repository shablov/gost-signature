/*****************************************************************************

    smith.cpp -- See declarations in smith.hpp.

    This file is a part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2005--2008 Sergey S. Lyalin

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
    See description for smith.hpp file.
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_smith)

#include <cstddef>

#include "exception.hpp"
#include "std_import.hpp"
#include "factory.hpp"
#include "cmp.hpp"
#include "intalg.hpp"
#include "gcd.hpp"
#include "powerest.hpp"
#include "vector.hpp"
#include "matrix.hpp"

#include "sparse_polynom.hpp"    // WARNING! TEMPORARY AS A WORKAROUND TO A BUG

#include "smith.hpp"


namespace Arageli
{


namespace _Internal
{


template <typename M>
bool find_pivot_item
(const M& B, std::size_t corner, std::size_t& min_i, std::size_t& min_j)
{
    typedef typename M::element_type T;
    T min = null<T>();

    for(std::size_t i = corner; i < B.nrows(); i++)
        for(std::size_t j = corner; j < B.ncols(); j++)
            if(!is_null(B(i, j)) && (is_null(min) || std::abs(B(i, j)) < min))
            {
                min = std::abs(B(i, j));
                min_i = i;
                min_j = j;
            }

    return !is_null(min);
}


template <typename M>
std::size_t find_pivot_item_in_row
(const M& B, std::size_t corner, std::size_t i)
{
    typedef typename M::element_type T;
    T min = null<T>();
    std::size_t min_j = 0;

    for(std::size_t j = corner; j < B.ncols(); j++)
        if(!is_null(B(i, j)) && (is_null(min) || std::abs(B(i, j)) < min))
        {
            min = std::abs(B(i, j));
            min_j = j;
        }

    return min_j;
}


template <typename M>
std::size_t find_pivot_item_in_col
(const M& B, std::size_t corner, std::size_t j)
{
    typedef typename M::element_type T;
    T min = null<T>();
    std::size_t min_i = 0;

    for(std::size_t i = corner; i < B.nrows(); i++)
        if(!is_null(B(i, j)) && (is_null(min) || std::abs(B(i, j)) < min))
        {
            min = std::abs(B(i, j));
            min_i = i;
        }

    return min_i;
}


template <typename M>
bool find_non_divisor_of_item
(
    const M& B,
    std::size_t corner,
    typename M::element_type item,
    std::size_t& k,
    std::size_t& l
)
{
    for(std::size_t i = corner; i < B.nrows(); i++)
        for(std::size_t j = corner; j < B.ncols(); j++)
            if(!is_null(B(i, j) % item))
            {
                k = i;
                l = j;
                return true;
            }

    return false;
}


} // namespace _Internal --------------------------------------


template
<
    typename MA,
    typename MB,
    typename MP,
    typename MQ,
    typename Rank,
    typename T_det,
    typename T_factory,
    typename Ctrler,
    typename Norm
>
void smith
(
    const MA& A,
    MB& B,
    MP& P,
    MQ& Q,
    Rank& rank,
    T_det& det,
    const T_factory& tfctr,
    Ctrler ctrler,
    Norm norm
)
{
    typedef typename MB::element_type T_B;

    det = tfctr.unit(det);
    std::size_t m = A.nrows();
    std::size_t n = A.ncols();
    Q.assign_eye(n);
    P.assign_eye(m);
    B = A;

    ctrler.preamble();
    ctrler.current_matrices(Q, B, P);

    for(std::size_t corner = 0; corner < m && corner < n; corner++)
    {
        std::size_t new_i, new_j, i, j;
        T_B pivot_item;

        if(!_Internal::find_pivot_item(B, corner, new_i, new_j))
            // the smallest (in absolute value) non-zero entry in the matrix
            break; // all others entries are 0

        ctrler.find_smallest_nonzero(new_i, new_j);

        for(;;)
        {
            if(ctrler.stop(corner, new_i, new_j, Q, B, P))
            {
                ctrler.conclusion();
                return;
            }

            do
            {
                i = new_i;
                j = new_j;
                pivot_item = B(i, j);

                ctrler.pivot_item(i, j);

                for(std::size_t jj = 0; jj < n; jj++)
                    if(jj != j)
                    {
                        T_B p = B(i, jj) / pivot_item;
                        B.addmult_cols(jj, j, -p);
                        Q.addmult_cols(jj, j, -p);
                    }

                for(std::size_t ii = 0; ii < m; ii++)
                    if(ii != i)
                    {
                        T_B p = B(ii, j) / pivot_item;
                        B.addmult_rows(ii, i, -p);
                        P.addmult_rows(ii, i, -p);
                    }


                ctrler.after_pivoting();
                ctrler.current_matrices(Q, B, P);

                new_j = _Internal::find_pivot_item_in_row(B, corner, i);

                if(new_j == j)
                    new_i = _Internal::find_pivot_item_in_col(B, corner, j);
                else
                    new_i = i;
            }while (i != new_i || j != new_j);
            // while pivot entry is changing,
            // i.e. until others entries in row i and column j are 0

            std::size_t k, l;
            if(!_Internal::find_non_divisor_of_item(B, corner, pivot_item, k, l))
                break;

            B.add_rows(i, k);
            P.add_rows(i, k);

            ctrler.nondivisor_entry(k, i, l);
            ctrler.current_matrices(Q, B, P);
        }

        bool if_alter = false;

        if(i != corner)
        {
            B.swap_rows(i, corner);
            P.swap_rows(i, corner);
            opposite(&det);
            if_alter = true;
        }

        if(j != corner)
        {
            B.swap_cols(j, corner);
            Q.swap_cols(j, corner);
            opposite(&det);
            if_alter = true;
        }

        if(is_negative(B(corner, corner)))
        {
            B.mult_row(corner, -1);
            P.mult_row(corner, -1);
            opposite(&det);
            if_alter = true;
        }

        if(if_alter)
        {
            ctrler.pivot_adjustment();
            ctrler.current_matrices(Q, B, P);
        }

    }

    rank = 0;

    for(std::size_t i = 0; i < m && i < n; i++)
        if(!is_null(B(i, i)))
        {
            det *= B(i, i);
            norm(B(i, i), P, i);
            rank++;
        }
        else
            break;

    ctrler.conclusion();
}


template <typename M>
M smith (const M& a)
{
    typedef typename M::element_type T;

    M b;
    matrix<T, false> p, q;
    T det;
    typename M::size_type rank;
    smith(a, b, p, q, rank, det);
    return b;
}


template
<
    typename MA,
    typename MB,
    typename MP,
    typename MQ,
    typename Rank,
    typename T_det
>
void smith_storjohann
(
    const MA &A,
    MB &B,
    MP &P,
    MQ &Q,
    Rank &rank,
    T_det &det
)
{
    typedef typename MA::element_type T_A;

    matrix<T_A, false> U, E, F;
    smith_storjohann(A, P, Q, U, E, F, rank, det);

    // WARNING! Hmmm... As far as I know the matrix multiplication is associative
    // and maybe better way is firstly compute P = E*U*P, Q = Q*F and then B = P*A*Q.
    B = E*(U*(P*A*Q))*F;
    P = E*U*P;
    Q = Q*F;
}


template
<
    typename MA,
    typename MB,
    typename MP,
    typename MQ,
    typename Rank,
    typename T_det
>
void smith_near_optimal
(
    const MA &A,
    MB &B,
    MP &P,
    MQ &Q,
    Rank &rank,
    T_det &det
)
{
    typedef typename MA::element_type T_A;

    matrix<T_A, false> U, V;
    smith_near_optimal(A, B, P, Q, U, V, rank, det);
    P = U*P;
    Q = Q*V;
}


}


#if ARAGELI_DEBUG_LEVEL > 3

#include "big_int.hpp"

namespace Arageli
{


template void smith_storjohann
(
    const matrix<big_int> &A,
    matrix<big_int> &B,
    matrix<big_int> &P,
    matrix<big_int>& Q,
    big_int &rank,
    big_int &det
);

template void smith_storjohann
(
    const matrix<int> &A,
    matrix<int> &B,
    matrix<int> &P,
    matrix<int>& Q,
    int &rank,
    int &det
);


template void smith_near_optimal
(
    const matrix<big_int> &A,
    matrix<big_int> &B,
    matrix<big_int> &P,
    matrix<big_int>& Q,
    big_int &rank,
    big_int &det
);

template void smith_near_optimal
(
    const matrix<int> &A,
    matrix<int> &B,
    matrix<int> &P,
    matrix<int>& Q,
    int &rank,
    int &det
);


template void smith
(
    const matrix<big_int> &A,
    matrix<big_int> &B,
    matrix<big_int> &P,
    matrix<big_int>& Q,
    big_int &rank,
    big_int &det
);

template void smith
(
    const matrix<int> &A,
    matrix<int> &B,
    matrix<int> &P,
    matrix<int>& Q,
    int &rank,
    int &det
);


}

#endif


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
