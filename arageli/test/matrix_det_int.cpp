/*****************************************************************************

    test/matrix_det_int.cpp

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

// File : matrix_det_int.cpp
// Author : Zaitsev Mikhail
// Tester : Aleksandrov Vladimir
// Date of creation: 2006/01/17
// Date of modification : 2006/01/31
// Testing date :
/*  Description : Test for function det_int
    I check the correct work of this function implementing my own function that calculates
    determinant and then compare it with the result of testing one.
*/
// Using functions : det_int(),ncols(),nrows(),swap_rows(),mult_row(),adddmul_rows()
// Errors : test shows errors
// Attention : first uncomment the need strings of generation examples and specification
// of template testing function


#include "stdafx.hpp"
//#include "arageli/arageli.hpp"
//#include "ts/ts.h"

using namespace Arageli;


// class of the arithmetic progression
class Arithmetic
{
public:
    static int progression(int start_point,int step)
    {
        return start_point+step;
    }
};


// class of the geometric progression
class Geometric
{
public:
    static int progression(int start_point,int step)
    {
        return start_point*step;
    }
};


template <class Q>
bool matrix_det_int(int size,int start_point,int step)
{
    matrix<int> C(size);
    int temp=start_point;

    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
        {
            C(i,j) = Q::progression(temp,step);
            temp = C(i,j);
        }
    matrix<rational<> > B=C;

    //adduction the matrix to the stepping view
    int rows_swap=0;//number of rows' swapping
    rational<> det_divide=1;
    int k=0;//auxiliary variable
    for(int z=0;z<B.nrows()-1;z++)
        for(int i=z;i<B.ncols()-1;i++)
        {

            if(B(z,i)==0)
            for(int p=i+1;p<B.nrows();p++)
            {
                if(B(p,i)!=0)
                {
                    B.swap_rows(i,p);
                    k=1;
                    rows_swap++;
                    break;
                }
            }
            else  k=1;

            if(k==0) continue;


            for(int j=z+1;j<B.nrows();j++)
            {
                if(B(j,i)==0) continue;
                rational<> temp=B(j,i);
                B.mult_row(j,safe_reference(B(z,i)));
                det_divide*=B(z,i);
                B.addmult_rows(j,z,-temp);
            }
            k=0;
            break;
        }

    //multiplication the diagonal elements
    rational<> mul_diag=1;
    for(int i=0;i<B.nrows();i++)
        mul_diag*=B(i,i);

    mul_diag/=det_divide;

    if(rows_swap%2) mul_diag=-mul_diag;

    if(det_int(C)!=mul_diag)
    {
        tout<<"function det failed with\n";
        output_aligned(tout,C,"|| "," ||"," ");
        return true;
    }

    return false;

}


TEST(matrix,det_int,"Test for function det_int")
{
    bool fail=false;

    for(int k=2;k<5;k++)
    {
        fail |=matrix_det_int<Arithmetic>(k,3,4);
        fail |=matrix_det_int<Arithmetic>(k,7,0);
        fail |=matrix_det_int<Geometric>(k,2,2);
    }

    if(fail) return resFAIL;
    return resOK;

}


/* End of file matrix_det_int.cpp */
