/*****************************************************************************

    subvector/indexed.hpp

    This file is a part of Aragali library.

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
    \file indexed.hpp
    \brief Representation of arbitrary subvector of any    vector by choosing
        a vector of indexes.

    This file contains structures to represent arbitrary subvector of any
    vector. This makes by choosing a vector of indexes to index original
    vector that contain real values. So, structures provided by this file
    are reference structures that don't contain original vector values.
*/


#ifndef _ARAGELI_subvector_indexed_hpp_
#define _ARAGELI_subvector_indexed_hpp_

#include "../config.hpp"

#include "../frwrddecl.hpp"

#include "../std_import.hpp"


namespace Arageli
{




template <typename Base, typename IndexVector, typename Index>
struct binary_function_traits
    <function_tag::subscript, indexed_subvector<Base, IndexVector>, Index>
        : public vec_binary_function_traits
            <indexed_subvector<Base, IndexVector>, Index>
{};
//{
//    static const bool is_specialized = true;
//    typedef indexed_subvector<Base, IndexVector> first_argument_type;
//    typedef Index second_argument_type;
//    static const bool alternates_first_argument = false;
//    static const bool alternates_second_argument = false;
//    static const bool has_side_effect = false;
//
//    typedef typename
//        _Internal::function_traits_subscript_select_index
//        <
//            indexed_subvector<Base, IndexVector>, Index,
//            typename type_traits<Index>::category_type
//        >::type
//            result_type;
//};

template <typename Base, typename IndexVector, typename Index>
struct binary_function_traits
    <function_tag::subscript, const indexed_subvector<Base, IndexVector>, Index>
        : public vec_binary_function_traits
            <const indexed_subvector<Base, IndexVector>, Index>
{};
//{
//    static const bool is_specialized = true;
//    typedef const indexed_subvector<Base, IndexVector> first_argument_type;
//    typedef Index second_argument_type;
//    static const bool alternates_first_argument = false;
//    static const bool alternates_second_argument = false;
//    static const bool has_side_effect = false;
//
//    typedef typename
//        _Internal::function_traits_subscript_select_index
//        <
//            const indexed_subvector<Base, IndexVector>, Index,
//            typename type_traits<Index>::category_type
//        >::type
//            result_type;
//};

template <typename Base, typename IndexVector, typename Index>
struct binary_function_traits
    <function_tag::parentheses_1, indexed_subvector<Base, IndexVector>, Index>
        : public binary_function_traits
            <function_tag::subscript, indexed_subvector<Base, IndexVector>, Index>
{};

template <typename Base, typename IndexVector, typename Index>
struct binary_function_traits
    <function_tag::parentheses_1, const indexed_subvector<Base, IndexVector>, Index>
        : public binary_function_traits
            <function_tag::subscript, const indexed_subvector<Base, IndexVector>, Index>
{};


/// Subvector referenced of the original vector by vector of indexes.
/** This structure contains two pice of information: reference to the original
    vector and copy of the vector of indexes. The type of the first one is Base,
    the second --- is Index. */
template <typename Base, typename Index>
class indexed_subvector
{
    Base* base_m;
    Index index_m;

public:

    // TODO: Complete the class implementation.

    /// The type of the original vector with values.
    typedef Base base_type;

    /// The type of the vector of indexes.
    typedef Index index_type;

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
    typedef typename Index::size_type size_type;

    /// Signed integer type for offsets and relative index representation.
    typedef typename Index::difference_type difference_type;

    /// Pointer to const item type.
    typedef typename cnc_pointer<Base>::type pointer;

    /// Pointer to item type.
    typedef typename Base::const_pointer const_pointer;


    // Constructors. -------------------------------------------------


    /// Creates subvector without the original vector.
    /** The subvector looks empty. */
    indexed_subvector () : base_m(0) {}

    /// Creates subvector with the particular original vector.
    /** The index vector is being initialized by the default.
        The subvector looks empty. */
    indexed_subvector (base_type* base_a) : base_m(base_a) {}

    /// Creates subvector with the particular original and index vectors.
    indexed_subvector (base_type* base_a, const index_type& index_a)
    : base_m(base_a), index_m(index_a) {}


    // Assignment. ---------------------------------------------------


    /// Assign elements to this vector from another.
    /** WARNING! Now it's working without mixing computations and
        for vectors only. */
    template <typename V>
    indexed_subvector operator= (const V& x)
    {
        ARAGELI_ASSERT_0(x.size() == size());
        for(size_type i = 0; i < size(); ++i)
            el(i) = x[i];
        return *this;
    }


    // Access to the original and index vectors.  --------------------


    base_type& base () const { return *base_m; }
    void base (base_type* base_a) { base_m = base_a; }

    const index_type& index () const { return index_m; }
    index_type& index () { return index_m; }


    // Some properties. ----------------------------------------------


    size_type size () const { return index_m.size(); }
    bool is_empty () const { return index_m.is_empty(); }


    // Access to the elements. ---------------------------------------


    const_reference el (size_type i) const
    { return (*static_cast<const Base*>(base_m))[index_m[i]]; }

    reference el (size_type i) { return (*base_m)[index_m[i]]; }

    const_reference at (size_type i) const
    { return (*static_cast<const Base*>(base_m)).at(index_m.at(i)); }

    reference at (size_type i) { return (*base_m).at(index_m.at(i)); }

    template <typename Vec>
    indexed_subvector<const indexed_subvector, Vec>
    subvector (const Vec& vec) const
    { return indexed_subvector<const indexed_subvector, Vec>(this, vec); }

    template <typename Vec>
    indexed_subvector<indexed_subvector, Vec>
    subvector (const Vec& vec)
    { return indexed_subvector<indexed_subvector, Vec>(this, vec); }

    template <typename Vec>
    indexed_subvector<const indexed_subvector, Vec>
    subvector_at (const Vec& vec) const
    {
        if(all_in_range(vec, 0, size()-1))return subvector(vec);
        else throw out_of_range();
    }

    template <typename Vec>
    indexed_subvector<indexed_subvector, Vec>
    subvector_at (const Vec& vec)
    {
        if(all_in_range(vec, 0, size()-1))return subvector(vec);
        else throw out_of_range();
    }


    /// Accesses to vector item by index or set of indexes. Constant form.
    /** The same as el method (const). */
    template <typename X>
    typename binary_function_traits
        <function_tag::subscript, const indexed_subvector, X>::result_type
    operator[] (const X& x) const
    {
        return vec_operator_sqbrackets_index_vector
            (this, x, type_traits<X>::category_value);
    }


    /// Accesses to vector item by index or set of indexes. Non-constant form.
    /** The same as el method (non-const). */
    template <typename X>
    typename binary_function_traits
        <function_tag::subscript, indexed_subvector, X>::result_type
    operator[] (const X& x)
    {
        return vec_operator_sqbrackets_index_vector
            (this, x, type_traits<X>::category_value);
    }


    /// Accesses to vector item by index or set of indexes. Constant form.
    /** The same as el method (const). */
    template <typename X>
    typename binary_function_traits
        <function_tag::subscript, const indexed_subvector, X>::result_type
    operator() (const X& x) const
    { return operator[](x); }


    /// Accesses to vector item by index or set of indexes. Non-constant form.
    /** The same as el method (non-const). */
    template <typename X>
    typename binary_function_traits
        <function_tag::subscript, indexed_subvector, X>::result_type
    operator() (const X& x)
    { return operator[](x); }

//private:
//
//    // The following group of functions is intended to operator[] implementation.
//
//    template <typename Index>
//    const_reference _operator_sqbrackets_index_vector
//    (const Index& i, const type_category::integer&) const
//    { return el(i); }
//
//    template <typename Index>
//    reference _operator_sqbrackets_index_vector
//    (const Index& i, const type_category::integer&)
//    { return el(i); }
//
//    template <typename X>
//    indexed_subvector<const vector, X>
//    _operator_sqbrackets_index_vector
//    (const X& x, const type_category::type&) const
//    { return subvector(x); }
//
//    template <typename X>
//    indexed_subvector<const vector, X>
//    _operator_sqbrackets_index_vector
//    (const X& x, const type_category::type&)
//    { return subvector(x); }
//
//public:
//
};


/// Simple output of the values of subvector.
template <typename Out, typename Base, typename Index>
inline Out& output_list
(
    Out& out,
    const indexed_subvector<Base, Index>& x,
    const char* first_bracket = vector_output_list_first_bracket_default,
    const char* second_bracket = vector_output_list_second_bracket_default,
    const char* separator = vector_output_list_separator_default
)
{ return vec_output_list(out, x, first_bracket, second_bracket, separator); }


}


//#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
//    #define _ARAGELI_subvector_indexed_hpp_
//    #include "indexed.cpp"
//    #undef  _ARAGELI_subvector_indexed_hpp_
//#endif


#endif

