/*****************************************************************************

    test/matrix_addmul_rows.cpp

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

// File : matrix_addmul_rows.cpp
// Author : Zaitsev Mikhail
// Tester : Aleksandrov Vladimir
// Date of creation: 2005/12/14
// Date of modification : 2006/01/31
// Testing date :
// Description : Test for function addmult_rows()
/*  I think it is not dificult to understand the way helping
    me to check the correct work of the function addmult_rows
*/
// Using functions : addmult_rows(),nrows()
// Attention : first uncomment the need strings of generation examples and specification
// of template testing function
// Errors :

#include "stdafx.hpp"

using namespace Arageli;

template <typename T>
bool m_addmul_rows(const char* s, T coef1, T coef2, int a, int m)
{
    matrix<T> A(s);
    matrix<T> B = A;

    int add = a%A.nrows();//index of the first row
    int mul = m%A.nrows();//index of the second row
    if(add == mul && add == 0)mul = A.nrows()-1;
    if(add == mul && add == A.nrows()-1)mul = 0;
    if(add == mul)add = add + 1;

    ARAGELI_ASSERT_ALWAYS(add != mul);
    ARAGELI_ASSERT_ALWAYS(add >= 0 && add < A.nrows());
    ARAGELI_ASSERT_ALWAYS(mul >= 0 && mul < A.nrows());

    A.addmult_rows(add, coef1, mul, coef2);

    for(int i = 0; i < A.ncols(); i++)
        if(A(add, i) != coef1*B(add, i) + coef2*B(mul, i))
        {
            tout
                << "function addmult_rows failed with string " << s
                << "\nA = \n";
            output_aligned(tout, A);
            tout
                << "\nB = \n";
            output_aligned(tout, B);
            tout
                << "\ncoef1 = " << coef1
                << "\ncoef2 = " << coef2
                << "\na = " << a
                << "\nm = " << m
                << "\ncol = " << i
                << "\nA(add, col) = " << A(add,i)
                << "\ncoef1*B(add, col) + coef2*B(mul, col) = " << coef1*B(add, i) + coef2*B(mul, i);
            return true;
        }

    return false;
}



TEST(matrix,addmult_rows,"Test addmult_rows function")
{
    bool fail=false;
    RNG element(2,16);


    for(int k = 0; k < 10; k++)
    {
        int rows = 2+element.Rand()%5;
        int cols = 1+element.Rand()%5;

        std::strstream buff;
        buff<<'(';
        for(int i=0; i<rows; i++)
        {
            buff<<'(';
            for(int j=0; j<cols-1; j++)
            {
                // 1: buff<<(element.Rand()-(1<<14))<<'/'<<element.Rand()+1<<',';
                // 2: buff<<(element.Rand()-(1<<14))<<',';
                buff
                    << element.Rand()-(1<<14)
                    << abs(element.Rand())
                    << abs(element.Rand())
                    << abs(element.Rand())
                    << abs(element.Rand()) << ',';
            }

            // 1: buff<<(element.Rand()-(1<<14))<<'/'<<element.Rand()+1<<')';
            // 2: buff<<(element.Rand()-(1<<14))<<')';
            buff
                << element.Rand()-(1<<14)
                << abs(element.Rand())
                << abs(element.Rand())
                << abs(element.Rand())
                << abs(element.Rand()) << ')';
            if(i != rows-1)buff<<',';
        }

        buff << ')';
        buff << '\x0';

        // 1: fail |=m_addmul_rows<rational<> >(buff.str(),element.Rand(),element.Rand(),element.Rand(),element.Rand());
        // 2: fail |=m_addmul_rows<int>(buff.str(),element.Rand(),element.Rand(),element.Rand(),element.Rand());
        fail |=m_addmul_rows<big_int>
        (
            buff.str(),
            element.Rand(), element.Rand(),
            element.Rand(), element.Rand()
        );
    }

    if(fail)return resFAIL;
    return resOK;
}

/* End of file matrix_addmul_rows.cpp */
