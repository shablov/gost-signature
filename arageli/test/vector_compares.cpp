/*****************************************************************************

    test/vector_compares.cpp

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
// Creation date : 31.01.2006
// Modification date: 05.02.2006
// Testing date:
// Description : Test compare functions: >, <, ==, !=, >=, <=, cmp.
//        Be used: compares.

#include "stdafx.hpp"

using namespace Arageli;

bool vector_compares_test(int param, int count)
{
    bool fail=false;
    RNG gen(param);

    for(int k=0;k<count;k++)
    {
        int v1size=gen.Rand()%30,
            v2size=gen.Rand()%30;
        Arageli::vector<int> vi1(v1size), vi2(v2size);

        for(int i=0; i<v1size; i++)
            vi1[i]=gen.Rand();

        for(int i=0; i<v2size; i++)
            vi2[i]=gen.Rand();

        Arageli::vector<big_int> vbi1(v1size), vbi2(v2size);

        for(int i=0; i<v1size; i++)
            vbi1[i]=gen.Rand();

        for(int i=0; i<v2size; i++)
            vbi2[i]=gen.Rand();

        Arageli::vector<double> vd1(v1size), vd2(v2size);

        for(int i=0; i<v1size; i++)
            vd1[i]=gen.Rand();

        for(int i=0; i<v2size; i++)
            vd2[i]=gen.Rand();

        Arageli::vector<float> vf1(v1size), vf2(v2size);

        for(int i=0; i<v1size; i++)
            vf1[i]=gen.Rand();

        for(int i=0; i<v2size; i++)
            vf2[i]=gen.Rand();

        fail |=t_compares< Arageli::vector<int> >(&vi1, &vi2);
        fail |=t_compares< Arageli::vector<big_int> >(&vbi1, &vbi2);
        fail |=t_compares< Arageli::vector<double> >(&vd1, &vd2);
        fail |=t_compares< Arageli::vector<float> >(&vf1, &vf2);

        Arageli::vector<rational<int> > vri1(v1size), vri2(v2size);

        for(int i=0; i<v1size; i++)
            vri1[i]=(rational<int>)gen.Rand();

        for(int i=0; i<v2size; i++)
            vri2[i]=(rational<int>)gen.Rand();

        Arageli::vector<rational<big_int> > vrbi1(v1size), vrbi2(v2size);

        for(int i=0; i<v1size; i++)
            vrbi1[i]=(rational<big_int>)gen.Rand();

        for(int i=0; i<v2size; i++)
            vrbi2[i]=(rational<big_int>)gen.Rand();

        fail |=t_compares< Arageli::vector<rational<int> > >(&vri1, &vri2);
        fail |=t_compares< Arageli::vector<rational<big_int> > >(&vrbi1, &vrbi2);

        if(fail)
        {
            tout<<"Function vector_compares_test failed on "<<k+1<<" step.\n";
            return fail;
        }
    }
    return false;
}

TEST(vector,compares,"Test compare functions.")
{
    bool fail=vector_compares_test(325,1000);

    if(fail)
        return resFAIL;
    else
        return resOK;
}
