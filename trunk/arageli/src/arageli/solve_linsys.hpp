/*****************************************************************************

    solve_linsys.hpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2006--2007 Sergey S. Lyalin
    Copyright (C) 2006 Anna Bestaeva

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
    \file solve_linsys.hpp
    \brief Solving of linear systems of equations over different structures.

    This file contains routines to solve of linear systems of equations over
    different mathematical domains.

    WARNING! This file contains partially not yet implemented functions.
    Some already implemented functions have restricted implementations.
    The implementation is in progress.
*/


#ifndef _ARAGELI_solve_linsys_hpp_
#define _ARAGELI_solve_linsys_hpp_

#include "config.hpp"

#include "exception.hpp"
#include "vector.hpp"


namespace Arageli
{


/// An exception for non-one-point solution of a linear system.
/** This exception is generated when one of the solve functions cannot
    represent a solution as one point. */
class no_unique_solution : public exception {};


/// Status of an execution completion for functions from solve_linsys family.
enum solve_linsys_result
{
    SLR_EMPTY,      ///< there no any point satisfied to a system
    SLR_UNIQUE,     ///< there is unique point satisfied to a system
    SLR_MULTIPLE    ///< there are many points satisfied to a system
};


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/**    @name Functions for solving integer linear systems. */
//@{

/// Solve a heterogeneous integer linear system.
/** See description for corresponding solve_linsys function.
    For this function lambda coefficients are any in the integer ring.
    All coefficients in matrices and vectors are integer numbers. */
template <typename A, typename B, typename GX, typename Offset>
solve_linsys_result solve_linsys_integer
(const A& a, const B& b, Offset& offset, GX& gx);


/// Solve a heterogeneous integer linear system.
/** See description for corresponding solve_linsys function.
    All coefficients in matrices and vectors are integer numbers. */
template <typename A, typename B, typename X>
inline solve_linsys_result solve_linsys_integer (const A& a, const B& b, X& x)
{
    A gx;
    return solve_linsys_integer(a, b, x, gx);
}


/// Solve a heterogeneous integer linear system with unique solution.
/** See description for corresponding solve_linsys function.
    All coefficients in matrices and vectors are integer numbers. */
template <typename A, typename B>
inline B solve_linsys_integer (const A& a, const B& b)
{
    B x;
    if(solve_linsys_integer(a, b, x) != SLR_UNIQUE)
        throw no_unique_solution();
    return x;
}


/// Solve a homogeneous integer linear system.
/** See description for corresponding solve_linsys function.
    For this function lambda coefficients are any in the integer ring.
    All coefficients in matrices and vectors are integer numbers. */
template <typename A>
inline A solve_linsys_integer (const A& a)
{
    vector<typename A::element_type> b(a.ncols()), offset;
    A gx;
    solve_linsys_integer(a, b, offset, gx);
    ARAGELI_ASSERT_1(is_null(offset));
    return gx;
}

//@}


/**    @name Functions for solving modular integer linear modular systems. */
//@{

/// Solve a homogeneous integer linear system modulo d.
/** Solve linear system a*x = 0 (modulo d), where a is a matrix, x is a vector,
    d is an integer number. Any solution x is uniquely determined by
    the formule: x = res(sum(lambda[i]*gx.col(i), i = 0..gx.ncols()-1), d),
    where lamdba[i] = 0..order[i]-1, i = 0..gx.ncols()-1, order is a vector.
    Temporary limitation is that matrix 'a' must be square and has a full rank.*/
template <typename A, typename D, typename GX, typename Order>
solve_linsys_result solve_linsys_integer_mod
(const A& a, const D& d, GX& gx, Order& order);


//@}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/**    @name Functions for solving rational linear systems. */
//@{

/// Solve a heterogeneous rational linear system.
/** See description for corresponding solve_linsys function.
    For this function lambda coefficients are any in the rational field.
    All coefficients in matrices and vectors are rational numbers. */
template <typename A, typename B, typename GX, typename Offset>
solve_linsys_result solve_linsys_rational
(const A& a, const B& b, Offset& offset, GX& gx);


/// Solve a heterogeneous rational linear system.
/** See description for corresponding solve_linsys_rational function.
    The difference from solve_linsys_rational is that this function
    take two parameters -- matrix A and vector b -- as one matrix:
    ba = (-b|A). */
template <typename BA, typename GX, typename Offset>
solve_linsys_result solve_linsys_rational_ba
(const BA& ba, Offset& offset, GX& gx);


/// Solve a heterogeneous rational linear system.
/** See description for corresponding solve_linsys function.
    All coefficients in matrices and vectors are rational numbers. */
template <typename A, typename B, typename X>
inline solve_linsys_result solve_linsys_rational (const A& a, const B& b, X& x)
{
    A gx;
    return solve_linsys_rational(a, b, x, gx);
}


/// Solve a heterogeneous rational linear system.
/** See description for corresponding solve_linsys_rational function.
    The difference from solve_linsys_rational is that this function
    take two parameters -- matrix A and vector b -- as one matrix:
    ba = (-b|A). */
template <typename BA, typename X>
solve_linsys_result solve_linsys_rational_ba (const BA& ba, X& x);


/// Solve a heterogeneous rational linear system with unique solution.
/** See description for corresponding solve_linsys function.
    All coefficients in matrices and vectors are rational numbers. */
template <typename A, typename B>
inline B solve_linsys_rational (const A& a, const B& b)
{
    B x;
    if(solve_linsys_rational(a, b, x) != SLR_UNIQUE)
        throw no_unique_solution();
    return x;
}


/// Solve a homogeneous rational linear system.
/** See description for corresponding solve_linsys function.
    For this function lambda coefficients are any in the rational field.
    All coefficients in matrices and vectors are rational numbers. */
template <typename A>
inline A solve_linsys_rational (const A& a)
{
    vector<typename A::element_type> b(a.ncols()), offset;
    A gx;
    solve_linsys_rational(a, b, offset, gx);
    ARAGELI_ASSERT_1(is_null(offset));
    return gx;
}

//@}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/**    @name Functions for solving linear systems in some field. */
//@{

/// Solve a heterogeneous linear system.
/** See description for corresponding solve_linsys function.
    For this function lambda coefficients are any in some field.
    All coefficients in matrices and vectors are from some field.
    WARNING! It isn't implemented yet. */
template <typename A, typename B, typename GX, typename Offset>
solve_linsys_result solve_linsys_field
(const A& a, const B& b, Offset& offset, GX& gx);


/// Solve a heterogeneous linear system.
/** See description for corresponding solve_linsys function.
    All coefficients in matrices and vectors are from some field.
    WARNING! Slow implementation. */
template <typename A, typename B, typename X>
solve_linsys_result solve_linsys_field (const A& a, const B& b, X& x);


/// Solve a heterogeneous linear system with unique solution.
/** See description for corresponding solve_linsys function.
    All coefficients in matrices and vectors are from some field.
    WARNING! Slow implementation. */
template <typename A, typename B>
B solve_linsys_field (const A& a, const B& b);


/// Solve a homogeneous linear system.
/** See description for corresponding solve_linsys function.
    For this function lambda coefficients are any in some field.
    All coefficients in matrices and vectors are from some field.
    WARNING! Slow implementation. */
template <typename A>
inline A solve_linsys_field (const A& a)
{
    vector<typename A::element_type> b(a.ncols()), offset;
    A gx;
    solve_linsys_field(a, b, offset, gx);
    ARAGELI_ASSERT_1(is_null(offset));
    return gx;
}

//@}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


/**    @name Generic functions for linear systems solving.
    Each function from this group solve heterogeneous or homogeneous
    linear system in domain that determined by types of arguments.
*/
//@{

/// Solve a heterogeneous linear system.
/** Solve linear system a*x = b, where 'a' is a matrix, x and b are vectors.
    A set what x from is determinated by types A and B.
    If the system doesn't have a solution, the function returns SLR_EMPTY.
    If the system have a solution(s), gx is a matrix of generators:
    any solution x is uniquely represented as
        x = offset + sum(lambda[i]*gx.col(i), i = 0..gx.ncols()-1),
    where lambda[i], i = 0..gx.ncols()-1 are some coefficients from a set that
    determinated by types A and B. Coefficients lambda are uniquely determined
    for each x.
    WARNING! Temporary this function just calls solve_linsys_field. */
template <typename A, typename B, typename GX, typename Offset>
inline solve_linsys_result solve_linsys
(const A& a, const B& b, Offset& offset, GX& gx)
{
    ARAGELI_ASSERT_0(type_traits<typename A::element_type>::is_field);
    return solve_linsys_field(a, b, offset, gx);
}


/// Solve a heterogeneous linear system.
/** Solve linear system a*x = b, where 'a' is a matrix, x and b are vectors.
    A set what x from is determinated by types A and B.
    If the system have only one point x as a solution, that solution is stored
    as x argument and the function returns SLR_UNIQUE. If the system doesn't have
    a solution, x isn't changed and the function returns SLR_EMPTY. If the system
    have a multiple solution, x is set equal to the one of them and the function
    returns SLR_MULTIPLE.
    WARNING! Temporary this function just calls solve_linsys_field. */
template <typename A, typename B, typename X>
inline solve_linsys_result solve_linsys (const A& a, const B& b, X& x)
{
    ARAGELI_ASSERT_0(type_traits<typename A::element_type>::is_field);
    return solve_linsys_field(a, b, x);
}


/// Solve a heterogeneous linear system with unique solution.
/** Solve linear system a*x = b, where 'a' is a matrix, x and b are vectors.
    A set what x from is determinated by types A and B.
    If the system have only one point x as a solution, that solution
    is returned. If the system doesn't have a solution or have multiple
    solutions, the function throws exception no_unique_solution.
    WARNING! Temporary this function just calls solve_linsys_field. */
template <typename A, typename B>
inline B solve_linsys (const A& a, const B& b)
{
    ARAGELI_ASSERT_0(type_traits<typename A::element_type>::is_field);
    return solve_linsys_field(a, b);
}


/// Solve a homogeneous linear system.
/** Solve linear system a*x = 0, where 'a' is a matrix, x is a vector.
    A set what x from is determinated by types A and B.    Returns matrix of
    generators. Each x is represented as
        x = sum(lambda[i]*gx.col(i), i = 0..gx.ncols()-1)
    where gx is returned matrix and lambda[i], i = 0..gx.ncols()-1 are some
    coefficients from a set that determinated by types A and B. Coefficients
    lambda are uniquely determined for each x.
    WARNING! Temporary this function just calls solve_linsys_field. */
template <typename A>
inline A solve_linsys (const A& a)
{
    ARAGELI_ASSERT_0(type_traits<typename A::element_type>::is_field);
    return solve_linsys(a);
}

//@}



} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_solve_linsys
    #include "solve_linsys.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_solve_linsys
#endif

#endif    // #ifndef _ARAGELI_solve_linsys_hpp_
