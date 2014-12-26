/*****************************************************************************

    big_const.hpp

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
    \file big_const.hpp
    \brief Functions that calculates several constants.

    All fuctions return integer result.
    But the sought value equal result / 2 ^ (nbits + 1)
    Functions terminating by _o return result with
    0 <= error <= (1/2) ^ ( nbits ).
    Functions terminating by _u return result with
    -(1/2 )^ ( nbits ) <= error <=0
    Functions without termination return result with
    -(1/2 )^ ( nbits + 1 ) <= error <= -(1/2 )^ ( nbits + 1 )
    It means that (sought value) * 2 ^ ( nbits + 1) is in range [ res, res + 2 ]
    where res is result returned by "_u"  function.
*/

#ifndef _ARAGELI_big_const_h_
#define _ARAGELI_big_const_h_

#include <cstddef>

#include "config.hpp"
#include "big_int.hpp"
#include "std_import.hpp"

namespace Arageli
{
    /// Returns ln(1.25) with nbits significant bits
    big_int ln1_25(std::size_t nbits);

    /// Returns ln(1.25) with nbits significant bits with overflow
    big_int ln1_25_o(std::size_t nbits);

    /// Returns ln(1.25) with nbits significant bits with imperfection
    big_int ln1_25_u(std::size_t nbits);

    /// Returns ln(2) with nbits significant bits
    big_int ln2 (std::size_t nbits);

    /// Returns ln(2) with nbits significant bits with imperfection
    big_int ln2_u(std::size_t nbits);

    /// Returns ln(2) with nbits significant bits with oveflow
    big_int ln2_o(std::size_t nbits);

    /// Returns lg2 with nbits significant bits
    big_int lg_2( std::size_t nbits );

    /// Returns lg2 with nbits significant bits with imperfection
    big_int lg_2_u( std::size_t nbits );

    /// Returns lg2 with nbits significant bits with oveflow
    big_int lg_2_o( std::size_t nbits );

    /// Returns log2_10 with nbits significant bits
    big_int log2_10( std::size_t nbits ) ;//Returnslog2_10

    /// Returns log2_10 with nbits significant bits with oveflow
    big_int log2_10_o( std::size_t nbits ) ;//Returnslog2_10 with nbits significant bits with overflow

    /// Returns log2_10 with nbits significant bits with imperfection
    big_int log2_10_u( std::size_t nbits ) ;//Returnslog2_10 with nbits significant bits with imperfection

    /// Returns exp(arg/2^factor) with nbits significant bits
    big_int exp ( big_int arg, std::size_t nbits, int factor = 0 );

    /// Returns exp(arg/2^factor) with nbits significant bits with imperfection
    big_int exp_u ( big_int arg, std::size_t nbits, int factor = 0 );

    /// Returns exp(arg/2^factor) with nbits significant bits with oveflow
    big_int exp_o ( big_int arg, std::size_t nbits, int factor = 0 );

    /// Returns 2^(arg/2^factor) with nbits significant bits
    big_int exp_2 ( big_int arg, std::size_t nbits, int factor = 0 );

    /// Returns 2^(arg/2^factor) with nbits significant bits with imperfection
    big_int exp_2_u ( big_int arg, std::size_t nbits, int factor = 0 );

    /// Returns 2^(arg/2^factor) with nbits significant bits with oveflow
    big_int exp_2_o (big_int arg, std::size_t nbits, int factor = 0 );

    /// Returns e number with nbits significant bits
    big_int e (std::size_t nbits  );

    /// Returns e number with nbits significant bits with imperfection
    big_int e_u (std::size_t nbits  );

    /// Returns e number with nbits significant bits with oveflow
    big_int e_o ( std::size_t nbits  );

    /// Returns ln10 with nbits significant bits
    big_int ln10 (std::size_t nbits);

    /// Returns ln10 with nbits significant bits with imperfection
    big_int ln10_u(std::size_t nbits);

    /// Returns ln10 with nbits significant bits with oveflow
    big_int ln10_o(std::size_t nbits);

    // WARNING! Macro!!!
    #define log2floor(x) ((std::size_t)(log((long double)(x))/log(2.0l)))
    // floor of log2(x)
}

#endif
