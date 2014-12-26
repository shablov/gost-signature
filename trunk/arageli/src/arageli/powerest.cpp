/*****************************************************************************

    powerest.cpp -- See description in file powerest.hpp.

    This file is a part of Arageli library.

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

#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_POWEREST)

#include "powerest.hpp"


namespace Arageli
{


template <typename T, typename I, typename T_factory>
T power (T a, I n, const T_factory& tfctr)
{
    ARAGELI_ASSERT_0(!is_null(a) || is_positive(n));

    bool isneg = is_negative(n);
    if(isneg)
        opposite(&n);
    T res = tfctr.unit(a);
    while(!is_null(n))
    {
        if(is_odd(n))
            res = res * a;
        a *= a;
        n >>= 1;
    }

    if(isneg)
        inverse(&res);

    return res;
}


} // namespace Arageli

#endif

