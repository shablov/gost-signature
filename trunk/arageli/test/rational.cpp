/*****************************************************************************

    test/rational.cpp

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
// Description : Test of all member functions of class rational.
//        Be used: all functions from rational.hpp and universal.hpp.

#include "stdafx.hpp"

typedef void* (*Generate)(big_int arg);

#define RATIONAL_CASE_COUNT 20

template <typename type>
bool t_rational(big_int arg, big_int count, Generate generate)
{
    RNG gen(arg);
    big_int step=0;
    bool fail=false;

    fail |= t_rational_zero_constructor<type>();

    big_int ind=gen.Rand();
    rational<type>
        per1=*(rational<type>*)generate(ind),
        per2=*(rational<type>*)generate(ind+1),
        temPer;

    if(fail)
    {
        tout<<"Function t_rational failed on first step\n";
        return fail;
    }

    for(big_int i=0; i<count; i++ )
    {
        step++;
        switch(int(ind)%RATIONAL_CASE_COUNT)
        {
        case 0:
            fail |= t_rational_is_null<type>(&per1);
            fail |= t_rational_is_null<type>(&per2);
            break;

        case 1:
            fail |= t_rational_plus_minus<type>(per1,per2);
            break;

        case 2:
            fail |= t_rational_is_normal<type>(per1);
            fail |= t_rational_is_normal<type>(per2);
            break;

        case 3:
            fail |= t_compares<rational<type> >(&per1,&per2);
            break;

        case 4:
            fail |= t_copy<rational<type> >(&per2,&temPer);
            fail |= t_copy<rational<type> >(&per1,&per2);
            fail |= t_copy<rational<type> >(&temPer,&per2);

            fail |= t_compares<rational<type> >(&temPer,&per2);
            break;

        case 5:
            per1=*(rational<type>*)generate(ind);
            per2=*((rational<type>*)generate(ind));
            break;

        case 6:
            fail |= t_rational_numerator_denominator<type>(per1.numerator(),per1.denominator());
            fail |= t_rational_numerator_denominator<type>(per2.numerator(),per2.denominator());
            break;

        case 7:
            fail |= t_rational_constructor<type>(&per1.numerator(),&per1.denominator());
            fail |= t_rational_constructor<type>(&per2.numerator(),&per2.denominator());
            break;

        case 8:
            fail |= t_rational_operator_double_bool<type>(per1);
            fail |= t_rational_operator_double_bool<type>(per2);
            break;

        case 9:
            fail |= t_rational_is_integer<type>(per1);
            fail |= t_rational_is_integer<type>(per2);
            break;

        case 10:
            fail |= t_rational_is_unit<type>(per1);
            fail |= t_rational_is_unit<type>(per2);
            break;

        case 11:
            fail |= t_rational_swap<type>(per1, per2);
            break;

        case 12:
            fail |= t_plus_minus<rational<type> >(per1, per2);
            break;

        case 13:
            fail |= t_copy_constructor<rational<type> >(&per1);
            fail |= t_copy_constructor<rational<type> >(&per2);
            break;

        case 14:
            fail |= t_opposite<rational<type> >(per1);
            fail |= t_opposite<rational<type> >(per2);
            break;

        case 15:
            fail |= t_inverse<rational<type> >(per1);
            fail |= t_inverse<rational<type> >(per2);
            break;

        case 16:
            fail |= t_sign<rational<type> >(per1);
            fail |= t_sign<rational<type> >(per2);
            break;

        case 17:
            fail |= t_mul_div<rational<type>, rational<type> >(per1, per2);
            break;

        case 18:
            fail |= t_rational_mul_div<type ,type>(per1, per2);
            break;

        case 19:
            fail |= t_rational_ifloor_iceil_frac_abs<type>(per1);
            fail |= t_rational_ifloor_iceil_frac_abs<type>(per2);
            break;
        }

        if(fail)
        {
            tout<<"Function t_rational failed on "<<step<<" step,on case "<<ind%RATIONAL_CASE_COUNT<<'\n';
            return fail;
        }

        ind=gen.Rand();
    }

    return fail;
}

rational<big_int> tem_per;

void* RationalIntGenerate(big_int arg)
{
    RNG gen(arg);
    big_int per=gen.Rand();
    while(!( per=gen.Rand() ));

    rational<big_int> b(gen.Rand(),per);
    tem_per=b;
    return &tem_per;
}

TEST(rational,all,"Test all functions.")
{
    bool fail=t_rational<big_int>(344,1000,RationalIntGenerate);

    if(fail)
        return resFAIL;
    else
        return resOK;
}
