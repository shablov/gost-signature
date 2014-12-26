/*****************************************************************************

    test/rational_plus_minus.cpp

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
// Description : Test of +, - operations(unary and binary).
//        Be used: rational_plus_minus, plus_minus.

#include "stdafx.hpp"

using namespace Arageli;

bool rational_plus_minus_test(int param, int count)
{
    bool fail=false;
    RNG gen(param);

    for(int k=0;k<count;k++)
    {
        int iper1=gen.Rand(), iper2=gen.Rand();
        big_int biper1=gen.Rand(), biper2=gen.Rand();

        while(!( iper2=gen.Rand() ));
        while(!( biper2=gen.Rand() ));

        //rational<int> ri(iper1,iper2), ri_tem(gen.Rand());
        rational<big_int> rbi(biper1,biper2), rbi_tem(gen.Rand());

        //fail |=t_rational_plus_minus<int,int>(ri, ri_tem);
        fail |=t_rational_plus_minus<big_int,big_int>(rbi, rbi_tem);
        //fail |=t_rational_plus_minus<big_int,int>(rbi, ri);

        //fail |=t_plus_minus<rational<big_int>,rational<int> >(rbi, ri);
        //fail |=t_plus_minus<rational<int>,rational<int> >(ri, ri_tem);
        fail |=t_plus_minus<rational<big_int>,rational<big_int> >(rbi, rbi_tem);

        if(fail)
        {
            tout<<"Function rational_plus_minus_test failed on "<<k+1<<" step.\n";
            return fail;
        }
    }
    return false;
}

TEST(rational,plus_minus,"Test +,- functions.")
{
    bool fail=rational_plus_minus_test(3457,1000);

    if(fail)
        return resFAIL;
    else
        return resOK;
}
