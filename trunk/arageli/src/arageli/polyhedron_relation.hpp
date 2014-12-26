/*****************************************************************************

    polyhedron_relation.hpp

    This file is a part of the Arageli library.

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
    \file polyhedron_relation.hpp
    \brief Routines for determination of various relations between polyhedra.

    This file containes routines to determine various relations between and
    properties of polyhedra. The relations are distance between polyhedra,
    angle between two subspaces.
*/


#ifndef _ARAGELI_polyhedron_relation_hpp_
#define _ARAGELI_polyhedron_relation_hpp_

#include "config.hpp"

// REFERENCE ADDITIONAL HEADERS HERE


namespace Arageli
{


/// Square of the Euclid minimum distance between a polyhedron and the origin. A simple algorithm.
/** What the distance does mean, see in the comments for origin_distance2
    function. This function implements a simple algorithm in which all
    facets of a given polyhedron are being enumerated. For each facet we
    find the closest point to the origin recursively. After all the function
    chooses the minimum value and returns it.
*/
template <typename P1>
typename P1::value_type
origin_distance2_complete_facet_enum (const P1& p1);


/// Square of the Euclid minimum distance between a polyhedron and the origin with the closest point. A simple algorithm.
/** What the distance does mean, see in the comments for origin_distance2
    function. This function implements a simple algorithm in which all
    facets of a given polyhedron are being enumerated. For each facet we
    find the closest point to the origin recursively. After all the function
    chooses the minimum value and returns it.

    @param p1           in: polyhedron for investigation
    @param closest      out: the closest to the origin point in p1
*/
template <typename P1, typename Closest>
typename P1::value_type
origin_distance2_complete_facet_enum (const P1& p1, Closest& closest);


/// Square of the Euclid minimum distance between a polyhedron and the origin.
/** The functions determines minimum distance between a given polyhedron
    in the Euclid space. If a polyhedron P1 doesn't contain the origin,
    minimum distance is
    min{euclid_norm(x1) | for all x1 in P1}.
    The function returns square value of the distance value.
    If a polyhedra P1 does contain the origin, the minimum distance is
    -min{euclid_norm(x1) | for all x1 in Delta(P1)},
    where Delta(P1) is frontier of P1.
*/
template <typename P1>
inline typename P1::value_type origin_distance2 (const P1& p1)
{
    return origin_distance2_complete_facet_enum(p1);
}


/// Square of the Euclid minimum distance between two polyhedra.
/** The functions determines minimum distance between two polyhedra
    in the Euclid space. If two polyhedra P1 and P2 aren't intersected,
    minimum distance between them is
    min{euclid_norm(x1) | for all x1 in P1, x2 in P2}.
    The function returns square value of the distance value.
    If two polyhedra P1 and P2 are intersected, the minimum distance is
    the length of minimum translation of one of the polyhedra to
    avoid the penetration.
*/
template <typename P1, typename P2>
typename P1::value_type distance2 (const P1& p1, const P2& p2);


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_polyhedron_relation
    #include "polyhedron_relation.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_polyhedron_relation
#endif

#endif    // #ifndef _ARAGELI_polyhedron_relation_hpp_
