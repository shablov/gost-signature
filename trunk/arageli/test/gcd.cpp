/*****************************************************************************

    test/gcd.cpp

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

#include "stdafx.hpp"


using namespace Arageli;

bool gcd_int_test(int a, int b)
{
    int g = gcd<int>(a, b);
    int g1 = gcd<int>(-a, b);
    int g2 = gcd<int>(a, -b);
    int g3 = gcd<int>(-a, -b);

    if (0 != a%g || 0 != b%g)
    {
        tout << "GCD fails with"
            << " gcd( " << a << ", " << b << ") = " << g << "\n";
        return true;
    }

    if (g != g1 || g != g2 || g != g3)
    {
        tout << "GCD fails with"
            << " g != g1 || g != g2 || g != g3\n";
            return true;
    }

    return false;
}

TEST_FUNCTION(gcd, "Test gcd function")
{
    bool fail = false;

    fail |= gcd_int_test(8, 8);
    fail |= gcd_int_test(8, 64);
    fail |= gcd_int_test(64, 8);
    fail |= gcd_int_test(0, 0);
    fail |= gcd_int_test(1, 0);
    fail |= gcd_int_test(0, 1);
    fail |= gcd_int_test(5, 7);
    fail |= gcd_int_test(14, 10);

    fail |= gcd_int_test(big_int(8), big_int(8));
    fail |= gcd_int_test(big_int(8), big_int(64));
    fail |= gcd_int_test(big_int(64), big_int(8));
    fail |= gcd_int_test(big_int(0), big_int(0));
    fail |= gcd_int_test(big_int(1), big_int(0));
    fail |= gcd_int_test(big_int(0), big_int(1));
    fail |= gcd_int_test(big_int(5), big_int(7));
    fail |= gcd_int_test(big_int(14), big_int(10));

    if (fail)
        return resFAIL;

    return resOK;
}

/*
TEST_FUNCTION(gcd_gauss)
{
    rational<int> aData[] = {-10, 10, 10, 0};
    rational<int> bData[] = {-5, -3, 0};
    polynom< rational<int> > a(aData, COUNT_OF(aData));
    polynom< rational<int> > b(bData, COUNT_OF(bData));

    polynom< rational<int> > g = gcd_gauss(a, b);

    tout << a << "\n";
    tout << b << "\n";
    tout << g << "\n";

    return resOK;
}
*/

/* End of file gcd.cpp */
