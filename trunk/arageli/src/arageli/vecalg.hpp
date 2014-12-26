/*****************************************************************************

    vecalg.hpp -- generic functions on vector-like structures.

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
    \file
    This file contains several generic functions which allows dealing with
    vector-like structures, such as Arageli::vector. For example, element-wise
    operations with two vectors, scalar by vector operations etc.
*/


#ifndef _ARAGELI_vecalg_hpp_
#define _ARAGELI_vecalg_hpp_

#include "config.hpp"

//#include "_utility.hpp"
#include "frwrddecl.hpp"
#include "misc.hpp"

#include "functional.hpp"


namespace Arageli
{


namespace _Internal
{

template <typename Vector, typename Index, typename Index_category>
struct function_traits_subscript_select_index
{
    typedef indexed_subvector<Vector, Index> type;
};

template <typename Vector, typename Index, typename Index_category>
struct function_traits_subscript_select_index
<
    const Vector,
    Index,
    Index_category
>
{
    typedef indexed_subvector<const Vector, Index> type;
};

template <typename Vector, typename Index>
struct function_traits_subscript_select_index
<
    Vector,
    Index,
    type_category::integer
>
{
    typedef typename Vector::reference type;
};

template <typename Vector, typename Index>
struct function_traits_subscript_select_index
<
    const Vector,
    Index,
    type_category::integer
>
{
    typedef typename Vector::const_reference type;
};

} // namespace _Internal


// Ingerit from this class binary_function_traits specialization for some vector.
template <typename Vec, typename Index>
struct vec_binary_function_traits
{
    static const bool is_specialized = true;
    typedef Vec first_argument_type;
    typedef Index second_argument_type;
    static const bool alternates_first_argument = false;
    static const bool alternates_second_argument = false;
    static const bool has_side_effect = false;

    typedef typename
        _Internal::function_traits_subscript_select_index
        <
            Vec,
            Index,
            typename type_traits<Index>::category_type
        >::type
            result_type;
};


template <typename Vec, typename Index>
inline typename cnc_reference<Vec>::type
vec_operator_sqbrackets_index_vector
(
    Vec* vec,
    const Index& ind,
    const type_category::integer&
)
{
    return vec->el(ind);
}


template <typename Vec, typename Index>
inline indexed_subvector<Vec, vector<typename Vec::size_type> >
vec_operator_sqbrackets_index_vector
(
    Vec* vec,
    const Index& ind,
    const type_category::string&
)
{
    return vec->subvector(vector<typename Vec::size_type>(ind));
}


template <typename Vec, typename Index>
inline indexed_subvector<Vec, Index>
vec_operator_sqbrackets_index_vector
(
    Vec* vec,
    const Index& ind,
    const type_category::type&
)
{
    return vec->subvector(ind);
}


// ---------------------------------------------------------------------


template <typename Vec, typename UFunc>
void apply1_wores_vec (Vec& a, UFunc f)
{
    if(a.is_empty())
        return;
    typedef typename cnc_iterator<Vec>::type Iter;
    for(Iter i = a.begin(), iend = a.end(); i != iend; ++i)
        f(*i);
}


template <typename Vec1, typename Vec2, typename BinOp>
void apply2_wores_vec_by_vec (Vec1& a, Vec2& b, BinOp f)
{
    ARAGELI_ASSERT_0(a.size() == b.size());
    if(a.is_empty())
        return;

    typename cnc_iterator<Vec1>::type
        ai = a.begin(),
        aend = a.end();
    typename cnc_iterator<Vec2>::type bi = b.begin();

    for(; ai != aend; ++ai, ++bi)
        f(*ai, *bi);
}

template <typename Vec, typename Val, typename BinOp>
void apply2_wores_vec_by_val (Vec& a, Val& b, BinOp f)
{
    if(a.is_empty())return;

    typename cnc_iterator<Vec>::type
        ai = a.begin(),
        aend = a.end();

    for(; ai != aend; ++ai)
        f(*ai, b);
}

template <typename Val, typename Vec, typename BinOp>
void apply2_wores_val_by_vec (Val& a, Vec& b, BinOp f)
{
    if(b.is_empty())return;

    typename cnc_iterator<Vec>::type
        bi = b.begin(),
        bend = b.end();

    for(; bi != bend; ++bi)
        f(a, *bi);
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4244)
#endif

template <typename Vec1, typename Vec2, typename VecRes, typename Func2>
VecRes& apply2_vec_by_vec
(const Vec1& a, const Vec2& b, VecRes& res, Func2 f)
{
    ARAGELI_ASSERT_0(a.size() == b.size());

    res.resize(a.size());
    typename Vec1::const_iterator
        ia = a.begin(),
        iaend = a.end();
    typename Vec2::const_iterator ib = b.begin();
    typename VecRes::iterator ires = res.begin();

    for(; ia != iaend; ++ia, ++ib, ++ires)
        *ires = f(*ia, *ib);

    return res;
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

template <typename Vec1, typename Vec2, typename Func2>
inline Vec1 apply2_vec_by_vec
(const Vec1& a, const Vec2& b, Func2 f)
{
    ARAGELI_ASSERT_0(a.size() == b.size());
    Vec1 res(a.size());
    return apply2_vec_by_vec(a, b, res, f);
}


template <typename Vec, typename Val, typename VecRes, typename Func2>
VecRes& apply2_vec_by_val
(const Vec& a, const Val& b, VecRes& res, Func2 f)
{
    res.resize(a.size());
    typename Vec::const_iterator
        ia = a.begin(),
        iaend = a.end();
    typename VecRes::iterator ires = res.begin();

    for(; ia != iaend; ++ia, ++ires)
        *ires = f(*ia, b);

    return res;
}


template <typename Vec, typename Val, typename Func2>
inline Vec apply2_vec_by_val
(const Vec& a, const Val& b, Func2 f)
{
    Vec res(a.size());
    return apply2_vec_by_val(a, b, res, f);
}


template <typename Val, typename Vec, typename VecRes, typename Func2>
VecRes& apply2_val_by_vec
(const Val& a, const Vec& b, VecRes& res, Func2 f)
{
    res.resize(b.size());
    typename Vec::const_iterator
        ib = b.begin(), ibend = b.end();
    typename VecRes::iterator ires = res.begin();

    for(; ib != ibend; ++ib, ++ires)
        *ires = f(a, *ib);

    return res;
}


template <typename Val, typename Vec, typename Func2>
inline Vec apply2_val_by_vec
(const Val& a, const Vec& b, Func2 f)
{
    Vec res(b.size());
    return apply2_val_by_vec(a, b, res, f);
}

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template <typename Vec1, typename Vec2, typename VecRes, typename Func3>
VecRes& apply3_vec_by_vec
(const Vec1& a, const Vec2& b, VecRes& res, Func3 f)
{
    ARAGELI_ASSERT_0(a.size() == b.size());

    res.resize(a.size());
    typename Vec1::const_iterator
        ia = a.begin(),
        iaend = a.end();
    typename Vec2::const_iterator ib = b.begin();
    typename VecRes::iterator ires = res.begin();

    for(; ia != iaend; ++ia, ++ib, ++ires)
        f(*ia, *ib, *ires);

    return res;
}


template <typename Vec1, typename Vec2, typename Func3>
inline Vec1 apply3_vec_by_vec
(const Vec1& a, const Vec2& b, Func3 f)
{
    ARAGELI_ASSERT_0(a.size() == b.size());
    Vec1 res(a.size());
    return apply3_vec_by_vec(a, b, res, f);
}


template <typename Vec, typename Val, typename VecRes, typename Func3>
VecRes& apply3_vec_by_val
(const Vec& a, const Val& b, VecRes& res, Func3 f)
{
    res.resize(a.size());
    typename Vec::const_iterator
        ia = a.begin(),
        iaend = a.end();
    typename VecRes::iterator ires = res.begin();

    for(; ia != iaend; ++ia, ++ires)
        f(*ia, b, *ires);

    return res;
}


template <typename Vec, typename Val, typename Func3>
inline Vec apply3_vec_by_val
(const Vec& a, const Val& b, Func3 f)
{
    Vec res(a.size());
    return apply3_vec_by_val(a, b, res, f);
}


template <typename Val, typename Vec, typename VecRes, typename Func3>
VecRes& apply3_val_by_vec
(const Val& a, const Vec& b, VecRes& res, Func3 f)
{
    res.resize(b.size());
    typename Vec::const_iterator
        ib = b.begin(),
        ibend = b.end();
    typename VecRes::iterator ires = res.begin();

    for(; ib != ibend; ++ib, ++ires)
        f(a, *ib, *ires);

    return res;
}


template <typename Val, typename Vec, typename Func3>
inline Vec apply3_val_by_vec
(const Val& a, const Vec& b, Func3 f)
{
    Vec res(b.size());
    return apply3_val_by_vec(a, b, res, f);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template <typename Vec1, typename Vec2, typename Cmp>
int cmp_vec_by_vec (const Vec1& a, const Vec2& b, Cmp cmp)
{
    typename Vec1::const_iterator
        ai = a.begin(),
        aend = a.end();
    typename Vec2::const_iterator
        bi = b.begin(),
        bend = b.end();

    for(; ai != aend && bi != bend; ++ai, ++bi)
    {
        int lres = cmp(*ai, *bi);
        if(lres < 0)
            return -1;
        else if(lres > 0)
            return +1;
    }

    if(ai == aend)
        if(bi == bend)
            return 0;
        else
            return -1;
    else
    { // ai != aend
        ARAGELI_ASSERT_1(bi == bend);
        return +1;
    }
}


template <typename Vec, typename Val, typename Cmp>
int cmp_vec_by_val (const Vec& a, const Val& val, Cmp cmp)
{
    typename Vec::const_iterator ai =
        a.begin(),
        aend = a.end();

    for(; ai != aend; ++ai)
    {
        int lres = cmp(*ai, val);
        if(lres < 0)
            return -1;
        else if(lres > 0)
            return +1;
    }

    return 0;
}


template <typename Val, typename Vec, typename Cmp>
int cmp_val_by_vec (const Val& val, const Vec& a, Cmp cmp)
{
    typename Vec::const_iterator ai =
        a.begin(),
        aend = a.end();

    for(; ai != aend; ++ai)
    {
        int lres = cmp(val, *ai);
        if(lres < 0)
            return -1;
        else if(lres > 0)
            return +1;
    }

    return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template <typename Vec1, typename Vec2>
int cmpdef_vec_by_vec (const Vec1& a, const Vec2& b)
{
    return cmp_vec_by_vec(a, b, gfunc::cmp());
}


template <typename Vec, typename Val>
int cmpdef_vec_by_val (const Vec& a, const Val& val)
{
    return cmp_vec_by_val(a, val, gfunc::cmp());
}


template <typename Val, typename Vec>
int cmpdef_val_by_vec (const Val& val, const Vec& a)
{
    return cmp_val_by_vec(val, a, gfunc::cmp());
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template <typename Vec1, typename Vec2, typename Cmp>
bool allcmp_vec_by_vec (const Vec1& a, const Vec2& b, Cmp cmp)
{
    ARAGELI_ASSERT_0(a.size() == b.size());

    typename Vec1::const_iterator
        ia = a.begin(),
        iaend = a.end();
    typename Vec2::const_iterator ib = b.begin();

    for(; ia != iaend; ++ia, ++ib)
        if(!cmp(*ia, *ib))
            return false;

    return true;
}


template <typename Vec, typename Val, typename Cmp>
bool allcmp_vec_by_val (const Vec& a, const Val& val, Cmp cmp)
{
    typename Vec::const_iterator
        ia = a.begin(),
        iaend = a.end();

    for(; ia != iaend; ++ia)
        if(!cmp(*ia, val))
            return false;

    return true;
}


template <typename Val, typename Vec, typename Cmp>
bool allcmp_val_by_vec (const Val& val, const Vec& a, Cmp cmp)
{
    typename Vec::const_iterator
        ia = a.begin(),
        iaend = a.end();

    for(; ia != iaend; ++ia)
        if(!cmp(val, *ia))
            return false;

    return true;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template <typename Vec1, typename Vec2, typename Cmp>
inline typename Vec1::template other_element_type_refcnt<bool, true>::type
eachcmp_vec_by_vec (const Vec1& a, const Vec2& b, Cmp cmp)
{
    typename Vec1::template other_element_type_refcnt<bool, true>::type res;
    return apply2_vec_by_vec(a, b, res, cmp);
}


template <typename Vec, typename Val, typename Cmp>
inline typename Vec::template other_element_type_refcnt<bool, true>::type
eachcmp_vec_by_val (const Vec& a, const Val& val, Cmp cmp)
{
    typename Vec::template other_element_type_refcnt<bool, true>::type res;
    return apply2_vec_by_val(a, val, res, cmp);
}


template <typename Val, typename Vec, typename Cmp>
inline typename Vec::template other_element_type_refcnt<bool, true>::type
eachcmp_val_by_vec (const Val& val, const Vec& a, Cmp cmp)
{
    typename Vec::template other_element_type_refcnt<bool, true>::type res;
    return apply2_val_by_val(val, a, res, cmp);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

template <typename Vec1, typename Vec2, typename Cmp>
inline typename Vec1::template other_element_type_refcnt<int, true>::type
elwisecmp_vec_by_vec (const Vec1& a, const Vec2& b, Cmp cmp)
{
    typename Vec1::template other_element_type_refcnt<int, true>::type res;
    return apply2_vec_by_vec(a, b, res, cmp);
}


template <typename Vec, typename Val, typename Cmp>
inline typename Vec::template other_element_type_refcnt<int, true>::type
elwisecmp_vec_by_val (const Vec& a, const Val& val, Cmp cmp)
{
    typename Vec::template other_element_type_refcnt<int, true>::type res;
    return apply2_vec_by_val(a, val, res, cmp);
}


template <typename Val, typename Vec, typename Cmp>
inline typename Vec::template other_element_type_refcnt<int, true>::type
elwisecmp_val_by_vec (const Val& val, const Vec& a, Cmp cmp)
{
    typename Vec::template other_element_type_refcnt<int, true>::type res;
    return apply2_val_by_val(val, a, res, cmp);
}


template <typename Index, typename T>
bool all_in_range (const Index& ind, const T& left, const T& right)
{
    for(typename Index::const_pointer i = ind.begin(); i != ind.end(); ++i)
        if(!(*i >= left && *i <= right))
            return false;
    return true;
}


extern const char* vector_output_list_first_bracket_default;
extern const char* vector_output_list_second_bracket_default;
extern const char* vector_output_list_separator_default;
extern const char* vector_input_list_first_bracket_default;
extern const char* vector_input_list_second_bracket_default;
extern const char* vector_input_list_separator_default;
extern const char* vector_input_list_range_default;
extern const char* vector_output_aligned_left_col_default;
extern const char* vector_output_aligned_right_col_default;


template <typename Out, typename Vec>
Out& vec_output_list
(
    Out& out,
    const Vec& x,
    const char* first_bracket = vector_output_list_first_bracket_default,
    const char* second_bracket = vector_output_list_second_bracket_default,
    const char* separator = vector_output_list_separator_default
);


}


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_VECALG
    #include "vecalg.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_VECALG
#endif


#endif  //  #ifndef _ARAGELI_vector_hpp_

