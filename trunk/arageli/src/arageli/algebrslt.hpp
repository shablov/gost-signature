/*****************************************************************************

    algebrslt.hpp

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
    \file algebrslt.hpp
    \brief Generic polynomial operations on algebraic numbers represented as
        a polynomial and a segment based on resultant.

    This file contains several functions that give possibility to deal with
    algebraic numbers represendes as a pair: polynomial and segment with one
    root of that polynomial. All routines based on Loos theorem about
    roots of resultant of two polynomials.

    All given methods ripped from the book (in Russian, but translated from
    English) Компьютерная алгебра: Символьные и алгебраические вычисления:
    Пер. с англ./Под ред. Б.Бухбергера, Дж.Коллинза, Р.Лооса. ---
    М.: Мир, 1986.
*/


#ifndef _ARAGELI_algebrslt_hpp_
#define _ARAGELI_algebrslt_hpp_

#include "config.hpp"

// REFERENCE ADDITIONAL HEADERS HERE


namespace Arageli
{


/// Arbitrary function on algebraic numbers based on precomputed resultant.
template
<
    typename PolyVec,
    typename SegVec,
    typename Rslt,
    typename P,
    typename Seg,
    typename F
>
void algebraic_func_rslt
(
    const PolyVec& polyvec, // source polynomials
    SegVec segvec, // source segments
    Rslt rslt, // source resultant
    P& p, Seg& seg,    // the result: polynomial and segment where root is located
    F f    // the function that is performing on the source algebraic numbers
);


// WARNING! TEMPORARY.
template
<
    typename P1, typename Seg1,
    typename P2, typename Seg2,
    typename P3, typename Seg3
>
void algebraic_plus
(
    const P1& p1, const Seg1& seg1,
    const P2& p2, const Seg2& seg2,
    P3& p3, Seg3& seg3
);


// WARNING! TEMPORARY.
template
<
    typename P1, typename Seg1,
    typename P2, typename Seg2,
    typename P3, typename Seg3
>
void algebraic_minus
(
    const P1& p1, const Seg1& seg1,
    const P2& p2, const Seg2& seg2,
    P3& p3, Seg3& seg3
);


// WARNING! TEMPORARY.
template
<
    typename P1, typename Seg1,
    typename P2, typename Seg2,
    typename P3, typename Seg3
>
void algebraic_multiplies
(
    const P1& p1, const Seg1& seg1,
    const P2& p2, const Seg2& seg2,
    P3& p3, Seg3& seg3
);


// WARNING! TEMPORARY.
template
<
    typename P1, typename Seg1,
    typename P2, typename Seg2,
    typename P3, typename Seg3
>
void algebraic_divides
(
    const P1& p1, const Seg1& seg1,
    const P2& p2, Seg2 seg2,
    P3& p3, Seg3& seg3
);


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_algebrslt
    #include "algebrslt.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_algebrslt
#endif

#endif    // #ifndef _ARAGELI_algebrslt_hpp_
