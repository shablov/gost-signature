/*****************************************************************************

    bareiss.hpp

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
    \file bareiss.hpp
    \brief Gauss-Jordan and Gauss forms of an interger matrix.

    Gauss-Jordan and Gauss forms of an interger matrix. Rank and determinant
    calculation based on those functions.
*/


#ifndef _ARAGELI_bareiss_hpp_
#define _ARAGELI_bareiss_hpp_

#include "config.hpp"


namespace Arageli
{


/// Produces the Gauss-Jordan form B of an integer matrix A.
/**
    Returns B, Q, basis, det:
    - Q is such that B = Q * A;
    - r = basis.size() is a rank of A;
    - B.sumbatrix(mesh_grid(1,r), basis) is the r-by-r non-singular
    diagonal matrix;
    - det is the basis minor of A.
*/
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
    const MA& A,
    MB& B,
    MQ& Q,
    Basis& basis,
    T_det& det
);


/// Produces the Gauss form B of an integer matrix A.
/**
    Returns P, Q, rank, det:
    - P, Q permutation matrices: (P * A * Q).sumbatrix(mesh_grid(1,r), mesh_grid(1,r))
    is the r-by-r non-singular matrix;
    - rank is a rank of A;
    - det is the basis minor of A.
*/
template
<
    typename MA,
    typename MP,
    typename MQ,
    typename Rank,
    typename T_det
>
void bareiss_pq
(    const MA& A,
    MP& P,
    MQ& Q,
    Rank& rank,    // WARNING! Maybe just templated typename will be better.
    T_det& det
);


template <typename MA>
MA adjoint (const MA& A);


template
<
    typename MA,
    typename MP,
    typename T_det
>
void adjoint
(
    const MA& A,
    MP& P,
    T_det& det
);


template <typename MA>
typename MA::element_type det_brs (const MA& A);


template <typename MA>
typename MA::size_type rank_brs (const MA& A);


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_bareiss
    #include "bareiss.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_bareiss
#endif

#endif    // #ifndef _ARAGELI_bareiss_hpp_
