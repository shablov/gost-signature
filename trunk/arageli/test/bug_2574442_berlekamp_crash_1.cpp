/*****************************************************************************

    test/bug_2574442_berlekamp_crash_1.cpp

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
    \file bug_2574442_berlekamp_crash_1.cpp
    \brief This file includes test for bug #2574442, part 1.

    This is a test for bug #2574442: Error in residue_berlekamp_router(...)
    function. It tests a couple of features inside sparse_polynom and
    berlekamp implementation itself. Part 1.
*/


#include "stdafx.hpp"

using namespace Arageli;


namespace
{

// PLACE AUXILIARY CODE HERE

}


TEST_FUNCTION
(
    residue_berlekamp_router_bug_2574442_1,
    "Test for bug #2574442: Error in residue_berlekamp_router (part 1)."
)
{
    bool is_ok = true;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN
    {
        {
            sparse_polynom<big_int> g = "x^2-x+5";
            vector<sparse_polynom<big_int> > vp;
            Arageli::factorize_berlekamp(g, (big_int)5, vp);
        }
    }
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END

    return is_ok ? resOK : resFAIL;
}
