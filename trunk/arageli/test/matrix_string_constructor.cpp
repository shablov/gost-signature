/*****************************************************************************

    test/matrix_string_constructor.cpp

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

// File : matrix_string_constructor.cpp
// Author : Zaitsev Mikhail
// Tester : Aleksandrov Vladimir
// Date of creation : 2006/01/31
// Date of modification : 2006/02/03
// Testing date :
// Description : Test for function matrix(const char* m)
// Using functions :

#include "stdafx.hpp"

using namespace Arageli;


template <class T,char s>
bool m_string_constructor()
{
    RNG element(2,16);

        int cols=1+element.Rand()%5;
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

    std::string array=buff.str();
    for(int i=0;i<rows+1;i++)
    {
        array.replace(array.find("("),1,"");
        array.replace(array.find(")"),1,"");
    }
    array.insert(array.begin(),1,'(');
    array.insert(array.end(),1,')');

    Arageli::vector<T> matrix_analog(array.c_str());//vector(matrix analog)

    if(A.nrows()!=rows || A.ncols()!=cols)
    {
        tout<<"function failed with"<<buff.str();
        return true;
    }

    if(A.size()!=matrix_analog.size())
    {
        tout<<"function failed with"<<buff.str();
        return true;
    }

    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            if(A(i,j)!=matrix_analog(cols*i+j))
            {
                tout<<"function failed with"<<buff.str();
                return true;

            }

    return false;

}


TEST(matrix,str_const,"Test for matrix string constructor")
{
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN

    bool fail=false;
    for(int i=0;i<100;i++)
    {
        fail |=m_string_constructor<rational<>,'r'>();
        fail |=m_string_constructor<int,'i'>();
        fail |=m_string_constructor<big_int,'b'>();
    }

    if(fail) return resFAIL;
    return resOK;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END
}


/* End of file matrix_string_constructor.cpp */
