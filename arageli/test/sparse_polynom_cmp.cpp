/*****************************************************************************

    test/sparse_polynom_cmp.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
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

/* /////////////////////////////////////////////////////////////////////////////
//
//  File:       sparse_polynom_cmp.cpp
//  Created:    16.01.2005(? strange...)
//
//  Author: Sergey Lyalin
*/


#include "stdafx.hpp"


using namespace Arageli;

template <typename P1, typename P2>
bool sparse_polynom_cmp_test ()
{
    bool res = false;
    std::ostringstream buftout;

    typedef oldrnd<P1> Random_P1;
    typedef oldrnd<P2> Random_P2;

    for(int i = 0; i < 100; ++i)try
    {
        P1 p11 = Random_P1::rand();

        try
        {
            if(cmp(p11, p11) != 0)
            {
                tout << "cmp(" << p11 << ", the same) != 0\n";
            }
        }
        catch(const Arageli::exception&)
        {
            buftout
                << "\nWHEN: cmp(" << p11 << ", the same) != 0\n";
            throw;
        }

        //try
        //{
        //    if(!is_null(p11) && cmp(p11, -p11) != +1)
        //    {
        //        tout << "!is_null(" << p11 << ") && cmp(the same, " << -p11 << ") != +1\n";
        //    }
        //}
        //catch(const Arageli::exception&)
        //{
        //    buftout
        //        << "\nWHEN: !is_null(" << p11 << ") && cmp(the same, " << -p11 << ") != +1\n";
        //    throw;
        //}

        //try
        //{
        //    if(!is_null(p11) && cmp(-p11, p11) != -1)
        //    {
        //        tout << "!is_null(" << p11 << ") && cmp(" << -p11 << ", the same) != -1\n";
        //    }
        //}
        //catch(const Arageli::exception&)
        //{
        //    buftout
        //        << "\nWHEN: !is_null(" << p11 << ") && cmp(" << -p11 << ", the same) != -1\n";
        //    throw;
        //}

        try
        {
            if(!is_null(p11) && cmp(p11*P1("x"), p11) != +1)
            {
                tout << "!is_null(" << p11 << ") && cmp(" << p11*P1("x") << ", the same) == +1\n";
            }
        }
        catch(const Arageli::exception&)
        {
            buftout
                << "\nWHEN: !is_null(" << p11 << ") && cmp(" << p11*P1("x") << ", the same) == +1\n";
            throw;
        }

        P2 p21 = Random_P2::rand();

        try
        {
            if(cmp(p11, p21) * cmp(p21, p11) > 0)
            {
                tout << "cmp(" << p11 << ", " << p21 << ") * cmp(" << p21 << ", " << p11 << ") <= 0\n";
            }
        }
        catch(const Arageli::exception&)
        {
            buftout
                << "\nWHEN: cmp(" << p11 << ", " << p21 << ") * cmp(" << p21 << ", " << p11 << ") <= 0\n";
            throw;
        }

    }
    catch(const Arageli::exception& e)
    {
        tout << "EXCEPTION: ";
        e.output(tout); tout << '\n';
        tout
            << "WITH:"
            << "\n\tP1 = " << typeid(P1).name()
            << "\n\tP2 = " << typeid(P2).name();
        std::string buf = buftout.str();
        if(!buf.empty())
            tout << buf;
        tout << '\n';
        res = true;
    }

    return res;
}

TEST_FUNCTION(sparse_polynom_cmp, "Test cmp function for sparse_polynom class.")
{
    bool fail = false;

    fail |= sparse_polynom_cmp_test<sparse_polynom<int>, sparse_polynom<int> >();
    fail |= sparse_polynom_cmp_test<sparse_polynom<int>, sparse_polynom<big_int> >();
    fail |= sparse_polynom_cmp_test<sparse_polynom<int>, sparse_polynom<rational<> > >();
    fail |= sparse_polynom_cmp_test<sparse_polynom<big_int>, sparse_polynom<int> >();
    fail |= sparse_polynom_cmp_test<sparse_polynom<big_int>, sparse_polynom<big_int> >();
    fail |= sparse_polynom_cmp_test<sparse_polynom<big_int>, sparse_polynom<rational<> > >();
    fail |= sparse_polynom_cmp_test<sparse_polynom<rational<> >, sparse_polynom<int> >();
    fail |= sparse_polynom_cmp_test<sparse_polynom<rational<> >, sparse_polynom<big_int> >();
    fail |= sparse_polynom_cmp_test<sparse_polynom<rational<> >, sparse_polynom<rational<> > >();

    if(fail)
        return resFAIL;

    return resOK;
}
