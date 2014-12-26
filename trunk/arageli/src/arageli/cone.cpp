/*****************************************************************************

    cone.cpp -- See declarations in cone.hpp.

    This file is a part of the Arageli library.

    Copyright (C) 2006--2007 Sergey S. Lyalin

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
    \file cone.cpp
    \brief The cone.hpp file stuff implementation.
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_cone)


#include "vector.hpp"
#include "skeleton.hpp"

#include "cone.hpp"


namespace Arageli
{


template <typename T, typename M, typename CFG>
template <typename Cone1>
cone<T, M, CFG>::cone (const Cone1& c, const fromdual_t&) : state(State(0))
{
    // WARNING! This implementation is correct if inequation is 'Ax >= 0'.
    // If inequation is 'Ax <= 0', we need to flip signs.
    // Ehh... Actualy we need unique definition of a dual cone.

    if(c.is_implicit_valid())
    {
        generatrix_m = c.inequation_matrix();
        basis_m = c.equation_matrix();

        if(c.is_implicit_normal())
            set_flag(STATE_PARAMETRIC);
        else
            set_flag(STATE_PARAMETRIC_VALID);

    }

    if(c.is_parametric_valid())
    {
        inequation_m = c.generatrix_matrix();
        equation_m = c.basis_matrix();

        if(c.is_parametric_normal())
            set_flag(STATE_IMPLICIT);
        else
            set_flag(STATE_IMPLICIT_VALID);
    }

    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(is_there_valid());
}


template <typename T, typename M, typename CFG>
bool cone<T, M, CFG>::is_pointed () const
{
    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(is_there_valid());

    // If a cone doesn't have non-zero subspace, it's a pointed cone.
    if(is_parametric_normal())
        return basis_m.is_empty();
    else if(is_implicit_normal() && inequation_m.is_empty())
        return equation_m.is_square();
    else if(is_parametric_valid() && !Arageli::is_null(basis_m))
        return false;
    else
    {
        normalize_parametric();
        return basis_m.is_empty();
    }
}


template <typename T, typename M, typename CFG>
bool cone<T, M, CFG>::is_subspace () const
{
    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(is_there_valid());

    if(is_implicit_normal())
        return inequation_m.is_empty();
    else if(is_parametric_normal())
        return generatrix_m.is_empty();
    else if(is_implicit_valid() && Arageli::is_null(inequation_m))
        return true;
    else if(is_parametric_valid() && Arageli::is_null(generatrix_m))
        return true;
    else
    {
        normalize_implicit();
        return inequation_m.is_empty();
    }
}


template <typename T, typename M, typename CFG>
bool cone<T, M, CFG>::is_space () const
{
    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(is_there_valid());

    if(is_implicit_valid())
        return Arageli::is_null(inequation_m) && Arageli::is_null(equation_m);
    else if(is_parametric_normal())
    {
        ARAGELI_ASSERT_1(!basis_m.is_square() || generatrix_m.is_empty());
        return basis_m.is_square();
    }
    else
    {
        force_normalize_implicit();
        return inequation_m.is_empty() && equation_m.is_empty();
    }
}


template <typename T, typename M, typename CFG>
bool cone<T, M, CFG>::is_null () const
{
    // WARNING! This function is completely dual to is_space function.
    // TODO: Rewrite these pair of functions as one general.

    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(is_there_valid());

    if(is_parametric_valid())
        return Arageli::is_null(generatrix_m) && Arageli::is_null(basis_m);
    else if(is_implicit_normal())
    {
        ARAGELI_ASSERT_1(!equation_m.is_square() || inequation_m.is_empty());
        return equation_m.is_square();
    }
    else
    {
        force_normalize_parametric();
        return generatrix_m.is_empty() && basis_m.is_empty();
    }
}


template <typename T, typename M, typename CFG>
typename cone<T, M, CFG>::dim_type
cone<T, M, CFG>::space_dim () const
{
    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(is_there_valid());

    if(is_implicit_valid())
        return inequation_m.ncols();
    else
        return generatrix_m.ncols();
}


template <typename T, typename M, typename CFG>
typename cone<T, M, CFG>::dim_type
cone<T, M, CFG>::dim () const
{
    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(is_there_valid());

    normalize_implicit();
    return equation_m.ncols() - equation_m.nrows();
}


template <typename T, typename M, typename CFG>
bool cone<T, M, CFG>::is_simplicial () const
{
    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(is_there_valid());

    normalize_implicit();
    ARAGELI_ASSERT_1
    (
        inequation_m.nrows() != equation_m.ncols() ||
        max_embedded_basis().is_empty()
    );
    return inequation_m.nrows() == equation_m.ncols() - equation_m.nrows();
}



template <typename T, typename M, typename CFG>
typename cone<T, M, CFG>::inequation_type
cone<T, M, CFG>::inequation () const
{
    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(is_there_valid());

    validate_implicit();

    if(Arageli::is_null(equation_m))
        return inequation_m;
    else
    {
        inequation_type res = inequation_m;
        extend_rep_matrix(res, equation_m);
        return res;
    }
}


template <typename T, typename M, typename CFG>
typename cone<T, M, CFG>::generatrix_type
cone<T, M, CFG>::generatrix () const
{
    // WARNING! This function is completely dual to inequation function.
    // TODO: Rewrite these pair of functions as one general.

    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(is_there_valid());

    validate_parametric();

    if(Arageli::is_null(basis_m))
        return generatrix_m;
    else
    {
        generatrix_type res = generatrix_m;
        extend_rep_matrix(res, basis_m);
        return res;
    }
}


template <typename T, typename M, typename CFG>
const typename cone<T, M, CFG>::equation_type&
cone<T, M, CFG>::equation () const
{
    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(is_there_valid());

    normalize_implicit();

    if(!is_subspace())
        throw cannot_represent_cone();
    else
        return equation_m;
}


template <typename T, typename M, typename CFG>
const typename cone<T, M, CFG>::basis_type&
cone<T, M, CFG>::basis () const
{
    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(is_there_valid());

    normalize_parametric();

    if(!is_subspace())
        throw cannot_represent_cone();
    else
        return basis_m;
}


template <typename T, typename M, typename CFG>
const typename cone<T, M, CFG>::equation_type&
cone<T, M, CFG>::min_ambient_equation () const
{
    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(is_there_valid());

    normalize_implicit();
    return equation_m;
}


template <typename T, typename M, typename CFG>
const typename cone<T, M, CFG>::basis_type&
cone<T, M, CFG>::max_embedded_basis () const
{
    // WARNING! This function is completely dual to min_ambient_equation function.
    // TODO: Rewrite these pair of functions as one general.

    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(is_there_valid());

    normalize_parametric();
    return basis_m;
}


template <typename T, typename M, typename CFG>
typename cone<T, M, CFG>::inequation_type cone<T, M, CFG>::min_inequation () const
{
    normalize_implicit();

    if(equation_m.is_empty())
        return inequation_m;
    else
    {
        inequation_type res = inequation_m;
        res.insert_matrix_bottom(equation_m);
        vector<T, false> lastrow = equation_m.copy_row(0);

        for(size_type i = 1; i < equation_m.nrows(); ++i)
            for(size_type j = 0; j < equation_m.ncols(); ++j)
                lastrow[j] += equation_m(i, j);

        Arageli::opposite(&lastrow);
        res.insert_row(res.nrows(), lastrow);

        return res;
    }
}


template <typename T, typename M, typename CFG>
typename cone<T, M, CFG>::generatrix_type cone<T, M, CFG>::min_generatrix () const
{
    // WARNING! This function is completely dual to min_inequation function.
    // TODO: Rewrite these pair of functions as one general.

    normalize_parametric();

    if(basis_m.is_empty())
        return generatrix_m;
    else
    {
        generatrix_type res = generatrix_m;
        res.insert_matrix_bottom(basis_m);
        vector<T, false> lastrow = basis_m.copy_row(0);

        for(size_type i = 1; i < basis_m.nrows(); ++i)
            for(size_type j = 0; j < basis_m.ncols(); ++j)
                lastrow[j] += basis_m(i, j);

        Arageli::opposite(&lastrow);
        res.insert_row(res.nrows(), lastrow);

        return res;
    }
}


template <typename T, typename M, typename CFG>
template <typename Cone1>
cone<T, M, CFG>& cone<T, M, CFG>::intersection (const Cone1& x)
{
    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(is_there_valid());
    ARAGELI_ASSERT_0(space_dim() == x.space_dim());

    if(!x.is_space())    // WARNING! Too expensive because normalization!
    {
        validate_implicit();
        x.validate_implicit();
        clear_flag(STATE_PARAMETRIC | STATE_IMPLICIT_NORMAL);
        inequation_m.insert_matrix_bottom(x.inequation_matrix());
        equation_m.insert_matrix_bottom(x.equation_matrix());
    }

    return *this;
}


template <typename T, typename M, typename CFG>
template <typename Cone1>
cone<T, M, CFG>& cone<T, M, CFG>::conic_union (const Cone1& x)
{
    // WARNING! This function is completely dual to intersection function.
    // TODO: Rewrite these pair of functions as one general.

    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(is_there_valid());
    ARAGELI_ASSERT_0(space_dim() == x.space_dim());

    if(!x.is_null())    // WARNING! Too expensive because normalization!
    {
        validate_parametric();
        x.validate_parametric();
        clear_flag(STATE_IMPLICIT | STATE_PARAMETRIC_NORMAL);
        generatrix_m.insert_matrix_bottom(x.generatrix_matrix());
        basis_m.insert_matrix_bottom(x.basis_matrix());
    }

    return *this;
}


template <typename T, typename M, typename CFG>
void cone<T, M, CFG>::force_validate_implicit () const
{
    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(get_flag(STATE_PARAMETRIC_VALID));

    generatrix_type a = generatrix_m;
    extend_rep_matrix(a, basis_m);

    inequation_type q;    // may be it should be saved after the operation?
    skeleton(a, inequation_m, q, equation_m);
    //output_aligned(std::cout << "\nq =\n", q);

    set_flag(STATE_IMPLICIT);
}


template <typename T, typename M, typename CFG>
void cone<T, M, CFG>::force_validate_parametric () const
{
    ARAGELI_ASSERT_1(is_correct_flags());
    ARAGELI_ASSERT_0(get_flag(STATE_IMPLICIT_VALID));

    inequation_type a = inequation_m;
    extend_rep_matrix(a, equation_m);

    generatrix_type q;    // may be it should be saved after the operation?
    skeleton(a, generatrix_m, q, basis_m);
    //output_aligned(std::cout << "\nq =\n", q);

    set_flag(STATE_PARAMETRIC);
}


template <typename T, typename M, typename CFG>
void cone<T, M, CFG>::force_normalize_implicit () const
{
    // WARNING! Slow implementation.
    // TODO: Make it faster!

    validate_parametric();
    force_validate_implicit();
}


template <typename T, typename M, typename CFG>
void cone<T, M, CFG>::force_normalize_parametric () const
{
    // WARNING! Slow implementation.
    // TODO: Make it faster!

    validate_implicit();
    force_validate_parametric();
}


template <typename T, typename M, typename CFG>
void cone<T, M, CFG>::force_normalize_all () const
{
    // WARNING! Slow implementation.
    // TODO: Make it faster!

    if(get_flag(STATE_IMPLICIT_VALID))
    {
        force_validate_parametric();
        force_validate_implicit();
    }
    else
    {
        ARAGELI_ASSERT_0(get_flag(STATE_PARAMETRIC_VALID));

        force_validate_implicit();
        force_validate_parametric();
    }
}


template <typename Out, typename T, typename M, typename CFG>
void output_list (Out& out, const cone<T, M, CFG>& x)
{
    out << "(" << x.state << ", " << x.space_dim() << ", ";
    output_list(out, x.inequation_m);
    out << ", ";
    output_list(out, x.equation_m);
    out << ", ";
    output_list(out, x.generatrix_m);
    out << ", ";
    output_list(out, x.basis_m);
    out << ")";
}


template
<
    typename T1,
    typename M1,
    typename CFG1,
    typename T2,
    typename M2,
    typename CFG2
>
int cmp
(
    const cone<T1, M1, CFG1>& a,
    const cone<T2, M2, CFG2>& b
)
{
    ARAGELI_ASSERT_0
    (
        !std::numeric_limits<T1>::is_specialized ||
        std::numeric_limits<T1>::is_exact
    );

    if(int c = cmp(a.space_dim(), b.space_dim()))
        return c;
    if(int c = cmp(a.dim(), b.dim()))
        return c;
    if(is_null(a.dim()) && is_null(b.dim()))
        return 0;

    ARAGELI_ASSERT_1
    (
        a.min_ambient_equation().nrows() ==
        b.min_ambient_equation().nrows()
    );

    a.normalize_implicit();
    b.normalize_implicit();

    if(int c = cmp(a.inequation_matrix().nrows(), b.inequation_matrix().nrows()))
        return -c;

    // WARNING! Too expensive!

    vector<vector<T1>, false> arows(a.inequation_matrix().nrows());
    vector<vector<T2>, false> brows(b.inequation_matrix().nrows());
    ARAGELI_ASSERT_1(arows.size() == brows.size());

    for(typename cone<T1, M1, CFG1>::size_type i = 0; i < arows.size(); ++i)
    {
        arows[i] = a.inequation_matrix().copy_row(i);
        arows[i] /= _Internal::content(arows[i].begin(), arows[i].end());
        brows[i] = b.inequation_matrix().copy_row(i);
        brows[i] /= _Internal::content(brows[i].begin(), brows[i].end());
    }

    std::sort(arows);
    std::sort(brows);

    if(int c = cmp(arows, brows))
        return c;

    if(int c = cmp(a.equation_matrix().nrows(), b.equation_matrix().nrows()))
        return -c;

    if(a.equation_matrix().nrows() == 0)
        return 0;

    if
    (
        int c = cmp
        (
            rref(matrix<rational<T1>, false>(a.equation_matrix())),
            rref(matrix<rational<T1>, false>(b.equation_matrix()))
        )
    )
        return c;

    return 0;
}


}


#else    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...


namespace Arageli
{

// PLACE ALL NOT TEMPLATE NOT INLINE IMPLEMENTATIONS HERE

}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
