/*****************************************************************************

    sideset.hpp

    This file is a part of the Arageli library.

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
    \file sideset.hpp
    \brief A representation of polyhedron side set with iterators
        for ordered enumeration and other routines.

    This file contains a representation of polyhedron side set with iterators
    for ordered enumeration and other miscellaneous routines.

    WARNING! The implementation isn't completed.
*/


#ifndef _ARAGELI_sideset_hpp_
#define _ARAGELI_sideset_hpp_

#include <cstddef>

#include "config.hpp"

#include "vector.hpp"


namespace Arageli
{


/// WARNING! Temporary implimentation. Only facets with vertices indexes.
/** Don't use it! */
class sideset
{
    typedef vector<vector<std::size_t> > VVs;
    VVs indices;
    std::size_t except;

public:

    typedef VVs::const_iterator const_iterator;
    typedef VVs::element_type vertex_indices_set;

    // WARNING! except ISN'T INITIALIZED!
    sideset (const VVs& x) :
        indices(x)
    {}

    sideset (const VVs& x, std::size_t except_a) :
        indices(x),
        except(except_a)
    {}

    VVs::const_iterator begin () const
    {
        return indices.begin();
    }

    VVs::const_iterator end () const
    {
        return indices.end();
    }

    // WARNING! TEMPORARY!
    class facet_iterator
    {
        friend class sideset;

        facet_iterator (std::size_t i_a, std::size_t except_a) :
            i(i_a),
            except(except_a)
        {}

        std::size_t i;  // facet number
        std::size_t except; // the inequation number that is not a facet

    public:

        facet_iterator ()
        {}

        std::size_t operator* () const
        {
            return i < except ? i : i + 1;
        }

        facet_iterator operator++ ()
        {
            ++i;
            return *this;
        }

        bool operator== (const facet_iterator x) const
        {
            return i == x.i;
        }

        bool operator!= (const facet_iterator x) const
        {
            return !operator==(x);
        }
    };

    typedef facet_iterator const_facet_iterator;

    const_facet_iterator facets_begin () const
    {
        return const_facet_iterator(0, except);
    }

    const_facet_iterator facets_end () const
    {
        return const_facet_iterator(indices.size(), except);
    }

};


/// Computes all sides for a cone.
/** The result v is 3x nest of vectors.
    The first level corresponds to the side dimentions (entry 0 points
    to 0-side). The second level corresponds to the sides of a particular
    dimention (denote the dimention as k). The third level corresponds to
    sides with (k-1) dimention that the particular k-side have.
    So, for example, if the cone isn't pointed, v[0] will be an empty. */
template <typename C, typename V>
V& sides_structure_cone (const C& c, V& v);


template <typename C>
inline vector<vector<vector<typename C::size_type> > >
sides_structure_cone (const C& c)
{
    vector<vector<vector<typename C::size_type> > > v;
    return sides_structure_cone(c, v);
}


/// Computes f-vector for the cone c as for a common polyhedron.
/** F-vector is a vector with numbers of sides sorted dimentionally-wise so
    that the number of k-sides corresponds to an element with the index k
    of the resulting vector. The formal (-1)-dimentional empty set side
    isn't counted. So, for example, v.size()-1 will be equaled to the cone
    dimention. */
template <typename C, typename V>
V& f_vector_cone (const C& c, V& v);


/// Computes f-vector for the cone c as for a common polyhedron.
template <typename C>
inline vector<typename C::size_type> f_vector_cone (const C& c)
{
    vector<typename C::size_type> v;
    return f_vector_cone(c, v);
}


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_sideset
    #include "sideset.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_sideset
#endif

#endif    // #ifndef _ARAGELI_sideset_hpp_
