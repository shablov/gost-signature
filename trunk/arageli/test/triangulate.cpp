/*****************************************************************************

    test/triangulate.cpp -- test for triangulate functions.

    This file is a part of the Arageli library test base.

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

/**
    \file
    This file includes test for triangulate.
*/


#include "stdafx.hpp"

using namespace Arageli;


namespace
{


template <typename Q, typename Tr, typename Dim>
bool concrete_test ()
{
    vector<Q, false> q =
        "(((1)),"
        " (),"
        " ((0, 1), (1, 0)),"
        " ((1, 0, 0), (0, 1, 0), (0, 0, 1)),"
        " ((0, 1, 0), (1, 0, 0), (0, 0, 1)),"
        " ((0, 1, 1, 0), (0, 0, 1, 1), (1, 0, 0, 1), (1, 1, 0, 0)),"
        " ((0, 1, 1, 1, 0), (0, 0, 1, 1, 1), (1, 0, 0, 1, 1), (1, 1, 0, 0, 1), (1, 1, 1, 0, 0))"
        ")";

    vector<Tr, false> ctr =
        "(((0)),"
        " (()),"
        " ((0, 1)),"
        " ((0, 1, 2)),"
        " ((0, 1, 2)),"
        " ((0, 1, 2), (0, 2, 3)),"
        " ((0, 1, 2), (0, 2, 3), (0, 3, 4))"
        ")";

    vector<Dim> dim   = "(1, 0, 2, 3, 3, 3, 3, 3, 3)";

    bool valid = true;

    for(std::size_t i = 0; i < q.size(); ++i)
    {
        Tr tr;
        triangulate_simple_1(q[i], dim[i], tr, 0);
        if(tr != ctr[i])
        {
            tout
                << "\ntr != trcorrect:"
                << "\n\tq = [" << q[i].nrows() << ", " << q[i].ncols() << "]" << q[i]
                << "\n\ttr = [" << tr.nrows() << ", " << tr.ncols() << "]" << tr
                << "\n\ttrcorrect = [" << ctr[i].nrows() << ", " << ctr[i].ncols() << "]" << ctr[i]
                << "\n\tdim = " << dim[i]
                ;

            valid = false;
        }
    }

    return valid;
}


}


TEST_FUNCTION(triangulate, "Description for triangulate functions test")
{
    bool is_ok = true;

    try
    {
        is_ok &= concrete_test<matrix<big_int>, matrix<big_int>, big_int>();
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
