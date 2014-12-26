/*****************************************************************************

    test/matrix_is_null.cpp

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

// File: matrix_is_null.cpp
// Author: Zaitsev Mikhail
// Tester: Aleksandrov Vladimir
// Date of creation : 2005/12/4
// Date of modification : 2006/01/31
// Testing date :
/*  Description: Test for function is_null()
    To understand that function works incorrectly I sequently check the value
    (zero or non-zero) of elements of matrix
*/
// Using functions : nrows(),ncols()
// Errors : test shows no errors

#include "stdafx.hpp"

using namespace Arageli;

template <typename T>
bool matrix_is_null(const char* s)
{
    matrix<T> A(s);
    bool temp_null=A.is_null();
    bool temp_use=true;

    for(int i=0;i<A.nrows();i++)
        for(int j=0;j<A.ncols();j++)
            if(A(i,j)!=0) temp_use=false;   //it not an optimum variant but I think it is
                                            //not critically in this case

    if((temp_null & temp_use) | ((!temp_null) & (!temp_use))) return false;
    else
    {

        tout<<"function is_null failed with"<<s;
        return true;

    }
}

TEST(matrix,is_null,"Test is_null function")
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
                // 1: buff<<(element.Rand()-(1<<14))<<'/'<<element.Rand()<<',';
                // 2: buff<<(element.Rand()-(1<<14))<<'.'<<abs(element.Rand())<<',';
                buff<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<',';
            // 1: buff<<(element.Rand()-(1<<14))<<'/'<<element.Rand()<<')';
            // 2: buff<<(element.Rand()-(1<<14))<<'.'<<abs(element.Rand())<<')';
            buff<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<')';
            if(i!=rows-1) buff<<',';
        }
        buff<<')';
        buff<<'\x0';

        // 1: fail |=matrix_is_null<rational<> >(buff.str());
        // 2: fail |=matrix_is_null<double>(buff.str());
        fail |=matrix_is_null<big_int>(buff.str());

    }

    if(fail) return resFAIL;
    return resOK;

}

/* End of file matrix_is_nul.cpp */
