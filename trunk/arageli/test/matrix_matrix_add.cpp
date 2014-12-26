/*****************************************************************************

    test/matrix_matrix_add.cpp

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

// File ; matrix_matrix_add.cpp
// Author : Zaitsev Mikhail
// Tester : Aleksandrov Vladimir
// Date of creation : 2005/12/16
// Dateof modification : 2006/01/31
// Testing date :
// Description : Test for function operator+=(const matrix<T,REFCNT>& x)
/*
    I check that testing function works correctly subtracting from the result of addition
    the previuos matrix
*/
// Using functions : operator-=(const matrix<T,REFCNT>& x)
// Attention : first uncomment the need strings of generation examples and specification
// of template testing function

// Errors : test shows that testing function works incorrectly with double matrixes

#include "stdafx.hpp"

using namespace Arageli;

template <typename T>
bool matrix_add(const char* s,const char* m)
{
    matrix<T> A(s);
    matrix<T> B(m);
    matrix<T> X=A;

    (A+=B)-=B;
    for(int i=0;i<X.nrows();i++)
        for(int j=0;j<X.ncols();j++)
            if(A(i,j)!=X(i,j))
            {
                tout<<"function failed with"<<s<<"    "<<m;
                return true;
            }

    return false;
}


TEST(matrix,matrix_add,"Test operator+= function")
{

    bool fail=false;
    RNG element(2,16);

    for(int k=0;k<20;k++)
    {
        int cols=1+element.Rand()%5;
        int rows=1+element.Rand()%5;

        std::strstream buffA,buffB;

        //generation of matrix A
        buffA<<'(';
        for(int i=0; i<rows; i++)
        {
            buffA<<'(';
            for(int j=0; j<cols-1; j++)
            {
                // 1: buffA<<(element.Rand()-(1<<14))<<'/'<<element.Rand()+1<<',';
                // 2: buffA<<(element.Rand()-(1<<14))<<'.'<<abs(element.Rand())<<',';
                // 3: buffA<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<',';
            }
            // 1: buffA<<(element.Rand()-(1<<14))<<'/'<<element.Rand()+1<<')';
            // 2: buffA<<(element.Rand()-(1<<14))<<'.'<<abs(element.Rand())<<')';
            // 3: buffA<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<')';
            if(i!=rows-1) buffA<<',';
        }
        buffA<<')';
        buffA<<'\x0';

        //generation of matrix B
        buffB<<'(';
        for(int i=0; i<rows; i++)
        {
            buffB<<'(';
            for(int j=0; j<cols-1; j++)
            {
                // 1: buffB<<(element.Rand()-(1<<14))<<'/'<<element.Rand()+1<<',';
                // 2: buffB<<(element.Rand()-(1<<14))<<'.'<<abs(element.Rand())<<',';
                // 3: buffB<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<',';
            }
            // 1: buffB<<(element.Rand()-(1<<14))<<'/'<<element.Rand()+1<<')';
            // 2: buffB<<(element.Rand()-(1<<14))<<'.'<<abs(element.Rand())<<')';
            // 3: buffB<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<')';
            if(i!=rows-1) buffB<<',';
        }
        buffB<<')';
        buffB<<'\x0';

        // 1: fail |=matrix_add<rational<> >(buffA.str(),buffB.str());
        // 2: fail |=matrix_add<double>(buffA.str(),buffB.str());
        // 3: fail |=matrix_add<big_int>(buffA.str(),buffB.str());

    }

    if(fail) return resFAIL;
    return resOK;

}

/* End of file matrix_matrix_add.cpp */
