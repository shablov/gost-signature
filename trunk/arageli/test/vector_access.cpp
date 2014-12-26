/*****************************************************************************

    test/vector_access.cpp

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
// Creation date : 18.02.2006
// Modification date: 18.02.2006
// Testing date:
// Description : Test of [], (...), el, at, insert, push, erase functions.
//        Be used: vector_access, vector_set_get.

#include "stdafx.hpp"

using namespace Arageli;

bool vector_access_test(int param, int count)
{
    bool fail=false;
    RNG gen(param);

    for(int k=0;k<count;k++)
    {
        int vsize=gen.Rand()%30;

        Arageli::vector<int> vi(vsize);
        int j=gen.Rand();
        for(int i=0; i<vsize; i++)
            vi[i]=gen.Rand();

        Arageli::vector<big_int> vbi(vsize);
        big_int bi=gen.Rand();
        for(int i=0; i<vsize; i++)
            vbi[i]=gen.Rand();

        Arageli::vector<double> vd(vsize);
        double d=gen.Rand();
        for(int i=0; i<vsize; i++)
            vd[i]=gen.Rand();

        Arageli::vector<float> vf(vsize);
        float f=gen.Rand();
        for(int i=0; i<vsize; i++)
            vf[i]=gen.Rand();

        Arageli::vector<rational<int> > vri(vsize);
        rational<int> ri=gen.Rand();
        for(int i=0; i<vsize; i++)
            vri[i]=(rational<int>)gen.Rand();

        Arageli::vector<rational<big_int> > vrbi(vsize);
        rational<big_int> rbi=gen.Rand();
        for(int i=0; i<vsize; i++)
            vrbi[i]=(rational<big_int>)gen.Rand();

        fail |=t_vector_access< rational<int> >(vri);
        fail |=t_vector_access< rational<big_int> >(vrbi);
        fail |=t_vector_access< int >(vi);
        fail |=t_vector_access< big_int >(vbi);
        fail |=t_vector_access< double >(vd);
        fail |=t_vector_access< float >(vf);

        fail |=t_vector_set_get< rational<int> >(vri, ri);
        fail |=t_vector_set_get< rational<big_int> >(vrbi, rbi);
        fail |=t_vector_set_get< int >(vi, j);
        fail |=t_vector_set_get< big_int >(vbi, bi);
        fail |=t_vector_set_get< double >(vd, d);
        fail |=t_vector_set_get< float >(vf, f);

        if(fail)
        {
            tout<<"Function vector_plus_minus_test failed on "<<k+1<<" step.\n";
            return fail;
        }
    }
    return false;
}

TEST(vector,access,"Test [], (...), el, at, insert, push, erase functions.")
{
    bool fail=vector_access_test(8542,1000);

    if(fail)
        return resFAIL;
    else
        return resOK;
}
