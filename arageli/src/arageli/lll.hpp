/*****************************************************************************

    lll.hpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh

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
    \file lll.hpp
    \brief LLL basis reduction.
*/


#ifndef _ARAGELI_lll_hpp_
#define _ARAGELI_lll_hpp_

#include "config.hpp"


#include "std_import.hpp"


namespace Arageli
{

/// LLL basis reduction.
/**
    Function implements LLL algorithm for finding LLL-reduced basis.

    Input matrix B must contain vectors of an initial basis (in columns).
    On output B will contain the vectors of reduced basis.
    H will be the corresponding transformation matrix such that B_new = B_old * H

    If columns of B are not linear independent then the function return false,
    otherwise it returns true.

    The entries of B and H must be belong to some subfieled of the fieled of real numbers.

        @param B should be a matrix
        @param H should be a matrix
*/
template <typename B_type, typename H_type>
bool lll_reduction(B_type& B, H_type& H);

/// Integral LLL basis reduction.
/**
    Function implements integral LLL algorithm for finding LLL-reduced basis.

    Input matrix B must contain vectors of an initial basis (in columns).
    On output B will contain the vectors of reduced basis.
    H will be the corresponding transformation matrix such that B_new = B_old * H

    If columns of B are not linear independent then the function return false,
    otherwise it returns true.

    The entries of B and H must be integers.

        @param B should be a matrix
        @param H should be a matrix
*/
template <typename B_type, typename H_type>
bool lll_reduction_int(B_type& B, H_type& H);

}


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_LLL
    #include "lll.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_LLL
#endif


#endif  //  #ifndef _ARAGELI_lll_hpp_

