/*****************************************************************************

    test/matrix_insert_col.cpp

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

// File : matrix_insert_col.cpp
// Author : Zaitsev Mikhail
// Tester : Aleksandrov Vladimir
// Date of creation : 2005/12/29
// Date of modification : 2006/01/31
// Testing date :
/*  Description : Test for function insert_col()
    I insert then erase inserted column checking after each operations the number
    of columns in the matrix.At the end I compare the teh testing matrix with its copy
*/

// Using functions : insert_col(),erase_col(),copy_col(),ncols()
// Errors : test shows no errors

#include "stdafx.hpp"

using namespace Arageli;


// class of the arithmetic progression
template <class T> class Arithmetic
{
public:
    static T progression(T start_point,T step)
    {
        return start_point+step;
    }
};


// class of the geometric progression
template <class T> class Geometric
{
public:
    static T progression(T start_point,T step)
    {
        return start_point*step;
    }
};


template <class T,class C>
bool m_insert_col(int rows,int cols,T start_point,T step)
{
    if(rows<2 && cols<2) return false;

    matrix<T> A(rows,cols,fromsize);


    T temp=start_point;
    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
        {
            A(i,j) = C::progression(temp,step);
            temp = A(i,j);
        }

    matrix<T> X=A;

    Arageli::vector<T> ins(rows);//column that will be inserted in matrix


    for(int j=0;j<cols;j++)
    {
        A.insert_col(j,ins);
        if(A.ncols()!=cols+1)
        {
            tout<<"function failed with\n";
            output_aligned(tout,A,"|| "," ||"," ");
            tout<<"quantity of columns";
            return true;
        }
        A.erase_col(j);
        if(A.ncols()!=cols)
        {
            tout<<"function failed with\n";
            output_aligned(tout,A,"|| "," ||"," ");
            tout<<"quantity of columns";
            return true;
        }
    }

    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            if(A(i,j)!=X(i,j))
            {
                tout<<"function failed with\n";
                output_aligned(tout,A,"|| "," ||"," ");
                tout<<"quantity of columns";
                return true;
            }

    return false;
}


TEST(matrix,insert_col,"Test for function insert_col")
{
    bool fail=false;

    for(int k=2;k<6;k++)
        for(int q=2;q<6;q++)
            fail |=m_insert_col<int,Arithmetic<int> >(k,q,4,3);

    if(fail) return resFAIL;
    return resOK;

}

/* End of file matrix_insert_col.cpp */
