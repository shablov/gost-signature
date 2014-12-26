/*****************************************************************************

    cantor_zassenhaus.hpp

    This file is a part of the Arageli library.

    Copyright (C) 2006--2007 Sergey V. Lobanov

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
    \file cantor_zassenhaus.hpp
    \brief Header file for Cantor-Zassenhaus factorization method for Z<sub>p</sub>[x]

    ADD WHOLE FILE DESCRIPTION HERE
*/


#ifndef _ARAGELI_cantor_zassenhaus_hpp_
#define _ARAGELI_cantor_zassenhaus_hpp_

#include "config.hpp"

// REFERENCE ADDITIONAL HEADERS HERE


namespace Arageli
{

/// Distinct-degree factorization
/** The input is square-free polynomial f_in in Z<sub>p</sub>[x]
    of degree n.
    The output is two vectors res and v_deg: res[i] is the product
    of monic irreducible polynomials, each of degree v_deg[i]
    Typically P is sparse_polynom<residue<big_int> >, V is vector<P>*/
template<typename P,typename V,typename V2>
void DDF_sparse(const P& f_in, V& res,V2& v_deg);

template<typename P,typename V,typename V2>
void DDF_dense(const P& f_in, V& res,V2& v_deg);


// PLACE ALL DECLARATIONS AND INLINE IMPLEMENTATIONS HERE


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_cantor_zassenhaus
    #include "cantor_zassenhaus.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_cantor_zassenhaus
#endif

#endif    // #ifndef _ARAGELI_cantor_zassenhaus_hpp_
