/*****************************************************************************

    skeleton.hpp -- algorithms for double-description of a convex polyhedron.

    This file is a part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
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
    Algorithms for double-description of a convex polyhedron.
    Implementation of skeleton function. Now this function just calls
    skeleton_motzkin_burger function.
*/

#ifndef _ARAGELI_skeleton_hpp_
#define _ARAGELI_skeleton_hpp_

#include "config.hpp"
#include "motzkin_burger.hpp"


namespace Arageli
{


namespace ctrl
{

struct skeleton_idler
{
    class abort :
        public ctrl::abort
    {};

    skeleton_motzkin_burger_idler motzkin_burger_ctrl () const
    {
        return skeleton_motzkin_burger_idler();
    }
};

}


/// The algorithm for double-description of a polyhedal cone.
/** Finds extreme rays f and basis e of a cone ax >= 0
    returns also an incidence matrix q = f * transpose(a). */
template
<
    typename A,
    typename F,
    typename Q,
    typename E,
    typename Ctrler
>
inline void skeleton
(
    A& a,
    F& f,
    Q& q,
    E& e,
    const Ctrler& ctrler
)
{
    skeleton_motzkin_burger(a, f, q, e, ctrler.motzkin_burger_ctrl());
}


/// The algorithm for double-description of a polyhedal cone.
/** Just calls full version of the function. */
template <typename A, typename F, typename Q, typename E>
inline void skeleton (A& a, F& f, Q& q, E& e)
{
    return skeleton (a, f, q, e, ctrl::skeleton_idler());
}


} // namesapce Arageli


#if 0
#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_SKELETON
    #include "skeleton.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_SKELETON
#endif
#endif

#endif
