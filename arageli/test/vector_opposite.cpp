/*****************************************************************************

    test/vector_opposite.cpp

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
// Description : Test of opposite().
//        Be used: opposite.

#include "stdafx.hpp"

using namespace Arageli;

bool vector_opposite_test(int param, int count)
{
    bool fail=false;
    RNG gen(param);

    for(int k=0;k<count;k++)
    {
        int vsize=gen.Rand()%30;
        Arageli::vector<int> vi(vsize);

        for(int i=0; i<vsize; i++)
            vi[i]=1+gen.Rand();

        Arageli::vector<big_int> vbi(vsize);

        for(int i=0; i<vsize; i++)
            vbi[i]=1+gen.Rand();

        Arageli::vector<double> vd(vsize);

        for(int i=0; i<vsize; i++)
            vd[i]=gen.Rand()+1.0;

        Arageli::vector<float> vf(vsize);

        for(int i=0; i<vsize; i++)
            vf[i]=gen.Rand()+1.0;

        fail |=t_opposite< Arageli::vector<int> >(vi);
        fail |=t_opposite< Arageli::vector<big_int> >(vbi);
        fail |=t_opposite< Arageli::vector<double> >(vd);
        fail |=t_opposite< Arageli::vector<float> >(vf);

        Arageli::vector<rational<int> > vri(vsize);

        for(int i=0; i<vsize; i++)
            vri[i]=(rational<int>)(gen.Rand()+1);


        Arageli::vector<rational<big_int> > vrbi(vsize);

        for(int i=0; i<vsize; i++)
            vrbi[i]=(rational<big_int>)(gen.Rand()+1);

        fail |=t_opposite< Arageli::vector<rational<int> > >(vri);
        fail |=t_opposite< Arageli::vector<rational<big_int> > >(vrbi);


        if(fail)
        {
            tout<<"Function vector_opposite_test failed on "<<k+1<<" step.\n";
            return fail;
        }
    }
    return false;
}

TEST(vector,opposite,"Test opposite() function.")
{
    bool fail=vector_opposite_test(3426,1000);

    if(fail)
        return resFAIL;
    else
        return resOK;
}
