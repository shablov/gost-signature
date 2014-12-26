/*****************************************************************************

    iomanip.hpp -- input/output manipulators.

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
    \file iomanip.hpp

    This file implements input/output manipulators for each Arageli main
    structure and time of input/output.
*/


#ifndef _ARAGELI_iomanip_hpp_
#define _ARAGELI_iomanip_hpp_

#include "config.hpp"

#include "sparse_polynom.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "io.hpp"


namespace Arageli
{

/// Manipulators for input/output from/to a stream.
/** Be carefull, an implementation is restricted. */
namespace iomanip
{

#define ARAGELI_IOMANIP_BINDER_PART1(NAME, MANIP, DIR)    \
    NAME    \
    (    \
        std::basic_##DIR##stream<Ch, ChT>& stream_a,    \
        const MANIP* manip_a    \
    ) :    \
        stream(stream_a),    \
        manip(manip_a)    \
    {}    \
    \
    std::basic_##DIR##stream<Ch, ChT>& stream;    \
    const MANIP* manip;

#define ARAGELI_IOMANIP_MANIP_PART1(MANIP, BINDER, DIR, DIRSYMB)    \
    template <typename Ch, typename ChT>    \
    inline BINDER<Ch, ChT> operator DIRSYMB    \
    (    \
        std::basic_##DIR##stream<Ch, ChT>& stream,    \
        const MANIP& x    \
    )    \
    {    \
        return BINDER<Ch, ChT>(stream, &x);    \
    }


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct monom_output_list;

template <typename Ch, typename ChT>
struct monom_output_list_binder
{
    ARAGELI_IOMANIP_BINDER_PART1
    (
        monom_output_list_binder,
        monom_output_list,
        o
    );

    template <typename F, typename I>
    std::basic_ostream<Ch, ChT>& operator<< (const monom<F, I>& x) const;
};

struct monom_output_list
{
    const char
        *first_bracket,
        *second_bracket,
        *separator;

    monom_output_list
    (
        const char* fb_a = monom_output_list_first_bracket_default,
        const char* sb_a = monom_output_list_second_bracket_default,
        const char* sep_a = monom_output_list_separator_default
    ) :
        first_bracket(fb_a),
        second_bracket(sb_a),
        separator(sep_a)
    {}
};

ARAGELI_IOMANIP_MANIP_PART1
(
    monom_output_list,
    monom_output_list_binder,
    o,
    <<
);

template <typename Ch, typename ChT>
template <typename F, typename I>
inline std::basic_ostream<Ch, ChT>&
monom_output_list_binder<Ch, ChT>::operator<< (const monom<F, I>& x) const
{
    return output_list
    (
        stream,
        x,
        manip->first_bracket,
        manip->second_bracket,
        manip->separator
    );
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct monom_input_list;

template <typename Ch, typename ChT>
struct monom_input_list_binder
{
    ARAGELI_IOMANIP_BINDER_PART1
    (
        monom_input_list_binder,
        monom_input_list,
        i
    );

    template <typename F, typename I>
    std::basic_istream<Ch, ChT>& operator>> (monom<F, I>& x) const;

};

struct monom_input_list
{
    const char
        *first_bracket,
        *second_bracket,
        *separator;

    monom_input_list
    (
        const char* fb_a = monom_input_list_first_bracket_default,
        const char* sb_a = monom_input_list_second_bracket_default,
        const char* sep_a = monom_input_list_separator_default
    ) :
        first_bracket(fb_a),
        second_bracket(sb_a),
        separator(sep_a)
    {}
};

ARAGELI_IOMANIP_MANIP_PART1
(
    monom_input_list,
    monom_input_list_binder,
    i,
    >>
);

template <typename Ch, typename ChT>
template <typename F, typename I>
inline std::basic_istream<Ch, ChT>&
monom_input_list_binder<Ch, ChT>::operator>> (monom<F, I>& x) const
{
    return input_list
    (
        stream,
        x,
        manip->first_bracket,
        manip->second_bracket,
        manip->separator
    );
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct monom_output_var;

template <typename Ch, typename ChT>
struct monom_output_var_binder
{
    ARAGELI_IOMANIP_BINDER_PART1
    (
        monom_output_var_binder,
        monom_output_var,
        o
    );

    template <typename F, typename I>
    std::basic_ostream<Ch, ChT>& operator<< (const monom<F, I>& x) const;

};

struct monom_output_var
{
    const char *var, *mul, *pow;
    bool first;

    monom_output_var
    (
        bool first_a = true,
        const char* var_a = monom_output_var_var_default,
        const char* mul_a = monom_output_var_mul_default,
        const char* pow_a = monom_output_var_pow_default
    ) :
        var(var_a),
        mul(mul_a),
        pow(pow_a),
        first(first_a)
    {}
};

ARAGELI_IOMANIP_MANIP_PART1
(
    monom_output_var,
    monom_output_var_binder,
    o,
    <<
);

template <typename Ch, typename ChT>
template <typename F, typename I>
inline std::basic_ostream<Ch, ChT>&
monom_output_var_binder<Ch, ChT>::operator<< (const monom<F, I>& x) const
{
    return output_var
    (
        stream,
        x,
        manip->first,
        manip->var,
        manip->mul,
        manip->pow
    );
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct monom_input_var;

template <typename Ch, typename ChT>
struct monom_input_var_binder
{
    ARAGELI_IOMANIP_BINDER_PART1
    (
        monom_input_var_binder,
        monom_input_var,
        i
    );

    template <typename F, typename I>
    std::basic_istream<Ch, ChT>& operator>> (monom<F, I>& x) const;
};

struct monom_input_var
{
    const char *var, *mul, *pow;
    bool first;

    monom_input_var
    (
        bool first_a = true,
        const char* var_a = monom_input_var_var_default,
        const char* mul_a = monom_input_var_mul_default,
        const char* pow_a = monom_input_var_pow_default
    ) :
        var(var_a),
        mul(mul_a),
        pow(pow_a),
        first(first_a)
    {}
};

ARAGELI_IOMANIP_MANIP_PART1
(
    monom_input_var,
    monom_input_var_binder,
    i,
    >>
);

template <typename Ch, typename ChT>
template <typename F, typename I>
inline std::basic_istream<Ch, ChT>&
monom_input_var_binder<Ch, ChT>::operator>> (monom<F, I>& x) const
{
    return input_var
    (
        stream,
        x,
        manip->first,
        manip->var,
        manip->mul,
        manip->pow
    );
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct monom_output_aligned;

template <typename Ch, typename ChT>
struct monom_output_aligned_binder
{
    ARAGELI_IOMANIP_BINDER_PART1
    (
        monom_output_aligned_binder,
        monom_output_aligned,
        o
    );

    template <typename F, typename I>
    std::basic_ostream<Ch, ChT>& operator<< (const monom<F, I>& x) const;
};

struct monom_output_aligned
{
    const char *var, *mul, *pow;
    bool first;

    monom_output_aligned
    (
        bool first_a = true,
        const char* var_a = monom_output_var_var_default,
        const char* mul_a = monom_output_var_mul_default,
        const char* pow_a = monom_output_var_pow_default
    ) :
        var(var_a),
        mul(mul_a),
        pow(pow_a),
        first(first_a)
    {}
};

ARAGELI_IOMANIP_MANIP_PART1(monom_output_aligned, monom_output_aligned_binder, o, <<);

template <typename Ch, typename ChT>
template <typename F, typename I>
inline std::basic_ostream<Ch, ChT>&
monom_output_aligned_binder<Ch, ChT>::operator<< (const monom<F, I>& x) const
{
    return output_aligned
    (
        stream,
        x,
        manip->first,
        manip->var,
        manip->mul,
        manip->pow
    );
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct sparse_polynom_output_list;

template <typename Ch, typename ChT>
struct sparse_polynom_output_list_binder
{
    ARAGELI_IOMANIP_BINDER_PART1
    (
        sparse_polynom_output_list_binder,
        sparse_polynom_output_list,
        o
    );

    template <typename F, typename I, bool REFCNT>
    std::basic_ostream<Ch, ChT>& operator<<
    (const sparse_polynom<F, I, REFCNT>& x) const;

};

struct sparse_polynom_output_list
{
    Arageli::monoms_order monoms_order;
    const char
        *first_bracket,
        *second_bracket,
        *separator;

    sparse_polynom_output_list
    (
        Arageli::monoms_order mo = mo_inc,
        const char* fb_a = monom_output_list_first_bracket_default,
        const char* sb_a = monom_output_list_second_bracket_default,
        const char* sep_a = monom_output_list_separator_default
    ) :
        monoms_order(mo),
        first_bracket(fb_a),
        second_bracket(sb_a),
        separator(sep_a)
    {}
};

ARAGELI_IOMANIP_MANIP_PART1
(
    sparse_polynom_output_list,
    sparse_polynom_output_list_binder,
    o,
    <<
);

template <typename Ch, typename ChT>
template <typename F, typename I, bool REFCNT>
inline std::basic_ostream<Ch, ChT>&
sparse_polynom_output_list_binder<Ch, ChT>::operator<<
(const sparse_polynom<F, I, REFCNT>& x) const
{
    return output_list
    (
        stream, x, manip->monoms_order, manip->first_bracket,
        manip->second_bracket, manip->separator
    );
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct sparse_polynom_input_list;

template <typename Ch, typename ChT>
struct sparse_polynom_input_list_binder
{
    ARAGELI_IOMANIP_BINDER_PART1
    (
        sparse_polynom_input_list_binder,
        sparse_polynom_input_list,
        i
    );

    template <typename F, typename I, bool REFCNT>
    std::basic_istream<Ch, ChT>& operator>>
    (sparse_polynom<F, I, REFCNT>& x) const;

};

struct sparse_polynom_input_list
{
    const char
        *first_bracket,
        *second_bracket,
        *separator;

    sparse_polynom_input_list
    (
        const char* fb_a = monom_input_list_first_bracket_default,
        const char* sb_a = monom_input_list_second_bracket_default,
        const char* sep_a = monom_input_list_separator_default
    ) :
        first_bracket(fb_a),
        second_bracket(sb_a),
        separator(sep_a)
    {}
};

ARAGELI_IOMANIP_MANIP_PART1
(
    sparse_polynom_input_list,
    sparse_polynom_input_list_binder,
    i,
    >>
);

template <typename Ch, typename ChT>
template <typename F, typename I, bool REFCNT>
inline std::basic_istream<Ch, ChT>&
sparse_polynom_input_list_binder<Ch, ChT>::operator>>
(sparse_polynom<F, I, REFCNT>& x) const
{
    return input_list
    (
        stream,
        x,
        manip->first_bracket,
        manip->second_bracket,
        manip->separator
    );
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct sparse_polynom_output_var;

template <typename Ch, typename ChT>
struct sparse_polynom_output_var_binder
{
    ARAGELI_IOMANIP_BINDER_PART1
    (
        sparse_polynom_output_var_binder,
        sparse_polynom_output_var,
        o
    );

    template <typename F, typename I, bool REFCNT>
    std::basic_ostream<Ch, ChT>& operator<<
    (const sparse_polynom<F, I, REFCNT>& x) const;

};

struct sparse_polynom_output_var
{
    Arageli::monoms_order monoms_order;
    const char *var, *mul, *pow;

    sparse_polynom_output_var
    (
        Arageli::monoms_order mo = mo_inc,
        const char* var_a = monom_output_var_var_default,
        const char* mul_a = monom_output_var_mul_default,
        const char* pow_a = monom_output_var_pow_default
    ) :
        monoms_order(mo),
        var(var_a),
        mul(mul_a),
        pow(pow_a)
    {}
};

ARAGELI_IOMANIP_MANIP_PART1
(
    sparse_polynom_output_var,
    sparse_polynom_output_var_binder,
    o,
    <<
);

template <typename Ch, typename ChT>
template <typename F, typename I, bool REFCNT>
inline std::basic_ostream<Ch, ChT>&
sparse_polynom_output_var_binder<Ch, ChT>::operator<<
(const sparse_polynom<F, I, REFCNT>& x) const
{
    return output_var
    (
        stream,
        x,
        manip->monoms_order,
        manip->var,
        manip->mul,
        manip->pow
    );
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct sparse_polynom_input_var;

template <typename Ch, typename ChT>
struct sparse_polynom_input_var_binder
{
    ARAGELI_IOMANIP_BINDER_PART1
    (
        sparse_polynom_input_var_binder,
        sparse_polynom_input_var,
        i
    );

    template <typename F, typename I, bool REFCNT>
    std::basic_istream<Ch, ChT>& operator>>
    (sparse_polynom<F, I, REFCNT>& x) const;
};

struct sparse_polynom_input_var
{
    const char *var, *mul, *pow;

    sparse_polynom_input_var
    (
        const char* var_a = monom_input_var_var_default,
        const char* mul_a = monom_input_var_mul_default,
        const char* pow_a = monom_input_var_pow_default
    ) :
        var(var_a),
        mul(mul_a),
        pow(pow_a)
    {}
};

ARAGELI_IOMANIP_MANIP_PART1
(
    sparse_polynom_input_var,
    sparse_polynom_input_var_binder,
    i,
    >>
);

template <typename Ch, typename ChT>
template <typename F, typename I, bool REFCNT>
inline std::basic_istream<Ch, ChT>&
sparse_polynom_input_var_binder<Ch, ChT>::operator>>
(sparse_polynom<F, I, REFCNT>& x) const
{
    return input_var
    (
        stream,
        x,
        manip->var,
        manip->mul,
        manip->pow
    );
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct sparse_polynom_output_aligned;

template <typename Ch, typename ChT>
struct sparse_polynom_output_aligned_binder
{
    ARAGELI_IOMANIP_BINDER_PART1
    (
        sparse_polynom_output_aligned_binder,
        sparse_polynom_output_aligned,
        o
    );

    template <typename F, typename I, bool REFCNT>
    std::basic_ostream<Ch, ChT>& operator<<
    (const sparse_polynom<F, I, REFCNT>& x) const;

};

struct sparse_polynom_output_aligned
{
    Arageli::monoms_order monoms_order;
    const char *var, *mul, *pow;

    sparse_polynom_output_aligned
    (
        Arageli::monoms_order mo = mo_inc,
        const char* var_a = monom_output_var_var_default,
        const char* mul_a = monom_output_var_mul_default,
        const char* pow_a = monom_output_var_pow_default
    ) :
        monoms_order(mo),
        var(var_a),
        mul(mul_a),
        pow(pow_a)
    {}
};

ARAGELI_IOMANIP_MANIP_PART1
(
    sparse_polynom_output_aligned,
    sparse_polynom_output_aligned_binder,
    o,
    <<
);

template <typename Ch, typename ChT>
template <typename F, typename I, bool REFCNT>
inline std::basic_ostream<Ch, ChT>&
sparse_polynom_output_aligned_binder<Ch, ChT>::operator<<
(const sparse_polynom<F, I, REFCNT>& x) const
{
    return output_aligned
    (
        stream,
        x,
        manip->monoms_order,
        manip->var,
        manip->mul,
        manip->pow
    );
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct vector_output_list;

template <typename Ch, typename ChT>
struct vector_output_list_binder
{
    ARAGELI_IOMANIP_BINDER_PART1
    (
        vector_output_list_binder,
        vector_output_list,
        o
    );

    template <typename T, bool REFCNT>
    std::basic_ostream<Ch, ChT>& operator<<
    (const vector<T, REFCNT>& x) const;

};

struct vector_output_list
{
    const char
        *first_bracket,
        *second_bracket,
        *separator;

    vector_output_list
    (
        const char* fb_a = vector_output_list_first_bracket_default,
        const char* sb_a = vector_output_list_second_bracket_default,
        const char* sep_a = vector_output_list_separator_default
    ) :
        first_bracket(fb_a),
        second_bracket(sb_a),
        separator(sep_a)
    {}
};

ARAGELI_IOMANIP_MANIP_PART1
(
    vector_output_list,
    vector_output_list_binder,
    o,
    <<
);

template <typename Ch, typename ChT>
template <typename T, bool REFCNT>
inline std::basic_ostream<Ch, ChT>&
vector_output_list_binder<Ch, ChT>::operator<<
(const vector<T, REFCNT>& x) const
{
    return output_list
    (
        stream,
        x,
        manip->first_bracket,
        manip->second_bracket,
        manip->separator
    );
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct vector_input_list;

template <typename Ch, typename ChT>
struct vector_input_list_binder
{
    ARAGELI_IOMANIP_BINDER_PART1
    (
        vector_input_list_binder,
        vector_input_list,
        i
    );

    template <typename T, bool REFCNT>
    std::basic_istream<Ch, ChT>& operator>>
    (vector<T, REFCNT>& x) const;

};

struct vector_input_list
{
    const char *first_bracket, *second_bracket, *separator;

    vector_input_list
    (
        const char* fb_a = vector_input_list_first_bracket_default,
        const char* sb_a = vector_input_list_second_bracket_default,
        const char* sep_a = vector_input_list_separator_default
    ) :
        first_bracket(fb_a),
        second_bracket(sb_a),
        separator(sep_a)
    {}
};

ARAGELI_IOMANIP_MANIP_PART1
(
    vector_input_list,
    vector_input_list_binder,
    i,
    >>
);

template <typename Ch, typename ChT>
template <typename T, bool REFCNT>
inline std::basic_istream<Ch, ChT>&
vector_input_list_binder<Ch, ChT>::operator>>
(vector<T, REFCNT>& x) const
{
    return input_list
    (
        stream,
        x,
        manip->first_bracket,
        manip->second_bracket,
        manip->separator
    );
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct vector_output_aligned;

template <typename Ch, typename ChT>
struct vector_output_aligned_binder
{
    ARAGELI_IOMANIP_BINDER_PART1
    (
        vector_output_aligned_binder,
        vector_output_aligned,
        o
    );

    /*
    template <typename T, bool REFCNT>
    std::basic_ostream<Ch, ChT>& operator<<
    (const vector<T, REFCNT>& x) const;

    template <typename T, bool REFCNT>
    std::basic_ostream<Ch, ChT>& operator<<
    (const vector<T, REFCNT>& x)
    {
        return (*static_cast<const vector_output_aligned_binder<Ch, ChT>*>(this)) << x;
    }
    */
};

struct vector_output_aligned
{
    const char* left_col;
    const char* right_col;

    vector_output_aligned
    (
        const char* left_col_a = vector_output_aligned_left_col_default,
        const char* right_col_a = vector_output_aligned_right_col_default
    ) :
        left_col(left_col_a),
        right_col(right_col_a)
    {}
};

ARAGELI_IOMANIP_MANIP_PART1
(
    vector_output_aligned,
    vector_output_aligned_binder,
    o,
    <<
);


/*
template <typename Ch, typename ChT>
template <typename T, bool REFCNT>
inline std::basic_ostream<Ch, ChT>&
vector_output_aligned_binder<Ch, ChT>::operator<<
(const vector<T, REFCNT>& x) const
{
    return
        output_aligned
        (
            stream,
            x,
            manip->left_col,
            manip->right_col
        );
}
*/


template <typename Ch, typename ChT, typename T, bool REFCNT>
inline std::basic_ostream<Ch, ChT>&
operator<<
(
    const vector_output_aligned_binder<Ch, ChT>& b,
    const vector<T, REFCNT>& x
)
{
    return
        output_aligned
        (
            b.stream,
            x,
            b.manip->left_col,
            b.manip->right_col
        );
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct matrix_output_list;

template <typename Ch, typename ChT>
struct matrix_output_list_binder
{
    ARAGELI_IOMANIP_BINDER_PART1
    (
        matrix_output_list_binder,
        matrix_output_list, o
    );

    template <typename T, bool REFCNT>
    std::basic_ostream<Ch, ChT>& operator<<
    (const matrix<T, REFCNT>& x) const;

};

struct matrix_output_list
{
    const char
        *first_bracket,
        *second_bracket,
        *row_separator,
        *first_row_bracket,
        *second_row_bracket,
        *col_separator;

    matrix_output_list
    (
        const char* fb_a = matrix_output_list_first_bracket_default,
        const char* sb_a = matrix_output_list_second_bracket_default,
        const char* rsep_a = matrix_output_list_row_separator_default,
        const char* frb_a = matrix_output_list_first_row_bracket_default,
        const char* srb_a = matrix_output_list_second_row_bracket_default,
        const char* csep_a = matrix_output_list_col_separator_default
    ) :
        first_bracket(fb_a),
        second_bracket(sb_a),
        row_separator(rsep_a),
        first_row_bracket(frb_a),
        second_row_bracket(srb_a),
        col_separator(csep_a)
    {}
};

ARAGELI_IOMANIP_MANIP_PART1
(
    matrix_output_list,
    matrix_output_list_binder,
    o,
    <<
);

template <typename Ch, typename ChT>
template <typename T, bool REFCNT>
inline std::basic_ostream<Ch, ChT>&
matrix_output_list_binder<Ch, ChT>::operator<<
(const matrix<T, REFCNT>& x) const
{
    return output_list
    (
        stream, x,
        manip->first_bracket,
        manip->second_bracket,
        manip->row_separator,
        manip->first_row_bracket,
        manip->second_row_bracket,
        manip->col_separator
    );
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct matrix_input_list;

template <typename Ch, typename ChT>
struct matrix_input_list_binder
{
    ARAGELI_IOMANIP_BINDER_PART1
    (
        matrix_input_list_binder,
        matrix_input_list,
        i
    );

    template <typename T, bool REFCNT>
    std::basic_istream<Ch, ChT>& operator>>
    (matrix<T, REFCNT>& x) const;

};

struct matrix_input_list
{
    const char
        *first_bracket,
        *second_bracket,
        *row_separator,
        *first_row_bracket,
        *second_row_bracket,
        *col_separator;

    matrix_input_list
    (
        const char* fb_a = matrix_input_list_first_bracket_default,
        const char* sb_a = matrix_input_list_second_bracket_default,
        const char* rsep_a = matrix_input_list_row_separator_default,
        const char* frb_a = matrix_input_list_first_row_bracket_default,
        const char* srb_a = matrix_input_list_second_row_bracket_default,
        const char* csep_a = matrix_input_list_col_separator_default
    ) :
        first_bracket(fb_a),
        second_bracket(sb_a),
        row_separator(rsep_a),
        first_row_bracket(frb_a),
        second_row_bracket(srb_a),
        col_separator(csep_a)
    {}
};

ARAGELI_IOMANIP_MANIP_PART1
(
    matrix_input_list,
    matrix_input_list_binder,
    i,
    >>
);

template <typename Ch, typename ChT>
template <typename T, bool REFCNT>
inline std::basic_istream<Ch, ChT>&
matrix_input_list_binder<Ch, ChT>::operator>>
(matrix<T, REFCNT>& x) const
{
    return input_list
    (
        stream, x,
        manip->first_bracket,
        manip->second_bracket,
        manip->row_separator,
        manip->first_row_bracket,
        manip->second_row_bracket,
        manip->col_separator
    );
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

struct matrix_output_aligned;

template <typename Ch, typename ChT>
struct matrix_output_aligned_binder
{
    ARAGELI_IOMANIP_BINDER_PART1
    (
        matrix_output_aligned_binder,
        matrix_output_aligned,
        o
    );

    /*
    template <typename T, bool REFCNT>
    std::basic_ostream<Ch, ChT>& operator<<
    (const matrix<T, REFCNT>& x) const;
    */
};

struct matrix_output_aligned
{
    const char
        *left_col,
        *right_col,
        *inter_col;

    matrix_output_aligned
    (
        const char* left_col_a = matrix_output_aligned_left_col_default,
        const char* right_col_a = matrix_output_aligned_right_col_default,
        const char* inter_col_a = matrix_output_aligned_inter_col_default
    ) :
        left_col(left_col_a),
        right_col(right_col_a),
        inter_col(inter_col_a)
    {}
};

ARAGELI_IOMANIP_MANIP_PART1
(
    matrix_output_aligned,
    matrix_output_aligned_binder,
    o,
    <<
);

/*
template <typename Ch, typename ChT>
template <typename T, bool REFCNT>
inline std::basic_ostream<Ch, ChT>&
matrix_output_aligned_binder<Ch, ChT>::operator<<
(const matrix<T, REFCNT>& x) const
{
    return output_aligned
    (
        stream,
        x,
        manip->left_col,
        manip->right_col,
        manip->inter_col
    );
}
*/

template <typename Ch, typename ChT, typename T, bool REFCNT>
inline std::basic_ostream<Ch, ChT>&
operator<<
(
    const matrix_output_aligned_binder<Ch, ChT>& binder,
    const matrix<T, REFCNT>& x
)
{
    return output_aligned
    (
        binder.stream,
        x,
        binder.manip->left_col,
        binder.manip->right_col,
        binder.manip->inter_col
    );
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


}    // namespace iomanip
}    // namespace Arageli


#endif  //  #ifndef _ARAGELI_iomanip_hpp_
