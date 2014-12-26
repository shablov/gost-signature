/*****************************************************************************

    test/matrix_insert_row.cpp

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

// File : matrix_insert_row.cpp
// Author : Zaitsev Mikhail
// Tester : Aleksandrov Vladimir
// Date of creation : 2005/12/29
// Date of modification : 2006/01/31
// Testing date :
/*  Description : Test for function insert_row()
    I insert then erase inserted row checking after each operations the number
    of rows in the matrix.At the end I compare the testing matrix with its copy
*/
// Using functions : insert_row(),erase_row(),copy_row(),nrows()
// Errors :

#include "stdafx.hpp"

using namespace Arageli;


template <class T>
bool m_insert_col(const char* s)
{
    matrix<T> A(s);
    int rows=A.nrows();
    int cols=A.ncols();

    if(rows<2 && cols<2) return false;

    matrix<T> X=A;

    Arageli::vector<T> ins(cols);//row that will be inserted in matrix


    for(int j=0;j<rows;j++)
    {
        A.insert_row(j,ins);
        if(A.nrows()!=rows+1)
        {
            tout<<"function failed with"<<s;
            tout<<"quantity of columns";
            return true;
        }
        A.erase_row(j);
        if(A.nrows()!=rows)
        {
            tout<<"function failed with"<<s;
            tout<<"quantity of rows";
            return true;
        }
    }

    if(A!=X)
    {
            tout<<"function failed with"<<s;
        tout<<"quantity of rows";
        return true;
    }

    return false;
}


TEST(matrix,insert_row,"Test for function insert_row")
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

        // 1: fail |=m_insert_col<rational<> >(buff.str());
        // 2: fail |=m_insert_col<int>(buff.str());
        fail |=m_insert_col<big_int>(buff.str());

    }

    if(fail) return resFAIL;
    return resOK;

}

/* End of file insert_row.cpp */
