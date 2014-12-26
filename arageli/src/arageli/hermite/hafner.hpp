/*****************************************************************************

    hermite/hafner.hpp -- the Hermite form H of an integer matrix A over Z/Z(N).

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
    This file contains a function for producing the Hermite form H of
    an integer matrix A over Z/Z(N).
*/


#ifndef _ARAGELI_hermite_hafner_hpp_
#define _ARAGELI_hermite_hafner_hpp_

#include "../config.hpp"


namespace Arageli
{


///    Produces the Hermite form H of an integer matrix A over Z/Z(N).
/**
    Returns H, U, basis:
    - U is unimodular princpal left transform such that H = U*A mod N;
    - rank is a rank of A;
    - H.sumbatrix(mesh_grid(1,r), basis) is the r-by-r non-singular
    upper triangular matrix.
*/
template
<
    typename MA,
    typename MH,
    typename MU,
    typename Basis,
    typename T_N
>
void hermite_hafner
(
    const MA &A,
    MH &H,
    MU &U,
    Basis &basis,
    T_N N
);


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_hermite_hafner
    #include "hafner.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_hermite_hafner
#endif

#endif    // #ifndef _ARAGELI_hermite_hafner_hpp_
