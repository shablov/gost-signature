/*****************************************************************************

    groebner_basis.hpp

    This file is a part of the Arageli library.

    Copyright (C) 2002, 2006 Max A. Alekseyev
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
    \file groebner_basis.hpp
    \brief Grobner basis routines.

    This file contains an implementation of Buchberger's algorithm to
    calculate Groebner basis for polynomial ideal.
*/


#ifndef _ARAGELI_groebner_basis_hpp_
#define _ARAGELI_groebner_basis_hpp_

#include "config.hpp"

// REFERENCE ADDITIONAL HEADERS HERE


namespace Arageli
{


/// Expand existing reduced Groebner basis by new polynomials.
/** The arguments vp and vres should be vectors of multivariate polynomials.
    If vgb is empty, the function just builds Groebner basis vgb by vp.
    Note, the polynomial are not normed.
    @param vp the new set of polynomials to be added to vgb
    @param existing Groebner basis to be expanded by vp. */
template <typename VP, typename VGB>
void groebner_basis (const VP& vp, VGB& vgb);


/// Compute reduced Groebner basis for a system of polynomials.
/** Note, the polynomial are not normed. */
template <typename VP>
inline VP groebner_basis (const VP& vp)
{
    VP res;
    groebner_basis(vp, res);
    return res;
}



} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_groebner_basis
    #include "groebner_basis.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_groebner_basis
#endif

#endif    // #ifndef _ARAGELI_groebner_basis_hpp_
