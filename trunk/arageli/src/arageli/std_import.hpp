/*****************************************************************************

    std_import.hpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    Copyright (C) 2008 Sergey S. Lyalin

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

#ifndef _ARAGELI_std_import_hpp_
#define _ARAGELI_std_import_hpp_

#include <cmath>
#include <complex>
#include <algorithm>

namespace Arageli
{

#define ARAGELI_STD_IMPORT_1(NAME)    \
    template <typename T>    \
    inline T NAME (const T& x)    \
    {    \
        return std::NAME(x);    \
    }

ARAGELI_STD_IMPORT_1(sin)
ARAGELI_STD_IMPORT_1(cos)
ARAGELI_STD_IMPORT_1(tan)
ARAGELI_STD_IMPORT_1(sinh)
ARAGELI_STD_IMPORT_1(cosh)
ARAGELI_STD_IMPORT_1(tanh)
ARAGELI_STD_IMPORT_1(asin)
ARAGELI_STD_IMPORT_1(acos)
ARAGELI_STD_IMPORT_1(atan)
ARAGELI_STD_IMPORT_1(abs)
ARAGELI_STD_IMPORT_1(exp)
ARAGELI_STD_IMPORT_1(floor)
ARAGELI_STD_IMPORT_1(ceil)
ARAGELI_STD_IMPORT_1(log)
ARAGELI_STD_IMPORT_1(log10)
ARAGELI_STD_IMPORT_1(sqrt)

template <typename T1, typename T2>
inline T1 pow (const T1& x, const T2& y)
{
    return std::pow(x, y);
}

template <typename T1, typename T2>
inline void swap (T1& x, T2& y)
{
    std::swap(x, y);
}

#undef min
#undef max

// WARNING! TEMPORARY DEFINITIONS FOR max AND min

template <typename T1, typename T2>
inline T1 min (const T1& x, const T2& y)
{
    return std::min(x, T1(y));  // WARNING! TYPE CAST
}

template <typename T1, typename T2>
inline T1 max (const T1& x, const T2& y)
{
    return std::max(x, T1(y));  // WARNING! TYPE CAST
}

// WARNING! TEMPORARY DEFINITION
template <typename T1, typename T2>
inline std::complex<T1> operator%
(
    const std::complex<T1>& a,
    const std::complex<T2>& b
)
{
    return a - b*(a/b);
}

// WARNING! TEMPORARY DEFINITION
template <typename T1, typename T2>
inline std::complex<T1>& operator%=
(
    std::complex<T1>& a,
    const std::complex<T2>& b
)
{
    return a -= b*(a/b);
}

}

#endif
