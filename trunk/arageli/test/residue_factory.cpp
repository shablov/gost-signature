/*****************************************************************************

    test/residue_factory.cpp

    This file is a part of the Arageli library test base.

    Copyright (C) 2005--2007 Sergey S. Lyalin

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
    \file residue_factory.cpp
    \brief This file includes test for factory specialization for residue.

    Tests several concrete cases with residue and factory functions:
    unit, null, opposite_unit. Tests correct behaviour of factory
    functons with an additional optional parameter.
*/


#include "stdafx.hpp"

using namespace Arageli;


namespace
{

// True if there is the bug.
template <typename T>
bool bug_1817521 (const T& val, const T& mod)
{
    residue<T> z(val, mod);
    residue<T> z2 = unit<residue<T> >(z);
    if(!is_unit(z2.value()) || z2.module() != mod)
    {
        tout
            << "There is 1817521 bug."
            << "\n\tT = " << typeid(T).name()
            << "\n\tz2.value() = " << z2.value()
            << "\n\tz2.module() = " << z2.module()
            << "\n";
        return true;
    }
    return false;
}

// Returns true if opposite_unit construction is OK. Only for numbers.
template <typename T>
bool opposite_unit_is_correct (const T& mod)
{
    residue<T> z(unit<T>(), mod);
    residue<T> z2 = opposite_unit(z);
    if(z2.value() != mod - 1)
    {
        tout
            << "Failed: opposite_unit_is_correct with"
            << "\n\tT = " << typeid(T).name()
            << "\n\tmod = " << mod
            << "\n\tz = " << z
            << "\n\tz2 = " << z2
            << "\n";
        return false;
    }
    return true;
}

}


TEST
(
    resudue,
    factory_unit_null_opposite_unit,
    "Test for factory functions specialized for residue class."
)
{
    bool is_ok = true;

    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_BEGIN
    {
        is_ok &= !bug_1817521<signed char>(2, 3);
        is_ok &= !bug_1817521<signed short>(2, 3);
        is_ok &= !bug_1817521<signed int>(2, 3);
        is_ok &= !bug_1817521<signed long>(2, 3);
        is_ok &= !bug_1817521<big_int>(2, 3);
        is_ok &= !bug_1817521<sparse_polynom<rational<> > >("x+2", "x^2+2*x+1");

        is_ok &= opposite_unit_is_correct<signed char>(5);
        is_ok &= opposite_unit_is_correct<signed char>(6);
        //is_ok &= opposite_unit_is_correct<unsigned char>(5);
        //is_ok &= opposite_unit_is_correct<unsigned char>(6);
        is_ok &= opposite_unit_is_correct<signed int>(5);
        is_ok &= opposite_unit_is_correct<signed int>(6);
        //is_ok &= opposite_unit_is_correct<unsigned int>(5);
        //is_ok &= opposite_unit_is_correct<unsigned int>(6);
        is_ok &= opposite_unit_is_correct<big_int>(5);
        is_ok &= opposite_unit_is_correct<big_int>(6);
    }
    ARAGELI_TS_ALLEXCEPT_CATCH_REGION_END

    return is_ok ? resOK : resFAIL;
}
