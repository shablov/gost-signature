/*****************************************************************************

    test/polynom_init_convert.cpp

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
    \file polynom_init_convert.cpp
    \brief This file includes test for dense polynomial initialization and
        converttion.

    Several tests with concrete data to check initialization for polynom
    template class and convertion to other types.
*/


#include "stdafx.hpp"

using namespace Arageli;


namespace
{

template <typename P>
bool init_concrete ()
{
    bool is_ok = true;

    {
        P p;

        if
        (
            !p.is_null() ||
            p.size() != 0 ||
            !p.is_empty() ||
            p.is_unit() ||
            p.is_opposite_unit() ||
            !is_null(p.leading_coef_cpy())
        )
        {
            tout
                << "\n\tp = " << p
                << "Error at {P p} stage of init_concrete:"
                << "\n\t!p.is_null() = " << !p.is_null()
                << "\n\tp.size() != 0 = " << p.size()
                << "\n\t!p.is_empty() = " << !p.is_empty()
                << "\n\tp.is_unit() = " << p.is_unit()
                << "\n\tp.is_opposite_unit() = " << p.is_opposite_unit()
                << "\n\t!is_null(p.leading_coef_cpy()) = " << !is_null(p.leading_coef_cpy())
                << std::endl;
            is_ok = false;
        }
    }
    {
        P p("");

        if
        (
            !p.is_null() ||
            p.size() != 0 ||
            !p.is_empty() ||
            p.is_unit() ||
            p.is_opposite_unit() ||
            !is_null(p.leading_coef_cpy()) ||
            p != P()
        )
        {
            tout
                << "Error at {P p("")} stage of init_concrete:"
                << "\n\tp = " << p
                << "\n\t!p.is_null() = " << !p.is_null()
                << "\n\tp.size() != 0 = " << p.size()
                << "\n\t!p.is_empty() = " << !p.is_empty()
                << "\n\tp.is_unit() = " << p.is_unit()
                << "\n\tp.is_opposite_unit() = " << p.is_opposite_unit()
                << "\n\t!is_null(p.leading_coef_cpy()) = " << !is_null(p.leading_coef_cpy())
                << "\n\tp != P() = " << (p != P()) << std::endl;
            is_ok = false;
        }
    }
    {
        P p(1, fromval);

        if(p != P("1"))
        {
            tout
                << "Error at {P p(1, fromval)} stage of init_concrete:"
                << "\n\tp = " << p
                << "\n\tp != P(\"1\")" << std::endl;
            is_ok = false;
        }
    }
    {
        P p(1, 2, fromval);

        if(p != P("x^2") || p != P("1*x^2"))
        {
            tout
                << "Error at {P p(1, 2, fromval)} stage of init_concrete:"
                << "\n\tp != P(\"x^2\") || p != P(\"1*x^2\")"
                << "\n\tp = " << p
                << "\n\tP(\"x^2\") = " << P("x^2")
                << "\n\tP(\"1*x^2\") = " << P("1*x^2")
                << std::endl;
            is_ok = false;
        }
    }
    {
        sparse_polynom<typename P::coef_type> p1;
        P p = p1;
        sparse_polynom<typename P::coef_type> p2 = p;

        if(p1 != p2)
        {
            tout
                << "Error at {sp.poly -> poly -> sp.poly} stage of init_concrete:"
                << "\n\tp1 = " << p1
                << "\n\tp = " << p
                << "\n\tp2 = " << p2
                << std::endl;
            is_ok = false;
        }
    }
    {
        P p1;
        sparse_polynom<typename P::coef_type> p = p1;
        P p2 = p;

        if(p1 != p2)
        {
            tout
                << "Error at {poly -> sp.poly -> poly} stage of init_concrete:"
                << "\n\tp1 = " << p1
                << "\n\tp = " << p
                << "\n\tp2 = " << p2
                << std::endl;
            is_ok = false;
        }
    }
    {
        sparse_polynom<typename P::coef_type> p1("x^5-12*x^2");
        P p = p1;
        sparse_polynom<typename P::coef_type> p2 = p;

        if(p1 != p2)
        {
            tout
                << "Error at {sp.poly -> poly -> sp.poly} stage of init_concrete:"
                << "\n\tp1 = " << p1
                << "\n\tp = " << p
                << "\n\tp2 = " << p2
                << std::endl;
            is_ok = false;
        }
    }
    {
        P p1("x^5-12*x^2");
        sparse_polynom<typename P::coef_type> p = p1;
        P p2 = p;

        if(p1 != p2)
        {
            tout
                << "Error at {poly -> sp.poly -> poly} stage of init_concrete:"
                << "\n\tp1 = " << p1
                << "\n\tp = " << p
                << "\n\tp2 = " << p2
                << std::endl;
            is_ok = false;
        }
    }

    return is_ok;
}

}


TEST(polynom, init_convert, "Test for dense polynomial initialization and converttion")
{
    bool is_ok = true;

    try
    {
        is_ok &= init_concrete<polynom<big_int> >();
        is_ok &= init_concrete<polynom<int> >();
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

    return resOK;
}
