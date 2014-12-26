/*****************************************************************************

    smith/near_optimal.hpp -- the normal diagonal form of an integer matrix.

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
    This file contains the fucntion for producing the normal diagonal form
    of an integer matrix.
*/


#ifndef _ARAGELI_smith_near_optimal_hpp_
#define _ARAGELI_smith_near_optimal_hpp_

#include "../config.hpp"


namespace Arageli
{


///    Produces normal diagonal form S of integer matrix A.
/**
    Returns S, P, Q, U, V, rank, det:
    - P, Q permutation matrices: (P*A*Q).sumbatrix(mesh_grid(1,rank), mesh_grid(1,rank))
    is the rank-by-rank non-singular matrix;
    - U, V are unimodular principal left- and rigth transform such that S = U*P*A*Q*V;
    - rank is a rank of A;
    - det is the basis minor of A
*/
template
<
    typename MA,
    typename MS,
    typename MP,
    typename MQ,
    typename MU,
    typename MV,
    typename Rank,
    typename T_det
>
void smith_near_optimal
(
    const MA& A,
    MS& S,  MP& P,  MQ& Q,
    MU& U,  MV& V,
    Rank& rank, T_det& det
);


// Document it!
// WARNING! Maybe the name of this function should make more special.
template
<
    typename MA,
    typename MV,
    typename T_N
>
void triangular_to_bidiagonal
(
    MA &A,
    MV &V,
    const T_N& N
);


// Document it!
// WARNING! Maybe the name of this function should make more special.
template
<
    typename MA,
    typename MV,
    typename T_N
>
void bidiagonal_to_smith
(
    MA &A,
    MV &V,
    const T_N& N
);


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_smith_near_optimal
    #include "near_optimal.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_smith_near_optimal
#endif

#endif    // #ifndef _ARAGELI_smith_near_optimal_hpp_
