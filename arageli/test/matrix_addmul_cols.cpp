/*****************************************************************************

    test/matrix_addmul_cols.cpp

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

// File : addmul_cols.cpp
// Author : Zaitsev Mikhail
// Tester : Aleksandrov Vladimir
// Date : 2005/01/28
// Description : I think it is not dificult to understand the way helping
// me to check the correct work of the function addmult_rows

// Attention : first uncomment the need strings of generation examples and specification
// of template testing function
// Errors :

#include "stdafx.hpp"

using namespace Arageli;


template <class T>
bool m_addmul_cols(const char* s,T coef1,T coef2,int a,int m)
{
    matrix<T> A(s);
    matrix<T> B=A;

    int add=a%A.ncols();//index of the first column
    int mul=m%A.ncols();//index of the second column
    if(add==mul && add==0) mul=A.ncols()-1;
    if(add==mul && add==A.ncols()-1) mul=0;
    if(add == mul)add = add + 1;

    ARAGELI_ASSERT_ALWAYS(add != mul);
    ARAGELI_ASSERT_ALWAYS(add >= 0 && add < A.ncols());
    ARAGELI_ASSERT_ALWAYS(mul >= 0 && mul < A.ncols());


    A.addmult_cols(add,coef1,mul,coef2);

    for(int i=0;i<A.nrows();i++)
        if(A(i,add)!=coef1*B(i,add)+coef2*B(i,mul))
        {
            tout<<"function addmult_cols failed with"<<s<<"   "<<coef1<<"   "<<coef2;
            return true;
        }

    return false;
}



TEST(matrix,addmult_cols,"Test addmult_cols function")
{
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN


    bool fail=false;
    RNG element(2,16);

    for(int k=0;k<10;k++)
    {
        int cols=2+element.Rand()%5;
        int rows=1+element.Rand()%5;
        std::strstream buff;
        buff<<'(';
        for(int i=0; i<rows; i++)
        {
            buff<<'(';
            for(int j=0; j<cols-1; j++)
                // 1: buff<<(element.Rand()-(1<<14))<<'/'<<element.Rand()<<',';
                // buff<<(element.Rand()-(1<<14))<<',';
                buff<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<',';
            // 1: buff<<(element.Rand()-(1<<14))<<'/'<<element.Rand()<<')';
            // buff<<(element.Rand()-(1<<14))<<')';
            buff<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<')';
            if(i!=rows-1) buff<<',';
        }
        buff<<')';
        buff<<'\x0';

        fail |=m_addmul_cols<rational<big_int> >(buff.str(),element.Rand(),element.Rand(),element.Rand(),element.Rand());

    }

    if(fail) return resFAIL;


    return resOK;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END
}

/* End of file addmul_cols.cpp */
