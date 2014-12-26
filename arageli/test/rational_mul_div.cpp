/*****************************************************************************

    test/rational_mul_div.cpp

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
// Creation date : 1.02.2006
// Modification date: 1.02.2006
// Testing date:
// Description : Test of *, / operations.
//        Be used: rational_mul_div, mul_div.


#include "stdafx.hpp"

using namespace Arageli;


bool rational_mul_div_test(int param, int count)
{
    bool fail=false;
    RNG gen(param);

    for(int k=0;k<count;k++)
    {
        int iper1=gen.Rand(), iper2=gen.Rand();
        big_int biper1=gen.Rand(), biper2=gen.Rand();

        while(!( iper2=gen.Rand() ));
        while(!( biper2=gen.Rand() ));

        //rational<int> ri1(iper1,iper2);
        rational<big_int> rbi1(biper1,biper2);

        while(!( iper2=gen.Rand() ));
        while(!( biper2=gen.Rand() ));

        //rational<int> ri2(gen.Rand(),iper2);
        rational<big_int> rbi2(gen.Rand(),biper2);

        //fail |=t_rational_mul_div<int,int>(ri1, ri2);
        fail |=t_rational_mul_div<big_int,big_int>(rbi2, rbi2);
        //fail |=t_rational_mul_div<big_int,int>(rbi1, ri1);

        //fail |=t_mul_div<rational<big_int>,rational<int> >(rbi1, ri1);
        //fail |=t_mul_div<rational<int>,rational<int> >(ri1, ri2);
        fail |=t_mul_div<rational<big_int>,rational<big_int> >(rbi1, rbi2);

        if(fail)
        {
            tout<<"Function rational_mul_div_test failed on "<<k+1<<" step.\n";
            return fail;
        }
    }
    return false;
}


TEST(rational,mul_div,"Test *,/ functions.")
{
    bool fail=rational_mul_div_test(3456,1000);

/*
    rational<int> a(4,44);
    rational<big_int> b(11,8);
    b/a;
*/

    if(fail)
        return resFAIL;
    else
        return resOK;
}
