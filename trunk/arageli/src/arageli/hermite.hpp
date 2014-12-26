/*****************************************************************************

    hermite.hpp

    This file is a part of the Arageli library.

    Created:    2005/10/18    23:48

    Copyright (C) 2005--2006 Andrey Somsikov
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
    \file hermite.hpp
    \brief The Hermite form of a matrix.

    This file contains a set of functions for producing the Hermite form of
    a matrix.
*/


#ifndef _ARAGELI_hermite_hpp_
#define _ARAGELI_hermite_hpp_

#include <cstddef>

#include "config.hpp"

#include "hermite/storjohann.hpp"
#include "hermite/classic.hpp"
#include "hermite/hafner.hpp"
#include "hermite/domich.hpp"
#include "vector.hpp"
#include "matrix.hpp"

namespace Arageli
{


// WARNING! Explicit matrix type.
/// The old original function for producing Hermite form.
template <class T>
matrix<T> hermite_old (const matrix<T> &m_);


/// Produces the upper Hermite form H of an integer matrix A.
/**
    Returns H, U:
    U is unimodular princpal left transform such that H = U * A;
*/
template
<
    typename MA,
    typename MH,
    typename MU
>
inline void hermite_upper
(
    const MA &A,
    MH &H,
    MU &U
)
{
    typedef typename MA::element_type T_A;

    vector<std::size_t, false> basis;
    T_A det;
    hermite_storjohann(A, H, U, basis, det);
    //classic_hermite(A, H, U, basis, det);

    ARAGELI_ASSERT_1(H == U*A);
}


/// Produces the lower Hermite form H of an integer matrix A.
/**
    Returns H, U:
    U is unimodular princpal rigth transform such that H = A * U;
*/
template
<
    typename MA,
    typename MH,
    typename MU
>
inline void hermite_lower
(
    const MA &A,
    MH &H,
    MU &U
)
{
    typedef typename MA::element_type T_A;

    matrix<T_A, false> A2 = A;
    A2.transpose();
    hermite_upper(A2, H, U);
    H.transpose();
    U.transpose();

    ARAGELI_ASSERT_1(H == A*U);
}


/// Produces the upper Hermite form H of an integer matrix A.
template <typename M>
inline M hermite (const M& A)
{
    typedef typename M::element_type T;

    vector<std::size_t, false> basis;
    matrix<T, false> Q;
    matrix<T, true> B;
    T det;

    hermite_storjohann(A, B, Q, basis, det);
    //classic_hermite(A, B, Q, basis, det);
    return B;
}


/// Produces the unimodular princpal left transform U such that U*A is a Hermite form of an integer matrix A.
template <typename M>
inline M hermite_transform_matrix (const M& A)
{
    typedef typename M::element_type T;

    matrix<T, false> B;
    matrix<T, true> Q;
    vector<std::size_t, false> basis;
    T det;

    hermite_storjohann(A, B, Q, basis, det);
    // classic_hermite(A, B, Q, basis, det);
    return Q;
}


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_hermite
    #include "hermite.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_hermite
#endif

#endif    // #ifndef _ARAGELI_hermite_hpp_
