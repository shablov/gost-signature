/*****************************************************************************

    ctrl_latexlog.hpp

    This file is part of Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2005--2007 Sergey S. Lyalin

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
    \file ctrl_latexlog.hpp
    \brief Collection of LaTeX loggers for contolled algorithm functions.

    This file contains a set of template classes that represent
    controllers of the controlled algorithms of the library.
    There are only "LaTeX loggers" which name is ended by _latexlog suffix.
    These controllers provide complex output to stream of chars as
    part of LaTeX file.
*/


#ifndef _ARAGELI_CTRL_LATEXLOG_HPP_
#define _ARAGELI_CTRL_LATEXLOG_HPP_

#include "config.hpp"

#include <cstddef>

#include "factory.hpp"
#include "exception.hpp"
#include "gauss.hpp"
#include "simplex_method.hpp"
#include "texout.hpp"
#include "ctrl_slog.hpp"

#include "std_import.hpp"

namespace Arageli
{
namespace ctrl
{

namespace _Internal
{

// TEMPORAL DEFINITIONS!!!

extern int table_on_one_line;

}

#if 0
// The following commented out block based on one of the old
// versions of the library and cannot be compiled.

/// Definition of this class is depricated. Do not use it.
template <typename Stream>
struct rref_latexlog : public rref_slog<Stream>
{
    class abort :
        public rref_int_idler::abort
    {};

    #if 0
    // Neither these names are in rref_slog defined.
    // This block remains from one of the old versions
    // of rref_slog.
    // TODO Indestigate reasonable replacment for rref_latexlog.

    using rref_slog<Stream>::stream_m;
    using rref_slog<Stream>::preamble_on;
    using rref_slog<Stream>::conclusion_on;
    using rref_slog<Stream>::begin_conclusion;
    using rref_slog<Stream>::rref_name;
    using rref_slog<Stream>::inverse_name;
    using rref_slog<Stream>::basis_name;
    using rref_slog<Stream>::det_name;
    using rref_slog<Stream>::title;
    using rref_slog<Stream>::finish_preamble;
    using rref_slog<Stream>::find_biggest_in_col_name;
    using rref_slog<Stream>::col_name;
    using rref_slog<Stream>::is_negligible_name;
    using rref_slog<Stream>::pivot_item_name;
    using rref_slog<Stream>::swap_rows_name;
    using rref_slog<Stream>::eliminate_col_name;

    #endif


private:

    mutable refcntr_proxy<std::ostringstream> buffer;

    struct Matrix_wrapper
    {
        virtual void output (Stream& stream) const = 0;
        virtual void output (Stream& stream, std::size_t i, std::size_t j) const = 0;
    };

    template <typename M>
    struct Custom_matrix_wrapper : public Matrix_wrapper
    {
        M m;
        std::size_t verpos;

        Custom_matrix_wrapper (const M& m_a, std::size_t verpos_a) :
            m(m_a),
            verpos(verpos_a)
        {}

        virtual void output (Stream& stream) const
        {
            stream << " $$";
            matrix_line v(verpos);
            output_latex_matrix_frames(stream, m, make_matrix_vers(&v, &v + 1), true);
            stream << "$$\n";
        }

        virtual void output (Stream& stream, std::size_t i, std::size_t j) const
        {
            stream << " $$";
            matrix_line ver(verpos);
            matrix_box box(i, j);

            output_latex_matrix_frames
            (
                stream, m,
                make_matrix_vers_boxes(&ver, &ver + 1, &box, &box + 1),
                true
            );

            stream << "$$\n";
        }
    };

    mutable Matrix_wrapper* m;

    //Stream& stream;

    void flush_buffer () const
    {
        stream_m << buffer.value().str();
        buffer.value().str("");
    }

    void flush_matrix () const
    {
        if(m)
        {
            m->output(stream_m);
            delete m;
            m = 0;
        }
    }

    void flush_matrix (std::size_t i, std::size_t j) const
    {
        if(m)
        {
            m->output(stream_m, i, j);
            delete m;
            m = 0;
        }
    }

    template <typename B, typename Q>
    void current_matrices (const B& b, const Q& q) const
    {
        B temp(b.nrows(), b.ncols() + q.ncols(), fromsize);

        for(std::size_t i = 0; i < b.nrows(); ++i)
        {
            for(std::size_t j = 0; j < b.ncols(); ++j)
                temp(i, j) = b(i, j);
            for(std::size_t j = 0; j < q.ncols(); ++j)
                temp(i, j + b.ncols()) = q(i, j);
        }

        //stream << " $$";
        //matrix_line v(b.ncols());
        //output_latex_matrix_frames(stream, temp, make_matrix_vers(&v, &v + 1), true);
        //stream << "$$\n";

        flush_matrix();
        flush_buffer();
        m = new Custom_matrix_wrapper<B>(temp, b.ncols());

        /*output_aligned_hor_pair(stream, b, q);*/
    }

public:

    rref_latexlog
    (
        Stream& stream_a,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        rref_slog<Stream>(stream_a, preamble_on_a, conclusion_on_a),
        m(0)
    {}

    ~rref_latexlog ()
    {
        delete m;
    }

    template <typename A>
    void preamble (const A& a) const
    {
        if(!preamble_on)return;

        title();
        stream() << "\n$$";
        output_latex(stream(), a, true);
        stream() << "$$\n";
        finish_preamble();
        stream() << '\n';
    }

    template <typename B, typename Q, typename Det, typename Basis>
    void conclusion
    (
        const B& b,
        const Q& q,
        const Det& det,
        const Basis& basis
    ) const
    {
        if(conclusion_on)
        {
            current_matrices(b, q);
            begin_conclusion();
            rref_name();
            output_latex(stream() << "\n$$", b, true);
            stream() << "$$";
            inverse_name();
            output_latex(stream() << "\n$$", q, true);
            stream() << "$$";
        }
        else
        {
            current_matrices(b, q);
        }

        basis_name();
        output_latex(stream() << " ", basis+1, false, eep_alone, true);
        stream() << ".\n";
        det_name();
        stream() << " $" << det << "$.\n";

        flush_matrix();
        flush_buffer();
    }


    template <typename B, typename Q, typename Det, typename Basis>
    void before_iter
    (
        const B& b,
        const Q& q,
        const Det& det,
        const Basis& basis
    ) const
    {
        current_matrices(b, q);
        //output_aligned_hor_pair(stream, b, q);
        basis_name();
        output_latex(stream() << " ", basis+1, false, eep_alone, true);
        stream() << ".\n";
        det_name();
        stream() << " $" << det << "$.\n";
    }

    template <typename B, typename Q, typename Det, typename Basis>
    void after_iter
    (
        const B& b,
        const Q& q,
        const Det& det,
        const Basis& basis
    ) const
    {}

    template <typename J>
    void find_biggest_in_col (const J& j) const
    {
        find_biggest_in_col_name();
        stream() << " $" << j+1 << "$.\n";
    }

    template <typename J>
    void negligible_col (const J& j) const
    {
        col_name();
        stream() << " $" << j+1 << "$ ";
        is_negligible_name();
        stream() << ".\n";
    }

    template <typename I, typename J>
    void pivot_item (const I& i, const J& j) const
    {
        flush_matrix(i, j);
        pivot_item_name();
        stream() << " $(" << i+1 << "$, $" << j+1 << ")$.\n";
        flush_buffer();
    }

    template <typename I1, typename I2, typename B, typename Q, typename Det>
    void swap_rows
    (
        const I1& i1,
        const I2& i2,
        const B& b,
        const Q& q,
        const Det& det
    ) const
    {
        swap_rows_name();
        stream() << " $" << i1+1 << "$, $" << i2+1 << "$.\n";
        current_matrices(b, q);
        det_name();
        stream() << " $" << det << "$.\n";
    }

    template <typename J>
    void eliminate_col (const J& j) const
    {
        eliminate_col_name();
        stream() << " $" << j+1 << "$.\n";
    }

    template <typename B, typename Q, typename Det>
    void after_elimination (const B& b, const Q& q, const Det& det) const
    {
        //current_matrices(b, q);
        //det_name();
        //stream() << det << '\n';
    }

protected:

    virtual std::ostream& stream () const
    {
        return buffer.value();
    }
};


template <typename Stream>
inline rref_latexlog<Stream> make_rref_latexlog (Stream& stream)
{
    return rref_latexlog<Stream>(stream);
}

#endif

namespace simplex_method
{


inline const matrix_frames<const matrix_line*, const matrix_line*>& table_frames ()
{
    static const matrix_line hf(1), vf(1);
    static const matrix_frames<const matrix_line*, const matrix_line*> frames =
        make_matrix_frames(&hf, &hf + 1, &vf, &vf + 1);
    return frames;
}


/// TEMPORARY IMPLEMENTATION!!!
template <typename Stream>
struct primal_row_iters_latexlog :
    public primal_row_iters_slog<Stream>
{
protected:

    using primal_row_iters_slog<Stream>::stream;

public:

    class abort :
        public primal_row_iters_slog<Stream>::abort
    {};

    primal_row_iters_latexlog
    (
        Stream& stream_a,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        primal_row_iters_slog<Stream>
        (
            stream_a,
            preamble_on_a,
            conclusion_on_a
        )
    {}

    template <typename Q, typename Basis>
    void preamble (const Q& q, const Basis& basis) const
    {
        //stream << "\n\\begin{verbatim}\n";
        //primal_row_iters_slog<Stream>::preamble(q, basis);
    }

    template <typename Q, typename Basis>
    void conclusion (const Q& q, const Basis& basis, result_kind rk) const
    {
        //primal_row_iters_slog<Stream>::conclusion(q, basis, rk);
        //stream << "\n\\end{verbatim}\n";
    }

    template <typename Q, typename Basis>
    void before_iter (const Q& q, const Basis& basis) const
    {
        stream << "\n$$\n";
        output_latex_matrix_frames(stream, q, table_frames(), true);
        stream << "\n$$\n";
        stream << basis_name() << " $" << basis << "$. \n";
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

    virtual const char* basis_name () const
    {
        return "$\\cal B = $";
    }

    virtual const char* pivot_col_name () const
    {
        return "Текущий столбец: ";
    }

    virtual const char* pivot_item_name () const
    {
        return "Текущий элемент: ";
    }

private:

    //matrix<rational<> > qbuf;
    //vector<std::size_t> basisbuf;

};


/// TEMPORARY SOLUTION
template <typename Stream>
struct basis_create_by_artificial_latexlog :
    public basis_create_by_artificial_slog<Stream>
{
    class abort :
        public basis_create_by_artificial_slog<Stream>
    {};

    basis_create_by_artificial_latexlog
    (
        Stream& stream_a,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        basis_create_by_artificial_slog<Stream>
        (
            stream_a,
            preamble_on_a,
            conclusion_on_a
        )
    {}

    template <typename Q, typename Basis>
    void before_erase_artif (const Q& q, const Basis& basis) const
    {
        //output_aligned(stream, q);
        //stream << basis_name() << ' ' << basis << '\n';
    }
};



template <typename Stream>
struct basis_artificial_latexlog :
    public basis_artificial_slog<Stream>
{
protected:

    using basis_artificial_slog<Stream>::stream;

public:

    class abort :
        public basis_artificial_slog<Stream>::abort
    {};

    basis_artificial_latexlog
    (
        Stream& stream_a,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        basis_artificial_slog<Stream>
        (
            stream_a,
            preamble_on_a,
            conclusion_on_a
        )
    {}

    template <typename Q>
    void preamble (const Q& q) const
    {
        //stream << "\n\\begin{verbatim}\n";
        //basis_artificial_slog<Stream>::preamble(q);
    }

    template <typename Q, typename Basis>
    void conclusion (const Q& q, const Basis& basis, result_kind rk) const
    {
        //basis_artificial_slog<Stream>::conclusion(q, basis, rk);
        //stream << "\n\\end{verbatim}\n";
    }

    primal_row_iters_latexlog<Stream> ctrl_primal_row_iters () const
    {
        return primal_row_iters_latexlog<Stream>(stream, false, false);
    }

    basis_create_by_artificial_latexlog<Stream>
    ctrl_basis_create_by_artificial () const
    {
        return basis_create_by_artificial_latexlog<Stream>
            (stream, false, false);
    }

protected:

    virtual const char* table_with_artif_name () const
    {
        return "Таблица с искуственными переменными";
    }

};


/// TEMPORARY IMPLEMENTATION!!!
template <typename Stream>
struct dual_col_iter_latexlog : public dual_col_iter_slog<Stream>
{
protected:

    using dual_col_iter_slog<Stream>::stream;

public:

    //Stream& stream;

    dual_col_iter_latexlog (Stream& stream_a) :
        dual_col_iter_slog<Stream>(stream_a)
    {}

    void preamble () const
    {}

    void conclusion () const
    {}

    template <typename Q, typename Nonbasis>
    void current_table (const Q& q, const Nonbasis& nonbasis) const
    {
        stream << "\n$$\n";
        output_latex_matrix_frames(stream, q, table_frames(), true);
        stream << "\n$$\n";
        //stream << "$$\cal N = " << nonbasis << "$$.\n";
    }

    template <typename R, typename S>
    void pivot_item (const R& r, const S& s) const
    {
        stream << "Текущий элемент $" << r << ", " << s << "$.\n";
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


/// TEMPORARY IMPLEMENTATION!!!
template <typename Stream>
struct dual_col_iters_latexlog :
    public dual_col_iters_slog<Stream>
{
protected:

    using dual_col_iters_slog<Stream>::stream;

public:

    //Stream& stream;

    dual_col_iters_latexlog
    (
        Stream& stream_a,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        dual_col_iters_slog<Stream>(stream_a)/*,
        preamble_on(preamble_on_a),
        conclusion_on(conlusion_on_a)*/
    {}

    void conclusion () const
    {
        /*stream << "\n\\end{verbatim}\n";*/
    }

    dual_col_iter_latexlog<Stream> iter_ctrl () const
    {
        return dual_col_iter_latexlog<Stream>(stream);
    }

    template <typename TQ, typename Tb>
    bool stop (const TQ&, const Tb&) const
    {
        return false;
    }
};


/// TEMPORARY IMPLEMENTATION!!!
template <typename Stream>
struct gomory1_iter_latexlog :
    public gomory1_iter_slog<Stream>
{
protected:

    using gomory1_iter_slog<Stream>::stream;

public:

    class abort :
        public gomory1_iter_slog<Stream>::abort
    {};

    //Stream& stream;
    //bool preamble_on, conclusion_on;

    gomory1_iter_latexlog
    (
        Stream& stream_a,
        bool preamble_on_a = true,
        bool conclusion_on_a = true
    ) :
        gomory1_iter_slog<Stream>
        (
            stream_a,
            preamble_on_a,
            conclusion_on_a
        )
    {}

    template <typename T, typename Prow>
    void after_gomory1_clip (const T& t, Prow prow, result_kind rk) const
    {
        if(rk == rk_nonoptimal)
            stream << "Номер производящей строки " << prow << ".\n";
    }

    dual_col_iters_latexlog<Stream> ctrl_for_dual_col_iters () const
    {
        return dual_col_iters_latexlog<Stream>(stream);
    }
};


/// TEMPORARY IMPLEMENTATION!!!
template <typename Stream>
struct gomory1_iters_latexlog :
    public gomory1_iters_idler
{
    class abort :
        public gomory1_iters_idler::abort
    {};

    Stream& stream;
    bool preamble_on, conclusion_on;

    gomory1_iters_latexlog
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
    {
        /*stream << "\n\\begin{verbatim}\n";*/
        output_latex_matrix_frames(stream, t, table_frames());
    }

    template <typename T, typename Nonbasis>
    void conclusion (const T& t, const Nonbasis& nonbasis, result_kind rk) const
    {
        /*stream << "\n\\end{verbatim}\n";*/
    }

    gomory1_iter_latexlog<Stream> ctrl_for_gomory1_iter () const
    {
        return gomory1_iter_latexlog<Stream>(stream);
    }
};



} // namespace simplex_method
} // namespace ctrl
} // namespace Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_CTRL_LATEXLOG
    //#include "ctrl_latexlog.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_CTRL_LATEXLOG
#endif


#endif
