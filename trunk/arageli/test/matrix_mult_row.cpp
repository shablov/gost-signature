/*****************************************************************************

    test/matrix_mult_row.cpp

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

// File : matrix_mult_row.cpp
// Author : Zaitsev Mikhail
// Tester : Aleksandrov Vladimir
// Date of creation: 2006/01/27
// Date of modification : 2006/01/31
// Testing date :
// Description : Test for function mult_row
/*  I check the correction of work of this function dividing row on the same value that
    I use in multiplication
*/
// Using functions : mult_row(),div_row(),nrows()
// Errors :


#include "stdafx.hpp"

using namespace Arageli;


template <class T>
bool m_mult_row(const char* s,T val)
{
    if(!val) return false;
    matrix<T> A(s);
    matrix<T> X=A;

    for(int i=0;i<A.nrows();i++)
    {
        A.mult_row(i,val);
        A.div_row(i,val);
    }

    if(A!=X)
    {
        tout<<"function failed with"<<s;
        return true;
    }

    return false;

}


TEST(matrix,mult_row,"Tets for mult_row function")
{

    bool fail=false;
    RNG element(2,16);

    for(int k=0;k<10;k++)
    {
        int cols=1+element.Rand()%5;
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

        //fail |=m_mult_row<rational<> >(buff.str(),element.Rand());
        //fail |=m_mult_row<int>(buff.str(),element.Rand());
        fail |=m_mult_row<big_int>(buff.str(),element.Rand());

    }

    if(fail) return resFAIL;

    return resOK;

}

/* End of file matrix_mult_row.cpp */
