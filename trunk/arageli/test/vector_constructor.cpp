/*****************************************************************************

    test/vector_constructor.cpp

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
// Creation date : 03.02.2006
// Modification date: 18.02.2006
// Testing date:
// Description : Test of constructors, operator=, type cast.
//        Be used: copy_constructor, copy, vector_resize.

#include "stdafx.hpp"

using namespace Arageli;

bool vector_constructor_test(int param, int count)
{
    bool fail=false;
    RNG gen(param);

    fail |=t_vector_zero_constructor<int>();
    fail |=t_vector_zero_constructor<double>();
    fail |=t_vector_zero_constructor<big_int>();
    fail |=t_vector_zero_constructor<float>();
    fail |=t_vector_zero_constructor< rational<int> >();
    fail |=t_vector_zero_constructor< rational<big_int> >();

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

        fail |=t_copy< Arageli::vector<int> >(&vi1, &vi2);
        fail |=t_copy< Arageli::vector<big_int> >(&vbi1, &vbi2);
        fail |=t_copy< Arageli::vector<double> >(&vd1, &vd2);
        fail |=t_copy< Arageli::vector<float> >(&vf1, &vf2);

        fail |=t_copy_constructor< Arageli::vector<int> >(&vi1);
        fail |=t_copy_constructor< Arageli::vector<big_int> >(&vbi1);
        fail |=t_copy_constructor< Arageli::vector<double> >(&vd1);
        fail |=t_copy_constructor< Arageli::vector<float> >(&vf1);

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

        fail |=t_copy< Arageli::vector<rational<int> > >(&vri1, &vri2);
        fail |=t_copy< Arageli::vector<rational<big_int> > >(&vrbi1, &vrbi2);

        fail |=t_copy_constructor< Arageli::vector<rational<int> > >(&vri1);
        fail |=t_copy_constructor< Arageli::vector<rational<big_int> > >(&vrbi1);

        fail |=t_vector_resize< rational<int> >(vri1, (rational<int>)gen.Rand(), gen.Rand()%20);
        fail |=t_vector_resize< rational<big_int> >(vrbi1, (rational<big_int>)gen.Rand(), gen.Rand()%20);
        fail |=t_vector_resize< int >(vi1, gen.Rand(), gen.Rand()%20);
        fail |=t_vector_resize< big_int >(vbi1, gen.Rand(), gen.Rand()%20);
        fail |=t_vector_resize< double >(vd1, gen.Rand(), gen.Rand()%20);
        fail |=t_vector_resize< float >(vf1, gen.Rand(), gen.Rand()%20);

        if(fail)
        {
            tout<<"Function vector_constructor_test failed on "<<k+1<<" step.\n";
            return fail;
        }
    }
    return false;
}

TEST(vector,constructor_resize,"Test constructors and resize functions.")
{
    bool fail=vector_constructor_test(844,1000);

    if(fail)
        return resFAIL;
    else
        return resOK;
}
