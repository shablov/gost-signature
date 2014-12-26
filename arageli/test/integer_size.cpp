/*****************************************************************************

    test/integer_size.cpp

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
    \file integer_size.cpp
    \brief This file includes tests for nbits, ndigits and magnitude.

    <!--ADD ADDITIONAL FILE DESCRIPTION HERE-->
*/


#include "stdafx.hpp"

using namespace Arageli;


namespace
{

template <typename T>
bool test_nbits (const T& x, std::size_t size)
{
    return nbits(x) == size;
}

template <typename T>
bool test_ndigits (const T& x, std::size_t size, std::size_t radix)
{
    if(ndigits(x, radix) == size)
        return true;
    else
    {
        tout << "ndigits(" << x << ", " << radix << ") != " << size << "\n";
        return false;
    }
}

template <typename T>
bool test_nbits_radix_bound (std::size_t size)
{
    bool is_ok = true;

    is_ok &= test_nbits<T>(T(1) << size-2, size-1);
    is_ok &= test_nbits<T>(-(T(1) << size-2), size-1);
    is_ok &= test_nbits<T>(T(1) << size-1, size);
    is_ok &= test_nbits<T>(-(T(1) << size-1), size);
    is_ok &= test_nbits<T>(T(1) << size, size+1);
    is_ok &= test_nbits<T>(-(T(1) << size), size+1);
    is_ok &= test_nbits<T>((T(1) << size) - 1, size);
    is_ok &= test_nbits<T>(-(T(1) << size) + 1, size);

    return is_ok;
}

template <typename T>
bool test_ndigits_radix_bound (std::size_t size, std::size_t radix)
{
    ARAGELI_ASSERT_0(size > 2 || radix > 2);

    bool is_ok = true;

    is_ok &= test_ndigits<T>(power(T(radix), size)-1, size, radix);
    is_ok &= test_ndigits<T>(-power(T(radix), size)+1, size, radix);
    is_ok &= test_ndigits<T>(power(T(radix), size), size+1, radix);
    is_ok &= test_ndigits<T>(-power(T(radix), size), size+1, radix);
    is_ok &= test_ndigits<T>(power(T(radix), size)+1, size+1, radix);
    is_ok &= test_ndigits<T>(-power(T(radix), size)-1, size+1, radix);

    return is_ok;
}

}


TEST_FUNCTION
(
    nbits,
    "Test for nbits function."
)
{
    bool is_ok = true;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN
    {
        is_ok &= test_nbits<big_int>("0", 0);
        is_ok &= test_nbits<big_int>("1", 1);
        is_ok &= test_nbits<big_int>("-1", 1);
        is_ok &= test_nbits<big_int>("2", 2);
        is_ok &= test_nbits<big_int>("-2", 2);
        is_ok &= test_nbits<big_int>("3", 2);
        is_ok &= test_nbits<big_int>("-3", 2);
        is_ok &= test_nbits<big_int>("4", 3);
        is_ok &= test_nbits<big_int>("-4", 3);

        is_ok &= test_nbits_radix_bound<big_int>(1*16);
        is_ok &= test_nbits_radix_bound<big_int>(2*16);
        is_ok &= test_nbits_radix_bound<big_int>(3*16);
        is_ok &= test_nbits_radix_bound<big_int>(4*16);
        is_ok &= test_nbits_radix_bound<big_int>(5*16);
    }
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END

    return is_ok ? resOK : resFAIL;
}

TEST_FUNCTION
(
    ndigits,
    "Test for ndigits function."
)
{
    bool is_ok = true;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN
    {
        is_ok &= test_ndigits<big_int>("0", 0, 10);
        is_ok &= test_ndigits<big_int>("1", 1, 10);
        is_ok &= test_ndigits<big_int>("-1", 1, 10);
        is_ok &= test_ndigits<big_int>("2", 1, 10);
        is_ok &= test_ndigits<big_int>("-2", 1, 10);
        is_ok &= test_ndigits<big_int>("3", 1, 10);
        is_ok &= test_ndigits<big_int>("-3", 1, 10);
        is_ok &= test_ndigits<big_int>("4", 1, 10);
        is_ok &= test_ndigits<big_int>("-4", 1, 10);

        is_ok &= test_ndigits_radix_bound<big_int>(1, 10);
        is_ok &= test_ndigits_radix_bound<big_int>(2, 10);
        is_ok &= test_ndigits_radix_bound<big_int>(3, 10);
        is_ok &= test_ndigits_radix_bound<big_int>(4, 10);
        is_ok &= test_ndigits_radix_bound<big_int>(5, 10);
    }
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END

    return is_ok ? resOK : resFAIL;
}

TEST_FUNCTION
(
    magnitude,
    "Test for magnitude function."
)
{
    bool is_ok = true;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN
    {
        is_ok &= (magnitude(big_int("0")) <= 1);
        is_ok &= (magnitude(big_int("1")) <= 1);
        is_ok &= (magnitude(big_int("-1")) <= 1);
        is_ok &= (magnitude(big_int("2")) <= 1);
        is_ok &= (magnitude(big_int("-2")) <= 1);
        is_ok &= (magnitude(big_int("3")) <= 1);
        is_ok &= (magnitude(big_int("-3")) <= 1);

        is_ok &= (magnitude(big_int(1) << _Internal::bits_per_digit) > 1);
        is_ok &= (magnitude(big_int(1) << 2*_Internal::bits_per_digit) > 2);
        is_ok &= (magnitude(big_int(1) << 3*_Internal::bits_per_digit) > 3);
    }
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END

    return is_ok ? resOK : resFAIL;
}
