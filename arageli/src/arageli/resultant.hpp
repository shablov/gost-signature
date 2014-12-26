/*****************************************************************************

    resultant.hpp

    This file is a part of the Arageli library.

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
    \file resultant.hpp
    \brief Routines for computing resultants and subresultant
        chains. Silvester matrix.

    This file contains historicaly first and primitive methods for computation
    subresultant chains and resultant in particular. Also there are routines
    for Silvester matrix generation.

    All given methods ripped from the book (in Russian, but translated from
    English): Компьютерная алгебра: Символьные и алгебраические вычисления:
    Пер. с англ./Под ред. Б.Бухбергера, Дж.Коллинза, Р.Лооса. ---
    М.: Мир, 1986.
*/


#ifndef _ARAGELI_resultant_hpp_
#define _ARAGELI_resultant_hpp_

#include "config.hpp"

#include "polyalg.hpp"
#include "matrix.hpp"


namespace Arageli
{


/// Fills non-zero elements in Sylvester matrix from two polynomials.
/** If res originaly is zero-matrix, the result is Sylvester matrix
    for these polynomials. Before filling the function appropriate
    resize a matrix. */
template <typename P1, typename P2, typename M>
M& sylvester_fill (const P1& p1, const P2& p2, M& res);


/// Creates Sylvester matrix from two polynomials.
/** This function resizes appropriatly and fills matrix 'res'. Returns
    just a reference to those matrix, i.e. a reference to 'res'.
    Factory 'fctr' should be intended to build objects of type 'res'
    element. */
template <typename P1, typename P2, typename M, typename MTFactory>
M& sylvester (const P1& p1, const P2& p2, M& res, MTFactory fctr);


/// Creates Sylvester matrix from two polynomials.
template <typename P1, typename P2, typename M>
inline M& sylvester (const P1& p1, const P2& p2, M& res)
{
    return sylvester(p1, p2, res, factory<typename M::element_type>());
}


/// Creates Sylvester matrix from two polynomials.
template <typename P1, typename P2>
inline matrix<typename P1::coef_type> sylvester
(const P1& p1, const P2& p2)
{
    // WARNING! Need to choose from P1::coef_type and P2::coef_type
    // as a type for the result matrix elements.

    matrix<typename P1::coef_type> res;
    return sylvester(p1, p2, res);
}


/// Computes the chain of subresultants of two polynomials.
/** Unfortunatly, we have a problem with signs of the result subresultants.
    BE CAREFUL! Use this function in case only if a sign of the result
    doesn't matter.
    @param s - should be a vector of polynomials compatible with p1 and p2. */
template <typename P1, typename P2, typename SRChain, typename PCFactory>
void subresultants_nonmodular
(const P1& p1, const P2& p2, SRChain& s, PCFactory fctr);


/// Computes the chain of subresultants of two polynomials.
/** See the full version of this function. */
template <typename P1, typename P2, typename SRChain>
inline void subresultants_nonmodular
(const P1& p1, const P2& p2, SRChain& s)
{
    // WARNING! Need to choose from P1::coef_type and P2::coef_type
    // insted just P1::coef_type as a type passed to construct the factory.

    subresultants_nonmodular(p1, p2, s, factory<typename P1::coef_type>());
}

/// Computes the resultant of two polynomials.
/** As we use subresultants_nonmodular in computation we have the same problem ---
    the problem with sign. See subresultants_nonmodular function for details. */
template <typename P1, typename P2, typename PCFactory>
typename P1::coef_type resultant_nonmodular
(const P1& p1, const P2& p2, PCFactory fctr);


/// Computes the resultant of two polynomials.
/** See the full version of this function. */
template <typename P1, typename P2>
inline typename P1::coef_type resultant_nonmodular
(const P1& p1, const P2& p2)
{
    // WARNING! Need to choose from P1::coef_type and P2::coef_type
    // insted just P1::coef_type as a type of the return value.

    return resultant_nonmodular(p1, p2, factory<typename P1::coef_type>());
}


/// Computes the resultant of two polynomials.
template <typename P1, typename P2, typename PCFactory>
inline typename P1::coef_type resultant
(const P1& p1, const P2& p2, PCFactory fctr)
{
    // WARNING! Need to choose from P1::coef_type and P2::coef_type
    // insted just P1::coef_type as a type of the return value.

    return resultant_nonmodular(p1, p2, fctr);
}

/// Computes the resultant of two polynomials.
template <typename P1, typename P2>
inline typename P1::coef_type resultant
(const P1& p1, const P2& p2)
{
    // WARNING! Need to choose from P1::coef_type and P2::coef_type
    // insted just P1::coef_type as a type of the return value and
    // type of factory.

    return resultant(p1, p2, factory<typename P1::coef_type>());
}


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_resultant
    #include "resultant.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_resultant
#endif

#endif    // #ifndef _ARAGELI_resultant_hpp_
