/*****************************************************************************

    test/type_pair_traits_is_convertible.cpp

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

TEST
(
    type_pair_traits, is_convertible,
    "Test convertible properties for Arageli and near-Arageli types"
)
{
    TestResult res = resOK;

    #define ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(T1, T2, FUNC, STATUS)    \
        if(type_pair_traits<T1, T2 >::is_specialized)    \
        {    \
            if(type_pair_traits<T1, T2 >::FUNC != STATUS)    \
            {    \
                res = resFAIL;    \
                tout    \
                    << "type_pair_traits<" << #T1 << ", "    \
                    << #T2 << ">::" << #FUNC << " != "    \
                    << #STATUS"\n";    \
            }    \
        }    \
        else    \
        {    \
            tout    \
                << "!type_pair_traits<" << #T1 << ", "    \
                << #T2 << ">::is_specialized\n";    \
        }

    #define ARAGELI_TESTSYS_TEST_BUITIN_PAIR6x(T1, T2, STATUS)    \
        ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(T1, T2, is_convertible, STATUS)    \
        ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(T1, T2, is_safe_convertible, STATUS)    \
        ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(T1, T2, is_assignable, STATUS)    \
        ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(T1, T2, is_safe_assignable, STATUS)    \
        ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(T1, T2, is_initializable, STATUS)    \
        ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(T1, T2, is_safe_initializable, STATUS)

    // The following is only some of the possibilites.
    // We need to add more.

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR6x(big_int, big_int, true);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR6x(rational<>, rational<>, true);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR6x(big_int, rational<>, true);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR6x(rational<char>, rational<int>, true);

    #ifdef ARAGELI_INT64_SUPPORT
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR6x(rational<int>, rational<signed __int64>, true);
    #endif

    #ifdef ARAGELI_LONG_LONG_SUPPORT
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR6x(rational<int>, rational<long long>, true);
    #endif

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR6x(int, big_int, true);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR6x(bool, big_int, true);

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(double, big_int, is_safe_initializable, false)

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(big_int, int, is_safe_convertible, false)
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(big_int, bool, is_safe_assignable, false)
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(big_int, double, is_safe_initializable, false)

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR6x(int, rational<> , true);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR6x(bool, rational<> , true);

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(double, rational<> , is_safe_initializable, false)

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(rational<> , int, is_safe_convertible, false)
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(rational<> , bool, is_safe_assignable, false)
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(rational<> , double, is_safe_initializable, false)

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR6x(int, rational<int> , true);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR6x(bool, rational<int> , true);
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(double, rational<int> , is_safe_initializable, false)

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(rational<int> , int, is_safe_convertible, false)
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(rational<int> , bool, is_safe_assignable, false)
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(rational<int> , double, is_safe_initializable, false)

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(rational<int> , rational<char>, is_safe_convertible, false)
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(rational<int> , rational<char>, is_safe_assignable, false)
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(rational<int> , rational<char>, is_safe_initializable, false)

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(rational<short> , rational<int>, is_safe_convertible, true)
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(rational<short> , rational<int>, is_safe_assignable, true)
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(rational<short> , rational<int>, is_safe_initializable, true)

    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(rational<short> , rational<big_int>, is_safe_convertible, true)
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(rational<short> , rational<big_int>, is_safe_assignable, true)
    ARAGELI_TESTSYS_TEST_BUITIN_PAIR1(rational<short> , rational<big_int>, is_safe_initializable, true)

    bool_type<type_pair_traits<big_int, big_int>::is_convertible>::value;

    return res;
}
