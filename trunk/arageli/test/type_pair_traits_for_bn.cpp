/*****************************************************************************

    test/type_pair_traits_for_bn.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    Copyright (C) 2006--2007 Sergey S. Lyalin

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

#include "stdafx.hpp"


using namespace Arageli;

TEST_FUNCTION
(
    type_pair_traits_for_bn,
    "Test convertible properties for buit-in numerics"
)
{
    TestResult res = resOK;

    #define ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(T1, T2)    \
        if(type_pair_traits<T1, T2>::is_specialized)    \
        {    \
            if(!type_pair_traits<T1, T2>::is_convertible)    \
            {    \
                res = resFAIL;    \
                tout    \
                    << "!type_pair_traits<" << #T1 << ", "    \
                    << #T2 << ">::is_convertible\n";    \
            }    \
            if(!type_pair_traits<T1, T2>::is_safe_convertible)    \
            {    \
                res = resFAIL;    \
                tout    \
                    << "!type_pair_traits<" << #T1 << ", "    \
                    << #T2 << ">::is_safe_convertible\n";    \
            }    \
        }    \
        else    \
        {    \
            tout    \
                << "!type_pair_traits<" << #T1 << ", "    \
                << #T2 << ">::is_specialized\n";    \
        }

    // The following is only some of the possibilites.
    // We need to add more.

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(bool, char);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(bool, signed char);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(bool, unsigned char);

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(bool, short);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(bool, int);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(bool, long);

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(bool, float);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(bool, double);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(bool, long double);

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(bool, unsigned short);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(bool, unsigned int);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(bool, unsigned long);

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(signed char, short);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(short, int);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(int, int);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(int, long);

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(float, double);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(double, long double);

    return res;
}
