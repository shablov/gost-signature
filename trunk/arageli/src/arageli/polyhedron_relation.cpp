/*****************************************************************************

    polyhedron_relation.cpp

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
    \file polyhedron_relation.cpp
    \brief The polyhedron_relation.hpp file stuff implementation.

    ADD ADDITIONAL FILE DESCRIPTION HERE
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_polyhedron_relation)

#include "exception.hpp"
#include "orthogonal.hpp"
#include "sideset.hpp"
#include "vector.hpp"
#include "cmp.hpp"

#include "polyhedron_relation.hpp"


namespace Arageli
{


template <typename P>
typename P::value_type
origin_distance2_complete_facet_enum
(const P& p)
{
    typedef typename P::value_type T;
    vector<T, false> closest;
    return origin_distance2_complete_facet_enum(p, closest);
}


template <typename P, typename Closest>
typename P::value_type
origin_distance2_complete_facet_enum
(const P& p, Closest& closest)
{
    typedef typename P::value_type T;
    typedef typename P::size_type size_type;
    typedef vector<T, false> V;
    typedef sideset::const_facet_iterator FI;
    typedef typename P::equation_type ET;

    ARAGELI_TRACE_BEGIN
    (
        tracing_common
            << tracing_indent()
            << "BEGIN origin_distance2_complete_facet_enum\n"
    );

    ARAGELI_TRACE
    (
        tracing_common
            << tracing_indent()
            << "p.generatrix_matrix() = " << p.generatrix_matrix() << '\n'
            << tracing_indent()
            << "p.basis_matrix() = " << p.basis_matrix() << '\n'
    );

    const V origin(p.space_dim());    // (0, 0, ..., 0)

    if(p.dim() == 0)
    {
        matrix<rational<> > vert = p.template vertices<matrix<rational<> > >();
        ARAGELI_ASSERT_1(vert.nrows() == 1);
        closest = vert.copy_row(0);

        ARAGELI_TRACE
        (
            tracing_common
                << tracing_indent()
                << "p.dim() == 0,\n"
                << tracing_indent()
                << "p.generatrix_matrix() = " << p.generatrix_matrix() << '\n'
                << tracing_indent()
                << "vertices = " << vert << '\n'
                << tracing_indent()
                << "origin inside = " << p.unstrict_inside(origin) << '\n'
        );

        ARAGELI_TRACE_END
        (
            tracing_common
                << tracing_indent()
                << "END origin_distance2_complete_facet_enum\n"
        );

        if(p.unstrict_inside(origin))
            return -dotsquare(vert.copy_row(0));
        else
            return dotsquare(vert.copy_row(0));
    }

    sideset ps = p.sides();
    T mindist;
    V minproj;
    bool firsttime = true;  // to perform the first iteration especially

    // Test if the origin is within p.
    if(p.unstrict_inside(origin))
    {
        // So, the origin is within p.

        ARAGELI_TRACE
        (
            tracing_common
                << tracing_indent()
                << "the origin IS within p\n"
        );

        // Enumerate all hyperplanes corresponding to facets of p
        // and choose minimum distance from the origin.

        for(FI i = ps.facets_begin(); i != ps.facets_end(); ++i)
        {
            // Obtain corresponding support hyperplane for a given facet.
            // WARNING! Here we aren't interested in exact configuration
            // of the facet; only minimum ambient plane is important.
            matrix<rational<> > h = p.facet(i).cone().min_ambient_equation();

            // Project origin to the obtained hyperplane and
            // compute distance between origin and the projection.
            V proj = project_orthogonal_eq_ba(h, origin);
            T dist = dotsquare(proj);

            if(firsttime)
            {
                // This block executes only once for the first iteration.
                mindist = dist;
                minproj = proj;
                firsttime = false;
            }
            else if(dist < mindist)
            {
                mindist = dist;
                minproj = proj;

                if(is_null(dist))
                    break; // we have reached undoubted minimum
            }
        }

        opposite(&mindist);
    }
    else
    {
        // The origin isn't within p.

        ARAGELI_TRACE
        (
            tracing_common
                << tracing_indent()
                << "the origin ISN'T within p\n"
        );

        for(FI i = ps.facets_begin(); i != ps.facets_end(); ++i)
        {
            // Obtain corresponding support hyperplane for a given facet.
            P f = p.facet(i);
            matrix<rational<> > h = f.cone().min_ambient_equation();

            // Project origin to the obtained hyperplane.
            V proj = project_orthogonal_eq_ba(h, origin);

            if(!f.unstrict_inside(proj))
            {
                // The projection isn't within the facet.
                // We recursively call this function for this facet in
                // the new coordinates with the projection as a new origin.

                f.translate(-proj);
                ARAGELI_ASSERT_1(!f.unstrict_inside(origin));
                V point;
                origin_distance2_complete_facet_enum(f, point);
                proj += point;
            }

            T dist = dotsquare(proj);
            ARAGELI_ASSERT_1(!is_null(dist));

            if(firsttime)
            {
                // This block executes only once for the first iteration.
                minproj = proj;
                mindist = dist;
                firsttime = false;
            }
            else if(dist < mindist)
            {
                mindist = dist;
                minproj = proj;
            }
        }
    }

    ARAGELI_ASSERT_1(!firsttime);

    ARAGELI_TRACE_END
    (
        tracing_common
            << tracing_indent()
            << "END origin_distance2_complete_facet_enum\n"
    );

    closest = minproj;
    return mindist;
}


template <typename P1, typename P2>
typename P1::value_type distance2 (const P1& p1, const P2& p2)
{
    // Compute Minkowski difference between given polyhedra.
    P1 p = p1;
    p.minkowski_diff(p2);

    // Compute the distance between the Minkowski difference
    // and the origin.
    return origin_distance2(p);
}


}


#else    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...


namespace Arageli
{

// PLACE ALL NOT TEMPLATE NOT INLINE IMPLEMENTATIONS HERE

}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
