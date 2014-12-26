/*****************************************************************************

    test/polyhedron_distance.cpp

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
    \file polyhedron_distance.cpp
    \brief This file includes test for polyhedron_distance.

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
    const char* sodist
)
{
    typedef matrix<rational<> > M;
    M mp1(sp1), mp2(sp2);
    polyhedron<> p1(mp1, fromvert), p2(mp2, fromvert);
    rational<> odist(sodist);
    rational<> dist = distance2(p1, p2);
    if(dist != odist)
    {
        tout
            << "Incorrect distance2 result:"
            << "\n\tsp1 = " << sp1
            << "\n\tsp2 = " << sp2
            << "\n\tdistance2 = " << dist
            << "\n\todist     = " << odist << '\n';
        return false;
    }
    return true;
}


}


// CHOOSE ONE OF THE FOLLOWING THREE HEADERS

TEST_FUNCTION
(
    distance2,
    "Test for polyhedron_distance function."
)
{
    bool is_ok = true;

    try
    {
        is_ok &= correct_prof("((0))", "((0))", "0");
        is_ok &= correct_prof("((1))", "((0))", "1");
        is_ok &= correct_prof("((0))", "((1))", "1");
        is_ok &= correct_prof("((1))", "((1))", "0");
        is_ok &= correct_prof("((1))", "((2))", "1");
        is_ok &= correct_prof("((0, 0))", "((0, 0))", "0");
        is_ok &= correct_prof("((0, 0))", "((1, 1))", "2");
        is_ok &= correct_prof("((0, 0, 0))", "((1, 1, 1))", "3");
        is_ok &= correct_prof("((-1, -1), (+1, -1), (-1, +1), (+1, +1))", "((0, 0), (1, 1))", "-1");
        is_ok &= correct_prof("((-1, -1), (+1, -1), (-1, +1), (+1, +1))", "((1, 1))", "0");
        is_ok &= correct_prof("((-1, -1), (+1, -1), (-1, +1), (+1, +1))", "((1, 0))", "0");
        is_ok &= correct_prof("((-1, -1), (+1, -1), (-1, +1), (+1, +1))", "((0, 0))", "-1");
        is_ok &= correct_prof("((-1, -1), (+1, -1), (-1, +1), (+1, +1))", "((2, 2))", "2");
        is_ok &= correct_prof("((-1, -1), (+1, -1), (-1, +1), (+1, +1))", "((1, 2))", "1");
        is_ok &= correct_prof("((-1, -1), (+1, -1), (-1, +1), (+1, +1))", "((2, 1))", "1");
        is_ok &= correct_prof("((-1, -1), (+1, -1), (-1, +1), (+1, +1))", "((2, 0))", "1");
        is_ok &= correct_prof("((-1, -1), (+1, -1), (-1, +1), (+1, +1))", "((1/2, 1/2))", "-1/4");
        is_ok &= correct_prof("((-1, -1), (+1, -1), (-1, +1), (+1, +1))", "((1, 1), (2, 2)))", "0");
        is_ok &= correct_prof("((-1, -1), (+1, -1), (-1, +1), (+1, +1))", "((2, 2), (2, -2)))", "1");

        is_ok &= correct_prof
        (
            "((-1, -1, -1),"
            " (+1, -1, -1),"
            " (-1, +1, -1),"
            " (+1, +1, -1),"
            " (-1, -1, +1),"
            " (+1, -1, +1),"
            " (-1, +1, +1),"
            " (+1, +1, +1))",
            "((2, 2, 2), (3, 3, 3)))",
            "3"
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
