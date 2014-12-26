/*****************************************************************************

    test/matrix_swap_col.cpp

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

// File : matrix_swap_col.cpp
// Author : Zaitsev Mikhail
// Tester : Aleksandrov Vladimir
// Date of creation : 2006/02/02
// Date of modification :
// Testing date :
// Using functions : swap_cols(),copy_col(),insert_col(),erase_col(),ncols()

// Midified : Sergey S. Lyalin, 27.02.2006


#include "stdafx.hpp"

using namespace Arageli;


template <class T,char s>
bool m_swap_col()
{
    RNG element(2,16);

    int cols=2+element.Rand()%5;
    int rows=1+element.Rand()%5;

    std::strstream buff;
    buff<<'(';

    switch(s)
    {
        case 'r':
            for(int i=0; i<rows; i++)
            {
                buff<<'(';
                for(int j=0; j<cols-1; j++)
                    buff<<element.Rand()-(1<<14)<<'/'<<element.Rand()+1<<',';

                buff<<(element.Rand()-(1<<14))<<'/'<<element.Rand()+1<<')';
                if(i!=rows-1) buff<<',';
            }
        break;

        case 'i':
            for(int i=0; i<rows; i++)
            {
                buff<<'(';
                for(int j=0; j<cols-1; j++)
                    buff<<(element.Rand()-(1<<14))<<',';

                buff<<(element.Rand()-(1<<14))<<')';
                if(i!=rows-1) buff<<',';
            }
        break;

        case 'b':
            for(int i=0; i<rows; i++)
            {
                buff<<'(';
                for(int j=0; j<cols-1; j++)
                    buff<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<',';

                buff<<(element.Rand()-(1<<14))<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<abs(element.Rand())<<')';
                if(i!=rows-1) buff<<',';
            }
        break;
    }
    buff<<')';
    buff<<'\x0';

    matrix<T> A(buff.str());//matrix constructor from string
    matrix<T> X=A, Y=A;// copy of testing matrix


    for(int i=0;i<A.ncols();i++)
        for(int j=i+1;j<A.ncols();j++)
        {
            A.swap_cols(i,j);

            Arageli::vector<T> temp;
            //my own algorithm of rows' swapping
            X.insert_col(i,X.copy_col(j,temp));
            //output_aligned(tout << "\nX after X.insert_col(i,X.copy_col(j,temp))\n", X);
            X.insert_col(j+1,X.copy_col(i+1,temp));
            //output_aligned(tout << "\nX after X.insert_col(j+1,X.copy_col(i+1,temp))\n", X);
            X.erase_col(i+1);
            //output_aligned(tout << "\nX after X.erase_col(i+1)\n", X);
            X.erase_col(j+1);
            //output_aligned(tout << "\nX after X.erase_col(j+1)\n", X);

            if(A!=X)
            {
                tout << "function failed (A != X) with " << buff.str() << '\n';
                output_aligned(tout << "original matrix is\n", Y);
                tout << "A is\n";
                output_aligned(tout, A);
                tout << "X is\n";
                output_aligned(tout, X);
                tout << "temp = " << temp << '\n';
                tout << "i = " << i << ", j = " << j << '\n';
                return true;

            }

            //A.swap_cols(i,j);

        }

    return false;

}

namespace
{

bool additional_test ()
{
    matrix<int> X = "((1, 2, 3, 4, 5), (6, 7, 8, 9, 10))", A = X;

    typedef int T;

    int i = 0, j = 2;
    A.swap_cols(0, 2);

    Arageli::vector<T> temp;
    X.insert_col(i,X.copy_col(j,temp));
    //output_aligned(tout << "\nX after X.insert_col(i,X.copy_col(j,temp))\n", X);
    X.insert_col(j+1,X.copy_col(i+1,temp));
    //output_aligned(tout << "\nX after X.insert_col(j+1,X.copy_col(i+1,temp))\n", X);
    X.erase_col(i+1);
    //output_aligned(tout << "\nX after X.erase_col(i+1)\n", X);
    X.erase_col(j+1);
    //output_aligned(tout << "\nX after X.erase_col(j+1)\n", X);

    //output_aligned(tout << "\nA\n", A);

    //output_aligned(tout << "\n------------\noriginal\n", a);
    //vector<int> b = "(101, 102)";
    //a.insert_col(0, b);
    //output_aligned(tout << "after inserting\n", a);

    if(X != A)
    {
        tout << "Additional test failed.";
        return true;
    }
    else return false;
}

}


TEST(matrix,swap_cols,"Test for function swap_cols")
{
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN
    bool fail=false;

    for(int i=0;i<10;i++)
    {
        fail |= m_swap_col<rational<>,'r'>();
        fail |= m_swap_col<int,'i'>();
        fail |= m_swap_col<big_int,'b'>();
        fail |= additional_test();
    }

    if(fail) return resFAIL;
    return resOK;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END
}

/* End file matrix_swap_col.cpp */
