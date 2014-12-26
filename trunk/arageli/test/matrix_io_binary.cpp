/*****************************************************************************

    test/matrix_io_binary.cpp

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
    \file matrix_io_binary.cpp
    \brief Test binary serialization functions for matrix.

    This file is a test for input_binary_stream, output_binary_stream etc.
    functions. We store and load various matrix values to and from
    binary file streams and memory.
*/


#include "stdafx.hpp"

using namespace Arageli;


namespace
{


}


TEST
(
    matrix,
    output_input_binary,
    "Test binary serialization functions for matrix."
)
{
    bool is_ok = true;
    const int mem_reserve = 32*1024;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN
    {
        for(std::size_t n = 0; n < 4 && is_ok; ++n)
        {
            typedef matrix<int> T;

            is_ok &= test_io_simple_binary(T(), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(0, 2, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(2, 0, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(2, 3, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(1), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((14), (567890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((1), (24), (0), (890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((12345), (67890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-1234), (567890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((12,34), (5,6), (7,8), (9,0))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-123,456,789,0))"), n, mem_reserve);
            //is_ok &= test_io_simple_binary(T("(-10:10)"), n, mem_reserve);
        }

        for(std::size_t n = 0; n < 4 && is_ok; ++n)
        {
            typedef matrix<float> T;

            is_ok &= test_io_simple_binary(T(), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(0, 2, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(2, 0, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(2, 3, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(1), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((145), (67.8), (90))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T( "((124.0), (890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((12), (345.67), (890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-1), (23.456), (7), (8), (90))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((123), (4.5), (67890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-1,0), (23,4), (5.6,78), (9,0))"), n, mem_reserve);
            //is_ok &= test_io_simple_binary(T("((-1:0.1:1))"), n, mem_reserve);
        }

        for(std::size_t n = 0; n < 4 && is_ok; ++n)
        {
            typedef matrix<double> T;

            is_ok &= test_io_simple_binary(T(), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(0, 2, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(2, 0, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(2, 3, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(1), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((145), (67.890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((124.08), (90))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((1), (2345.67), (890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-12), (3.45), (678), (90))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((12,3,4.5,6), (7,8,9,0))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-123,45.6,789,0))"), n, mem_reserve);
            //is_ok &= test_io_simple_binary(T("((-1:0.1:1))"), n, mem_reserve);
        }

        for(std::size_t n = 0; n < 4 && is_ok; ++n)
        {
            typedef matrix<big_int> T;

            is_ok &= test_io_simple_binary(T(), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(0, 2, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(2, 0, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(2, 3, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(1), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((12323741923723444567890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((12345), (6789234082), (21240890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((1), (2), (3), (4), (5), (6), (7), (8), (9), (0))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-1234567890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((123,456), (78901234567,8), (9,0))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-1,23456), (78901234567,890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((1234567,890,12,3456), (78901234,5,6,7890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-1234,5,6,7), (8901234567890123,4,5,678), (9,0,0,0))"), n, mem_reserve);
            //is_ok &= test_io_simple_binary(T("((-10:10))"), n, mem_reserve);
            //is_ok &= test_io_simple_binary(T("((10000000000010:10000000000030))"), n, mem_reserve);
        }

        for(std::size_t n = 0; n < 4 && is_ok; ++n)
        {
            typedef matrix<rational<> > T;

            is_ok &= test_io_simple_binary(T(), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(0, 2, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(2, 0, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(2, 3, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(1), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((1234567), (890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((123456), (7890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-123456), (789012345678901234567890/654321781827334234))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-123), (456789012345678901234567890/654321781827334234))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((123456), (7890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-1234), (567890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((1234,56), (78901234567,890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-12345678901234567,890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((123456789012345678901234,5,6,7890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-123456,7,8,9), (01234567890123,4,5,67890))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((1234,567890/654,321))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-1,23,4), (5678,90/6543,2121))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((12345,6,7890123), (45678,90/6,5432143))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-1234,5,6,-78901,-234567890/65432165))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((12,3,4567890,1,2), (345678901234,5678,90/6543219873,24,1/2))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((-12345678/9012345678901), (234567890/654321781827334234))"), n, mem_reserve);
        }

        for(std::size_t n = 0; n < 3 && is_ok; ++n)
        {
            typedef matrix<matrix<int> > T;

            is_ok &= test_io_simple_binary(T(), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(0, 2, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(2, 0, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(2, 3, fromsize), n, mem_reserve);
            is_ok &= test_io_simple_binary(T(1), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((((1234567890))))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((((1,2,0),(3,4,5), (6,7,890))))"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("((((-1234, 56,-78,901,-234,-56), (78901,-56,2345,6543,217,81827334), (2,2,-56,2,2,2), (3,3,3,-56,3,3), (4,3,3,3,-56,3))))"), n, mem_reserve);
        }
    }
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END

    return is_ok ? resOK : resFAIL;
}
