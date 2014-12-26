/*****************************************************************************

    test/rational_io_binary.cpp

    This file is a part of the Arageli library test base.

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

/**
    \file rational_io_binary.cpp
    \brief Test binary serialization functions for rational.

    This file is a test for input_binary_stream, output_binary_stream etc.
    functions. We store and load various rational values to and from
    binary file streams and memory.
*/


#include "stdafx.hpp"

using namespace Arageli;


namespace
{


}


TEST
(
    rational,
    output_input_binary,
    "Test binary serialization functions for rational."
)
{
    bool is_ok = true;
    const int mem_reserve = 32*1024;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN
    {
        for(std::size_t n = 0; n < 10 && is_ok; ++n)
        {
            typedef rational<int> T;

            is_ok &= test_io_simple_binary(T(), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(1), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(-1), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("12345"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-13456"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("123/65"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-10/6321"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("1/6"), n, mem_reserve);
        }

        for(std::size_t n = 0; n < 4 && is_ok; ++n)
        {
            typedef rational<big_int> T;

            is_ok &= test_io_simple_binary(T(), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(1), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(-1), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("1234567890"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-1234567890"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("12345678901234567890"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-12345678901234567890"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("123456789012345678901234567890"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-123456789012345678901234567890"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("1234567890/654321"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-1234567890/65432121"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("12345678901234567890/65432143"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-12345678901234567890/65432165"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("123456789012345678901234567890/654321987324"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-123456789012345678901234567890/654321781827334234"), n, mem_reserve);
        }
    }
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END

    return is_ok ? resOK : resFAIL;
}
