/*****************************************************************************

    test/matrix_matrix_mult.cpp

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

// File: matrix_matrix_mult.cpp
// Author : Zaitsev Mikhail
// Tester : Aleksandrov Vladimir
// Date of creation : 2005/12/10
// Date of modification : 206/01/31
// Testing date :
// Description: test for function operator*=(const matrix<,REFCNT>& x).
/*  To understand that function works correctly I check that c(i,j)=a(i,0)b(0,j)+......+
    a(i,a.ncols()-1)b(b.nrows()-1,j)
*/
// Attention : first uncomment the need strings of generation examples and specification
// of template testing function
// Errors : tets shows no errors

#include "stdafx.hpp"

using namespace Arageli;


template <class T>
bool matrix_mult(const char* s,const char* m)
{
    matrix<T> A(s);//the first matrix
    matrix<T> B(m);//the second matrix
    matrix<T> C=A;//the copy of the first matrix

    C*=B;

    for(int i=0;i<A.nrows();i++)
        for(int j=0;j<B.ncols();j++)
        {
            T sum=0;//a(i,0)b(0,j)+.....+a(i,A.ncols()-1)b(A.ncols()-1,j)
            for(int k=0;k<A.ncols();k++)
                sum+=A(i,k)*B(k,j);

            if(C(i,j)!=sum)
            {
                tout<<"function operator*= failed with"<<s<<"   "<<m;
                return true;
            }
        }

    return false;
}

TEST(matrix,matrix_mult,"Test matrix_mult function : ")
{
    bool fail=false;
    RNG element(2,16);

    for(int k=0;k<50;k++)
    {
        int rows=1+element.Rand()%5;
        int cols=1+element.Rand()%5;
        int temp=1+element.Rand()%5;

        std::strstream buffA,buffB;

        //generation of matrix A
        buffA<<'(';
        for(int i=0; i<rows; i++)
        {
            buffA<<'(';
            for(int j=0; j<cols-1; j++)
                // 1: buffA<<(element.Rand()-(1<<14))<<'/'<<element.Rand()+1<<',';
                // 2: buffA<<(element.Rand()-(1<<14))<<',';
                buffA<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<',';

            // 1: buffA<<(element.Rand()-(1<<14))<<'/'<<element.Rand()+1<<')';
            // 2: buffA<<(element.Rand()-(1<<14))<<')';
            buffA<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<')';
            if(i!=rows-1) buffA<<',';
        }
        buffA<<')';
        buffA<<'\x0';

        //generation of matrix B
        buffB<<'(';
        for(int i=0; i<cols; i++)
        {
            buffB<<'(';
            for(int j=0; j<temp-1; j++)
                // 1: buffB<<(element.Rand()-(1<<14))<<'/'<<element.Rand()+1<<',';
                // 2: buffB<<(element.Rand()-(1<<14))<<',';
                buffB<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<',';

            // 1: buffB<<(element.Rand()-(1<<14))<<'/'<<element.Rand()+1<<')';
            // 2: buffB<<(element.Rand()-(1<<14))<<')';
            buffB<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<')';
            if(i!=cols-1) buffB<<',';
        }
        buffB<<')';
        buffB<<'\x0';

        // 1: fail |=matrix_mult<rational<> >(buffA.str(),buffB.str());
        // 2: fail |=matrix_mult<int>(buffA.str(),buffB.str());
        fail |=matrix_mult<big_int>(buffA.str(),buffB.str());

    }

    if(fail) return resFAIL;
    return resOK;

}

/* End of file matrix_matrix_mult.cpp */
