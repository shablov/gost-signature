/*****************************************************************************

    submatrix/indexed.hpp -- A representation to a minor-like submatrix of any
        matrix by choosing two vectors of indexes for rows and columns.

    This file is a part of Aragali library.

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
    \file
    This file contains structures to represent arbitrary submatrix of any
    matrix. This makes by choosing two vectors of indexes to index  by rows
    and coluns original    matrix that contain real values. So, structures
    provided by this file are reference structures that don't contain
    original matrix values.
*/


#ifndef _ARAGELI_submatrix_indexeded_hpp_
#define _ARAGELI_submatrix_indexeded_hpp_

#include "../config.hpp"

#include "../type_opers.hpp"
#include "../frwrddecl.hpp"
#include "../misc.hpp"

#include "../std_import.hpp"


namespace Arageli
{


struct fromall_t {};
const fromall_t fromall = fromall_t();


/// submatrix references the original vector by vector of indexes for rows and columns.
/** This structure contains two pice of information: reference to the original
    matrix and copy of the two vectors of indexes for rows and columns.
    The type of the first one is Base, the second --- are RowIndex and ColIndex. */
template <typename Base, typename RowIndex, typename ColIndex>
class indexed_submatrix
{
    Base* base_m;
    RowIndex row_index_m;
    ColIndex col_index_m;

public:

    // TODO: Complete the class implementation.

    /// The type of the original vector with values.
    typedef Base base_type;

    /// The type of the vector of indexes for rows.
    typedef RowIndex row_index_type;

    /// The type of the vector of indexes for columns.
    typedef ColIndex col_index_type;

    /// The item type of the original vector.
    typedef typename cnc_value_type<Base>::type value_type;

    /// The item type of the original vector.
    typedef value_type element_type;

    /// Lvalue of item type.
    typedef typename cnc_reference<Base>::type reference;

    /// Const lvalue of item type.
    typedef typename Base::const_reference const_reference;

    /// Unsigned integer type for size and absolute index representation.
    /** Can represent each non-negative value of difference_type. */
    typedef typename RowIndex::size_type size_type;    // WARNING! Only row select!

    /// Signed integer type for offsets and relative index representation.
    typedef typename RowIndex::difference_type difference_type;    // WARNING! Only row select!

    /// Pointer to const item type.
    typedef typename cnc_pointer<Base>::type pointer;

    /// Pointer to item type.
    typedef typename Base::const_pointer const_pointer;


    // Constructors. -------------------------------------------------


    /// Creates submatrix without the original matrix.
    /** The submatrix looks empty. */
    indexed_submatrix () :
        base_m(0)
    {}

    /// Creates empty submatrix with the particular original matrix.
    /** The index vectors are being initialized by the default.
        The submatrix looks empty. */
    indexed_submatrix (base_type* base_a) :
        base_m(base_a)
    {}

    /// Creates submatrix as whole original matrix.
    /** The index vectors are being initialized by the ranges
        [0, base_a->nrows()) and [0, base_a->ncol()).
        The submatrix looks empty. */
    indexed_submatrix (base_type* base_a, const fromall_t&) :
        base_m(base_a),
        row_index_m(base_a->nrows()),
        col_index_m(base_a->ncols())
    {
        size_type zero = 0;
        if(!row_index_m.is_empty())
            generate_range_helper
            (
                zero,
                row_index_m.size() - 1,
                row_index_m.begin()
            );
        zero = 0;
        if(!col_index_m.is_empty())
            generate_range_helper
            (
                zero,
                col_index_m.size() - 1,
                col_index_m.begin()
            );
    }

    /// Creates submatrix with the particular original and index vectors.
    indexed_submatrix
    (
        base_type* base_a,
        const row_index_type& row_index_a,
        const col_index_type& col_index_a
    ) :
        base_m(base_a),
        row_index_m(row_index_a),
        col_index_m(col_index_a)
    {}


    // Access to the original and index vectors.  --------------------


    base_type& base () const
    {
        return *base_m;
    }

    void base (base_type* base_a)
    {
        base_m = base_a;
    }

    const row_index_type& row_index () const
    {
        return row_index_m;
    }

    row_index_type& row_index ()
    {
        return row_index_m;
    }

    const col_index_type& col_index () const
    {
        return col_index_m;
    }

    col_index_type& col_index ()
    {
        return col_index_m;
    }


    // Some properties. ----------------------------------------------


    size_type size () const
    {
        return nrows()*ncols();
    }

    size_type nrows () const
    {
        return row_index_m.size();
    }

    size_type ncols () const
    {
        return col_index_m.size();
    }

    bool is_empty () const
    {
        return row_index_m.is_empty() || col_index_m.is_empty();
    }


    // Access to the elements. ---------------------------------------


    const_reference el (size_type i, size_type j) const    // WARNING! The same size_type for rows and cols.
    {
        return
            (*static_cast<const Base*>(base_m))
                (row_index_m[i], col_index_m[j]);
    }

    reference el (size_type i, size_type j)
    {
        return (*base_m)(row_index_m[i], col_index_m[j]);
    }

    const_reference at (size_type i, size_type j) const
    {
        return
            (*static_cast<const Base*>(base_m)).at
                (row_index_m.at(i), col_index_m.at(j));
    }

    reference at (size_type i, size_type j)
    {
        return (*base_m).at(row_index_m.at(i), col_index_m.at(j));
    }

    // WARNING! The same size_type for rows and cols.
    const_reference operator() (size_type i, size_type j) const
    {
        return
            (*static_cast<const Base*>(base_m))
                (row_index_m[i], col_index_m[j]);
    }

    reference operator() (size_type i, size_type j)
    {
        return (*base_m)(row_index_m[i], col_index_m[j]);
    }

    // WARNING! Temporary implementation.
    vector<typename omit_const<element_type>::type> copy_col (size_type i) const
    {
        return base_m->copy_col(col_index_m[i]).copy_subvector(row_index_m);
    }

};


template <typename Out, typename Base, typename RowIndex, typename ColIndex>
void output_aligned
(
    Out& out,
    const indexed_submatrix<Base, RowIndex, ColIndex>& x
)
{
    output_aligned
    (
        out,
        x.base().copy_submatrix(x.row_index(), x.col_index())
    );
}


}


//#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
//    #define _ARAGELI_submatrix_indexed_hpp_
//    #include "indexed.cpp"
//    #undef  _ARAGELI_submatrix_indexed_hpp_
//#endif


#endif

