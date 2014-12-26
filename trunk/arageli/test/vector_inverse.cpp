/*****************************************************************************

    test/vector_inverse.cpp

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
// Description : Test of inverse().
//        Be used: inverse, vector_bitwise_not, vector_logical_not.

#include "stdafx.hpp"

using namespace Arageli;

bool vector_inverse_test(int param, int count)
{
    bool fail=false;
    RNG gen(param);

    for(int k=0;k<count;k++)
    {
        int vsize=gen.Rand()%30 + 1;

        Arageli::vector<rational<int> > vri(vsize);
        for(int i=0; i<vsize; i++)
            vri[i]=(rational<int>)(gen.Rand()+1);

        Arageli::vector<rational<big_int> > vrbi(vsize);
        for(int i=0; i<vsize; i++)
            vrbi[i]=(rational<big_int>)(gen.Rand()+1);

        fail |=t_inverse< Arageli::vector<rational<int> > >(vri);
        fail |=t_inverse< Arageli::vector<rational<big_int> > >(vrbi);

        Arageli::vector<int> vi(vsize);
        for(int i=0; i<vsize; i++)
            vi[i]=gen.Rand();

        fail |=t_vector_bitwise_not< int >(vi);

        Arageli::vector<double> vd(vsize);
        for(int i=0; i<vsize; i++)
            vd[i]=gen.Rand();

        Arageli::vector<float> vf(vsize);
        for(int i=0; i<vsize; i++)
            vf[i]=gen.Rand();

        Arageli::vector<big_int> vbi(vsize);
        for(int i=0; i<vsize; i++)
            vbi[i]=gen.Rand();

        fail |=t_vector_logical_not< double >(vd);
        fail |=t_vector_logical_not< int >(vi);
        fail |=t_vector_logical_not< big_int >(vbi);
        fail |=t_vector_logical_not< float >(vf);
        fail |=t_vector_logical_not< rational<int> >(vri);
        fail |=t_vector_logical_not< rational<big_int> >(vrbi);

        if(fail)
        {
            tout<<"Function vector_inverse_test failed on "<<k+1<<" step.\n";
            return fail;
        }
    }
    return false;
}

TEST(vector,inverse_bitwise_not_logical_not,"Test inverse(), bitwise_not(), logical_not() function.")
{
    bool fail=vector_inverse_test(5284,1000);

    if(fail)
        return resFAIL;
    else
        return resOK;
}
