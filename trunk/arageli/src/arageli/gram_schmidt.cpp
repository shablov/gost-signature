/*****************************************************************************

    gramschmidt.cpp

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

#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_GRAM_SCHMIDT)


#include <cmath>

#include "cmp.hpp"
#include "vector.hpp"
#include "gram_schmidt.hpp"


namespace Arageli
{

template <typename B_type, typename Mu_type>
bool gram_schmidt(B_type& B, Mu_type& Mu)
{
    typedef typename B_type::difference_type index;
    typedef typename B_type::value_type T;

    Mu.resize(B.ncols(), B.ncols());
    index B_ncols = B.ncols();

    for (index k = 0; k < B_ncols; k++)
    {
        Mu(k, k) = dotprod(B.copy_col(k), B.copy_col(k));
        if(is_null(Mu(k, k)))
            return false; // B(i) did not form the basis

        for(index j = 0; j < k; j++)
        {
            Mu(j, k) = dotprod(B.copy_col(k), B.copy_col(j)) / Mu(j, j);
            B.assign_col(k, B.copy_col(k) - B.copy_col(j) * Mu(j, k));
            Mu(k, k) = dotprod(B.copy_col(k), B.copy_col(k));
        }
    }

    return true;
}

} // namespace Arageli

#endif // #ifndef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
