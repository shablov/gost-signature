/*****************************************************************************

    test/matrix_swap_row.cpp

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

// File : matrix_swap_row.cpp
// Author : Zaitsev Mikhail
// Tester : Aleksandrov Vladimir
// Date of creation : 2006/02/02
// Date of modification :
// Testing date :
// Using functions : swap_rows(),copy_row(),insert_row(),erase_row(),nrows()


#include "stdafx.hpp"

using namespace Arageli;


template <class T,char s>
bool m_swap_row()
{
    RNG element(2,16);

    int cols=1+element.Rand()%5;
    int rows=2+element.Rand()%5;

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
    matrix<T> X=A;// copy of testing matrix


    for(int i=0;i<A.nrows();i++)
        for(int j=i+1;j<A.nrows();j++)
        {
            A.swap_rows(i,j);

            //my own algorithm of rows' swapping
            X.insert_row(i,X.copy_row(j));
            X.insert_row(j+1,X.copy_row(i+1));
            X.erase_row(i+1);
            X.erase_row(j+1);

            if(A!=X)
            {
                tout<<"function failed with"<<buff.str();
                return true;

            }

            //A.swap_rows(i,j);

        }

    return false;

}


TEST(matrix,swap_rows,"Test for function swap_rows")
{
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN

    bool fail=false;

    for(int i=0;i<10;i++)
    {
        fail |=m_swap_row<rational<>,'r'>();
        fail |=m_swap_row<int,'i'>();
        fail |=m_swap_row<big_int,'b'>();
    }

    if(fail) return resFAIL;
    return resOK;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END

}

/* End file matrix_swap_row.cpp */
