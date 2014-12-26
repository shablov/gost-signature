/*****************************************************************************

    test/bigint_swap.cpp

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

// Author : Aleksandrov Vladimir
// Tester : Sidnev Alexey
// Creation date : 20.01.2006
// Modification date: 30.01.2006
// Testing date: 12.02.2006
// Description :

#include "stdafx.hpp"

//#include <time.h>
//#include <strstream>

using namespace Arageli;



template <class T>
bool big_int_swap (const T A,const T B)
{
    try
    {
        big_int a=A, b=A, d=B, c=B;

        a.swap(d);
        if ((a!=c)||(d!=b))
        {
            tout<<"function swap failed with A="<<A<<", B="<<B<<'\n';
            return true;
        }
        return false;
    }
    catch(Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        return true;
    }
}


bool big_int_swap_test(int param, int count)
{
    bool fail=false;
    RNG gen(param,1<<16);

        fail |= big_int_swap(0,0);

        char x=3;
        char y=4;
        fail |= big_int_swap((signed char) (x),(signed char) (y));
        fail |= big_int_swap((unsigned char) (x),(unsigned char) (y));
        fail |= big_int_swap(x,y);

        if(fail)
        {
            tout<<"Function big_int_swap_test failed on first step.\n";
            return fail;
        }


        for (int k=0; k<count; k++)
        {
            int a=gen.Rand();
            int b=gen.Rand();
            fail |= big_int_swap(a,b);
            fail |= big_int_swap(double (a),double (b));
            fail |= big_int_swap(float (a),float (b));
            fail |= big_int_swap(long (a),long (b));
            fail |= big_int_swap((long double) (a),(long double) (b));
            fail |= big_int_swap((signed short) (a),(signed short) (b));
            fail |= big_int_swap((unsigned short) (a),(unsigned short) (b));
            fail |= big_int_swap((unsigned int) (a),(unsigned int) (b));
            fail |= big_int_swap((signed int) (a),(signed int) (b));

            if(fail)
            {
                tout<<"Function big_int_swap_test failed on "<<k+1<<" step.\n";
                return fail;
            }
        }
        bool a1=1;
        bool a2=1;
        fail |= big_int_swap(a1,a2);

        if(fail)
        {
            tout<<"Function big_int_swap_test failed on last step.\n";
            return fail;
        }

    return false;
}


TEST(big_int,swap,"Test")
{
    bool fail=big_int_swap_test(100,1000);

    if(fail) return resFAIL;
    return resOK;

}
