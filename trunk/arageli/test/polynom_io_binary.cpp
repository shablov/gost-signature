/*****************************************************************************

    test/polynom_io_binary.cpp

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
    \file polynom_io_binary.cpp
    \brief Test binary serialization functions for polynomial classes.

    This file is a test for input_binary_stream, output_binary_stream etc.
    functions. We store and load various polynom values to and from
    binary file streams and memory.
*/


#include "stdafx.hpp"

using namespace Arageli;


namespace
{


template <typename P>
bool test_polynom_1 ()
{
    bool is_ok = true;
    const int mem_reserve = 32*1024;

    for(std::size_t n = 0; n < 4 && is_ok; ++n)
    {
        typedef typename P::template type<int>::type2 T;

        is_ok &= test_io_simple_binary(T(), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(14567890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(1240890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(1234*x+567*x^2+890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-1234567890*x^10)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(12-15*x+3*x^4+5*x^2+67+8+9*x^4+0)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-123+456+789+0)"), n, mem_reserve);
        //is_ok &= test_io_simple_binary(T("(-10:10)"), n, mem_reserve);
    }

    for(std::size_t n = 0; n < 4 && is_ok; ++n)
    {
        typedef typename P::template type<float>::type2 T;

        is_ok &= test_io_simple_binary(T(), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(14567.890*x^2)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(124.0890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(12345.67890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-123.4567890*x^2)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(1234.567*x^2+8*x^4+9+0)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-123*x^2+45.6*x^2+789*x^4+0)"), n, mem_reserve);
        //is_ok &= test_io_simple_binary(T("(-1:0.1:1)"), n, mem_reserve);
    }

    for(std::size_t n = 0; n < 4 && is_ok; ++n)
    {
        typedef typename P::template type<double>::type2 T;

        is_ok &= test_io_simple_binary(T(), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(14567.890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(124.0890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(12345.67890*x^2)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-123.4567890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(12*x^4+34.567*x^2+8*x^2+9+0)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-123*x^4+45.6+789*x^2+0)"), n, mem_reserve);
        //is_ok &= test_io_simple_binary(T("(-1:0.1:1)"), n, mem_reserve);
    }

    for(std::size_t n = 0; n < 4 && is_ok; ++n)
    {
        typedef typename P::template type<big_int>::type2 T;

        is_ok &= test_io_simple_binary(T(), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(12323741923723444567890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(12345678923408221240890*x^2)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(1234567890*x^2)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-1234567890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(12345678901234567+890*x^4)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-12345678901234567*x^4+890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(123456789012345678901234*x^2+5+6+7890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-12345678901234567890123+4*x^2+5+67890*x^2)"), n, mem_reserve);
        //is_ok &= test_io_simple_binary(T("(-10:10)"), n, mem_reserve);
        //is_ok &= test_io_simple_binary(T("(10000000000010:10000000000030)"), n, mem_reserve);
    }

    for(std::size_t n = 0; n < 4 && is_ok; ++n)
    {
        typedef typename P::template type<rational<> >::type2 T;

        is_ok &= test_io_simple_binary(T(), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(1234567890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(1234567890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-123456789012345678901234567890/654321781827334234)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-123456789012345678901234567890/654321781827334234*x^2)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(1234567890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-1234567890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(12345678901234567+890*x^2)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-12345678901234567+890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(123456789012345678901234+5*x^2+6+7890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-12345678901234567890123+4*x^4+5*x^2+67890)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(1234+567890/654+321*x^2)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-12345678+90/6543+2121*x^4)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(123456789012345678*x^2+90/6+5432143)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-1234+5+6*x^4-78901-234567890/65432165)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(123456789012345678901234*x^2+5678+90/6543219873+24*x^4+1/2)"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("(-123456789012345678901234567890/654321781827334234*x^2)"), n, mem_reserve);
    }

    for(std::size_t n = 0; n < 3 && is_ok; ++n)
    {
        typedef typename P::template type<typename P::template type<int>::type2>::type2 T;

        is_ok &= test_io_simple_binary(T(), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("((1234567890*x^2))"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("((12*x^2)*x^1+(345*x-1)*x^2+(6+7*x^2+890))"), n, mem_reserve);
        is_ok &= test_io_simple_binary(T("((-1234+56*x^3-78*x^4-901*x^2-234*x-56*x^10)+(789012345-6543217*x^2+81827334*x^3)*x^2+(2*x^4)+(3)*x^4+(4))"), n, mem_reserve);
    }

    return is_ok;
}


struct polynom_help
{
    template <typename T>
    struct type
    {
        typedef polynom<T> type2;
    };
};


struct sparse_polynom_help
{
    template <typename T>
    struct type
    {
        typedef sparse_polynom<T> type2;
    };
};


}


TEST
(
    polynom,
    output_input_binary,
    "Test binary serialization functions for polynom."
)
{
    bool is_ok = false;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN
    {
        is_ok |= test_polynom_1<polynom_help>();
    }
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END

    return is_ok ? resOK : resFAIL;
}


TEST
(
    sparse_polynom,
    output_input_binary,
    "Test binary serialization functions for sparse_polynom."
)
{
    bool is_ok = false;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN
    {
        is_ok |= test_polynom_1<sparse_polynom_help>();
    }
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END

    return is_ok ? resOK : resFAIL;
}


TEST
(
    monom,
    output_input_binary,
    "Test binary serialization functions for monom."
)
{
    bool is_ok = true;
    const int mem_reserve = 32*1024;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN
    {
        for(std::size_t n = 0; n < 4 && is_ok; ++n)
        {
            typedef monom<int> T;

            is_ok &= test_io_simple_binary(T(), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("14567890"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("1240890"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("1234*x"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-1234567890*x^10"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-15*x"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-123"), n, mem_reserve);
        }

        for(std::size_t n = 0; n < 4 && is_ok; ++n)
        {
            typedef monom<float> T;

            is_ok &= test_io_simple_binary(T(), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("14567.890*x^2"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("124.0890"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("12345.67890"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-123.4567890*x^2"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("12"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-123*x"), n, mem_reserve);
        }

        for(std::size_t n = 0; n < 4 && is_ok; ++n)
        {
            typedef monom<double> T;

            is_ok &= test_io_simple_binary(T(), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("14567.890*x^2"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("124.0890"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("12345.67890"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-123.4567890*x^2"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("12"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-123*x"), n, mem_reserve);
        }

        for(std::size_t n = 0; n < 4 && is_ok; ++n)
        {
            typedef monom<big_int> T;

            is_ok &= test_io_simple_binary(T(), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("12323741923723444567890"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("12345678923408221240890*x^2"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("1234567890*x^2"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-1234567890"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("12345678901234567"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-12345678901234567*x^4"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("123456789012345678901234*x"), n, mem_reserve);
            is_ok &= test_io_simple_binary(T("-12345678901234567890123"), n, mem_reserve);
        }

    }
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END

    return is_ok ? resOK : resFAIL;
}

