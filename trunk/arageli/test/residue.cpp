/*****************************************************************************

    test/residue.cpp

    This file is a part of the Arageli library test base.

    Copyright (C) 2007 Sergey V. Lobanov

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

/**
    \file residue.cpp
    \brief This file includes test for residue class.

    Test description
*/


#include "stdafx.hpp"
using namespace Arageli;


namespace
{


bool simple_test1()
{

    int num_of_err = 0;
    try
    {
        residue<signed int,signed int> a1(101,13);//a1=101 (mod13)
        signed int r=a1.value();
        if(r != 10)
        {
            tout
                << "Error at simple_test1:\n"
                << "101 mod 13 = "<<r<<"\n";
            ++num_of_err;
        }

        residue<signed int,signed int> a2(-20,13);
        r=a2.value();
        if(r != 6)
        {
            tout
                << "Error at simple_test1:\n"
                << "-20 mod 13 = "<<r<<"\n";
            ++num_of_err;
        }



    }
    catch(const Arageli::exception& e)
    {
        tout << "\n" << e;
        ++num_of_err;
        tout
            << "\nWhere\n";
    }
    catch(const std::exception& e)
    {
        tout << "\n" << e.what();
        ++num_of_err;
        tout
            << "\nWhere\n";
    }
    catch(...)
    {
        tout << "\nUnknown exception\n";
        ++num_of_err;
        tout
            << "\nWhere\n";
    }

    return num_of_err == 0;
}

template <typename T>
bool simple_test2
(
    int seed,
    int coverage
)
{
    int num_of_err = 0;
    for(std::size_t i = 0; i < coverage && num_of_err < 10; ++i) try
    {
        if(0 != 0)
        {
            tout
                << "Error at simple_test1:"
                << "\n";
            ++num_of_err;
        }

    }
    catch(const Arageli::exception& e)
    {
        tout << "\n" << e;
        ++num_of_err;
        tout
            << "\nWhere\n";
    }
    catch(const std::exception& e)
    {
        tout << "\n" << e.what();
        ++num_of_err;
        tout
            << "\nWhere\n";
    }
    catch(...)
    {
        tout << "\nUnknown exception\n";
        ++num_of_err;
        tout
            << "\nWhere\n";
    }

    return num_of_err == 0;
}
}

TEST_CLASS(residue, "General tests for residue class")
{

    int seed = 1, coverage = ARAGELI_TESTSYS_COVERAGE_DEFAULT;

    bool is_ok=true;

    try
    {
        is_ok &= simple_test1();
        //test function
    }
    catch(const Arageli::exception& e)
    {
        tout << e;
        return resEXCEPT;
    }
    catch(const std::exception& e)
    {
        tout << e.what();
        return resEXCEPT;
    }

    return is_ok ? resOK : resFAIL;

}
