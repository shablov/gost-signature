/*****************************************************************************

    test/rational_swap.cpp

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
// Description : Test of swap(...).
//        Be used: rational_swap.

#include "stdafx.hpp"

using namespace Arageli;

bool rational_swap_test(int param, int count)
{
    bool fail=false;
    RNG gen(param);

    for(int k=0;k<count;k++)
    {
        int per1=gen.Rand(), per2=gen.Rand();
        big_int bper1=gen.Rand(), bper2=gen.Rand();

        while(!( per2=gen.Rand() ));
        while(!( bper2=gen.Rand() ));

        rational<int> ri1(per1,per2);
        rational<big_int> rbi1(bper1,bper2);

        per1=gen.Rand(), bper1=gen.Rand();

        while(per2==0)
            per2=gen.Rand();

        while(bper2==0)
            bper2=gen.Rand();

        rational<int> ri2(per1,per2);
        rational<big_int> rbi2(bper1,bper2);

        fail |=t_rational_swap<int>(ri1, ri2);
        fail |=t_rational_swap<big_int>(rbi1, rbi2);

        if(fail)
        {
            tout<<"Function rational_swap_test failed on "<<k+1<<" step.\n";
            return fail;
        }
    }
    return false;
}

TEST(rational,swap,"Test swap() function.")
{
    bool fail=rational_swap_test(997,1000);

    if(fail)
        return resFAIL;
    else
        return resOK;
}
