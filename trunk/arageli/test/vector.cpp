/*****************************************************************************

    test/vector.cpp

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
// Modification date: 12.02.2006
// Testing date:
// Description : Test of all member functions of class rational.
//        Be used: all functions from vector.hpp and universal.hpp.

#include "stdafx.hpp"

typedef void* (*Generate)(int arg);

#define VECTOR_CASE_COUNT 14


template <typename VType>
VType VectorIntGenerate (int arg)
{
    RNG gen(arg);
    int vsize=gen.Rand()%30;
    VType vi(vsize);

    for(int i=0; i<vsize; i++)
        vi[i]=1+gen.Rand();

    return vi;
}


template <typename type>
bool t_vector(int arg, int count)
{
    RNG gen(arg);
    int step=0;
    bool fail=false;

    fail |= t_vector_zero_constructor<type>();

    int ind=gen.Rand();

    Arageli::vector<type>
        per1=VectorIntGenerate<Arageli::vector<type> >(ind),
        per2=VectorIntGenerate<Arageli::vector<type> >(ind),
        temPer;

    if(fail)
    {
        tout<<"Function t_vector failed on first step\n";
        return fail;
    }

    for(int i=0; i<count; i++ )
    {
        step++;
        switch(ind%VECTOR_CASE_COUNT)
        {
        case 0:
            fail |=t_vector_is_unit< type >(per1);
            fail |=t_vector_is_unit< type >(per2);
            break;

        case 1:
            fail |= t_vector_plus_minus<type>(per1,per2);
            fail |=t_vector_scalar_plus_minus< type >(per1, gen.Rand());
            break;

        case 2:
            fail |= t_mul_div<Arageli::vector<type>, Arageli::vector<type> >(per1, per2);
            break;

        case 3:
            fail |= t_compares<Arageli::vector<type> >(&per1,&per2);
            break;

        case 4:
            fail |= t_copy<Arageli::vector<type> >(&per2,&temPer);
            fail |= t_copy<Arageli::vector<type> >(&per1,&per2);
            fail |= t_copy<Arageli::vector<type> >(&temPer,&per2);

            fail |= t_compares<Arageli::vector<type> >(&temPer,&per2);
            break;

        case 5:
            per1=VectorIntGenerate<Arageli::vector<type> >(ind);
            per2=VectorIntGenerate<Arageli::vector<type> >(ind);
            break;

        case 6:
            fail |= t_plus_minus<Arageli::vector<type> >(per1, per2);
            break;

        case 7:
            fail |= t_copy_constructor<Arageli::vector<type> >(&per1);
            fail |= t_copy_constructor<Arageli::vector<type> >(&per2);
            break;

        case 8:
            fail |= t_opposite<Arageli::vector<type> >(per1);
            fail |= t_opposite<Arageli::vector<type> >(per2);
            break;

        case 9:
            fail |= t_vector_access<type>(per1);
            fail |= t_vector_set_get<type>(per1, gen.Rand());
            break;

        case 10:
            fail |=t_vector_resize< type >(per1, gen.Rand(), gen.Rand()%20);
            fail |=t_vector_resize< type >(per2, gen.Rand(), gen.Rand()%20);
            fail |=t_copy_constructor< Arageli::vector<type> >(&per1);
            fail |=t_copy_constructor< Arageli::vector<type> >(&per2);
            break;

        case 11:
            fail |=t_swap< Arageli::vector<type> >(per1, per2);
            fail |=t_vector_swap< type >(per1, gen.Rand()%per1.size(), gen.Rand()%per1.size());
            break;

        case 12:
            fail |=t_vector_is_null< type >(&per1);
            fail |=t_vector_is_null< type >(&per2);
            fail |=t_vector_is_empty< type >(&per1);
            fail |=t_vector_is_empty< type >(&per2);
            break;

        case 13:
            fail |=t_inverse< Arageli::vector<rational<type> > >(per1);
            fail |=t_inverse< Arageli::vector<rational<type> > >(per2);
            fail |=t_vector_logical_not< type >(per1);
            fail |=t_vector_logical_not< type >(per2);
            break;
        }

        if(fail)
        {
            tout<<"Function t_vector failed on "<<step<<" step, on case "<<ind%VECTOR_CASE_COUNT<<'\n';
            return fail;
        }

        ind=gen.Rand();
    }

    return fail;
}


TEST(vector,all,"Test all functions.")
{
    bool fail = false;

    fail |= t_vector<rational<big_int> >(3455,1000);

    if(fail)
        return resFAIL;
    else
        return resOK;
}
