/*****************************************************************************

    test/mandatory_object_functions.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    Copyright (C) 2005 Andrey Somsikov
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

#include "stdafx.hpp"


using namespace Arageli;

template<typename T>
bool mandatoryConstFunctions()
{
    bool fail = false;

#define ARAGELI_TS_CHECK(CHF, F)    \
    if(!CHF(F))    \
    {    \
        fail = true;    \
        tout    \
            << "Failed: " << #CHF "(" #F ")\nwith"    \
            << "\n\tT = " << typeid(T).name()    \
            << "\n\t" #F " = " << F << '\n';    \
    }

    // positive tests
    ARAGELI_TS_CHECK(is_null, factory<T>::null());
    ARAGELI_TS_CHECK(is_unit, factory<T>::unit());
    ARAGELI_TS_CHECK(is_opposite_unit, factory<T>::opposite_unit());

    // negative tests
    ARAGELI_TS_CHECK(!is_unit, factory<T>::null());
    ARAGELI_TS_CHECK(!is_opposite_unit, factory<T>::null());
    ARAGELI_TS_CHECK(!is_null, factory<T>::unit());
    ARAGELI_TS_CHECK(!is_opposite_unit, factory<T>::unit());
    ARAGELI_TS_CHECK(!is_null, factory<T>::opposite_unit());
    ARAGELI_TS_CHECK(!is_unit, factory<T>::opposite_unit());

    //if(fail)
    //{
    //    tout << "\nFailed: type = " << typeid(T).name() << '\n';
    //}

    return fail;
}

TEST_FUNCTION
(
    mandatory_const_functions,
    "Tests existance and algorithmic correctness of the "
        "constant related functions that is mandatory for all objects"
)
{
    bool fail = false;

//    fail |= mandatoryConstFunctions<polynom<int> >();
    fail |= mandatoryConstFunctions<sparse_polynom<int> >();
    fail |= mandatoryConstFunctions<monom<int> >();
    fail |= mandatoryConstFunctions<matrix<int> >();
    fail |= mandatoryConstFunctions<vector<int> >();
    fail |= mandatoryConstFunctions<rational<int> >();
    fail |= mandatoryConstFunctions<big_int>();

    fail |= mandatoryConstFunctions<sparse_polynom<big_int> >();
    fail |= mandatoryConstFunctions<monom<big_int> >();
    fail |= mandatoryConstFunctions<matrix<big_int> >();
    fail |= mandatoryConstFunctions<vector<big_int> >();
    fail |= mandatoryConstFunctions<rational<big_int> >();

    fail |= mandatoryConstFunctions<sparse_polynom<rational<int> > >();
    fail |= mandatoryConstFunctions<monom<rational<int> > >();
    fail |= mandatoryConstFunctions<matrix<rational<int> > >();
    fail |= mandatoryConstFunctions<vector<rational<int> > >();

    fail |= mandatoryConstFunctions<sparse_polynom<rational<big_int> > >();
    fail |= mandatoryConstFunctions<monom<rational<big_int> > >();
    fail |= mandatoryConstFunctions<matrix<rational<big_int> > >();
    fail |= mandatoryConstFunctions<vector<rational<big_int> > >();

    fail |= mandatoryConstFunctions<sparse_polynom<residue<int> > >();
    fail |= mandatoryConstFunctions<monom<residue<int> > >();
    fail |= mandatoryConstFunctions<matrix<residue<int> > >();
    fail |= mandatoryConstFunctions<vector<residue<int> > >();


    if (fail)
        return resFAIL;

    return resOK;
}

/* End of file mandatory_object_functions.cpp */
