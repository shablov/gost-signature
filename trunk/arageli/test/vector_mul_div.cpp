/*****************************************************************************

    test/vector_mul_div.cpp

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
// Description : Test of *, / operations.
//        Be used: mul_div, vector_scalar_mul_div, vector_shlr.

#include "stdafx.hpp"

using namespace Arageli;

bool vector_mul_div_test(int param, int count)
{
    bool fail=false;
    RNG gen(param,1<<15);

    for(int k=0;k<count;k++)
    {
        int vsize=gen.Rand()%30;

        Arageli::vector<rational<int> > vri1(vsize), vri2(vsize);

        for(int i=0; i<vsize; i++)
            vri1[i]=(rational<int>)gen.Rand()+1;

        for(int i=0; i<vsize; i++)
            vri2[i]=(rational<int>)gen.Rand()+1;

        Arageli::vector<rational<big_int> > vrbi1(vsize), vrbi2(vsize);

        for(int i=0; i<vsize; i++)
            vrbi1[i]=(rational<big_int>)gen.Rand()+1;

        for(int i=0; i<vsize; i++)
            vrbi2[i]=(rational<big_int>)gen.Rand()+1;

        fail |=t_mul_div< Arageli::vector<rational<int> >, Arageli::vector<rational<int> > >(vri1, vri2);
        fail |=t_mul_div< Arageli::vector<rational<big_int> >, Arageli::vector<rational<big_int> > >(vrbi1, vrbi2);

        Arageli::vector<int> vi(vsize);
        int j=gen.Rand();

        for(int i=0; i<vsize; i++)
            vi[i]=gen.Rand();

        Arageli::vector<big_int> vbi(vsize);
        big_int bi=gen.Rand();

        for(int i=0; i<vsize; i++)
            vbi[i]=gen.Rand();

        fail |=t_vector_scalar_mul_div< rational<int> >(vri1, (rational<int>)gen.Rand());
        fail |=t_vector_scalar_mul_div< rational<big_int> >(vrbi1, (rational<big_int>)gen.Rand());

        fail |=t_vector_shlr< int >(vi, j%8);
        fail |=t_vector_shlr< big_int >(vbi, bi%100);

        if(fail)
        {
            tout<<"Function vector_mul_div_test failed on "<<k+1<<" step.\n";
            return fail;
        }
    }
    return false;
}

TEST(vector,mul_div,"Test *, /, shl, shr, scalar *, scalar / functions.")
{
    bool fail=vector_mul_div_test(4901,1000);

    if(fail)
        return resFAIL;
    else
        return resOK;
}
