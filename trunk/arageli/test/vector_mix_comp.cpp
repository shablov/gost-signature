/*****************************************************************************

    test/vector_mix_comp.cpp

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
using Arageli::vector;


namespace
{


template <typename XT1, typename XT2>
bool test_particular (const XT1& xt1, const XT2& xt2, const char* result, const char* name)
{
    //if
    //(
    //    typeid(typename function_traits_plus<vector<T1>, vector<T2> >::result_type) !=
    //    typeid(vector<typename function_traits_plus<T1, T2>::result_type))
    //)
    //{
    //    tout
    //        << "\nIncorrect return type: \n"
    //        << typeid(typename function_traits_plus<vector<T1>,
    //            vector<T2> >::result_type).name()
    //        << "\nNeeded:\n"
    //        << typeid(vector<typename function_traits_plus<T1, T2>::result_type)).name();

    //    return true;
    //}

    typename binary_function_traits<function_tag::plus, const XT1&, const XT2&>::result_type
        xt1_plus_xt2 = xt1 + xt2,
        correct_plus = result;

    if(xt1_plus_xt2 != correct_plus)
    {
        tout
            << "\nIncorrect operator+ result for " << name << ": " << xt1_plus_xt2
            << "\nNeeded: " << correct_plus
            << "\nOriginal result string: " << result;
        return true;
    }

    return false;
}



template <typename T1, typename T2>
bool test ()
{
    ARAGELI_ASSERT_ALWAYS((type_pair_traits<T1, T2>::is_convertible));
    ARAGELI_ASSERT_ALWAYS((type_pair_traits<T2, T1>::is_convertible));

    T1 t1a = 2;
    T2 t2a = 3;

    vector<T1> vt1a = "(1, 2, 3, 4)";
    vector<T2> vt2a = "(5, 6, 7, 8)";

    // This condition is satisfied only if we have mixed computation
    // mechanism alive. But Arageli doesn't have it now yet; and this
    // section is commented out.
    #if 0

    if
    (
        typeid(typename binary_function_traits
            <function_tag::plus, vector<T1>, vector<T2> >::result_type) !=
        typeid(vector<typename binary_function_traits
            <function_tag::plus, T1, T2>::result_type>)
    )
    {
        tout
            << "\nIncorrect return type: \n"
            << typeid(typename binary_function_traits<function_tag::plus, vector<T1>,
                vector<T2> >::result_type).name()
            << "\nNeeded:\n"
            << typeid(vector<typename binary_function_traits
                <function_tag::plus, T1, T2>::result_type>).name();

        return true;
    }

    #endif

    return
        test_particular
        (
            vector<T1>("(1, 2, 3, 4)"),
            T2(3),
            "(4, 5, 6, 7)",
            "VT1 by T2"
        )
        ||
        test_particular
        (
            T1(2),
            vector<T2>("(5, 6, 7, 8)"),
            "(7, 8, 9, 10)",
            "T1 by VT2"
        )
        ||
        test_particular
        (
            vector<T1>("(1, 2, 3, 4)"),
            vector<T2>("(5, 6, 7, 8)"),
            "(6, 8, 10, 12)",
            "VT1 by VT2"
        )
        ||
        test_particular
        (
            vector<vector<T1> >("((1, 2), (3, 4))"),
            T2(3),
            "((4, 5), (6, 7))",
            "VT1 by T2"
        )
        ||
        test_particular
        (
            T1(2),
            vector<vector<T2> >("((5, 6), (7), (8))"),
            "((7, 8), (9), (10))",
            "T1 by VT2"
        )
        ||
        test_particular
        (
            vector<vector<T1> >("((1), (2, 3, 4))"),
            vector<vector<T2> >("((5), (6, 7, 8))"),
            "((6), (8, 10, 12))",
            "VT1 by VT2"
        )
        ||
        test_particular
        (
            vector<vector<T1> >("((1, 2), (3, 4))"),
            vector<T2>("(10, 20)"),
            "((11, 22), (13, 24))",
            "VT1 by T2"
        )
        ||
        test_particular
        (
            vector<T1>("(2, 3)"),
            vector<vector<T2> >("((5, 6), (6, 7), (7, 8), (8, 9))"),
            "((7, 9), (8, 10), (9, 11), (10, 12))",
            "T1 by VT2"
        );
}

}


TEST_FUNCTION
(
    vector_mix_comp,
    "Test mixed computations with vector."
)
{
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN

    bool fail = false;

    fail |= test<int, int>();
    fail |= test<big_int, int>();
    fail |= test<int, big_int>();
    fail |= test<int, rational<int> >();
    fail |= test<big_int, rational<int> >();
    fail |= test<int, rational<big_int> >();
    fail |= test<rational<int>, rational<int> >();
    fail |= test<rational<big_int>, rational<int> >();
    fail |= test<rational<int>, rational<big_int> >();

    if(fail)return resFAIL;
    else return resOK;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END
}
