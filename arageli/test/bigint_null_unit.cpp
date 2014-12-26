/*****************************************************************************

    test/bigint_null_unit.cpp

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

//Неверная строка

#include "stdafx.hpp"

//#include <time.h>
//#include <strstream>

using namespace Arageli;



template <class T>
bool big_int_null_unit (const T A)
{
    try
    {
        big_int a=A;

        //if (a.is_null()==a.is_unit()) //Неверная строка
        if (a.is_null()==bool(a))
        {
            tout<<"function null_unit failed with A="<<A<<'\n';
            return true;
        }

        if (a.is_unit()&&(a!=(T)1))
        {
            tout<<"function null_unit failed with A="<<A<<'\n';
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


bool big_int_null_unit_test(int param, int count)
{
    bool fail=false;
    RNG gen(param,1<<16);

    fail |= big_int_null_unit(0);
    fail |= big_int_null_unit(1);

    char x=3;
    fail |= big_int_null_unit((signed char) (x));
    fail |= big_int_null_unit((unsigned char) (x));
    fail |= big_int_null_unit(x);

    if(fail)
        {
            tout<<"Function big_int_null_unit_test failed on first step.\n";
            return fail;
        }

    for (int k=0; k<count; k++)
    {
        int a=gen.Rand();
        fail |= big_int_null_unit(a);
        fail |= big_int_null_unit(double (a));
        fail |= big_int_null_unit(float (a));
        fail |= big_int_null_unit(long (a));
        fail |= big_int_null_unit((long double) (a));
        fail |= big_int_null_unit((signed short) (a));
        fail |= big_int_null_unit((unsigned short) (a));
        fail |= big_int_null_unit((unsigned int) (a));
        fail |= big_int_null_unit((signed int) (a));

        if(fail)
            {
                tout<<"Function big_int_null_unit_test failed on "<<k+1<<" step.\n";
                return fail;
            }
    }

    bool a1=1;
    fail |= big_int_null_unit(a1);

    if(fail)
        {
            tout<<"Function big_int_null_unit_test failed on last step.\n";
            return fail;
        }
    return false;
}



TEST(big_int,null_unit,"Test")
{
    bool fail=big_int_null_unit_test(100,1000);

    if(fail) return resFAIL;
    return resOK;

}
