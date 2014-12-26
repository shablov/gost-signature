/*****************************************************************************

    test/matrix_is_unit.cpp

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

// File : matrix_is_unit.cpp
// Author : Zaitsev Mikhail
// Tester : Aleksandrov Vladimir
// Date of creation : 2006/02/05
// Date of modification :
// Testing date :
// Using functions : is_unit(),is_opposite_unit()


#include "stdafx.hpp"

using namespace Arageli;


template <class T,char s>
bool matrix_is_unit()
{

    RNG element(2,16);

    int size=1+element.Rand()%6;

    std::strstream buff;
    buff<<'(';

    switch(s)
    {
        case 'r':
            for(int i=0; i<size; i++)
            {
                buff<<'(';
                for(int j=0; j<size-1; j++)
                    buff<<element.Rand()-(1<<14)<<'/'<<element.Rand()+1<<',';

                buff<<(element.Rand()-(1<<14))<<'/'<<element.Rand()+1<<')';
                if(i!=size-1) buff<<',';
            }
        break;

        case 'i':
            for(int i=0; i<size; i++)
            {
                buff<<'(';
                for(int j=0; j<size-1; j++)
                    buff<<(element.Rand()-(1<<14))<<',';

                buff<<(element.Rand()-(1<<14))<<')';
                if(i!=size-1) buff<<',';
            }
        break;

        case 'b':
            for(int i=0; i<size; i++)
            {
                buff<<'(';
                for(int j=0; j<size-1; j++)
                    buff<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<',';

                buff<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<')';
                if(i!=size-1) buff<<',';
            }
        break;
    }

    buff<<')';
    buff<<'\x0';

    matrix<T> A(buff.str());//matrix constructor from string
    matrix<T> B=A;

    A.opposite();

    if(B.is_unit()!=A.is_opposite_unit())
    {
        tout<<"function failed with"<<buff.str();
        return true;
    }

    return false;
}


TEST(matrix,is_unit,"Test for function is_unit")
{
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN

    bool fail=false;
    for(int i=0;i<100;i++)
    {
        fail |=matrix_is_unit<rational<>,'r'>();
        fail |=matrix_is_unit<int,'i'>();
        fail |=matrix_is_unit<big_int,'b'>();
    }

    if(fail) return resFAIL;
    return resOK;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END

}

/* End of file matrix_is_unit.cpp */
