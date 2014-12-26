/*****************************************************************************

    test/rho_pollard_function.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    Copyright (C) 2006 Aleksey Bader
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

#include "stdafx.hpp"


TEST_FUNCTION(rho_pollard, "Test rho_pollard function")
{
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN

    using namespace Arageli;
    srand( (unsigned)time( NULL ) );
    TestResult res = resOK;
    int i = 20;
    while(i)
    {
        const unsigned int num_lengths = 50;
        big_int q1 = big_int::random_with_length(num_lengths);
        big_int q2 = big_int::random_with_length(num_lengths);
        big_int test = q1*q2;
        big_int result = rho_pollard(test);
        if(result == test)
        {
            res = resFAIL;
            tout << "Test fail: " << test << " = " << q1 << '*' << q2 << '\n';
            tout << "Result: " << result << '\n';
        }
        --i;
    }
    return res;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END
}


/* End of file rho_pollard_function.cpp */
