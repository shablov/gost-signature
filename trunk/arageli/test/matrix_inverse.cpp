/*****************************************************************************

    test/matrix_inverse.cpp

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

// File : matrix_inverse.cpp
// Author : Zaitsev Mikhail
// Tester : Aleksandrov Vladimir
// Date of creation : 2005/12/1
// Date of modification : 2006/01/31
// Testing date :
/*  Decription: test for function "Arageli::inverse(const matrix<T,REFCNT>&)".
    Testing matrix is constructed using its string notation.I don't know if this matrix
    has the inverse matrix or not that's why I check its determinant and rank both.
    To understand that function works correctly I multiplicate the initial matrix and
    its inverse matrix and check if the result of multiplication is a unit matrix.
    If function works incorrectly in certain case test print matrix using its string
    notation
*/
// Using functions: det(),rank(),nrows(),is_unit()
// Errors: test shows no errors

// Attention : first uncomment the need strings of generation examples and specification
// of template testing function

#include "stdafx.hpp"

using namespace Arageli;


template <typename T>
bool inverse_matrix(const char* s)
{
    matrix<T> A(s);
    matrix<T> X;

    if(A.is_square()&& rank(A)==A.nrows())
        X=inverse(A);
    else
        return false;

    if((A*X).is_unit())
        return false;
    else
    {
        tout<<"inverse failed with"<<s;
        return true;
    }
}

TEST(matrix,inverse,"Test inverse function")
{
    bool fail=false;
    RNG element(2,16);

    for(int k=0;k<10;k++)
    {
        int size=1+element.Rand()%5;
        std::strstream buff;
        buff<<'(';
        for(int i=0; i<size; i++)
        {
            buff<<'(';
            for(int j=0; j<size-1; j++)
                // 1: buff<<(element.Rand()-(1<<14))<<'/'<<element.Rand()+1<<',';
                // 2: buff<<(element.Rand()-(1<<14))<<',';
                buff<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<',';
            // 1: buff<<(element.Rand()-(1<<14))<<'/'<<element.Rand()+1<<')';
            // 2: buff<<(element.Rand()-(1<<14))<<')';
            buff<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<')';
            if(i!=size-1) buff<<',';
        }
        buff<<')';
        buff<<'\x0';

        // 1: fail |=inverse_matrix<rational<> >(buff.str());
        // 2: fail |=inverse_matrix<rational<int> >(buff.str());
        fail |=inverse_matrix<rational<big_int> >(buff.str());
        if(fail) return resFAIL;
    }

    if(fail) return resFAIL;
    return resOK;

}

/* End of file matrix_inverse.cpp */
