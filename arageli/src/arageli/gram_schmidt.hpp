/*****************************************************************************

    gram_schmidt.hpp

    This file is a part of the Arageli library.

    Copyright (C) 2006 Nikolai Yu. Zolotykh

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
    \file gram_schmidt.hpp
    \brief Gram-Schmidt ortogonalization
*/


#ifndef _ARAGELI_gram_schmidt_hpp_
#define _ARAGELI_gram_schmidt_hpp_

#include "config.hpp"


#include "std_import.hpp"


namespace Arageli
{

/// Gram-Schmidt orthogonalization algorithm
/**
    Function implements Gram-Schmidt algorithm
    for finding orthogonal basis.

    Input matrix B must contain vectors of an initial basis (in columns).
    On output B will contain the vectors of reduced basis.
    Mu will be an upper triangular matrix such that its
    diagonal entries are equal to dot square of corresponding
    vectors in new basis. Taking diagonal entries of Mu to be 1
    we obtain the transformation matrix from new basis to
    old basis such that B_old = B_new * Mu

    If columns of B are not linear independent then the function return false,
    otherwise it returns true.

    The entries of B and H must be belong to some subfieled of the fieled of real numbers.

    @param B should be a matrix
    @param Mu should be a matrix
*/
template <typename B_type, typename Mu_type>
bool gram_schmidt (B_type& B, Mu_type& Mu);

}


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_GRAM_SCHMIDT
    #include "gram_schmidt.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_GRAM_SCHMIDT
#endif


#endif  //  #ifndef _ARAGELI_gram_schmidt_hpp_

