/*****************************************************************************

    hermite/domich.hpp -- an algorithm for the Hermite form of an integer
        matrix with full column rank.

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
    This file contains a declaration of a function that produce the Hermite
    form B of an integer matrix A with full column rank.
*/


#ifndef _ARAGELI_hermite_domich_hpp_
#define _ARAGELI_hermite_domich_hpp_

#include "../config.hpp"


namespace Arageli
{


// WARNING! The documentation doesn't fit to function definition.
// TODO: Document it!

//    Produces the Hermite form B of an integer matrix A with full column rank.
/*
    Returns B, P, basis, det:
    - P is unimodular princpal left transform such that B = P * A;
    - rank is a rank of A;
    - B.sumbatrix(mesh_grid(1,r), basis) is the r-by-r non-singular
    upper triangular matrix;
    - det is the basis minor of A.
*/

template
<
    typename MA,
    typename MH,
    typename MU,
    typename Basis,
    typename T_det
>
void hermite_domich
(
    const MA &A,
    MH &H,
    MU &U,
    Basis &basis,
    T_det &det
);


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_hermite_domich
    #include "domich.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_hermite_domich
#endif

#endif    // #ifndef _ARAGELI_hermite_domich_hpp_
