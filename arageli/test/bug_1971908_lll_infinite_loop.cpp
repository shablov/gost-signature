/*****************************************************************************

    test/bug_1971908_lll_infinite_loop.cpp

    This file is a part of the Arageli library test base.

    Copyright (C) 2010 Sergey S. Lyalin

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
    \file bug_1971908_lll_infinite_loop.cpp
    \brief This file contains test for bug #1971908.

    This file contains code that triggered bug #1971908: Infinite looping in
    lll_reduction_int() function.
*/


#include "stdafx.hpp"

using namespace Arageli;


namespace
{

// PLACE AUXILIARY CODE HERE

}


TEST_FUNCTION
(
    lll_reduction_int_bug_1971908_lll_infinite_loop,
    "Test for bug inside lll_reduction_int (bug_1971908_lll_infinite_loop)."
)
{
    bool is_ok = true;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN
    {
        // If the bug exists, uncomment the following line to prevent test system from hanging.
        //return resHANG;

        {
            matrix<int> res(3);
            matrix<int> a = "((49, 48, 0), (0, 1, 48), (0, 0, 1))";
            lll_reduction_int(a, res);  // if bug exists, we will hang inside this call
        }
    }
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END

    return is_ok ? resOK : resFAIL;
}
