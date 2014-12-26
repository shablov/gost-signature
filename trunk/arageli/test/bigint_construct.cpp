/*****************************************************************************

    test/bigint_construct.cpp

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

//Исправлено !c на c


#include "stdafx.hpp"

//#include <time.h>
//#include <strstream>

using namespace Arageli;

template <class T>
bool big_int_construct (const T A)
{
    try
    {
        big_int c;
        big_int a=A;
        big_int b=a;

        if (a!=b)
        {
            tout<<"function construct failed on 1 check with A="<<A<<'\n';
            return true;
        }
        if (c)
        {
            tout<<"function construct failed on 2 check with A="<<A<<'\n';
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

bool big_int_construct_test(int param, int count)
{
    bool fail=false;
    RNG gen(param,1<<16);

    fail |= big_int_construct(0);

    char x=3;
    fail |= big_int_construct((signed char) (x));
    fail |= big_int_construct((unsigned char) (x));
    fail |= big_int_construct(x);

    if(fail)
        {
            tout<<"Function big_int_construct_test failed on first step.\n";
            return fail;
        }

    for (int k=0; k<count; k++)
    {
        int a=gen.Rand();
        fail |= big_int_construct(a);
        fail |= big_int_construct(double (a));
        fail |= big_int_construct(float (a));
        fail |= big_int_construct(long (a));
        fail |= big_int_construct((long double) (a));
        fail |= big_int_construct((signed short) (a));
        fail |= big_int_construct((unsigned short) (a));
        fail |= big_int_construct((unsigned int) (a));
        fail |= big_int_construct((signed int) (a));

        if(fail)
            {
                tout<<"Function big_int_construct_test failed on "<<k+1<<" step.\n";
                return fail;
            }
    }

    bool a1=1;
    fail |= big_int_construct(a1);

    if(fail)
        {
            tout<<"Function big_int_construct_test failed on last step.\n";
            return fail;
        }
    return false;
}



TEST(big_int,construct,"Test")
{
    bool fail=big_int_construct_test(100,1000);

    if(fail) return resFAIL;
    return resOK;

}
