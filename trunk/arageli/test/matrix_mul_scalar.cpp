/*****************************************************************************

    test/matrix_mul_scalar.cpp

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

// File: matrix_mul_scalar.cpp
// Author : Zaitsev Mikhail
// Tester : Aleksandrov Vladimir
// Date of creation : 2005/12/4
// Date of modification :2006/01/31
// Testing date :
// Description: test for function matrix<T,REFCNT>::assign_multiplies_val(const C& x)
/*  To understand that function works correctly I divide the result of previous
    operation on the same scalar.
*/
// Using functions : assign_multiplies_val(),is_null()
// Errors : test shows errors with double matrixes


#include "stdafx.hpp"

using namespace Arageli;



template <typename T>
bool m_mul_scalar(const char* s,T number)
{
    matrix<T> A(s);
    matrix<T> X=A;

    if((number==0) && (A.mul_scalar(number).is_null())) return false;

    if((number==0) && (!A.mul_scalar(number).is_null()))
    {
        tout<<"function mul_scalar failed with"<<s<<"     "<<number;
        return true;
    }

    if(number!=0)
    {
        A.mul_scalar(number).div_scalar(number);

        for(int i=0;i<A.nrows();i++)
            for(int j=0;j<A.ncols();j++)
                if(A(i,j)!=X(i,j))
                {
                    tout<<"function mul_scalar failed with"<<s<<"     "<<number;
                    return true;
                }
    }
    return false;
}

TEST(matrix,assign_multiplies_val,"Test assign_multiplies_val function : ")
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
            // 2:    buff<<(element.Rand()-(1<<14))<<')';
            buff<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<')';
            if(i!=rows-1) buff<<',';
        }
        buff<<')';
        buff<<'\x0';

        //fail |=m_mul_scalar<rational<> >(buff.str(),element.Rand());
        //fail |=m_mul_scalar<int>(buff.str());
        fail |=m_mul_scalar<big_int>(buff.str(),element.Rand());

    }

    if(fail) return resFAIL;
    return resOK;

}

/* End of file matrix_mul_scalar.cpp */
