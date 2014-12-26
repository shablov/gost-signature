/*****************************************************************************

    test/vector_swap.cpp

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
// Modification date: 06.02.2006
// Testing date:
// Description : Test swap functions.
//        Be used: vector_swap, swap.

#include "stdafx.hpp"

using namespace Arageli;

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4244)
#endif

bool vector_swap_test(int param, int count)
{
    bool fail=false;
    RNG gen(param);

    for(int k=0;k<count;k++)
    {
        int vsize=gen.Rand()%15;

        Arageli::vector<int> vi1(vsize), vi2(vsize);
        int i=gen.Rand();

        for(int i=0; i<vsize; i++)
        {
            vi1[i]=gen.Rand();
            vi2[i]=gen.Rand();
        }

        Arageli::vector<big_int> vbi1(vsize), vbi2(vsize);
        big_int bi=gen.Rand();

        for(int i=0; i<vsize; i++)
        {
            vbi1[i]=gen.Rand();
            vbi2[i]=gen.Rand();
        }

        Arageli::vector<double> vd1(vsize), vd2(vsize);
        double d=gen.Rand();

        for(int i=0; i<vsize; i++)
        {
            vd1[i]=gen.Rand();
            vd2[i]=gen.Rand();
        }

        Arageli::vector<float> vf1(vsize), vf2(vsize);
        float f=gen.Rand();

        for(int i=0; i<vsize; i++)
        {
            vf1[i]=gen.Rand();
            vf2[i]=gen.Rand();
        }

        fail |=t_swap< Arageli::vector<int> >(vi1, vi2);
        fail |=t_swap< Arageli::vector<big_int> >(vbi1, vbi2);
        fail |=t_swap< Arageli::vector<float> >(vf1, vf2);
        fail |=t_swap< Arageli::vector<double> >(vd1, vd2);

        if(vsize)
        {
            fail |=t_vector_swap< int >(vi1, gen.Rand()%vsize, gen.Rand()%vsize);
            fail |=t_vector_swap< big_int >(vbi1, gen.Rand()%vsize, gen.Rand()%vsize);
            fail |=t_vector_swap< float >(vf1, gen.Rand()%vsize, gen.Rand()%vsize);
            fail |=t_vector_swap< double >(vd1, gen.Rand()%vsize, gen.Rand()%vsize);
        }

        Arageli::vector<rational<int> > vri1(vsize), vri2(vsize);
        rational<int> ri(gen.Rand());

        for(int i=0; i<vsize; i++)
        {
            vri1[i]=(rational<int>)gen.Rand();
            vri2[i]=(rational<int>)gen.Rand();
        }

        Arageli::vector<rational<big_int> > vrbi1(vsize), vrbi2(vsize);
        rational<big_int> rbi(gen.Rand());

        for(int i=0; i<vsize; i++)
        {
            vrbi1[i]=(rational<big_int>)gen.Rand();
            vrbi2[i]=(rational<big_int>)gen.Rand();
        }


        fail |=t_swap< Arageli::vector<rational<int> > >(vri1, vri2);
        fail |=t_swap< Arageli::vector<rational<big_int> > >(vrbi1, vrbi2);

        if(vsize)
        {
            fail |=t_vector_swap< rational<int> >(vri1, gen.Rand()%vsize, gen.Rand()%vsize);
            fail |=t_vector_swap< rational<big_int> >(vrbi1, gen.Rand()%vsize, gen.Rand()%vsize);
        }

        if(fail)
        {
            tout<<"Function vector_swap_test failed on "<<k+1<<" step.\n";
            return fail;
        }
    }
    return false;
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

TEST(vector,swap,"Test swap functions.")
{
    bool fail=vector_swap_test(2333,1000);

    if(fail)
        return resFAIL;
    else
        return resOK;
}
