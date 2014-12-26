/*****************************************************************************

    gauss.hpp

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

#ifndef _ARAGELI_GAUSS_HPP_
#define _ARAGELI_GAUSS_HPP_


/**
    \file gauss.hpp
    \brief Gauss algorithm and related.

    Gauss interation (row, col), The Gauss algorithm, Gauss-Bareiss algorithm
    and derived.
*/


#include "config.hpp"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <functional>

#include "type_opers.hpp"
#include "type_traits.hpp"
#include "factory.hpp"
#include "exception.hpp"
#include "powerest.hpp"
#include "vector.hpp"
#include "matrix.hpp"

#include "std_import.hpp"

namespace Arageli
{


/// Gauss elimination of column col with row manipulation.
/** Eliminates nonzeros in column col exclude the element (row, col)
    by row manipulation. Pivot element (row, col) becomes 1. */
template <typename M>
void gauss_field_row_iter
(
    M& a,
    typename M::size_type row,
    typename M::size_type col
);


/// Gauss elimination of row with column manipulation.
/** Eliminates nonzeros in row exclude the element (row, col)
    by column manipulation. Pivot element (row, col) becomes 1. */
template <typename M>
void gauss_field_col_iter
(
    M& a,
    typename M::size_type row,
    typename M::size_type col
);


/// Gauss-Bareiss elimination of column col with row manipulation.
/** Eliminates nonzeros in column col exclude the element (row, col)
    by row manipulation. */
template <typename M, typename T>
void gauss_bareiss_row_iter
(
    M& a,
    typename M::size_type row,
    typename M::size_type col,
    T& det, T& det_denom
);


/// Gauss-Bareiss elimination of row with column manipulation.
/** Eliminates nonzeros in row exclude the element (row, col)
    by column manipulation. */
template <typename M, typename T>
void gauss_bareiss_col_iter
(
    M& a,
    typename M::size_type row,
    typename M::size_type col,
    T& det, T& det_denom
);


/// Gauss-Bareiss elimination of column col with row manipulation.
/** Eliminates nonzeros in column col exclude the element (row, col)
    by row manipulation. */
template <typename M>
void gauss_bareiss_row_iter
(
    M& a,
    typename M::size_type row,
    typename M::size_type col
)
{
    typename M::element_type det, det_denom;
    gauss_bareiss_row_iter(a, row, col, det, det_denom);
}


/// Gauss-Bareiss elimination of row with column manipulation.
/** Eliminates nonzeros in row exclude the element (row, col)
    by column manipulation. */
template <typename M>
void gauss_bareiss_col_iter
(
    M& a,
    typename M::size_type row,
    typename M::size_type col
)
{
    typename M::element_type det, det_denom;
    gauss_bareiss_col_iter(a, row, col, det, det_denom);
}


namespace ctrl
{

/// Idler controller for Arageli::rref_gauss_field function.
/** The names of parameters of the members are the same as for
    the ones of controlled function itself. */
struct rref_gauss_field_idler
{
    class abort :
        public ctrl::abort
    {};

    /// Called in the very beginning.
    template <typename A>
    void preamble (const A&) const
    {}

    /// Called in the very end.
    /** The arguments have the resulted values. */
    template <typename B, typename Q, typename Det, typename Basis>
    void conclusion (const B&, const Q&, const Det&, const Basis&) const
    {}

    /// Called before an iteration.
    /** This function is called before choosing a pivot item in the current
        column. We consider only one column on each iteration. */
    template <typename B, typename Q, typename Det, typename Basis>
    void before_iter (const B&, const Q&, const Det&, const Basis&) const
    {}

    /// Called just after an iteration.
    /** This function is called after each iteration has been completed.
        Actually there is no difference in the arguments between after_iter
        call and before_iter call inside the algorithm because between
        the iterations the algorithm does really nothing. */
    template <typename B, typename Q, typename Det, typename Basis>
    void after_iter (const B&, const Q&, const Det&, const Basis&) const
    {}

    /// Called just before finding the biggest element in the column j.
    template <typename J>
    void find_biggest_in_col (const J& j) const
    {}

    /// Called after finding the biggest element if there are no non-zero items.
    /** This function is called after the algorithm make sure that in column j
        there are no non-zero etries, i.e. this column is negligible. */
    template <typename J>
    void negligible_col (const J& j) const
    {}

    /// Called after finding the biggest element if one has been found.
    /** The chosen item is (i, j), it becomes a pivot item. */
    template <typename I, typename J>
    void pivot_item (const I& i, const J& j) const
    {}

    /// Called after swapping rows due to the pivot item isn't on the diagonal.
    /** This function is called just after swapping the rows i1 and i2 to
        obtain the pivot item on the diagonal. Before the call the current det
        value is changed appropriately. */
    template <typename I1, typename I2, typename B, typename Q, typename Det>
    void swap_rows
    (
        const I1& i1, const I2& i2,
        const B&, const Q&, const Det&
    ) const
    {}

    /// Called just before elimination of column j.
    /** Between this call and a call of after_elimination function,
        the algorithm changes appropriately det and eliminates the
        columnt j. */
    template <typename J>
    void eliminate_col (const J& j) const
    {}

    /// Called just after elimination of the column.
    template <typename B, typename Q, typename Det>
    void after_elimination (const B&, const Q&, const Det&) const
    {}
};


/// Idler controller for Arageli::rref_gauss_bareiss function.
/** The names of parameters of the members are the same as for
    the ones of controlled function itself. Det_denom is a type
    for representing a denominator of the current fractional value
    of the determinant.

    This class is inherited from rref_gauss_field_idler because
    some functions are the same for both these controllers.
    But not all of them. There is one additional argument for some of
    them --- det_denom.    Ideologically, these functions have the same
    goal as their parents and have the same names. */
struct rref_gauss_bareiss_idler : public rref_gauss_field_idler
{
    class abort :
        public ctrl::abort
    {};

    /// See rref_gauss_field_idler::before_iter.
    template
    <
        typename B,
        typename Q,
        typename Det,
        typename Basis,
        typename Det_denom
    >
    void before_iter
    (
        const B&,
        const Q&,
        const Det&,
        const Basis&,
        const Det_denom&
    ) const
    {}

    /// See rref_gauss_field_idler::after_iter.
    template
    <
        typename B,
        typename Q,
        typename Det,
        typename Basis,
        typename Det_denom
    >
    void after_iter
    (
        const B&,
        const Q&,
        const Det&,
        const Basis&,
        const Det_denom&
    ) const
    {}

    /// See rref_gauss_field_idler::swap_rows.
    template
    <
        typename I1,
        typename I2,
        typename B,
        typename Q,
        typename Det,
        typename Det_denom
    >
    void swap_rows
    (
        const I1&,
        const I2&,
        const B&,
        const Q&,
        const Det&,
        const Det_denom&
    ) const
    {}

    /// See rref_gauss_field_idler::after_elimination.
    template <typename B, typename Q, typename Det, typename Det_denom>
    void after_elimination
    (
        const B&,
        const Q&,
        const Det&,
        const Det_denom&
    ) const
    {}
};


/// Idler controller for Arageli::rref_field function.
/** As the rref_field function just calls another controlled
    function, this controller has only the method to obtain
    appropriate controller for that. */
struct rref_field_idler
{
    class abort :
        public ctrl::abort
    {};

    /// Retruns a controller object for the rref_gauss_field function.
    rref_gauss_field_idler ctrl_rref_gauss_field () const
    {
        return rref_gauss_field_idler();
    }
};


/// Idler controller for Arageli::rref_int function.
/** As the rref_int function just calls another controlled
    function, this controller has only the method to obtain
    appropriate controller for that. */
struct rref_int_idler
{
    class abort :
        public ctrl::abort
    {};

    /// Retruns a controller object for the rref_gauss_bareiss function.
    rref_gauss_bareiss_idler ctrl_rref_gauss_bareiss () const
    {
        return rref_gauss_bareiss_idler();
    }
};


/// Idler controller for Arageli::rref function.
/** As the rref function just calls other controlled
    functions, this controller have only the methods to obtain
    appropriate controllers for that. */
struct rref_idler
{
    class abort :
        public ctrl::abort
    {};

    /// Retruns a controller object for the rref_field function.
    rref_field_idler ctrl_rref_field () const
    {
        return rref_field_idler();
    }

    /// Retruns a controller object for the rref_int function.
    rref_int_idler ctrl_rref_int () const
    {
        return rref_int_idler();
    }
};


} // namespace ctrl


/// Produces the reduced row echelon form `b' of the matrix `a' over a field.
/** Elementary row operations are performed on `a', a matrix over
    a field, to reduce it to the reduced row echelon (Gauss-Jordan) form.
    Returns b, q, basis, det:
    - q is such that b = q * a;
    - r = basis.size() is the rank of A;
    - B.sumbatrix(0..(r-1), basis) is the r-by-r identity matrix; (?)
    - det is the value of the max size left upper corner minor of A;
        in general, it isn't a determinant of the matrix.

    @param ctrler the controller; must has an interface of
        ctrl::rref_gauss_field_idler class. */
template
<
    typename A,
    typename B,
    typename Q,
    typename Basis,
    typename T,
    typename T_factory,
    typename Ctrler
>
void rref_gauss_field
(
    const A& a, B& b, Q& q, Basis& basis,
    T& det,
    const T_factory& tfctr,
    Ctrler ctrler
);


/// Produces the reduced row echelon form B of a matrix A.
template
<
    typename A,
    typename B,
    typename Q,
    typename Basis,
    typename T,
    typename Ctrler    // control visitor
>
inline void rref_gauss_field
(
    const A& a,
    B& b,
    Q& q,
    Basis& basis,
    T& det,
    Ctrler ctrler
)
{
    rref_gauss_field(a, b, q, basis, det, factory<T>(), ctrler);
}


/// Produces the reduced row echelon form B of a matrix A.
template <typename A, typename B, typename Q, typename Basis, typename T>
inline void rref_gauss_field (const A& a, B& b, Q& q, Basis& basis, T& det)
{
    rref_gauss_field
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_gauss_field_idler()
    );
}


/// Produces the reduced row echelon form B of a matrix A.
template <typename A, typename B, typename Q, typename Basis>
inline void rref_gauss_field (const A& a, B& b, Q& q, Basis& basis)
{
    typedef typename B::element_type T;
    T det;
    rref_gauss_field
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_gauss_field_idler()
    );
}


/// Produces the reduced row echelon form B of a matrix A.
template <typename A, typename B, typename Q>
inline void rref_gauss_field (const A& a, B& b, Q& q)
{
    typedef typename B::element_type T;
    T det;
    vector<typename B::size_type, false> basis;
    rref_gauss_field
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_gauss_field_idler()
    );
}


/// Produces the reduced row echelon form B of a matrix A.
template <typename A, typename B>
inline void rref_gauss_field (const A& a, B& b)
{
    typedef typename B::element_type T;
    T det;
    vector<typename B::size_type, false> basis;
    B q;
    rref_gauss_field
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_gauss_field_idler()
    );
}


/// Produces the reduced row echelon form B of a matrix A.
template <typename A>
inline A rref_gauss_field (const A& a)
{
    typedef typename A::element_type T;
    T det;
    vector<typename A::size_type, false> basis;
    A b, q;
    rref_gauss_field
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_gauss_field_idler()
    );
    return b;
}

// ----------------------------------------

/// Produces the reduced row echelon form `b' of the matrix `a' over a field.
/** It's the same that rref_gauss_field funtion.

    @param ctrler the controller; must has an interface of
        ctrl::rref_field_idler class. */
template
<
    typename A, typename B, typename Q, typename Basis,
    typename T, typename T_factory,
    typename Ctrler    // control visitor
>
inline void rref_field
(
    const A& a, B& b, Q& q, Basis& basis,
    T& det,
    const T_factory& tfctr,
    Ctrler ctrler
)
{
    rref_gauss_field
    (
        a,
        b,
        q,
        basis,
        det,
        tfctr,
        ctrler.ctrl_rref_gauss_field()
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a' over a field.
template
<
    typename A, typename B, typename Q, typename Basis, typename T,
    typename Ctrler    // control visitor
>
inline void rref_field
(
    const A& a,
    B& b,
    Q& q,
    Basis& basis,
    T& det,
    Ctrler ctrler
)
{
    rref_field
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrler
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a' over a field.
template <typename A, typename B, typename Q, typename Basis, typename T>
inline void rref_field (const A& a, B& b, Q& q, Basis& basis, T& det)
{
    rref_field
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_field_idler()
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a' over a field.
template <typename A, typename B, typename Q, typename Basis>
inline void rref_field (const A& a, B& b, Q& q, Basis& basis)
{
    typedef typename B::element_type T;
    T det;
    rref_field
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_field_idler()
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a' over a field.
template <typename A, typename B, typename Q>
inline void rref_field (const A& a, B& b, Q& q)
{
    typedef typename B::element_type T;
    T det;
    vector<typename B::size_type, false> basis;
    rref_field
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_field_idler()
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a' over a field.
template <typename A, typename B>
inline void rref_field (const A& a, B& b)
{
    typedef typename B::element_type T;
    T det;
    vector<typename B::size_type, false> basis;
    B q;
    rref_field
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_field_idler()
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a' over a field.
template <typename A>
inline A rref_field (const A& a)
{
    typedef typename A::element_type T;
    T det;
    vector<typename A::size_type> basis;
    A b, q;
    rref_field
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_field_idler()
    );
    return b;
}


// ----------------------------------------

/// Copies columns from a1 to a2 in order indexes in idx then tails the rest.
template <typename A1, typename A2, typename I>
void mat_col_copy_order (const A1& a1, A2& a2, const I& idx);

/// Copies elements from a1 to a2 in order indexes in idx then tails the rest.
template <typename A1, typename A2, typename I>
void vec_copy_order (const A1& a1, A2& a2, const I& idx);


/// Inverses permutation vector.
template <typename A, typename B>
void vec_inverse_permutation (const A& a, B& b);


/// Produces the reduced row echelon from B of the matrix A with a specific column order.
/** Makes RREF(A) with permutated columns according to basis_in.
    basis_in contains indexes of columns, basis_out indicates additional permutaitaon
    of columns. */
// TODO Make this function with an algorithm controller.
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
);


/// Matrix inversion.
/**
    Returns inversion of matrix A
    Requirement: A must be square.
*/
template <typename T, bool REFCNT>
matrix<T, REFCNT> inverse (const matrix<T, REFCNT>& A);


/// Returns determinant of A.
/**
    Returns determinant of A.
    Precondition: A must be square
*/
template <typename T, bool REFCNT>
T det (const matrix<T, REFCNT>& A);


/// Returns a rank of a matrix.
template <typename T, bool REFCNT>
typename matrix<T, REFCNT>::size_type rank (const matrix<T, REFCNT>& A);


/// Produces the reduced row echelon form `b' of the matrix `a' over an integer domain.
/**
    Produces the reduced row echelon form B of the integer matrix a
    by Gauss-Bareiss algorithm.
    Returns b, q, basis, det:
    - q is such that b = q * a;
    - r = basis.size() is the rank of A;
    - B.sumbatrix(mesh_grid(1,r), basis) is the r-by-r non-singular
    diagonal matrix;    (?)
    - det is a determinant of a.    (?)

    @param ctrler the controller; must has an interface of
        ctrl::rref_gauss_bareiss_idler class. */
template
<
    typename A,
    typename B,
    typename Q,
    typename Basis,
    typename T,
    typename T_factory,
    typename Ctrler
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
);


/// Produces the reduced row echelon form `b' of the matrix `a' over an integer domain.
template
<
    typename A,
    typename B,
    typename Q,
    typename Basis,
    typename T,
    typename Ctrler
>
inline void rref_gauss_bareiss
(
    const A& a,
    B& b,
    Q& q,
    Basis& basis,
    T& det,
    Ctrler ctrler
)
{
    rref_gauss_bareiss(a, b, q, basis, det, factory<T>(), ctrler);
}


/// Produces the reduced row echelon form `b' of the matrix `a' over an integer domain.
template <typename A, typename B, typename Q, typename Basis, typename T>
inline void rref_gauss_bareiss (const A& a, B& b, Q& q, Basis& basis, T& det)
{
    rref_gauss_bareiss
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_gauss_bareiss_idler()
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a' over an integer domain.
template <typename A, typename B, typename Q, typename Basis>
inline void rref_gauss_bareiss (const A& a, B& b, Q& q, Basis& basis)
{
    typedef typename B::element_type T;
    T det;
    rref_gauss_bareiss
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_gauss_bareiss_idler()
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a' over an integer domain.
template <typename A, typename B, typename Q>
inline void rref_gauss_bareiss (const A& a, B& b, Q& q)
{
    typedef typename B::element_type T;
    T det;
    vector<typename B::size_type, false> basis;
    rref_gauss_bareiss
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_gauss_bareiss_idler()
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a' over an integer domain.
template <typename A, typename B>
inline void rref_gauss_bareiss (const A& a, B& b)
{
    typedef typename B::element_type T;
    T det;
    vector<typename B::size_type, false> basis;
    B q;
    rref_gauss_bareiss
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_gauss_bareiss_idler()
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a' over an integer domain.
template <typename A>
inline A rref_gauss_bareiss (const A& a)
{
    typedef typename A::element_type T;
    T det;
    vector<typename A::size_type, false> basis;
    A b, q;
    rref_gauss_bareiss
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_gauss_bareiss_idler()
    );
    return b;
}


// ----------------------------------------

/// Produces the reduced row echelon form `b' of the matrix `a' over an integer domain.
/** It's the same that rref_gauss_bareiss function.

    @param ctrler the controller; must has an interface of
        ctrl::rref_int_idler class. */
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
inline void rref_int
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
    rref_gauss_bareiss
    (
        a,
        b,
        q,
        basis,
        det,
        tfctr,
        ctrler.ctrl_rref_gauss_bareiss()
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a' over an integer domain.
template
<
    typename A,
    typename B,
    typename Q,
    typename Basis,
    typename T,
    typename Ctrler    // control visitor
>
inline void rref_int
(
    const A& a,
    B& b,
    Q& q,
    Basis& basis,
    T& det,
    Ctrler ctrler
)
{
    rref_int(a, b, q, basis, det, factory<T>(), ctrler);
}


/// Produces the reduced row echelon form `b' of the matrix `a' over an integer domain.
template <typename A, typename B, typename Q, typename Basis, typename T>
inline void rref_int (const A& a, B& b, Q& q, Basis& basis, T& det)
{
    rref_int
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(), ctrl::rref_int_idler()
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a' over an integer domain.
template <typename A, typename B, typename Q, typename Basis>
inline void rref_int (const A& a, B& b, Q& q, Basis& basis)
{
    typedef typename B::element_type T;
    T det;
    rref_int
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_int_idler()
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a' over an integer domain.
template <typename A, typename B, typename Q>
inline void rref_int (const A& a, B& b, Q& q)
{
    typedef typename B::element_type T;
    T det;
    vector<typename B::size_type, false> basis;
    rref_int
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_int_idler()
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a' over an integer domain.
template <typename A, typename B>
inline void rref_int (const A& a, B& b)
{
    typedef typename B::element_type T;
    T det;
    vector<typename B::size_type, false> basis;
    B q;
    rref_int
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_int_idler()
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a' over an integer domain.
template <typename A>
inline A rref_int (const A& a)
{
    typedef typename A::element_type T;
    T det;
    vector<typename A::size_type, false> basis;
    A b, q;
    rref_int
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_int_idler()
    );
    return b;
}


// ----------------------------------------

namespace _Internal
{

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
inline void rref_helper_1
(
    const A& a,
    B& b,
    Q& q,
    Basis& basis,
    T& det,
    const T_factory& tfctr,
    Ctrler ctrler,
    const true_type&
)
{
    rref_field
    (
        a,
        b,
        q,
        basis,
        det,
        tfctr,
        ctrler.ctrl_rref_field()
    );
}


template
<
    typename A,
    typename B,
    typename Q,
    typename Basis,
    typename T,
    typename T_factory,
    typename Ctrler
>
inline void rref_helper_1
(
    const A& a,
    B& b,
    Q& q,
    Basis& basis,
    T& det,
    const T_factory& tfctr,
    Ctrler ctrler,
    const false_type&
)
{
    rref_int
    (
        a,
        b,
        q,
        basis,
        det,
        tfctr,
        ctrler.ctrl_rref_int()
    );
}


}


/// Produces the reduced row echelon form `b' of the matrix `a'.
/** Exact meaning of RREF depends on type of result (integer domain or field);
    see rref_field and rref_int respectively.

    @param ctrler the controller; must has an interface of
        ctrl::rref_idler class. */
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
inline void rref
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
    _Internal::rref_helper_1
    (
        a,
        b,
        q,
        basis,
        det,
        tfctr,
        ctrler,
        bool_type<type_traits<typename B::element_type>::is_field>::value
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a'.
template
<
    typename A,
    typename B,
    typename Q,
    typename Basis,
    typename T,
    typename Ctrler    // control visitor
>
inline void rref
(
    const A& a,
    B& b,
    Q& q,
    Basis& basis,
    T& det,
    Ctrler ctrler
)
{
    rref(a, b, q, basis, det, factory<T>(), ctrler);
}


/// Produces the reduced row echelon form `b' of the matrix `a'.
template <typename A, typename B, typename Q, typename Basis, typename T>
inline void rref (const A& a, B& b, Q& q, Basis& basis, T& det)
{
    rref
    (
        a,
        b,
        q,
        basis,
        det,
        factory<T>(),
        ctrl::rref_idler()
    );
}


/// Produces the reduced row echelon form `b' of the matrix `a'.
template <typename A, typename B, typename Q, typename Basis>
void rref (const A& a, B& b, Q& q, Basis& basis)
{
    typedef typename B::element_type T;
    T det;
    rref(a, b, q, basis, det, factory<T>(), ctrl::rref_idler());
}


/// Produces the reduced row echelon form `b' of the matrix `a'.
template <typename A, typename B, typename Q>
inline void rref (const A& a, B& b, Q& q)
{
    typedef typename B::element_type T;
    T det;
    vector<typename B::size_type, false> basis;
    rref(a, b, q, basis, det, factory<T>(), ctrl::rref_idler());
}


/// Produces the reduced row echelon form `b' of the matrix `a'.
template <typename A, typename B>
inline void rref (const A& a, B& b)
{
    typedef typename B::element_type T;
    T det;
    vector<typename B::size_type, false> basis;
    B q;
    rref(a, b, q, basis, det, factory<T>(), ctrl::rref_idler());
}


/// Produces the reduced row echelon form `b' of the matrix `a'.
template <typename A>
inline A rref (const A& a)
{
    typedef typename A::element_type T;
    T det;
    vector<typename A::size_type, false> basis;
    A b, q;
    rref(a, b, q, basis, det, factory<T>(), ctrl::rref_idler());
    return b;
}


///    Returns determinant of integer matrix A.
template <typename T, bool REFCNT>
T det_int (const matrix<T, REFCNT>& A);


/// Returns a rank of integer matrix.
template <typename T, bool REFCNT>
typename matrix<T, REFCNT>::size_type rank_int (const matrix<T, REFCNT>& A);

/*
// The following solve_linsys function have migrated to solve_linsys.{hpp, cpp} files.
/// Solves a linear system.
template
<
    typename T_A,
    bool REFCNT_A,
    typename T_B,
    bool REFCNT_B,
    typename T_res,
    bool REFCNT_res
>
inline void solve_linsys
(
    const matrix<T_A, REFCNT_A>& A,
    const vector<T_B, REFCNT_B>& B,
    vector<T_res, REFCNT_res>& res
)
{
    // WARNING! Slow implementation.
    res = inverse(A)*B;
}


/// Solves a linear system.
template
<
    typename T_A,
    bool REFCNT_A,
    typename T_B,
    bool REFCNT_B
>
inline vector<T_B, REFCNT_B> solve_linsys
(
    const matrix<T_A, REFCNT_A>& A,
    const vector<T_B, REFCNT_B>& B
)
{
    vector<T_B, REFCNT_B> res(B.size());
    solve_linsys(A, B, res);
    return res;
}
*/

}


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_GAUSS
    #include "gauss.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_GAUSS
#endif


#endif
