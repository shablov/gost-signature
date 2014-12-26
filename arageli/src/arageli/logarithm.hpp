/*****************************************************************************

    logarithm.hpp

    This file is a part of the Arageli library.

    Copyright (C) 2005--2006 Alexander Pshenichnikov
    Copyright (C) 2005--2006 Nikolay Santalov

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
    \file logarithm.hpp
    \brief  <!--ADD BRIEF HEADER DESCRIPTION HERE-->

    <!--ADD ADDITIONAL FILE DESCRIPTION HERE-->
*/


#ifndef _ARAGELI_logarithm_h_
#define _ARAGELI_logarithm_h_

#include "config.hpp"

#include <cstddef>

#include "big_int.hpp"
#include "std_import.hpp"

namespace Arageli
{

std::size_t lg10 (std::size_t b);

void do_bin_convert
(
    const big_int& dm,
    const big_int& de,
    std::size_t p,
    big_int& bm,
    big_int& be
);

std::size_t do_dec_convert
(
    big_int& dm,
    big_int& de,
    std::size_t p,
    const big_int& bm,
    const big_int& be
);

}

#endif
