/*****************************************************************************

    texout.cpp -- See description in file texout.hpp.

    This file is a part of Arageli library.

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

#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_TEXOUT)

#include <cstddef>
#include <sstream>

#include "texout.hpp"


namespace Arageli
{


template <typename Out, typename T>
void output_latex
(
    Out& out,
    const rational<T>& x,
    bool in_math,
    ex_environ_priority eep,
    std::size_t maxlensimple,
    bool externsign
)
{
    if(!in_math)
        out << '$';

    if(x.is_null())
        out << '0';
    else if(is_unit(x.denominator()))
        output_latex(out, x.numerator(), true, eep);
    else
    {
        std::ostringstream buf;
        std::string sn, sd;
        bool minus = false;

        if(externsign && is_negative(x.numerator()))
        {
            minus = true;
            output_latex(buf, -x.numerator(), true, eep_mul);
        }
        else
            output_latex(buf, x.numerator(), true, eep_mul);

        sn = buf.str();
        buf.str(std::string());

        output_latex(buf, x.denominator(), true, eep_mul);
        sd = buf.str();
        buf.str(std::string());

        if(sn.length() + sd.length() > maxlensimple)
        {
            if(minus)
                out << '-';
            out << "\\frac{";
            output_latex(out, minus ? -x.numerator() : x.numerator(), true, eep_alone);
            out << "}{";
            output_latex(out, x.denominator(), true, eep_alone);
            out << "}";
        }
        else if(eep == eep_mul)
        {
            out << '(';
            if(minus)
                out << '-';
            out << sn << '/' << sd << ')';
        }
        else
        {
            if(minus)
                out << '-';
            out << sn << '/' << sd;
        }
    }

    if(!in_math)
        out << '$';
}


template <typename Out, typename T, bool REFCNT>
void output_latex
(
    Out& out,
    const vector<T, REFCNT>& x,
    bool in_math,
    ex_environ_priority eep,
    bool hor,
    const char* first_bracket,
    const char* second_bracket,
    const char* delim
)
{
    if(!in_math)
        out << '$';

    out << "\\left" << first_bracket;

    if(!x.is_empty())
        if(hor)
        {
            output_latex(out, x[0], true);
            for(typename vector<T, REFCNT>::size_type i = 1; i < x.size(); ++i)
                output_latex(out << delim, x[i], true);
        }
        else    // ver
        {
            out << "\\begin{tabular}{c}";

            output_latex(out, x[0], false);
            for(typename vector<T, REFCNT>::size_type i = 1; i < x.size(); ++i)
                output_latex(out << " \\\\[3pt] ", x[i], false);

            out << "\\end{tabular}";
        }

    out << "\\right" << second_bracket;

    if(!in_math)
        out << '$';
}


template <typename Out, typename T, bool REFCNT>
void output_latex
(
    Out& out,
    const matrix<T, REFCNT>& xx,
    bool in_math,
    ex_environ_priority eep,
    bool transposed,
    const char* first_bracket,
    const char* second_bracket
)
{
    if(!in_math)
        out << '$';

    matrix<T, REFCNT> x = xx;
    if(transposed)
        x.transpose();

    out << "\\left" << first_bracket;

    if(!x.is_empty())
    {
        out
            << "\\begin{tabular}{"
            << std::string(x.ncols(), 'c')
            << "}";

        typedef typename vector<T, REFCNT>::size_type size_type;

        output_latex(out, x(0, 0), false);
        for(size_type j = 1; j < x.ncols(); ++j)
            output_latex(out << " & ", x(0, j), false);

        for(size_type i = 1; i < x.nrows(); ++i)
        {
            output_latex(out << " \\\\[3pt] ", x(i, 0), false);
            for(size_type j = 1; j < x.ncols(); ++j)
                output_latex(out << " & ", x(i, j), false);
        }

        out << "\\end{tabular}";
    }

    out << "\\right" << second_bracket;

    if(!in_math)
        out << '$';
}


template <typename Out, typename F, typename I>
void output_latex
(
    Out& out,
    const monom<F, I>& x,
    bool in_math,
    ex_environ_priority eep,
    bool first,
    const char* var
)
{
    if(!in_math)
        out << '$';

    if(x.is_const())
        if(first)
            output_latex(out, x.coef(), true, eep);
        else
        {
            if(is_negative(x.coef()))
                output_latex(out << '-', -x.coef(), true, eep);
            else
                output_latex(out << '+', x.coef(), true, eep);
        }
    else
    {
        if(is_opposite_unit(x.coef()))
            out << '-';
        else if(is_unit(x.coef()))
        {
            if(!first)
                out << '+';
        }
        else if(first)
        {
            output_latex(out, x.coef(), true, eep_mul);
        }
        else
        {
            if(is_negative(x.coef()))
                output_latex(out << '-', -x.coef(), true, eep_mul);
            else
                output_latex(out << '+', x.coef(), true, eep_mul);
        }

        out << var;
        if(!is_unit(x.degree()))
        {
            out << "^{";
            output_latex(out, x.degree(), true);
            out << "}";
        }
    }

    if(!in_math)out << '$';
}


template <typename Out, typename F, typename I, bool REFCNT>
void output_latex
(
    Out& out,
    const sparse_polynom<F, I, REFCNT>& x,
    bool in_math,
    ex_environ_priority eep,
    const char* var
)
{
    typedef typename
        std::reverse_iterator
            <typename sparse_polynom<F, I, REFCNT>::monom_const_iterator>
        Ri;
    Ri
        begin = Ri(x.monoms_end()),
        end = Ri(x.monoms_begin());

    if(!in_math)
        out << '$';
    if(begin == end)
        out << '0';
    else
    {
        ++begin;
        if(begin == end)
        {
            --begin;
            output_latex(out, *begin, true, eep, true, var);
        }
        else
        {
            --begin;
            if(eep == eep_mul)
                out << "\\left(";
            output_latex(out, *begin, true, eep_add, true, var);

            for(++begin; begin != end; ++begin)
                output_latex(out, *begin, true, eep_add, false, var);

            if(eep == eep_mul)
                out << "\\right)";
        }
    }
    if(!in_math)
        out << '$';
}


namespace _Internal
{

template <typename Stream>
void mlt_output (matrix_line_type mlt, Stream& out, bool ver = false)
{
    switch(mlt)
    {
        case mlt_solid:
            if(ver)
                out << '|';
            else
                out << '-';
            break;
        case mlt_dot:
            out << '.';
            break;
        case mlt_hatch:
            out << ':';
            break;
        case mlt_chain:
            out << ';';
            break;
        default:
            ARAGELI_ASSERT_ALWAYS(!"Not appropriate value of mtl.");
    }
}

}

template
<
    typename T,
    bool REFCNT,
    typename Ch,
    typename ChT,
    typename In_hor,
    typename In_ver,
    typename In_box
>
void output_latex_matrix_frames
(
    std::basic_ostream<Ch, ChT>& out,
    const matrix<T, REFCNT>& xx,
    const matrix_frames<In_hor, In_ver, In_box>& mf,
    bool in_math,
    bool transposed,
    const char* first_bracket,
    const char* second_bracket
)
{
    if(!in_math)
        out << '$';

    matrix<T, REFCNT> x = xx;
    if(transposed)
        x.transpose();

    out << "\\left" << first_bracket;

    if(!x.is_empty())
    {
        out << "\\begin{MAT}(b,0.7cm,0.7cm){";

        In_ver curver = mf.vers_first;
        for(std::size_t i = 0; i < x.ncols(); ++i)
        {
            if(curver != mf.vers_last && curver->pos == i)
            {
                _Internal::mlt_output(curver->mlt, out, true);
                ++curver;
            }

            out << 'c';
        }

        if(curver != mf.vers_last && curver->pos == x.ncols())
        {
            _Internal::mlt_output(curver->mlt, out, true);
            ++curver;
        }

        out << "}\n";

        typedef typename vector<T, REFCNT>::size_type size_type;

        In_hor curhor = mf.hors_first;

        // It's not supported yet.
        ARAGELI_ASSERT_0(!(curhor != mf.hors_last && curhor->pos == 0));
        /*
        if(curhor != mf.hors_last && curhor->pos == 0)
        {
            out << "\\hline ";
            ++curhor;
        }
        */

        In_box curbox = mf.boxes_first;

        if(curbox != mf.boxes_last && curbox->row == 0 && curbox->col == 0)
        {
            out << "\\fbox{";
            output_latex(out, x(0, 0), true);
            out << "}";
            ++curbox;
        }
        else
            output_latex(out, x(0, 0), true);

        for(size_type j = 1; j < x.ncols(); ++j)
            if(curbox != mf.boxes_last && curbox->row == 0 && curbox->col == j)
            {
                out << " & \\fbox{";
                output_latex(out, x(0, j), true);
                out << "}";
                ++curbox;
            }
            else
                output_latex(out << " & ", x(0, j), true);

        for(size_type i = 1; i < x.nrows(); ++i)
        {
            //out << " \\\\[5pt] ";
            out << " \\\\";

            if(curhor != mf.hors_last && curhor->pos == i)
            {
                //out << "\\hline ";
                _Internal::mlt_output(curhor->mlt, out);
                ++curhor;
            }

            out << '\n';

            if(curbox != mf.boxes_last && curbox->row == i && curbox->col == 0)
            {
                out << "\\fbox{";
                output_latex(out, x(i, 0), true);
                out << "}";
                ++curbox;
            }
            else
                output_latex(out, x(i, 0), true);

            for(size_type j = 1; j < x.ncols(); ++j)
                if(curbox != mf.boxes_last && curbox->row == i && curbox->col == j)
                {
                    out << " & \\fbox{";
                    output_latex(out, x(i, j), true);
                    out << "}";
                    ++curbox;
                }
                else
                    output_latex(out << " & ", x(i, j), true);
        }

        out << " \\\\";

        if(curhor != mf.hors_last && curhor->pos == x.ncols())
        {
            //out << "\\hline ";
            _Internal::mlt_output(curhor->mlt, out);
            ++curhor;
        }

        out << "\n\\end{MAT}";
    }

    out << "\\right" << second_bracket;

    if(!in_math)
        out << '$';
}


}

#endif // #ifndef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
