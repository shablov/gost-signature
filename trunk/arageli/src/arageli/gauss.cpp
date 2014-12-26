/*****************************************************************************

    gauss.cpp

    This file is part of Arageli library.

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
    \file gauss.cpp
    \brief The gauss.hpp file stuff implementation.
*/

#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_GAUSS)

#include <cstddef>
#include <cmath>
#include <limits>

#include "gauss.hpp"
#include "cmp.hpp"
#include "misc.hpp"
#include "submatrix/hpair.hpp"
//#include "rational.hpp"    // compilation fails on GCC 3.4.3 without this

namespace Arageli
{


template <typename M>
void gauss_field_row_iter
(
    M& a,
    typename M::size_type row,
    typename M::size_type col
)
{
    ARAGELI_ASSERT_0(row < a.nrows());
    ARAGELI_ASSERT_0(col < a.ncols());

    a.div_row(row, safe_reference(a(row, col)));

    typedef typename M::size_type size_type;

    for(size_type i = 0; i < row; ++i)
        a.addmult_rows(i, row, -a(i, col));
    for(size_type i = row + 1; i < a.nrows(); ++i)
        a.addmult_rows(i, row, -a(i, col));
}


template <typename M>
void gauss_field_col_iter
(
    M& a,
    typename M::size_type row,
    typename M::size_type col
)
{
    ARAGELI_ASSERT_0(row < a.nrows());
    ARAGELI_ASSERT_0(col < a.ncols());

    a.div_col(col, safe_reference(a(row, col)));

    typedef typename M::size_type size_type;

    for(size_type j = 0; j < col; ++j)
        a.addmult_cols(j, col, -a(row, j));
    for(size_type j = col + 1; j < a.ncols(); ++j)
        a.addmult_cols(j, col, -a(row, j));
}


template <typename M, typename T>
void gauss_bareiss_row_iter
(
    M& a,
    typename M::size_type row,
    typename M::size_type col,
    T& det, T& det_denom
)
{
    // TODO: Make similar function for column manipulation.

    ARAGELI_ASSERT_0(row < a.nrows());
    ARAGELI_ASSERT_0(col < a.ncols());

    typedef typename M::element_type TM;
    typedef typename M::size_type size_type;

    TM pivot = a(row, col);

    if(is_negative(pivot))
    {
        a.mult_row(row, opposite_unit<TM>());
        opposite(&pivot);
        opposite(&det);
    }

    for(size_type i = 0; i < a.nrows(); i++)
        if(i != row)
        {
            TM alpha = a(i, col);
            TM delta = gcd(pivot, alpha);
            TM bi = -alpha / delta;
            TM brow = pivot / delta;
            a.mult_row(i, brow);
            a.addmult_rows(i, row, bi);
            alpha = gcd(a.copy_row(i));
            a.div_row(i, alpha);

            det *= alpha;
            det_denom *= brow;
        }
}


template
<
    typename A,
    typename B,
    typename Q,
    typename Basis,
    typename T,
    typename T_factory,
    typename Ctrler    // control visitor
>
void rref_gauss_field
(
    const A& a,
    B& b,
    Q& q,
    Basis& basis,
    T& det,
    const T_factory& tfctr,
    Ctrler ctrler
)
{
    ARAGELI_ASSERT_0(!a.is_empty());

    typedef typename B::element_type TB;
    typedef typename B::size_type size_type;

    det = tfctr.unit(det);
    size_type m = a.nrows(); size_type n = a.ncols();
    q.assign_eye(m);
    b = a;
    basis.resize(0);
    TB bnull = null(b(0, 0));    // needed for main loop of the algorithm
    hpair_matrix<B, Q> bq(b, q);

    ctrler.preamble(a);

    for(size_type i = 0, j = 0; i < m && j < n; j++)    // (i, j) -- pivot element
    {
        ctrler.before_iter(b, q, det, basis);

        // Elimination of column j of matrix b.

        // Firstly find the biggest (in absolute value) entry in the column j.

        ctrler.find_biggest_in_col(j);
        TB max = bnull;
        size_type i_max = i;

        for(size_type ii = i; ii < m; ii++)
        {
            TB absval = abs(b(ii, j));
            if(absval > max)
            {
                max = absval;
                i_max = ii;
            }
        }

        // If there has no non-zero element found, the column j is negligible,
        // it contains only zeros.

        if(is_null(max))
        {
            ctrler.negligible_col(j);
            det = tfctr.null(det);    // WARNING!
            ctrler.after_iter(b, q, det, basis);
            continue;    // next column
        }

        // Now the pivot item is (i_max, j).

        ctrler.pivot_item(i_max, j);

        // If it is not on the diagonal, swap the rows to do it so.

        if(i != i_max)
        {
            bq.swap_rows(i, i_max);
            opposite(&det);    // WARNING!
            ctrler.swap_rows(i, i_max, b, q, det);
        }

        // Now (i, j) is a pivot element.
        // Next eliminate column j.

        ctrler.eliminate_col(j);
        det *= b(i, j);
        gauss_field_row_iter(bq, i, j);
        ctrler.after_elimination(b, q, det);
        basis.push_back(j);
        i++;
        ctrler.after_iter(b, q, det, basis);
    } // end of main loop

    ctrler.conclusion(b, q, det, basis);
}


template <typename A1, typename A2, typename I>
void mat_col_copy_order
(const A1& a1, A2& a2, const I& idx)
{
    A1 temp = a1;
    temp.take_cols(idx, a2);

    for(typename A1::size_type i = 0; i < temp.ncols(); ++i)
        a2.insert_col(a2.ncols(), temp.copy_col(i));
}


template <typename A1, typename A2, typename I>
void vec_copy_order
(const A1& a1, A2& a2, const I& idx)
{
    A1 temp = a1;
    temp.take_subvector(idx, a2);

    for(typename A1::size_type i = 0; i < temp.size(); ++i)
        a2.push_back(temp[i]);
}

template <typename A, typename B>
void vec_inverse_permutation (const A& a, B& b)
{
    b.resize(a.size());
    typedef typename A::size_type size_type;
    for(size_type i = 0; i < a.size(); ++i)
    {
        ARAGELI_ASSERT_0(size_type(a[i]) < a.size());
        size_type ii = std::find(a.begin(), a.end(), i) - a.begin();
        ARAGELI_ASSERT_0(ii < a.size());
        b[i] = ii;
    }
}


template
<
    typename A,
    typename B,
    typename Q,
    typename Basis_in,
    typename Basis_out,
    typename Det
>
void rref_order
(
    const A& a,
    B& b,
    Q& q,
    const Basis_in& basis_in,
    Basis_out& basis_out,
    Det& det
)
{
    A aa;
    Basis_in order = basis_in;
    for(std::size_t i = 0; i < a.ncols(); ++i)
        if(std::find(basis_in.begin(), basis_in.end(), i) == basis_in.end())
            order.push_back(i);

    mat_col_copy_order(a, aa, order);
    //std::cout << "\norder = " << order;

    Q qq = q;
    B bb;
    rref(aa, bb, qq, basis_out, det);

    Basis_in invorder;
    vec_inverse_permutation(order, invorder);
    //std::cout << "\ninvorder = " << invorder;

    mat_col_copy_order(bb, b, invorder);
    //mat_col_copy_order(qq, q, invorder);
    // TODO Something with basis_out
}


template <typename T, bool REFCNT>
matrix<T, REFCNT> inverse (const matrix<T, REFCNT>& A)
{
    // precondition: A must be square
    //if(!A.is_square())throw exception("matrix must be square");
    ARAGELI_ASSERT_0(A.is_square());

    matrix<T, false> B;
    matrix<T, true> Q;
    vector<std::size_t, false> basis;
    T det;

    rref(A, B, Q, basis, det);

    if(is_null(det))
        throw matrix_is_singular();

    ARAGELI_ASSERT_1
    (
        !std::numeric_limits<T>::is_specialized ||
        !std::numeric_limits<T>::is_exact ||
        is_unit(A*Q)
    );

    return Q;
}


template <typename T, bool REFCNT>
T det (const matrix<T, REFCNT>& A)
{
    // precondition: A must be square
    //if(!A.is_square())throw exception("matrix must be square");
    ARAGELI_ASSERT_0(A.is_square());

    matrix<T, false> B;
    matrix<T, false> Q;
    vector<std::size_t, false> basis;
    T det;

    rref(A, B, Q, basis, det/*, ctrl::make_rref_slog(std::cout)*/);

    return basis.size() < A.nrows() ? null(det) : det;
}


template <typename T, bool REFCNT>
typename matrix<T, REFCNT>::size_type rank
(const matrix<T, REFCNT>& A)
{
    matrix<T, false> B;
    matrix<T, false> Q;
    vector<std::size_t, false> basis;
    T det;

    rref(A, B, Q, basis, det);

    return basis.size();
}


template
<
    typename A,
    typename B,
    typename Q,
    typename Basis,
    typename T,
    typename T_factory,
    typename Ctrler    // control visitor
>
void rref_gauss_bareiss
(
    const A& a,
    B& b,
    Q& q,
    Basis& basis,
    T& det,
    const T_factory& tfctr,
    Ctrler ctrler
)
{
    typedef typename B::element_type TB;
    typedef typename B::size_type size_type;

    det = tfctr.unit(det);
    T det_denom = tfctr.unit(det); // differs from the field version
    size_type m = a.nrows();
    size_type n = a.ncols();
    q.assign_eye(m);
    b = a;
    basis.resize(0);
    TB bnull = null(b(0, 0));    // needed for main loop of the algorithm
    hpair_matrix<B, Q> bq(b, q);

    ctrler.preamble(a);

    for(size_type i = 0, j = 0; i < m && j < n; j++)
    {
        ctrler.before_iter(b, q, det, basis, det_denom);

        // Elimination of column j of matrix b.

        // Firstly find the biggest (in absolute value) entry in the column j.

        ctrler.find_biggest_in_col(j);
        TB max = bnull;
        size_type i_max = i;

        for(size_type ii = i; ii < m; ii++)
        {
            TB absval = abs(b(ii, j));
            if(absval > max)
            {
                max = absval;
                i_max = ii;
            }
        }

        if(is_null(max))
        {
            // column j is negligible (only zeros)
            ctrler.negligible_col(j);
            det = tfctr.null(det);    // WARNING!
            ctrler.after_iter(b, q, det, basis, det_denom);
            continue;    // next column
        }

        ctrler.pivot_item(i_max, j);

        if(i != i_max)
        {
            bq.swap_rows(i, i_max);
            opposite(&det);    // WARNING!

            ctrler.swap_rows(i, i_max, b, q, det, det_denom);
        }

        ctrler.eliminate_col(j);
        gauss_bareiss_row_iter(bq, i, j, det, det_denom);
        ctrler.after_elimination(b, q, det, det_denom);
        basis.push_back(j);
        i++;
        ctrler.after_iter(b, q, det, basis, det_denom);
    } // end of main loop

    for(size_type i = 0; i < basis.size(); i++)
        det *= b(i, basis[i]);
    det /= det_denom;

    ctrler.conclusion(b, q, det, basis);
}


template <typename T, bool REFCNT>
T det_int (const matrix<T, REFCNT>& A)
{
    // precondition: A must be square
    //if(!A.is_square())throw exception("matrix must be square");
    ARAGELI_ASSERT_0(A.is_square());

    matrix<T, false> B;
    matrix<T, false> Q;
    vector<std::size_t, false> basis;
    T d;

    rref_int(A, B, Q, basis, d);

    return basis.size() < A.nrows() ? null(d) : d;
}


template <typename T, bool REFCNT>
typename matrix<T, REFCNT>::size_type rank_int
(const matrix<T, REFCNT>& A)
{
    matrix<T, false> B;
    matrix<T, false> Q;
    vector<std::size_t, false> basis;
    T det;

    rref_int(A, B, Q, basis, det);

    return basis.size();
}



} // namespace Arageli


#endif // #ifndef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
