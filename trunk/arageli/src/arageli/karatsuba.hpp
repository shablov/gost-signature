/*****************************************************************************

    karatsuba.hpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2006 Aleksey Bader

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
    \file karatsuba.hpp
    \brief This file contains definition of karatsuba multiplication function.

    Here is implementation of karatsuba multiplication algorithm invented by
    Karatsuba Anatolii Alexeevich in 1962.
    You can find description of algoritm on this web page -
    http://en.wikipedia.org/wiki/Karatsuba_algorithm

    Official publication:
    A. Karatsuba and Yu Ofman, "Multiplication of Many-Digital Numbers
    by Automatic Computers." Doklady Akad. Nauk SSSR Vol. 145 (1962),
    pp. 293–294. Translation in Physics-Doklady 7 (1963), pp. 595–596.
*/


#ifndef _ARAGELI_karatsuba_hpp_
#define _ARAGELI_karatsuba_hpp_

#include "config.hpp"

namespace Arageli
{
/// Multiply two integers by karatsuba algorithm.
/** @param u list of digits of the first number
    @param v list of digits of the second number
    @param w list of digits for result number
    @param t temporary memory
    @param m length of the first number
    @param n length of the second number*/
template <typename N,typename T>
T do_mult_karatsuba(const N *u, const N *v, N *w, N *t, T m, T n, T threshold = 50);
} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_karatsuba
    #include "karatsuba.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_karatsuba
#endif

#endif    // #ifndef _ARAGELI_karatsuba_hpp_

