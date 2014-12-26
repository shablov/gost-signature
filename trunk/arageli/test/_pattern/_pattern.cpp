/*****************************************************************************

    test/_pattern.cpp

    This file is a part of the Arageli library test base.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    Copyright (C) 2005--2007 Sergey S. Lyalin
    REFERENCE ADDITIONAL COPYRIGHTS HERE

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
    \file _pattern.cpp
    \brief This file includes test for _pattern.

    <!--ADD ADDITIONAL FILE DESCRIPTION HERE-->
*/


#include "stdafx.hpp"

using namespace Arageli;


namespace
{

// PLACE AUXILIARY CODE HERE

}


// CHOOSE ONE OF THE FOLLOWING THREE HEADERS

TEST
(
    /*CLASS NAME*/,
    /*FUNCTION NAME*/,
    "Test for _pattern."
)
TEST_FUNCTION
(
    /*FUNCTION NAME*/,
    "Test for _pattern function."
)
TEST_CLASS
(
    /*CLASS NAME*/,
    "Test for _pattern class."
)
{
    bool is_ok = true;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN
    {
        // PLACE TEST CODE HERE

        // FOR EXAMPLE:
        // is_ok &= your_test();
    }
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END

    return is_ok ? resOK : resFAIL;
}
