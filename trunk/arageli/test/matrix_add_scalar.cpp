/*****************************************************************************

    test/matrix_add_scalar.cpp

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

// File: matrix_add_scalar.cpp
// Author: Zaitsev Mikhail
// Tester :Aleksandrov Vladimir
// Date of creation: 2005/12/4
// Date of modification : 2006/01/31
// Testing date :
/*  Description: test for function matrix<T,REFCNT>::assign_plus_val(const C& x).
    To understand that function works correctly I subtract from the result of previous
    operation the same scalar.
*/
// Using functions : assign_plus_val(),assign_minus_val()
// Attention : first uncomment the need strings of generation examples and specification
// of template testing function

// Errors :

#include "stdafx.hpp"

using namespace Arageli;


template <class T>
bool m_add_scalar(const char* s,T number)
{
    matrix<T> A(s);
    matrix<T> X=A;

    A.add_scalar(number).sub_scalar(number);

    if(A!=X)
    {
        tout<<"function assign_plus_val failed with"<<s<<"     "<<number;
        return true;
    }

    return false;
}

TEST(matrix,assign_plus_val,"Test assign_plus_val function : ")
{

    bool fail=false;
    RNG element(2,16);

    for(int k=0;k<20;k++)
    {
        int rows=1+element.Rand()%5;
        int cols=1+element.Rand()%5;

        std::strstream buff;
        buff<<'(';
        for(int i=0; i<rows; i++)
        {
            buff<<'(';
            for(int j=0; j<cols-1; j++)
                // 1: buff<<(element.Rand()-(1<<14))<<'/'<<element.Rand()+1<<',';
                // 2: buff<<(element.Rand()-(1<<14))<<',';
                buff<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<',';
            // 1: buff<<(element.Rand()-(1<<14))<<'/'<<element.Rand()+1<<')';
            // 2: buff<<(element.Rand()-(1<<14))<<')';
            buff<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<')';
            if(i!=rows-1) buff<<',';
        }
        buff<<')';
        buff<<'\x0';

        // 1: fail |=m_add_scalar<rational<> >(buff.str(),element.Rand());
        // 2: fail |=m_add_scalar<int>(buff.str(),element.Rand());
        fail |=m_add_scalar<big_int>(buff.str(),element.Rand());

    }

    if(fail) return resFAIL;
    return resOK;

}

/* End of file matrix_add_scalar.cpp */
