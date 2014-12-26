/*****************************************************************************

    motzkin_burger.hpp

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
    \file motzkin_burger.hpp
    \brief Motzkin-Burger algorithm for the double-description of
        a polyhedral cone.

    Motzkin-Burger algorithm for double-description of a polyhedral cone.
    The algorithm finds extreame rays and basis of cone Ax >= 0, where
    A is a matrix and x is a vector. Also the file contains an idle
    controller for this algorithm.
*/

#ifndef _ARAGELI_motzkin_burger_hpp_
#define _ARAGELI_motzkin_burger_hpp_

#include "config.hpp"

#include <vector>


namespace Arageli
{

namespace ctrl
{

struct skeleton_motzkin_burger_idler
{
    class abort :
        public ctrl::abort
    {};

    template <typename A, typename F, typename Q, typename E>
    void preamble (const A& a, const F& f, const Q& q, const E& e) const
    {}

    void begin_gauss () const
    {}

    void end_gauss () const
    {}

    template <typename I>
    void find_non_zero (const I& i) const
    {}

    void zero_row () const
    {}

    template <typename I, typename J>
    void swap_cols_rows (const I& i, const J& j) const
    {}

    template <typename J>
    void eliminate_col (const J& j) const
    {}

    template <typename A, typename F, typename Q>
    void show_matrices (const A& a, const F& f, const Q& q) const
    {}

    void begin_motzkin () const
    {}

    void end_motzkin () const
    {}

    template <typename I1, typename I2>
    void select_col (const I1& current_column, const I2& new_column) const
    {}

    void zero_solution () const
    {}

    void corollary_inequality () const
    {}

    void begin_row_balancing () const
    {}

    template <typename I, typename J>
    void balanced_rows (const I& j_p, const J& j_m) const
    {}

    void end_row_balancing () const
    {}

    void delete_negates () const
    {}

    template <typename A, typename F, typename Q, typename E>
    void conclusion (const A& a, const F& f, const Q& q, const E& e) const
    {}
};

} // namespace ctrl


/// The Motzkin-Burger algorithm without modifications.
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
void skeleton_motzkin_burger
(
    A& a,
    F& f,
    Q& q,
    E& e,
    Ctrler ctrler
);


/// The Motzkin-Burger algorithm without modifications.
/** Just calls full version of the function. */
template <typename A, typename F, typename Q, typename E>
inline void skeleton_motzkin_burger (A& a, F& f, Q& q, E& e)
{
    return
        skeleton_motzkin_burger
        (
            a,
            f,
            q,
            e,
            ctrl::skeleton_motzkin_burger_idler()
        );
}


/// The Motzkin-Burger algorithm with selecting minimum elements.
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
void skeleton_motzkin_burger_min
(
    A& a,
    F& f,
    Q& q,
    E& e,
    Ctrler ctrler
);


/// The Motzkin-Burger algorithm with selecting minimum elements.
/** Just calls full version of the function. */
template <typename A, typename F, typename Q, typename E>
inline void skeleton_motzkin_burger_min (A& a, F& f, Q& q, E& e)
{
    return
        skeleton_motzkin_burger_min
        (
            a,
            f,
            q,
            e,
            ctrl::skeleton_motzkin_burger_idler()
        );
}


/// The Motzkin-Burger algorithm with selecting maximum elements.
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
void skeleton_motzkin_burger_max
(
    A& a,
    F& f,
    Q& q,
    E& e,
    Ctrler ctrler
);


/// The Motzkin-Burger algorithm with selecting maximum elements.
/** Just calls full version of the function. */
template <typename A, typename F, typename Q, typename E>
inline void skeleton_motzkin_burger_max (A& a, F& f, Q& q, E& e)
{
    return
        skeleton_motzkin_burger_max
        (
            a,
            f,
            q,
            e,
            ctrl::skeleton_motzkin_burger_idler()
        );
}


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_MOTZKIN_BURGER
    #include "motzkin_burger.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_MOTZKIN_BURGER
#endif

#endif
