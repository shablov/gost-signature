/*****************************************************************************

    triangulation.hpp -- routines for triangulation of polyhedral cone and
        general polyhedron.

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
    \file triangulation.hpp
    \brief Routines for triangulation of polyhedral cone and
        general polyhedron.

    This file contains a set of function for triangulation polyhedral cone
    and any polyhedron.
*/


#ifndef _ARAGELI_triangulation_hpp_
#define _ARAGELI_triangulation_hpp_

#include "config.hpp"

// REFERENCE ADDITIONAL HEADERS HERE


namespace Arageli
{


namespace ctrl
{

struct triangulate_simple_1_idler
{
    class abort :
        public ctrl::abort
    {};

    template <typename Q, typename Dim1, typename Dim2>
    void preamble (const Q& q, const Dim1& dim, const Dim2& subspdim) const
    {
        //std::cerr << "\nDEBUG OUTPUT triangulate_simple_1_idler\n";
        //output_aligned(std::cerr << "q =\n", q);
        //std::cerr << "dim = " << dim << ", subspdim = " << subspdim << "\n";
    }

    template <typename Dim2>
    void before_subspace (const Dim2& subspdim) const
    {
        //std::cerr << "before_subspace: subspdim = " << subspdim << "\n";
    }

    template <typename Dim2>
    void after_subspace (const Dim2& subspdim) const
    {
        //std::cerr << "after_subspace: subspdim = " << subspdim << "\n";
    }

    template <typename Simplex, typename Tr, typename Curq, typename Q>
    void new_simplex
    (
        const Simplex& simplex,
        const Tr& tr,
        const Curq& curq,
        const Q& q
    ) const
    {
        //std::cerr << "new_simplex: simplex = " << simplex << "\n";
    }

    template <typename Ray, typename Curq, typename Q>
    void select_ray
    (
        const Ray& ray,
        const Curq& curq,
        const Q& q
    ) const
    {
        //std::cerr << "select_ray: ray = " << ray << "\n";
        //output_aligned(std::cerr << "curq =\n", curq);
    }

    template <typename Ineq, typename Curq, typename Q>
    void select_facet
    (
        const Ineq& ineq,
        const Curq& curq,
        const Q& q
    ) const
    {
        //std::cerr << "select_facet: ineq = " << ineq << "\n";
    }

    template <typename Curq, typename Q>
    void before_eliminating_ineqs (const Curq& curq, const Q& q) const
    {
        //output_aligned(std::cerr << "before_eliminating_ineqs: curq =\n", curq);
    }

    template <typename Curq, typename Q>
    void after_eliminating_ineqs (const Curq& curq, const Q& q) const
    {
        //output_aligned(std::cerr << "after_eliminating_ineqs: curq =\n", curq);
    }

    template <typename Q, typename Tr>
    void conclusion (const Q& q, const Tr& tr) const
    {
        //std::cerr << "conclusion:";
        //output_aligned(std::cerr << "\nq =\n", q);
        //output_aligned(std::cerr << "\ntr =\n", tr);
    }

};


} // namespace ctrl


/// Triangulates a polyhedral cone with structure matrix q.
/** Matrix q is a matrix of the structure of a cone ---
    adjacency matrix rays/facets. In particular this matrix is obtained
    from Motzkin-Burger algorithm as q-matrix.
    Values itself in that matrix is not matter, only equality
    with zero is valuable. Each row corresponds to one of the cone rays, and
    each column corresponds to one of the cone facets.

    Output matrix tr is a matrix with indexes.
    It contains dim columns and several    rows, number of which is equal to
    the number of simpixes founded in the polyhedral cone.
    Matrix tr contains indexes of vertexes that include
    in each simplex according to an order in q matrix.

    TEMPORARY LIMITATION: All subspace generators should'n be included
    to q matrix, and only subspdim points to subspace presence.
*/
template
<
    typename Q,
    typename Dim1,
    typename TR,
    typename Dim2,
    typename Ctrler
>
void triangulate_simple_1
(
    const Q& q,
    const Dim1& dim,
    TR& tr,
    const Dim2& subspdim,
    Ctrler ctrler
);


/// Triangulates a cone with the relations matrix q.
/** See full version of this function. */
template
<
    typename Q,
    typename Dim1,
    typename TR,
    typename Dim2
>
inline void triangulate_simple_1
(
    const Q& q,
    const Dim1& dim,
    TR& tr,
    const Dim2& subspdim
)
{
    triangulate_simple_1
    (
        q,
        dim,
        tr,
        subspdim,
        ctrl::triangulate_simple_1_idler()
    );
}


/// Triangulates a pointed cone with the relations matrix q.
/**    See full version of this function. */
template
<
    typename Q,
    typename Dim1,
    typename TR
>
inline void triangulate_simple_1 (const Q& q, const Dim1& dim, TR& tr)
{
    triangulate_simple_1
    (
        q,
        dim,
        tr,
        0,
        ctrl::triangulate_simple_1_idler()
    );
}



} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_triangulation
    #include "triangulation.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_triangulation
#endif

#endif    // #ifndef _ARAGELI_triangulation_hpp_
