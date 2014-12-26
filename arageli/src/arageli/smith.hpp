/*****************************************************************************

    smith.hpp -- Smith Normal Diagonal Form of a matrix.

    This file is a part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2005--2007 Sergey S. Lyalin
    Copyright (C) 2006 Anna Bestaeva

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
    Smith Normal Diagonal Form of a matrix.
*/


#ifndef _ARAGELI_smith_hpp_
#define _ARAGELI_smith_hpp_

#include "config.hpp"

#include "smith/classic.hpp"
#include "smith/near_optimal.hpp"
#include "smith/storjohann.hpp"


namespace Arageli
{


namespace ctrl
{

struct smith_idler
{
    void preamble ()
    {}

    void conclusion ()
    {}

    template <typename Q, typename B, typename P>
    void current_matrices (const Q&, const B&, const P&)
    {}

    template <typename I, typename J>
    void find_smallest_nonzero (const I&, const J&)
    {}

    template <typename I, typename J>
    void pivot_item (const I&, const J& j)
    {}

    void after_pivoting () {}

    template <typename I, typename J>
    void nondivisor_entry (const I& k, const I& i, const J& l)
    {}

    void pivot_adjustment ()
    {}

    template
    <
        typename C,
        typename I,
        typename J,
        typename Q,
        typename B,
        typename P
    >
    bool stop
    (
        const C& corner,
        const I& i, const J& j,
        const Q& q, const B& b,
        const P& p
    )
    {
        return false;
    }
};

}    // namespace ctrl


/// Normalize for resulting elements of Smith form.
template <typename T>
struct smith_norm : std::unary_function<T&, void>
{
    template <typename P>
    void operator() (T& x, P& p, std::size_t i) const
    {
        do_norm(x, p, i, bool_type<type_traits<T>::is_polynom>::value);
    }

private:

    /// version for polynomials
    template <typename P>
    void do_norm (T& x, P& p, std::size_t i, true_type) const
    {
        ARAGELI_ASSERT_0(!is_null(x));
        p.div_row(i, x.leading_coef());
        x /= safe_reference(x.leading_coef());
    }

    /// version for other types
    template <typename P>
    void do_norm (T& x, P& p, std::size_t i, false_type) const
    {}
};

/// Produces normal diagonal form B of integer matrix A.
/**
    Produces normal diagonal form B of integer matrix A.
    Returns B, P, Q, rank, det:
    - P, Q are unimodular such that B = P * A * Q;
    - rank is a rank of A;
    - det is the basis minor of A
*/
template
<
    typename MA,
    typename MB,
    typename MP,
    typename MQ,
    typename Rank,
    typename T_det,
    typename T_factory,
    typename Ctrler,
    typename Norm
>
void smith
(
    const MA& A,
    MB& B,
    MP& P,
    MQ& Q,
    Rank& rank,
    T_det& det,
    const T_factory& tfctr,
    Ctrler ctrler,
    Norm norm
);


/// Produces normal diagonal form B of integer matrix A.
template
<
    typename MA,
    typename MB,
    typename MP,
    typename MQ,
    typename Rank,
    typename T_det,
    typename Ctrler
>
inline void smith
(
    const MA& A,
    MB& B,
    MP& P,
    MQ& Q,
    Rank& rank,
    T_det& det,
    Ctrler ctrler
)
{
    typedef typename MA::element_type T_A;

    return smith
    (
        A,
        B,
        P,
        Q,
        rank,
        det,
        factory<T_det>(),
        ctrler,
        smith_norm<T_A>()
    );
}


/// Produces normal diagonal form B of integer matrix A.
template
<
    typename MA,
    typename MB,
    typename MP,
    typename MQ,
    typename Rank,
    typename T_det
>
inline void smith
(
    const MA& A,
    MB& B,
    MP& P,
    MQ& Q,
    Rank& rank,
    T_det& det
)
{
    typedef typename MA::element_type T_A;

    return smith
    (
        A,
        B,
        P,
        Q,
        rank,
        det,
        factory<T_det>(),
        ctrl::smith_idler(),
        smith_norm<T_A>()
    );
}


///    Produces normal diagonal form B of integer matrix A.
template <typename M>
M smith (const M& a);


/// Produces normal diagonal form B of integer matrix A.
/**
    Produces normal diagonal form B of integer matrix A.
    Returns B, P, Q, rank, det:
    - P, Q are unimodular such that B = P * A * Q;
    - rank is a rank of A;
    - det is the basis minor of A
*/
template
<
    typename MA,
    typename MB,
    typename MP,
    typename MQ,
    typename Rank,
    typename T_det
>
void smith_storjohann
(
    const MA &A,
    MB &B,
    MP &P,
    MQ &Q,
    Rank &rank,
    T_det &det
);


/// Produces normal diagonal form B of integer matrix A.
/**
    Returns B, P, Q, rank, det:
    - P, Q are unimodular such that B = P * A * Q;
    - rank is a rank of A;
    - det is the basis minor of A
*/
template
<
    typename MA,
    typename MB,
    typename MP,
    typename MQ,
    typename Rank,
    typename T_det
>
void smith_near_optimal
(
    const MA &A,
    MB &B,
    MP &P,
    MQ &Q,
    Rank &rank,
    T_det &det
);


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_smith
    #include "smith.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_smith
#endif

#endif    // #ifndef _ARAGELI_smith_hpp_
