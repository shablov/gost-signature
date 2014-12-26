/*****************************************************************************

    texout.hpp

    This file is a part of the Arageli library.

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
    \file texout.hpp
    \brief Routines for output in LaTeX.
*/


#ifndef _ARAGELI_texout_hpp_
#define _ARAGELI_texout_hpp_

#include "config.hpp"

#include <cstddef>
#include <iostream>
#include <sstream>

#include "big_int.hpp"
#include "rational.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "sparse_polynom.hpp"

#include "std_import.hpp"


namespace Arageli
{

/// Type of environment (context) for an expression.
/** These values helps to bracket placing in nested expressions. */
enum ex_environ_priority
{
    eep_alone,    ///< an expression is alone (matrix or vector item, power of another expression and so on)
    eep_add,    /// additive context (monom in polynomial and so on)
    eep_mul        /// multiplicative context (coefficient of monom with degree that greater than 0 and so on)
};

/// Verbativ output in LaTeX.
/** Uses \verb definition. */
template <typename Out, typename T>
inline void output_latex_verb
(
    Out& out,
    const T& x,
    bool in_math = false,
    ex_environ_priority eep = eep_alone
)
{
    out << "\\verb\"" << x << '"';
}

/// Outputs the object in LaTeX notation.  Common case.
/** @param out the stream which the function outputs to
    @param x object for outputting
    @param in_math indicates if there is already
        environment of mathematical formula (have effect on $ placing).
    @param eep an environment, see ex_environ_priority    */
template <typename Out, typename T>
inline void output_latex
(
    Out& out,
    const T& x,
    bool in_math = false,
    ex_environ_priority eep = eep_alone
)
{
    output_latex_verb(out, x, in_math, eep);
}


/// Encloses the standard output of x in $ brackets.
template <typename Out, typename T>
inline void output_latex_math
(
    Out& out,
    const T& x,
    bool in_math = false,
    ex_environ_priority eep = eep_alone
)
{
    std::ostringstream buf;    // WARNIGN! OUTPUTTING THROUGH BUFFER.
    if(!in_math)
        buf << '$' << x << '$';
    else buf << x;
    out << buf.str();
}


#define ARAGELI_TEXOUT_NUMBER(TYPE)    \
    template <typename Out>    \
    inline void output_latex    \
    (    \
        Out& out,    \
        const TYPE& x,    \
        bool in_math = false,    \
        ex_environ_priority eep = eep_alone    \
    )    \
    {    \
        output_latex_math(out, x, in_math, eep);    \
    }


/// @name LaTeX output for built-in types and big_int

//@{

ARAGELI_TEXOUT_NUMBER(signed int)
ARAGELI_TEXOUT_NUMBER(unsigned int)
ARAGELI_TEXOUT_NUMBER(signed short int)
ARAGELI_TEXOUT_NUMBER(unsigned short int)
ARAGELI_TEXOUT_NUMBER(signed long int)
ARAGELI_TEXOUT_NUMBER(unsigned long int)
ARAGELI_TEXOUT_NUMBER(float)
ARAGELI_TEXOUT_NUMBER(double)
ARAGELI_TEXOUT_NUMBER(long double)
ARAGELI_TEXOUT_NUMBER(big_int)

#ifdef ARAGELI_INT64_SUPPORT
    ARAGELI_TEXOUT_NUMBER(signed __int64)
    ARAGELI_TEXOUT_NUMBER(unsigned __int64)
#endif

#ifdef ARAGELI_LONG_LONG_SUPPORT
    ARAGELI_TEXOUT_NUMBER(signed long long)
    ARAGELI_TEXOUT_NUMBER(unsigned long long)
#endif

//@}

#undef ARAGELI_TEXOUT_NUMBER


/// Outputs a rational in LaTeX notation.
/**    @param minlensimple a minimum length (in symbols) of notation in LaTeX of
        numerator and denominator of rational when function produces nate with
        slash;  if lenght is greater then this value then the function used
        horizontal line.
    @param externsign if this flag is true then the function takes a minus sign
        (if present) out of fraction;  applicable if only function using
        horizontal line. */
template <typename Out, typename T>
void output_latex
(
    Out& out,
    const rational<T>& x,
    bool in_math,
    ex_environ_priority eep,
    std::size_t minlensimple,
    bool externsign = true
);

template <typename Out, typename T>
inline void output_latex
(
    Out& out,
    const rational<T>& x,
    bool in_math,
    ex_environ_priority eep
)
{
    return output_latex(out, x, in_math, eep, 3, true);
}

template <typename Out, typename T>
inline void output_latex
(
    Out& out,
    const rational<T>& x,
    bool in_math
)
{
    return output_latex(out, x, in_math, eep_alone, 3, true);
}

template <typename Out, typename T>
inline void output_latex
(
    Out& out,
    const rational<T>& x
)
{
    return output_latex(out, x, false, eep_alone, 3, true);
}

/// Outputs a vector in LaTeX notation.
template <typename Out, typename T, bool REFCNT>
void output_latex
(
    Out& out,
    const vector<T, REFCNT>& x,
    bool in_math,
    ex_environ_priority eep,
    bool hor,    // if true, the function produces horizontal output, otherwise is vertical
    const char* first_bracket = "(",
    const char* second_bracket = ")",
    const char* delim = ","
);

template <typename Out, typename T, bool REFCNT>
inline void output_latex
(
    Out& out,
    const vector<T, REFCNT>& x,
    bool in_math,
    ex_environ_priority eep
)
{
    // WARNING! See the default arguments in original function.
    output_latex(out, x, in_math, eep, true, "(", ")", ",");
}

template <typename Out, typename T, bool REFCNT>
inline void output_latex
(
    Out& out,
    const vector<T, REFCNT>& x,
    bool in_math
)
{
    // WARNING! See the default arguments in original function.
    output_latex(out, x, in_math, eep_alone, true, "(", ")", ",");
}

template <typename Out, typename T, bool REFCNT>
inline void output_latex
(
    Out& out,
    const vector<T, REFCNT>& x
)
{
    // WARNING! See the default arguments in original function.
    output_latex(out, x, false, eep_alone, true, "(", ")", ",");
}

/// Outputs a matrix in LaTeX notation.
template <typename Out, typename T, bool REFCNT>
void output_latex
(
    Out& out,
    const matrix<T, REFCNT>& x,
    bool in_math,
    ex_environ_priority eep,
    bool transposed,
    const char* first_bracket = "(",
    const char* second_bracket = ")"
);


template <typename Out, typename T, bool REFCNT>
inline void output_latex
(
    Out& out,
    const matrix<T, REFCNT>& x
)
{
    // WARNING! See the default values for brackets in main function.
    output_latex(out, x, false, eep_alone, false, "(", ")");
}


template <typename Out, typename T, bool REFCNT>
inline void output_latex
(
    Out& out,
    const matrix<T, REFCNT>& x,
    bool in_math
)
{
    // WARNING! See the default values for brackets in main function.
    output_latex(out, x, in_math, eep_alone, false, "(", ")");
}


template <typename Out, typename T, bool REFCNT>
inline void output_latex
(
    Out& out,
    const matrix<T, REFCNT>& x,
    bool in_math,
    ex_environ_priority eep
)
{
    // WARNING! See the default values for brackets in main function.
    output_latex(out, x, in_math, eep, false, "(", ")");
}


/// Outputs a monom in LaTeX notation.
template <typename Out, typename F, typename I>
void output_latex
(
    Out& out,
    const monom<F, I>& x,
    bool in_math,
    ex_environ_priority eep,
    bool first,
    const char* var = "x"
);

template <typename Out, typename F, typename I>
inline void output_latex
(
    Out& out,
    const monom<F, I>& x,
    bool in_math,
    ex_environ_priority eep
)
{
    return output_latex(out, x, in_math, eep, true, "x");
}

template <typename Out, typename F, typename I>
inline void output_latex
(
    Out& out,
    const monom<F, I>& x,
    bool in_math
)
{
    return output_latex(out, x, in_math, eep_alone, true, "x");
}

template <typename Out, typename F, typename I>
inline void output_latex
(
    Out& out,
    const monom<F, I>& x
)
{
    return output_latex(out, x, false, eep_alone, true, "x");
}

/// Outputs a sparse_polynom in LaTeX notation.
template <typename Out, typename F, typename I, bool REFCNT>
void output_latex
(
    Out& out,
    const sparse_polynom<F, I, REFCNT>& x,
    bool in_math,
    ex_environ_priority eep,
    const char* var
);

template <typename Out, typename F, typename I, bool REFCNT>
inline void output_latex
(
    Out& out,
    const sparse_polynom<F, I, REFCNT>& x,
    bool in_math,
    ex_environ_priority eep
)
{
    return output_latex(out, x, in_math, eep, "x");
}

template <typename Out, typename F, typename I, bool REFCNT>
inline void output_latex
(
    Out& out,
    const sparse_polynom<F, I, REFCNT>& x,
    bool in_math
)
{
    return output_latex(out, x, in_math, eep_alone, "x");
}

template <typename Out, typename F, typename I, bool REFCNT>
inline void output_latex
(
    Out& out,
    const sparse_polynom<F, I, REFCNT>& x
)
{
    return output_latex(out, x, false, eep_alone, "x");
}


// -------------------------------

/// Type of a line that used in matrix output functions with boxes and dividing lines.
enum matrix_line_type
{
    mlt_solid,    ///< Solid line   "_______".
    mlt_dot,    ///< Dotted line  "......." (it's not supported for boxes yet).
    mlt_hatch,    ///< Hatched line "_ _ _ _" (it's not supported for boxes yet).
    mlt_chain    ///< Chain line   "_ . _ ." (it's not supported for boxes yet).
};

/// A line descriptor for matrix output.
/** Determines the line as its position and type. */
struct matrix_line
{
    /// Not initialized object.
    matrix_line ()
    {}

    matrix_line (std::size_t pos_a, matrix_line_type mlt_a = mlt_solid) :
        pos(pos_a),
        mlt(mlt_a)
    {}

    std::size_t pos;    ///< Line position: before this row or column line will be inserted.
    matrix_line_type mlt;    ///< Type of a line.
};


/// A box (or frame) descriptor for matrix output.
/**    Determines the box as its row, column and type. */
struct matrix_box
{
    /// Not initialized object.
    matrix_box ()
    {}

    matrix_box (std::size_t row_a, std::size_t col_a, matrix_line_type mlt_a = mlt_solid) :
        row(row_a),
        col(col_a),
        mlt(mlt_a)
    {}

    std::size_t
        row,    ///< Box position: row number of an element with frame around.
        col;    ///< Box position: col number of an element with frame around.
    matrix_line_type mlt;    ///< Type of a line in frame.
};


/// Contains information about horizontals and verticals lines and boxes for matrix output.
template
<
    typename In_hor = const matrix_line*,
    typename In_ver = const matrix_line*,
    typename In_box = const matrix_box*
>
struct matrix_frames
{
    matrix_frames
    (
        In_hor hf = 0,
        In_hor hl = 0,
        In_ver vf = 0,
        In_ver vl = 0,
        In_box bf = 0,
        In_box bl = 0
    ) :
        hors_first(hf),
        hors_last(hl),
        vers_first(vf),
        vers_last(vl),
        boxes_first(bf),
        boxes_last(bl)
    {}

    In_hor
        hors_first,    ///< Begin of the sequence with horizontal lines.
        hors_last;    ///< End of the sequence with horizontal lines.
    In_ver
        vers_first,    ///< Begin of the sequence with vertical lines.
        vers_last;    ///< End of the sequence with vertical lines.
    In_box
        boxes_first,    ///< Begin of the sequence with boxes.
        boxes_last;    ///< End of the sequence with boxes.
};


/// @name Matrix frames maker functions.
/** Helper functions that creates matrix_frames with appropriate parameters. */
// @{

template <typename In_hor, typename In_ver, typename In_box>
inline matrix_frames<In_hor, In_ver, In_box> make_matrix_frames
(
    In_hor hf,
    In_hor hl,
    In_ver vf,
    In_ver vl,
    In_box bf,
    In_box bl
)
{
    return matrix_frames<In_hor, In_ver, In_box>(hf, hl, vf, vl, bf, bl);
}


template <typename In_hor, typename In_ver>
inline matrix_frames<In_hor, In_ver> make_matrix_frames
(
    In_hor hf,
    In_hor hl,
    In_ver vf,
    In_ver vl
)
{
    return matrix_frames<In_hor, In_ver>(hf, hl, vf, vl);
}


template <typename In_hor>
inline matrix_frames<In_hor> make_matrix_frames
(
    In_hor hf,
    In_hor hl
)
{
    return matrix_frames<In_hor>(hf, hl);
}


inline matrix_frames<> make_matrix_frames ()
{
    return matrix_frames<>();
}


template <typename In_ver>
inline matrix_frames<const matrix_line*, In_ver> make_matrix_vers
(
    In_ver vf,
    In_ver vl
)
{
    return matrix_frames<const matrix_line*, In_ver>(0, 0, vf, vl);
}


template <typename In_box>
inline matrix_frames<const matrix_line*, const matrix_line*, In_box>
make_matrix_boxes
(
    In_box bf,
    In_box bl
)
{
    return matrix_frames<const matrix_line*, const matrix_line*, In_box>
        (0, 0, 0, 0, bf, bl);
}


template <typename In_ver, typename In_box>
inline matrix_frames<const matrix_line*, In_ver, In_box> make_matrix_vers_boxes
(
    In_ver vf,
    In_ver vl,
    In_box bf,
    In_box bl
)
{
    return matrix_frames<const matrix_line*, In_ver, In_box>
        (0, 0, vf, vl, bf, bl);

}

//@}


/// Outputs a matrix in LaTeX notation with lines and boxes.
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
    const matrix<T, REFCNT>& x,
    const matrix_frames<In_hor, In_ver, In_box>& mf,
    bool in_math = false,
    bool transposed = false,
    const char* first_bracket = "(",
    const char* second_bracket = ")"
);

}


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_TEXOUT
    #include "texout.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_TEXOUT
#endif


#endif  //  #ifndef _ARAGELI_texout_hpp_
