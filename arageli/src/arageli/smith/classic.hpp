/*****************************************************************************

    smith/classic.hpp -- the classic algirithm for the Smith Normal Diagonal
        Form of a matrix.

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
    This file contains declaration of the classic algirithm for the Smith
    Normal Diagonal Form of a matrix.
*/


#ifndef _ARAGELI_smith_classic_hpp_
#define _ARAGELI_smith_classic_hpp_

#include "../config.hpp"


namespace Arageli
{


///    Produces normal diagonal form B of integer matrix A.
/**
    Produces normal diagonal form B of integer matrix A.
    Returns B, P, Q, rank, det:
    - P, Q are unimodular such that B = P * A * Q;
    - rank is a rank of A;
    - det is the basis minor of A
*/
template
<
    typename MA,
    typename MB,
    typename MP,
    typename MQ,
    typename Rank,
    typename T_det
>
void smith_classic
(
    const MA &A,
    MB &B,
    MP &P,
    MQ &Q,
    Rank &rank,
    T_det &det
);


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_smith_classic
    #include "classic.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_smith_classic
#endif

#endif    // #ifndef _ARAGELI_smith_classic_hpp_
