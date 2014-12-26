/*****************************************************************************

    polyhedron.hpp

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
    \file polyhedron.hpp
    \brief Representation of a convex polyhedron and operations on it.

    This file contains a template class for representing of a convex polyhedron
    as the corresponding cone, operations such as intersection, sum etc,
    way for access to sides any dimention like iterator access to a containter
    and other.
*/


#ifndef _ARAGELI_polyhedron_hpp_
#define _ARAGELI_polyhedron_hpp_

#include "config.hpp"

#include "gcd.hpp"
#include "big_int.hpp"
#include "rational.hpp"
#include "cone.hpp"
#include "sideset.hpp"
#include "vector.hpp"
#include "matrix.hpp"


namespace Arageli
{

struct fromempty_t {};
const fromempty_t fromempty = fromempty_t();

struct fromivert_t {};
const fromivert_t fromivert = fromivert_t();

struct fromvert_t {};
const fromvert_t fromvert = fromvert_t();


template <typename T, typename M, typename C>
class polyhedron_default_config {};


/// The polyhedron representation as the base cone.
/** The polyhedron in n-dimetion space is represented as a cone Ax >= 0
    in (n+1) dimention space and hyperplane x[0] = 1, so the polyhedra is
    {x : Ax >= 0 & x[0] = 1}. In other words, polyhedron that is completely
    defined by system Ax >= b, represented as a cone (-b|A)x' >= 0, where
    x is n-vector and x' is (n+1)-vector. */
template
<
    typename T = big_int,
    typename M = matrix<T>,
    typename C = cone<T, M>,
    typename CFG = polyhedron_default_config<T, M, C>
>
class polyhedron
{
public:

    //typedef matrix<R> vertices_type;

    typedef C cone_type;

    typedef typename C::inequation_element_type inequation_element_type;
    typedef typename C::equation_element_type equation_element_type;
    typedef typename C::generatrix_element_type generatrix_element_type;
    typedef typename C::basis_element_type basis_element_type;

    typedef typename C::inequation_type inequation_type;
    typedef typename C::equation_type equation_type;
    typedef typename C::generatrix_type generatrix_type;
    typedef typename C::basis_type basis_type;

    typedef typename C::size_type size_type;
    typedef typename C::dim_type dim_type;
    typedef typename C::difference_type difference_type;

    typedef rational<typename C::value_type> value_type;


    /// Creates an empty polyhedron in 0-space.
    /** After creating the base cone is x[0] = 0 and
        only parametric representation is valid and normal. */
    polyhedron () :
        cone_m(1, fromnull)
    {}


    /// Creates an empty polyhedron in the space with specified dimention.
    /** After creating only parametric representation is valid and normal. */
    template <typename D>
    polyhedron (const D& dim, const fromempty_t&) :
        cone_m(dim + 1, fromnull)
    {}


    /// Creates polyhedron as all space with specified dimention.
    /** After creating only implicit representation is valid and normal. */
    template <typename D>
    polyhedron (const D& dim, const fromspace_t&);


    /// Creates polyhedron as a solution of specified inequations set with extended matrix.
    /**    Matrix ineqmat = (-b|A).
        After creating only implicit representation is valid but
        not necessarily normal. */
    template <typename M1>
    polyhedron (const M1& ineqmat, const fromineq_t&);


    /// Creates a poytope from vertices each element of which is representable as T.
    template <typename M1>
    polyhedron (const M1& vert, const fromivert_t&);


    /// Creates a poytope from vertices set.
    /** The vertices can be have not only T-compatible values.
        In this case we multiply each vertex by the particular multiplier
        so make them representable as element of type T and take the
        multiplier out of real vertices elements to x[0].
        <br><bold>WARNING! Now it works only for rational elements.</bold> */
    template <typename M1>
    polyhedron (const M1& vert, const fromvert_t&);


    //////////////////////////////////////////////////////////////////////////////////
    /// WARNING! TEMPORARY IMPLEMENTATION! ONLY FACETS AND ONLY FOR A POLYTOPE.

    sideset sides () const;

    polyhedron facet (sideset::const_facet_iterator fi) const;

    //////////////////////////////////////////////////////////////////////////////////


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Access to valid/normal flags.

    //@{


    /// If it returns true than implicit representation (inequations and equations) is actual.
    bool is_implicit_valid () const
    {
        return cone_m.is_implicit_valid();
    }

    /// If it returns true than parametric representation (generatrices and basis) is actual.
    bool is_parametric_valid () const
    {
        return cone_m.is_parametric_valid();
    }

    /// If it returns true than implicit and parametric representations are both valid.
    bool is_all_valid () const
    {
        return cone_m.is_all_valid();
    }

    /// If it returns true than representation (inequations and equations) is in normal form.
    /** The implicit representation is in normal form iff this representation is valid
        and maximum non-zero linear variety (if any) represented only as a set of equations
        and there are no negligible inequations or negligible equations.
        Note, this form is not a single form. */
    bool is_implicit_normal () const
    {
        return cone_m.is_implicit_normal();
    }

    /// If it returns true than parametric representation (generatrices and basis) is in normal form.
    /** The parametric representation is in normal form iff this representation is valid
        maximum non-zero linear variety (if any) represented only as its corresponding
        basis and there are no negligible generatrix.
        Note, this form is not a single form. */
    bool is_parametric_normal () const
    {
        return cone_m.is_parametric_normal();
    }

    /// If it returns true than implicit and parametric representations are both in normal form.
    bool is_all_normal () const
    {
        return cone_m.is_all_normal();
    }


    //@}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Validation and normalization of the representations.

    //@{


    /// Make the implicit representation valid.
    /** This is based on the parametric representation.
        Actual validation is performed only if is_implicit_valid() == false. */
    void validate_implicit () const
    {
        cone_m.validate_implicit();
    }

    /// Make the parametric representation valid.
    /** This is based on the implicit representation.
        Actual validation is performed only if is_parametric_valid() == false. */
    void validate_parametric () const
    {
        cone_m.validate_parametric();
    }

    /// Make invalid representation valid.
    void validate_all () const
    {
        cone_m.validate_all();
    }

    /// Normalize the implicit representation.
    /** This is based on the implicit representation itself (if it's valid) or/and
        on the parametric representation. */
    void normalize_implicit () const
    {
        cone_m.normalize_implicit();
    }

    /// Normalize the parametric representation.
    /** This is based on the parametric representation itself (if it's valid) or/and
        on the parametric representation. */
    void normalize_parametric () const
    {
        cone_m.normalize_parametric();
    }

    /// Normalize both implicit and parametric representations.
    void normalize_all () const
    {
        cone_m.normalize_all();
    }


    //@}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Explicit set of valid/normal flags.

    //@{


    /// Set the valid implicit flag without an actual valid test or validation.
    /** Be careful! If you have set valid flags for both implicit and parametric
        representations but they don't represent the same polyhedron base cone,
        the future behaviour is undefined. */
    void set_valid_implicit ()
    {
        cone_m.set_valid_implicit();
    }

    /// Set the valid parametric flag without an actual valid test or validation.
    /** Be careful! If you have set valid flags for both implicit and parametric
        representations but they don't represent the same polyhedron base cone,
        the future behaviour is undefined. */
    void set_valid_parametric ()
    {
        cone_m.set_valid_parametric();
    }

    /// Set the valid both implicit and parametric flags without an actual valid test or validation.
    /** Be careful! If you have set valid flags for both implicit and parametric
        representations but they don't represent the same polyhedron base cone,
        the future behaviour is undefined. */
    void set_valid_all ()
    {
        cone_m.set_valid_all();
    }

    /// Set the normal implicit flag without an actual normal test or normalization.
    /** Be careful! If you have set normal flags for both implicit and parametric
        representations but one of them or both aren't normal or they don't represent
        the same polyhedron base cone, the future behaviour is undefined. */
    void set_normal_implicit ()
    {
        cone_m.set_normal_implicit();
    }

    /// Set the normal parametric flag without an actual normal test or normalization.
    /** Be careful! If you have set normal flags for both implicit and parametric
        representations but one of them or both aren't normal or they don't represent
        the same polyhedron base cone, the future behaviour is undefined. */
    void set_normal_parametric ()
    {
        cone_m.set_normal_parametric();
    }

    /// Set the normal both implicit and parametric flags without an actual normal test or normalization.
    /** Be careful! If you have set normal flags for both implicit and parametric
        representations but one of them or both aren't normal or they don't represent
        the same polyhedron base cone, the future behaviour is undefined. */
    void set_normal_all ()
    {
        cone_m.set_normal_all();
    }

    //@}

    /// True iff the polyhedron doesn't have subspace with dimention greater then 0.
    /** Note, a polyhedron in 0-space is always pointed.
        An empty polyhedron is pointed too. */
    bool is_pointed () const
    {
        return cone_m.is_pointed();
    }


    dim_type dim () const
    {
        // WARNING! It's incorrect in some cases.
        return cone_m.dim() - 1;
    }

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Direct access to the representation matrices.

    //@{

    const inequation_type& inequation_matrix () const
    {
        return cone_m.inequation_matrix();
    }

    inequation_type& inequation_matrix ()
    {
        return cone_m.inequation_matrix();
    }

    const equation_type& equation_matrix () const
    {
        return cone_m.equation_matrix();
    }

    equation_type& equation_matrix ()
    {
        return cone_m.equation_matrix();
    }

    const generatrix_type& generatrix_matrix () const
    {
        return cone_m.generatrix_matrix();
    }

    generatrix_type& generatrix_matrix ()
    {
        return cone_m.generatrix_matrix();
    }

    const basis_type& basis_matrix () const
    {
        return cone_m.basis_matrix();
    }

    basis_type& basis_matrix ()
    {
        return cone_m.basis_matrix();
    }

    //@}

    matrix<bool> relation () const
    {
        return cone_m.relation();
    }

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name The polyhedron and a point positional relationship.

    //@{

    template <typename V1>
    bool strict_inside (V1 x) const
    {
        x.push_front(unit<typename V1::element_type>());
        return cone_m.strict_inside(x);
    }

    template <typename V1>
    bool strict_outside (const V1& x) const;

    template <typename V1>
    bool unstrict_inside (V1 x) const
    {
        x.push_front(unit<typename V1::element_type>());
        return cone_m.unstrict_inside(x);
    }

    template <typename V1>
    bool unstrict_outside (const V1& x) const;

    //@}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Intersection and convex union.

    //@{

    template <typename Polyhedron1>
    polyhedron& intersection (const Polyhedron1& x)
    {
        cone_m.intersection(x.cone_m);
        return *this;
    }

    template <typename Polyhedron1>
    polyhedron& convex_union (const Polyhedron1& x)
    {
        cone_m.conic_union(x.cone_m);
        return *this;
    }

    //@}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Minkowski operations.

    //@{

    /// Compute minkowski difference between this polyhedron and the argument.
    /** The result is stored as this object. The same as operator-=.*/
    template <typename P1>
    polyhedron& minkowski_diff (const P1& x);

#if 0
    /// Compute minkowski difference between this polyhedron and the argument.
    /** The result is stored as this object. The same as minkowski_diff
        function. */
    template <typename P1>
    polyhedron& operator-= (const P1& x)
    {
        return minkowski_diff(x);
    }
#endif

    //@}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Coordinate transformations.

    //@{


    /// Translate all points in the polyhedron along a given vector.
    /** Resulting polyhedron is { y' = y + x | y \in P},
        where P is this polyhedron. */
    template <typename T1>
    polyhedron& translate (const T1& x);

#if 0
    /// Translate all points in the polyhedron along a given vector.
    /** Resulting polyhedron is { y' = y + x | y \in P},
        where P is this polyhedron. */
    template <typename T1>
    polyhedron& operator+= (const T1& x);


    /// Translate all points in the polyhedron opposite a given vector.
    /** Resulting polyhedron is { y' = y - x | y \in P},
        where P is this polyhedron. */
    template <typename T1>
    polyhedron& operator-= (const T1& x);
#endif

    //@}


    // Brrrrr!
    template <typename M1>
    M1 vertices () const
    {
        normalize_parametric();
        M1 res = cone_m.generatrix_matrix();

        for(std::size_t i = 0; i < res.nrows();)
            if(Arageli::is_null(res(i, 0)))
                res.erase_row(i);
            else
            {
                res.div_row(i, safe_reference(res(i, 0)));
                ++i;
            }

        res.erase_col(0);

        return res;
    }

    // Brrrrr!
    const T& inequation_matrix (std::size_t i, std::size_t j) const
    {
        return cone_m.inequation_matrix()(i, j);
    }

    // Brrrrr!
    vector<T> facet_normal (std::size_t i) const
    {
        vector<T> res = cone_m.inequation_matrix().copy_row(i);
        res.pop_front();
        return res;
    }

    std::size_t space_dim () const
    {
        return cone_m.space_dim() - 1;
    }

    const C& cone () const
    {
        return cone_m;
    }

    C& cone ()
    {
        return cone_m;
    }

private:

    C cone_m;

};


/// VRML Output. WARNING! The view will be correct only in wired mode.
template
<
    typename Out,
    typename T,
    typename R,
    typename M,
    typename CFG
>
void output_vrml (Out& out, const polyhedron<T, R, M, CFG>& p);


class pstricks_color_map
{
public:

    pstricks_color_map
    (
        double rstart_a,
        double gstart_a,
        double bstart_a,
        double rend_a,
        double gend_a,
        double bend_a,
        std::size_t n_a,
        const std::string& name_a
    );

    /// x in [0, 1]
    const std::string& name (double x) const;

};


/// PostScript LaTeX Output with pstricks package. WARNING! Only for 3-dim polytopes.
/** This function produces command chain for pstricks package in LaTex that draw
    given polytope. You should place this chain into \pspicture environment.
    WARNING! This function isn't implemented yet. */
template
<
    typename Out,
    typename P,
    typename WM,
    typename Colormap
>
void output_polytope_pstricks_3d
(
    Out& out,            ///< output stream
    const P& p,            ///< 3-polytope
    double left,        ///< left of a rectangle bounding polytope picture
    double bottom,        ///< bottom of it
    double right,        ///< right of it
    double top,            ///< top of it
    double linewidth,    ///< width of edge lines
    const WM& wm,        ///< world matrix (orientation + position) that is preprocessed p
        // WARNING! May be the orientation matrix is enought
    const Colormap& colormap    ///< color map for pstricks --- a function maping [0, 1] in color names
);


template <typename Out, typename P>
void output_polyhedron_2d
(
    Out& out,        ///< output stream
    const P& p,        ///< 2-polyhedron
    double left,    ///< left of a rectangle bounding polytope picture
    double bottom,    ///< bottom of it
    double right,    ///< right of it
    double top,        ///< top of it
    double extreme_margin = 1,    ///< margin from the border of the picture for all extreme points
    double axis_margin = 1,    ///< margin from the coordinate lines to the border
    double axis_linewidth = 0.025,    ///< width of lines for axis
    double poly_linewidth = 0.05,    ///< width of lines for polyhedron
    const std::string& cn_internal = "lightgray",    ///< internal area color name
    const std::string& cn_polyborder = "black",        ///< polyhedron line color name
    bool show_extreme = true,        ///< show extreme points as *-dots
    bool show_intinternal = true    ///< show internal integer points as o-dots
);


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_polyhedron
    #include "polyhedron.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_polyhedron
#endif

#endif    // #ifndef _ARAGELI_polyhedron_hpp_
