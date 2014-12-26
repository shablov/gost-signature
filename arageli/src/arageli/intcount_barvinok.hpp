/*****************************************************************************

    intcount_barvinok.hpp

    This file is a part of the Arageli library.

    An implementation of all algorithms in this file
    have been done by Ekaterina Schukina.

    An integration of the implemented algorithms
    into Arageli and extention to not bodily polytopes
    by Sergey Lyalin.

    Copyright (C) 2006 Ekaterina Shchukina
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
    \file intcount_barvinok.hpp
    \brief The algorithm that counts all integer points in polytope.

    This file contains declaration for the Barvinok algorithm for counting
    integer points in a polytope.  A polytop Ax >= b is represented as
    a matrix (-b|A) with additional (1, 0, ..., 0) row if this inequality
    doesn't follow from others.
*/

//TODO: This file needs to rewrite with new approaches. Polyhedrons, for example.

#ifndef _ARAGELI_intcount_barvinok_hpp_
#define _ARAGELI_intcount_barvinok_hpp_

#include "config.hpp"

#include "_utility.hpp"
#include "exception.hpp"


namespace Arageli
{


namespace ctrl
{

struct intcount_barvinok_idler    // Need to complete.
{
    class abort :
        public Arageli::ctrl::abort
    {};
};

}


/// Counts all integer points in polytope.
template <typename A, typename T, typename Ctrler>
void intcount_barvinok (const A& a, T& res, Ctrler ctrler);


/// Counts all integer points in polytope.
template <typename A, typename T>
inline void intcount_barvinok (const A& a, T& res)
{
    intcount_barvinok(a, res, ctrl::intcount_barvinok_idler());
}


/// Counts all integer points in polytope.
template <typename A>
inline typename A::element_type intcount_barvinok (const A& a)
{
    typename A::element_type res;
    intcount_barvinok(a, res, ctrl::intcount_barvinok_idler());
    return res;
}


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_INTCOUNT_BARVINOK
    #include "intcount_barvinok.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_INTCOUNT_BARVINOK
#endif

#endif
