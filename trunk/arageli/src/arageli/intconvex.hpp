/*****************************************************************************

    intconvex.hpp

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
    \file intconvex.hpp
    \brief Routines to find convex hull of integer points of a polyhedron.

    This file contains the primitive algorithm to find convex hull of integer
    points of a polyhedron.
*/


#ifndef _ARAGELI_intconvex_hpp_
#define _ARAGELI_intconvex_hpp_

#include "config.hpp"

// REFERENCE ADDITIONAL HEADERS HERE


namespace Arageli
{


/// A convex hull of integer points of a polyhedron.
/** This function takes a set of generatrices of the base cone of a polyhedron.
    And it returns a set of generatrices of the base cone of convex hull of
    integer pointes of that polyhedron. This function implements a primitive
    algorithm and finds the convex hull    very slow. It's deathly algorithm!
    Source of the idea: Emelichev V.A., Kovalev M.M., Kravcov M.K.
    Mnogogranniki, grafy, optimizacija (Nauka, 1981)(Russian), pp. 107--110.
    System of generatrices in gen is not necessarily minimal; and retrieved
    system intgen is not necessarily minimal (actually it's very big).
    WARINING! Type of elements of gen should be integer. */
template <typename Gen, typename IntGen>
void intconvex_simple (const Gen& gen, IntGen& intgen);


/// Status of near_extreme_intpoint function execution.
enum nei_result
{
    NEIR_EMPTY,         ///< there are no integer points in a simplex
    NEIR_ALL_VERTS,     ///< all vertices in a simplex are already integer
    NEIR_AMONG_VERTS,   ///< there are integer points but among vertices only
    NEIR_NEW            ///< new point has been found
};


/// Find near extreme integer point in a generalized simplex.
/** @param a a matrix of the simplex, each row is a extended vertex
    @param v a place where the new point will be stored, a vector
    @param rows a vector with indeces corresponding to combined rows in a. */
template <typename A, typename V, typename Rows>
nei_result near_extreme_intpoint (const A& a, V& v, Rows& rows);


namespace ctrl
{

/// It's temporary version of controller for intconvex_triangulation function.
struct intconvex_triangulation_idler
{

    class abort :
        public ctrl::abort
    {};

    /// WARNING! TEMPORARY MEMBER FOR DEBUGGING.
    std::ostream& stream ()
    {
        return blackhole.value();
    }

private:

    refcntr_proxy<std::ostringstream> blackhole;
};

}


/// A convex hull of integer points of a bodily polyhedron with given triangulation.
/** @param gen generatrices of a polyhedron in extended shape
    @param tr a triangulation matrix; each row is indexes of vertices of one simplex
    @param intineq the resulting matrix of inequations. */
template <typename Gen, typename TR, typename IntIneq, typename Ctrler>
void intconvex_triangulation
(
    const Gen& gen,
    const TR& tr,
    IntIneq& intineq,
    Ctrler ctrler
);


/// A convex hull of integer points of a bodily polyhedron with given triangulation.
/** See full version of this function. */
template <typename Gen, typename TR, typename IntIneq>
inline void intconvex_triangulation
(
    const Gen& gen,
    const TR& tr,
    IntIneq& intineq
)
{
    intconvex_triangulation
    (
        gen,
        tr,
        intineq,
        ctrl::intconvex_triangulation_idler()
    );
}


/// A convex hull of integer points of a bodily polyhedron via triangulation.
/** @param gen generatrices of a polyhedron in extended shape
    @param q relation matrix
    @dim dimention of the polyhedron
    @param intineq the resulting matrix of inequations. */
template <typename Gen, typename Q, typename Dim, typename IntIneq>
void intconvex_triangulate_simple_1
(
    const Gen& gen,
    const Q& q,
    const Dim& dim,
    IntIneq& intineq
);


/// Find the set containing all extreme points of a solutions 2-d equation on a group.
/** The equation is  t_1 + alpha*t_2 = gamma (mod delta), t_1, t_2 in Z+.
    Type for intermediate calculation is Alpha, signed integer type.
    The algorithm is obtained from Shevchenko, Kachestvennye voprosy celochislennogo
    programmirovaniya (in Russian), p. 99--100. */
template
<
    typename Alpha,
    typename Gamma,
    typename Delta,
    typename V,
    typename Ctrler
>
void near_extreme_2d_mod
(
    Alpha alpha,
    Gamma gamma,
    Delta delta,
    V& v,
    Ctrler ctrler
);


template
<
    typename Alpha,
    typename Gamma,
    typename Delta,
    typename V
>
inline void near_extreme_2d_mod
(
    Alpha alpha,
    Gamma gamma,
    Delta delta,
    V& v
)
{
    // WARNING! TEMPORARY WE PASS intconvex_triangulation_idler
    near_extreme_2d_mod
    (
        alpha,
        gamma,
        delta,
        v,
        ctrl::intconvex_triangulation_idler()
    );
}


/// Find the set containing all extreme points of a solutions equation on a group.
/** This function find such set v = {x} that containing all extreve points of a set of
    solution of a system modulo: a*x = b (mod vd), where vd is a vector of modules.
    delta is a determinant of the system.
    WARNING! RESTRICTED IMPLEMENTATION! */
template
<
    typename A,
    typename B,
    typename VD,
    typename Delta,
    typename V,
    typename Ctrler
>
void near_extreme_2_3_mod
(
    const A& a,
    const B& b,
    const VD& vd,
    const Delta& delta,
    V& v,
    Ctrler ctrler
);


template
<
    typename A,
    typename B,
    typename VD,
    typename Delta,
    typename V
>
inline void near_extreme_2_3_mod
(
    const A& a,
    const B& b,
    const VD& vd,
    const Delta& delta,
    V& v
)
{
    // WARNING! TEMPORARY WE PASS intconvex_triangulation_idler
    near_extreme_2_3_mod
    (
        a,
        b,
        vd,
        delta,
        v,
        ctrl::intconvex_triangulation_idler()
    );
}


template
<
    typename A,
    typename B,
    typename VD,
    typename Delta,
    typename Beta,
    typename Res,
    typename Ctrler
>
void minimize_2_3_mod
(
    const A& a,
    const B& b,
    const VD& vd,
    const Delta& delta,
    const Beta& beta,
    Res& res,
    Ctrler ctrler
);


template
<
    typename A,
    typename B,
    typename VD,
    typename Delta,
    typename Beta,
    typename Res
>
inline void minimize_2_3_mod
(
    const A& a,
    const B& b,
    const VD& vd,
    const Delta& delta,
    const Beta& beta,
    Res& res
)
{
    // WARNING! TEMPORARY WE PASS intconvex_triangulation_idler
    minimize_2_3_mod
    (
        a,
        b,
        vd,
        delta,
        beta,
        res,
        ctrl::intconvex_triangulation_idler()
    );
}


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_intconvex
    #include "intconvex.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_intconvex
#endif

#endif    // #ifndef _ARAGELI_intconvex_hpp_
