/*****************************************************************************

    hermite.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    Copyright (C) 2005 Andrey Somsikov

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
    \file hermite.cpp
    \brief The hermite.hpp file stuff implementation.
*/

#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_hermite)

#include "hermite.hpp"
#include <iostream>

namespace Arageli
{

template <class T>
matrix<T> hermite_old (const matrix<T> &m)
{
    typedef matrix<T> M;
    typedef typename M::value_type MV;
    typedef typename M::size_type size_type;

    M res(m);

    size_type i = 0, j = 0;
    while(i < res.nrows()-1 && j < res.ncols()-1)
    {
/*
output_aligned(std::cout, res, "|| ", " ||", "  ");
std::cout << "\n";
*/
        // ensure that el(i,j)>0
        if (is_null(res.el(i, j)))
        {
            size_type k;
            // find non zero element in column j
            for (k = i+1; k < res.nrows(); k++)
                if (!is_null(res.el(k, j)))
                    break;
            if (k == res.nrows())
            {
                // skip column if not found
                j++;
                continue;
            }
            res.swap_rows(i, k);
/*
            // ???
            if (res.el(i, j) < factory<MV>::null())
                res.mult_row(i, factory<MV>::opposite_unit());
*/
        }

        // find pivot element
        size_type minValueIndex = i;
        bool isNonZero = false;
        for (size_type k = i+1; k < res.ncols(); k++)
        {
            if (!is_null(res.el(k, j)))
            {
                isNonZero = true;
                if (res.el(k, j).degree() < res.el(minValueIndex, j).degree())
                    minValueIndex = k;
            }
        }
        if (!isNonZero)
        {
            // nothing to do with this element
            i++;
            j++;
            continue;
        }
        res.swap_rows(i, minValueIndex);
/*
        if (res.el(i, j) < factory<MV>::null())
            res.mult_row(i, factory<MV>::opposite_unit());
*/

        for (size_type k = 0; k < i; k++)
            res.addmult_rows(k, i, factory<MV>::opposite_unit()*(res.el(k, j) / res.el(i, j)));
        for (size_type k = i+1; k < res.ncols(); k++)
            res.addmult_rows(k, i, factory<MV>::opposite_unit()*(res.el(k, j) / res.el(i, j)));
    }

    return res;
}

} // namespace Arageli

#endif // #ifndef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE

/* End of file hermite.cpp */
