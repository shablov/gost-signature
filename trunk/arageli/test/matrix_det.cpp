/*****************************************************************************

    test/matrix_det.cpp

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

// File : matrix_det.cpp
// Author : Zaitsev Mikhail
// Tester : Aleksandrov Vladimir
// Date of creation : 2005/12/14
// Date of modification : 2006/01/31
// Testing date :
// Description : Test for function det(const matrix<T,REFCNT>& x)
/*  Testing matrix is constructed using its string notation
    To understand that function works correctly I adduct the matrix to the
    diagonal(stepping) view and then compare the result of the testing function with
    multiplication of diagonal elements
*/
// Using functions : swap_rows(),addmult_rows(),nrows(),mult_row()

// Attention : first uncomment the need strings of generation examples and specification
// of template testing function
// Errors : test shows no errors

#include "stdafx.hpp"

using namespace Arageli;


template<typename T>
bool matrix_det(const char* s)
{
    matrix<T> B(s);
    matrix<T> C=B;

    //adduction the matrix to the stepping view
    int rows_swap=0;//number of rows' swapping
    T det_divide=1;
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
                T temp=B(j,i);
                B.mult_row(j,safe_reference(B(z,i)));
                det_divide*=B(z,i);
                B.addmult_rows(j,z,-temp);
            }
            k=0;
            break;
        }

    //multiplication the diagonal elements
    T mul_diag=1;
    for(int i=0;i<B.nrows();i++)
        mul_diag*=B(i,i);

    mul_diag/=det_divide;

    if(rows_swap%2) mul_diag=-mul_diag;

    if(det(C)!=mul_diag)
    {
        tout<<"function det failed with"<<s;
        return true;
    }

    return false;
}


TEST(matrix,det,"Test det function")
{

    bool fail=false;
    RNG element(2,16);


    for(int k=0;k<20;k++)
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

        // 1: fail |=matrix_det<rational<> >(buff.str());
        // 2: fail |=matrix_det<rational<int> >(buff.str());
        fail |=matrix_det<rational<big_int> >(buff.str());

    }

    if(fail) return resFAIL;
    return resOK;
}

/* End of file matrix_det.cpp */
