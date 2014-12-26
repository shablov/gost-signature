/*****************************************************************************

    test/rational_constructor.cpp

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
// Modification date: 1.02.2006
// Testing date:
// Description : Test of constructors, operator=, type cast.
//        Be used: rational_zero_constructor, copy_constructor, rational_constructor,
//                 copy, rational_copy, rational_operator_double_bool.

#include "stdafx.hpp"

using namespace Arageli;

bool rational_constructor_test(int param, int count)
{
    bool fail=false;
    RNG gen(param);

    fail |=t_rational_zero_constructor<int>();
    fail |=t_rational_zero_constructor<big_int>();
    fail |=t_rational_zero_constructor<rational<int> >();
    fail |=t_rational_zero_constructor<rational<big_int> >();

    if(fail)
    {
        tout<<"Function rational_constructor_test failed on "<<0<<" step.\n";
        return fail;
    }

    for(int k=0;k<count;k++)
    {
        float fper=(float)gen.Rand();
        double dper=(double)gen.Rand();
        int iper1=gen.Rand(), iper2=gen.Rand();
        char cper=gen.Rand();
        big_int biper1=gen.Rand(), biper2=gen.Rand();

        while(!( iper2=gen.Rand() ));
        while(!( biper2=gen.Rand() ));

        rational<int> ri(iper1,iper2), ri_tem(gen.Rand());
        rational<big_int> rbi(biper1,biper2), rbi_tem(gen.Rand());

        fail |=t_copy_constructor< rational<int> >(&ri);
        fail |=t_copy_constructor< rational<big_int> >(&rbi);
        fail |=t_copy_constructor< rational<int> >(&ri_tem);
        fail |=t_copy_constructor< rational<big_int> >(&rbi_tem);

        fail |=t_rational_constructor<int>(&iper1, &iper2);
        fail |=t_rational_constructor<big_int>(&biper1, &biper2);

        fail |=t_copy< rational<int> >(&ri, &ri_tem);
        fail |=t_copy< rational<big_int> >(&rbi, &rbi_tem);

        fail |=t_rational_operator_double_bool<int>(ri);
        fail |=t_rational_operator_double_bool<big_int>(rbi);
        fail |=t_rational_operator_double_bool<int>(ri_tem);
        fail |=t_rational_operator_double_bool<big_int>(rbi_tem);

        fail |=t_rational_copy<big_int, int>(ri, biper1);
        fail |=t_rational_copy<big_int, double>(rbi, dper);
        fail |=t_rational_copy<big_int, float>(rbi, fper);
        fail |=t_rational_copy<int, big_int>(rbi, iper1);
        //fail |=t_rational_copy<int, double>(ri, dper);
        //fail |=t_rational_copy<int, float>(ri, fper);
        //fail |=t_rational_copy<int, char>(ri, cper);
        //fail |=t_rational_copy<big_int, char>(rbi, cper);

        if(fail)
        {
            tout<<"Function rational_constructor_test failed on "<<k+1<<" step.\n";
            return fail;
        }

        while(!( iper1=gen.Rand() ));
        while(!( iper2=gen.Rand() ));
        rational<int> ri3(iper1,iper2);

        while(!( iper1=gen.Rand() ));
        while(!( iper2=gen.Rand() ));
        rational<int> ri4(iper1,iper2);

        //rational<rational<int> > rri1(ri, ri3), rri2(ri_tem, ri4);
        //fail |=t_rational_copy< rational<rational<int> >, rational<int> >(rri1, ri);
        //fail |=t_rational_copy< rational<rational<int> >, rational<int> >(rri1, ri_tem);
        //fail |=t_rational_copy< rational<rational<int> >, rational<big_int> >(rri1, rbi_tem);
        //fail |=t_rational_copy< rational<rational<int> >, rational<big_int> >(rri1, rbi);
        //fail |=t_rational_operator_double_bool<rational<int> >(rri1);
        //fail |=t_rational_operator_double_bool<rational<int> >(rri2);
        //fail |=t_copy< rational<rational<int> > >(&rri1, &rri2);
        //fail |=t_rational_constructor<rational<int> >(&ri, &ri_tem);
        //fail |=t_copy_constructor< rational<rational<int> > >(&rri1);
        //fail |=t_copy_constructor< rational<rational<int> > >(&rri2);

        while(!( biper1=gen.Rand() ));
        while(!( biper2=gen.Rand() ));
        rational<big_int> rbi3(biper1,biper2);

        while(!( biper1=gen.Rand() ));
        while(!( biper2=gen.Rand() ));
        rational<big_int> rbi4(biper1,biper2);

        //rational<rational<big_int> > rrbi1(rbi, rbi3), rrbi2(rbi_tem, rbi4);
        //fail |=t_rational_copy< rational<rational<big_int> >, rational<big_int> >(rrbi1, rbi);
        //fail |=t_rational_copy< rational<rational<big_int> >, rational<big_int> >(rrbi1, rbi_tem);
        //fail |=t_rational_copy< rational<rational<big_int> >, rational<int> >(rrbi1, ri_tem);
        //fail |=t_rational_copy< rational<rational<big_int> >, rational<int> >(rrbi1, ri);
        //fail |=t_rational_operator_double_bool<rational<big_int> >(rrbi1);
        //fail |=t_rational_operator_double_bool<rational<big_int> >(rrbi2);
        //fail |=t_copy< rational<rational<big_int> > >(&rrbi1, &rrbi2);
        //fail |=t_rational_constructor<rational<big_int> >(&rbi, &rbi_tem);
        //fail |=t_copy_constructor< rational<rational<big_int> > >(&rrbi1);
        //fail |=t_copy_constructor< rational<rational<big_int> > >(&rrbi2);


        if(fail)
        {
            tout<<"Function rational_constructor_test failed on "<<k+1<<" step.\n";
            return fail;
        }
    }
    return false;
}

TEST(rational,constructor,"Test constructors.")
{
    bool fail=rational_constructor_test(844,1000);

    /*
    rational<int> b(3,4);
    char c=(char)b;
    */

    if(fail)
        return resFAIL;
    else
        return resOK;
}
