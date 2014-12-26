/*****************************************************************************

    orthogonal.hpp

    This file is a part of the Arageli library.

    Copyright (C) 2007 Sergey S. Lyalin

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
    \file orthogonal.hpp
    \brief Basis orthogonalization.

    ADD WHOLE FILE DESCRIPTION HERE
*/


#ifndef _ARAGELI_orthogonal_hpp_
#define _ARAGELI_orthogonal_hpp_

#include "config.hpp"

// REFERENCE ADDITIONAL HEADERS HERE


namespace Arageli
{


/// Generic Gram-Schmidt Orthogonalization in an integer domain with explicit division.
/** The algorithm is described in
    Ulfar Erlingsson, Erich Kaltofen, David Musser
    Generic Gram-Schmidt orthogonalization by exact division.
    In Proc. 1996 Internat. Symp. Symbolic Algebraic Comput.
    (ISSAC'96) [-16], pages 275-282.
    URL: BASE/96/EKM96.pdf

    @param b_inp    an input basis for orthogonalization;
        a matrix where each row is a basis vector
    @param b_ort    an output orthogonal basis;
        a matrix where each row is a basis vector
*/
template <typename BInp, typename BOrt>
void orthogonalization_generic_gram_schmidt
(
    const BInp& b_inp,
    BOrt& b_ort
);


/// Project vector onto hyperplane defined through heterogeneous system of equalities.
/** System of equalities is ba = (-b|A), where Ax = b defines the hyperplane. */
template <typename BA, typename V>
V project_orthogonal_eq_ba
(
    const BA& ba,
    const V& v
);


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_orthogonal
    #include "orthogonal.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_orthogonal
#endif

#endif    // #ifndef _ARAGELI_orthogonal_hpp_
