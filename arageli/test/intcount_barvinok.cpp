/*****************************************************************************

    test/intcount_barvinok.cpp

    This file is a part of the Arageli library test base.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    REPLACE OR/AND REFERENCE ADDITIONAL COPYRIGHTS HERE

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
    \file intcount_barvinok.cpp
    \brief This file includes test for intcount_barvinok.
*/


#include "stdafx.hpp"

using namespace Arageli;


namespace
{

template <typename M>
bool concrete_tests ()
{
    typedef typename M::element_type T;
    bool is_ok = true;

    // Empty polyhedrons.

    {
        M m = "((1), (-1))";
        T c = intcount_barvinok(m);
        if(c != 0)
        {
            tout
                << "Error: c != 0 for"
                << "\n\tan empty polyhedron Ax>=b with (-b|A) = " << m
                << "\n\tM = " << typeid(M).name()
                << "\n\tc = " << c << '\n';
            is_ok = false;
        }
    }
    {
        M m = "((1 , 0), (-1 , 0))";
        T c = intcount_barvinok(m);
        if(c != 0)
        {
            tout
                << "Error: c != 0 for"
                << "\n\tan empty polyhedron Ax>=b with (-b|A) = " << m
                << "\n\tM = " << typeid(M).name()
                << "\n\tc = " << c << '\n';
            is_ok = false;
        }
    }
    {
        M m = "((1 , 0 , 0), (-1 , 0 , 0))";
        T c = intcount_barvinok(m);
        if(c != 0)
        {
            tout
                << "Error: c != 0 for"
                << "\n\tan empty polyhedron Ax>=b with (-b|A) = " << m
                << "\n\tM = " << typeid(M).name()
                << "\n\tc = " << c << '\n';
            is_ok = false;
        }
    }

    // Polytopes without integer points.

    {
        M m = "((1 , 0), (-1 , 2), (1 , -2))";
        T c = intcount_barvinok(m);
        if(c != 0)
        {
            tout
                << "Error: c != 0 for"
                << "\n\tpolytope Ax>=b without integer points with (-b|A) = " << m
                << "\n\tM = " << typeid(M).name()
                << "\n\tc = " << c << '\n';
            is_ok = false;
        }
    }
    {
        M m = "((1 , 0), (-1 , 3), (2 , -3))";
        T c = intcount_barvinok(m);
        if(c != 0)
        {
            tout
                << "Error: c != 0 for"
                << "\n\tpolytope Ax>=b without integer points with (-b|A) = " << m
                << "\n\tM = " << typeid(M).name()
                << "\n\tc = " << c << '\n';
            is_ok = false;
        }
    }
    {
        M m = "((1 , 0 , 0), (-1 , 3 , 0), (-1 , 0 , 3), (1 , -1 , -1))";
        T c = intcount_barvinok(m);
        if(c != 0)
        {
            tout
                << "Error: c != 0 for"
                << "\n\tpolytope Ax>=b without integer points with (-b|A) = " << m
                << "\n\tM = " << typeid(M).name()
                << "\n\tc = " << c << '\n';
            is_ok = false;
        }
    }
    {
        M m = "((1 , 0 , 0), (-1 , 3 , 0), (-1 , 0 , 3), (1 , -3 , 0), (1 , 0 , -3))";
        T c = intcount_barvinok(m);
        if(c != 0)
        {
            tout
                << "Error: c != 0 for"
                << "\n\tpolytope Ax>=b without integer points with (-b|A) = " << m
                << "\n\tM = " << typeid(M).name()
                << "\n\tc = " << c << '\n';
            is_ok = false;
        }
    }

    // Not bodily polytopes.

    {
        M m = "((1 , 0 , 0), (0 , 1 , 0), (0 , 0 , 1), (-1 , 1 , 1), (1 , -1 , -1))";
        T c = intcount_barvinok(m);
        if(c != 2)
        {
            tout
                << "Error: c != 2 for"
                << "\n\tnot bodily polytope Ax>=b with 2 integer points with (-b|A) = " << m
                << "\n\tM = " << typeid(M).name()
                << "\n\tc = " << c << '\n';
            is_ok = false;
        }
    }

    { // 2-cube in 3-space
        M m =
            "((1 , 0 , 0 , 0),"
            "(5 , 0 , 1 , 0),"
            "(-2 , 0 , -1 , 0),"
            "(5 , 0 , 0 , 1),"
            "(-2 , 0 , 0 , -1),"
            "(-1 , -1 , 0 , 0),"
            "(1 , 1 , 0 , 0))";
        T c = intcount_barvinok(m);
        if(c != 4*4)
        {
            tout
                << "Error: c != 4*4 for"
                << "\n\t2-cube in 3-space with 16 integer points with (-b|A) = " << m
                << "\n\tM = " << typeid(M).name()
                << "\n\tc = " << c << '\n';
            is_ok = false;
        }

    }

    // Not bounded polytopes.

    { // whole 1-space
        M m =
            "((1 , 0))";
        T c = intcount_barvinok(m);
        if(c != -1)
        {
            tout
                << "Error: c != -1 for"
                << "\n\twhole 1-space with infinite number of integer points with (-b|A) = " << m
                << "\n\tM = " << typeid(M).name()
                << "\n\tc = " << c << '\n';
            is_ok = false;
        }

    }

    { // whole 2-space
        M m =
            "((1 , 0 , 0))";
        T c = intcount_barvinok(m);
        if(c != -1)
        {
            tout
                << "Error: c != -1 for"
                << "\n\twhole 2-space with infinite number of integer points with (-b|A) = " << m
                << "\n\tM = " << typeid(M).name()
                << "\n\tc = " << c << '\n';
            is_ok = false;
        }

    }

    { // whole 3-space
        M m =
            "((1 , 0 , 0 , 0))";
        T c = intcount_barvinok(m);
        if(c != -1)
        {
            tout
                << "Error: c != -1 for"
                << "\n\twhole 3-space with infinite number of integer points with (-b|A) = " << m
                << "\n\tM = " << typeid(M).name()
                << "\n\tc = " << c << '\n';
            is_ok = false;
        }

    }

    { // octant
        M m =
            "((1 , 0 , 0 , 0),"
            "(0 , 0 , 1 , 0),"
            "(0 , 0 , 0 , 1),"
            "(0 , 1 , 0 , 0))";
        T c = intcount_barvinok(m);
        if(c != -1)
        {
            tout
                << "Error: c != -1 for"
                << "\n\toctant with infinite number of integer points with (-b|A) = " << m
                << "\n\tM = " << typeid(M).name()
                << "\n\tc = " << c << '\n';
            is_ok = false;
        }

    }

    { // quadrant
        M m =
            "((1 , 0 , 0),"
            "(0 , 0 , 1),"
            "(0 , 1 , 0))";
        T c = intcount_barvinok(m);
        if(c != -1)
        {
            tout
                << "Error: c != -1 for"
                << "\n\tquadrant with infinite number of integer points with (-b|A) = " << m
                << "\n\tM = " << typeid(M).name()
                << "\n\tc = " << c << '\n';
            is_ok = false;
        }

    }

    { // bodily 2-ribbon without integer points
        M m =
            "((1 , 0 , 0),"
            "(-1 , 0 , 3),"
            "(2 , 0 , -3))";
        //{
        //    polyhedron<T> p(m, fromineq);
        //    std::ofstream file("p.ps.tex");
        //    output_polyhedron_2d(file, p, 0, 0, 12, 12);
        //}

        T c = intcount_barvinok(m);
        if(c != 0)
        {
            tout
                << "Error: c != 0 for"
                << "\n\tbodily 2-ribbon without integer points with (-b|A) = " << m
                << "\n\tM = " << typeid(M).name()
                << "\n\tc = " << c << '\n';
            is_ok = false;
        }

    }

    { // 2-ribbon without integer points in 3-space
        M m =
            "((1 , 0 , 0 , 0),"
            "(5 , 0 , 0 , 2),"
            "(-5 , 0 , 0 , -2),"
            "(-1 , 0 , 3 , 0),"
            "(2 , 0 , -3 , 0))";

        T c = intcount_barvinok(m);
        if(c != 0)
        {
            tout
                << "Error: c != 0 for"
                << "\n\t2-ribbon without integer points in 3-space with (-b|A) = " << m
                << "\n\tM = " << typeid(M).name()
                << "\n\tc = " << c << '\n';
            is_ok = false;
        }

    }



    return is_ok;
}

}


TEST_FUNCTION(intcount_barvinok, "Description for intcount_barvinok test")
{
    bool is_ok = true;

    try
    {
        is_ok &= concrete_tests<matrix<big_int> >();
        is_ok &= concrete_tests<matrix<int> >();
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
