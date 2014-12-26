/*****************************************************************************

    cone.hpp

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
    \file cone.hpp
    \brief Representation of a polyhedral cone and operations on it.

    This file contains a template class for representing of a polyhedral cone
    as a set of facets and/or generators, operations such as intersection,
    sum etc, computing some geometrical properties (dimentions, subspace
    dimention etc.)    and relations.

    In general this file isn't intended to implement combinatoric properties
    of the cones such as f-vector, side structure, topological equivalence
    detection etc. For this properties see sideset.hpp file.

    WARNING! The implementation isn't completed.
*/


#ifndef _ARAGELI_cone_hpp_
#define _ARAGELI_cone_hpp_

#include "config.hpp"

#include "factory.hpp"
#include "frwrddecl.hpp"
#include "big_int.hpp"
#include "matrix.hpp"


namespace Arageli
{


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// @name Helper types and constants to choose a particular cone class constructor.

//@{

struct fromnull_t {};

/// Helper constant to create cone as one point (zero).
const fromnull_t fromnull = fromnull_t();

struct fromspace_t {};

/// Helper constant to create cone from dimention.
const fromspace_t fromspace = fromspace_t();

struct fromineq_t {};

/// Helper constant to create cone from the set of inequalities.
const fromineq_t fromineq = fromineq_t();

struct fromeq_t {};

/// Helper constant to create cone from the set of equalities.
const fromeq_t fromeq = fromeq_t();

struct fromgen_t {};

/// Helper constant to create cone from the set of generatrices.
const fromgen_t fromgen = fromgen_t();

struct frombasis_t {};

/// Helper constant to create cone from the set of basis vectors.
const frombasis_t frombasis = frombasis_t();

struct fromdual_t {};

const fromdual_t fromdual = fromdual_t();

//@}


/// This exception is generated when the implementation cannon represent cone in required way.
class cannot_represent_cone : public exception {};


template <typename Out, typename T, typename M, typename CFG>
void output_list (Out& out, const cone<T, M, CFG>& x);


template <typename T, typename M>
class cone_default_config {};


/// The polyhedral cone representation as a set of facets and a set of generators.
/** This class represents cone as four matrices. Two matrices for
    the facet representation: the left matrix of homogeneous system of
    inequations and left matrix for homogeneous system of equations.
    Other two matrices for the parametric representations: vectors involved
    to cone hull (generatrix) and a basis of subspace.

    Note, only one of those pairs matrices is necessary for full
    representation of a cone, but another pair builds on-the-fly when
    it is need for a particular    operation.

    So we have four matrices:
    inequation_matrix,    equation_matrix, generatrix_matrix,
    basis_matrix and two way to represent a cone:
        - { x : inequation_matrix*x >= 0 AND equation_matrix*x == 0 },
        - { x : x = Cone(generatrix_matrix) + Linear(basis_matrix) }
    where equality and inequality signs mean a elementwise comparision
    for vectors, and 0 is a vector with zeros.

    Template parameters. T is a type to represent element of matrices
    and vectors in given vector space. M is a type for each of those
    matrices.

    An object of the class cone can be in several states depends on
    what of the representations listed above is build.
    Each representation can be in three states --- not built
    (not valid and not normal),    built but may be not minimal (valid),
    built and minimal (valid and normal). Both the representations
    cannot be in the state `not built' because in such case the cone
    isn't defined. You can obtain the current state and manage it
    via functions is_X_normal, is_X_valid and set_valid_X, set_normal_X
    correspondingly, where X is one of the word `implicit', `parametric',
    `all'. You can validate or normalize a particular representation
    by another one via functions validate_X and normalize_X.
    Some functions implicitly complete a representaion required for
    its normal operation or normalize existing ones.

    TEMPORARY LIMITATION. Only an integer T is allowed.
    */
template <typename T, typename M, typename CFG>
class cone
{
public:

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Standard type definition.

    //@{

    typedef T inequation_element_type;
    typedef T equation_element_type;
    typedef T generatrix_element_type;
    typedef T basis_element_type;

    typedef M inequation_type;
    typedef M equation_type;
    typedef M generatrix_type;
    typedef M basis_type;

    typedef typename M::difference_type dim_type;
    typedef typename M::difference_type difference_type;
    typedef typename M::size_type size_type;

    typedef T value_type;

    //@}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Constructors.

    //@{


    /// Creates cone in 0-space (only one point --- zero).
    /** All matrices are zero-by-zero matrices.
        After creating all representations are valid. */
    cone () :
        state(STATE_ALL)
    {}


    /// Creates cone as one point (zero) in the space with specified dimention.
    /** After creating the parametric representation is valid and normal.
        If dim == 0, the implicit representation is valid and normal too. */
    template <typename D>
    cone (const D& dim, const fromnull_t&) :
        basis_m(0, dim),
        generatrix_m(0, dim),
        state(STATE_PARAMETRIC)
    {
        if(Arageli::is_null(dim))
            set_flag(STATE_IMPLICIT);
    }


    /// Creates cone as all space with specified dimention.
    /** After creating the implicit representation is valid and normal.
        If dim == 0, the parametric representation is valid and normal too. */
    template <typename D>
    cone (const D& dim, const fromspace_t&) :
        inequation_m(0, dim),
        equation_m(0, dim),
        state(STATE_IMPLICIT)
    {
        if(Arageli::is_null(dim))
            set_flag(STATE_PARAMETRIC);
    }


    /// Creates cone as a solution of specified inequations set.
    /** After creating only implicit representation is valid
        but not necessarily normal. */
    template <typename M1>
    cone (const M1& ineqmat, const fromineq_t&) :
        inequation_m(ineqmat),
        equation_m(0, ineqmat.ncols()),
        state(STATE_IMPLICIT_VALID)
    {}


    /// Creates cone as a solution of specified equations set.
    /** After creating only implicit representation is valid
        but not necessarily normal. */
    template <typename M1>
    cone (const M1& eqmat, const fromeq_t&) :
        inequation_m(0, eqmat.ncols()),
        equation_m(eqmat),
        state(STATE_IMPLICIT_VALID)
    {}


    /// Creates cone as a cone hull of specified generatrix set.
    /** After creating only parametric representation is valid
        but not necessarily normal. */
    template <typename M1>
    cone (const M1& genmat, const fromgen_t&) :
        generatrix_m(genmat),
        basis_m(0, genmat.ncols()),
        state(STATE_PARAMETRIC_VALID)
    {}


    /// Creates cone as a subspace with specified basis.
    /** After creating only parametric representation is valid
        but not necessarily normal. */
    template <typename M1>
    cone (const M1& basismat, const frombasis_t&) :
        generatrix_m(0, basismat.ncols()),
        basis_m(basismat),
        state(STATE_PARAMETRIC_VALID)
    {}


    /// Creates cone as a dual to given cone.
    /** A dual cone is a cone with swapped implicit and    parametric
        representations. */
    template <typename Cone1>
    cone (const Cone1& c, const fromdual_t&);


    //@}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Access to valid/normal flags.

    //@{


    /// If it returns true than implicit representation (inequations and equations) is actual.
    bool is_implicit_valid () const
    {
        ARAGELI_ASSERT_1(is_correct_flags());
        return get_flag(STATE_IMPLICIT_VALID);
    }

    /// If it returns true than parametric representation (generatrices and basis) is actual.
    bool is_parametric_valid () const
    {
        ARAGELI_ASSERT_1(is_correct_flags());
        return get_flag(STATE_PARAMETRIC_VALID);
    }

    /// If it returns true than implicit and parametric representations are both valid.
    bool is_all_valid () const
    {
        ARAGELI_ASSERT_1(is_correct_flags());
        return get_flag(STATE_VALID);
    }


    /// If it returns true than representation (inequations and equations) is in normal form.
    /** The implicit representation is in normal form iff this representation is valid
        and maximum non-zero subspace (if any) represented only as a set of equations
        and there are no negligible inequations or negligible equations.
        Note, this form is not a single form. */
    bool is_implicit_normal () const
    {
        ARAGELI_ASSERT_1(is_correct_flags());
        return get_flag(STATE_IMPLICIT);
    }


    /// If it returns true than parametric representation (generatrices and basis) is in normal form.
    /** The parametric representation is in normal form iff this representation is valid
        maximum non-zero subspace (if any) represented only as its basis and there are no
        negligible generatrix.
        Note, this form is not a single form. */
    bool is_parametric_normal () const
    {
        ARAGELI_ASSERT_1(is_correct_flags());
        return get_flag(STATE_PARAMETRIC);
    }


    /// If it returns true than implicit and parametric representations are both in normal form.
    bool is_all_normal () const
    {
        ARAGELI_ASSERT_1(is_correct_flags());
        return get_flag(STATE_ALL);
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
        ARAGELI_ASSERT_1(is_correct_flags());
        if(!get_flag(STATE_IMPLICIT_VALID))
            force_validate_implicit();
    }

    /// Make the parametric representation valid.
    /** This is based on the implicit representation.
        Actual validation is performed only if is_parametric_valid() == false. */
    void validate_parametric () const
    {
        ARAGELI_ASSERT_1(is_correct_flags());
        if(!get_flag(STATE_PARAMETRIC_VALID))
            force_validate_parametric();
    }

    /// Make invalid representation valid.
    void validate_all () const
    {
        ARAGELI_ASSERT_1(is_correct_flags());

        if(!get_flag(STATE_IMPLICIT_VALID))
            force_validate_implicit();
        if(!get_flag(STATE_PARAMETRIC_VALID))
            force_validate_parametric();
    }

    /// Normalize the implicit representation.
    /** This is based on the implicit representation itself (if it's valid) or/and
        on the parametric representation. */
    void normalize_implicit () const
    {
        ARAGELI_ASSERT_1(is_correct_flags());
        if(!get_flag(STATE_IMPLICIT))
            force_normalize_implicit();
    }

    /// Normalize the parametric representation.
    /** This is based on the parametric representation itself (if it's valid) or/and
        on the parametric representation. */
    void normalize_parametric () const
    {
        ARAGELI_ASSERT_1(is_correct_flags());
        if(!get_flag(STATE_PARAMETRIC))
            force_normalize_parametric();
    }

    /// Normalize both implicit and parametric representations.
    void normalize_all () const
    {
        ARAGELI_ASSERT_1(is_correct_flags());
        if(!get_flag(STATE_ALL))
            force_normalize_all();
    }


    //@}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Explicit set of valid/normal flags.

    //@{


    /// Set the valid implicit flag without an actual valid test or validation.
    /** Be careful! If you have set valid flags for both implicit and parametric
        representations but they don't represent the same cone, the future
        behaviour is undefined. */
    void set_valid_implicit ()
    {
        ARAGELI_ASSERT_1(is_correct_flags());
        set_flag(STATE_IMPLICIT_VALID);
    }

    /// Set the valid parametric flag without an actual valid test or validation.
    /** Be careful! If you have set valid flags for both implicit and parametric
        representations but they don't represent the same cone, the future
        behaviour is undefined. */
    void set_valid_parametric ()
    {
        ARAGELI_ASSERT_1(is_correct_flags());
        set_flag(STATE_PARAMETRIC_VALID);
    }

    /// Set the valid both implicit and parametric flags without an actual valid test or validation.
    /** Be careful! If you have set valid flags for both implicit and parametric
        representations but they don't represent the same cone, the future
        behaviour is undefined. */
    void set_valid_all ()
    {
        ARAGELI_ASSERT_1(is_correct_flags());
        set_flag(STATE_VALID);
    }

    /// Set the normal implicit flag without an actual normal test or normalization.
    /** Be careful! If you have set normal flags for both implicit and parametric
        representations but one of them or both aren't normal or they don't represent
        the same cone, the future behaviour is undefined. */
    void set_normal_implicit ()
    {
        ARAGELI_ASSERT_1(is_correct_flags());
        set_flag(STATE_IMPLICIT);
    }

    /// Set the normal parametric flag without an actual normal test or normalization.
    /** Be careful! If you have set normal flags for both implicit and parametric
        representations but one of them or both aren't normal or they don't represent
        the same cone, the future behaviour is undefined. */
    void set_normal_parametric ()
    {
        ARAGELI_ASSERT_1(is_correct_flags());
        set_flag(STATE_PARAMETRIC);
    }

    /// Set the normal both implicit and parametric flags without an actual normal test or normalization.
    /** Be careful! If you have set normal flags for both implicit and parametric
        representations but one of them or both aren't normal or they don't represent
        the same cone, the future behaviour is undefined. */
    void set_normal_all ()
    {
        ARAGELI_ASSERT_1(is_correct_flags());
        set_flag(STATE_ALL);
    }

    //@}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Some properties of a cone.

    //@{


    /// True iff cone is pointed.
    /** Note, cone in 0-space is always pointed. */
    bool is_pointed () const;

    /// True iff cone is proper subspace (including 0-subspace) or whole space.
    bool is_subspace () const;

    /// True iff cone is whole space.
    bool is_space () const;

    /// True iff cone has only one point --- zero.
    bool is_null () const;

    /// True iff cone has only one point. The same as is_null.
    bool is_point () const
    {
        return is_null();
    }

    /// Space dimention.
    dim_type space_dim () const;

    /// Cone dimention.
    dim_type dim () const;

    /// True iff cone is a bodily, i.e. when its dimention is equal to the space dimention.
    bool is_bodily () const
    {
        return dim() == space_dim();
    }

    /// True iff cone is a simplicial cone, i.e. when the minimal system of generatrices is a square non singular matrix.
    bool is_simplicial () const;

    //@}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name The cone and a ray positional relationship.

    //@{


    /// True iff the ray x extends through relative internal area of the cone.
    template <typename V1>
    bool strict_inside (const V1& x) const
    {
        normalize_implicit();
        return
            all_greater(inequation_m*x, null<typename V1::element_type>()) &&
            Arageli::is_null(equation_m*x);
    }

    /// True iff the ray x and the cone have only one common point --- zero.
    template <typename V1>
    bool strict_outside (const V1& x) const
    {
        normalize_implicit();
        return
            !all_greater_equal(inequation_m*x, null<typename V1::element_type>()) ||
            !Arageli::is_null(equation_m*x);
    }

    /// True iff the ray x and the cone have more than one common point.
    template <typename V1>
    bool unstrict_inside (const V1& x) const
    {
        normalize_implicit();
        return
            all_greater_equal(inequation_m*x, null<typename V1::element_type>()) &&
            Arageli::is_null(equation_m*x);
    }

    /// True iff the ray lies on a facet of the cone or they have only one common point.
    template <typename V1>
    bool unstrict_outside (const V1& x) const
    {
        normalize_implicit();
        return
            !all_greater(inequation_m*x, null<typename V1::element_type>()) ||
            !Arageli::is_null(equation_m*x);
    }

    //@}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Several representations of cone and related representations.

    //@{

    /// Returns set of inequations that completely defines the cone.
    /** The returned system can be not a minimal system.
        To obtain a minimal system of inequations use min_inequation. */
    inequation_type inequation () const;

    /// Try to represent cone as a subspace. Returns equation matrix.
    /** If this cone isn't actually subspase,
        cannot_represent_cone exception is being generated. */
    const equation_type& equation () const;

    /// Returns set of generatrices that completely defines the cone.
    /** The returned system can be not a minimal system.
        To obtain a minimal system of generatrix use min_generatrix. */
    generatrix_type generatrix () const;

    /// Try to represent cone as a subspace. Returns basis.
    /** If this cone isn't actually subspace,
        cannot_represent_cone exception is being generated. */
    const basis_type& basis () const;

    const equation_type& min_ambient_equation () const;

    /// A basis of the minimal subspace that contains the cone.
    /** WARNING! IT IS NOT IMPLEMENTED! */
    basis_type min_ambient_basis () const;
    //{ return inverse(min_ambient_equation()); }    // It's incorrect.

    /// An equation system for the maximum subspace that is contained in the cone.
    /** WARNING! IT IS NOT IMPLEMENTED! */
    equation_type max_embedded_equation () const;
    //{ return inverse(max_embedded_basis()); }    // It's incorrect.

    /// A basis of the maximum subspace that is contained in the cone.
    const basis_type& max_embedded_basis () const;

    /// Returns a minimal set of inequations that completely defines the cone.
    inequation_type min_inequation () const;

    /// Returns a minimal set of generatrices that completely defines the cone.
    generatrix_type min_generatrix () const;

    //@}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Direct access to the representation matrices.

    //@{

    const inequation_type& inequation_matrix () const
    {
        return inequation_m;
    }

    inequation_type& inequation_matrix ()
    {
        clear_flag(STATE_PARAMETRIC | STATE_IMPLICIT_NORMAL);
        return inequation_m;
    }

    const equation_type& equation_matrix () const
    {
        return equation_m;
    }

    equation_type& equation_matrix ()
    {
        clear_flag(STATE_PARAMETRIC | STATE_IMPLICIT_NORMAL);
        return equation_m;
    }

    const generatrix_type& generatrix_matrix () const
    {
        return generatrix_m;
    }

    generatrix_type& generatrix_matrix ()
    {
        clear_flag(STATE_IMPLICIT | STATE_PARAMETRIC_NORMAL);
        return generatrix_m;
    }

    const basis_type& basis_matrix () const
    {
        return basis_m;
    }

    basis_type& basis_matrix ()
    {
        clear_flag(STATE_IMPLICIT | STATE_PARAMETRIC_NORMAL);
        return basis_m;
    }

    //@}


    /// Free all temporary storages and the one of the representations.
    void pack ();

    /// Validate the implicit representation and free parametric one.
    void keep_only_implicit ();

    /// Validate the parametric representation and free implicit one.
    void keep_only_parametric ();

    void assign_null ();

    /// Relation matrix between the rays and the hyperplanes.
    /** Each row of the returned matrix corresponds to the ray with
        the same number in the generatrix matrix. Each column coresponds to
        the inequality with the same number in the inequality matrix.

        If an element of the matrix is zero than the corresponding ray
        lies on the corresponding hyperplane. If an element is 1
        than the ray and the hyperplane have only one common point --- zero.

        Be careful! This function doesn't guarantees that the matrix
        will be minimal. So the rays can be not extreme rays and
        the inequations may not correspond to the facets of the cone.
        To obtain minimal matrix, call normalize_all function before
        calling this function.

        WARNING! TEMPORARY LIMITATION.
        Function returns only information concerning the pointed part
        of the cone (i.e. without subspace). It definitely works well
        for pointed cones.
        */
    matrix<bool> relation () const
    {
        validate_all();
        return generatrix_m*transpose(inequation_m);
    }


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Intersection and conic union.

    //@{

    /// Intersects this cone with the cone x.
    /** The result is stored as this object. */
    template <typename Cone1>
    cone& intersection (const Cone1& x);

    /// Unites this cone with the cone x and builds conic hull on the result.
    /** The result is stored as this object. */
    template <typename Cone1>
    cone& conic_union (const Cone1& x);

    //@}

private:

    void force_validate_implicit () const;
    void force_validate_parametric () const;
    void force_normalize_implicit () const;
    void force_normalize_parametric () const;
    void force_normalize_all () const;

    template <typename M1, typename M2>
    inline void extend_rep_matrix (M1& res, const M2& x) const
    {
        // WARNING! Here we add 2*x.nrows() rows in res,
        // but only (x.nrows()+1) is enough.
        res.insert_matrix_bottom(x);
        res.insert_matrix_bottom(-x);
    }

    template <typename M1, typename M2>
    inline void fast_extend_rep_matrix (M1& res, const M2& x) const
    {
        res.insert_matrix_bottom(x);
        res.insert_matrix_bottom(-x);
    }

    enum State
    {
        STATE_IMPLICIT_VALID    = 0x01,
        STATE_IMPLICIT_NORMAL   = 0x01 << 1,
        STATE_PARAMETRIC_VALID  = 0x01 << 2,
        STATE_PARAMETRIC_NORMAL = 0x01 << 3,

        STATE_VALID = STATE_IMPLICIT_VALID | STATE_PARAMETRIC_VALID,
        STATE_NORMAL = STATE_IMPLICIT_NORMAL | STATE_PARAMETRIC_NORMAL,
        STATE_IMPLICIT = STATE_IMPLICIT_VALID | STATE_IMPLICIT_NORMAL,
        STATE_PARAMETRIC = STATE_PARAMETRIC_VALID | STATE_PARAMETRIC_NORMAL,

        STATE_ALL = STATE_IMPLICIT | STATE_PARAMETRIC
    };

    bool get_flag (int st) const
    {
        return (state & st) == st;
    }

    void set_flag (int st) const
    {
        state = State(state | st);
    }

    void clear_flag (int st) const
    {
        state = State(state & ~st);
    }


    mutable inequation_type inequation_m;
    mutable equation_type equation_m;
    mutable generatrix_type generatrix_m;
    mutable basis_type basis_m;

    mutable State state;


    // We suppose that the following functions will be called
    // only in the debug purposes.

    bool is_correct_flags () const
    {
        return
            (get_flag(STATE_IMPLICIT_NORMAL) ?
                get_flag(STATE_IMPLICIT_VALID) : true) &&
            (get_flag(STATE_PARAMETRIC_NORMAL) ?
                get_flag(STATE_PARAMETRIC_VALID) : true);
    }

    bool is_there_valid () const
    {
        return
            get_flag(STATE_IMPLICIT_VALID) &&
                inequation_m.ncols() == equation_m.ncols() ||
            get_flag(STATE_PARAMETRIC_VALID) &&
                generatrix_m.ncols() == basis_m.ncols();
    }

    template <typename Out, typename T2, typename M2, typename CFG2>
    friend void output_list (Out& out, const cone<T2, M2, CFG2>& x);

};


/// Specialization of factory structure for cone template class.
template <typename TT, typename M, typename CFG>
struct factory<cone<TT, M, CFG> >
{
private:

    typedef cone<TT, M, CFG> T;

public:

    static const bool is_specialized = true;

    ///// Unite element don't have meaning for cones.
    //static const T& unit ();

    ///// Unite element don't have meaning for cones.
    //static const T& unit (const T& x);

    ///// противоположный к единичному элементу (-1)
    //static const T& opposite_unit ();

    ///// противоположный к единичному элементу (-1)
    //static const T& opposite_unit (const T& x);

    /// Zero.
    static const T& null ()
    {
        static const T null_s = T();
        return null_s;
    }

    /// Zero.
    static const T& null (const T& x)
    {
        return null();
    }

};



template <typename Out, typename T, typename M, typename CFG>
inline Out& operator<< (Out& out, const cone<T, M, CFG>& x)
{
    output_list(out, x);
    return out;
}



template
<
    typename T1, typename M1, typename CFG1,
    typename T2, typename M2, typename CFG2
>
inline cone<T1, M1, CFG1> intersection
(
    cone<T1, M1, CFG1> a,
    const cone<T2, M2, CFG2>& b
)
{
    return a.intersection(b);
}


template
<
    typename T1, typename M1, typename CFG1,
    typename T2, typename M2, typename CFG2
>
inline cone<T1, M1, CFG1> cone_union
(
    cone<T1, M1, CFG1> a,
    const cone<T2, M2, CFG2>& b
)
{
    return a.cone_union(b);
}


/// Compares two cones.
/** WARNING! Temporary implimentation. Maybe it doesn't work always. */
template
<
    typename T1, typename M1, typename CFG1,
    typename T2, typename M2, typename CFG2
>
int cmp
(
    const cone<T1, M1, CFG1>& a,
    const cone<T2, M2, CFG2>& b
);


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_cone
    #include "cone.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_cone
#endif

#endif    // #ifndef _ARAGELI_cone_hpp_
