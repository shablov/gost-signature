/*****************************************************************************

    test/rational_is_null.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
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

// Author : Sidnev Alexey
// Tester :
// Creation date : 20.01.2006
// Modification date: 01.02.2006
// Testing date:
// Description : Test of is_null().
//        Be used: rational_is_null.

#include "stdafx.hpp"

using namespace Arageli;

bool rational_is_null_test(int param, int count)
{
    int k;
    try
    {
        bool fail=false;
        RNG gen(param);

        for(k=0;k<count;k++)
        {
            int per1=gen.Rand(), per2=gen.Rand();
            big_int bper1=gen.Rand(), bper2=gen.Rand();

            while(!( per2=gen.Rand() ));
            while(!( bper2=gen.Rand() ));

            rational<int> ri(per1,per2);
            rational<big_int> rbi(bper1,bper2);

            fail |=t_rational_is_null<int>(&ri);
            fail |=t_rational_is_null<big_int>(&rbi);

            if(fail)
            {
                tout<<"Function rational_is_null_test failed on "<<k+1<<" step.\n";
                return fail;
            }
        }
        return false;
    }
    catch(...)
    {
        tout<<"Function rational_is_null_test failed on "<<k+1<<" step this EXCEPTION.\n";
        return true;
    }
}

TEST(rational,is_null,"Test is_null() function.")
{
    bool fail=rational_is_null_test(9234,1000);

    if(fail)
        return resFAIL;
    else
        return resOK;
}
