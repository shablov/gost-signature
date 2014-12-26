/*****************************************************************************

    ctrl_slog.hpp

    This file is part of Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2005--2007 Sergey S. Lyalin
    Copyright (C) 2006 Alexey Polovinkin

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

#ifndef _ARAGELI_CTRL_SLOG_HPP_
#define _ARAGELI_CTRL_SLOG_HPP_


/**
    \file ctrl_slog.hpp
    \brief Collection of simple loggers for contolled algorithm functions.

    This file contains a set of template classes that represent
    controllers of the controlled algorithms of the library.
    There are only "simple loggers" which name is ended by _slog suffix.
    These controllers provide simple output to stream of chars with little
    support of aligned output.
*/


#include "config.hpp"

#include "factory.hpp"
#include "exception.hpp"
#include "gauss.hpp"
#include "simplex_method.hpp"
#include "matrix.hpp"
#include "motzkin_burger.hpp"
#include "skeleton.hpp"
#include "advsimplmeth.hpp"

#include "std_import.hpp"

namespace Arageli
{
namespace ctrl
{

template <typename Stream>
struct rref_gauss_field_slog :
    public rref_gauss_field_idler
{
    class abort :
        public rref_gauss_field_idler::abort
    {};

    Stream& stream_m;
    bool preamble_on, conclusion_on;

    rref_gauss_field_slog
    (
        Stream& stream_a,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        stream_m(stream_a),
        preamble_on(preamble_on_a),
        conclusion_on(conclusion_on_a)
    {}

    template <typename A>
    void preamble (const A& a) const
    {
        if(!preamble_on)return;

        title();
        stream() << '\n';
        output_aligned(stream(), a);
        stream();
        finish_preamble();
        stream() << '\n';
    }

    template <typename B, typename Q, typename Det, typename Basis>
    void conclusion (const B& b, const Q& q, const Det& det, const Basis& basis) const
    {
        if(conclusion_on)
        {
            output_aligned_hor_pair(stream(), b, q);
            begin_conclusion(); stream() << ' ';
            rref_name();
            output_aligned(stream() << '\n', b);
            inverse_name();
            output_aligned(stream() << '\n', q);
        }
        else
        {
            output_aligned_hor_pair(stream(), b, q);
        }

        basis_name();
        stream() << ' ' << basis+1 << '\n';
        det_name();
        stream() << ' ' << det << '\n';
    }

    template <typename B, typename Q, typename Det, typename Basis>
    void before_iter (const B& b, const Q& q, const Det& det, const Basis& basis) const
    {
        output_aligned_hor_pair(stream(), b, q);
        basis_name();
        stream() << ' ' << basis+1 << '\n';
        det_name();
        stream() << ' ' << det << '\n';
    }

    template <typename B, typename Q, typename Det, typename Basis>
    void after_iter (const B& b, const Q& q, const Det& det, const Basis& basis) const
    {}

    template <typename J>
    void find_biggest_in_col (const J& j) const
    {
        find_biggest_in_col_name();
        stream() << ' ' << j+1 << '\n';
    }

    template <typename J>
    void negligible_col (const J& j) const
    {
        col_name();
        stream() << ' ' << j+1 << ' ';
        is_negligible_name();
        stream() << '\n';
    }

    template <typename I, typename J>
    void pivot_item (const I& i, const J& j) const
    {
        pivot_item_name();
        stream() << " (" << i+1 << ", " << j+1 << ")\n";
    }

    template <typename I1, typename I2, typename B, typename Q, typename Det>
    void swap_rows (const I1& i1, const I2& i2, const B& b, const Q& q, const Det& det) const
    {
        swap_rows_name();
        stream() << ' ' << i1+1 << ", " << i2+1 << '\n';
        output_aligned_hor_pair(stream(), b, q);
        det_name();
        stream() << ' ' << det << '\n';
    }

    template <typename J>
    void eliminate_col (const J& j) const
    {
        eliminate_col_name();
        stream() << ' ' << j+1 << '\n';
    }

    template <typename B, typename Q, typename Det>
    void after_elimination (const B& b, const Q& q, const Det& det) const
    {
        //output_aligned_hor_pair(stream(), b, q);
        //det_name();
        //stream() << det << '\n';
    }

protected:

    virtual std::ostream& stream () const
    {
        return stream_m;
    }

    virtual void title () const
    {
        stream() << "Producing of reduced row echelon form for matrix";
    }

    virtual void begin_conclusion () const
    {
        stream() << "The task has been solved. ";
    }

    virtual void finish_preamble () const
    {
        stream() << "Lets write additionaly an unitary matrix "
            "and produce gauss iterations. ";
    }

    virtual void find_biggest_in_col_name () const
    {
        stream() << "Find the biggest (in absolute value) entry in a column";
    }

    virtual void det_name () const
    {
        stream() << "Determinant is";
    }

    virtual void basis_name () const
    {
        stream() << "Basis is";
    }

    virtual void col_name () const
    {
        stream() << "Column";
    }

    virtual void is_negligible_name () const
    {
        stream() << "is negligible";
    }

    virtual void pivot_item_name () const
    {
        stream() << "Pivot item is";
    }

    virtual void swap_rows_name () const
    {
        stream() << "Swap rows";
    }

    virtual void eliminate_col_name () const
    {
        stream() << "Eliminate in column";
    }

    virtual void rref_name () const
    {
        stream() << "Found reduced row echelon form is";
    }

    virtual void inverse_name () const
    {
        stream() << "Inverse of input matrix is";
    }
};


template <typename Stream>
inline rref_gauss_field_slog<Stream> make_rref_gauss_field_slog
(
    Stream& stream,
    bool preamble = true,
    bool conclusion = true
)
{
    return rref_gauss_field_slog<Stream>(stream, preamble, conclusion);
}


template <typename Stream>
struct rref_gauss_bareiss_slog :
    public rref_gauss_field_slog<Stream>
{
protected:

    using rref_gauss_field_slog<Stream>::stream;

public:

    class abort :
        public rref_gauss_bareiss_idler::abort
    {};

    rref_gauss_bareiss_slog
    (
        Stream& stream_a,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        rref_gauss_field_slog<Stream>
        (
            stream_a,
            preamble_on_a,
            conclusion_on_a
        )
    {}

    template
    <
        typename B, typename Q, typename Det,
        typename Basis, typename Det_denom
    >
    void before_iter
    (
        const B& b, const Q& q, const Det& det,
        const Basis& basis, const Det_denom& det_denom
    ) const
    {
        rref_gauss_field_slog<Stream>::before_iter(b, q, det, basis);
        det_denom_name();
        stream() << ' ' << det_denom << '\n';
    }

    template
    <
        typename B, typename Q, typename Det,
        typename Basis, typename Det_denom
    >
    void after_iter
    (
        const B& b, const Q& q, const Det& det,
        const Basis& basis, const Det_denom& det_denom
    ) const
    {
        rref_gauss_field_slog<Stream>::after_iter(b, q, det, basis);
    }

    template
    <
        typename I1, typename I2, typename B,
        typename Q, typename Det, typename Det_denom
    >
    void swap_rows
    (
        const I1& i1, const I2& i2, const B& b,
        const Q& q, const Det& det, const Det_denom& det_denom
    ) const
    {
        rref_gauss_field_slog<Stream>::swap_rows(i1, i2, b, q, det);
        det_denom_name();
        stream() << ' ' << det_denom << '\n';
    }

    template <typename B, typename Q, typename Det, typename Det_denom>
    void after_elimination
    (
        const B& b, const Q& q, const Det& det,
        const Det_denom& det_denom
    ) const
    {
        //rref_slog<Stream>::after_elimination(b, q, det);
        //det_denom_name();
        //stream() << ' ' << det_denom << '\n';
    }

    template <typename J>
    void find_nonzero_in_col (const J& j) const
    {
        find_nonzero_in_col_name();
        stream() << ' ' << j+1 << '\n';
    }

protected:

    virtual void title () const
    {
        stream() << "Producing of reduced row echelon form for integer matrix";
    }

    virtual void find_nonzero_in_col_name () const
    {
        stream() << "Find nonzero entry in a column";
    }

    virtual void det_denom_name () const
    {
        stream() << "Determinant denominator";
    }

    virtual void det_name () const
    {
        stream() << "Determinant numerator";
    }
};


template <typename Stream>
inline rref_gauss_bareiss_slog<Stream> make_rref_gauss_bareiss_slog
(
    Stream& stream,
    bool preamble = true,
    bool conclusion = true
)
{
    return rref_gauss_bareiss_slog<Stream>(stream, preamble, conclusion);
}


template <typename Stream>
struct rref_field_slog :
    public rref_field_idler
{
    class abort :
        public rref_field_idler::abort
    {};

    Stream& stream_m;
    bool preamble_on, conclusion_on;

    rref_field_slog
    (
        Stream& stream_a,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        stream_m(stream_a),
        preamble_on(preamble_on_a),
        conclusion_on(conclusion_on_a)
    {}

    rref_gauss_field_slog<Stream> ctrl_rref_gauss_field () const
    {
        return
            rref_gauss_field_slog<Stream>
            (
                stream_m,
                preamble_on,
                conclusion_on
            );
    }
};


template <typename Stream>
inline rref_field_slog<Stream> make_rref_field_slog
(
    Stream& stream,
    bool preamble = true,
    bool conclusion = true
)
{
    return rref_field_slog<Stream>(stream, preamble, conclusion);
}


template <typename Stream>
struct rref_int_slog :
    public rref_int_idler
{
    class abort :
        public rref_int_idler::abort
    {};

    Stream& stream_m;
    bool preamble_on, conclusion_on;

    rref_int_slog
    (
        Stream& stream_a,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        stream_m(stream_a),
        preamble_on(preamble_on_a),
        conclusion_on(conclusion_on_a)
    {}

    rref_gauss_bareiss_slog<Stream> ctrl_rref_gauss_bareiss () const
    {
        return
            rref_gauss_bareiss_slog<Stream>
            (
                stream_m,
                preamble_on,
                conclusion_on
            );
    }
};


template <typename Stream>
inline rref_int_slog<Stream> make_rref_int_slog
(
    Stream& stream,
    bool preamble = true,
    bool conclusion = true
)
{
    return rref_int_slog<Stream>(stream, preamble, conclusion);
}


template <typename Stream>
struct rref_slog :
    public rref_idler
{
    class abort :
        public rref_idler::abort
    {};

    Stream& stream_m;
    bool preamble_on, conclusion_on;

    rref_slog
    (
        Stream& stream_a,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        stream_m(stream_a),
        preamble_on(preamble_on_a),
        conclusion_on(conclusion_on_a)
    {}

    rref_field_slog<Stream> ctrl_rref_field () const
    {
        return rref_field_slog<Stream>(stream_m, preamble_on, conclusion_on);
    }

    rref_int_slog<Stream> ctrl_rref_int () const
    {
        return rref_int_slog<Stream>(stream_m, preamble_on, conclusion_on);
    }
};


template <typename Stream>
inline rref_slog<Stream> make_rref_slog
(
    Stream& stream,
    bool preamble = true,
    bool conclusion = true
)
{
    return rref_slog<Stream>(stream, preamble, conclusion);
}


template <typename Stream>
struct smith_slog
{
    Stream& stream;

    smith_slog (Stream& stream_a) :
        stream(stream_a)
    {}

    void preamble ()
    {
        stream << "Smith's normal diagonal form\n";
    }

    void conclusion ()
    {}

    template <typename Q, typename B, typename P>
    void current_matrices (const Q& q, const B& b, const P& p)
    {
        output_aligned_corner_triplet_br(stream, q, b, p);
    }

    template <typename I, typename J>
    void find_smallest_nonzero (const I& i, const J& j)
    {
        stream
            << "The smallest (in absolute value) non-zero entry: ("
            << i << ", " << j << ")\n";
    }

    template <typename I, typename J>
    void pivot_item (const I& i, const J& j)
    {
        stream << "Pivoting item: (" << i << ", " << j << ")\n";
    }

    void after_pivoting ()
    {
        stream << "After pivoting step:\n";
    }

    template <typename I, typename J>
    void nondivisor_entry (const I& k, const I& i, const J& l)
    {
        stream
            << "Non-divisor entry: (" << k << ", " << l << ")\n"
            << "Add the " << k << "-th row to the " << i
            << "-th row\n";
    }

    void pivot_adjustment ()
    {
        stream << "After pivot item adjustment:\n";
    }

    template
    <
        typename C,
        typename I,
        typename J,
        typename Q,
        typename B,
        typename P
    >
    bool stop
    (
        const C& corner,
        const I& i,
        const J& j,
        const Q& q,
        const B& b,
        const P& p
    )
    {
        return false;
    }
};


template <typename Stream>
inline smith_slog<Stream> make_smith_slog (Stream& stream)
{
    return smith_slog<Stream>(stream);
}


namespace simplex_method
{

//using namespace Arageli::simplex_method;


/// Simple controller for the primal_col_iters function.  It outputs into a stream.
template <typename Stream>
struct primal_col_iters_slog
{
    Stream& stream;

    primal_col_iters_slog (Stream& stream_a) :
        stream(stream_a)
    {}

    void preamble () const
    {
        stream << "Direct column simplex method iterations.\n";
    }

    void conclusion () const
    {}

    primal_col_iter_slog<Stream> iter_ctrl () const
    {
        return primal_col_iter_slog<Stream>(stream);
    }

    template <typename TQ, typename Tb>
    bool stop (const TQ&, const Tb&) const
    {
        return false;
    }
};


/// Simple controller for the dual_col_iters function.  It outputs into a stream.
template <typename Stream>
struct dual_col_iters_slog
{
    Stream& stream;

    dual_col_iters_slog (Stream& stream_a) :
        stream(stream_a)
    {}

    void preamble () const
    {
        stream << "Direct column simplex method iterations.\n";
    }

    void conclusion () const
    {}

    dual_col_iter_slog<Stream> iter_ctrl () const
    {
        return dual_col_iter_slog<Stream>(stream);
    }

    template <typename TQ, typename Tb>
    bool stop (const TQ&, const Tb&) const
    {
        return false;
    }
};


/// Simple controller for the primal_row_iters function.  It outputs into a stream.
template <typename Stream>
struct primal_row_iters_slog :
    public primal_row_iters_idler
{
    class abort :
        public primal_row_iters_idler::abort
    {};

    Stream& stream;
    bool preamble_on, conclusion_on;

    primal_row_iters_slog
    (
        Stream& stream_a,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        stream(stream_a),
        preamble_on(preamble_on_a),
        conclusion_on(conclusion_on_a)
    {}

    template <typename Q, typename Basis>
    void preamble (const Q& q, const Basis& basis) const
    {
        if(!preamble_on)return;

        stream << begin_preamble();
        if(*begin_preamble())
            stream << '\n';
        stream << finish_preamble();
        if(*finish_preamble())
            stream << '\n';
    }

    template <typename Q, typename Basis>
    void conclusion (const Q& q, const Basis& basis, result_kind rk) const
    {
        stream << result_name(rk) << ".\n";
        if(!conclusion_on)return;
        stream << finish_conclusion() << ".\n";
    }

    template <typename Q, typename Basis>
    void before_iter (const Q& q, const Basis& basis) const
    {
        output_aligned(stream, q);
        stream << basis_name() << ' ' << basis << '\n';
    }

    template <typename Q, typename Basis, typename Pivot>
    void after_iter
    (
        const Q& q, const Basis& basis,
        Pivot prow, Pivot pcol,
        result_kind rk
    ) const
    {
        ARAGELI_ASSERT_0(rk != rk_empty);

        switch(rk)
        {
            case rk_infinite:
                stream << pivot_col_name() << ' ' << pcol << '\n';
                break;
            case rk_nonoptimal:
                stream
                    << pivot_item_name()
                    << " (" << prow << ", " << pcol << ")\n";
                break;
        }
    }

protected:

    virtual const char* begin_preamble () const
    {
        return "The primal row iterations on simplex table";
    }

    virtual const char* finish_preamble () const
    {
        return "";
    }

    virtual const char* basis_name () const
    {
        return "Basis is";
    }

    virtual const char* finish_conclusion () const
    {
        return "The task has been solved";
    }

    virtual const char* result_name (result_kind rk) const
    {
        static const char* rns[] =
        {
            "An optimum is found",
            "The set of feasible points is empty",
            "The criterion function is unbounded",
            "The table is nonoptimal"
        };

        return rns[rk];
    }

    virtual const char* pivot_col_name () const
    {
        return "Pivot column is";
    }

    virtual const char* pivot_item_name () const
    {
        return "Pivot item is";
    }
};


/// Simple controller for the basis_create_by_artificial function.  It outputs into a stream.
template <typename Stream>
struct basis_create_by_artificial_slog :
    public basis_create_by_artificial_idler
{
    class abort :
        public basis_create_by_artificial_idler::abort
    {};

    Stream& stream;
    bool preamble_on, conclusion_on;

    basis_create_by_artificial_slog
    (
        Stream& stream_a,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        stream(stream_a),
        preamble_on(preamble_on_a),
        conclusion_on(conclusion_on_a)
    {}

    template <typename Q, typename Basis>
    void preamble (const Q& q, const Basis& basis) const
    {
        if(!preamble_on)return;

        stream << begin_preamble();
        if(*begin_preamble())
            stream << '\n';
        output_aligned(stream, q);
        stream << basis_name() << ' ' << basis << '\n';
        stream << finish_preamble();
        if(*finish_preamble())
            stream << '\n';
    }

    template <typename Q, typename Basis>
    void conclusion (const Q& q, const Basis& basis) const
    {
        if(!conclusion_on)return;

        stream << built_table_name() << '\n';
        output_aligned(stream, q);
        stream << basis_name() << ' ' << basis << '\n';
        stream << finish_conclusion() << ".\n";
    }

    template <typename Q, typename Basis, typename Index>
    void artif_in_basis
    (
        const Q& q,
        const Basis& basis,
        const Index& index
    ) const
    {
        output_aligned(stream, q);
        stream << basis_name() << ' ' << basis << '\n';
        stream << artif_in_basis_name() << ' ' << basis[index] << ".\n";
    }

    template <typename I>
    void negligible_row (const I& i) const
    {
        stream
            << row_name() << ' ' << i << ' '
            << is_negligible_name() << ".\n";
    }

    template <typename Q, typename Basis, typename Index>
    void replace_basis_item
    (
        const Q& q, const Basis& basis,
        const Index& iold, const Index& r, const Index& inew
    ) const
    {
        stream
            << replace_basis_item_name() << ' ' << basis[iold]
            << ' ' << to_name() << ' ' << inew << '\n';
        stream << pivot_item_name() << " (" << r << ", " << inew << '\n';
    }

    template <typename Q, typename Basis>
    void before_erase_artif (const Q& q, const Basis& basis) const
    {
        output_aligned(stream, q);
        stream << basis_name() << ' ' << basis << '\n';
    }

protected:

    virtual const char* begin_preamble () const
    {
        return "Eliminating of artificial variables out of basis";
    }

    virtual const char* finish_preamble () const
    {
        return "";
    }

    virtual const char* basis_name () const
    {
        return "Basis is";
    }

    virtual const char* built_table_name () const
    {
        return "The resulting table is";
    }

    virtual const char* finish_conclusion () const
    {
        return "The task has been solved";
    }

    virtual const char* artif_in_basis_name () const
    {
        return "Artificial variable in basis is";
    }

    virtual const char* row_name () const
    {
        return "Row";
    }

    virtual const char* is_negligible_name () const
    {
        return "is negligible";
    }

    virtual const char* pivot_item_name () const
    {
        return "Pivot item is";
    }

    virtual const char* replace_basis_item_name () const
    {
        return "Replace artificial variable";
    }

    virtual const char* to_name () const
    {
        return "to";
    }
};


/// Simple controller for the primal_row_iters function.  It outputs into a stream.
template <typename Stream>
struct basis_artificial_slog :
    public basis_artificial_idler
{
    class abort :
        public basis_artificial_idler::abort
    {};

    Stream& stream;
    bool preamble_on, conclusion_on;

    basis_artificial_slog
    (
        Stream& stream_a,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        stream(stream_a),
        preamble_on(preamble_on_a),
        conclusion_on(conclusion_on_a)
    {}

    template <typename Q>
    void preamble (const Q& q) const
    {
        if(!preamble_on)return;

        stream << begin_preamble();
        if(*begin_preamble())
            stream << '\n';
        output_aligned(stream, q);
        stream << finish_preamble();
        if(*finish_preamble())
            stream << '\n';
    }

    template <typename Q, typename Basis>
    void conclusion (const Q& q, const Basis& basis, result_kind rk) const
    {
        ARAGELI_ASSERT_0(rk == rk_found || rk == rk_empty);

        if(rk == rk_empty)
        {
            stream << conclusion_empty_name() << ".\n";
            stream << finish_conclusion() << ".\n";
        }

        if(rk == rk_found && conclusion_on)
        {
            stream << built_table_name() << '\n';
            output_aligned(stream, q);
            stream << basis_name() << ' ' << basis << '\n';
            stream << finish_conclusion() << ".\n";
        }
    }

    primal_row_iters_slog<Stream>
    ctrl_primal_row_iters () const
    {
        return primal_row_iters_slog<Stream>(stream, false, false);
    }

    basis_create_by_artificial_slog<Stream>
    ctrl_basis_create_by_artificial () const
    {
        return basis_create_by_artificial_slog<Stream>(stream, false, false);
    }

    template <typename Q, typename Basis>
    void after_artif (const Q& q, const Basis& basis) const
    {
        stream << table_with_artif_name() << '\n';
        //output_aligned(stream, q);
        //stream << basis_name() << ' ' << basis << '\n';
    }

    template <typename Q, typename Basis>
    void before_iters (const Q& q, const Basis& basis) const
    {
        stream << optimize_artif_name() << '\n';
    }

    template <typename Q, typename Basis>
    void after_iters (const Q& q, const Basis& basis) const
    {}

    template <typename Q, typename Basis>
    void before_orig (const Q& q, const Basis& basis) const
    {
        stream << eliminate_artif_name() << '\n';
    }

    template <typename Q, typename Basis>
    void after_orig (const Q& q, const Basis& basis) const
    {}

protected:

    virtual const char* begin_preamble () const
    {
        return "Building a primal feasible basis of a simplex table";
    }

    virtual const char* finish_preamble () const
    {
        return "";
    }

    virtual const char* basis_name () const
    {
        return "Basis is";
    }

    virtual const char* conclusion_empty_name () const
    {
        return "Set of feseable points is empty";
    }

    virtual const char* built_table_name () const
    {
        return "The resulting table is";
    }

    virtual const char* finish_conclusion () const
    {
        return "The task has been solved";
    }

    virtual const char* table_with_artif_name () const
    {
        return "Table with artificial variables";
    }

    virtual const char* optimize_artif_name () const
    {
        return "Optimization of table with artificial variables";
    }

    virtual const char* eliminate_artif_name () const
    {
        return "Elimination artificial variables from basis";
    }
};

/// Simple controller for the primal_row_with_artificial_basis function.  It outputs into a stream.
template <typename Stream>
struct primal_row_with_artificial_basis_slog
{
    Stream& stream;

    primal_row_with_artificial_basis_slog (Stream& stream_a) :
        stream(stream_a)
    {}

    void preamble () const
    {
        stream
            << "The primal simple method with building the first "
            << "allowable table via the artificial basis method.\n";
    }

    template <typename A, typename B, typename C>
    void input_data (const A& a, const B& b, const C& c) const
    {
        stream << "Initial data:\nA =\n";
        output_aligned(stream, a);
        stream << "b = " << b << "\nc = " << c << "\n";
    }

    template <typename T>
    void table_for_artificial (const T& table) const
    {
        stream << "Table for artificial:\n";
        output_aligned(stream, table);
        stream << "\n";
    }

    template <typename T, typename B>
    void artificial_table (const T& table, const B& basis) const
    {
        stream << "Artificial table:\n";
        output_aligned(stream, table);
        stream << "basis = " << basis << "\n";
    }

    ctrl::simplex_method::primal_row_iters_slog<Stream> ctrl_for_artificial () const
    {
        return ctrl::simplex_method::primal_row_iters_slog<Stream>(stream);
    }

    template <typename T, typename B>
    void optimal_artificial_table (const T& table, const B& basis) const
    {
        stream << "Optimal artificial table:\n";
        output_aligned(stream, table);
        stream << "basis = " << basis << "\n";
    }

    ctrl::simplex_method::basis_create_by_artificial_slog<Stream>
    ctrl_for_basis_create_by_artificial () const
    {
        return ctrl::simplex_method::basis_create_by_artificial_slog<Stream>(stream);
    }

    template <typename T>
    void pre_first_table (const T& table) const
    {
        stream << "The pre-first allowable table:\n";
        output_aligned(stream, table);
    }


    ctrl::simplex_method::primal_row_iters_slog<Stream>
    ctrl_for_main_row_iters () const
    {
        return ctrl::simplex_method::primal_row_iters_slog<Stream>(stream);
    }

    void result (result_kind rk) const
    {
        const char* status[] = {"FOUND", "EMPTY", "INFINITE", "NONOPTIMAL"};
        stream << "\nStatus: " << status[rk] << "\n";
    }

    void conclusion () const
    {
        stream << "\nDone.\n";
    }
};


/// TEMPORARY IMPLEMENTATION!!!
template <typename Stream>
struct gomory1_iter_slog :
    public gomory1_iter_idler
{
    class abort :
        public gomory1_iter_idler::abort
    {};

    Stream& stream;
    bool preamble_on, conclusion_on;

    gomory1_iter_slog
    (
        Stream& stream_a,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        stream(stream_a),
        preamble_on(preamble_on_a),
        conclusion_on(conclusion_on_a)
    {}

    template <typename T, typename Nonbasis>
    void preamble (const T& t, const Nonbasis& nonbasis) const
    {}

    template <typename T, typename Nonbasis>
    void conclusion (const T& t, const Nonbasis& nonbasis, result_kind rk) const
    {}

    template <typename T, typename Prow>
    void after_gomory1_clip (const T& t, Prow prow, result_kind rk) const
    {
        if(rk == rk_nonoptimal)
            stream << "Номер производящей строки " << prow << ".\n";
    }

    dual_col_iters_slog<Stream> ctrl_for_dual_col_iters () const
    {
        return dual_col_iters_slog<Stream>(stream);
    }
};

} // namespace simplex_method


template <typename Stream>
struct skeleton_motzkin_burger_slog :
    public skeleton_motzkin_burger_idler
{
    class abort :
        public skeleton_motzkin_burger_idler::abort
    {};

    Stream& stream;
    bool output_top, preamble_on, conclusion_on;

    skeleton_motzkin_burger_slog
    (
        Stream& stream_a,
        bool output_top_a = true,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        stream(stream_a),
        output_top(output_top_a),
        preamble_on(preamble_on_a),
        conclusion_on(conclusion_on_a)
    {}

    template <typename A, typename F, typename Q, typename E>
    void preamble (const A& a, const F& f, const Q& q, const E& e) const
    {
        if(!preamble_on)return;
        stream << "Motzkin-Burger algorithm start of work with:\n";
        output_aligned(stream << "\nA =\n", a);
        output_aligned(stream << "\nF =\n", f);
        output_aligned(stream << "\nQ =\n", q);
        output_aligned(stream << "\nE =\n", e);
    }

    void begin_gauss () const
    {
        stream
            << "1. Gaussian algorithm\n"
            << "   ******************\n";
    }

    void end_gauss () const
    {}

    template <typename I>
    void find_non_zero (const I& i) const
    {
        stream
            << "Current row: " << i
            << "\nFind non-zero entry in the " << i
            << "-th row beginning from " << i << "-th entry\n";
    }

    void zero_row () const
    {
        stream << "Zero row. Matrix is not of full rank.\n";
    }

    template <typename I, typename J>
    void swap_cols_rows (const I& i, const J& j) const
    {
        stream << "Swap " << i << "-th and " << j << "-th columns:\n";
    }

    template <typename J>
    void eliminate_col (const J& j) const
    {
        stream << "Gaussian elimination in the " << j << "-th column: \n";
    }

    template <typename A, typename F, typename Q>
    void show_matrices (const A& a, const F& f, const Q& q) const
    {
        if(output_top)
            output_aligned_corner_triplet_br(stream, transpose(a), q, f);
        else
            output_aligned(stream, q);
    }

    void begin_motzkin () const
    {
        stream
            << "2. Motzkin algorithm\n"
            << "   *****************\n";
    }

    void end_motzkin () const
    {}

    template <typename I1, typename I2>
    void select_col (const I1& current_column, const I2& new_column) const
    {
        stream << "Current column: " << current_column << '\n';
        stream << "New column: " << new_column << '\n';
    }

    void zero_solution () const
    {
        stream << "No solution exept zero\n";
    }

    void corollary_inequality () const
    {
        stream << "This inequality is a corollary\n";
    }

    void begin_row_balancing () const
    {
        stream << "Balanced rows: ";
    }

    template <typename I, typename J>
    void balanced_rows (const I& j_p, const J& j_m) const
    {
        stream << "(" << j_p << ", " << j_m << ") ";
    }

    void end_row_balancing () const
    {
        stream << '\n';
    }

    void delete_negates () const
    {
        stream << "Delete negative rows:\n";
    }

    template <typename A, typename F, typename Q, typename E>
    void conclusion (const A& a, const F& f, const Q& q, const E& e) const
    {
        if(!conclusion_on)return;
        stream << "Motzkin-Burger is finished. Output matrices are:\n";
        output_aligned(stream << "\nA =\n", a);
        output_aligned(stream << "\nF =\n", f);
        output_aligned(stream << "\nQ =\n", q);
        output_aligned(stream << "\nE =\n", e);
    }
};


template <typename Stream>
inline skeleton_motzkin_burger_slog<Stream>
make_skeleton_motzkin_burger_slog
(
    Stream& stream,
    bool output_top = true,
    bool preamble_on = true,
    bool conclusion_on = true
)
{
    return
        skeleton_motzkin_burger_slog<Stream>
        (
            stream,
            output_top,
            preamble_on,
            conclusion_on
        );
}


template <typename Stream>
struct skeleton_slog :
    public skeleton_idler
{
    class abort :
        public skeleton_idler::abort
    {};

    Stream& stream;
    bool output_top, preamble_on, conclusion_on;

    skeleton_slog
    (
        Stream& stream_a,
        bool output_top_a,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        stream(stream_a),
        output_top(output_top_a),
        preamble_on(preamble_on_a),
        conclusion_on(conclusion_on_a)
    {}

    skeleton_motzkin_burger_slog<Stream> motzkin_burger_ctrl () const
    {
        return
            make_skeleton_motzkin_burger_slog
            (
                stream,
                output_top,
                preamble_on,
                conclusion_on
            );
    }

};


template <typename Stream>
inline skeleton_slog<Stream>
make_skeleton_slog
(
    Stream& stream,
    bool output_top = true,
    bool preamble_on = true,
    bool conclusion_on = true
)
{
    return
        skeleton_slog<Stream>
        (
            stream,
            output_top,
            preamble_on,
            conclusion_on
        );
}


namespace simplex_method
{

const std::string status_str[] =
{
    "\"free\"",
    "\"on lower bound\"",
    "\"on upper bound\"",
    "\"fixed\""
};

template<typename Stream>
struct adv_simplex_method_alg_slog :
    public adv_simplex_method_alg_idler
{
    Stream& m_Stream;

    adv_simplex_method_alg_slog(Stream& stream) :
        m_Stream(stream)
    {}

    template<typename T1, typename T2, typename T3>
    void before_iter
    (
        const T1& B,
        const T1& N,
        const T2& basic_var_nums,
        const T2& non_basic_var_nums,
        const T3& iter_num
    )
    {
        m_Stream << "Simplex method iteration " << iter_num << '\n';
        m_Stream << "Basic variables numbers are: " << basic_var_nums << '\n';
        m_Stream << "Basic matrix is:\n";
        output_aligned(m_Stream, B);
        m_Stream << "Non basic variables numbers are: " << non_basic_var_nums << '\n';
        m_Stream << "Non basic matrix is:\n";
        output_aligned(m_Stream, N);
    }

    template<typename T>
    void print_reduced_costs(const T& d)
    {
        m_Stream << "Reduced costs: " << d << '\n';
    }

    template<typename T>
    void print_vars_values
    (
        const T& basic_vars_values,
        const T& non_basic_vars_values
    )
    {
        m_Stream << "Values of basic variables:\n";
        m_Stream << basic_vars_values << '\n';
        m_Stream << "Values of non basic variables:\n";
        m_Stream << non_basic_vars_values << '\n';
    }

    template <typename T1, typename T2, typename T3>
    void print_non_basic_pivot_var
    (
        const T1& var_num,
        const T2& var_status,
        const T3& di
    )
    {
        m_Stream << "Pivot non basic variable number is " << var_num << ",\n";
        m_Stream << "because variable status is " << status_str[var_status];
        m_Stream << " and reduced cost is " << di << '\n';
    }

    template <typename T1, typename T2, typename T3, typename T4, typename T5>
    void task_info
    (
        const T1& num_aux_vars,
        const T1& num_struct_vars,
        const T2& var_names,
        const T3& A,
        const T4& is_lower_bounds_inf,
        const T4& is_upper_bounds_inf,
        const T5& lower_bounds,
        const T5& upper_bounds
    )
    {
        m_Stream << "Task info:\n";
        m_Stream << "Number of auxiliary variables: " << num_aux_vars << '\n';
        m_Stream << "Number of structural variables: " << num_struct_vars << '\n';
        m_Stream << "Auxiliary variables are: ";
        for (int i = 0; i < num_aux_vars - 1; i++)
            m_Stream << var_names[i] << ", ";
        m_Stream << var_names[num_aux_vars - 1] << '\n';
        m_Stream << "Structural variables are: ";
        for (int i = num_aux_vars; i < num_aux_vars + num_struct_vars - 1; i++)
            m_Stream << var_names[i] << ", ";
        m_Stream << var_names[num_aux_vars + num_struct_vars - 1] << '\n';
        m_Stream << "Constraint matrix A:\n";
        output_aligned(m_Stream, A);
        m_Stream << "Variables bounds:\n";
        for (int i = 0; i < num_aux_vars + num_struct_vars; i++)
        {
            if (is_lower_bounds_inf[i])
                m_Stream << "-inf <= ";
            else
                m_Stream << lower_bounds[i] << " <= ";
            m_Stream << var_names[i] << " <= ";
            if (is_upper_bounds_inf[i])
                m_Stream << "+inf\n";
            else
                m_Stream << upper_bounds[i] << '\n';
        }
        after_iter();
    }

    template<typename T1, typename T2, typename T3, typename T4, typename T5>
    void art_basis_method_info
    (
        const T1& c,
        const T2& num_aux_vars,
        const T3& aux_var_names,
        const T4& is_lower_bounds_inf,
        const T4& is_upper_bounds_inf,
        const T5& lower_bounds,
        const T5& upper_bounds
    )
    {
        m_Stream << "Starting artificial basis method...\n";
        m_Stream << "New objective function coefficients: " << c << '\n';
        m_Stream << "New bounds for auxiliary variables:\n";
        for (int i = 0; i < num_aux_vars; i++)
        {
            if (is_lower_bounds_inf[i])
                m_Stream << "-inf <= ";
            else
                m_Stream << lower_bounds[i] << " <= ";
            m_Stream << aux_var_names[i] << " <= ";
            if (is_upper_bounds_inf[i])
                m_Stream << "+inf\n";
            else
                m_Stream << upper_bounds[i] << '\n';
        }
        m_Stream << '\n';
    }

    template<typename T1>
    void print_basic_pivot_var(const T1& var_num)
    {
        m_Stream << "Pivot basic variable number is " << var_num << "\n";
    }

    void print_unbounded_sol_info()
    {
        m_Stream << "Objective function is unbounded!\n";
    }

    template <typename T1, typename T2>
    void print_non_basic_var_status_changed
    (
        const T1& var_num,
        const T2& curr_status,
        const T2& new_status
    )
    {
        m_Stream << "Non basic variable " << var_num << " status is changed\n";
        m_Stream << "from " << status_str[curr_status] << " to " << status_str[new_status] << '\n';
    }

    void after_iter()
    {
        for (int i = 0; i < 80; i++)
            m_Stream << "-";
        m_Stream << '\n';
    }

    template <typename T1, typename T2, typename T3, typename T4>
    void print_solution
    (
        const T1& num_struct_vars,
        const T2& basic_var_nums,
        const T2& non_basic_var_nums,
        const T3& basic_vars_values,
        const T3& non_basic_vars_values,
        const T4& var_names
    )
    {
        int i = 0;
        int num_vars = basic_var_nums.size() + non_basic_var_nums.size();
        int num_aux_vars = num_vars - num_struct_vars;
        T3 solution(num_struct_vars);
        for (i = 0; i < num_vars - num_struct_vars; i++)
            if (basic_var_nums[i] >= num_aux_vars)
                solution[basic_var_nums[i] - num_aux_vars] = basic_vars_values[i];
        for (i = 0; i < num_struct_vars; i++)
            if (non_basic_var_nums[i] >= num_aux_vars)
                solution[non_basic_var_nums[i] - num_aux_vars] = non_basic_vars_values[i];
        m_Stream << "Optimal solution:\n";
        for (i = 0; i < num_struct_vars; i++)
            m_Stream << var_names[i + num_aux_vars] << " = " << solution[i] << '\n';
    }

    void print_nofeasible_exists_info()
    {
        m_Stream << "Feasible solution was not found during artificial basis method!\n";
    }

    void art_basis_successful_info()
    {
        m_Stream << "Primal feasible solution was found\nEnding of artificial basis method...\n";
        after_iter();
        m_Stream << '\n';
    }
};


// TODO Add make functions for adv_simplex_method_alg_slog.

} // namespace simplex_method

} // namespace ctrl

} // namespace Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_CTRL_SLOG
    //#include "ctrl_slog.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_CTRL_SLOG
#endif


#endif
