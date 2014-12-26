/*****************************************************************************

    test/pollard_pm1.cpp

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


TEST_FUNCTION(pollard_pm1, "Test pollard_pm1 algorithm.")
{
    using namespace Arageli;
    srand( (unsigned)time( NULL ) );
    TestResult res = resOK;
    int i = 10;
    while(i)
    {
        const unsigned int num_lengths = 30;
        big_int q1 = big_int::random_with_length(num_lengths);
        big_int q2 = big_int::random_with_length(num_lengths);
        big_int test = q1*q2;
        long inter_num = 0;
        big_int result = pollard_pm1(test, inter_num);
        if(result == test || is_null(result))
        {
            res = resFAIL;
            tout << "Test fail: " << test << " = " << q1 << '*' << q2 << '\n';
            tout << "Result: " << result << '\n';
            break;
        }
        --i;
    }
    return res;
}


/* End of file pollard_pm1.cpp */
