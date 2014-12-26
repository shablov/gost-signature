/*****************************************************************************

    vector.hpp -- template class vector definition and relative operations.

    This file is a part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2005--2010 Sergey S. Lyalin

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
    Template class vector definition and relative operations.
    This file contains template class vector, relative nonmember functions and
    a specialization of type_traits common template for the vector class.

    This file contains only declarations of all noninline operations.
    Implementation of other operations see in vector.cpp file.

    Indexes of entries in the vector begin from zero.
*/


#ifndef _ARAGELI_vector_hpp_
#define _ARAGELI_vector_hpp_

#include "config.hpp"

#include <cstddef>
#include <vector>
#include <iostream>
#include <fstream>    // for temporary implementation of input/output
#include <sstream>    // for temporary implementation of input/output
#include <limits>
#include <cmath>
#include <algorithm>

#include "frwrddecl.hpp"

#include "type_opers.hpp"
#include "type_traits.hpp"
#include "type_pair_traits.hpp"
#include "function_traits.hpp"
#include "mixcomp.hpp"
#include "exception.hpp"
#include "vecalg.hpp"
#include "refcntr.hpp"
#include "_utility.hpp"
#include "factory.hpp"
#include "cmp.hpp"
#include "io.hpp"
#include "functional.hpp"
#include "big_int.hpp"

#include "subvector/indexed.hpp"

#include "std_import.hpp"


namespace Arageli
{


struct fromseq_t {};
const fromseq_t fromseq = fromseq_t();

struct fromval_t {};
const fromval_t fromval = fromval_t();

struct fromvec_t {};
const fromvec_t fromvec = fromvec_t();

struct fromsize_t {};
const fromsize_t fromsize = fromsize_t();

struct fromstr_t {};
const fromstr_t fromstr = fromstr_t();


template <typename T, bool REFCNT, typename Index>
struct binary_function_traits
<
    function_tag::subscript,
    vector<T, REFCNT>,
    Index
> :
    public vec_binary_function_traits<vector<T, REFCNT>, Index>
{};

template <typename T, bool REFCNT, typename Index>
struct binary_function_traits
<
    function_tag::subscript,
    const vector<T, REFCNT>,
    Index
> :
    public vec_binary_function_traits<const vector<T, REFCNT>, Index>
{};

template <typename T, bool REFCNT, typename Index>
struct binary_function_traits
<
    function_tag::parentheses_1,
    vector<T, REFCNT>,
    Index
> :
    public binary_function_traits
    <
        function_tag::subscript,
        vector<T, REFCNT>,
        Index
    >
{};

template <typename T, bool REFCNT, typename Index>
struct binary_function_traits
<
    function_tag::parentheses_1,
    const vector<T, REFCNT>,
    Index
> :
    public binary_function_traits
    <
        function_tag::subscript,
        const vector<T, REFCNT>,
        Index
    >
{};


template <typename X, bool is_assignable>
struct _element_type_vec_val_1
{
    typedef typename X::element_type type;
};

template <typename X>
struct _element_type_vec_val_1<X, true>
{
    typedef X type;
};

template <typename T, typename X>
struct _element_type_vec_val
{
    typedef typename _element_type_vec_val_1
    <
        X,
        type_pair_traits<X, T>::is_assignable
    >::type
        type;
};


/// Template class vector definition.
/** @param T type of vector's items
    @param REFCNT reference counter switch */
template
<
    typename T,
    bool REFCNT
>
class vector
{
    template <typename T1, bool REFCNT1>
    friend class vector;

    // Rep -- the internal representation of vector values;
    // std::vector -- temporary decision, we need more light-weight
    // vector contatiner (like native array of std::valarray).

    typedef std::vector<T> Rep;

public:


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Standard type definition.


    //@{

    /// The item type. It is T.
    typedef typename Rep::value_type value_type;

    /// The item type. It is T.
    typedef typename Rep::value_type element_type;

    /// Lvalue of item type.
    typedef typename Rep::reference reference;

    /// Const lvalue of item type.
    typedef typename Rep::const_reference const_reference;

    /// Unsigned integer type for size and absolute index representation.
    /** Can represent each non-negative value of difference_type. */
    typedef typename Rep::size_type size_type;

    /// Signed integer type for offsets and relative index representation.
    typedef typename Rep::difference_type difference_type;

    /// Pointer to const item type.
    typedef typename Rep::pointer pointer;

    /// Pointer to item type.
    typedef typename Rep::const_pointer const_pointer;

    /// Iterator type.
    typedef typename Rep::iterator iterator;

    /// Const iterator type.
    typedef typename Rep::const_iterator const_iterator;

    ///// Reverse iterator type.
    //typedef typename Rep::reverse_iterator reverse_iterator;

    ///// Const reverse iterator type.
    //typedef typename Rep::const_reverse_iterator const_reverse_iterator;

    //@}


    /// Defines vector type with other element type and the same reference counter.
    template <typename T1> struct other_element_type
    {
        typedef vector<T1, REFCNT> type;
    };

    /// Defines vector type with other element type ane reference counter.
    template <typename T1, bool REFCNT1> struct other_element_type_refcnt
    {
        typedef vector<T1, REFCNT1> type;
    };

    /// Reference counting property.
    /**    If it is true then counting is enabled else it is disabled. */
    static const bool refcounting = REFCNT;


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Constructors.


    //@{

    /// Creates empty vector, i.e. vector that contains 0 items.
    vector ()
    {}

    // BE CAREFUL! There is one more constructor. It is automaticaly generated
    // copy-constructor. Worry about its meaning is being belonged to other
    // constructors during modifications.

    // ---------------------------------

    /// Creates vector sized by 'size_a' with values factory<T>::null().
    vector (std::size_t size_a, const fromsize_t& select)
    {
        assign_fromsize(size_a);
    }

    /// Creates vector from string noted by usual rules for vectors.
    vector (const char* s, const fromstr_t& select)
    {
        assign_fromstr(s);
    }

    /// Creates vector as a copy of another vector-like structure.
    template <typename Vec>
    vector (const Vec& vec, const fromvec_t& select)
    {
        assign_fromvec(vec);
    }

    /// Mixed constructor: creates from a string, by a size, or from another vector.
    template <typename X>
    vector (const X& x)
    {
        assign(x);
    }

    // ---------------------------------

    /// Creates vector by size size_a with each element equals val.
    template <typename Val>
    vector (std::size_t size_a, const Val& val, const fromval_t& select)
    {
        assign_fromval(size_a, val);
    }

    /// Creates vector from a sequence by size size_a.
    /** @param seq iterator on begin of particular secuence with init values. */
    template <typename Seq>
    vector (std::size_t size_a, const Seq& seq, const fromseq_t& select)
    {
        assign_fromseq(size_a, seq);
    }

    /// Mixed constructor: creates from a value or from a sequence.
    template <typename X>
    vector (std::size_t size_a, const X& x)
    {
        assign(size_a, x);
    }


    //@}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Assignments.


    //@{


    /// Makes vector empty.
    void assign ()
    {
        if(unique_clear())
            rep().clear();
    }

    /// Makes zero-vector with size_a length.
    void assign_fromsize (size_type size_a)
    {
        unique_clear();
        rep().assign(size_a, factory<T>::null());
    }

    /// Parses s as a vector as usual.
    void assign_fromstr (const char* s);

    /// Makes copy of the argument.
    template <typename Vec>
    void assign_fromvec (const Vec& vec)
    {
        unique_clear();

        ARAGELI_ASSERT_0
        (
            reinterpret_cast<const void*>(this) !=
            reinterpret_cast<const void*>(&vec)
        );

        rep().assign(vec.begin(), vec.end());
    }


    /// Mixed assignment: by a size, from a string, or from another vector.
    template <typename X>
    void assign (const X& x)
    {
        _assign_from_size_str_vec(x, type_traits<X>::category_value);
    }

private:

    template <typename Size>
    void _assign_from_size_str_vec
    (const Size& size_a, const type_category::integer&)
    {
        assign_fromsize(size_a);
    }

    template <typename Str>
    void _assign_from_size_str_vec
    (const Str& s, const type_category::string&)
    {
        assign_fromstr(s);
    }

    template <typename Vec>
    void _assign_from_size_str_vec
    (const Vec& vec, const type_category::vector&)
    {
        assign_fromvec(vec);
    }

public:

    /// Makes vector from x, where X can be a value, string or some vector.
    template <typename X>
    vector& operator= (const X& x)
    {
        _assign_from_val_str_vec
        (
            x,
            type_traits<X>::category_value,
            bool_type<type_pair_traits<X, T>::is_assignable>::value
        );

        return *this;
    }

private:

    template <typename Val>
    void _assign_from_val_str_vec
    (const Val& val, const type_category::type&, const true_type&)
    {
        fill_fromval(val);
    }

    template <typename Str>
    void _assign_from_val_str_vec
    (const Str& str, const type_category::string&, const false_type&)
    {
        assign_fromstr(str);
    }

    template <typename Vec>
    void _assign_from_val_str_vec
    (const Vec& vec, const type_category::vector&, const false_type&)
    {
        assign_fromvec(vec);
    }

public:

    // -------------------------------------

    /// Makes vector with size_a length and val as each element.
    template <typename Val>
    void assign_fromval (size_type size_a, const Val& val)
    {
        unique_clear();
        rep().assign(size_a, val);
    }


    /// Makes vector with size_a length and load the values from sequence seq.
    template <typename Seq>
    void assign_fromseq (size_type size_a, Seq seq)
    {
        unique_clear();
        Seq last = seq;
        // TODO: Make the following unrestricted.
        std::advance(last, size_a);    // WARNING! Temporary solution.
        rep().assign(seq, last);
    }


    /// Makes vector with specific length from a value or a sequence.
    template <typename X>
    void assign (size_type size_a, const X& x)
    {
        _assign_from_val_seq
        (
            size_a,
            x,
            bool_type<type_pair_traits<X, T>::is_convertible>::value
        );
    }


private:

    template <typename Val>
    void _assign_from_val_seq
    (size_type size_a, const Val& val, const true_type&)
    {
        assign_fromval(size_a, val);
    }


    template <typename Seq>
    void _assign_from_val_seq
    (size_type size_a, const Seq& seq, const false_type&)
    {
        assign_fromseq(size_a, seq);
    }

public:

    // TODO: Make it faster.
    /// Fills the vector of a value val without length change.
    template <typename Val>
    void fill_fromval (const Val& val)
    {
        assign_fromval(size(), val);
    }

    // TODO: Make it faster.
    /// Fills the vector from a sequence without length change.
    template <typename Seq>
    void fill_fromseq (const Seq& seq)
    {
        assign_fromseq(size(), seq);
    }

    // TODO: Make it faster.
    /// Fills the vector without length change by a value or from a sequence.
    template <typename X>
    void fill (const X& x)
    {
        // WARNING! TEMPORARY SOLUTION!
        assign(size(), x);
    }


    //@}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // @name Element access.


    //@{

    ///    Accesses to vector item by index 'i'. Constant form.
    /**    Accesses without validation of index 'i'. */
    const_reference el (size_type i) const
    {
        ARAGELI_ASSERT_0(i < size());
        return rep(i);
    }

    /// Accesses to vector item by index 'i'. Non-constant form.
    /** Accesses without validation of index 'i'. */
    reference el (size_type i)
    {
        ARAGELI_ASSERT_0(i < size());
        unique();
        return rep(i);
    }

    /// Accesses to vector item by index 'i'. Constant form.
    /**    Accesses with validation of index 'i'.
        If index is invalid function throws 'out_of_range' exception. */
    const_reference at (size_type i) const
    {
        if(i > size())
            throw out_of_range();
        return el(i);
    }

    /// Accesses to vector item by index 'i'. Non-constant form.
    /**    Accesses with validation of index 'i'.
        If index is invalid function throws 'out_of_range' exception. */
    reference at (size_type i)
    {
        if(i > size())
            throw out_of_range();
        return el(i);
    }

    /// Erases element at the position i from the vector and returns its value.
    value_type take (size_type i)
    {
        value_type t = el(i);
        erase(i);
        return t;
    }

    // ---------------------------------------------------------------------

    template <typename Vec>
    indexed_subvector<const vector, Vec> subvector (const Vec& vec) const
    {
        return indexed_subvector<const vector, Vec>(this, vec);
    }

    template <typename Vec>
    indexed_subvector<vector, Vec> subvector (const Vec& vec)
    {
        return indexed_subvector<vector, Vec>(this, vec);
    }

    template <typename Vec>
    indexed_subvector<const vector, Vec> subvector_at (const Vec& vec) const
    {
        if(all_in_range(vec, size_type(0), size_type(size()-1)))
            return subvector(vec);
        else
            throw out_of_range();
    }

    template <typename Vec>
    indexed_subvector<vector, Vec> subvector_at (const Vec& vec)
    {
        if(all_in_range(vec, size_type(0), size_type(size()-1)))
            return subvector(vec);
        else
            throw out_of_range();
    }

    // ---------------------------------------------------------------------

    /// Accesses to vector item by index 'i'. Constant form.
    /** The same as el or subvector methods (const). */
    template <typename X>
    typename binary_function_traits<function_tag::subscript, const vector, X>::result_type
    operator[] (const X& x) const
    {
        //return _operator_sqbrackets_index_vector
        //    (x, type_traits<X>::category_value);
        return vec_operator_sqbrackets_index_vector
            (this, x, type_traits<X>::category_value);
    }


    /// Accesses to vector item by index 'i'. Non-constant form.
    /** The same as el or subvector methods (non-const). */
    template <typename X>
    typename binary_function_traits<function_tag::subscript, vector, X>::result_type
    operator[] (const X& x)
    {
        //return _operator_sqbrackets_index_vector
        //    (x, type_traits<X>::category_value);
        return
            vec_operator_sqbrackets_index_vector
            (
                this,
                x,
                type_traits<X>::category_value
            );
    }


    /// Accesses to vector item by index 'i'. Constant form.
    /** The same as operator[] (const). */
    template <typename X>
    typename binary_function_traits
    <
        function_tag::parentheses_1,
        const vector,
        X
    >::result_type
    operator() (const X& x) const
    {
        return operator[](x);
    }


    /// Accesses to vector item by index 'i'. Non-constant form.
    /** The same as operator[] (non-const). */
    template <typename X>
    typename binary_function_traits
    <
        function_tag::parentheses_1,
        vector,
        X
    >::result_type
    operator() (const X& x)
    {
        return operator[](x);
    }


    const_reference front () const
    {
        return rep().front();
    }

    reference front ()
    {
        unique(); return rep().front();
    }

    const_reference back () const
    {
        return rep().back();
    }

    reference back ()
    {
        unique(); return rep().back();
    }

    //@}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // @name Subvector access.

    //@{

    /// Copies specified subvector and erases it from the original.
    /** @param sv vector with indexes that identified subvector (in particular order)
        @param res target for copying
        Return value just is a reference to res. */
    template <typename SV, typename V>
    V& take_subvector (const SV& sv, V& res)
    {
        copy_subvector(sv, res);
        erase_subvector(sv);
        return res;
    }

    /// Copies specified subvector as the result and erases it from the original.
    /** @param sv vector with indexes that identified subvector (in particular order).
        The result is returned value. */
    template <typename SV>
    vector<T, true> take_subvector (const SV& sv)
    {
        vector<T, true> res;
        return take_subvector(sv, res);
    }

    /// Copies specified subvector without alternating of the original.
    template <typename SV, typename V>
    V& copy_subvector (const SV& sv, V& res) const;

    /// Copies specified subvector without alternating of the original.
    template <typename SV>
    vector<T, true> copy_subvector (const SV& sv) const
    {
        vector<T, true> res;
        return copy_subvector(sv, res);
    }

    /// Erases subvector.
    template <typename SV>
    void erase_subvector (const SV& sv);

    // fill_subvector
    // assign_subvector
    // apply_subvector
    // insert_subvector
    // And special versions of some functions for sorted indexes.

    //@}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Some properties access.


    /// The number of items in the vector.
    size_type size () const
    {
        return rep().size();
    }

    /// The number of items in the vector.
    /** The same as size() function. Compare with matrix::length(). */
    size_type length () const
    {
        return size();
    }

    /// Returns true if the vector is empty.
    /** Returns true if and only if vector is not contained any items
        (i.e. if size() == 0). */
    bool is_empty () const
    {
        return rep().empty();
    }

    /// Returns true if the vector is null-vector.
    /** Returns true if and only if for each item 'x' of the vector
        expression is_null(x) is true. If the vector is empty
        then this function returns true. */
    bool is_null () const;

    /// Returns true if the vector is unit-vector.
    /** Returns true if and only if for each item 'x' of the vector
        expression value_traits::is_unit(x) is true. If the vector is empty
        then this function returns true. */
    bool is_unit () const;

    /// Returns true if the vector is opposite unit-vector.
    /** Returns true if and only if for each item 'x' of the vector
        expression value_traits::is_opposite_unit(x) is true. If the vector
        is empty then this function returns true. */
    bool is_opposite_unit () const;


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Capacity managing.


    /// Size of the storage.
    size_type capacity () const
    {
        return rep().capacity();
    }

    /// Reserve n bytes of memory to store vector items if n < size(), otherwise do nothing.
    void reserve (size_type n)
    {
        unique();
        rep().reserve(n);
    }

    /// Shrink the storage size to eliminate unused cells of the memory.
    /** After this operation size() == cpacity().  Returned value indicates if
        reallocation is actually performed. */
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
    // Some modificatory operations.


    /// Makes opposite vector.
    /** Calls Arageli::opposite for each item of the vector.
        Returns reference to the vector. */
    vector& opposite ();

    /// Makes inversed vector.
    /** Calls Arageli::inverse for each item of the vector.
        Returns reference to the vector. */
    vector& inverse ();

    /// Makes bitwise inversed vector.
    /** Calls operator~ for each item of the vector and store the result to the source.
        Returns reference to the vector. */
    vector& bitwise_not ();

    /// Makes logical negate of the vector.
    /** Calls operator! for each item of the vector and store the result to the source.
        Returns reference to the vector. */
    vector& logical_not ();


    // TODO: Make it faster.
    /// Alternates size of this vector.
    /** New items become undefined. */
    void resize (size_type sz)
    {
        unique();
        rep().resize(sz, T());
    }

    // TODO: Make it faster.
    /// Alternates size of this vector.
    /** New items are equal to val. */
    template <typename Val>
    void resize (size_type sz, const Val& val)
    {
        unique();
        rep().resize(sz, val);
    }


    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    /** @name Vector element modification operators.
        This group of operators applied the specified operation
        for each item of the vector and argument (if any).
        Returns reference to the vector.
    */
    //@{


    template <typename UOp>
    vector& apply (UOp f)
    {
        apply1_wores_vec(*this, f);
        return *this;
    }

    template <typename Val, typename BinAsnOp>
    vector& right_apply_val (const Val& val, BinAsnOp f)
    {
        apply2_wores_vec_by_val(*this, val, f);
        return *this;
    }

    template <typename Val, typename BinAsnOp>
    vector& left_apply_val (const Val& val, BinAsnOp f)
    {
        apply2_wores_val_by_vec(val, *this, f);
        return *this;
    }

    template <typename Vec, typename BinAsnOp>
    vector& right_apply_vec (const Vec& vec, BinAsnOp f)
    {
        apply2_wores_vec_by_vec(*this, vec, f);
        return *this;
    }

    template <typename Vec, typename BinAsnOp>
    vector& left_apply_vec (const Vec& vec, BinAsnOp f)
    {
        apply2_wores_vec_by_vec(vec, *this, f);
        return *this;
    }

    template <typename X, typename BinAsnOp>
    vector& right_apply (const X& x, BinAsnOp f)
    {
        return _right_apply_val_vec
        (
            x,
            f,
            bool_type<type_pair_traits<X, T>::is_assignable>::value
        );
    }

private:

    template <typename X, typename BinAsnOp>
    vector& _right_apply_val_vec (const X& x, BinAsnOp f, const true_type&)
    {
        return right_apply_val(x, f);
    }

    template <typename X, typename BinAsnOp>
    vector& _right_apply_val_vec (const X& x, BinAsnOp f, const false_type&)
    {
        return right_apply_vec(x, f);
    }

public:

    template <typename X, typename Binop>
    vector& left_apply (const X& x, Binop f)
    {
        return _left_apply_val_vec
        (
            x,
            f,
            bool_type<type_pair_traits<X, T>::is_assignable>::value
        );
    }

private:

    template <typename X, typename BinAsnOp>
    vector& _left_apply_val_vec (const X& x, BinAsnOp f, const true_type&)
    {
        return left_apply_val(x, f);
    }

    template <typename X, typename BinAsnOp>
    vector& _left_apply_val_vec (const X& x, BinAsnOp f, const false_type&)
    {
        return left_apply_vec(x, f);
    }

public:


    #define _ARAGELI_VECTOR_ASSIGN_FUNC(MNEM, OPER)    \
        template <typename Val> vector& right_assign_##MNEM##_val (const Val& val)    \
        {    \
            return right_apply_val(val, func::right_assign_##MNEM<T, Val, T&>());    \
        }    \
        template <typename Val> vector& left_assign_##MNEM##_val (const Val& val)    \
        {    \
            return left_apply_val(val, func::left_assign_##MNEM<Val, T, T&>());    \
        }    \
        template <typename Val> vector& assign_##MNEM##_val (const Val& val)    \
        {    \
            return right_apply_val(val, func::assign_##MNEM<T, Val, T&>());    \
        }    \
    \
        template <typename Vec> vector& right_assign_##MNEM##_vec (const Vec& vec)    \
        {    \
            return    \
                right_apply_vec    \
                (    \
                    vec,    \
                    func::right_assign_##MNEM    \
                    <    \
                        T,    \
                        typename type_traits<Vec>::element_type,    \
                        T&    \
                    >()    \
                );    \
        }    \
    \
        template <typename Vec> vector& left_assign_##MNEM##_vec (const Vec& vec)    \
        {    \
            return    \
                left_apply_vec    \
                (    \
                    vec,    \
                    func::left_assign_##MNEM    \
                    <    \
                        typename type_traits<Vec>::element_type,    \
                        T,    \
                        T&    \
                    >()    \
                );    \
        }    \
    \
        template <typename Vec> vector& assign_##MNEM##_vec (const Vec& vec)    \
        {    \
            return    \
                right_apply_vec    \
                (    \
                    vec,    \
                    func::assign_##MNEM    \
                    <    \
                        T,    \
                        typename type_traits<Vec>::element_type,    \
                        T&    \
                    >()    \
                );    \
        }    \
    \
        template <typename X> vector& right_assign_##MNEM (const X& x)    \
        {    \
            return    \
                left_apply    \
                (    \
                    x,    \
                    func::right_assign_##MNEM    \
                    <    \
                        T,    \
                        typename _element_type_vec_val<T, X>::type,    \
                        T&    \
                    >()    \
                );    \
        }    \
    \
        template <typename X> vector& left_assign_##MNEM (const X& x)    \
        {    \
            return    \
                right_apply    \
                (    \
                    x,    \
                    func::left_assign_##MNEM    \
                    <    \
                        typename _element_type_vec_val<T, X>::type,    \
                        T,    \
                        T&    \
                    >()    \
                );    \
        }    \
    \
        template <typename X> vector& assign_##MNEM (const X& x)    \
        {    \
            return    \
                right_apply    \
                (    \
                    x,    \
                    func::assign_##MNEM    \
                    <    \
                        T,    \
                        typename _element_type_vec_val<T, X>::type,    \
                        T&    \
                    >()    \
                );    \
        }    \
    \
        template <typename X> vector& operator OPER (const X& x)    \
        {    \
            return assign_##MNEM(x);    \
        }


    _ARAGELI_VECTOR_ASSIGN_FUNC(plus, +=);
    _ARAGELI_VECTOR_ASSIGN_FUNC(minus, -=);
    _ARAGELI_VECTOR_ASSIGN_FUNC(multiplies, *=);
    _ARAGELI_VECTOR_ASSIGN_FUNC(divides, /=);
    _ARAGELI_VECTOR_ASSIGN_FUNC(modulus, %=);
    _ARAGELI_VECTOR_ASSIGN_FUNC(bitwise_or, |=);
    _ARAGELI_VECTOR_ASSIGN_FUNC(bitwise_and, &=);
    _ARAGELI_VECTOR_ASSIGN_FUNC(bitwise_xor, ^=);
    _ARAGELI_VECTOR_ASSIGN_FUNC(shift_left, <<=);
    _ARAGELI_VECTOR_ASSIGN_FUNC(shift_right, >>=);


    //@}


    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    /// Returns vector with opposite values of items of this vector.
    /** The original vector stills the same. */
    vector operator- () const
    {
        vector res(*this);
        return res.opposite();
    }

    /// Returns reference to this vector. For sameness only.
    const vector& operator+ () const
    {
        return *this;
    }

    /// Returns vector with bitwise inverted of each item of this vector.
    vector operator~ () const
    {
        vector res(*this);
        return res.bitwise_not();
    }

    /// Returns vector with results of applying operator! for each item of this vector.
    bool operator! () const
    {
        return is_null();
    }

    vector& operator++ ()
    {
        apply1_wores_vec(*this, func::prefix_increment<T, T&>());
        return *this;
    }

    vector operator++ (int)
    {
        vector t = *this;
        operator++();
        return t;
    }

    vector& operator-- ()
    {
        apply1_wores_vec(*this, func::prefix_decrement<T, T&>());
        return *this;
    }

    vector operator-- (int)
    {
        vector t = *this;
        operator--();
        return t;
    }


    /// Makes an individual representation for this object.
    /** If ‘refcounting’ is false then this is empty operation.
        Otherwise it makes so that this vector descriptor
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
    /** If ‘refcounting’ is false this is empty operation.
        Otherwise if reference counter value is one (only this descriptor
        refers to) then immediate returns true.
        Else create for this descriptor new clean internal representation
        object (empty vector) and return false. */
    bool unique_clear ()
    {
        return store.unique_clear();
    }


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Access via iterators.
    /** Returned iterators become invalid if the size of the vector or
        the storage area are modified. For instance that can accure after
        call unique, unique_clear, resize, assign, insert, erase, take. */


    // @{

    /// Returns iterator on begin of items sequence. Non-constant form.
    /** Calls unique(). */
    iterator begin ()
    {
        unique();
        return rep().begin();
    }

    /// Returns iterator on end of items sequence. Non-constant form.
    /** Calls unique(). */
    iterator end ()
    {
        unique();
        return rep().end();
    }

    /// Returns iterator on begin of items sequence. Constant form.
    const_iterator begin () const
    {
        return rep().begin();
    }

    /// Returns iterator on end of items sequence. Constant form.
    const_iterator end () const
    {
        return rep().end();
    }

    // @}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Modificatory operations: inserting and erasing.
    /** All functions are divided into two groups. Functions from the first
        group take an iterator as the first parameter. Functions from
        the second group take an integer as the first parameter.
        In both cases the parameter means the position of the start of
        insertion/erasing. If a particular function takes an iterator
        as the first parameter then the function returns an iterator.
        The returned iterator points to the position where the new
        element(s) begins at the vector. */

    // @{

    // To developers. Be careful! Iterators passed to the functions may become
    // invalid after call function unique().

    /// Inserts item with value 'val' before item 'pos' (it's iterator).
    template <typename Val>
    iterator insert_fromval (iterator pos, const Val& val)
    {
        ARAGELI_ASSERT_0(store.refs() == 1);
        ARAGELI_ASSERT_0(rep().begin() <= pos);
        ARAGELI_ASSERT_0(pos <= rep().end());
        return rep().insert(pos, val);
    }

    /// Inserts item with value 'val' before item 'pos' (it's index).
    template <typename Val>
    void insert_fromval (size_type pos, const Val& val)
    {
        insert_fromval(begin() + pos, val);
    }

    template <typename Vec>
    iterator insert_fromvec (iterator pos, const Vec& vec)
    {
        ARAGELI_ASSERT_0(store.refs() == 1);
        ARAGELI_ASSERT_0(rep().begin() <= pos);
        ARAGELI_ASSERT_0(pos <= rep().end());
        size_type i = pos - rep().begin() + vec.size();
        rep().insert(pos, vec.begin(), vec.end());

        // WARNING! IT SHOULD BE CLARIFIED WHAT POSITION WE RETURN WHEN VECTOR IS INSERTED
        return rep().begin() + i;   // the next element after inserted
    }

    template <typename Vec>
    void insert_fromvec (size_type pos, const Vec& vec)
    {
        insert_fromvec(begin() + pos, vec);
    }

    template <typename Str>
    iterator insert_fromstr (iterator pos, const Str& str)
    {
        // TODO: Make it faster.
        return insert_fromvec(pos, vector(str, fromstr));
    }

    template <typename Str>
    void insert_fromstr (size_type pos, const Str& str)
    {
        // TODO: Make it faster.
        insert_fromvec(pos, vector(str, fromstr));
    }


    template <typename X>
    iterator insert (iterator pos, const X& x)
    {
        ARAGELI_ASSERT_0(store.refs() == 1);
        ARAGELI_ASSERT_0(rep().begin() <= pos);
        ARAGELI_ASSERT_0(pos <= rep().end());

        size_type ipos = pos - rep().begin();
        insert(ipos, x);
        return rep().begin() + ipos;
    }

    template <typename X>
    void insert (size_type pos, const X& x)
    {
        _insert_from_val_str_vec
        (
            pos,
            x,
            type_traits<X>::category_value,
            bool_type<type_pair_traits<X, T>::is_assignable>::value
        );
    }

private:

    template <typename Val>
    void _insert_from_val_str_vec
    (size_type pos, const Val& val, const type_category::type&, const true_type&)
    {
        insert_fromval(pos, val);
    }

    template <typename Str>
    void _insert_from_val_str_vec
    (size_type pos, const Str& str, const type_category::string&, const false_type&)
    {
        insert_fromstr(pos, str);
    }

    template <typename Vec>
    void _insert_from_val_str_vec
    (size_type pos, const Vec& vec, const type_category::vector&, const false_type&)
    {
        insert_fromvec(pos, vec);
    }

public:

    template <typename Val>
    iterator insert_fromval (iterator pos, size_type size_a, const Val& val)
    {
        // TODO: Make it faster.
        return insert_fromvec(pos, vector(size_a, val, fromval));
    }

    template <typename Val>
    void insert_fromval (size_type pos, size_type size_a, const Val& val)
    {
        // TODO: Make it faster.
        insert_fromvec(pos, vector(size_a, val, fromval));
    }

    template <typename Seq>
    iterator insert_fromseq (iterator pos, size_type size_a, const Seq& seq)
    {
        // TODO: Make it faster.
        return insert_fromvec(pos, vector(size_a, seq, fromseq));
    }

    template <typename Seq>
    void insert_fromseq (size_type pos, size_type size_a, const Seq& seq)
    {
        // TODO: Make it faster.
        insert_fromvec(pos, vector(size_a, seq, fromseq));
    }


    template <typename X>
    void insert (size_type pos, size_type size_a, const X& x)
    {
        _insert_from_val_seq
        (
            pos,
            size_a,
            x,
            bool_type<type_pair_traits<X, T>::is_convertible>::value
        );
    }

private:

    template <typename Val>
    void _insert_from_val_seq
    (size_type pos, size_type size_a, const Val& val, const true_type&)
    {
        insert_fromval(pos, size_a, val);
    }

    template <typename Seq>
    void _insert_from_val_seq
    (size_type pos, size_type size_a, const Seq& seq, const false_type&)
    {
        insert_fromseq(pos, size_a, seq);
    }

public:

    #if 0   // TODO: CHECK FOR WHY THIS IS DISABLED.
    /**    Inserts item sequence in iterator range [first, last)
        before item 'pos' (it's iterator).
        All elements on and after 'pos' are shifting to back. */
    template <typename In>
    void insert (iterator pos, In first, In last)
    {
        ARAGELI_ASSERT_0(store.refs() == 1);
        ARAGELI_ASSERT_0(rep().begin() <= pos);
        ARAGELI_ASSERT_0(pos <= rep().end());
        return rep().insert(pos, first, last);
    }

    /**    Inserts item sequence in iterator range [first, last)
        before item 'pos' (it's index).
        All elements on and after 'pos' are shifting to back. */
    template <typename In>
    void insert (size_type pos, In first, In last)
    { return insert(begin() + pos, first, last); }

    /**    Inserts 'n' copies of 'val' before item 'pos' (it's iterator).
        All elements on and after 'pos' are shifting to back. */
    void insert (iterator pos, size_type n, const T& val)
    {
        ARAGELI_ASSERT_0(store.refs() == 1);
        ARAGELI_ASSERT_0(rep().begin() <= pos);
        ARAGELI_ASSERT_0(pos <= rep().end());

        rep().insert(pos, n, val);
    }

    /**    Inserts 'n' copies of 'val' before item 'pos' (it's index).
        All elements on and after 'pos' are shifting to back. */
    void insert (size_type pos, size_type n, const T& val)
    { insert(begin() + pos, n, val); }
    #endif

    /// Deletes item by iterator 'pos'.
    iterator erase (iterator pos)
    {
        ARAGELI_ASSERT_0(store.refs() == 1);
        ARAGELI_ASSERT_0(rep().begin() <= pos);
        ARAGELI_ASSERT_0(pos <= rep().end());
        return rep().erase(pos);
    }

    /// Deletes item by index 'pos'.
    void erase (size_type pos)
    {
        erase(begin() + pos);
    }

    /// Deletes items in iterator range [first, last).
    void erase (iterator first, iterator last)
    {
        ARAGELI_ASSERT_0(store.refs() == 1);
        ARAGELI_ASSERT_0(rep().begin() <= first);
        ARAGELI_ASSERT_0(first <= last);
        ARAGELI_ASSERT_0(last <= rep().end());

        // WARNING! HACK FOR VS 2008; see artifact #2870186
        rep().erase(first - begin() + begin(), last - begin() + begin());
    }

    /// Deletes items beginning from index 'pos' and next 'n' items.
    void erase (size_type pos, size_type n)
    {
        erase(begin() + pos, begin() + pos + n);
    }

    /// Inserts 'val' to back of the vector.
    void push_back (const T& val)
    {
        unique();
        rep().push_back(val);
    }

    /// Inserts 'val' to front of the vector.
    void push_front (const T& val) { insert(begin(), val); }

    void pop_back ()
    {
        ARAGELI_ASSERT_0(!is_empty());
        unique();
        rep().pop_back();
    }

    void pop_front ()
    {
        ARAGELI_ASSERT_0(!is_empty());
        erase(size_type(0));
    }

    /// Removes from vector all elements that is equal to v.
    template <typename T2>
    void remove (const T2& v)
    {
        erase(std::remove(begin(), end(), v), end());
    }

    /// Removes from vector all elements for that f returns true.
    template <typename Fu>
    void remove_if (Fu f)
    {
        erase(std::remove_if(begin(), end(), f), end());
    }

    // @}


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Swap utilities.


    // @{

    /// Swaps each other two items of the vector at iterators 'x' and 'y'.
    void swap_els (iterator x, iterator y)
    {
        if(x != y)
        {
            ARAGELI_ASSERT_0(store.refs() == 1);
            ARAGELI_ASSERT_0(rep().begin() <= x);
            ARAGELI_ASSERT_0(x < rep().end());
            ARAGELI_ASSERT_0(rep().begin() <= y);
            ARAGELI_ASSERT_0(y < rep().end());

            std::iter_swap(x, y);
        }
    }

    /// Swaps each other two items of the vector at indexes 'x' and 'y'.
    void swap_els (size_type xpos, size_type ypos)
    {
        if(xpos != ypos)
        {
            unique();
            std::swap(el(xpos), el(ypos));
        }
    }

    /// Swaps this vector with x without actually coping (if possible).
    template <typename T1, bool REFCNT1>
    void swap (vector<T1, REFCNT1>& x)
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

    // @}


private:

    Rep& rep ()
    {
        return store.value();
    }

    const Rep& rep () const
    {
        return store.value();
    }

    reference rep (size_type i)
    {
        return rep()[i];
    }

    const_reference rep (size_type i) const
    {
        return rep()[i];
    }

    void do_pack ();

    refcntr_proxy<Rep, REFCNT> store;
};

///////////////////////////

template <typename T, bool REFCNT>
struct type_traits<vector<T, REFCNT> > :
    public type_traits_default<vector<T, REFCNT> >
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
    static const bool is_matrix = false;
    static const bool is_vector = true;

    static const bool has_commutative_multiplication =
        type_traits<T>::has_commutative_multiplication;

    static const bool has_commutative_addition =
        type_traits<T>::has_commutative_addition;

    static const bool has_null = type_traits<T>::has_null;
    static const bool has_unit = type_traits<T>::has_unit;
    static const bool has_opposite_unit = type_traits<T>::has_opposite_unit;
    static const bool is_aggregate = true;
    typedef T element_type;
    typedef type_category::dense_vector category_type;
    static const category_type category_value;

    template <typename T1, bool REFCNT2>
    struct other_element_type_refcnt
    {
        typedef vector<T1, REFCNT2> type;
    };

};


template <typename T, bool REFCNT>
const type_category::dense_vector
type_traits<vector<T, REFCNT> >::category_value =
    type_category::dense_vector();


template <typename T>
inline vector<T> make_vector (const T& x)
{
    return vector<T>(1, x, fromval);
}

template <typename T>
inline vector<T> make_vector (const T& x1, const T& x2)
{
    vector<T> res;
    res.reserve(2);
    res.push_back(x1);
    res.push_back(x2);
    return res;
}

template <typename T>
inline vector<T> make_vector (const T& x1, const T& x2, const T& x3)
{
    vector<T> res;
    res.reserve(3);
    res.push_back(x1);
    res.push_back(x2);
    res.push_back(x3);
    return res;
}


#define _ARAGELI_VECTOR_RIGHT_OPER(MNEM)    \
    template <typename T1, typename T2, bool REFCNT2>    \
    inline vector<T2, REFCNT2>& left_assign_##MNEM    \
    (const T1& x, vector<T2, REFCNT2>& y)    \
    {    \
        return y.left_assign_##MNEM(x);    \
    }

_ARAGELI_VECTOR_RIGHT_OPER(plus);
_ARAGELI_VECTOR_RIGHT_OPER(minus);
_ARAGELI_VECTOR_RIGHT_OPER(multiplies);
_ARAGELI_VECTOR_RIGHT_OPER(divides);
_ARAGELI_VECTOR_RIGHT_OPER(modulus);
_ARAGELI_VECTOR_RIGHT_OPER(bitwise_or);
_ARAGELI_VECTOR_RIGHT_OPER(bitwise_and);
_ARAGELI_VECTOR_RIGHT_OPER(bitwise_xor);
_ARAGELI_VECTOR_RIGHT_OPER(shift_left);
_ARAGELI_VECTOR_RIGHT_OPER(shift_right);


#undef _ARAGELI_GENERIC_VECTOR_OPERATORS

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template <typename Tag, typename T, bool REFCNT>
struct unary_function_traits<Tag, vector<T, REFCNT> > :
    public unary_function_traits_base
    <
        Tag, vector<T, REFCNT>,
        vector<typename unary_function_traits<Tag, T>::result_type, REFCNT>,
        unary_function_traits<Tag, T>::alternates_argument,
        unary_function_traits<Tag, T>::has_side_effect
    >
{};



namespace _Internal
{

template <typename T1, typename T2>
struct select_non_vector;

template <typename T1, bool REFCNT1, typename T2>
struct select_non_vector<vector<T1, REFCNT1>, T2>
{
    typedef T2 type;
};

template <typename T1, typename T2, bool REFCNT2>
struct select_non_vector<T1, vector<T2, REFCNT2> >
{
    typedef T1 type;
};

}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Helper structures for definition of the gate slot for vector template.
// WARNING! May be should they be placed in the _Internal namespace?

template
<
    typename Tag,
    typename V1,
    typename V2,
    bool IS_CONVERT_12,
    bool IS_CONVERT_21,
    typename Tag_class
>
struct bfgs_vector_helper_1
{
    typedef typename _Internal::select_non_vector<V1, V2>::type NV;

    typedef
        typename binary_function_gate_slot<Tag, NV, V1, V2>::function_traits
            function_traits;

    static inline typename function_traits::result_type
    function (const V1& a, const V2& b)
    {
        return binary_function_gate_slot<Tag, NV, V1, V2>::function(a, b);
    }
};

// For an operation that is not a compare operation.

// WARNING! This specialization triggers very often for not appropriate types.

template <typename Tag, typename V1, typename V2, typename Tag_class>
struct bfgs_vector_helper_1<Tag, V1, V2, false, false, Tag_class>
{
private:

    typedef binary_function_traits
    <
        Tag,
        typename type_traits<V1>::element_type,
        typename type_traits<V2>::element_type
    > Fte;

public:

    struct function_traits :
        public binary_function_traits_base
        <
            // WARNING! We should choose between V1 and V2 as a base for resulting type.
            // (Now just V2 is selected.)
            //Tag, V1, V2, typename V2::template other_element_type<typename Fte::result_type>::type,
            Tag,
            V1,
            V2,
            V1,
            Fte::alternates_first_argument,
            Fte::alternates_second_argument,
            Fte::has_side_effect
        >
    {};

    static inline V1 function (const V1& a, const V2& b)
    {
        return apply2_vec_by_vec(a, b, gfunc::by_tag<Tag>());
    }
};


template <typename Tag, typename V1, typename V2, typename Tag_class>
struct bfgs_vector_helper_1<Tag, V1, V2, false, true, Tag_class>
{
private:

    typedef binary_function_traits
    <
        Tag,
        typename type_traits<V1>::element_type,
        V2
    > Fte;

public:

    struct function_traits :
        public binary_function_traits_base
        <
            //Tag, V1, V2, typename V1::template other_element_type<typename Fte::result_type>::type,
            Tag,
            V1,
            V2,
            V1,
            Fte::alternates_first_argument,
            Fte::alternates_second_argument,
            Fte::has_side_effect
        >
    {};

    static inline V1 function (const V1& a, const V2& b)
    {
        return apply2_vec_by_val(a, b, gfunc::by_tag<Tag>());
    }
};


template <typename Tag, typename V1, typename V2, typename Tag_class>
struct bfgs_vector_helper_1<Tag, V1, V2, true, false, Tag_class>
{
private:

    typedef binary_function_traits
    <
        Tag,
        V1,
        typename type_traits<V2>::element_type
    > Fte;

public:

    struct function_traits :
        public binary_function_traits_base
        <
            //Tag, V1, V2, typename V2::template other_element_type<typename Fte::result_type>::type,
            Tag,
            V1,
            V2,
            V2,
            Fte::alternates_first_argument,
            Fte::alternates_second_argument,
            Fte::has_side_effect
        >
    {};

    static inline V2 function (const V1& a, const V2& b)
    {
        return apply2_val_by_vec(a, b, gfunc::by_tag<Tag>());
    }
};

// For cmp function.

template <typename V1, typename V2, typename Tag_class>
struct bfgs_vector_helper_1<function_tag::cmp, V1, V2, false, false, Tag_class>
{
private:

    typedef binary_function_traits
    <
        function_tag::cmp,
        typename type_traits<V1>::element_type,
        typename type_traits<V2>::element_type
    > Fte;

public:

    struct function_traits :
        public binary_function_traits_base
        <
            function_tag::cmp,
            V1,
            V2,
            int,
            Fte::alternates_first_argument,
            Fte::alternates_second_argument,
            Fte::has_side_effect
        >
    {};

    static inline int function (const V1& a, const V2& b)
    {
        return cmpdef_vec_by_vec(a, b);
    }
};


template <typename V1, typename V2, typename Tag_class>
struct bfgs_vector_helper_1<function_tag::cmp, V1, V2, false, true, Tag_class>
{
private:

    typedef binary_function_traits
    <
        function_tag::cmp,
        typename type_traits<V1>::element_type,
        V2
    > Fte;

public:

    struct function_traits :
        public binary_function_traits_base
        <
            function_tag::cmp,
            V1,
            V2,
            int,
            Fte::alternates_first_argument,
            Fte::alternates_second_argument,
            Fte::has_side_effect
        >
    {};

    static inline int function (const V1& a, const V2& b)
    {
        return cmpdef_vec_by_val(a, b);
    }
};


template <typename V1, typename V2, typename Tag_class>
struct bfgs_vector_helper_1<function_tag::cmp, V1, V2, true, false, Tag_class>
{
private:

    typedef binary_function_traits
    <
        function_tag::cmp,
        V1,
        typename type_traits<V2>::element_type
    > Fte;

public:

    struct function_traits :
        public binary_function_traits_base
        <
            function_tag::cmp,
            V1,
            V2,
            int,
            Fte::alternates_first_argument,
            Fte::alternates_second_argument,
            Fte::has_side_effect
        >
    {};

    static inline int function (const V1& a, const V2& b)
    {
        return cmpdef_val_by_vec(a, b);
    }
};

// For each_cmp function.

template <typename V1, typename V2, typename Tag_class>
struct bfgs_vector_helper_1<function_tag::each_cmp, V1, V2, false, false, Tag_class>
{
private:

    typedef binary_function_traits
    <
        function_tag::each_cmp,
        typename type_traits<V1>::element_type,
        typename type_traits<V2>::element_type
    > Fte;

public:

    struct function_traits :
        public binary_function_traits_base
        <
            function_tag::each_cmp,
            V1,
            V2,
            vector<int, true>,
            Fte::alternates_first_argument,
            Fte::alternates_second_argument,
            Fte::has_side_effect
        >
    {};

    static inline vector<int, true> function (const V1& a, const V2& b)
    {
        return elwisecmp_vec_by_vec(a, b, gfunc::cmp());
    }
};


template <typename V1, typename V2, typename Tag_class>
struct bfgs_vector_helper_1<function_tag::each_cmp, V1, V2, false, true, Tag_class>
{
private:

    typedef binary_function_traits
    <
        function_tag::each_cmp,
        typename type_traits<V1>::element_type,
        V2
    > Fte;

public:

    struct function_traits :
        public binary_function_traits_base
        <
            function_tag::each_cmp,
            V1,
            V2,
            vector<int, true>,
            Fte::alternates_first_argument,
            Fte::alternates_second_argument,
            Fte::has_side_effect
        >
    {};

    static inline vector<int, true> function (const V1& a, const V2& b)
    {
        return elwisecmp_vec_by_val(a, b, gfunc::cmp());
    }
};


template <typename V1, typename V2, typename Tag_class>
struct bfgs_vector_helper_1<function_tag::each_cmp, V1, V2, true, false, Tag_class>
{
private:

    typedef binary_function_traits
    <
        function_tag::each_cmp, V1,
        typename type_traits<V2>::element_type
    > Fte;

public:

    struct function_traits :
        public binary_function_traits_base
        <
            function_tag::each_cmp,
            V1,
            V2,
            vector<int, true>,
            Fte::alternates_first_argument,
            Fte::alternates_second_argument,
            Fte::has_side_effect
        >
    {};

    static inline vector<int, true> function (const V1& a, const V2& b)
    {
        return elwisecmp_val_by_vec(a, b);
    }
};

// For each* functions.

template <typename Tag, typename V1, typename V2>
struct bfgs_vector_helper_1<Tag, V1, V2, false, false, function_tag::each_compare>
{
private:

    typedef binary_function_traits
    <
        Tag,
        typename type_traits<V1>::element_type,
        typename type_traits<V2>::element_type
    > Fte;

public:

    struct function_traits :
        public binary_function_traits_base
        <
            Tag,
            V1,
            V2,
            vector<bool, true>,
            Fte::alternates_first_argument,
            Fte::alternates_second_argument,
            Fte::has_side_effect
        >
    {};

    static inline vector<bool, true> function (const V1& a, const V2& b)
    {
        return eachcmp_vec_by_vec
        (
            a,
            b,
            gfunc::by_tag<typename function_tag::omit_elemwise<Tag>::type>()
        );
    }
};


template <typename Tag, typename V1, typename V2>
struct bfgs_vector_helper_1<Tag, V1, V2, false, true, function_tag::each_compare>
{
private:

    typedef binary_function_traits
    <
        Tag,
        typename type_traits<V1>::element_type,
        V2
    > Fte;

public:

    struct function_traits :
        public binary_function_traits_base
        <
            Tag,
            V1,
            V2,
            vector<bool, true>,
            Fte::alternates_first_argument,
            Fte::alternates_second_argument,
            Fte::has_side_effect
        >
    {};

    static inline vector<bool, true> function (const V1& a, const V2& b)
    {
        return eachcmp_vec_by_val
        (
            a,
            b,
            gfunc::by_tag<typename function_tag::omit_elemwise<Tag>::type>()
        );
    }
};


template <typename Tag, typename V1, typename V2>
struct bfgs_vector_helper_1<Tag, V1, V2, true, false, function_tag::each_compare>
{
private:

    typedef binary_function_traits
    <
        Tag,
        V1,
        typename type_traits<V2>::element_type
    > Fte;

public:

    struct function_traits :
        public binary_function_traits_base
        <
            Tag,
            V1,
            V2,
            vector<bool, true>,
            Fte::alternates_first_argument,
            Fte::alternates_second_argument,
            Fte::has_side_effect
        >
    {};

    static inline vector<bool, true> function (const V1& a, const V2& b)
    {
        return eachcmp_val_by_vec
        (
            a,
            b,
            gfunc::by_tag<typename function_tag::omit_elemwise<Tag>::type>()
        );
    }
};


// For all_X functions.

template <typename Tag, typename V1, typename V2>
struct bfgs_vector_helper_1<Tag, V1, V2, false, false, function_tag::all_compare>
{
private:

    typedef binary_function_traits
    <
        Tag,
        typename type_traits<V1>::element_type,
        typename type_traits<V2>::element_type
    > Fte;

public:

    struct function_traits :
        public binary_function_traits_base
        <
            Tag,
            V1,
            V2,
            bool,
            Fte::alternates_first_argument,
            Fte::alternates_second_argument,
            Fte::has_side_effect
        >
    {};

    static inline bool function (const V1& a, const V2& b)
    {
        return allcmp_vec_by_vec
        (
            a,
            b,
            gfunc::by_tag<typename function_tag::omit_elemwise<Tag>::type>()
        );
    }
};


template <typename Tag, typename V1, typename V2>
struct bfgs_vector_helper_1<Tag, V1, V2, false, true, function_tag::all_compare>
{
private:

    typedef binary_function_traits
    <
        Tag,
        typename type_traits<V1>::element_type,
        V2
    > Fte;

public:

    struct function_traits :
        public binary_function_traits_base
        <
            Tag,
            V1,
            V2,
            bool,
            Fte::alternates_first_argument,
            Fte::alternates_second_argument,
            Fte::has_side_effect
        >
    {};

    static inline bool function (const V1& a, const V2& b)
    {
        return allcmp_vec_by_val
        (
            a,
            b,
            gfunc::by_tag<typename function_tag::omit_elemwise<Tag>::type>()
        );
    }
};


template <typename Tag, typename V1, typename V2>
struct bfgs_vector_helper_1<Tag, V1, V2, true, false, function_tag::all_compare>
{
private:

    typedef binary_function_traits
    <
        Tag, V1,
        typename type_traits<V2>::element_type
    > Fte;

public:

    struct function_traits :
        public binary_function_traits_base
        <
            Tag,
            V1,
            V2,
            bool,
            Fte::alternates_first_argument,
            Fte::alternates_second_argument,
            Fte::has_side_effect
        >
    {};

    static inline bool function (const V1& a, const V2& b)
    {
        return allcmp_val_by_vec
        (
            a,
            b,
            gfunc::by_tag<typename function_tag::omit_elemwise<Tag>::type>()
        );
    }
};

// -------------------------------------------------------

template <typename Tag, typename T1, typename T2>
struct bfgs_vector_helper_2 :
    public bfgs_vector_helper_1
    <
        Tag,
        T1,
        T2,
        type_traits<T2>::is_vector &&
            type_pair_traits
            <
                T1,
                typename type_traits<T2>::element_type
            >::is_convertible,
        type_traits<T1>::is_vector &&
            type_pair_traits
            <
                T2,
                typename type_traits<T1>::element_type
            >::is_convertible,
        typename function_tag::category<Tag>::type
    >
{};


template <typename Tag, typename T1, typename T2>
struct bfgs_vector_helper_3 :
    public bfgs_vector_helper_2
    <
        Tag,
        typename omit_const_ref<T1>::type,
        typename omit_const_ref<T2>::type
    >
{};


// ---------------------------------------------------------------------------
// Definition of the gate slot for mixing computation involving vector template.

template <typename Tag, typename T, bool REFCNT, typename T1, typename T2>
struct binary_function_gate_slot<Tag, vector<T, REFCNT>, T1, T2>:
    public bfgs_vector_helper_3<Tag, T1, T2>
{};

template <typename Tag, typename T, bool REFCNT, typename T1, typename T2>
struct binary_function_gate_slot<Tag, const vector<T, REFCNT>, T1, T2>:
    public bfgs_vector_helper_3<Tag, T1, T2>
{};

template <typename Tag, typename T, bool REFCNT, typename T1, typename T2>
struct binary_function_gate_slot<Tag, vector<T, REFCNT>&, T1, T2>:
    public bfgs_vector_helper_3<Tag, T1, T2>
{};

template <typename Tag, typename T, bool REFCNT, typename T1, typename T2>
struct binary_function_gate_slot<Tag, const vector<T, REFCNT>&, T1, T2>:
    public bfgs_vector_helper_3<Tag, T1, T2>
{};

// ---------------------------------------------------------------------------


// WARNING! Doxygen fails on the following fragment.
// We turn-off it from doxygen processing.
#ifndef DOXYGEN_SHOULD_SKIP_THIS


#define ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_PARAMS    typename T1, bool REFCNT1
#define ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE    const vector<T1, REFCNT1>&
#define ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_PARAMS    typename T2, bool REFCNT2
#define ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE    const vector<T2, REFCNT2>&


#define ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_PARAMS    typename T3
#define ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE        const T3&
#define ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OTHER_COMMA    ,

ARAGELI_ALL_BINARY_ARITHM_FUNCTIONS_MIXGATE
ARAGELI_ALL_BINARY_LOGIC_FUNCTIONS_MIXGATE
ARAGELI_ALL_BINARY_CMP_MIXGATE
ARAGELI_ALL_EACH_ALL_CMP_FUNCTIONS_MIXGATE

#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_PARAMS
#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE
#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OTHER_COMMA


#define ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OWNER_COMMA    ,

ARAGELI_ALL_BINARY_ARITHM_FUNCTIONS_SELF_MIXGATE
ARAGELI_ALL_BINARY_LOGIC_FUNCTIONS_SELF_MIXGATE
ARAGELI_ALL_BINARY_CMP_SELF_MIXGATE
ARAGELI_ALL_EACH_ALL_CMP_FUNCTIONS_SELF_MIXGATE

#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OWNER_COMMA


#define ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_PARAMS
#define ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE        const big_int&
#define ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OTHER_COMMA

ARAGELI_ALL_BINARY_ARITHM_FUNCTIONS_MIXGATE
ARAGELI_ALL_BINARY_LOGIC_FUNCTIONS_MIXGATE
ARAGELI_ALL_BINARY_CMP_MIXGATE
ARAGELI_ALL_EACH_ALL_CMP_FUNCTIONS_MIXGATE

#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_PARAMS
#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE
#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OTHER_COMMA


#define ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_PARAMS    typename T3, typename I3, bool REFCNT3
#define ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE        const sparse_polynom<T3, I3, REFCNT3>
#define ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OTHER_COMMA    ,

ARAGELI_ALL_BINARY_ARITHM_FUNCTIONS_MIXGATE
ARAGELI_ALL_BINARY_LOGIC_FUNCTIONS_MIXGATE
ARAGELI_ALL_BINARY_CMP_MIXGATE
ARAGELI_ALL_EACH_ALL_CMP_FUNCTIONS_MIXGATE

#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_PARAMS
#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE
#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OTHER_COMMA


#define ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_PARAMS    typename T
#define ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE        const rational<T>&
#define ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OTHER_COMMA    ,

ARAGELI_ALL_BINARY_ARITHM_FUNCTIONS_MIXGATE
ARAGELI_ALL_BINARY_LOGIC_FUNCTIONS_MIXGATE
ARAGELI_ALL_BINARY_CMP_MIXGATE
ARAGELI_ALL_EACH_ALL_CMP_FUNCTIONS_MIXGATE

#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_PARAMS
#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_2_TYPE
#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_PARAMS
#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OTHER_TYPE
#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_OTHER_COMMA


#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_PARAMS
#undef ARAGELI_BINARY_FUNCTION_MIXGATE_OWNER_1_TYPE



#endif    // #ifndef DOXYGEN_SHOULD_SKIP_THIS



/// Dot product (scalar product) of two vectors.
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename T1_factory
>
T1 dotprod
(
    const vector<T1, REFCNT1>& a,
    const vector<T2, REFCNT2>& b,
    const T1_factory& t1fctr
);


/// Dot product (scalar product) of two vectors.
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline T1 dotprod
(
    const vector<T1, REFCNT1>& a,
    const vector<T2, REFCNT2>& b
)
{
    return dotprod(a, b, factory<T1>());
}


/// Dot square (scalar square) of vector.
template <typename T1, bool REFCNT1>
inline T1 dotsquare (const vector<T1, REFCNT1>& a)
{ return dotprod(a, a); }


/// Multiplies all elements of vector.
/**    Multiplies all elements of x via operator *= applied on res.
    Returns a reference to res.  */
template <typename T, bool REFCNT, typename T2, typename T2_factory>
T2& product (const vector<T, REFCNT>& x, T2& res, const T2_factory& t2fctr);

/// Sums all elements of vector.
/**    Sums all elements of x via operator += applied on res.
    Returns a reference to res.  */
template <typename T, bool REFCNT, typename T2, typename T2_factory>
T2& sum (const vector<T, REFCNT>& x, T2& res, const T2_factory& t2fctr);

/// Multiplies all elements of vector.
/**    Multiplies all elements of x via operator *= applied on res.
    Returns a reference to res.  */
template <typename T, bool REFCNT, typename T2>
inline T2& product (const vector<T, REFCNT>& x, T2& res)
{
    return product(x, res, factory<T2>());
}

/// Sums all elements of vector.
/**    Sums all elements of x via operator += applied on res.
    Returns a reference to res.  */
template <typename T, bool REFCNT, typename T2>
inline T2& sum (const vector<T, REFCNT>& x, T2& res)
{
    return sum(x, res, factory<T2>());
}

/// Multiplies all elements of vector.
template <typename T, bool REFCNT>
inline T product (const vector<T, REFCNT>& x)
{
    T res;
    return product(x, res, factory<T>());
}

/// Sums all elements of vector.
template <typename T, bool REFCNT>
inline T sum (const vector<T, REFCNT>& x)
{
    T res;
    return sum(x, res, factory<T>());
}


template <typename T1, bool REFCNT1, typename T2, bool REFCNT2>
inline vector<T1, REFCNT1> elwisemax
(
    const vector<T1, REFCNT1>& a,
    const vector<T2, REFCNT2>& b
)
{
    return apply2_vec_by_vec(a, b, gfunc::max());
}

template <typename T1, bool REFCNT1, typename T2, bool REFCNT2>
inline vector<T1, REFCNT1> elwisemin
(
    const vector<T1, REFCNT1>& a,
    const vector<T2, REFCNT2>& b
)
{
    return apply2_vec_by_vec(a, b, gfunc::min());
}

template <typename T1, bool REFCNT1>
inline vector<T1, REFCNT1> elwisemax
(
    const vector<T1, REFCNT1>& a,
    const vector<T1, REFCNT1>& b
)
{
    return apply2_vec_by_vec(a, b, gfunc::max());
}

template <typename T1, bool REFCNT1>
inline vector<T1, REFCNT1> elwisemin
(
    const vector<T1, REFCNT1>& a,
    const vector<T1, REFCNT1>& b
)
{
    return apply2_vec_by_vec(a, b, gfunc::min());
}


// TODO: Migrate all input/output function to vecalg equivalents (begins with vec_).
// By the way, implement them! :)

/// Simple output of the vector.
template <typename Out, typename T, bool REFCNT>
inline Out& output_list
(
    Out& out,
    const vector<T, REFCNT>& x,
    const char* first_bracket = vector_output_list_first_bracket_default,
    const char* second_bracket = vector_output_list_second_bracket_default,
    const char* separator = vector_output_list_separator_default
)
{
    return vec_output_list(out, x, first_bracket, second_bracket, separator);
}


/// Simple input of the vector.
template <typename In, typename T, bool REFCNT>
In& input_list
(
    In& in,
    vector<T, REFCNT>& x,
    const char* first_bracket = vector_input_list_first_bracket_default,
    const char* second_bracket = vector_input_list_second_bracket_default,
    const char* separator = vector_input_list_separator_default,
    const char* range = vector_input_list_range_default
);


/// Aligned output of the vector.
/** The vector `x' is output in column, each its elements is output
    with output_aligned too, so this function provides recursive aligned
    format. If `x' is empty, the function does nothing. From the left
    and from the right there are columns with bounding brackets built
    from strings `left_col' and `right_col' on each line. */
template <typename T, bool REFCNT>
std::ostream& output_aligned
(
    std::ostream& out,
    const vector<T, REFCNT>& x,
    const char* left_col = vector_output_aligned_left_col_default,
    const char* right_col = vector_output_aligned_right_col_default
);


/// Prints vector as a row-vector.
template <typename T, bool REFCNT>
inline std::ostream& vector_output_row
(
    std::ostream& out,
    const vector<T, REFCNT>& x
)
{
    return output_list(out, x, "", "", " ");
}


/// Prints vector as a column-vector.
template <typename T, bool REFCNT>
inline std::ostream& vector_output_col
(
    std::ostream& out,
    const vector<T, REFCNT>& x
)
{
    return output_list(out, x, "", "", "\n");
}


/// Standard output routine (default type of outputting) for the vector.
template <typename T, bool REFCNT>
inline std::ostream& operator<< (std::ostream& out, const vector<T, REFCNT>& x)
{
    return output_list(out, x);
}


/// Standard input routine (default type of inputting) for the vector.
template <typename T, bool REFCNT>
inline std::istream& operator>> (std::istream& in, vector<T, REFCNT>& x)
{
    return input_list(in, x);
}


/// WARNING! TEMPORARY!.
template <typename T, bool REFCNT>
inline std::ofstream& operator<< (std::ofstream& out, const vector<T, REFCNT>& x)
{
    return static_cast<std::ofstream&>(output_list(out, x));
}


/// WARNING! TEMPORARY!.
template <typename T, bool REFCNT>
inline std::ifstream& operator>> (std::ifstream& in, vector<T, REFCNT>& x)
{
    return static_cast<std::ifstream&>(input_list(in, x));
}


/// WARNING! TEMPORARY!.
template <typename T, bool REFCNT>
inline std::ostringstream& operator<< (std::ostringstream& out, const vector<T, REFCNT>& x)
{
    return static_cast<std::ostringstream&>(output_list(out, x));
}


/// WARNING! TEMPORARY!.
template <typename T, bool REFCNT>
inline std::istringstream& operator>> (std::istringstream& in, vector<T, REFCNT>& x)
{
    return static_cast<std::istringstream&>(input_list(in, x));
}


/// Specialization of io_binary for vector.
template <typename T, bool REFCNT>
class io_binary<vector<T, REFCNT> > :
    public io_binary_base<vector<T, REFCNT> >
{
public:

    using io_binary_base<vector<T, REFCNT> >::output_stream;
    using io_binary_base<vector<T, REFCNT> >::input_stream;
    using io_binary_base<vector<T, REFCNT> >::calc;
    using io_binary_base<vector<T, REFCNT> >::input_mem;
    using io_binary_base<vector<T, REFCNT> >::output_mem;

    /// Stores vector object state to a binary stream. Seft-delimeted binary serialization.
    /** This functions uses the following format:
            SIZE ELEMENTS
        where SIZE is a size of a given vector and ELEMENTS is
        all elements of the vector stored as an array of length SIZE
        All output is in The Simple Binary format. */
    template <typename Stream>
    static inline Stream& output_stream (Stream& out, const vector<T, REFCNT>& x)
    {
        typename vector<T, REFCNT>::size_type size = x.size();
        output_binary_stream(out, size);
        if(size)
            output_binary_stream(out, &*x.begin(), size);
        return out;
    }


    /// Loads vector object state from a binary stream. Compatible with output_stream.
    /** See output_stream(stream, vector) function for detailes on the format.
        If the function fails to read some of state components, an old value of x
        may be lost. All depends on input_binary_stream function for T.
        So, do not relay on the value of x when a given stream is not in a good state
        after call returns.

        The function takes input in The Simple Binary format.
    */
    template <typename Stream>
    static Stream& input_stream (Stream& in, vector<T, REFCNT>& x);


    /// Calculates the number of chars required to store a given vector object in The Simple Binary form.
    /** This function calculates precise number of chars that will emit
        any function outputs in The Simple Binary format for one vector object,
        for example, output_binary_mem function. */
    static inline std::size_t calc (const vector<T, REFCNT>& x)
    {
        typename vector<T, REFCNT>::size_type size = x.size();
        if(size)
            return calc_binary(size) + calc_binary(&*x.begin(), size);
        else
            return calc_binary(size);
    }
};


/// Specialization of the template 'factory' for the Arageli::vector template.
template <typename T, bool REFCNT>
struct factory<vector<T, REFCNT> >
{
private:

    typedef vector<T, REFCNT> TTT;

public:

    static const bool is_specialized = true;

    static const TTT& unit ()
    {
        static const TTT unit_s(1, Arageli::unit<T>());
        return unit_s;
    }

    static TTT unit (const TTT& pat)
    {
        if(pat.is_empty())
            return unit();
        else
            return TTT(pat.size(), Arageli::unit<T>(pat.front()));
    }

    static const TTT& opposite_unit ()
    {
        static const TTT opposite_unit_s(1, Arageli::opposite_unit<T>());
        return opposite_unit_s;
    }

    static TTT opposite_unit (const TTT& pat)
    {
        if(pat.is_empty())
            return opposite_unit();
        else
            return TTT(pat.size(), Arageli::opposite_unit<T>(pat.front()));
    }

    static const TTT& null ()
    {
        static const TTT null_s(1, Arageli::null<T>());
        return null_s;
    }

    static TTT null (const TTT& pat)
    {
        if(pat.is_empty())
            return null();
        else
            return TTT(pat.size(), Arageli::null<T>(pat.front()));
    }

};


template <typename T, bool REFCNT>
inline vector<T, REFCNT> opposite
(const vector<T, REFCNT>& x)
{
    return -x;
}

template <typename T, bool REFCNT>
inline vector<T, REFCNT>& opposite
(const vector<T, REFCNT>& x, vector<T, REFCNT>* y)
{
    return (*y = x).opposite();
}

template <typename T, bool REFCNT>
inline vector<T, REFCNT>& opposite
(vector<T, REFCNT>* x)
{
    return x->opposite();
}

template <typename T, bool REFCNT>
inline vector<T, REFCNT> inverse
(const vector<T, REFCNT>& x)
{
    vector<T, REFCNT> t(x);
    return x.inverse();
}

template <typename T, bool REFCNT>
inline vector<T, REFCNT>& inverse
(const vector<T, REFCNT>& x, vector<T, REFCNT>* y)
{
    return (*y = x).inverse();
}

template <typename T, bool REFCNT>
inline vector<T, REFCNT>& inverse
(vector<T, REFCNT>* x)
{
    return x->inverse();
}


template <typename T, bool REFCNT>
inline bool is_unit (const vector<T, REFCNT>& x)
{
    return x.is_unit();
}

template <typename T, bool REFCNT>
inline bool is_opposite_unit (const vector<T, REFCNT>& x)
{
    return x.is_opposite_unit();
}

template <typename T, bool REFCNT>
inline bool is_null (const vector<T, REFCNT>& x)
{
    return x.is_null();
}

template <typename T, bool REFCNT>
inline std::ostream& output_polynom_first
(std::ostream& out, const vector<T, REFCNT>& x)
{
    return output_list(out, x);
}

template <typename T, bool REFCNT>
inline std::ostream& output_polynom_internal
(std::ostream& out, const vector<T, REFCNT>& x)
{
    return output_list(out << "+", x);
}

template <typename T, bool REFCNT>
inline std::ostream& output_pow
(std::ostream& out, const vector<T, REFCNT>& x)
{
    return output_list(out, x);
}

template <typename T, bool REFCNT>
inline std::istream& input_polynom_first
(std::istream& in, vector<T, REFCNT>& x)
{
    return input_list(in, x);
}

template <typename T, bool REFCNT>
std::istream& input_polynom_internal
(std::istream& in, vector<T, REFCNT>& x);

template <typename T, bool REFCNT>
inline std::istream& input_pow
(std::istream& in, vector<T, REFCNT>& x)
{
    return input_polynom_first(in, x);
}




/// Makes subvector from another vector and index vector.
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename T3,
    bool REFCNT3
>
void fill_subvector
(
    const vector<T1, REFCNT1>& orig,
    const vector<T2, REFCNT2>& indexes,
    vector<T3, REFCNT3>& res
)
{
    res.resize(indexes.size());

    for(typename vector<T2, REFCNT2>::size_type i = 0; i < indexes.size(); ++i)
    {
        ARAGELI_ASSERT_0(indexes[i] >= 0);
        ARAGELI_ASSERT_0(indexes[i] < orig.size());

        res[i] = orig[indexes[i]];
    }
}


#define ARAGELI_VECTOR_MATH_FUNCS1_DECL(NAME)    \
    template <typename T, bool REFCNT>    \
    vector<T, true> NAME (const vector<T, REFCNT>& x);

/// @name Standard mathematical functions on vector.
/** Each of this function evaluate specific mathematical function
    of each element in vector and results store to returned vector. */

// @{

ARAGELI_VECTOR_MATH_FUNCS1_DECL(sin)
ARAGELI_VECTOR_MATH_FUNCS1_DECL(cos)
ARAGELI_VECTOR_MATH_FUNCS1_DECL(tan)
ARAGELI_VECTOR_MATH_FUNCS1_DECL(sinh)
ARAGELI_VECTOR_MATH_FUNCS1_DECL(cosh)
ARAGELI_VECTOR_MATH_FUNCS1_DECL(tanh)
ARAGELI_VECTOR_MATH_FUNCS1_DECL(asin)
ARAGELI_VECTOR_MATH_FUNCS1_DECL(acos)
ARAGELI_VECTOR_MATH_FUNCS1_DECL(atan)
ARAGELI_VECTOR_MATH_FUNCS1_DECL(abs)
ARAGELI_VECTOR_MATH_FUNCS1_DECL(exp)
ARAGELI_VECTOR_MATH_FUNCS1_DECL(floor)
ARAGELI_VECTOR_MATH_FUNCS1_DECL(ceil)
ARAGELI_VECTOR_MATH_FUNCS1_DECL(log)
ARAGELI_VECTOR_MATH_FUNCS1_DECL(log10)
ARAGELI_VECTOR_MATH_FUNCS1_DECL(sqrt)

template
<
    typename T1, bool REFCNT1,
    typename T2, bool REFCNT2
>
vector<T1, REFCNT1> pow
(
    const vector<T1, REFCNT1>& a,
    const vector<T2, REFCNT2>& b
);

template
<
    typename T1, bool REFCNT1,
    typename P
>
inline vector<T1, REFCNT1> pow
(
    const vector<T1, REFCNT1>& a,
    const P& b
);


// @}

#undef ARAGELI_VECTOR_MATH_FUNCS1_DECL

} // namespace Arageli


namespace std
{

#define ARAGELI_VECTOR_STD_MATH_FUNCS1(NAME)    \
    template <typename T, bool REFCNT>    \
    inline Arageli::vector<T, true> NAME    \
    (const Arageli::vector<T, REFCNT>& x)    \
    {    \
        return Arageli::NAME(x);    \
    }

/// @name Standard mathematical functions on vector.
/** Each of this function evaluate specific mathematical function
    of each element in vector and results store to returned vector. */

// @{

ARAGELI_VECTOR_STD_MATH_FUNCS1(sin)
ARAGELI_VECTOR_STD_MATH_FUNCS1(cos)
ARAGELI_VECTOR_STD_MATH_FUNCS1(tan)
ARAGELI_VECTOR_STD_MATH_FUNCS1(sinh)
ARAGELI_VECTOR_STD_MATH_FUNCS1(cosh)
ARAGELI_VECTOR_STD_MATH_FUNCS1(tanh)
ARAGELI_VECTOR_STD_MATH_FUNCS1(asin)
ARAGELI_VECTOR_STD_MATH_FUNCS1(acos)
ARAGELI_VECTOR_STD_MATH_FUNCS1(atan)
ARAGELI_VECTOR_STD_MATH_FUNCS1(abs)
ARAGELI_VECTOR_STD_MATH_FUNCS1(exp)
ARAGELI_VECTOR_STD_MATH_FUNCS1(floor)
ARAGELI_VECTOR_STD_MATH_FUNCS1(ceil)
ARAGELI_VECTOR_STD_MATH_FUNCS1(log)
ARAGELI_VECTOR_STD_MATH_FUNCS1(log10)
ARAGELI_VECTOR_STD_MATH_FUNCS1(sqrt)

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline Arageli::vector<T1, REFCNT1> pow
(
    const Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b
)
{
    return Arageli::pow(a, b);
}

template
<
    typename T1,
    bool REFCNT1,
    typename P
>
inline Arageli::vector<T1, REFCNT1> pow
(
    const Arageli::vector<T1, REFCNT1>& a,
    const P& b
)
{
    return Arageli::pow(a, b);
}

// @}

#undef ARAGELI_VECTOR_STD_MATH_FUNCS1


/// Swaps two vectors without actully copying (if possible).
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline void swap
(
    Arageli::vector<T1, REFCNT1>& a,
    Arageli::vector<T2, REFCNT2>& b
)
{
    a.swap(b);
}


/// Swaps two vectors without actully copying (if possible).
template
<
    typename T1,
    bool REFCNT1,
    bool REFCNT2
>
inline void swap
(
    Arageli::vector<T1, REFCNT1>& a,
    Arageli::vector<T1, REFCNT2>& b
)
{
    a.swap(b);
}


/// Swaps two vectors without actully copying (if possible).
template <typename T1, bool REFCNT1>
inline void swap
(
    Arageli::vector<T1, REFCNT1>& a,
    Arageli::vector<T1, REFCNT1>& b
)
{
    a.swap(b);
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/// @name Standard algorithm overloading for Arageli::vector.
/**    See C++ International Standard ISO/IEC 14882:1998(E), 25.1--25.3.
    WARNING.  Not all functions are implemented yet.  */

//@{

// for_each

template <typename T, bool REFCNT, typename Fu>
inline Fu for_each (Arageli::vector<T, REFCNT>& x, Fu f)
{
    return for_each(x.begin(), x.end(), f);
}

template <typename T, bool REFCNT, typename Fu>
inline Fu for_each (const Arageli::vector<T, REFCNT>& x, Fu f)
{
    return for_each(x.begin(), x.end(), f);
}

// find

template <typename T, bool REFCNT, typename T1>
inline typename Arageli::vector<T, REFCNT>::iterator
find (Arageli::vector<T, REFCNT>& x, const T1& val)
{
    return find(x.begin(), x.end(), val);
}

template <typename T, bool REFCNT, typename T1>
inline typename Arageli::vector<T, REFCNT>::const_iterator
find (const Arageli::vector<T, REFCNT>& x, const T1& val)
{
    return find(x.begin(), x.end(), val);
}

// find_if

template <typename T, bool REFCNT, typename Fu>
inline typename Arageli::vector<T, REFCNT>::iterator
find_if (Arageli::vector<T, REFCNT>& x, Fu f)
{
    return find_if(x.begin(), x.end(), f);
}

template <typename T, bool REFCNT, typename Fu>
inline typename Arageli::vector<T, REFCNT>::const_iterator
find_if (const Arageli::vector<T, REFCNT>& x, Fu f)
{
    return find_if(x.begin(), x.end(), f);
}

// find_end

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline typename Arageli::vector<T1, REFCNT1>::iterator
find_end
(
    Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b
)
{
    return find_end(a.begin(), a.end(), b.begin(), b.end());
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline typename Arageli::vector<T1, REFCNT1>::const_iterator
find_end
(
    const Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b
)
{
    return find_end(a.begin(), a.end(), b.begin(), b.end());
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Fb
>
inline typename Arageli::vector<T1, REFCNT1>::iterator
find_end
(
    Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b,
    Fb f
)
{
    return find_end(a.begin(), a.end(), b.begin(), b.end(), f);
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Fb
>
inline typename Arageli::vector<T1, REFCNT1>::const_iterator
find_end
(
    const Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b,
    Fb f
)
{
    return find_end(a.begin(), a.end(), b.begin(), b.end(), f);
}

// find_first_of

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline typename Arageli::vector<T1, REFCNT1>::iterator
find_first_of
(
    Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b
)
{
    return find_first_of(a.begin(), a.end(), b.begin(), b.end());
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline typename Arageli::vector<T1, REFCNT1>::const_iterator
find_first_of
(
    const Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b
)
{
    return find_first_of(a.begin(), a.end(), b.begin(), b.end());
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Fb
>
inline typename Arageli::vector<T1, REFCNT1>::iterator
find_first_of
(
    Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b,
    Fb f
)
{
    return find_first_of(a.begin(), a.end(), b.begin(), b.end(), f);
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Fb
>
inline typename Arageli::vector<T1, REFCNT1>::const_iterator
find_first_of
(
    const Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b,
    Fb f
)
{
    return find_first_of(a.begin(), a.end(), b.begin(), b.end(), f);
}

// adjacent_find

template <typename T, bool REFCNT>
inline typename Arageli::vector<T, REFCNT>::iterator
adjacent_find (Arageli::vector<T, REFCNT>& x)
{
    return adjacent_find(x.begin(), x.end());
}

template <typename T, bool REFCNT>
inline typename Arageli::vector<T, REFCNT>::const_iterator
adjacent_find (const Arageli::vector<T, REFCNT>& x)
{
    return adjacent_find(x.begin(), x.end());
}

template <typename T, bool REFCNT, typename Fb>
inline typename Arageli::vector<T, REFCNT>::iterator
adjacent_find (Arageli::vector<T, REFCNT>& x, Fb f)
{
    return adjacent_find(x.begin(), x.end(), f);
}

template <typename T, bool REFCNT, typename Fb>
inline typename Arageli::vector<T, REFCNT>::const_iterator
adjacent_find (const Arageli::vector<T, REFCNT>& x, Fb f)
{
    return adjacent_find(x.begin(), x.end(), f);
}

// count

template <typename T, bool REFCNT, typename T1>
inline typename Arageli::vector<T, REFCNT>::difference_type
count (const Arageli::vector<T, REFCNT>& x, const T1& val)
{
    return count(x.begin(), x.end(), val);
}

// count_if

template <typename T, bool REFCNT, typename Fu>
inline typename Arageli::vector<T, REFCNT>::difference_type
count_if (const Arageli::vector<T, REFCNT>& x, Fu f)
{
    return count_if(x.begin(), x.end(), f);
}

// mismatch

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline
    pair
    <
        typename Arageli::vector<T1, REFCNT1>::const_iterator,
        typename Arageli::vector<T2, REFCNT2>::const_iterator
    >
mismatch
(
    const Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b
)
{
    return mismatch
    (
        a.begin(),
        a.begin() + min(a.size(), b.size()),
        b.begin()
    );
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Fb
>
inline
    pair
    <
        typename Arageli::vector<T1, REFCNT1>::const_iterator,
        typename Arageli::vector<T2, REFCNT2>::const_iterator
    >
mismatch
(
    const Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b,
    Fb f
)
{
    return mismatch
    (
        a.begin(),
        a.begin() + min(a.size(), b.size()),
        b.begin(), f
    );
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline
    pair
    <
        typename Arageli::vector<T1, REFCNT1>::const_iterator,
        typename Arageli::vector<T2, REFCNT2>::iterator
    >
mismatch
(
    const Arageli::vector<T1, REFCNT1>& a,
    Arageli::vector<T2, REFCNT2>& b
)
{
    return mismatch
    (
        a.begin(),
        a.begin() + min(a.size(), b.size()),
        b.begin()
    );
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Fb
>
inline
    pair
    <
        typename Arageli::vector<T1, REFCNT1>::const_iterator,
        typename Arageli::vector<T2, REFCNT2>::iterator
    >
mismatch
(
    const Arageli::vector<T1, REFCNT1>& a,
    Arageli::vector<T2, REFCNT2>& b,
    Fb f
)
{
    return mismatch
    (
        a.begin(),
        a.begin() + min(a.size(), b.size()),
        b.begin(), f
    );
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline
    pair
    <
        typename Arageli::vector<T1, REFCNT1>::iterator,
        typename Arageli::vector<T2, REFCNT2>::const_iterator
    >
mismatch
(
    Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b
)
{
    return mismatch
    (
        a.begin(),
        a.begin() + min(a.size(), b.size()),
        b.begin()
    );
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Fb
>
inline
    pair
    <
        typename Arageli::vector<T1, REFCNT1>::iterator,
        typename Arageli::vector<T2, REFCNT2>::const_iterator
    >
mismatch
(
    Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b,
    Fb f
)
{
    return mismatch
    (
        a.begin(),
        a.begin() + min(a.size(), b.size()),
        b.begin(),
        f
    );
}

// equal

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline bool equal
(
    const Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b
)
{
    return a == b;
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Fb
>
inline bool equal
(
    const Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b,
    Fb f
)
{
    if(a.size() != b.size())
        return false;
    return equal(a.begin(), a.end(), b.begin(), f);
}

// search

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline typename Arageli::vector<T1, REFCNT1>::const_iterator
search
(
    const Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b
)
{
    return search(a.begin(), a.end(), b.begin(), b.end());
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Fb
>
inline typename Arageli::vector<T1, REFCNT1>::const_iterator
search
(
    const Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b,
    Fb f
)
{
    return search(a.begin(), a.end(), b.begin(), b.end(), f);
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline typename Arageli::vector<T1, REFCNT1>::iterator
search
(
    Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b
)
{
    return search(a.begin(), a.end(), b.begin(), b.end());
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Fb
>
inline typename Arageli::vector<T1, REFCNT1>::iterator
search
(
    Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b,
    Fb f
)
{
    return search(a.begin(), a.end(), b.begin(), b.end(), f);
}

// search_n

template
<
    typename T1, bool REFCNT1,
    typename Size, typename T
>
inline typename Arageli::vector<T1, REFCNT1>::const_iterator
search
(
    const Arageli::vector<T1, REFCNT1>& a,
    Size count, const T& value
)
{
    return search_n(a.begin(), a.end(), count, value);
}


template
<
    typename T1,
    bool REFCNT1,
    typename Size,
    typename T,
    typename Fb
>
inline typename Arageli::vector<T1, REFCNT1>::const_iterator
search
(
    const Arageli::vector<T1, REFCNT1>& a,
    Size count,
    const T& value,
    Fb f
)
{
    return search_n(a.begin(), a.end(), count, value, f);
}


template
<
    typename T1,
    bool REFCNT1,
    typename Size,
    typename T
>
inline typename Arageli::vector<T1, REFCNT1>::iterator
search
(
    Arageli::vector<T1, REFCNT1>& a,
    Size count, const T& value
)
{
    return search_n(a.begin(), a.end(), count, value);
}


template
<
    typename T1,
    bool REFCNT1,
    typename Size,
    typename T,
    typename Fb
>
inline typename Arageli::vector<T1, REFCNT1>::iterator
search
(
    Arageli::vector<T1, REFCNT1>& a,
    Size count,
    const T& value,
    Fb f
)
{
    return search_n(a.begin(), a.end(), count, value, f);
}

// copy

template
<
    typename T1,
    bool REFCNT1,
    typename Out
>
inline Out copy
(
    const Arageli::vector<T1, REFCNT1>& a,
    Out out
)
{
    return copy(a.begin(), a.end(), out);
}

// copy_backward

template
<
    typename T1,
    bool REFCNT1,
    typename Out
>
inline Out copy_backward
(
    const Arageli::vector<T1, REFCNT1>& a,
    Out out
)
{
    return copy_backward(a.begin(), a.end(), out);
}

// transform

/// Applies f to each item of a and stores the result to the same item.
template <typename T1, bool REFCNT1, typename Fu>
inline void transform (Arageli::vector<T1, REFCNT1>& a, Fu f)
{
    return transform(a.begin(), a.end(), a.begin(), f);
}


/// Applies f to each item in a and stores the result to corresponding item of b.
/** Requires: a.size() <= b.size() */
template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Fu
>
inline void transform
(
    const Arageli::vector<T1, REFCNT1>& a,
    Arageli::vector<T2, REFCNT2>& b,
    Fu f
)
{
    ARAGELI_ASSERT_0(a.size() <= b.size());
    return transform(a.begin(), a.end(), b.begin(), f);
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename T3,
    bool REFCNT3,
    typename Fb
>
inline void transform
(
    const Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b,
    Arageli::vector<T3, REFCNT3>& c,
    Fb f
)
{
    ARAGELI_ASSERT_0(a.size() <= b.size() && a.size() <= c.size());
    return transform(a.begin(), a.end(), b.begin(), c.begin(), f);
}

// replace

template
<
    typename T1,
    bool REFCNT1,
    typename T2
>
inline void replace
(
    Arageli::vector<T1, REFCNT1>& a,
    const T2& oldv,
    const T2& newv    // WARNING! Type of oldv must not be equal to type of newv.
)
{
    replace(a.begin(), a.end(), oldv, newv);
}

// replace_if

template
<
    typename T1,
    bool REFCNT1,
    typename Fu,
    typename T2
>
inline void replace_if
(
    Arageli::vector<T1, REFCNT1>& a,
    Fu f,
    const T2& newv
)
{
    replace_if(a.begin(), a.end(), f, newv);
}

// replace_copy

template
<
    typename T1,
    bool REFCNT1,
    typename Out,
    typename T2
>
inline Out replace_copy
(
    Arageli::vector<T1, REFCNT1>& a,
    Out out,
    const T2& oldv,
    const T2& newv    // WARNING! Type of oldv must not be equal to type of newv.
)
{
    return replace_copy(a.begin(), a.end(), out, oldv, newv);
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename T3
>
inline void replace_copy
(
    Arageli::vector<T1, REFCNT1>& a,
    Arageli::vector<T2, REFCNT2>& b,
    const T3& oldv,
    const T3& newv    // WARNING! Type of oldv must not be equal to type of newv.
)
{
    ARAGELI_ASSERT_0(a.size() <= b.size());
    replace_copy(a.begin(), a.end(), b.begin(), oldv, newv);
}

// replace_copy_if

template
<
    typename T1,
    bool REFCNT1,
    typename Out,
    typename Fu,
    typename T2
>
inline Out replace_copy_if
(
    Arageli::vector<T1, REFCNT1>& a,
    Out out,
    Fu f,
    const T2& newv
)
{
    return replace_copy_if(a.begin(), a.end(), out, f, newv);
}


template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Fu,
    typename T3
>
inline void replace_copy_if
(
    Arageli::vector<T1, REFCNT1>& a,
    Arageli::vector<T2, REFCNT2>& b,
    Fu f,
    const T3& newv
)
{
    ARAGELI_ASSERT_0(a.size() <= b.size());
    replace_copy_if(a.begin(), a.end(), b.begin(), f, newv);
}

// fill

template
<
    typename T1,
    bool REFCNT1,
    typename T2
>
inline void fill
(
    Arageli::vector<T1, REFCNT1>& a,
    const T2& v
)
{
    fill(a.begin(), a.end(), v);
}

// generate

template
<
    typename T1, bool REFCNT1,
    typename Fn
>
inline void generate
(
    Arageli::vector<T1, REFCNT1>& a,
    Fn f
)
{
    generate(a.begin(), a.end(), f);
}

// remove

template
<
    typename T1,
    bool REFCNT1,
    typename T2
>
inline typename Arageli::vector<T1, REFCNT1>::iterator
remove
(
    Arageli::vector<T1, REFCNT1>& a,
    const T2& v
)
{
    return remove(a.begin(), a.end(), v);
}

// remove_if

template
<
    typename T1,
    bool REFCNT1,
    typename Fu
>
inline typename Arageli::vector<T1, REFCNT1>::iterator
remove_if
(
    Arageli::vector<T1, REFCNT1>& a,
    Fu f
)
{
    return remove_if(a.begin(), a.end(), f);
}

// remove_copy

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    typename Out
>
inline Out remove_copy
(
    const Arageli::vector<T1, REFCNT1>& a,
    Out out,
    const T2& v
)
{
    return remove_copy(a.begin(), a.end(), out, v);
}


// remove_copy_if

template
<
    typename T1,
    bool REFCNT1,
    typename Fu,
    typename Out
>
inline Out remove_copy
(
    const Arageli::vector<T1, REFCNT1>& a,
    Out out,
    Fu f
)
{
    return remove_copy_if(a.begin(), a.end(), out, f);
}

// unique

template <typename T1, bool REFCNT1>
inline typename Arageli::vector<T1, REFCNT1>::iterator
unique (Arageli::vector<T1, REFCNT1>& a)
{
    return unique(a.begin(), a.end());
}

template <typename T1, bool REFCNT1, typename Fb>
inline typename Arageli::vector<T1, REFCNT1>::iterator
unique (Arageli::vector<T1, REFCNT1>& a, Fb f)
{
    return unique(a.begin(), a.end(), f);
}

// unique_copy

template
<
    typename T1,
    bool REFCNT1,
    typename Out
>
inline Out unique_copy
(const Arageli::vector<T1, REFCNT1>& a, Out out)
{
    return unique_copy(a.begin(), a.end(), out);
}


template
<
    typename T1,
    bool REFCNT1,
    typename Out,
    typename Fu
>
inline Out unique_copy
(const Arageli::vector<T1, REFCNT1>& a, Out out, Fu f)
{
    return unique_copy(a.begin(), a.end(), out, f);
}

// reverse

template <typename T1, bool REFCNT1>
inline void reverse (Arageli::vector<T1, REFCNT1>& a)
{
    reverse(a.begin(), a.end());
}

// reverse_copy

template <typename T1, bool REFCNT1, typename Out>
inline Out reverse_copy (const Arageli::vector<T1, REFCNT1>& a, Out out)
{
    return reverse_copy(a.begin(), a.end(), out);
}

// rotate

template <typename T1, bool REFCNT1>
inline void rotate
(
    Arageli::vector<T1, REFCNT1>& a,
    typename Arageli::vector<T1, REFCNT1>::iterator middle
)
{
    rotate(a.begin(), middle, a.end());
}

template <typename T1, bool REFCNT1>
inline void rotate
(
    Arageli::vector<T1, REFCNT1>& a,
    typename Arageli::vector<T1, REFCNT1>::difference_type middle
)
{
    rotate(a.begin(), a.begin() + middle, a.end());
}

// rotate_copy

template <typename T1, bool REFCNT1, typename Out>
inline Out rotate
(
    Arageli::vector<T1, REFCNT1>& a,
    typename Arageli::vector<T1, REFCNT1>::iterator middle,
    Out out
)
{
    return rotate_copy(a.begin(), middle, a.end(), out);
}

template <typename T1, bool REFCNT1, typename Out>
inline Out rotate
(
    Arageli::vector<T1, REFCNT1>& a,
    typename Arageli::vector<T1, REFCNT1>::difference_type middle,
    Out out
)
{
    rotate_copy(a.begin(), a.begin() + middle, a.end(), out);
}

// random_shuffle

template <typename T1, bool REFCNT1>
inline void random_shuffle (Arageli::vector<T1, REFCNT1>& a)
{
    random_shuffle(a.begin(), a.end());
}

template <typename T1, bool REFCNT1, typename Rand>
inline void random_shuffle
(Arageli::vector<T1, REFCNT1>& a, Rand r)
{
    random_shuffle(a.begin(), a.end(), r);
}

// partition

template <typename T1, bool REFCNT1, typename Fu>
inline typename Arageli::vector<T1, REFCNT1>::iterator
partition (Arageli::vector<T1, REFCNT1>& a, Fu f)
{
    return partition(a.begin(), a.end(), f);
}

// stable_partition

template <typename T1, bool REFCNT1, typename Fu>
inline typename Arageli::vector<T1, REFCNT1>::iterator
stable_partition (Arageli::vector<T1, REFCNT1>& a, Fu f)
{
    return stable_partition(a.begin(), a.end(), f);
}

// sort

template <typename T1, bool REFCNT1>
inline void sort (Arageli::vector<T1, REFCNT1>& a)
{
    sort(a.begin(), a.end());
}

template <typename T1, bool REFCNT1, typename Cmp>
inline void sort (Arageli::vector<T1, REFCNT1>& a, Cmp cmp)
{
    sort(a.begin(), a.end(), cmp);
}

// stable_sort

template <typename T1, bool REFCNT1>
inline void stable_sort (Arageli::vector<T1, REFCNT1>& a)
{
    stable_sort(a.begin(), a.end());
}

template <typename T1, bool REFCNT1, typename Cmp>
inline void stable_sort (Arageli::vector<T1, REFCNT1>& a, Cmp cmp)
{
    stable_sort(a.begin(), a.end(), cmp);
}

// partial_sort

template <typename T1, bool REFCNT1>
inline void partial_sort
(
    Arageli::vector<T1, REFCNT1>& a,
    typename Arageli::vector<T1, REFCNT1>::iterator middle
)
{
    partial_sort(a.begin(), middle, a.end());
}

template <typename T1, bool REFCNT1>
inline void partial_sort
(
    Arageli::vector<T1, REFCNT1>& a,
    typename Arageli::vector<T1, REFCNT1>::difference_type middle
)
{
    partial_sort(a.begin(), a.begin() + middle, a.end());
}

template <typename T1, bool REFCNT1, typename Cmp>
inline void partial_sort
(
    Arageli::vector<T1, REFCNT1>& a,
    typename Arageli::vector<T1, REFCNT1>::iterator middle,
    Cmp cmp
)
{
    partial_sort(a.begin(), middle, a.end(), cmp);
}

template <typename T1, bool REFCNT1, typename Cmp>
inline void partial_sort
(
    Arageli::vector<T1, REFCNT1>& a,
    typename Arageli::vector<T1, REFCNT1>::difference_type middle,
    Cmp cmp
)
{
    partial_sort(a.begin(), a.begin() + middle, a.end(), cmp);
}

// partial_sort_copy

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2
>
inline typename Arageli::vector<T2, REFCNT2>::iterator
partial_sort_copy
(
    const Arageli::vector<T1, REFCNT1>& a,
    Arageli::vector<T2, REFCNT2>& b
)
{
    partial_sort_copy(a.begin(), a.end(), b.begin(), b.end());
}

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Cmp
>
inline typename Arageli::vector<T2, REFCNT2>::iterator
partial_sort_copy
(
    const Arageli::vector<T1, REFCNT1>& a,
    Arageli::vector<T2, REFCNT2>& b,
    Cmp cmp
)
{
    partial_sort_copy(a.begin(), a.end(), b.begin(), b.end(), cmp);
}

// nth_element

template <typename T1, bool REFCNT1>
inline void nth_element
(
    Arageli::vector<T1, REFCNT1>& a,
    typename Arageli::vector<T1, REFCNT1>::iterator nth
)
{
    nth_element(a.begin(), nth, a.end());
}

template <typename T1, bool REFCNT1, typename Cmp>
inline void nth_element
(
    Arageli::vector<T1, REFCNT1>& a,
    typename Arageli::vector<T1, REFCNT1>::iterator nth,
    Cmp cmp
)
{
    nth_element(a.begin(), nth, a.end(), cmp);
}

template <typename T1, bool REFCNT1>
inline void nth_element
(
    Arageli::vector<T1, REFCNT1>& a,
    typename Arageli::vector<T1, REFCNT1>::difference_type nth
)
{
    nth_element(a.begin(), a.begin() + nth, a.end());
}

template <typename T1, bool REFCNT1, typename Cmp>
inline void nth_element
(
    Arageli::vector<T1, REFCNT1>& a,
    typename Arageli::vector<T1, REFCNT1>::difference_type nth,
    Cmp cmp
)
{
    nth_element(a.begin(), a.begin() + nth, a.end(), cmp);
}

// lower_bound

template <typename T1, bool REFCNT1, typename T2>
inline typename Arageli::vector<T1, REFCNT1>::iterator
lower_bound
(
    Arageli::vector<T1, REFCNT1>& a,
    const T2& v
)
{
    lower_bound(a.begin(), a.begin(), v);
}

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    typename Cmp
>
inline typename Arageli::vector<T1, REFCNT1>::iterator
lower_bound
(
    Arageli::vector<T1, REFCNT1>& a,
    const T2& v,
    Cmp cmp
)
{
    lower_bound(a.begin(), a.begin(), v, cmp);
}

template <typename T1, bool REFCNT1, typename T2>
inline typename Arageli::vector<T1, REFCNT1>::const_iterator
lower_bound
(
    const Arageli::vector<T1, REFCNT1>& a,
    const T2& v
)
{
    lower_bound(a.begin(), a.begin(), v);
}

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    typename Cmp
>
inline typename Arageli::vector<T1, REFCNT1>::const_iterator
lower_bound
(
    const Arageli::vector<T1, REFCNT1>& a,
    const T2& v,
    Cmp cmp
)
{
    lower_bound(a.begin(), a.begin(), v, cmp);
}

// upper_bound

template <typename T1, bool REFCNT1, typename T2>
inline typename Arageli::vector<T1, REFCNT1>::iterator
upper_bound
(
    Arageli::vector<T1, REFCNT1>& a,
    const T2& v
)
{
    upper_bound(a.begin(), a.begin(), v);
}

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    typename Cmp
>
inline typename Arageli::vector<T1, REFCNT1>::iterator
upper_bound
(
    Arageli::vector<T1, REFCNT1>& a,
    const T2& v,
    Cmp cmp
)
{
    upper_bound(a.begin(), a.begin(), v, cmp);
}

template <typename T1, bool REFCNT1, typename T2>
inline typename Arageli::vector<T1, REFCNT1>::const_iterator
upper_bound
(
    const Arageli::vector<T1, REFCNT1>& a,
    const T2& v
)
{
    upper_bound(a.begin(), a.begin(), v);
}

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    typename Cmp
>
inline typename Arageli::vector<T1, REFCNT1>::const_iterator
upper_bound
(
    const Arageli::vector<T1, REFCNT1>& a,
    const T2& v,
    Cmp cmp
)
{
    upper_bound(a.begin(), a.begin(), v, cmp);
}

// equal_range

template <typename T1, bool REFCNT1, typename T2>
inline
pair
<
    typename Arageli::vector<T1, REFCNT1>::iterator,
    typename Arageli::vector<T1, REFCNT1>::iterator
>
equal_range
(
    Arageli::vector<T1, REFCNT1>& a,
    const T2& v
)
{
    equal_range(a.begin(), a.begin(), v);
}

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    typename Cmp
>
inline
pair
<
    typename Arageli::vector<T1, REFCNT1>::iterator,
    typename Arageli::vector<T1, REFCNT1>::iterator
>
equal_range
(
    Arageli::vector<T1, REFCNT1>& a,
    const T2& v,
    Cmp cmp
)
{
    equal_range(a.begin(), a.begin(), v, cmp);
}

template <typename T1, bool REFCNT1, typename T2>
inline
pair
<
    typename Arageli::vector<T1, REFCNT1>::const_iterator,
    typename Arageli::vector<T1, REFCNT1>::const_iterator
>
equal_range
(
    const Arageli::vector<T1, REFCNT1>& a,
    const T2& v
)
{
    equal_range(a.begin(), a.begin(), v);
}

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    typename Cmp
>
inline
pair
<
    typename Arageli::vector<T1, REFCNT1>::const_iterator,
    typename Arageli::vector<T1, REFCNT1>::const_iterator
>
equal_range
(
    const Arageli::vector<T1, REFCNT1>& a,
    const T2& v, Cmp cmp
)
{
    equal_range(a.begin(), a.begin(), v, cmp);
}

// binary_search

template <typename T, bool REFCNT, typename T1>
inline bool binary_search
(const Arageli::vector<T, REFCNT>& x, const T1& val)
{
    return binary_search(x.begin(), x.end(), val);
}

template
<
    typename T,
    bool REFCNT,
    typename T1,
    typename Cmp
>
inline bool binary_search
(const Arageli::vector<T, REFCNT>& x, const T1& val, Cmp cmp)
{
    return binary_search(x.begin(), x.end(), val, cmp);
}

// merge

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Out
>
inline Out merge
(
    const Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b,
    Out out
)
{
    merge(a.begin(), a.end(), b.begin(), b.end(), out);
}

template
<
    typename T1,
    bool REFCNT1,
    typename T2,
    bool REFCNT2,
    typename Out,
    typename Cmp
>
inline Out merge
(
    const Arageli::vector<T1, REFCNT1>& a,
    const Arageli::vector<T2, REFCNT2>& b,
    Out out, Cmp cmp
)
{
    merge(a.begin(), a.end(), b.begin(), b.end(), out, cmp);
}

// inplace_merge

template <typename T1, bool REFCNT1, typename Out>
inline Out inplace_merge
(
    Arageli::vector<T1, REFCNT1>& a,
    typename Arageli::vector<T1, REFCNT1>::iterator middle
)
{
    inplace_merge(a.begin(), middle, a.end());
}

template
<
    typename T1,
    bool REFCNT1,
    typename Out,
    typename Cmp
>
inline Out inplace_merge
(
    Arageli::vector<T1, REFCNT1>& a,
    typename Arageli::vector<T1, REFCNT1>::iterator middle,
    Cmp cmp
)
{
    inplace_merge(a.begin(), middle, a.end(), cmp);
}

template <typename T1, bool REFCNT1, typename Out>
inline Out inplace_merge
(
    Arageli::vector<T1, REFCNT1>& a,
    typename Arageli::vector<T1, REFCNT1>::difference_type middle
)
{
    inplace_merge(a.begin(), a.begin() + middle, a.end());
}

template
<
    typename T1,
    bool REFCNT1,
    typename Out,
    typename Cmp
>
inline Out inplace_merge
(
    Arageli::vector<T1, REFCNT1>& a,
    typename Arageli::vector<T1, REFCNT1>::difference_type middle,
    Cmp cmp
)
{
    inplace_merge(a.begin(), a.begin() + middle, a.end(), cmp);
}


// WARNING. Implementation of all other standard algorithm for vector
// must be here. Those are 25.3.5--25.3.9.

//@}


}


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_VECTOR
    #include "vector.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_VECTOR
#endif


#endif  //  #ifndef _ARAGELI_vector_hpp_

