/*****************************************************************************

    linopt.hpp -- root functions for linear and integer linear optimization.

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
    This file contains declarations for functions for solving linear and
    integer linear programming problems. For more information about terminology
    see Шевченко В.Н., Золотых Н.Ю. Линейное и целочисленное линейное
    программирование: Учебник. -- Нижний Новгород: Изд-во Нижегородского
    университета им. Н.И. Лобачевского, 2005, paragraph 1.3.
*/


#ifndef _ARAGELI_linopt_hpp_
#define _ARAGELI_linopt_hpp_

#include "config.hpp"

// REFERENCE ADDITIONAL HEADERS HERE


namespace Arageli
{


/// Resulting type of all optimization functions.
enum linopt_result
{
    lor_found,      ///< an optimal vector is found
    lor_empty,      ///< there are no allowable vectors
    lor_infinite    ///< the criterion function is unbounded
};


/// Solve a linear programming problem in a canonical notation.
template
<
    typename A,
    typename B,
    typename C,
    typename Res,
    typename BasisX,
    typename Basis
>
linopt_result linear_maximize_canonical
(
    const A& a,
    const B& b,
    const C& c,
    Res& res,
    BasisX& basis_x,
    Basis& basis
);


/// Solve a linear programming problem in a canonical notation.
template
<
    typename A,
    typename B,
    typename C,
    typename Res,
    typename X
>
linopt_result linear_maximize_canonical
(
    const A& a,
    const B& b,
    const C& c,
    Res& res,
    X& x
);


/// Solve an integer linear programming problem in a canonical notation.
template
<
    typename A,
    typename B,
    typename C,
    typename Res,
    typename X
>
linopt_result intlinear_maximize_canonical
(
    A a,
    B b,
    const C& c,
    Res& res,
    X& x
);


/// Solve a linear programming problem in a canonical notation.
template
<
    typename A,
    typename B,
    typename C,
    typename Res,
    typename BasisX,
    typename Basis
>
linopt_result linear_minimize_canonical
(
    const A& a,
    const B& b,
    const C& c,
    Res& res,
    BasisX& basis_x,
    Basis& basis
);


/// Solve a linear programming problem in a canonical notation.
template
<
    typename A,
    typename B,
    typename C,
    typename Res,
    typename X
>
linopt_result linear_minimize_canonical
(
    const A& a,
    const B& b,
    const C& c,
    Res& res,
    X& x
);


/// Solve an integer linear programming problem in a canonical notation.
template
<
    typename A,
    typename B,
    typename C,
    typename Res,
    typename X
>
linopt_result intlinear_minimize_canonical
(
    const A& a,
    const B& b,
    const C& c,
    Res& res,
    X& x
);


/// Solve an integer linear programming problem that is dual to the given in a canonical notation.
template
<
    typename A,
    typename B,
    typename C,
    typename Res,
    typename X
>
linopt_result intlinear_minimize_dual_canonical
(
    const A& a,
    const B& b,
    const C& c,
    Res& res,
    X& x
);


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_linopt
    #include "linopt.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_linopt
#endif

#endif    // #ifndef _ARAGELI_linopt_hpp_
