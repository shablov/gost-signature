/*****************************************************************************

    test/vector_is_unit.cpp

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
// Creation date : 15.02.2006
// Modification date: 16.02.2006
// Testing date:
// Description : Test of is_unit.
//        Be used: vector_is_unit.

#include "stdafx.hpp"

using namespace Arageli;

bool vector_is_unit_test(int param, int count)
{
    bool fail=false;
    RNG gen(param,4);

    for(int k=0;k<count;k++)
    {
        int vsize=gen.Rand()%15;

        Arageli::vector<int> vi(vsize);
        for(int i=0; i<vsize; i++)
            vi[i]=gen.Rand();

        Arageli::vector<big_int> vbi(vsize);
        for(int i=0; i<vsize; i++)
            vbi[i]=gen.Rand();

        Arageli::vector<double> vd(vsize);
        for(int i=0; i<vsize; i++)
            vd[i]=gen.Rand();

        Arageli::vector<float> vf(vsize);
        for(int i=0; i<vsize; i++)
            vf[i]=gen.Rand();


        fail |=t_vector_is_unit< int >(vi);
        fail |=t_vector_is_unit< big_int >(vbi);
        fail |=t_vector_is_unit< double >(vd);
        fail |=t_vector_is_unit< float >(vf);

        Arageli::vector<rational<int> > vri(vsize);
        for(int i=0; i<vsize; i++)
            vri[i]=(rational<int>)gen.Rand();


        Arageli::vector<rational<big_int> > vrbi(vsize);
        for(int i=0; i<vsize; i++)
            vrbi[i]=(rational<big_int>)gen.Rand();

        fail |=t_vector_is_unit< rational<int> >(vri);
        fail |=t_vector_is_unit< rational<big_int> >(vrbi);

        if(fail)
        {
            tout<<"Function vector_plus_minus_test failed on "<<k+1<<" step.\n";
            return fail;
        }
    }
    return false;
}

TEST(vector,is_unit_is_opposite_unit,"Test is_unit(), is_opposite_unit() functions.")
{
    bool fail=vector_is_unit_test(35731,1000);

    if(fail)
        return resFAIL;
    else
        return resOK;
}
