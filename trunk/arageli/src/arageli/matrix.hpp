/*****************************************************************************

    matrix.hpp

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

/**
    \file matrix.hpp
    \brief Matrix template class definition.

    Template class matrix definition and relative operations.
    This file contains template class matrix, relative nonmember functions and
    a specialization of type_traits common template for the matrix class.

    This file contains only declarations of all noninline operations.
    Implementation of other operations see in matrix.cpp file.

    Indexes of entries in a matrix begin from zero.
*/


#ifndef _ARAGELI_matrix_hpp_
#define _ARAGELI_matrix_hpp_

#include "config.hpp"

#include <cstddef>
#include <iostream>
#include <vector>
#include <utility>

#include "frwrddecl.hpp"

#include "type_opers.hpp"
#include "type_traits.hpp"
#include "type_pair_traits.hpp"
#include "exception.hpp"
#include "refcntr.hpp"
#include "factory.hpp"
#include "cmp.hpp"
#include "io.hpp"
#include "vector.hpp"

#include "std_import.hpp"


namespace Arageli
{


/// Type for helper object fromsize.
/** See fromsize. */
struct nonsquare_t {};

///// Helper object for construct a fromsize matrix.
///** See constructors of matrix template class. */
//const nonsquare_t fromsize = nonsquare_t();


/// Type for helper object for columnwise filling of the matrix.
/** See colwise. */
struct colwise_t {};

/// Helper object for columnwise filling of the matrix.
/** See constructors of matrix template class. */
const colwise_t colwise = colwise_t();


/// Type for helper object diag.
/** See diag. */
struct diag_t {};

/// Helper object for construct a diagonal matrix.
/** See constructors of matrix template class. */
const diag_t diag = diag_t();


/// Type for helper object eye.
/** See eye. */
struct eye_t {};

/// Helper object for construct an indentity matrix.
/** See constructors of matrix template class. */
const eye_t eye = eye_t();


struct frommat_t {};
const frommat_t frommat = frommat_t();


namespace _Internal
{

template <typename T>
struct Rep_matrix    // internal representation for matrix template
{
    // std::vector -- âðåìåííî, íóæíî ñâîé îáëåã÷¸ííûé âåêòîð
    typedef std::vector<T> Mem;
    typedef typename Mem::size_type size_type;
    size_type rows, cols;
    Mem mem;

    Rep_matrix () :
        rows(0),
        cols(0)
    {}

    void assign (size_type rows_a, size_type cols_a, const T& val)
    {
        rows = rows_a;
        cols = cols_a;
        mem.assign(rows*cols, val);
    }

    template <typename In>
    void assign (size_type rows_a, size_type cols_a, In b, In e)
    {
        rows = rows_a;
        cols = cols_a;
        mem.assign(b, e);
        ARAGELI_ASSERT_1(rows*cols == mem.size());
    }
};

} // namespace _Internal
} // namespace Arageli


namespace std
{

template <typename T>
inline void swap
(
    Arageli::_Internal::Rep_matrix<T>& a,
    Arageli::_Internal::Rep_matrix<T>& b
)
{
    swap(a.mem, b.mem);
    swap(a.rows, b.rows);
    swap(a.cols, b.cols);
}

} // namespace std


namespace Arageli
{


/// Dynamicly resizable dense matrix.
/** Definition of "dense" matrix data structure.
    All indexes are started from zero.
    @param T type of matrix's items
    @param REFCNT reference counter switch */
template
<
    typename T,
    bool REFCNT
>
class matrix
{
    template <typename T1, bool REFCNT1>
    friend class matrix;

    typedef _Internal::Rep_matrix<T> Rep;
    typedef typename Rep::Mem Mem;

public:


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Standard type definition.


    //@{

    /// The item type. It is T.
    typedef typename Mem::value_type value_type;

    /// The item type. It is T.
    typedef typename Mem::value_type element_type;

    /// Lvalue of item type.
    typedef typename Mem::reference reference;

    /// Const lvalue of item type.
    typedef typename Mem::const_reference const_reference;

    /// Pointer to const item type.
    typedef typename Mem::pointer pointer;

    /// Pointer to item type.
    typedef typename Mem::const_pointer const_pointer;

    /// Unsigned integer type for size and absolute index representation.
    /** Can represent each non-negative value of difference_type. */
    typedef typename Mem::size_type size_type;

    /// Signed integer type for offsets and relative index representation.
    typedef typename Mem::difference_type difference_type;

    /// Iterator type.
    typedef typename Rep::Mem::iterator iterator;

    /// Const iterator type.
    typedef typename Rep::Mem::const_iterator const_iterator;

    // @}


    /// Defines vector type with other element type and the same reference counter.
    template <typename T1> struct other_element_type
    {
        typedef matrix<T1, REFCNT> type;
    };

    /// Defines vector type with other element type ane reference counter.
    template <typename T1, bool REFCNT1> struct other_element_type_refcnt
    {
        typedef matrix<T1, REFCNT1> type;
    };

    /// Reference counting property.
    /**    If it is true then counting is enabled else it is disabled. */
    static const bool refcounting = REFCNT;


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Constructors.


    //@{


    /// Creates a matrix with zero dimensions.
    matrix ()
    {}

    // BE CAREFUL! There is one more constructor. It is automaticaly generated
    // copy-constructor. Worry about its meaning is being belonged to other
    // constructors during modifications.


    // ------------------------------------------------------------------


    /// Creates square matrix sized by size_a by size_a with values factory<T>::null().
    matrix (std::size_t size_a, const fromsize_t& select)
    {
        assign_fromsize(size_a);
    }


    /// Creates matrix from string noted by usual rules for matrices.
    matrix (const char* s, const fromstr_t& select)
    {
        assign_fromstr(s);
    }


    /// Creates a row-matrix copied from a vector-like structure.
    template <typename Vec>
    matrix (const Vec& vec, const fromvec_t& select)
    {
        assign_fromvec(vec);
    }


    /// Creates a matrix as a copy of another matrix-like structure.
    template <typename Mat>
    matrix (const Mat& mat, const frommat_t& select)
    {
        assign_frommat(mat);
    }


    /// Mixed constructor: creates from a string, by a size, from another vector or matrix.
    template <typename X>
    matrix (const X& x)
    {
        assign(x);
    }


    // ------------------------------------------------------------------


    /// Creates square matrix with val as each element.
    /** @param rowscols_a size of a matrix, it is number of rows
            and number of columns
        @param val an initial value for each element */
    template <typename Val>
    matrix
    (
        size_type rowscols_a,
        const Val& val,
        const fromval_t& select
    )
    {
        assign_fromval(rowscols_a, val);
    }


    /// Creates square matrix from a sequence.
    /** @param rowscols_a    size of a matrix, it is number of rows
            and number of columns
        @param seq    an iterator to begin of the sequence, and elements of matrix
            will be rowwise constructed from this sequence. */
    template <typename Seq>
    matrix
    (
        size_type rowscols_a,
        const Seq& seq,
        const fromseq_t& select
    )
    {
        assign_fromseq(rowscols_a, seq);
    }


    /// Creates matrix with any dimensions, each item is equal to factory<T>::null().
    /** @param rows_a    number of rows
        @param cols_a    number of columns */
    matrix
    (
        size_type rows_a,
        size_type cols_a,
        const fromsize_t& select
    )
    {
        assign_fromsize(rows_a, cols_a);
    }


    /// Creates matrix with each row copied from some vector.
    template <typename Vec>
    matrix
    (
        size_type rows_a,
        const Vec& vec,
        const fromvec_t& select
    )
    {
        assign_fromvec(rows_a, vec);
    }


    /// Mixed constructor: creates square matrix from a sequence or just null matrix with any dimensions.
    template <typename X>
    matrix
    (
        size_type rowscols_a,
        const X& x
    )
    {
        assign(rowscols_a, x);
    }


    // ------------------------------------------------------------------


    /// Creates a column-matrix copied from a vector-like structure.
    template <typename Vec>
    matrix
    (
        const Vec& vec,
        const colwise_t& select
    )
    {
        assign_colwise(vec);
    }


    /// Creates a matrix each column copied from a vector.
    template <typename Vec>
    matrix
    (
        size_type cols_a,
        const Vec& vec,
        const colwise_t& select_1,
        const fromvec_t& select_2
    )
    {
        assign_colwise_fromvec(cols_a, vec);
    }


    /// Creates a square matrix from a sequence with column-wise filling.
    template <typename Seq>
    matrix
    (
        size_type rowcols_a,
        const Seq& seq,
        const colwise_t& select_1,
        const fromseq_t& select_2
    )
    {
        assign_colwise_fromseq(rowcols_a, seq);
    }


    template <typename X>
    matrix
    (
        size_type rowcols_a,
        const X& x,
        const colwise_t& select
    )
    {
        assign_colwise(rowcols_a, x);
    }


    /// Creates a matrix from a sequence with column-wise filling.
    template <typename Seq>
    matrix
    (
        size_type rows_a,
        size_type cols_a,
        const Seq& seq,
        const colwise_t& select
    )
    {
        assign_colwise(rows_a, cols_a, seq);
    }


    // ------------------------------------------------------------------


    /// Creates matrix with any dimentions and with val as each element.
    /** @param rows_a    number of rows
        @param cols_a    number of columns
        @param val    an initial value for each element */
    template <typename Val>
    matrix
    (
        size_type rows_a,
        size_type cols_a,
        const Val& val,
        const fromval_t& select
    )
    {
        assign_fromval(rows_a, cols_a, val);
    }


    /// Creates matrix with any dimentions from sequence.
    /** @param rows_a    number of rows
        @param cols_a    number of columns
        @param in    an iterator to begin of the sequence, and elements of matrix
            will be rowwise constructed from this sequence */
    template <typename Seq>
    matrix
    (
        size_type rows_a,
        size_type cols_a,
        const Seq& seq,
        const fromseq_t& select
    )
    {
        assign_fromseq(rows_a, cols_a, seq);
    }


    /// Mixed constructor: creates matrix with any dimentions from a value or from a sequence.
    /** Chooses between
                matrix(rows_a, cols_a, x, const fromval_t& select)
            and
                matrix(rows_a, cols_a, x, const fromseq_t& select).

        @param rows_a    number of rows
        @param cols_a    number of columns
        @param x    Meaning of this parameter depends on a structure of In_or_T1.
            If X is conversible to T then x is an initial value for each
            element of the matrix; otherwise x is an iterator to begin of the sequence,
            and elements of matrix will be rowwise constructed from this sequence. */
    template <typename X>
    matrix
    (
        size_type rows_a,
        size_type cols_a,
        const X& x
    )
    {
        assign(rows_a, cols_a, x);
    }


    // ------------------------------------------------------------------


    /// Creates a square diagonal matrix from a vector.
    /** @param rowscols_a size of a matrix
        @param seq sequence of init values */
    template <typename Vec>
    matrix (const Vec& vec, const diag_t& select)
    {
        assign_diag(vec);
    }


    /// Creates a square diagonal matrix with a particular value on the diagonal.
    /** @param rowscols_a size of a matrix
        @param val value on the diagonal */
    template <typename Val>
    matrix
    (
        size_type rowscols_a,
        const Val& val,
        const diag_t& select_1,
        const fromval_t& select_2
    )
    {
        assign_diag_fromval(rowscols_a, val);
    }


    /// Creates a square diagonal matrix from a sequence.
    /** @param rowscols_a size of a matrix
        @param seq sequence of init values */
    template <typename Seq>
    matrix
    (
        size_type rowscols_a,
        const Seq& seq,
        const diag_t& select_1,
        const fromseq_t& select_2
    )
    {
        assign_diag_fromseq(rowscols_a, seq);
    }


    /// Creates a square diagonal matrix from a vector.
    /** @param rowscols_a size of a matrix
        @param seq sequence of init values */
    template <typename Vec>
    matrix
    (
        size_type rowscols_a,
        const Vec& vec,
        const diag_t& select_1,
        const fromvec_t& select_2
    )
    {
        assign_diag_fromvec(rowscols_a, vec);
    }


    /// Mixed constructor: creates a square diagonal matrix from a value, from a sequence or from a vector.
    template <typename X>
    matrix
    (
        size_type rowscols_a,
        const X& x,
        const diag_t& select
    )
    {
        assign_diag(rowscols_a, x);
    }


    /// Creates a diagonal matrix with any dimensions with a particular value on the diagonal.
    /** @param rows_a    number of rows
        @param cols_a    number of columns
        @param val value on the diagonal */
    template <typename Val>
    matrix
    (
        size_type rows_a,
        size_type cols_a,
        const Val& val,
        const diag_t& select_1,
        const fromval_t& select_2
    )
    {
        assign_diag_fromval(rows_a, cols_a, val);
    }


    /// Creates a diagonal with any dimensions matrix from a sequence.
    /** @param rows_a    number of rows
        @param cols_a    number of columns
        @param seq sequence of init values */
    template <typename Seq>
    matrix
    (
        size_type rows_a,
        size_type cols_a,
        const Seq& seq,
        const diag_t& select_1,
        const fromseq_t& select_2
    )
    {
        assign_diag_fromseq(rows_a, cols_a, seq);
    }


    /// Creates a diagonal with any dimensions matrix from a sequence.
    /**    @param rows_a    number of rows
        @param cols_a    number of columns
        @param vec sequence of init values */
    template <typename Vec>
    matrix
    (
        size_type rows_a,
        size_type cols_a,
        const Vec& vec,
        const diag_t& select_1,
        const fromvec_t& select_2
    )
    {
        assign_diag_fromvec(rows_a, cols_a, vec);
    }


    /// Mixed constructor: creates a diagonal matrix with any dimentions from a value or from a sequence.
    /** @param rows_a    number of rows
        @param cols_a    number of columns
        @param x    Meaning of this parameter depends on a structure of X. */
    template <typename X>
    matrix
    (
        size_type rows_a,
        size_type cols_a,
        const X& x,
        const diag_t& select
    )
    {
        assign_diag(rows_a, cols_a, x);
    }


    // ------------------------------------------------------------------


    /// Creates a square identity matrix.
    /**    @param rowscols_a size of a matrix
        @param select helper argument for choosing this constructor,
            use eye  */
    matrix (size_type rowscols_a, const eye_t& select)
    {
        assign_eye(rowscols_a);
    }


    /// Creates an identity matrix with any dimensions.
    /**    @param rowscols_a size of a matrix
        @param select helper argument for choosing this constructor,
            use eye  */
    matrix
    (
        size_type rows_a,
        size_type cols_a,
        const eye_t& select
    )
    {
        assign_eye(rows_a, cols_a);
    }


    // @}


    /// Creates square matrix sized by size_a by size_a with values factory<T>::null().
    void assign_fromsize (std::size_t size_a)
    {
        assign_fromsize(size_a, size_a);
    }


    /// Creates matrix from string noted by usual rules for matrices.
    void assign_fromstr (const char* s);


    /// Creates a row-matrix copied from a vector-like structure.
    template <typename Vec>
    void assign_fromvec (const Vec& vec)
    {
        assign_fromseq(1u, vec.size(), vec.begin());
    }


    /// Creates a matrix as a copy of another matrix-like structure.
    template <typename Mat>
    void assign_frommat (const Mat& mat)
    {
        assign_fromseq(mat.nrows(), mat.ncols(), mat.begin());
    }


    // ------------------------------------------------------------------


    /// Creates square matrix with val as each element.
    /** @param rowscols_a size of a matrix, it is number of rows
            and number of columns
        @param val an initial value for each element */
    template <typename Val>
    void assign_fromval (size_type rowscols_a, const Val& val)
    {
        assign_fromval(rowscols_a, rowscols_a, val);
    }


    /// Creates square matrix from a sequence.
    /**    @param rowscols_a    size of a matrix, it is number of rows
            and number of columns
        @param seq    an iterator to begin of the sequence, and elements of matrix
            will be rowwise constructed from this sequence. */
    template <typename Seq>
    void assign_fromseq (size_type rowscols_a, const Seq& seq)
    {
        assign_fromseq(rowscols_a, rowscols_a, seq);
    }


    /// Creates matrix with any dimensions, each item is equal to factory<T>::null().
    /** @param rows_a    number of rows
        @param cols_a    number of columns */
    void assign_fromsize (size_type rows_a, size_type cols_a)
    {
        assign_fromval(rows_a, cols_a, factory<T>::null());
    }


    /// Creates matrix with each row copied from some vector.
    /** WARNING! It isn't implemented yet. */
    template <typename Vec>
    void assign_fromvec (size_type rows_a, const Vec& vec);


    // ------------------------------------------------------------------


    /// Creates a column-matrix copied from a vector-like structure.
    /** WARNING! It isn't implemented yet. */
    template <typename Vec>
    void assign_colwise (const Vec& vec);


    /// Creates a matrix each column copied from a vector.
    /** WARNING! It isn't implemented yet. */
    template <typename Vec>
    void assign_colwise_fromvec (size_type cols_a, const Vec& vec);


    /// Creates a square matrix from a sequence with column-wise filling.
    /** WARNING! It isn't implemented yet. */
    template <typename Seq>
    void assign_colwise_fromseq (size_type rowcols_a, const Seq& seq);


    /** WARNING! It isn't implemented yet. */
    template <typename X>
    void assign_colwise (size_type rowcols_a, const X& x);


    /// Creates a matrix from a sequence with column-wise filling.
    /** WARNING! It isn't implemented yet. */
    template <typename Seq>
    void assign_colwise
    (size_type rows_a, size_type cols_a, const Seq& seq);


    // ------------------------------------------------------------------


    /// Creates matrix with any dimentions and with val as each element.
    /** @param rows_a    number of rows
        @param cols_a    number of columns
        @param val    an initial value for each element */
    template <typename Val>
    void assign_fromval
    (
        size_type rows_a,
        size_type cols_a,
        const Val& val
    );


    /// Creates matrix with any dimentions from sequence.
    /** @param rows_a    number of rows
        @param cols_a    number of columns
        @param in    an iterator to begin of the sequence, and elements of matrix
            will be rowwise constructed from this sequence */
    template <typename Seq>
    void assign_fromseq
    (
        size_type rows_a,
        size_type cols_a,
        Seq seq
    );


    // ------------------------------------------------------------------


    /// Creates a square diagonal matrix from a vector.
    /**    @param rowscols_a size of a matrix
        @param seq sequence of init values */
    template <typename Vec>
    void assign_diag (const Vec& vec)
    {
        assign_diag_fromseq(vec.size(), vec.begin());
    }


    /// Creates a square diagonal matrix with a particular value on the diagonal.
    /**    @param rowscols_a size of a matrix
        @param val value on the diagonal */
    template <typename Val>
    void assign_diag_fromval (size_type rowscols_a, const Val& val)
    {
        assign_diag_fromval(rowscols_a, rowscols_a, val);
    }


    /// Creates a square diagonal matrix from a sequence.
    /**    @param rowscols_a size of a matrix
        @param seq sequence of init values */
    template <typename Seq>
    void assign_diag_fromseq (size_type rowscols_a, const Seq& seq)
    {
        assign_diag_fromseq(rowscols_a, rowscols_a, seq);
    }


    /// Creates a square diagonal matrix from a vector.
    /**    @param rowscols_a size of a matrix
        @param seq sequence of init values */
    template <typename Vec>
    void assign_diag_fromvec (size_type rowscols_a, const Vec& vec)
    {
        assign_diag_fromseq(rowscols_a, vec.begin());
    }


    /// Mixed constructor: creates a square diagonal matrix from a value, from a sequence or from a vector.
    template <typename X>
    void assign_diag (size_type rowscols_a, const X& x)
    {
        assign_diag(rowscols_a, rowscols_a, x);
    }


    /// Creates a diagonal matrix with any dimensions with a particular value on the diagonal.
    /**    @param rows_a    number of rows
        @param cols_a    number of columns
        @param val value on the diagonal */
    template <typename Val>
    void assign_diag_fromval
    (
        size_type rows_a,
        size_type cols_a,
        const Val& val
    );


    /// Creates a diagonal with any dimensions matrix from a sequence.
    /**    @param rows_a    number of rows
        @param cols_a    number of columns
        @param seq sequence of init values */
    template <typename Seq>
    void assign_diag_fromseq
    (
        size_type rows_a,
        size_type cols_a,
        Seq seq
    );


    /// Creates a diagonal with any dimensions matrix from a sequence.
    /**    @param rows_a    number of rows
        @param cols_a    number of columns
        @param vec sequence of init values */
    template <typename Vec>
    void assign_diag_fromvec
    (
        size_type rows_a,
        size_type cols_a,
        const Vec& vec
    )
    {
        assign_diag_fromseq(rows_a, cols_a, vec.begin());
    }


    /// Mixed constructor: creates a diagonal matrix with any dimentions from a value or from a sequence.
    /** @param rows_a    number of rows
        @param cols_a    number of columns
        @param x    Meaning of this parameter depends on a structure of X. */
    template <typename X>
    void assign_diag
    (
        size_type rows_a,
        size_type cols_a,
        const X& x
    )
    {
        _assign_diag_val_seq_vec
        (
            rows_a,
            cols_a,
            x,
            type_traits<X>::category_value
        );
    }

private:

    template <typename Val>
    void _assign_diag_val_seq_vec
    (
        size_type rows_a,
        size_type cols_a,
        const Val& val,
        const type_category::type&
    )
    {
        assign_diag_fromval(rows_a, cols_a, val);
    }

    template <typename Seq>
    void _assign_diag_val_seq_vec
    (
        size_type rows_a,
        size_type cols_a,
        const Seq& seq,
        const type_category::iterator&
    )
    {
        assign_diag_fromseq(rows_a, cols_a, seq);
    }

    template <typename Vec>
    void _assign_diag_val_seq_vec
    (
        size_type rows_a,
        size_type cols_a,
        const Vec& vec,
        const type_category::vector&
    )
    {
        assign_diag_fromvec(rows_a, cols_a, vec);
    }

public:

    // ------------------------------------------------------------------


    /// Creates a square identity matrix.
    /**    @param rowscols_a size of a matrix
        @param select helper argument for choosing this constructor,
            use eye  */
    void assign_eye (size_type rowscols_a)
    {
        assign_eye(rowscols_a, rowscols_a);
    }


    /// Creates an identity matrix with any dimensions.
    /**    @param rowscols_a size of a matrix
        @param select helper argument for choosing this constructor,
            use eye  */
    void assign_eye (size_type rows_a, size_type cols_a)
    {
        assign_diag_fromval(rows_a, cols_a, factory<T>::unit());
    }


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Assignments.


    // @{


    /// Creates a matrix with zero dimensions.
    void assign ()
    {
        resize(0, 0);
    }


    // ------------------------------------------------------------------


    /// Mixed assignment: creates from a string, by a size, from another vector or matrix.
    template <typename X>
    void assign (const X& x)
    {
        _assign_size_str_vec_mat(x, type_traits<X>::category_value);
    }

private:

    template <typename Size>
    void _assign_size_str_vec_mat
    (const Size& size_a, const type_category::integer&)
    {
        assign_fromsize(size_a);
    }

    template <typename Str>
    void _assign_size_str_vec_mat
    (const Str& s, const type_category::string&)
    {
        assign_fromstr(s);
    }

    template <typename Vec>
    void _assign_size_str_vec_mat
    (const Vec& vec, const type_category::vector&)
    {
        assign_fromvec(vec);
    }

    template <typename Mat>
    void _assign_size_str_vec_mat
    (const Mat& mat, const type_category::matrix&)
    {
        assign_frommat(mat);
    }

public:

    // ------------------------------------------------------------------

    /// Mixed assignment: creates square matrix from a sequence or just null matrix with any dimensions.
    template <typename X>
    void assign (size_type rowscols_a, const X& x)
    {
        _assign_seq_size_vec
        (
            rowscols_a, x,
            type_traits<X>::category_value//,
            //bool_type<type_pair_traits<X, T>::is_assignable>::value
        );
    }


private:

    /*
    template <typename Val>
    void _assign_val_seq_size_vec
    (
        size_type rowscols_a,
        const Val& val,
        const type_category::type&,
        const true_type&
    )
    {
        assign_fromval(rowscols_a, val);
    }
    */

    template <typename Seq>
    void _assign_seq_size_vec
    (
        size_type rowscols_a,
        const Seq& seq,
        const type_category::iterator&
        //, const false_type&
    )
    {
        assign_fromseq(rowscols_a, seq);
    }

    /*
    template <typename Size>
    void _assign_val_seq_size_vec
    (
        size_type rows_a,
        const Size& cols_a,
        const type_category::integer&
        //, const true_type&
    )
    {
        assign_fromsize(rows_a, cols_a);
    }
    */

    template <typename Size>
    void _assign_seq_size_vec
    (
        size_type rows_a,
        const Size& cols_a,
        const type_category::integer&
        //, const false_type&
    )
    {
        assign_fromsize(rows_a, cols_a);
    }

    template <typename Vec>
    void _assign_seq_size_vec
    (
        size_type rows_a,
        const Vec& vec,
        const type_category::vector&
        //, const false_type&
    )
    {
        assign_fromvec(rows_a, vec);
    }

public:

    // ------------------------------------------------------------------

    /// Mixed assignment: creates matrix with any dimentions from a value or from a sequence.
    /** @param rows_a    number of rows
        @param cols_a    number of columns
        @param x    Meaning of this parameter depends on a structure of In_or_T1.
            If X is conversible to T then x is an initial value for each
            element of the matrix; otherwise x is an iterator to begin of the sequence,
            and elements of matrix will be rowwise constructed from this sequence. */
    template <typename X>
    void assign (size_type rows_a, size_type cols_a, const X& x)
    {
        _assign_val_seq
        (
            rows_a,
            cols_a,
            x,
            bool_type<type_pair_traits<X, T>::is_assignable>::value
        );
    }


private:

    template <typename Val>
    void _assign_val_seq
    (
        size_type rows_a,
        size_type cols_a,
        const Val& val,
        const true_type&
    )
    {
        assign_fromval(rows_a, cols_a, val);
    }

    template <typename Seq>
    void _assign_val_seq
    (
        size_type rows_a,
        size_type cols_a,
        const Seq& seq,
        const false_type&
    )
    {
        assign_fromseq(rows_a, cols_a, seq);
    }

public:

    // ------------------------------------------------------------------

    template <typename X>
    matrix& operator= (const X& x)
    {
        _assign_from_val_str_vec_mat
        (
            x,
            type_traits<X>::category_value,
            bool_type<type_pair_traits<X, T>::is_assignable>::value
        );

        return *this;
    }

private:

    template <typename Val>
    void _assign_from_val_str_vec_mat
    (
        const Val& val,
        const type_category::type&,
        const true_type&
    )
    {
        fill_fromval(val);
    }

    template <typename Str>
    void _assign_from_val_str_vec_mat
    (
        const Str& str,
        const type_category::string&,
        const false_type&
    )
    {
        assign_fromstr(str);
    }

    template <typename Vec>
    void _assign_from_val_str_vec_mat
    (
        const Vec& vec,
        const type_category::vector&,
        const false_type&
    )
    {
        assign_fromvec(vec);
    }

    template <typename Mat>
    void _assign_from_val_str_vec_mat
    (
        const Mat& mat,
        const type_category::matrix&,
        const false_type&
    )
    {
        assign_frommat(mat);
    }

public:

    /// Fills vector value v. WARNING! NOT IMPLEMENTED.
    template <typename Val>
    void fill_fromval (const Val& v);

    // @}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Element accessing.


    // @{

    /// Accesses to matrix item by index (i, j). Constant form.
    /** Accesses without validation of index (i, j). */
    const_reference el (size_type i, size_type j) const
    {
        ARAGELI_ASSERT_0(i < nrows() && j < ncols());
        return mem(i*ncols() + j);
    }

    /// Accesses to matrix item by index (i, j). Non-constant form.
    /** Accesses without validation of index (i, j). */
    reference el (size_type i, size_type j)
    {
        ARAGELI_ASSERT_0(i < nrows() && j < ncols());
        unique();
        return mem(i*ncols() + j);
    }

    /// Accesses to matrix item by index (i, j). Constant form.
    /** The same as el method (const). */
    const_reference operator() (size_type i, size_type j) const
    {
        return el(i, j);
    }

    /// Accesses to matrix item by index (i, j). Non-constant form.
    /** The same as el method (non-const). */
    reference operator() (size_type i, size_type j)
    {
        return el(i, j);
    }

    /// Accesses to matrix item by index (i, j). Constant form.
    /** Accesses with validation of index (i, j).
        If index is invalid function throws 'out_of_range' exception. */
    const_reference at (size_type i, size_type j) const
    {
        if(i > nrows() || j > ncols())
            throw out_of_range();
        else
            return el(i, j);
    }

    /// Accesses to matrix item by index (i, j). Non-constant form.
    /**    Accesses with validation of index (i, j).
        If index is invalid function throws 'out_of_range' exception. */
    reference at (size_type i, size_type j)
    {
        if(i > nrows() || j > ncols())
            throw out_of_range();
        else
            return el(i, j);
    }

    // @}


    void resize (size_type rows_a, size_type cols_a)
    {
        unique_clear();
        mem().resize(rows_a*cols_a);
        rep().rows = rows_a;
        rep().cols = cols_a;
    }


    /// Shrink an allocated memory to minim size to hold all values.
    /** If the shrinking is actually performed, returns true,
        otherwise (if capacity was equal to size) returns false. */
    bool pack ()
    {
        if(size() != capacity())
        {
            do_pack();
            return true;
        }
        else
            return false;
    }


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Some properties access.


    /// Number of rows in the matrix.
    size_type nrows () const
    {
        return rep().rows;
    }

    /// Number of rows in the matrix.
    size_type ncols () const
    {
        return rep().cols;
    }

    /// Number of items in the matrix.
    size_type size () const
    {
        return mem().size();
    }

    /// The maximum from nrows() and ncols().
    /** Note, if we have a vector-like matrix (matrix-row or matrix-col),
        the returned value is the same as size() returns.
        Compare with vector::length(). */
    size_type length () const
    {
        return std::max(nrows(), ncols());
    }

    size_type capacity () const
    {
        return mem().capacity();
    }

    /// Returns true if the matrix is empty.
    /** Returns true if and only if matrix is not contained any items
        (i.e. if size() == 0).  Note that nrows() or ncols() maybe returns
        nonzero value if is_empty() == true.  */
    bool is_empty () const
    {
        ARAGELI_ASSERT_1
        (
            !mem().empty() ||
            (nrows() == 0 || ncols() == 0)
        );
        return mem().empty();
    }

    /// Returns true if the matrix is null-matrix.
    /** Returns true if and only if for each item 'x' of the matrix
        expression value_traits::is_null(x) is true. If the matrix is empty
        then this function returns true. */
    bool is_null () const;

    /// Returns true if the matrix is unit-matrix.
    /** Returns true if and only if for each item 'x' of the matrix
        expression value_traits::is_unit(x) is true. If the matrix is empty
        then this function returns true. */
    bool is_unit () const;

    /// Returns true if the matrix is opposite unit-matrix.
    /** Returns true if and only if for each item 'x' of the matrix
        expression value_traits::is_opposite_unit(x) is true. If the matrix
        is empty then this function returns true. */
    bool is_opposite_unit () const;

    /// Returns true if the matrix is square matrix.
    /** Returns true if nrows() == ncols().  Those dimentions maybe are zero. */
    bool is_square () const
    {
        return nrows() == ncols();
    }


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Some modificatory operations.


    /// Makes opposite matrix.
    /** Calls 'value_traits::opposite' for each item of the matrix.
        Returns reference to the matrix. */
    matrix& opposite ();

    /// Makes inversed matrix.
    /** Inverses this matrix.
        Requirement: the matrix is not singular, otherwise will be throwed
        matrix_is_singular exception.
        Returns reference to the matrix. */
    matrix& inverse ();

    /// Makes matrix with inversed elements.
    /** Calls 'value_traits::inverse' for each item of the matrix.
        Returns reference to the matrix. */
    matrix& each_inverse ();


    template <typename T1>
    matrix& add_scalar (const T1& x);

    template <typename T1>
    matrix& sub_scalar (const T1& x);

    template <typename T1>
    matrix& mul_scalar (const T1& x);

    template <typename T1>
    matrix& div_scalar (const T1& x);

    template <typename T1>
    matrix& mod_scalar (const T1& x);

    template <typename T1>
    matrix& bitand_scalar (const T1& x);

    template <typename T1>
    matrix& bitor_scalar (const T1& x);

    template <typename T1>
    matrix& bitxor_scalar (const T1& x);

    template <typename T1>
    matrix& shl_scalar (const T1& x);

    template <typename T1>
    matrix& shr_scalar (const T1& x);

    template <typename T1>
    matrix& add_matrix (const T1& x)
    {
        return each_add_matrix(x);
    }

    template <typename T1>
    matrix& sub_matrix (const T1& x)
    {
        return each_sub_matrix(x);
    }

    template <typename T1>
    matrix& mul_matrix (const T1& x);

    template <typename T1>
    matrix& div_matrix (const T1& x);

    template <typename T1>
    matrix& mod_matrix (const T1& x)
    {
        return each_mod_matrix(x);
    }

    template <typename T1>
    matrix& bitand_matrix (const T1& x)
    {
        return each_bitand_matrix(x);
    }

    template <typename T1>
    matrix& bitor_matrix (const T1& x)
    {
        return each_bitor_matrix(x);
    }

    template <typename T1>
    matrix& bitxor_matrix (const T1& x)
    {
        return each_bitxor_matrix(x);
    }

    template <typename T1>
    matrix& shl_matrix (const T1& x)
    {
        return each_shl_matrix(x);
    }

    template <typename T1>
    matrix& shr_matrix (const T1& x)
    {
        return each_shr_matrix(x);
    }

    template <typename T1>
    matrix& each_add_matrix (const T1& x);

    template <typename T1>
    matrix& each_sub_matrix (const T1& x);

    template <typename T1>
    matrix& each_mul_matrix (const T1& x);

    template <typename T1>
    matrix& each_div_matrix (const T1& x);

    template <typename T1>
    matrix& each_mod_matrix (const T1& x);

    template <typename T1>
    matrix& each_bitand_matrix (const T1& x);

    template <typename T1>
    matrix& each_bitor_matrix (const T1& x);

    template <typename T1>
    matrix& each_bitxor_matrix (const T1& x);

    template <typename T1>
    matrix& each_shl_matrix (const T1& x);

    template <typename T1>
    matrix& each_shr_matrix (const T1& x);


    /** @name Matrix with scalar assignment operators.
        This group of operators applied the specified operation
        for each item of the matrix with scalar-value 'x'.
        Returns reference to the matrix.
    */
    //@{

    template <typename T1>
    matrix& operator+= (const T1& x)
    {
        return add_scalar(x);
    }

    template <typename T1>
    matrix& operator-= (const T1& x)
    {
        return sub_scalar(x);
    }

    template <typename T1>
    matrix& operator*= (const T1& x)
    {
        return mul_scalar(x);
    }

    template <typename T1>
    matrix& operator/= (const T1& x)
    {
        return div_scalar(x);
    }

    template <typename T1>
    matrix& operator%= (const T1& x)
    {
        return mod_scalar(x);
    }

    template <typename T1>
    matrix& operator&= (const T1& x)
    {
        return bitand_scalar(x);
    }

    template <typename T1>
    matrix& operator|= (const T1& x)
    {
        return bitor_scalar(x);
    }

    template <typename T1>
    matrix& operator^= (const T1& x)
    {
        return bitxor_scalar(x);
    }

    template <typename T1>
    matrix& operator<<= (const T1& x)
    {
        return shl_scalar(x);
    }

    template <typename T1>
    matrix& operator>>= (const T1& x)
    {
        return shr_scalar(x);
    }

    // BEGIN OF TEMPORARY EXTENTION

    matrix& operator+= (const T& x)
    {
        return add_scalar(x);
    }

    matrix& operator-= (const T& x)
    {
        return sub_scalar(x);
    }

    matrix& operator*= (const T& x)
    {
        return mul_scalar(x);
    }

    matrix& operator/= (const T& x)
    {
        return div_scalar(x);
    }

    matrix& operator%= (const T& x)
    {
        return mod_scalar(x);
    }

    matrix& operator&= (const T& x)
    {
        return bitand_scalar(x);
    }

    matrix& operator|= (const T& x)
    {
        return bitor_scalar(x);
    }

    matrix& operator^= (const T& x)
    {
        return bitxor_scalar(x);
    }

    matrix& operator<<= (const T& x)
    {
        return shl_scalar(x);
    }

    matrix& operator>>= (const T& x)
    {
        return shr_scalar(x);
    }

    // END OF TEMPORARY EXTENTION


    // @}


    /** @name Matrix with matrix assignment operators.
        This group of operators applied the specified operation
        for each pair of the corresponding items of two matrices.
        Returns reference to the first matrix.
    */
    //@{

    template <typename T1, bool REFCNT1>
    matrix& operator+= (const matrix<T1, REFCNT1>& x)
    {
        return add_matrix(x);
    }

    template <typename T1, bool REFCNT1>
    matrix& operator-= (const matrix<T1, REFCNT1>& x)
    {
        return sub_matrix(x);
    }

    template <typename T1, bool REFCNT1>
    matrix& each_add (const matrix<T1, REFCNT1>& x)
    {
        return each_add_matrix(x);
    }

    template <typename T1, bool REFCNT1>
    matrix& each_sub (const matrix<T1, REFCNT1>& x)
    {
        return each_sub_matrix(x);
    }

    template <typename T1, bool REFCNT1>
    matrix& each_mul (const matrix<T1, REFCNT1>& x)
    {
        return each_mul_matrix(x);
    }

    template <typename T1, bool REFCNT1>
    matrix& each_div (const matrix<T1, REFCNT1>& x)
    {
        return each_div_matrix(x);
    }

    template <typename T1, bool REFCNT1>
    matrix& each_mod (const matrix<T1, REFCNT1>& x)
    {
        return each_mod_matrix(x);
    }

    template <typename T1, bool REFCNT1>
    matrix& each_bitand (const matrix<T1, REFCNT1>& x)
    {
        return each_bitand_matrix(x);
    }

    template <typename T1, bool REFCNT1>
    matrix& each_bitor (const matrix<T1, REFCNT1>& x)
    {
        return each_bitor_matrix(x);
    }

    template <typename T1, bool REFCNT1>
    matrix& each_bitxor (const matrix<T1, REFCNT1>& x)
    {
        return each_bitxor_matrix(x);
    }

    template <typename T1, bool REFCNT1>
    matrix& each_shl (const matrix<T1, REFCNT1>& x)
    {
        return each_shl_matrix(x);
    }

    template <typename T1, bool REFCNT1>
    matrix& each_shr (const matrix<T1, REFCNT1>& x)
    {
        return each_shr_matrix(x);
    }

    // @}


    /// Matrix multiplication.
    template <typename T1, bool REFCNT1>
    matrix& operator*= (const matrix<T1, REFCNT1>& x)
    {
        return mul_matrix(x);
    }

    /// Divides one square matrix by another square matrix.
    /** This performs via inversing of the second matrix and multiplication.
        Requirement:
            - x is not singular matrix
            - this matrix and x have the same sizes
            - this matrix and x are square matrices  */
    template <typename T1, bool REFCNT1>
    matrix& operator/= (const matrix<T1, REFCNT1>& x)
    {
        return div_matrix(x);
    }

    /// Returns matrix with opposite values of items of this matrix.
    /** The original matrix stills the same. */
    matrix operator- () const
    {
        matrix res(*this);
        return res.opposite();
    }

    /// Returns reference to this matrix. For sameness only.
    const matrix& operator+ () const
    {
        return *this;
    }

    matrix& operator++ ()
    {
        std::for_each
        (
            begin(),
            end(),
            func::prefix_increment<T, T&>()
        );
        return *this;
    }

    matrix operator++ (int)
    {
        matrix t = *this;
        operator++();
        return t;
    }

    matrix& operator-- ()
    {
        std::for_each(begin(), end(), func::prefix_decrement<T, T&>());
        return *this;
    }

    matrix operator-- (int)
    {
        matrix t = *this;
        operator--();
        return t;
    }


    /// Transposes the matrix.
    void transpose ();

    /// Swaps this matrix with x without actually coping (if possible).
    template <typename T1, bool REFCNT1>
    void swap (matrix<T1, REFCNT1>& x)
    {
        _Internal::swap_help_1
        (
            *this,
            x,
            store,
            x.store,
            equal_types<T, T1>::value
        );
    }


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Access via iterators.


    // @{

    /// Returns iterator on begin of items sequence. Non-constant form.
    /** Calls unique(). */
    iterator begin ()
    {
        unique();
        return mem().begin();
    }

    /// Returns iterator on end of items sequence. Non-constant form.
    /** Calls unique(). */
    iterator end ()
    {
        unique();
        return mem().end();
    }

    /// Returns iterator on begin of items sequence. Constant form.
    const_iterator begin () const
    {
        return mem().begin();
    }

    /// Returns iterator on end of items sequence. Constant form.
    const_iterator end () const
    {
        return mem().end();
    }

    // @}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Working with rows.


    // @{

    /// Inserts new row filled by zeros.
    /** All lower rows will shift down. */
    void insert_row (size_type pos)
    {
        insert_row(pos, null<element_type>());
    }

    /// Inserts new row filled by val at postition pos.
    /** All lower rows will shift down. */
    void insert_row (size_type pos, const T& val);

    /// Inserts new row filled by values from vals at positition pos.
    /** All lower rows will shift down.
        Size of vals must be not less then ncols() value. */
    template <typename T1, bool REFCNT1>
    void insert_row (size_type pos, const vector<T1, REFCNT1>& vals);

    /// Inserts new row filled by values from range [first, first + ncols()) at posititon pos.
    /** All lower rows will shift down. */
    template <typename In>
    void insert_row (size_type pos, In first);

    /// Inserts n new rows filled by val at position pos.
    /** All lower rows will shift down. */
    void insert_rows
    (
        size_type pos,
        size_type n,
        const T& val
    );

    /// Inserts n new rows that each row is filled by values from vals at positition pos.
    /** All lower rows will shift down.
        Size of vals must be not less then ncols() value. */
    template <typename T1, bool REFCNT1>
    void insert_rows
    (
        size_type pos,
        size_type n,
        const vector<T1, REFCNT1>& vals
    );

    /// Inserts n new rows that each row is filled by values from range [first, first + ncols()) at posititon pos.
    /** All lower rows will shift down. */
    template <typename In>
    void insert_rows
    (
        size_type pos,
        size_type n,
        In first
    );

    /// Erases pos-th row.
    void erase_row (size_type pos);

    /// Erases n rows from range [pos, pos + n).
    void erase_rows (size_type pos, size_type n);

    /// Swaps two rows at xpos and ypos.
    void swap_rows (size_type xpos, size_type ypos);

    /// Multiplies row i by value x.
    template <typename T1>
    void mult_row (size_type i, const T1& x);

    /// Divides row i by value x.
    template <typename T1>
    void div_row (size_type i, const T1& x);

    /// Gives row i by modulo x.
    template <typename T1>
    void mod_row (size_type i, const T1& x);

    /// Performs row[dstpos] += row[srcpos].
    void add_rows (size_type dstpos, size_type srcpos);

    /// Performs row[dstpos] -= row[srcpos].
    void sub_rows (size_type dstpos, size_type srcpos);

    /// Performs row[dstpos] += y*row[srcpos].
    template <typename T2>
    void addmult_rows (size_type dstpos, size_type srcpos, const T2& y);

    /// Performs row[dstpos] = x*row[dstpos] + y*row[srcpos]
    template <typename T1, typename T2>
    void addmult_rows
    (
        size_type dstpos,
        const T1& x,
        size_type srcpos,
        const T2& y
    );

    // WARNING! TEMPORARY DECLARATION. Conflicts are possible.
    /// Performs row[dstpos] = x*row[dstpos] + y*row[srcpos]
    template <typename T1, typename T2>
    void addmult_rows
    (
        size_type dstpos,
        const T1& x,
        int srcpos,
        const T2& y
    )
    {
        addmult_rows(dstpos, x, size_type(srcpos), y);
    }

    // WARNING! TEMPORARY DECLARATION. Conflicts are possible.
    /// Performs row[dstpos] = x*row[dstpos] + y*srcvec
    template <typename T1, typename V, typename T2>
    void addmult_rows
    (
        size_type dstpos,
        const T1& x,
        const V& srcvec,
        const T2& y
    );

    /// Returns a copy of the specified row of the matrix.
    vector<T, true> copy_row (size_type i) const
    //{ return vector<T, true>(begin() + i*ncols(), begin() + (i + 1)*ncols()); }
    {
        return vector<T, true>(ncols(), begin() + i*ncols());
    }

    /// Fills the argument of a copy of the specified row of the matrix.
    /** Returns a reference to the second argument (the resulting vector). */
    template <typename V>
    V& copy_row (size_type i, V& res) const
    {
        //res.assign(begin() + i*ncols(), begin() + (i + 1)*ncols());
        res.assign(ncols(), begin() + i*ncols());
        return res;
    }

    template <typename SV>
    matrix<T, true> copy_rows (const SV& sv) const
    {
        matrix<T, true> res;
        return copy_rows(sv, res);
    }

    template <typename SV, typename M>
    M& copy_rows (const SV& sv, M& res) const;

    template <typename SV>
    void erase_rows (const SV& sv);

    template <typename SV, typename M>
    M& take_rows (const SV& sv, M& res)
    {
        copy_rows(sv, res);
        erase_rows(sv);
        return res;
    }

    template <typename SV>
    matrix<T, true> take_rows (const SV& sv)
    {
        matrix<T, true> res;
        return take_rows(sv, res);
    }


    template <typename V>
    V& take_row (size_type i, V& res)
    {
        copy_row(i, res);
        erase_row(i);
        return res;
    }

    vector<T, true> take_row (size_type i)
    {
        vector<T, true> res;
        return take_row(i, res);
    }


    template <typename V>
    void assign_row (size_type i, const V& v)
    {
        ARAGELI_ASSERT_0(i < nrows());
        ARAGELI_ASSERT_0(v.size() >= ncols());
        std::copy(v.begin(), v.begin() + ncols(), begin() + ncols()*i);
    }


    // @}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Working with columns.


    // @{

    /// Inserts new col filled by zeros.
    /** All lower cols will shift down. */
    void insert_col (size_type pos)
    { insert_col(pos, null<element_type>()); }

    /// Inserts new col filled by val at postition pos.
    /** All lower cols will shift down. */
    void insert_col (size_type pos, const T& val);

    /// Inserts new col filled by values from vals at positition pos.
    /** All lower cols will shift down.
        Size of vals must be not less then ncols() value. */
    template <typename T1, bool REFCNT1>
    void insert_col (size_type pos, const vector<T1, REFCNT1>& vals);

    /// Inserts new col filled by values from range [first, first + ncols()) at posititon pos.
    /** All lower cols will shift down. */
    template <typename In>
    void insert_col (size_type pos, In first);

    /// Inserts n new cols filled by val at position pos.
    /** All lower cols will shift down. */
    void insert_cols (size_type pos, size_type n, const T& val);

    /// Inserts n new cols that each col is filled by values from vals at positition pos.
    /** All lower cols will shift down.
        Size of vals must be not less then ncols() value. */
    template <typename T1, bool REFCNT1>
    void insert_cols
    (
        size_type pos,
        size_type n,
        const vector<T1, REFCNT1>& vals
    );

    /// Inserts n new cols that each col is filled by values from range [first, first + ncols()) at posititon pos.
    /** All lower cols will shift down. */
    template <typename In>
    void insert_cols (size_type pos, size_type n, In first);

    /// Erases pos-th col.
    void erase_col (size_type pos);

    /// Erases n cols from range [pos, pos + n).
    void erase_cols (size_type pos, size_type n);

    /// Swaps two cols at xpos and ypos.
    void swap_cols (size_type xpos, size_type ypos);

    /// Multiplies col i by value x.
    template <typename T1>
    void mult_col (size_type i, const T1& x);

    /// Divides col i by value x.
    template <typename T1>
    void div_col (size_type i, const T1& x);

    /// Gives col i by modulo x.
    template <typename T1>
    void mod_col (size_type i, const T1& x);

    /// Performs col[dstpos] += col[srcpos].
    void add_cols (size_type dstpos, size_type srcpos);

    /// Performs col[dstpos] -= col[srcpos].
    void sub_cols (size_type dstpos, size_type srcpos);

    /// Performs col[dstpos] += y*col[srcpos].
    template <typename T2>
    void addmult_cols (size_type dstpos, size_type srcpos, const T2& y);

    /// Performs col[dstpos] = x*col[dstpos] + y*col[srcpos]
    template <typename T1, typename T2>
    void addmult_cols
    (
        size_type dstpos,
        const T1& x,
        size_type srcpos,
        const T2& y
    );

    /// Performs col[dstpos] = x*col[dstpos] + y*col[srcpos]
    template <typename T1, typename T2>
    void addmult_cols
    (
        size_type dstpos,
        const T1& x,
        int srcpos,
        const T2& y
    )
    {
        addmult_cols(dstpos, x, size_type(srcpos), y);
    }

    /// Performs col[dstpos] = x*col[dstpos] + y*srcvec
    template <typename T1, typename V, typename T2>
    void addmult_cols
    (
        size_type dstpos,
        const T1& x,
        const V& srcvec,
        const T2& y
    );

    /// Returns a copy of the specified col of the matrix.
    vector<T, true> copy_col (size_type i) const
    {
        vector<T, true> res;
        return copy_col(i, res);
    }

    /// Fills the argument of a copy of the specified col of the matrix.
    /** Returns a reference to the second argument (the resulting vector). */
    template <typename V>
    V& copy_col (size_type i, V& res) const;

    template <typename SV>
    matrix<T, true> copy_cols (const SV& sv) const
    {
        matrix<T, true> res;
        return copy_cols(sv, res);
    }

    template <typename SV, typename M>
    M& copy_cols (const SV& sv, M& res) const;

    template <typename SV>
    void erase_cols (const SV& sv);

    template <typename SV, typename M>
    M& take_cols (const SV& sv, M& res)
    {
        copy_cols(sv, res);
        erase_cols(sv);
        return res;
    }

    template <typename SV>
    matrix<T, true> take_cols (const SV& sv)
    {
        matrix<T, true> res;
        return take_cols(sv, res);
    }

    template <typename V>
    V& take_col (size_type i, V& res)
    {
        copy_col(i, res);
        erase_col(i);
        return res;
    }

    vector<T, true> take_col (size_type i)
    {
        vector<T, true> res;
        return take_col(i, res);
    }

    template <typename V>
    void assign_col (size_type j, const V& v)
    {
        ARAGELI_ASSERT_0(j < ncols());
        ARAGELI_ASSERT_0(v.size() >= nrows());

        for(size_type i = 0; i < nrows(); ++i)
            el(i, j) = v[i];
    }

    // @}


    template <typename M>
    void insert_matrix_bottom (const M& m)
    {
        ARAGELI_ASSERT_0(m.ncols() == ncols());

        // TODO: Make it faster!
        for(size_type i = 0; i < m.nrows(); ++i)
            insert_row(nrows(), m.copy_row(i));
    }


    template <typename RS, typename CS>
    matrix<T, true> copy_submatrix (const RS& rs, const CS& cs) const
    {
        matrix<T, true> res;
        return copy_submatrix(rs, cs, res);
    }

    template <typename RS, typename CS, typename M>
    M& copy_submatrix (const RS& rs, const CS& cs, M& res) const;

    template <typename RS, typename CS>
    void erase_submatrix (const RS& rs, const CS& cs);

    template <typename RS, typename CS, typename M>
    M& take_submatrix (const RS& rs, const CS& cs, M& res)
    {
        copy_submatrix(rs, cs, res);
        erase_submatrix(rs, cs);
        return res;
    }

    template <typename RS, typename CS>
    matrix<T, true> take_submatrix (const RS& rs, const CS& cs)
    {
        matrix<T, true> res;
        return take_submatrix(rs, cs, res);
    }


    /// Makes an individual representation for this object.
    /**    If ‘refcounting’ is false then this is empty operation.
        Otherwise it makes so that this matrix descriptor
        gets individual copy of internal representation.
        Thus after this operation reference counter value
        is equal to one exactly.
        Returns true if the internal representation object is still the same
        and false if new copy has been made. */
    bool unique ()
    {
        return store.unique();
    }

    /// Makes an individual or a clean individual representation for this object.
    /**    If ‘refcounting’ is false this is empty operation.
        Otherwise if reference counter value is one (only this descriptor
        refers to) then immediate returns true.
        Else create for this descriptor new clean internal representation
        object (empty matrix) and return false. */
    bool unique_clear ()
    {
        return store.unique_clear();
    }

private:

    Rep& rep ()
    {
        return store.value();
    }

    const Rep& rep () const
    {
        return store.value();
    }

    Mem& mem ()
    {
        return rep().mem;
    }

    const Mem& mem () const
    {
        return rep().mem;
    }

    reference mem (size_type i)
    {
        return mem()[i];
    }

    const_reference mem (size_type i) const
    {
        return mem()[i];
    }

    void do_pack ();

    refcntr_proxy<Rep, REFCNT> store;
};


#if 0

/// Specialization of type_pair_traits for big_int and matrix.
template <typename TE2, bool REFCNTE2>
struct type_pair_traits<big_int, matrix<TE2, REFCNTE2> > :
    public
        type_pair_traits_for_unknown_and_vector_matrix
        <
            big_int,
            matrix<TE2, REFCNTE2>
        >
{};

#endif

template <typename T, bool REFCNT>
matrix<T, true> transpose (const matrix<T, REFCNT>& a)
{
    matrix<T, true> res = a;
    res.transpose();
    return res;
}


/// Lexicographical comparision of two matrix.
/** Each matrix is represented as (rows, cols, elements) and on this form
    performs lexicographical comparision, where sequences of elements
    have compared lexicographicaly too.
    Returns
    -   0  if a == b,
    -  -1  if a < b
    -  +1  if a > b    */
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline int cmp
(
    const matrix<T1, REFCNT1>& a,
    const matrix<T2, REFCNT2>& b
)
{
    if(a.nrows() < b.nrows())
        return -1;
    if(a.nrows() > b.nrows())
        return +1;
    if(a.ncols() < b.ncols())
        return -1;
    if(a.ncols() > b.ncols())
        return +1;
    else
        return _Internal::aggregate_cmp(a.begin(), a.end(), b.begin(), b.end());
}



#define _ARAGELI_MATRIX_CMP(OPER)    \
    template    \
    <    \
        typename T1, bool REFCNT1,    \
        typename T2, bool REFCNT2    \
    >    \
    inline bool operator OPER    \
    (const matrix<T1, REFCNT1>& a, const matrix<T2, REFCNT2>& b)    \
    {    \
        return cmp(a, b) OPER 0;    \
    }    \
    \
    template    \
    <    \
        typename T1, bool REFCNT1,    \
        typename T2    \
    >    \
    inline bool operator OPER    \
    (const matrix<T1, REFCNT1>& a, const T2& b)    \
    {    \
        return cmp    \
        (    \
            a,    \
            matrix<T2, false>(a.nrows(), a.ncols, b, fromsize)    \
        ) OPER 0;    \
    }    \
    \
    template    \
    <    \
        typename T1,    \
        typename T2, bool REFCNT2    \
    >    \
    inline bool operator OPER    \
    (const T1& a, const matrix<T2, REFCNT2>& b)    \
    {    \
        return cmp    \
        (    \
            matrix<T2, false>(b.nrows(), b.ncols(), a, fromsize),    \
            b    \
        ) OPER 0;    \
    }


/// @name Standard comparision operators.
// @{

_ARAGELI_MATRIX_CMP(<=)
_ARAGELI_MATRIX_CMP(>=)
_ARAGELI_MATRIX_CMP(<)
_ARAGELI_MATRIX_CMP(>)
_ARAGELI_MATRIX_CMP(==)
_ARAGELI_MATRIX_CMP(!=)

// @}

#undef _ARAGELI_MATRIX_CMP


#define _ARAGELI_GENERIC_MATRIX_OPERATORS1(MNEM, OPERB)    \
template <typename T1, typename T2, typename T3>    \
T3& each_##MNEM##_matrix_matrix (const T1& a, const T2& b, T3& res)    \
{    \
    ARAGELI_ASSERT_0(a.size() == b.size());    \
    ARAGELI_ASSERT_0(res.size() == a.size());    \
    \
    typename T1::const_iterator    \
        ia = a.begin(),    \
        iend = a.end();    \
    typename T2::const_iterator ib = b.begin();    \
    typename T3::iterator ic = res.begin();    \
    \
    for(; ia != iend; ++ia, ++ib, ++ic)    \
        *ic = *ia OPERB *ib;    \
    \
    return res;    \
}    \
    \
template <typename T1, typename T2, typename T3>    \
T3& MNEM##_matrix_scalar (const T1& a, const T2& b, T3& res)    \
{    \
    ARAGELI_ASSERT_0(res.size() == a.size());    \
    \
    typename T1::const_iterator    \
        ia = a.begin(),    \
        iend = a.end();    \
    typename T3::iterator ic = res.begin();    \
    \
    for(; ia != iend; ++ia, ++ic)    \
        *ic = *ia OPERB b;    \
    \
    return res;    \
}    \
    \
template <typename T1, typename T2, typename T3>    \
T3& MNEM##_scalar_matrix (const T1& a, const T2& b, T3& res)    \
{    \
    ARAGELI_ASSERT_0(res.size() == b.size());    \
    \
    typename T2::const_iterator    \
        ib = b.begin(),    \
        iend = b.end();    \
    typename T3::iterator ic = res.begin();    \
    \
    for(; ib != iend; ++ib, ++ic)    \
        *ic = a OPERB *ib;    \
    \
    return res;    \
}


_ARAGELI_GENERIC_MATRIX_OPERATORS1(add, +)
_ARAGELI_GENERIC_MATRIX_OPERATORS1(sub, -)
_ARAGELI_GENERIC_MATRIX_OPERATORS1(mul, *)
_ARAGELI_GENERIC_MATRIX_OPERATORS1(div, /)
_ARAGELI_GENERIC_MATRIX_OPERATORS1(mod, %)
_ARAGELI_GENERIC_MATRIX_OPERATORS1(bitand, &)
_ARAGELI_GENERIC_MATRIX_OPERATORS1(bitor, |)
_ARAGELI_GENERIC_MATRIX_OPERATORS1(bitxor, ^)
_ARAGELI_GENERIC_MATRIX_OPERATORS1(shl, <<)
_ARAGELI_GENERIC_MATRIX_OPERATORS1(shr, >>)


#define _ARAGELI_GENERIC_MATRIX_OPERATORS2(MNEM)    \
template <typename T1, typename T2, typename T3>    \
T3& MNEM##_matrix_matrix (const T1& a, const T2& b, T3& res)    \
{    \
    return each_##MNEM##_matrix_matrix(a, b, res);    \
}

template <typename T1, typename T2, typename T3>
T3& mul_matrix_matrix (const T1& a, const T2& b, T3& res)
{
    res = a; return res.mul_matrix(b);
}

template <typename T1, typename T2, typename T3>
T3& div_matrix_matrix (const T1& a, const T2& b, T3& res)
{
    res = a; return res.div_matrix(b);
}


_ARAGELI_GENERIC_MATRIX_OPERATORS2(add)
_ARAGELI_GENERIC_MATRIX_OPERATORS2(sub)
_ARAGELI_GENERIC_MATRIX_OPERATORS2(mod)
_ARAGELI_GENERIC_MATRIX_OPERATORS2(bitand)
_ARAGELI_GENERIC_MATRIX_OPERATORS2(bitor)
_ARAGELI_GENERIC_MATRIX_OPERATORS2(bitxor)
_ARAGELI_GENERIC_MATRIX_OPERATORS2(shl)
_ARAGELI_GENERIC_MATRIX_OPERATORS2(shr)


#undef _ARAGELI_GENERIC_MATRIX_OPERATORS1
#undef _ARAGELI_GENERIC_MATRIX_OPERATORS2


#define _ARAGELI_MATRIX_BINARY_BY_UNARY(MNEM, OPERB)    \
    template    \
    <    \
        typename T1,    \
        bool REFCNT1,    \
        typename T2,    \
        bool REFCNT2    \
    >    \
    inline matrix<T1, REFCNT1> operator OPERB    \
    (    \
        const matrix<T1, REFCNT1>& a,    \
        const matrix<T2, REFCNT2>& b    \
    )    \
    {    \
        matrix<T1, REFCNT1> t(a.nrows(), a.ncols(), fromsize);    \
        return MNEM##_matrix_matrix(a, b, t);    \
    }    \
    \
    template    \
    <    \
        typename T1,    \
        bool REFCNT1,    \
        typename T2    \
    >    \
    inline matrix<T1, REFCNT1> operator OPERB    \
    (    \
        const matrix<T1, REFCNT1>& a,    \
        const T2& b    \
    )    \
    {    \
        matrix<T1, REFCNT1> t(a.nrows(), a.ncols(), fromsize);    \
        return MNEM##_matrix_scalar(a, b, t);    \
    }    \
    \
    template    \
    <    \
        typename T1,    \
        typename T2,    \
        bool REFCNT2    \
    >    \
    inline matrix<T2, REFCNT2> operator OPERB    \
    (    \
        const T1& a,    \
        const matrix<T2, REFCNT2>& b    \
    )    \
    {    \
        matrix<T2, REFCNT2> t(b.nrows(), b.ncols(), fromsize);    \
        return MNEM##_scalar_matrix(a, b, t);    \
    }    \
    \
    template <typename T1, bool REFCNT1>    \
    inline matrix<T1, REFCNT1> operator OPERB    \
    (    \
        const matrix<T1, REFCNT1>& a,    \
        const T1& b    \
    )    \
    {    \
        matrix<T1, REFCNT1> t(a.nrows(), a.ncols(), fromsize);    \
        return MNEM##_matrix_scalar(a, b, t);    \
    }    \
    \
    template <typename T2, bool REFCNT2>    \
    inline matrix<T2, REFCNT2> operator OPERB    \
    (    \
        const T2& a,    \
        const matrix<T2, REFCNT2>& b    \
    )    \
    {    \
        matrix<T2, REFCNT2> t(b.nrows(), b.ncols(), fromsize);    \
        return MNEM##_scalar_matrix(a, b, t);    \
    }    \
    \
    /* BEGIN OF TEMPORARY EXTENTION */    \
    template <typename T1, bool REFCNT1>    \
    inline matrix<rational<T1>, REFCNT1>    \
    operator OPERB    \
    (    \
        const matrix<rational<T1>, REFCNT1>& a,    \
        const rational<T1>& b    \
    )    \
    {    \
        matrix<rational<T1>, REFCNT1> t(a.nrows(), a.ncols(), fromsize);    \
        return MNEM##_matrix_scalar(a, b, t);    \
    }    \
    \
    template <typename T2, bool REFCNT2>    \
    inline matrix<rational<T2>, REFCNT2>    \
    operator OPERB    \
    (    \
        const rational<T2>& a,    \
        const matrix<rational<T2>, REFCNT2>& b    \
    )    \
    {    \
        matrix<rational<T2>, REFCNT2> t(b.nrows(), b.ncols(), fromsize);    \
        return MNEM##_scalar_matrix(a, b, t);    \
    }    \
    \
    template    \
    <    \
            typename T1,    \
            typename D1,    \
            bool REFCNT1,    \
            bool REFCNT2    \
    >    \
    inline matrix<sparse_polynom<T1, D1, REFCNT1>, REFCNT2>    \
    operator OPERB    \
    (    \
        const matrix<sparse_polynom<T1, D1, REFCNT1>, REFCNT2>& a,    \
        const sparse_polynom<T1, D1, REFCNT1>& b    \
    )    \
    {    \
        matrix<sparse_polynom<T1, D1, REFCNT1>, REFCNT2>    \
            t(a.nrows(), a.ncols(), fromsize);    \
        return MNEM##_matrix_scalar(a, b, t);    \
    }    \
    \
    template    \
    <    \
            typename T1,    \
            typename D1,    \
            bool REFCNT1,    \
            bool REFCNT2    \
    >    \
    inline matrix<sparse_polynom<T1, D1, REFCNT1>, REFCNT2>    \
    operator OPERB    \
    (    \
        const sparse_polynom<T1, D1, REFCNT1>& a,    \
        const matrix<sparse_polynom<T1, D1, REFCNT1>, REFCNT2>& b    \
    )    \
    {    \
        matrix<sparse_polynom<T1, D1, REFCNT1>, REFCNT2>    \
            t(b.nrows(), b.ncols(), fromsize);    \
        return MNEM##_scalar_matrix(a, b, t);    \
    }
    /* END OF TEMPORARY EXTENTION */


/// @name Binary operators with matrices and scalars.
// @{

_ARAGELI_MATRIX_BINARY_BY_UNARY(add, +)
_ARAGELI_MATRIX_BINARY_BY_UNARY(sub, -)
_ARAGELI_MATRIX_BINARY_BY_UNARY(mul, *)
_ARAGELI_MATRIX_BINARY_BY_UNARY(div, /)
_ARAGELI_MATRIX_BINARY_BY_UNARY(mod, %)
_ARAGELI_MATRIX_BINARY_BY_UNARY(bitand, &)
_ARAGELI_MATRIX_BINARY_BY_UNARY(bitor, |)
_ARAGELI_MATRIX_BINARY_BY_UNARY(bitxor, ^)
_ARAGELI_MATRIX_BINARY_BY_UNARY(shl, <<)
_ARAGELI_MATRIX_BINARY_BY_UNARY(shr, >>)

// @}


#undef _ARAGELI_MATRIX_BINARY_BY_UNARY


/// Matrix by vector multiplication.
/** Regards x as columt-vector and performs standard
    matrix by vector multiplication. */
template
<
    typename T1, bool REFCNT1,
    typename T2, bool REFCNT2
>
vector<T2, REFCNT2> operator*
(
    const matrix<T1, REFCNT1>& m,
    const vector<T2, REFCNT2>& x
);


/// Vector by matrix multiplication.
/** Regards x as row-vector and performs standard
    matrix by vector multiplication. */
template
<
    typename T2, bool REFCNT2,
    typename T1, bool REFCNT1
>
vector<T2, REFCNT2> operator*
(
    const vector<T2, REFCNT2>& x,
    const matrix<T1, REFCNT1>& m
);


extern const char* matrix_output_list_first_bracket_default;
extern const char* matrix_output_list_second_bracket_default;
extern const char* matrix_output_list_row_separator_default;
extern const char* matrix_output_list_first_row_bracket_default;
extern const char* matrix_output_list_second_row_bracket_default;
extern const char* matrix_output_list_col_separator_default;
extern const char* matrix_input_list_first_bracket_default;
extern const char* matrix_input_list_second_bracket_default;
extern const char* matrix_input_list_row_separator_default;
extern const char* matrix_input_list_first_row_bracket_default;
extern const char* matrix_input_list_second_row_bracket_default;
extern const char* matrix_input_list_col_separator_default;
extern const char* matrix_output_aligned_left_col_default;
extern const char* matrix_output_aligned_right_col_default;
extern const char* matrix_output_aligned_inter_col_default;


/// Simple output of the matrix.
/** @param out an output stream
    @param x the matrix for outputting */
template <typename T, bool REFCNT>
std::ostream& output_list
(
    std::ostream& out,
    const matrix<T, REFCNT>& x,
    const char* first_bracket = matrix_output_list_first_bracket_default,
    const char* second_bracket = matrix_output_list_second_bracket_default,
    const char* row_separator = matrix_output_list_row_separator_default,
    const char* first_row_bracket = matrix_output_list_first_row_bracket_default,
    const char* second_row_bracket = matrix_output_list_second_row_bracket_default,
    const char* col_separator = matrix_output_list_col_separator_default
);


/// Simple input of the matrix.
template <typename T, bool REFCNT>
std::istream& input_list
(
    std::istream& out,
    matrix<T, REFCNT>& x,
    const char* first_bracket = matrix_input_list_first_bracket_default,
    const char* second_bracket = matrix_input_list_second_bracket_default,
    const char* row_separator = matrix_input_list_row_separator_default,
    const char* first_row_bracket = matrix_input_list_first_row_bracket_default,
    const char* second_row_bracket = matrix_input_list_second_row_bracket_default,
    const char* col_separator = matrix_input_list_col_separator_default
);


/// Aligned output of the matrix.
template <typename T, bool REFCNT>
std::ostream& output_aligned
(
    std::ostream& out,
    const matrix<T, REFCNT>& x,
    const char* left_col = matrix_output_aligned_left_col_default,
    const char* right_col = matrix_output_aligned_right_col_default,
    const char* inter_col = matrix_output_aligned_inter_col_default
);


/// Vertical aligned output of a pair of matrices.
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
std::ostream& output_aligned_ver_pair
(
    std::ostream& out,
    const matrix<T1, REFCNT1>& m1,
    const matrix<T2, REFCNT2>& m2,
    const char* left_col = "|| ",
    const char* right_col = " ||",
    const char* inter_col = " ",
    const char* hsplitter = "-"
);


/// Horizontal aligned output of a pair of matrices.
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
std::ostream& output_aligned_hor_pair
(
    std::ostream& out,
    const matrix<T1, REFCNT1>& m1,
    const matrix<T2, REFCNT2>& m2,
    const char* left_col = "|| ",
    const char* right_col = " ||",
    const char* inter_col = " ",
    const char* vsplitter = " | "
);


/// Aligned output of a triplet of matrices.
/** The matrix m2 is cetral element in the triplet.
    This matrix is placed in buttom-right corner of a gabarite rectangel.
    See implementation at matrix.cpp for details. */
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename T3,
    bool REFCNT3
>
std::ostream& output_aligned_corner_triplet_br
(
    std::ostream& out,
    const matrix<T1, REFCNT1>& m1,
    const matrix<T2, REFCNT2>& m2,
    const matrix<T3, REFCNT3>& m3,
    const char* left_col = "|| ",
    const char* right_col = " ||",
    const char* inter_col = " ",
    const char* vsplitter = " | ",
    const char* hsplitter = "-"
);


/// Standard output routine (default type of outputting) for the matrix.
template <typename T, bool REFCNT>
std::ostream& operator<< (std::ostream& out, const matrix<T, REFCNT>& x)
{
    return output_list(out, x);
}

/// Standard input routine (default type of inputting) for the matrix.
template <typename T, bool REFCNT>
std::istream& operator>> (std::istream& out, matrix<T, REFCNT>& x)
{
    return input_list(out, x);
}


/// WARNING! TEMPORARY DEFINITION!!!
template <typename T, bool REFCNT>
std::ofstream& operator<< (std::ofstream& out, const matrix<T, REFCNT>& x)
{
    return static_cast<std::ofstream&>(output_list(out, x));
}

/// WARNING! TEMPORARY DEFINITION!!!
template <typename T, bool REFCNT>
std::ifstream& operator>> (std::ifstream& out, matrix<T, REFCNT>& x)
{
    return static_cast<std::ifstream&>(input_list(out, x));
}


// BEGIN OF TEMPORARY EXTENTION

template <typename T, bool REFCNT>
std::ostream& operator<< (std::ostringstream& out, const matrix<T, REFCNT>& x)
{
    return output_list(out, x);
}

/// Standard input routine (default type of inputting) for the matrix.
template <typename T, bool REFCNT>
std::istream& operator>> (std::istringstream& out, matrix<T, REFCNT>& x)
{
    return input_list(out, x);
}

// END OF TEMPORARY EXTENTION


/// Specialization of io_binary for matrix.
template <typename T, bool REFCNT>
class io_binary<matrix<T, REFCNT> > :
    public io_binary_base<matrix<T, REFCNT> >
{
public:

    using io_binary_base<matrix<T, REFCNT> >::output_stream;
    using io_binary_base<matrix<T, REFCNT> >::input_stream;
    using io_binary_base<matrix<T, REFCNT> >::calc;
    using io_binary_base<matrix<T, REFCNT> >::input_mem;
    using io_binary_base<matrix<T, REFCNT> >::output_mem;

    /// Stores matrix object state to a binary stream. Seft-delimeted binary serialization.
    /** This functions uses the following format:
            NROWS NCOLS ELEMENTS
        where NROWS and NCOLS are dimentions of a given matrix and ELEMENTS is
        all elements of the matrix stored as an array of length NROWS*NCOLS
        All output is in The Simple Binary format. */
    template <typename Stream>
    static inline Stream& output_stream (Stream& out, const matrix<T, REFCNT>& x)
    {
        output_binary_stream(out, x.nrows());
        output_binary_stream(out, x.ncols());
        if(x.size())
            output_binary_stream(out, &*x.begin(), x.size());
        return out;
    }


    /// Loads matrix object state from a binary stream. Compatible with output_stream.
    /** See output_stream(stream, matrix) function for detailes on the format.
        If the function fails to read some of state components, an old value of x
        may be lost. All depends on input_binary_stream function for T.
        So, do not relay on the value of x when a given stream is not in a good state
        after call returns.

        The function takes input in The Simple Binary format.
    */
    template <typename Stream>
    static Stream& input_stream (Stream& in, matrix<T, REFCNT>& x);


    /// Calculates the number of chars required to store a given matrix object in The Simple Binary form.
    /** This function calculates precise number of chars that will emit
        any function outputs in The Simple Binary format for one rational object,
        for example, output_binary_mem function. */
    static inline std::size_t calc (const matrix<T, REFCNT>& x)
    {
        typename matrix<T, REFCNT>::size_type size = x.size();
        if(size)
            return 2*calc_binary(size) + calc_binary(&*x.begin(), size);
        else
            return 2*calc_binary(size);
    }
};


/// Specialization of the template 'factory' for the Arageli::matrix template.
template <typename T, bool REFCNT>
struct factory<matrix<T, REFCNT> >
{
private:

    typedef matrix<T, REFCNT> TTT;

public:

    static const bool is_specialized = true;

    static const TTT& unit ()
    {
        static const TTT unit_s(1, eye);
        return unit_s;
    }

    static TTT unit (const TTT& pat)
    {
        return TTT(pat.nrows(), Arageli::unit<T>(), diag);
    }

    static const TTT& opposite_unit ()
    {
        static const TTT
            opposite_unit_s(1, Arageli::opposite_unit<T>(), diag);
        return opposite_unit_s;
    }

    static TTT opposite_unit (const TTT& pat)
    {
        return TTT(pat.nrows(), Arageli::opposite_unit<T>(), diag);
    }

    static const TTT& null ()
    {
        static const TTT null_s(1, Arageli::null<T>(), fromval);
        return null_s;
    }

    static TTT null (const TTT& pat)
    {
        return TTT(pat.nrows(), Arageli::null<T>(), diag);
    }

};


template <typename T, bool REFCNT>
inline matrix<T, REFCNT> opposite
(const matrix<T, REFCNT>& x)
{
    return -x;
}

template <typename T, bool REFCNT>
inline matrix<T, REFCNT>& opposite
(const matrix<T, REFCNT>& x, matrix<T, REFCNT>* y)
{
    return (*y = x).opposite();
}

template <typename T, bool REFCNT>
inline matrix<T, REFCNT>& opposite
(matrix<T, REFCNT>* x)
{
    return x->opposite();
}

template <typename T, bool REFCNT>
inline matrix<T, REFCNT>& inverse
(const matrix<T, REFCNT>& x, matrix<T, REFCNT>* y)
{
    return (*y = x).inverse();
}

template <typename T, bool REFCNT>
inline matrix<T, REFCNT>& inverse
(matrix<T, REFCNT>* x)
{
    return x->inverse();
}


template <typename T, bool REFCNT>
inline bool is_unit (const matrix<T, REFCNT>& x)
{
    return x.is_unit();
}

template <typename T, bool REFCNT>
inline bool is_opposite_unit (const matrix<T, REFCNT>& x)
{
    return x.is_opposite_unit();
}

template <typename T, bool REFCNT>
inline bool is_null (const matrix<T, REFCNT>& x)
{
    return x.is_null();
}


template <typename T, bool REFCNT>
struct type_traits<matrix<T, REFCNT> > :
    public type_traits_default<matrix<T, REFCNT> >
{
    static const bool is_specialized = type_traits<T>::is_specialized;

    static const bool is_number = false;
    static const bool is_integer_number = false;
    static const bool is_polynom = false;
    static const bool is_real_number = false;
    static const bool is_rational_number = false;
    static const bool is_complex_number = false;
    static const bool is_ring = type_traits<T>::is_ring;    // +, -, *, null, unit
    static const bool is_field = false;
    static const bool is_finite = type_traits<T>::is_finite;
    static const bool is_additive_group = type_traits<T>::is_additive_group;    // +, -, null
    static const bool is_multiplicative_group =    false;
    static const bool has_zero_divisor = type_traits<T>::has_zero_divisor;
    static const bool is_integer_modulo_ring = false;
    static const bool is_matrix = true;
    static const bool is_vector = false;

    static const bool has_commutative_multiplication =
        type_traits<T>::has_commutative_multiplication &&
        type_traits<T>::has_commutative_addition;

    static const bool has_commutative_addition =
        type_traits<T>::has_commutative_addition;

    static const bool has_null = type_traits<T>::has_null;

    static const bool has_unit =
        type_traits<T>::has_unit &&
        type_traits<T>::has_null;

    static const bool has_opposite_unit =
        type_traits<T>::has_opposite_unit &&
        type_traits<T>::has_null;

    /// True iff type is composite type consists another elements.
    static const bool is_aggregate = true;

    /// Type of each element.
    typedef T element_type;

    typedef type_category::dense_matrix category_type;
    static const category_type category_value;
};


template <typename T, bool REFCNT>
const type_category::dense_matrix
type_traits<matrix<T, REFCNT> >::category_value =
    type_category::dense_matrix();


template <typename T, bool REFCNT>
inline std::ostream& output_polynom_first
(std::ostream& out, const matrix<T, REFCNT>& x)
{
    return output_list(out, x);
}

template <typename T, bool REFCNT>
inline std::ostream& output_polynom_internal
(std::ostream& out, const matrix<T, REFCNT>& x)
{
    return output_list(out << "+", x);
}

template <typename T, bool REFCNT>
inline std::ostream& output_pow
(std::ostream& out, const matrix<T, REFCNT>& x)
{
    return output_list(out, x);
}

template <typename T, bool REFCNT>
inline std::istream& input_polynom_first
(std::istream& in, matrix<T, REFCNT>& x)
{
    return input_list(in, x);
}

template <typename T, bool REFCNT>
std::istream& input_polynom_internal
(std::istream& in, matrix<T, REFCNT>& x);

template <typename T, bool REFCNT>
inline std::istream& input_pow
(std::istream& in, matrix<T, REFCNT>& x)
{
    return input_polynom_first(in, x);
}


/// Makes submatrix from columns of another matrix with particular indexes.
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename T3,
    bool REFCNT3
>
void fill_submatrix_col
(
    const matrix<T1, REFCNT1>& orig,
    const vector<T2, REFCNT2>& indexes,
    matrix<T3, REFCNT3>& res
)
{
    res.assign_fromsize(orig.nrows(), indexes.size());

    for(typename vector<T2, REFCNT2>::size_type j = 0; j < indexes.size(); ++j)
    {
        const T2& index = indexes[j];

        ARAGELI_ASSERT_0(index >= 0);
        ARAGELI_ASSERT_0(index < orig.ncols());

        for(typename matrix<T3, REFCNT3>::size_type i = 0; i < res.nrows(); ++i)
            res(i, j) = orig(i, index);
    }
}


/// Makes submatrix from rows of another matrix with particular indexes.
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename T3,
    bool REFCNT3
>
void fill_submatrix_row
(
    const matrix<T1, REFCNT1>& orig,
    const vector<T2, REFCNT2>& indexes,
    matrix<T3, REFCNT3>& res
)
{
    res.assign_fromsize(indexes.size(), orig.ncols());

    for(typename vector<T2, REFCNT2>::size_type i = 0; i < indexes.size(); ++i)
    {
        const T2& index = indexes[i];

        ARAGELI_ASSERT_0(index >= 0);
        ARAGELI_ASSERT_0(index < orig.nrows());

        for(typename matrix<T3, REFCNT3>::size_type j = 0; j < res.ncols(); ++j)
            res(i, j) = orig(index, j);
    }
}


}


namespace std
{

/// Swaps two matrix without actully copying (if possible).
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline void swap
(
    Arageli::matrix<T1, REFCNT1>& a,
    Arageli::matrix<T2, REFCNT2>& b
)
{
    a.swap(b);
}


/// Swaps two matrix without actully copying (if possible).
template <typename T1, bool REFCNT1>
inline void swap
(
    Arageli::matrix<T1, REFCNT1>& a,
    Arageli::matrix<T1, REFCNT1>& b
)
{
    a.swap(b);
}

}


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_MATRIX
    #include "matrix.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_MATRIX
#endif


#endif
