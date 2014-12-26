/*****************************************************************************

    test/minkowski_diff.cpp

    This file is a part of the Arageli library test base.

    Copyright (C) 2007 Sergey S. Lyalin

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
    \file minkowski_diff.cpp
    \brief This file includes test for minkowski_diff.

    ADD FILE DESCRIPTION HERE
*/


#include "stdafx.hpp"

using namespace Arageli;


namespace
{

bool correct_prof
(
    const char* sp1,
    const char* sp2,
    const char* sorig
)
{
    typedef matrix<rational<> > M;
    M mp1(sp1), mp2(sp2);
    polyhedron<> p1(mp1, fromvert), p2(mp2, fromvert);
    p1.minkowski_diff(p2);
    M mres = p1.vertices<M>(), morig(sorig);
    if(mres != morig)
    {
        tout
            << "mres != morig:\n"
            << "\tsp1   = " << sp1 << '\n'
            << "\tsp2   = " << sp2 << '\n'
            << "\tmres  = " << mres << '\n'
            << "\tmorig = " << morig << '\n';
        return false;
    }
    return true;
}

}


TEST_FUNCTION
(
    minkowski_diff,
    "Test minkowski_diff functions for polyhedron class."
)
{
    bool is_ok = true;

    try
    {
        is_ok &= correct_prof("((0))", "((0))", "((0))");
        is_ok &= correct_prof("((1))", "((0))", "((1))");
        is_ok &= correct_prof("((0))", "((1))", "((-1))");
        is_ok &= correct_prof("((1))", "((1))", "((0))");
        is_ok &= correct_prof("((1, 2))", "((1, 1))", "((0, 1))");
        is_ok &= correct_prof
        (
            "((0, 1), (1, 1))",
            "((0, 0), (1, 0))",
            "((1, 1), (-1, 1))"
        );
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
