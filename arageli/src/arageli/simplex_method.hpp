/*****************************************************************************

    simplex_method.hpp -- Simplex method and other.

    This file is part of Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2005--2008 Sergey S. Lyalin

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

#ifndef _ARAGELI_SIMPLEX_METHOD_HPP_
#define _ARAGELI_SIMPLEX_METHOD_HPP_


/**
    \file
    Direct and dual simplex methods with different ways of choosing
    of pivot elements. Set of controller objects.  There are as row
    as columnt methods, as noninteger as integer ones.
*/


#include "config.hpp"

#include <cstddef>
#include <iostream>
#include <iomanip>
#include <algorithm>

#include "factory.hpp"
#include "exception.hpp"
#include "cmp.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "gauss.hpp"

#include "std_import.hpp"

namespace Arageli
{


/// Facilities for doing a linear optimization with the simplex method.
namespace simplex_method
{

// ***************************************************************************

/// @name Looking for parts of a simplex table that is not satisfied certain criterion.
//@{

/// Finds the first not allowable basis column in a row simplex table.
/** WARNING! EXPLANATION MAYBE INCORRECT.
    Suppose 'a' is a row simplex table corresponding the basis.
    The function finds the column such that 1) its number j (j > 0)
    is contained in basis at position k, 2) there is such number i
    in {0, ..., a.nrows()-1} \ {k+1} that a(i, j) != 0, 3) a(k+1, j) == 1.
    Returns the first such index at 'basis', i.e. basis[return value] is
    such column number. */
template <typename T1, bool REFCNT1, typename T2, bool REFCNT2>
inline typename matrix<T1, REFCNT1>::size_type find_row_notallow
(const matrix<T1, REFCNT1>& a, const vector<T2, REFCNT2>& basis)
{
    for(std::size_t i = 0; i < basis.size(); ++i)
    {
        T2 j = basis[i];
        for(std::size_t r = 0; r < a.nrows(); ++r)
            if
            (!(
                (r == i+1 && is_unit(a(r, j))) ||
                is_null(a(r, j))
            ))
                return i;
    }

    return basis.size();
}


/// Finds the first not allowable basis row in a column simplex table.
/** WARNING! EXPLANATION MAYBE INCORRECT.
    Suppose 'a' is a column simplex table corresponding the nonbasis.
    The function finds the row such that 1) its number j (j > 0)
    is contained in nonbasis at position k, 2) there is a number i
    in {0, ..., a.ncols()-1} \ {k+1}, a(j, i) != 0, 3) a(j, k+1) == -1.
    Returns the first such index at 'nonbasis', i.e. nonbasis[return value] is
    such column number. */
template <typename T1, bool REFCNT1, typename T2, bool REFCNT2>
inline typename matrix<T1, REFCNT1>::size_type find_col_notallow
(const matrix<T1, REFCNT1>& a, const vector<T2, REFCNT2>& nonbasis)
{
    for(std::size_t i = 0; i < nonbasis.size(); ++i)
    {
        T2 j = nonbasis[i];
        for(std::size_t s = 0; s < a.ncols(); ++s)
            if
            (!(
                (s == i+1 && is_opposite_unit(a(j, s))) ||
                is_null(a(j, s))
            ))
                return i;
    }

    return nonbasis.size();
}

// ---------------------------------------------------------------------------

/// Finds the first not allowable row in a primal simplex table.
/** Finds the first row i that satisfies an expression a(i, j) < 0 for
    some j (if any).  If there is no such i, returns a.nrows(). */
template <typename T1, bool REFCNT1>
inline typename matrix<T1, REFCNT1>::size_type find_primal_notallow
(const matrix<T1, REFCNT1>& a)
{
    ARAGELI_ASSERT_0(a.ncols() > 0);

    for(std::size_t i = 1; i < a.nrows(); ++i)
        if(is_negative(a(i, 0)))return i;
    return a.nrows();
}


/// Finds the first not allowable column in a dual simplex table.
/** Finds the first column j that satisfies an expression a(i, j) < 0 for
    some i (if any).  If there is no such j, returns a.ncols(). */
template <typename T1, bool REFCNT1>
inline typename matrix<T1, REFCNT1>::size_type find_dual_notallow
(const matrix<T1, REFCNT1>& a)
{
    ARAGELI_ASSERT_0(a.ncols() > 0);

    return std::find_if
    (
        a.begin() + 1, a.begin() + a.ncols(),
        func::is_negative<T1>()
    ) - a.begin();
}

//@}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/// @name Determination type of a simplex table.
//@{

/// Returns true if simplex table 'a' with basis 'basis' is row simplex table.
template <typename T1, bool REFCNT1, typename T2, bool REFCNT2>
inline bool is_row_allow
(const matrix<T1, REFCNT1>& a, const vector<T2, REFCNT2>& basis)
{
    return find_row_notallow(a, basis) == basis.size();
}


/// Returns true if simplex table 'a' with nonbasis 'nonbasis' is column simplex table.
template <typename T1, bool REFCNT1, typename T2, bool REFCNT2>
inline bool is_col_allow
(const matrix<T1, REFCNT1>& a, const vector<T2, REFCNT2>& nonbasis)
{
    return find_col_notallow(a, nonbasis) == nonbasis.size();
}


/// Returns true if simplex table 'a' is primal allowable.
template <typename T1, bool REFCNT1>
inline bool is_primal_allow
(const matrix<T1, REFCNT1>& a)
{
    return find_primal_notallow(a) == a.nrows();
}


/// Returns true if simplex table 'a' is dual allowable.
template <typename T1, bool REFCNT1>
inline bool is_dual_allow
(const matrix<T1, REFCNT1>& a)
{
    return find_dual_notallow(a) == a.ncols();
}

//@}
// ***************************************************************************

/// General kind of result of one or more iterations of the simplex method.
enum result_kind
{
    rk_found,    ///< an optimal vector is found
    rk_empty,    ///< there are no allowable vectors
    rk_infinite,    ///< the criterion function is unbounded
    rk_nonoptimal    ///< an optimal vector is not found (to be found next iter)
    //rk_nonint        ///< an optimal vector is not integer yet
};

// ***************************************************************************

/// @name Rules to choice a pivot item in a simplex table.
//@{

/// Rule to choice the first appropriate column as pivot in the primal simplex method.
struct rule_s_primal_first
{
    template <typename T_Q, bool REFCNT_Q, typename T_basis, bool REFCNT_basis>
    typename matrix<T_Q, REFCNT_Q>::size_type operator()
    (
        const matrix<T_Q, REFCNT_Q>& Q,
        const vector<T_basis, REFCNT_basis>& basis
    ) const
    {
        return find_dual_notallow(Q);
    }
};


/// Rule to choice the first appropriate row as pivot in the primal simplex method.
struct rule_r_primal_first
{
    template <typename T_Q, bool REFCNT_Q, typename T_basis, bool REFCNT_basis>
    typename matrix<T_Q, REFCNT_Q>::size_type operator()
    (
        const matrix<T_Q, REFCNT_Q>& Q,
        const vector<T_basis, REFCNT_basis>& basis,
        typename matrix<T_Q, REFCNT_Q>::size_type s
    ) const
    {
        ARAGELI_ASSERT_0(s > 0);
        ARAGELI_ASSERT_0(s < Q.ncols());
        ARAGELI_ASSERT_0(Q.nrows() > 0);

        typedef typename matrix<T_Q, REFCNT_Q>::size_type size_type;
        size_type r = 1;
        for(; r < Q.nrows() && !is_positive(Q(r, s)); ++r);
        if(r == Q.nrows())
            return r;

        T_Q min = Q(r, 0)/Q(r, s);

        for(size_type i = r + 1; i < Q.nrows(); ++i)
            if(is_positive(Q(i, s)))
            {
                T_Q t = Q(i, 0)/Q(i, s);
                if(t < min)
                {
                    min = t;
                    r = i;
                }
            }

        return r;
    }
};


/// Rule to choice the lexmin appropriate row as pivot in the primal simplex method.
struct rule_r_primal_lex
{
    template <typename T_Q, bool REFCNT_Q, typename T_basis, bool REFCNT_basis>
    typename matrix<T_Q, REFCNT_Q>::size_type operator()
    (
        const matrix<T_Q, REFCNT_Q>& Q,
        const vector<T_basis, REFCNT_basis>& basis,
        typename matrix<T_Q, REFCNT_Q>::size_type s
    ) const
    {
        ARAGELI_ASSERT_0(s > 0);
        ARAGELI_ASSERT_0(s < Q.ncols());
        ARAGELI_ASSERT_0(Q.nrows() > 0);

        typedef typename matrix<T_Q, REFCNT_Q>::size_type size_type;
        size_type r = 1;
        for(; r < Q.nrows() && !is_positive(Q(r, s)); ++r);
        if(r == Q.nrows())
            return r;

        T_Q min = Q(r, 0)/Q(r, s);

        for(size_type i = r + 1; i < Q.nrows(); ++i)
            if(is_positive(Q(i, s)))
            {
                T_Q t = Q(i, 0)/Q(i, s);
                if(t < min)
                {
                    min = t;
                    r = i;
                }
                else if(t == min)
                    for(size_type j = 1; j < Q.ncols(); ++j)
                    {
                        t = Q(i, j)/Q(i, s);

                        // Little inefficiency: every time it recomputes
                        // the result of division; maybe cache for those
                        // values is better way.
                        T_Q oldt = Q(r, j)/Q(r, s);
                        if(t < oldt)
                        {
                            r = i;
                            break;
                        }
                        else if(t > oldt)break;
                    }
            }

        return r;
    }
};


/// Rule to choice the first appropriate row as pivot in the dual simplex method.
struct rule_r_dual_first
{
    template <typename T_Q, bool REFCNT_Q, typename T_nonbasis, bool REFCNT_nonbasis>
    typename matrix<T_Q, REFCNT_Q>::size_type operator()
    (
        const matrix<T_Q, REFCNT_Q>& Q,
        const vector<T_nonbasis, REFCNT_nonbasis>& basis
    ) const
    {
        return find_primal_notallow(Q);
    }
};


/// Rule to choice the first appropriate column as pivot in the dual simplex method.
struct rule_s_dual_first
{
    template <typename T_Q, bool REFCNT_Q, typename T_nonbasis, bool REFCNT_nonbasis>
    typename matrix<T_Q, REFCNT_Q>::size_type operator()
    (
        const matrix<T_Q, REFCNT_Q>& Q,
        const vector<T_nonbasis, REFCNT_nonbasis>& basis,
        typename matrix<T_Q, REFCNT_Q>::size_type r
    ) const
    {
        ARAGELI_ASSERT_0(r > 0);
        ARAGELI_ASSERT_0(r < Q.nrows());
        ARAGELI_ASSERT_0(Q.ncols() > 0);

        typedef typename matrix<T_Q, REFCNT_Q>::size_type size_type;
        size_type s = 1;
        for(; s < Q.ncols() && !is_negative(Q(r, s)); ++s);
        if(s == Q.ncols())
            return s;

        T_Q max = Q(0, s)/Q(r, s);

        for(size_type i = s + 1; i < Q.ncols(); ++i)
            if(is_negative(Q(r, i)))
            {
                T_Q t = Q(0, i)/Q(r, i);
                if(t > max)
                {
                    max = t;
                    s = i;
                }
            }

        return s;
    }
};

// ***************************************************************************

/// @name The primal row iterations on a row simplex table.
//@{

/// Performs one primal row iteration on a row simplex table.
/** Performs the iteration, determines if the table is optimal or
    if there are no an optimum. The result value depends on status of
    input table, not output. In particular if not optimal table is passed
    to the algorithm and on the output we have an optimal table then
    the functions returns rk_nonoptimal. */
template
<
    typename T_q,
    bool REFCNT_q,
    typename T_basis,
    bool REFCNT_basis,
    typename T_pivot,
    typename Rule_s,
    typename Rule_r
>
result_kind primal_row_iter
(
    matrix<T_q, REFCNT_q>& q,    ///< the simplex table
    vector<T_basis, REFCNT_basis>& basis,    ///< the basis
    T_pivot& prow,    ///< row number of pivot item (output, valid after call if table is nonoptimal)
    T_pivot& pcol,    ///< column number of pivot item (output, valid after call if table is nonoptimal)
    Rule_s rule_s,    ///< the rule to choice a pivot column
    Rule_r rule_r    ///< the rule to choice a pivot row
)
{
    ARAGELI_ASSERT_0(is_row_allow(q, basis));
    ARAGELI_ASSERT_0(is_primal_allow(q));
    typedef typename matrix<T_q, REFCNT_q>::size_type size_type;

    size_type s = rule_s(q, basis);
    if(s == q.ncols())
        return rk_found;
    pcol = s;
    size_type r = rule_r(q, basis, s);
    if(r == q.nrows())
        return rk_infinite;
    prow = r;
    gauss_field_row_iter(q, r, s);
    basis[r-1] = s;
    return rk_nonoptimal;
}


/// Performs one primal row iteration on a row simplex table.
/** Just calls full vertion of the method with some default arguments. */
template
<
    typename T_q,
    bool REFCNT_q,
    typename T_basis,
    bool REFCNT_basis
>
inline result_kind primal_row_iter
(
    matrix<T_q, REFCNT_q>& q,
    vector<T_basis, REFCNT_basis>& basis
)
{
    typename matrix<T_q, REFCNT_q>::size_type t;

    return primal_row_iter
    (
        q, basis, t, t,
        rule_s_primal_first(),
        rule_r_primal_lex()
    );
}


/// Performs one primal row iteration on a row simplex table.
/** Just calls full vertion of the method with some default arguments. */
template
<
    typename T_q,
    bool REFCNT_q,
    typename T_basis,
    bool REFCNT_basis,
    typename Rule_s,
    typename Rule_r
>
inline result_kind primal_row_iter
(
    matrix<T_q, REFCNT_q>& q,
    vector<T_basis, REFCNT_basis>& basis,
    Rule_s rule_s,
    Rule_r rule_r
)
{
    int t;
    return primal_row_iter(q, basis, t, t, rule_s, rule_r);
}


/// Performs one primal row iteration on a row simplex table.
/** Just calls full vertion of the method primal_row_iter with
    some default arguments. */
template
<
    typename T_q, bool REFCNT_q,
    typename T_basis, bool REFCNT_basis,
    typename T_pivot
>
inline result_kind primal_row_iter_pivotout
(
    matrix<T_q, REFCNT_q>& q,
    vector<T_basis, REFCNT_basis>& basis,
    T_pivot& prow,
    T_pivot& pcol
)
{
    return primal_row_iter
    (
        q,
        basis,
        prow,
        pcol,
        rule_s_primal_first(),
        rule_r_primal_lex()
    );
}

// ---------------------------------------------------------------------------

}

namespace ctrl
{

/// All controller's classes for Arageli::simplex_method routines.
namespace simplex_method
{

using namespace Arageli::simplex_method;

/// Default controller for the primal_row_iters function.  It's doing nothing.
struct primal_row_iters_idler
{
    class abort :
        public ctrl::abort
    {};

    template <typename Q, typename Basis>
    void preamble (const Q& q, const Basis& basis) const
    {}

    template <typename Q, typename Basis>
    void conclusion (const Q& q, const Basis& basis, result_kind rk) const
    {}

    template <typename Q, typename Basis>
    void before_iter (const Q& q, const Basis& basis) const
    {}

    template <typename Q, typename Basis, typename Pivot>
    void after_iter
    (
        const Q& q, const Basis& basis,
        Pivot prow, Pivot pcol,
        result_kind rk
    ) const
    {}
};

}}


namespace simplex_method
{

/// Performs numerous primal row iterations on a row simplex table.
/** Performs the iteration, determines if the table is optimal or
    if there are no an optimum and so on until table becomes optimal. */
template
<
    typename T_q,
    bool REFCNT_q,
    typename T_basis,
    bool REFCNT_basis,
    typename Rule_s,
    typename Rule_r,
    typename Ctrler
>
result_kind primal_row_iters
(
    matrix<T_q, REFCNT_q>& q,
    vector<T_basis, REFCNT_basis>& basis,
    Rule_s rule_s,
    Rule_r rule_r,
    Ctrler ctrler
)
{
    ARAGELI_ASSERT_0(is_row_allow(q, basis));
    ARAGELI_ASSERT_0(is_primal_allow(q));
    typedef typename matrix<T_q, REFCNT_q>::size_type size_type;
    typedef vector<T_basis, REFCNT_basis> Basis;

    ctrler.preamble(q, basis);
    result_kind res = rk_nonoptimal;

    // for the circularity controll
    ARAGELI_DEBUG_EXEC_2(std::list<Basis> bases);

    while(res == rk_nonoptimal)
    {
        ARAGELI_ASSERT_2
        (
            std::find(bases.begin(), bases.end(), basis) == bases.end()
        );

        ARAGELI_DEBUG_EXEC_2(bases.push_back(basis));

        ctrler.before_iter(q, basis);
        size_type prow, pcol;
        res = primal_row_iter(q, basis, prow, pcol, rule_s, rule_r);
        ctrler.after_iter(q, basis, prow, pcol, res);
    }

    ctrler.conclusion(q, basis, res);
    return res;
}


/// Performs numerous primal row iterations on a row simplex table.
/** Just calls full vertion of the method with some default arguments. */
template
<
    typename T_q,
    bool REFCNT_q,
    typename T_basis,
    bool REFCNT_basis
>
inline result_kind primal_row_iters
(
    matrix<T_q, REFCNT_q>& q,
    vector<T_basis, REFCNT_basis>& basis
)
{
    return primal_row_iters
    (
        q,
        basis,
        rule_s_primal_first(),
        rule_r_primal_lex(),
        ::Arageli::ctrl::simplex_method::primal_row_iters_idler()
    );
}


/// Performs numerous primal row iterations on a row simplex table.
/** Just calls full vertion of the method with some default arguments. */
template
<
    typename T_q,
    bool REFCNT_q,
    typename T_basis,
    bool REFCNT_basis,
    typename Ctrler
>
inline result_kind primal_row_iters
(
    matrix<T_q, REFCNT_q>& q,
    vector<T_basis, REFCNT_basis>& basis,
    Ctrler ctrler
)
{
    return primal_row_iters
    (
        q,
        basis,
        rule_s_primal_first(),
        rule_r_primal_lex(),
        ctrler
    );
}

//@}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

} // namespace simplex_method


namespace ctrl
{
namespace simplex_method
{

/// @name The primal column iterations on a column simplex table.  With controllers.
//@{

// WARNING!!! The following two controllers are to remove!

/// Default controller for the primal_col_iter function.  It's doing nothing.
struct primal_col_iter_idler
{
    void preamble () const
    {}

    void conclusion () const
    {}

    template <typename Q, typename Basis>
    void current_table (const Q& q, const Basis& basis) const
    {}

    template <typename R, typename S>
    void pivot_item (const R& r, const S& s) const
    {}

    void found () const
    {}

    void infinite () const
    {}

    void nonoptimal () const
    {}
};


/// Simple controller for the primal_col_iter function.  It outputs into a stream.
template <typename Stream>
struct primal_col_iter_slog
{
    Stream& stream;

    primal_col_iter_slog (Stream& stream_a) :
        stream(stream_a)
    {}

    void preamble () const
    {
        stream << "Find pivot item.\n";
    }

    void conclusion () const
    {}

    template <typename Q, typename Basis>
    void current_table (const Q& q, const Basis& basis) const
    {
        output_aligned(stream << "Q =\n", q);
        stream << "basis = " << basis << ".\n";
    }

    template <typename R, typename S>
    void pivot_item (const R& r, const S& s) const
    {
        stream << "Pivot item: r = " << r << ", s = " << s << ".\n";
    }

    void found () const
    {
        stream << "The table is optimal.\n";
    }

    void infinite () const
    {
        stream << "The function isn't limited.\n";
    }

    void nonoptimal () const
    {}
};

}} // namespace simplex_method, ctrl

namespace simplex_method
{
#if 0    // WARNING! TEMPORARY DISABLED!
/// Performs one primal column iteration on a column simplex table.
/** Performs the iteration, determines if the table is optimal or
    if there are no an optimum. */
template
<
    typename T_Q,
    bool REFCNT_Q,
    typename T_nonbasis,
    bool REFCNT_nonbasis,
    typename Rule_s,
    typename Rule_r,
    typename Ctrler
>
result_kind primal_col_iter
(
    matrix<T_Q, REFCNT_Q>& Q,
    vector<T_nonbasis, REFCNT_nonbasis>& nonbasis,
    Rule_s rule_s,
    Rule_r rule_r,
    Ctrler ctrler
)
{
    ARAGELI_ASSERT_0(is_col_allow(Q, nonbasis));
    ARAGELI_ASSERT_0(is_primal_allow(Q));
    typedef typename matrix<T_Q, REFCNT_Q>::size_type size_type;

    ctrler.preamble();
    ctrler.current_table(Q, nonbasis);

    size_type s = rule_s(Q, nonbasis);
    if(s == Q.ncols())
    {
        ctrler.found();
        ctrler.conclusion();
        return rk_found;
    }

    size_type r = rule_r(Q, nonbasis, s);
    if(r == Q.nrows())
    {
        ctrler.infinite();
        ctrler.conclusion();
        return rk_infinite;
    }

    ctrler.pivot_item(r, s);

    gauss_field_col_iter(Q, r, s);

    for(std::size_t i = 0; i < Q.nrows(); ++i)
        opposite(&Q(i, s));

    nonbasis[s-1] = r;

    ctrler.nonoptimal();
    ctrler.conclusion();

    return rk_nonoptimal;
}


/// Performs one primal column iteration on a column simplex table.
/** Just calls full vertion of the method with some default arguments. */
template
<
    typename T_Q,
    bool REFCNT_Q,
    typename T_nonbasis,
    bool REFCNT_nonbasis
>
inline result_kind primal_col_iter
(
    matrix<T_Q, REFCNT_Q>& Q,
    vector<T_nonbasis, REFCNT_nonbasis>& nonbasis
)
{
    return primal_col_iter
    (
        Q,
        nonbasis,
        rule_s_primal_first(),
        rule_r_primal_first(),
        ::Arageli::ctrl::simplex_method::primal_col_iter_idler()
    );
}


/// Performs one primal column iteration on a column simplex table.
/** Just calls full vertion of the method with some default arguments. */
template
<
    typename T_Q,
    bool REFCNT_Q,
    typename T_nonbasis
    , bool REFCNT_nonbasis,
    typename Rule_s,
    typename Rule_r
>
inline result_kind primal_col_iter
(
    matrix<T_Q, REFCNT_Q>& Q,
    vector<T_nonbasis, REFCNT_nonbasis>& nonbasis,
    Rule_s rule_s,
    Rule_r rule_r
)
{
    return primal_col_iter
    (
        Q,
        nonbasis,
        rule_s,
        rule_r,
        ::Arageli::ctrl::simplex_method::primal_row_iter_idler()
    );
}

#endif


// ---------------------------------------------------------------------------

}

namespace ctrl
{
namespace simplex_method
{

/// Default controller for the primal_col_iters function.  It's doing nothing.
struct primal_col_iters_idler
{
    void preamble () const
    {}

    void conclusion () const
    {}

    primal_col_iter_idler iter_ctrl () const
    {
        return primal_col_iter_idler();
    }

    template <typename TQ, typename Tb>
    bool stop (const TQ&, const Tb&) const
    {
        return false;
    }
};


}}

namespace simplex_method
{


/// Performs numerous primal colunt iterations on a column simplex table.
/** Performs the iteration, determines if the table is optimal or
    if there are no an optimum and so on until table becomes optimal. */
template
<
    typename T_Q,
    bool REFCNT_Q,
    typename T_nonbasis,
    bool REFCNT_nonbasis,
    typename Rule_s,
    typename Rule_r,
    typename Ctrler
>
result_kind primal_col_iters
(
    matrix<T_Q, REFCNT_Q>& Q,
    vector<T_nonbasis, REFCNT_nonbasis>& nonbasis,
    Rule_s rule_s,
    Rule_r rule_r,
    Ctrler ctrler
)
{
    ARAGELI_ASSERT_0(is_col_allow(Q, nonbasis));
    ARAGELI_ASSERT_0(is_primal_allow(Q));
    typedef typename matrix<T_Q, REFCNT_Q>::size_type size_type;

    ctrler.preamble();
    result_kind res = rk_nonoptimal;

    while(res == rk_nonoptimal && !ctrler.stop(Q, nonbasis))
        res = primal_col_iter(Q, nonbasis, rule_s, rule_r, ctrler.iter_ctrl());

    ctrler.conclusion();
    return res;
}


/// Performs numerous primal column iterations on a column simplex table.
/** Just calls full vertion of the method with some default arguments. */
template
<
    typename T_Q,
    bool REFCNT_Q,
    typename T_nonbasis,
    bool REFCNT_nonbasis
>
inline result_kind primal_col_iters
(
    matrix<T_Q, REFCNT_Q>& Q,
    vector<T_nonbasis, REFCNT_nonbasis>& nonbasis
)
{
    return primal_col_iters
    (
        Q,
        nonbasis,
        rule_s_primal_first(),
        rule_r_primal_first(),
        ::Arageli::ctrl::simplex_method::primal_col_iters_idler()
    );
}

//@}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

///// @name The dual column iterations on a column simplex table.  With controllers.
////@{
//
//// WARNING!!! The following two classes are to remove!

}

namespace ctrl
{
namespace simplex_method
{

/// Default controller for the dual_col_iter function.  It's doing nothing.
struct dual_col_iter_idler
{
    void preamble () const
    {}

    void conclusion () const
    {}

    template <typename Q, typename Nonbasis>
    void current_table (const Q& q, const Nonbasis& nonbasis) const
    {}

    template <typename R, typename S>
    void pivot_item (const R& r, const S& s) const
    {}

    void found () const
    {}

    void infinite () const
    {}

    void nonoptimal () const
    {}

    void empty () const
    {}
};


/// Simple controller for the primal_col_iter function.  It outputs into a stream.
template <typename Stream>
struct dual_col_iter_slog : public dual_col_iter_idler
{
    Stream& stream;

    dual_col_iter_slog (Stream& stream_a) :
        stream(stream_a)
    {}

    void preamble () const
    {
        stream << "Find pivot item.\n";
    }

    void conclusion () const
    {}

    template <typename Q, typename Nonbasis>
    void current_table (const Q& q, const Nonbasis& nonbasis) const
    {
        output_aligned(stream << "Q =\n", q);
        stream << "nonbasis = " << nonbasis << ".\n";
    }

    template <typename R, typename S>
    void pivot_item (const R& r, const S& s) const
    {
        stream << "Pivot item: r = " << r << ", s = " << s << ".\n";
    }

    void found () const
    {
        stream << "The table is optimal.\n";
    }

    void infinite () const
    {
        stream << "The function isn't limited.\n";
    }

    void nonoptimal () const
    {}

    void empty () const
    {
        stream << "Set of allowable vectors is empty.";
    }
};

}}

namespace simplex_method
{

/// Performs one dual column iteration on a column simplex table.
/** Performs the iteration, determines if the table is optimal or
    if there are no an optimum. */
template
<
    typename T_Q,
    bool REFCNT_Q,
    typename T_nonbasis,
    bool REFCNT_nonbasis,
    typename Rule_s,
    typename Rule_r,
    typename Ctrler
>
result_kind dual_col_iter
(
    matrix<T_Q, REFCNT_Q>& Q,
    vector<T_nonbasis, REFCNT_nonbasis>& nonbasis,
    Rule_s rule_s,
    Rule_r rule_r,
    Ctrler ctrler
)
{
    ARAGELI_ASSERT_0(is_col_allow(Q, nonbasis));
    ARAGELI_ASSERT_0(is_dual_allow(Q));
    typedef typename matrix<T_Q, REFCNT_Q>::size_type size_type;

    ctrler.preamble();
    ctrler.current_table(Q, nonbasis);

    size_type r = rule_r(Q, nonbasis);
    if(r == Q.nrows())
    {
        ctrler.found();
        ctrler.conclusion();
        return rk_found;
    }

    size_type s = rule_s(Q, nonbasis, r);
    if(s == Q.ncols())
    {
        ctrler.empty();
        ctrler.conclusion();
        return rk_empty;
    }

    ctrler.pivot_item(r, s);

    gauss_field_col_iter(Q, r, s);
    for(std::size_t i = 0; i < Q.nrows(); ++i)
        opposite(&Q(i, s));

    nonbasis[s-1] = r;

    ctrler.nonoptimal();
    ctrler.conclusion();

    return rk_nonoptimal;
}


/// Performs one dual column iteration on a column simplex table.
/** Just calls full vertion of the method with some default arguments. */
template
<
    typename T_Q,
    bool REFCNT_Q,
    typename T_nonbasis,
    bool REFCNT_nonbasis
>
inline result_kind dual_col_iter
(
    matrix<T_Q, REFCNT_Q>& Q,
    vector<T_nonbasis, REFCNT_nonbasis>& nonbasis
)
{
    return dual_col_iter
    (
        Q,
        nonbasis,
        rule_s_dual_first(),
        rule_r_dual_first(),
        ::Arageli::ctrl::simplex_method::dual_col_iter_idler()
    );
}


/// Performs one dual column iteration on a column simplex table.
/** Just calls full vertion of the method with some default arguments. */
template
<
    typename T_Q,
    bool REFCNT_Q,
    typename T_nonbasis,
    bool REFCNT_nonbasis,
    typename Rule_s,
    typename Rule_r
>
inline result_kind dual_col_iter
(
    matrix<T_Q, REFCNT_Q>& Q,
    vector<T_nonbasis, REFCNT_nonbasis>& nonbasis,
    Rule_s rule_s,
    Rule_r rule_r
)
{
    return dual_col_iter
    (
        Q,
        nonbasis,
        rule_s,
        rule_r,
        ::Arageli::ctrl::simplex_method::dual_col_iter_idler()
    );
}

// ---------------------------------------------------------------------------

}

namespace ctrl
{
namespace simplex_method
{


/// Default controller for the dual_col_iters function.  It's doing nothing.
struct dual_col_iters_idler
{
    void preamble () const
    {}

    void conclusion () const
    {}

    dual_col_iter_idler iter_ctrl () const
    {
        return dual_col_iter_idler();
    }

    template <typename TQ, typename Tb>
    bool stop (const TQ&, const Tb&) const
    {
        return false;
    }
};


}}

namespace simplex_method
{


/// Performs numerous dual colunt iterations on a column simplex table.
/** Performs the iteration, determines if the table is optimal or
    if there are no an optimum and so on until table becomes optimal. */
template
<
    typename T_Q,
    bool REFCNT_Q,
    typename T_nonbasis,
    bool REFCNT_nonbasis,
    typename Rule_s,
    typename Rule_r,
    typename Ctrler
>
result_kind dual_col_iters
(
    matrix<T_Q, REFCNT_Q>& Q,
    vector<T_nonbasis, REFCNT_nonbasis>& nonbasis,
    Rule_s rule_s,
    Rule_r rule_r,
    Ctrler ctrler
)
{
    ARAGELI_ASSERT_0(is_col_allow(Q, nonbasis));
    ARAGELI_ASSERT_0(is_dual_allow(Q));
    typedef typename matrix<T_Q, REFCNT_Q>::size_type size_type;

    ctrler.preamble();
    result_kind res = rk_nonoptimal;

    while(res == rk_nonoptimal && !ctrler.stop(Q, nonbasis))
        res = dual_col_iter(Q, nonbasis, rule_s, rule_r, ctrler.iter_ctrl());

    ctrler.conclusion();
    return res;
}


/// Performs numerous dual colunt iterations on a column simplex table.
/** Rules of selection of pivot rows and columns are rule_s_dual_first
    and rule_r_dual_first. See complete version of this function. */
template
<
    typename T_q,
    bool REFCNT_q,
    typename T_nonbasis,
    bool REFCNT_nonbasis,
    typename Ctrler
>
inline result_kind dual_col_iters
(
    matrix<T_q, REFCNT_q>& q,
    vector<T_nonbasis, REFCNT_nonbasis>& nonbasis,
    Ctrler ctrler
)
{
    return dual_col_iters
    (
        q, nonbasis,
        rule_s_dual_first(),
        rule_r_dual_first(),
        ctrler
    );
}


/// Performs numerous dual column iterations on a column simplex table.
/** Just calls full vertion of the method with some default arguments. */
template
<
    typename T_q,
    bool REFCNT_q,
    typename T_nonbasis,
    bool REFCNT_nonbasis
>
inline result_kind dual_col_iters
(
    matrix<T_q, REFCNT_q>& q,
    vector<T_nonbasis, REFCNT_nonbasis>& nonbasis
)
{
    return dual_col_iters
    (
        q,
        nonbasis,
        rule_s_dual_first(),
        rule_r_dual_first(),
        ::Arageli::ctrl::simplex_method::dual_col_iters_idler()
    );
}

//@}
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/// @name Artificial basis method.
//@{

/// Creates artificial task to find valid basis by the artifical basis method.
/**    New variables (and columns) will have been placed to the beginning of the table. */
template <typename T1, bool REFCNT1, typename T2, bool REFCNT2>
void artificial_basis_create (matrix<T1, REFCNT1>& a, vector<T2, REFCNT2>& basis)
{
    ARAGELI_ASSERT_0(!a.is_empty());

    T1& pattern = a(0, 0);
    std::fill_n(a.begin(), a.ncols(), null(pattern));

    for(std::size_t i = 1; i < a.nrows(); ++i)
        a.sub_rows(0, i);

    // WARNING. Replace it to the special form of insert when it will have been written.
    a.insert_cols(1, a.nrows() - 1, null<T1>(pattern));

    basis.resize(a.nrows() - 1);

    for(std::size_t i = 1; i <= a.nrows() - 1; ++i)
    {
        a(i, i) = unit<T1>(pattern);
        basis[i-1] = i;
    }
}

} // namespace simlex_table

namespace ctrl
{
namespace simplex_method
{

//using namespace Arageli::simplex_method;


/// Default controller for the basis_create_by_artificial function.  It's doing nothing.
struct basis_create_by_artificial_idler
{
    class abort :
        public ctrl::abort
    {};

    template <typename Q, typename Basis>
    void preamble (const Q& q, const Basis& basis) const
    {}

    template <typename Q, typename Basis>
    void conclusion (const Q& q, const Basis& basis) const
    {}

    template <typename Q, typename Basis, typename Index>
    void artif_in_basis
    (const Q& q, const Basis& basis, const Index& index) const
    {}

    template <typename I>
    void negligible_row (const I& i) const
    {}

    template <typename Q, typename Basis, typename Index>
    void replace_basis_item
    (
        const Q& q, const Basis& basis,
        const Index& iold, const Index& r, const Index& inew
    ) const
    {}

    template <typename Q, typename Basis>
    void before_erase_artif (const Q& q, const Basis& basis) const
    {}
};


}}

namespace simplex_method
{

/// Produces a valid basis from artifical optimal simplex table due the artifical basis method.
/**    The artifical variables must be located in the beginning of the table. */
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Ctrler
>
void basis_create_by_artificial
(
    matrix<T1, REFCNT1>& q,
    vector<T2, REFCNT2>& basis,
    Ctrler ctrler
)
{
    ARAGELI_ASSERT_0(!q.is_empty());
    ARAGELI_ASSERT_0(is_null(q(0, 0)));
    ARAGELI_ASSERT_0(is_row_allow(q, basis));
    ARAGELI_ASSERT_0(is_primal_allow(q));
    ARAGELI_ASSERT_0(is_dual_allow(q));

    ctrler.preamble(q, basis);

    std::size_t m = q.nrows();

    for(std::size_t i = 0; i < basis.size(); ++i)
    {
        ARAGELI_ASSERT_0(is_positive(basis[i]));

        if(basis[i] < m)
        {
            std::size_t r = i+1;
            ctrler.artif_in_basis(q, basis, i);

            ARAGELI_ASSERT_0(is_null(q(r, 0)));
            std::size_t j;
            for(j = m; j < q.ncols(); ++j)
                if(!is_null(q(r, j)))break;

            if(j == q.ncols())
            {
                // we found negligible row
                ctrler.negligible_row(r);
                q.erase_row(r);
                basis.erase(i);
            }
            else
            {
                ctrler.replace_basis_item(q, basis, i, r, j);
                gauss_field_row_iter(q, r, j);
                basis[i] = j;
            }
        }
    }

    ctrler.before_erase_artif(q, basis);

    q.erase_cols(1, m-1);
    basis -= m-1;
    ctrler.conclusion(q, basis);
}


/// Produces a valid basis from artifical optimal simplex table due the artifical basis method.
/** Just calls full vertion of the method with some default arguments. */
template <typename T1, bool REFCNT1, typename T2, bool REFCNT2>
inline void basis_create_by_artificial
(
    matrix<T1, REFCNT1>& a,
    vector<T2, REFCNT2>& basis
)
{
    return
        basis_create_by_artificial
        (
            a,
            basis,
            ::Arageli::ctrl::simplex_method::basis_create_by_artificial_idler()
        );
}

} // namespace simplex_method

namespace ctrl
{
namespace simplex_method
{
using namespace Arageli::simplex_method;

/// Default controller for the primal_row_iters function.  It's doing nothing.
struct basis_artificial_idler
{
    class abort :
        public ctrl::abort
    {};

    template <typename Q>
    void preamble (const Q& q) const
    {}

    template <typename Q, typename Basis>
    void conclusion (const Q& q, const Basis& basis, result_kind rk) const
    {}

    primal_row_iters_idler ctrl_primal_row_iters () const
    {
        return primal_row_iters_idler();
    }

    basis_create_by_artificial_idler ctrl_basis_create_by_artificial () const
    {
        return basis_create_by_artificial_idler();
    }

    template <typename Q, typename Basis>
    void after_artif (const Q& q, const Basis& basis) const
    {}

    template <typename Q, typename Basis>
    void after_iters (const Q& q, const Basis& basis) const
    {}

    template <typename Q, typename Basis>
    void before_orig (const Q& q, const Basis& basis) const
    {}

    template <typename Q, typename Basis>
    void after_orig (const Q& q, const Basis& basis) const
    {}
};


}
}

namespace simplex_method
{


/// Builds primal allowable basis from simplex table q by artificial basis method.
/** After calling this method the old values of the first row of q is lost. */
template
<
    typename T1,
    bool REFCNT1,
    typename T3,
    bool REFCNT3,
    typename Rule_s,
    typename Rule_r,
    typename Ctrler
>
result_kind basis_artificial
(
    matrix<T1, REFCNT1>& q,
    vector<T3, REFCNT3>& basis,
    Rule_s rule_s,
    Rule_r rule_r,
    Ctrler ctrler
)
{
    ARAGELI_ASSERT_0(is_primal_allow(q));

    ctrler.preamble(q);
    artificial_basis_create(q, basis);
    ctrler.after_artif(q, basis);

    result_kind rk =
        primal_row_iters
        (
            q,
            basis,
            rule_s,
            rule_r,
            ctrler.ctrl_primal_row_iters()
        );

    ARAGELI_ASSERT_1(rk == rk_found);

    ctrler.after_iters(q, basis);

    if(!is_null(q(0, 0)))
        rk = rk_empty;
    else
    {
        basis_create_by_artificial
        (
            q,
            basis,
            ctrler.ctrl_basis_create_by_artificial()
        );

        ctrler.after_orig(q, basis);
        rk = rk_found;
    }

    ARAGELI_ASSERT_1(is_primal_allow(q));

    ctrler.conclusion(q, basis, rk);
    return rk;
}


/// Builds primal allowable basis from simplex table q by artificial basis method.
/** Rule of selection of row and column are by the default. */
template
<
    typename T1,
    bool REFCNT1,
    typename T3,
    bool REFCNT3,
    typename Ctrler
>
inline result_kind basis_artificial
(
    matrix<T1, REFCNT1>& q,
    vector<T3, REFCNT3>& basis,
    Ctrler ctrler
)
{
    return basis_artificial
    (
        q,
        basis,
        rule_s_primal_first(),
        rule_r_primal_lex(),
        ctrler
    );
}


/// Builds primal allowable basis from simplex table q by artificial basis method.
/** With default idle conroller. See complete version of this function. */
template
<
    typename T1,
    bool REFCNT1,
    typename T3,
    bool REFCNT3
>
inline result_kind basis_artificial
(
    matrix<T1, REFCNT1>& q,
    vector<T3, REFCNT3>& basis
)
{
    return basis_artificial
    (
        q,
        basis,
        rule_s_primal_first(),
        rule_r_primal_lex(),
        ::Arageli::ctrl::simplex_method::basis_artificial_idler()
    );
}


//@}
// ***************************************************************************

// @name Direct row simplex method with artificial basis method as a zeroth step.
//@{

}

namespace ctrl
{
namespace simplex_method
{

/// Default controller for the primal_row_with_artificial_basis function.  It's doing nothing.
struct primal_row_with_artificial_basis_idler
{
    void preamble () const
    {}

    template <typename A, typename B, typename C>
    void input_data (const A& a, const B& b, const C& c) const
    {}

    template <typename T>
    void table_for_artificial (const T& table) const
    {}

    template <typename T, typename B>
    void artificial_table (const T& table, const B& basis) const
    {}

    primal_row_iters_idler ctrl_for_artificial () const
    {
        return primal_row_iters_idler();
    }

    template <typename T, typename B>
    void optimal_artificial_table (const T& table, const B& basis) const
    {}

    basis_create_by_artificial_idler
    ctrl_for_basis_create_by_artificial () const
    {
        return basis_create_by_artificial_idler();
    }

    template <typename T>
    void first_table (const T& table) const
    {}

    template <typename T>
    void pre_first_table (const T& table) const
    {}

    primal_row_iters_idler ctrl_for_main_row_iters () const
    {
        return primal_row_iters_idler();
    }

    void result (result_kind rk) const
    {}

    void conclusion () const
    {}
};

}}

namespace simplex_method
{


/// Creating of a row table from matrix A and vector b, with zeros in the first row.
/** The result will have been placed into q. */
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename T3,
    bool REFCNT3
>
void row_table_create
(
    const matrix<T1, REFCNT1>& a,
    const vector<T2, REFCNT2>& b,
    matrix<T3, REFCNT3>& q
)
{
    ARAGELI_ASSERT_0(b.size() == a.nrows());
    q.assign_fromsize(a.nrows() + 1, a.ncols() + 1);

    for(std::size_t i = 1; i < q.nrows(); ++i)
    {
        q(i, 0) = b[i-1];
        for(std::size_t j = 1; j < q.ncols(); ++j)
            q(i, j) = a(i-1, j-1);
    }
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
void row_table_place_c
(
    const vector<T1, REFCNT1>& c,
    matrix<T2, REFCNT2>& q
)
{
    ARAGELI_ASSERT_0(c.size() == q.ncols() - 1);

    for(std::size_t j = 1; j < q.ncols(); ++j)
        q(0, j) = -c[j-1];
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
void row_table_extract_c
(
    vector<T1, REFCNT1>& c,
    const matrix<T2, REFCNT2>& q
)
{
    c.resize(q.ncols() - 1);

    for(std::size_t j = 1; j < q.ncols(); ++j)
        c[j-1] = -q(0, j);
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
void row_table_pivot_basis_c
(
    matrix<T1, REFCNT1>& q,
    const vector<T2, REFCNT2>& basis
)
{
    for(std::size_t i = 0; i < basis.size(); ++i)
        q.addmult_rows(0, i+1, -q(0, basis[i]));
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename T3,
    bool REFCNT3
>
void row_table_extract_solution
(
    const matrix<T1, REFCNT1>& q,
    const vector<T2, REFCNT2>& basis,
    vector<T3, REFCNT3>& x
)
{
    x.assign(q.ncols()-1, null(q(0, 0)));
    for(std::size_t i = 0; i < basis.size(); ++i)
        x[basis[i]-1] = q(i+1, 0);
}


template
<
    typename T1,
    bool REFCNT1,
    typename N,
    typename T2,
    bool REFCNT2
>
void col_table_extract_solution
(
    const matrix<T1, REFCNT1>& t,
    const N& n,    ///< number of meaningful variables
    vector<T2, REFCNT2>& x
)
{
    x.resize(n);
    for(std::size_t i = 0; i < n; ++i)
        x[i] = t(i+1, 0);
}


/// Creating of a row table from matrix A and vectors b, c.
/** The result will have been placed into q. */
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename T3,
    bool REFCNT3,
    typename T4,
    bool REFCNT4
>
void row_table_create
(
    const matrix<T1, REFCNT1>& a,
    const vector<T2, REFCNT2>& b,
    const vector<T3, REFCNT3>& c,
    matrix<T4, REFCNT4>& q
)
{
    ARAGELI_ASSERT_0(c.size() == a.ncols());
    row_table_create(a, b, q);
    row_table_place_c(c, q);
}


/// Creating of matrix A and vectors b, c from a row table q.
/** The result will have been placed into a, b, c. */
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename T4,
    bool REFCNT4
>
void row_table_split
(
    matrix<T1, REFCNT1>& a,
    vector<T2, REFCNT2>& b,
    const matrix<T4, REFCNT4>& q
)
{
    ARAGELI_ASSERT_0(!q.is_empty());

    a.assign_fromsize(q.nrows() - 1, q.ncols() - 1);
    b.resize(a.nrows());

    for(std::size_t i = 1; i < q.nrows(); ++i)
    {
        b[i-1] = q(i, 0);
        for(std::size_t j = 1; j < q.ncols(); ++j)
            a(i-1, j-1) = q(i, j);
    }
}


/// Creating of matrix A and vectors b, c from a row table q.
/** The result will have been placed into a, b, c. */
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename T3,
    bool REFCNT3,
    typename T4,
    bool REFCNT4
>
void row_table_split
(
    matrix<T1, REFCNT1>& a,
    vector<T2, REFCNT2>& b,
    const vector<T3, REFCNT3>& c,
    const matrix<T4, REFCNT4>& q
)
{
    ARAGELI_ASSERT_0(!q.is_empty());

    row_table_split(a, b, q);
    c.resize(a.ncols());

}


/// Makes primal column table by given task in standard shape.
/** Additional variables will have been placed to the end of the list
    of main variables.  basis will have been filled by numbers additional variables. */
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename T3,
    bool REFCNT3,
    typename T4,
    bool REFCNT4,
    typename T5,
    bool REFCNT5
>
void col_table_create_by_standard
(
    const matrix<T1, REFCNT1>& a,
    const vector<T2, REFCNT2>& b,
    const vector<T3, REFCNT3>& c,
    matrix<T4, REFCNT4>& t,
    vector<T5, REFCNT5>& nonbasis
)
{
    t.assign_fromsize(a.ncols() + a.nrows() + 1, a.ncols() + 1);
    typedef typename matrix<T1, REFCNT1>::size_type size_type;

    t(0, 0) = null(a(0, 0));
    const T4& tnull = t(0, 0);

    nonbasis.resize(a.ncols());
    for(size_type i = 1; i <= a.ncols(); ++i)
    {
        t(i, 0) = tnull;
        nonbasis[i-1] = i;
    }

    for(size_type i = a.ncols() + 1; i < t.nrows(); ++i)
        t(i, 0) = b[i - a.ncols() - 1];

    for(size_type j = 0; j < a.ncols(); ++j)
    {
        t(0, j+1) = -c[j];
        t(j+1, j+1) = opposite_unit(tnull);

        for(size_type i = a.ncols() + 1; i < t.nrows(); ++i)
            t(i, j+1) = a(i - a.ncols() - 1, j);
    }
}


/// Solves the linear programming problem with artificial basis method as zeroth step.
/** All entries in b should be nonnegative. */
template
<
    typename Ta,
    bool REFCNTa,
    typename Tb,
    bool REFCNTb,
    typename Tc,
    bool REFCNTc,
    typename Tx,
    bool REFCNTx,
    typename Tbasis,
    bool REFCNTbasis,
    typename Tres,
    typename Rule_s,
    typename Rule_r,
    typename Ctrler
>
result_kind primal_row_with_artificial_basis
(
    const matrix<Ta, REFCNTa>& a,
    const vector<Tb, REFCNTb>& b,
    const vector<Tc, REFCNTc>& c,
    vector<Tx, REFCNTx>& basis_x,
    vector<Tbasis, REFCNTbasis>& basis,
    Tres& res,
    Rule_s rule_s,
    Rule_r rule_r,
    Ctrler ctrler
)
{
    ARAGELI_ASSERT_0(a.nrows() == b.size());
    ARAGELI_ASSERT_0(a.ncols() == c.size());
    ARAGELI_ASSERT_0(!a.is_empty());

    ctrler.preamble();
    ctrler.input_data(a, b, c);

    // building the first simplex table

    matrix<Ta, false> table;
    row_table_create(a, b, table);

    ctrler.table_for_artificial(table);

    // the artificial basis method

    artificial_basis_create(table, basis);

    ctrler.artificial_table(table, basis);

    result_kind rk =
        primal_row_iters
        (
            table,
            basis,
            rule_s,
            rule_r,
            ctrler.ctrl_for_artificial()
        );

    ctrler.optimal_artificial_table(table, basis);

    ARAGELI_ASSERT_1(rk == rk_found);

    if(is_negative(table(0, 0)))
    {
        ctrler.result(rk_empty);
        ctrler.conclusion();
        return rk_empty;
    }

    ARAGELI_ASSERT_1(is_null(table(0, 0)));

    // building the first allowable simplex table

    basis_create_by_artificial
    (
        table,
        basis,
        ctrler.ctrl_for_basis_create_by_artificial()
    );

    //ctrler.first_table(table);

    row_table_place_c(c, table);
    ctrler.pre_first_table(table);
    row_table_pivot_basis_c(table, basis);

    rk =
        primal_row_iters
        (
            table,
            basis,
            rule_s,
            rule_r,
            ctrler.ctrl_for_main_row_iters()
        );

    if(rk == rk_found)
    {
        basis_x.resize(basis.size());
        for(std::size_t i = 0; i < basis.size(); ++i)
            basis_x[i] = table(i+1, 0);
        res = table(0, 0);

        //output_aligned(std::cout << "\n\nrk_found:\n", table);

    }

    ctrler.result(rk);
    ctrler.conclusion();
    return rk;
}


/// Solves the linear programming problem with artificial basis method as zeroth step.
/** Just calls full vertion of the method with some default arguments. */
template
<
    typename Ta,
    bool REFCNTa,
    typename Tb,
    bool REFCNTb,
    typename Tc,
    bool REFCNTc,
    typename Tx,
    bool REFCNTx,
    typename Tbasis,
    bool REFCNTbasis,
    typename Tres
>
inline result_kind primal_row_with_artificial_basis
(
    const matrix<Ta, REFCNTa>& a,
    const vector<Tb, REFCNTb>& b,
    const vector<Tc, REFCNTc>& c,
    vector<Tx, REFCNTx>& basis_x,
    vector<Tbasis, REFCNTbasis>& basis,
    Tres& res
)
{
    return primal_row_with_artificial_basis
    (
        a,
        b,
        c,
        basis_x,
        basis,
        res,
        rule_s_primal_first(),
        rule_r_primal_lex(),
        ::Arageli::ctrl::simplex_method::primal_row_with_artificial_basis_idler()
    );
}

//@}

// ***************************************************************************

/// Makes nonbasis by basis.
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
void basis_to_nonbasis
(
    const vector<T1, REFCNT1>& basis,
    vector<T2, REFCNT2>& nonbasis,
    std::size_t first_number,    ///< number of first variable
    std::size_t n    ///< number of variables
)
{
    std::size_t m = basis.size();

    vector<bool, false> basis_marks(n, false);
    for(std::size_t i = 0; i < m; ++i)
        basis_marks[basis[i]-first_number] = true;

    nonbasis.resize(n - m);
    std::size_t ib = 0;
    for(std::size_t i = 0; i < n; ++i)
        if(!basis_marks[i])nonbasis[ib++] = i + first_number;

    ARAGELI_ASSERT_1(ib == nonbasis.size());
}


/// Makes nonbasis by basis.
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline void basis_to_nonbasis
(
    const vector<T1, REFCNT1>& basis,
    vector<T2, REFCNT2>& nonbasis,
    std::size_t n    ///< number of variables
)
{
    basis_to_nonbasis(basis, nonbasis, std::size_t(1), n);
}



/// Converts a valid simplex table from a row form to a column form.
template
<
    typename T_Q,
    bool REFCNT_Q,
    typename T_basis,
    bool REFCNT_basis,
    typename T_T,
    bool REFCNT_T,
    typename T_nonbasis,
    bool REFCNT_nonbasis
>
void row_to_col_table
(
    const matrix<T_Q, REFCNT_Q>& q,
    const vector<T_basis, REFCNT_basis>& basis,
    matrix<T_T, REFCNT_T>& t,
    vector<T_nonbasis, REFCNT_nonbasis>& nonbasis
)
{
    ARAGELI_ASSERT_0(!q.is_empty());

    std::size_t m = q.nrows() - 1;
    std::size_t n = q.ncols() - 1;

    ARAGELI_ASSERT_0(basis.size() == m);
    ARAGELI_ASSERT_0(n >= m);
    ARAGELI_ASSERT_0(m == basis.size());

    t.assign_fromsize(q.ncols(), n - m + 1);
    ARAGELI_ASSERT_1(!t.is_empty());

    // nonbasis building
    basis_to_nonbasis(basis, nonbasis, n);

    // target function value moving
    t(0, 0) = q(0, 0);

    // basis rows filling

    for(std::size_t i = 0; i < m; ++i)
    {
        std::size_t r = basis[i];
        ARAGELI_ASSERT_0(r >= 1 && r <= n);
        t(r, 0) = q(i+1, 0);

        for(std::size_t j = 1; j < t.ncols(); ++j)
            t(r, j) = q(i+1, nonbasis[j-1]);
    }

    // zeroth row filling and nonbasis rows filling (minus ones placing)

    for(std::size_t j = 0; j < nonbasis.size(); ++j)
    {
        t(0, j+1) = q(0, nonbasis[j]);
        t(nonbasis[j], j+1) = opposite_unit(t(0, 0));
    }
}


/// Converts a valid simplex table from a row form to a column form.
/** Just calls full vertion of the method with some default arguments. */
template
<
    typename T_QT,
    bool REFCNT_QT,
    typename T_basis_nonbasis,
    bool REFCNT_basis_nonbasis
>
void row_to_col_table
(
    matrix<T_QT, REFCNT_QT>& qt,
    vector<T_basis_nonbasis, REFCNT_basis_nonbasis>& basis_nonbasis
)
{
    matrix<T_QT, REFCNT_QT> t;
    vector<T_basis_nonbasis, REFCNT_basis_nonbasis> nonbasis;

    row_to_col_table(qt, basis_nonbasis, t, nonbasis);

    swap(qt, t);
    swap(basis_nonbasis, nonbasis);
}


/// Creates a dual row simplex table by primal row simplex table with selected basis.
/** WARNING! IMPLEMENTATION MAYBE INCORRECT!
    A new simplex table negates vector of a goal function, thus you should
    negate the value of goal function if you want to have correct result.
    During building a new task formulation, this function introduces a set of
    new variables to prepare the task in the canonical shape.  Those variables will
    have been placed into the end of new table. */
template
<
    typename T_Q,
    bool REFCNT_Q,
    typename T_basis,
    bool REFCNT_basis,
    typename T_DQ,
    bool REFCNT_DQ,
    typename T_Dbasis,
    bool REFCNT_Dbasis
>
void primal_row_to_dual_row_table
(
    const matrix<T_Q, REFCNT_Q>& q,
    const vector<T_basis, REFCNT_basis>& basis,
    matrix<T_DQ, REFCNT_DQ>& dq,
    vector<T_Dbasis, REFCNT_Dbasis>& dbasis
)
{
    ARAGELI_ASSERT_0(!q.is_empty());
    ARAGELI_ASSERT_0(!basis.is_empty());
    ARAGELI_ASSERT_0(q.nrows() - 1 == basis.size());
    ARAGELI_ASSERT_0(q.nrows() <= q.ncols());

    dq.assign_fromsize
    (
        q.ncols() - basis.size(),
        q.nrows() + q.ncols() - basis.size() - 1
    );

    dq(0, 0) = -q(0, 0);

    for(std::size_t j = 1; j < q.nrows(); ++j)
        dq(0, j) = q(j, 0);

    vector<std::size_t, false> nonbasis;
    basis_to_nonbasis(basis, nonbasis, q.ncols() - 1);
    ARAGELI_ASSERT_1(nonbasis.size() == dq.nrows() - 1);

    const T_Q& pattern = q(0, 0);
    dbasis.resize(nonbasis.size());

    for(std::size_t i = 1; i <= nonbasis.size(); ++i)
    {
        std::size_t k = nonbasis[i-1];
        dq(i, 0) = q(0, k);
        for(std::size_t j = 1; j < q.nrows(); ++j)
            dq(i, j) = -q(j, k);

        dq(i, q.nrows() + i - 1) = unit(pattern);
        dbasis[i-1] = q.nrows() + i - 1;
    }
}

// ***************************************************************************
/// @name The Gomory's algorithms.
//@{

/// Creates Gomory's clip on optimal column simplex table.
template <typename T, bool REFCNT, typename Prow>
result_kind gomory1_clip (matrix<T, REFCNT>& t, Prow& prow)
{
    ARAGELI_ASSERT_0(!t.is_empty());
    ARAGELI_ASSERT_0(is_primal_allow(t));
    ARAGELI_ASSERT_0(is_dual_allow(t));

    typedef typename matrix<T, REFCNT>::size_type size_type;

    size_type r = 0;
    while(r < t.nrows() && is_integer(t(r, 0)))
        ++r;

    if(r == t.nrows())
        return rk_found;
    prow = r;

    t.insert_row(t.nrows(), null(t(0, 0)));
    for(size_type j = 0; j < t.ncols(); ++j)
        t(t.nrows() - 1, j) = -frac(t(r, j));

    ARAGELI_ASSERT_1(is_negative(t(t.nrows() - 1, 0)));
    ARAGELI_ASSERT_1(!is_integer(t(t.nrows() - 1, 0)));

    return rk_nonoptimal;
}

/// Creates Gomory's clip on optimal column simplex table.
/** Just calls the full version of the function with some default arguments. */
template <typename T, bool REFCNT>
result_kind gomory1_clip (matrix<T, REFCNT>& t)
{
    typename matrix<T, REFCNT>::size_type prow;
    return gomory1_clip(t, prow);
}

} // namespace simplex_method

namespace ctrl
{
namespace simplex_method
{

using namespace Arageli::simplex_method;

/// Default controller for the primal_row_iters function.  It's doing nothing.
struct gomory1_iter_idler
{
    class abort :
        public ctrl::abort
    {};

    template <typename T, typename Nonbasis>
    void preamble (const T& t, const Nonbasis& nonbasis) const
    {}

    template <typename T, typename Nonbasis>
    void conclusion (const T& t, const Nonbasis& nonbasis, result_kind rk) const
    {}

    template <typename T, typename Prow>
    void after_gomory1_clip (const T& t, Prow prow, result_kind rk) const
    {}

    dual_col_iters_idler ctrl_for_dual_col_iters () const
    {
        return dual_col_iters_idler();
    }
};


/// Default controller for the primal_row_iters function.  It's doing nothing.
struct gomory1_iters_idler
{
    class abort :
        public ctrl::abort
    {};

    template <typename T, typename Nonbasis>
    void preamble (const T& t, const Nonbasis& nonbasis) const
    {}

    template <typename T, typename Nonbasis>
    void conclusion (const T& t, const Nonbasis& nonbasis, result_kind rk) const
    {}

    template <typename T, typename Nonbasis>
    void before_iter (const T& t, const Nonbasis& nonbasis) const
    {}

    template <typename T, typename Nonbasis>
    void after_iter
    (
        const T& t,
        const Nonbasis& nonbasis,
        result_kind rk
    ) const
    {}

    gomory1_iter_idler ctrl_for_gomory1_iter () const
    {
        return gomory1_iter_idler();
    }
};

}}


namespace simplex_method
{

/// One iteration of The First Gomory's Algorithm on an optimal column simplex table.
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Ctrler
>
result_kind gomory1_iter
(
    matrix<T1, REFCNT1>& t,
    vector<T2, REFCNT2>& nonbasis,
    Ctrler ctrler
)
{
    ARAGELI_ASSERT_0(!t.is_empty());
    ARAGELI_ASSERT_0(is_primal_allow(t));
    ARAGELI_ASSERT_0(is_dual_allow(t));

    ctrler.preamble(t, nonbasis);
    typename matrix<T1, REFCNT1>::size_type prow;
    result_kind res = gomory1_clip(t, prow);
    ctrler.after_gomory1_clip(t, prow, res);

    if(res == rk_nonoptimal)
    {
        res = dual_col_iters(t, nonbasis, ctrler.ctrl_for_dual_col_iters());
        if(res == rk_found)
            res = rk_nonoptimal;
    }

    ctrler.conclusion(t, nonbasis, res);
    return res;
}


/// Runs the First Gomory's Algorithm on an optimal column simplex table up to the end.
/** Finds of the optimal table with integer solution or proof absence of integer
    points in search area. Be carefull: already optimized column simple table should
    be passed as input. */
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Ctrler
>
result_kind gomory1_iters
(
    matrix<T1, REFCNT1>& t,
    vector<T2, REFCNT2>& nonbasis,
    Ctrler ctrler
)
{
    ARAGELI_ASSERT_0(!t.is_empty());
    ARAGELI_ASSERT_0(is_primal_allow(t));
    ARAGELI_ASSERT_0(is_dual_allow(t));

    ctrler.preamble(t, nonbasis);
    result_kind res = rk_nonoptimal;

    while(res == rk_nonoptimal)
    {
        ctrler.before_iter(t, nonbasis);
        res = gomory1_iter(t, nonbasis, ctrler.ctrl_for_gomory1_iter());
        ctrler.after_iter(t, nonbasis, res);
    }

    ctrler.conclusion(t, nonbasis, res);
    return res;
}


/// Runs the First Gomory's Algorithm on an optimal column simplex table up to the end.
/** See full version of the function. */
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline result_kind gomory1_iters
(
    matrix<T1, REFCNT1>& t,
    vector<T2, REFCNT2>& nonbasis
)
{
    return
        gomory1_iters
        (
            t,
            nonbasis,
            ::Arageli::ctrl::simplex_method::gomory1_iters_idler()
        );
}


//@}

// ***************************************************************************

/// Makes the dual task in canonical shape by primal task.
/** Introcudes two additional set of variables: to limitation by sign,
    and to transforming from >= to =. If the primal task needs to maximize
    the resulting task needs to maximize too. */
template
<
    typename T_a,
    bool REFCNT_a,
    typename T_b,
    bool REFCNT_b,
    typename T_c,
    bool REFCNT_c,
    typename T_da,
    bool REFCNT_da,
    typename T_db,
    bool REFCNT_db,
    typename T_dc,
    bool REFCNT_dc,
    typename T_basis,
    bool REFCNT_basis
>
void primal_to_dual_canonical
(
    const matrix<T_a, REFCNT_a>& a,
    const vector<T_b, REFCNT_b>& b,
    const vector<T_c, REFCNT_c>& c,
    matrix<T_da, REFCNT_da>& da,
    vector<T_db, REFCNT_db>& db,
    vector<T_dc, REFCNT_dc>& dc,
    vector<T_basis, REFCNT_basis>& basis
)
{
    ARAGELI_ASSERT_0(!a.is_empty());
    ARAGELI_ASSERT_0(a.ncols() == c.size());
    ARAGELI_ASSERT_0(a.nrows() == b.size());

    db = -c;

    da.assign_fromsize(a.ncols(), 2*a.nrows() + a.ncols());
    dc.resize(2*b.size() + a.ncols());
    basis.resize(da.nrows());

    for(std::size_t j = 0; j < a.ncols(); ++j)
    {
        da(j, 2*a.nrows() + j) = unit(a(0, 0));
        basis[j] = 2*a.nrows() + j + 1;

        for(std::size_t i = 0; i < a.nrows(); ++i)
        {
            da(j, 2*i) = -a(i, j);
            da(j, 2*i + 1) = a(i, j);

            dc[2*i] = -b[i];
            dc[2*i + 1] = b[i];
        }
    }
}


template <typename XE, typename N, typename X>
void extract_solution_from_extended_canonical
(const XE& xe, const N& n, X& x)
{
    x.resize(n);
    for(typename XE::size_type i = 0; i < n; ++i)
    {
        if(!is_null(xe[2*i]))x[i] = xe[2*i];
        else x[i] = -xe[2*i + 1];
    }
}


/// Makes the dual task in standard shape by primal task via introducing discrepancy variables.
/** The final dual task is formulated in the discrepancy variables set
    y = u*bsuba + bsubc, where bsuba is basis submatrix in a,
    bsubc is subvector of c that corresponds to bsuba.
    Native dual variables are found by u = (y+bsubc)*bsuba^(-1).
    Note the result task is formulated as "-max" task, so you should negate
    the result function value for THIS task to produce the result of original
    dual task.  You must add value of res_offset to "-max" task result function value. */
template
<
    typename T_a,
    bool REFCNT_a,
    typename T_b,
    bool REFCNT_b,
    typename T_c,
    bool REFCNT_c,
    typename T_basis,
    bool REFCNT_basis,
    typename T_da,
    bool REFCNT_da,
    typename T_db,
    bool REFCNT_db,
    typename T_dc,
    bool REFCNT_dc,
    typename T_res,
    typename T_ab,
    bool REFCNT_ab,
    typename T_cb,
    bool REFCNT_cb
>
void primal_to_dual_standard_discr
(
    const matrix<T_a, REFCNT_a>& a,
    const vector<T_b, REFCNT_b>& b,
    const vector<T_c, REFCNT_c>& c,
    const vector<T_basis, REFCNT_basis> basis,
    matrix<T_da, REFCNT_da>& da,
    vector<T_db, REFCNT_db>& db,
    vector<T_dc, REFCNT_dc>& dc,
    T_res& res_offset,
    matrix<T_ab, REFCNT_ab>& bsuba,    ///< basis submatrix for a
    vector<T_cb, REFCNT_cb>& bsubc    ///< basis subvector for c
)
{
    ARAGELI_ASSERT_0(!a.is_empty());
    ARAGELI_ASSERT_0(a.ncols() == c.size());
    ARAGELI_ASSERT_0(a.nrows() == b.size());
    ARAGELI_ASSERT_0(!basis.is_empty());

    vector<T_basis, false> basism1 = basis - unit(basis[0]);
    vector<T_basis, false> nonbasism1;
    basis_to_nonbasis(basis, nonbasism1, c.size());
    nonbasism1 -= unit(basis[0]);
    //std::sort(basism1);
    //std::sort(nonbasism1);

    fill_submatrix_col(a, basism1, bsuba);
    fill_subvector(c, basism1, bsubc);    // c_B
    matrix<T_ab, false> inv_bsuba = inverse(bsuba);    // B^(-1) matrix
    //output_aligned(std::cout << "\n$$$$$$B^(-1) matrix\n", inv_bsuba);

    matrix<T_ab, false> nonbsuba;    // N matrix
    fill_submatrix_col(a, nonbasism1, nonbsuba);

    matrix<T_ab, false> bm1n = inv_bsuba*nonbsuba;    // B^(-1) * N
    //output_aligned(std::cout << "\n$$$$$$B^(-1) * N\n", bm1n);

    vector<T_cb, false> nonbsubc;    // c_N
    fill_subvector(c, nonbasism1, nonbsubc);

    da = -bm1n;
    da.transpose();
    db = (bsubc*bm1n - nonbsubc);

    //std::cout
    //    << "\n$$$$$$bsubc:\n" << bsubc
    //    << "\n$$$$$$bsubc*bm1n:\n" << bsubc*bm1n
    //    << "\n$$$$$$nonbsubc:\n" << nonbsubc
    //    << "\n$$$$$$db:\n" << db;

    dc = -inv_bsuba*b;
    res_offset = -dotprod(bsubc*inv_bsuba, b);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

struct linear_prog_task_base
{
    enum constraint_type
    {
        ct_equal = 0x01,
        ct_greater_equal = 0x02,
        ct_less_equal = 0x04,
        ct_integer = 0x08
    };

    enum optimum_type
    {
        ot_min,
        ot_max
    };
};


/// Model of the linear programming task with partial integer constraints.
/** c*x -> min/max, A*x RELATION b, where RELATION is vector
    relations (for each row) */
template <typename T, bool REFCNT = true>
class linear_prog_task : public linear_prog_task_base
{
public:

    typedef vector<T, REFCNT> c_type;
    typedef matrix<T, REFCNT> a_type;
    typedef vector<T, REFCNT> b_type;
    typedef vector<constraint_type, REFCNT> relation_type;
    typedef vector<constraint_type, REFCNT> individual_type;

    linear_prog_task ()
    {}

    linear_prog_task
    (
        const c_type& c_a,
        const a_type& a_a,
        const b_type& b_a,
        const relation_type& relation_a,
        const individual_type& individual_a,
        optimum_type optimum_a = ot_max
    ) :
        c_m(c_a),
        a_m(a_a),
        b_m(b_a),
        relation_m(relation_a),
        individual_m(individual_a),
        optimum_m(optimum_a)
    {}


    // Creates a canonical task.
    linear_prog_task
    (
        const c_type& c_a,
        const a_type& a_a,
        const b_type& b_a,
        optimum_type optimum_a = ot_max
    ) :
        c_m(c_a),
        a_m(a_a),
        b_m(b_a),
        relation_m(b_m.size(), ct_equal),
        //individual_m(individual_a, ct_greater_equal),        // WARNING! This line isn't compilable.
        optimum_m(optimum_a)
    {}


    optimum_type optimum () const
    {
        return optimum_m;
    }

    const c_type& c () const
    {
        return c_m;
    }

    const a_type& a () const
    {
        return a_m;
    }

    const b_type& b () const
    {
        return b_m;
    }

    const relation_type& relation () const
    {
        return relation_m;
    }

    const individual_type& individual () const
    {
        return individual_m;
    }

    bool is_canonical () const;
    bool is_standard () const;
    bool is_integer () const;
    bool is_partial_integer () const;

    bool is_normal () const;

    void change_optimal (optimum_type);

    void canonical ()
    {}

    void standard ();

    void dual ();

private:

    c_type c_m;
    a_type a_m;
    b_type b_m;
    relation_type relation_m;
    individual_type individual_m;
    optimum_type optimum_m;

};


template <typename T, bool REFCNT, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& output_list
(
    std::basic_ostream<Ch, ChT>& out,
    const linear_prog_task<T, REFCNT>& lpt
)
{
    // WARNING! Restricted implementation.

    out << "(";
    out << lpt.optimum() << ", ";
    output_list(out, lpt.c()); out << ", ";
    output_list(out, lpt.a()); out << ", ";
    output_list(out, lpt.b()); out << ", ";
    output_list(out, lpt.relation());
    output_list(out, lpt.individual());
    out << ")";

    return out;
}


template <typename T, bool REFCNT, typename Ch, typename ChT>
std::basic_istream<Ch, ChT>& input_list
(
    std::basic_istream<Ch, ChT>& in,
    linear_prog_task<T, REFCNT>& lpt
)
{
    if(!_Internal::read_literal(in, "("))
    {
        in.clear(std::ios_base::failbit);
        return in;
    }

    int ot;
    in >> ot;

    if(!_Internal::read_literal(in, ","))
    {
        in.clear(std::ios_base::failbit);
        return in;
    }

    typedef linear_prog_task<T, REFCNT> LPT;
    typename LPT::c_type c;
    in >> c;

    if(!_Internal::read_literal(in, ","))
    {
        in.clear(std::ios_base::failbit);
        return in;
    }

    typename LPT::a_type a;
    in >> a;

    if(!_Internal::read_literal(in, ","))
    {
        in.clear(std::ios_base::failbit);
        return in;
    }

    typename LPT::b_type b;
    in >> b;

    if(!_Internal::read_literal(in, ","))
    {
        in.clear(std::ios_base::failbit);
        return in;
    }

    typename LPT::relation_type relation;
    vector<int> relt;
    in >> relt;

    if(!_Internal::read_literal(in, ","))
    {
        in.clear(std::ios_base::failbit);
        return in;
    }

    relation.resize(relt.size());
    for(std::size_t i = 0; i < relt.size(); ++i)
        relation[i] = linear_prog_task_base::constraint_type(relt[i]);

    typename LPT::individual_type individual;
    vector<int> indt;
    in >> indt;

    individual.resize(indt.size());
    for(std::size_t i = 0; i < indt.size(); ++i)
        individual[i] = linear_prog_task_base::constraint_type(indt[i]);

    if(!_Internal::read_literal(in, ")"))
    {
        in.clear(std::ios_base::failbit);
        return in;
    }

    lpt = LPT
    (
        c,
        a, b,
        relation,
        individual,
        linear_prog_task_base::optimum_type(ot)
    );

    return in;
}


template <typename T, bool REFCNT, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& output_aligned
(
    std::basic_ostream<Ch, ChT>& out,
    const linear_prog_task<T, REFCNT>& lpt,
    const char* var = "x"
)
{
    if(lpt.optimum() == linear_prog_task_base::ot_max)
        out << "max";
    else
        out << "min";

    out << "(";
    bool first = true;
    for(std::size_t i = 0; i < lpt.c().size(); ++i)
        if(!is_null(lpt.c().el(i)))
        {
            if(first)
            {
                first = false;
                output_polynom_first(out, lpt.c().el(i));
            }
            else
            {
                output_polynom_internal(out, lpt.c().el(i));
            }

            out << "*" << var << i+1;
        }

    if(first)
        out << "0";
    out << ")\n";

    std::ostringstream buf;

    matrix<std::string, false> bufmatrix
    (
        lpt.a().nrows(),
        2*lpt.a().ncols() + 1 /*relation*/ + 1 /*right part*/,
        std::string()
    );

    for(std::size_t i = 0; i < bufmatrix.nrows(); ++i)
    {
        for(std::size_t j = 0; j < lpt.a().ncols(); ++j)
        {
            buf.str("");
            const T& el = lpt.a().el(i, j);
            if(is_negative(el))
            {
                bufmatrix(i, 2*j) = "-";
                buf << -el;
            }
            else if(is_positive(el))
            {
                buf << el;
            }

            buf << "*" << var << j+1;
            bufmatrix(i, 2*j + 1) = buf.str();
        }

        ARAGELI_ASSERT_1(lpt.relation().el(i) != linear_prog_task_base::ct_integer);

        switch(lpt.relation().el(i))
        {
            case linear_prog_task_base::ct_equal:
                bufmatrix(i, bufmatrix.ncols() - 2) = "=";
                break;
            case linear_prog_task_base::ct_greater_equal:
                bufmatrix(i, bufmatrix.ncols() - 2) = ">=";
                break;
            case linear_prog_task_base::ct_less_equal:
                bufmatrix(i, bufmatrix.ncols() - 2) = "<=";
                break;
        }

        buf.str("");
        buf << lpt.b().el(i);
        bufmatrix(i, bufmatrix.ncols() - 1) = buf.str();
    }

    output_aligned(out, bufmatrix, "", "", " ");

    for(std::size_t i = 0; i < lpt.individual().size(); ++i)
    {
        out << var << i+1;
        linear_prog_task_base::constraint_type ct =
            linear_prog_task_base::constraint_type
                (lpt.individual().el(i) & ~linear_prog_task_base::ct_integer);

        switch(ct)
        {
            case 0:
                break;
            case linear_prog_task_base::ct_equal:
                out << " = ";
                break;
            case linear_prog_task_base::ct_greater_equal:
                out << " >= ";
                break;
            case linear_prog_task_base::ct_less_equal:
                out << " <= ";
                break;
            default:
                ARAGELI_ASSERT_1(!"It's impossible!");
        }

        if(ct)
            out << "0";
        if(lpt.individual().el(i) & linear_prog_task_base::ct_integer)
        {
            if(ct)
                out << ",";
            out << " integer";
        }

        out << "\n";
    }

    return out;
}


}
}

#if 0
#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_SIMPLEX_METHOD
    #include "simplex_method.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_SIMPLEX_METHOD
#endif
#endif

#endif
