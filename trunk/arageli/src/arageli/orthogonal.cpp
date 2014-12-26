/*****************************************************************************

    orthogonal.cpp

    This file is a part of the Arageli library.

    Copyright (C) 2007 Sergey S. Lyalin

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
    \file orthogonal.cpp
    \brief The orthogonal.hpp file stuff implementation.

    ADD ADDITIONAL FILE DESCRIPTION HERE
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_orthogonal)

#include "exception.hpp"
#include "vector.hpp"
#include "matrix.hpp"
#include "solve_linsys.hpp"
#include "factory.hpp"

#include "orthogonal.hpp"


namespace Arageli
{


template <typename BInp, typename BOrt>
void orthogonalization_generic_gram_schmidt
(
    const BInp& b_inp,
    BOrt& b_ort
)
{
    ARAGELI_ASSERT_0(!b_inp.is_empty());
    ARAGELI_ASSERT_0(b_inp.nrows() <= b_inp.ncols());

    typedef int dim_type;      // dimention type
    typedef typename BInp::value_type value_type;   // working field

    dim_type n = b_inp.nrows(); // the number of vectors

    vector<value_type, false> d(n+1);
    matrix<value_type, false> mu(n, n, fromsize);
    b_ort.assign_fromsize(n, b_inp.ncols());

    d[0] = unit<value_type>();

    // Orthogonalization. See the original paper for proof.

    for(dim_type i = 0; i < n; ++i)
    {
        value_type sigma = null<value_type>();
        for(dim_type j = 0; j <= i-1; ++j)
        {
            for(dim_type l = 0; l <= j-1; ++l)
                sigma = (d[l+1]*sigma + mu(i,l)*mu(j,l))/d[l];
            mu(i,j) = d[j]*dotprod(b_inp.copy_row(i), b_inp.copy_row(j)) - sigma;
        }

        sigma = null<value_type>();
        for(dim_type l = 0; l <= i-1; ++l)
            sigma = (d[l+1]*sigma + square(mu(i,l)))/d[l];
        d[i+1] = d[i]*dotprod(b_inp.copy_row(i), b_inp.copy_row(i)) - sigma;
        mu(i,i) = d[i+1];

        vector<value_type> a = d[1]*b_inp.copy_row(i) - mu(i,0)*b_inp.copy_row(0);
        for(dim_type l = 0; l <= i-2; ++l)
            a = (d[l+2]*a - mu(i,l+1)*b_ort.copy_row(l+1))/d[l+1];
        b_ort.assign_row(i, a);
    }

    b_ort.assign_row(0, b_inp.copy_row(0));

    ARAGELI_ASSERT_1(is_unit(d[0]));
}


template <typename BA, typename V>
V project_orthogonal_eq_ba
(
    const BA& ba,
    const V& v
)
{
    typedef typename BA::value_type T;

    // obtain the basis of plane ba.
    matrix<T, false> gx;
    vector<T, false> offset;
    if(solve_linsys_rational_ba(ba, offset, gx) == SLR_UNIQUE)
        return offset;
    gx.transpose(); // col -> row basis representation

    // translate coordinate to the new origin (offset)
    vector<T, false> vnew = v - offset;   // v in new coordinates

    // orthogonalize the basis
    matrix<T, false> gxort;
    orthogonalization_generic_gram_schmidt(gx, gxort);

    // decompose vnew in the orthogonal basis
    vector<T, false> mu = gxort*vnew;
    vector<T, false> proj(v.size());
    for(std::size_t i = 0; i < mu.size(); ++i)
    {
        vector<T> iort = gxort.copy_row(i);
        proj += iort*mu[i]/dotsquare(iort);
    }

    return proj + offset;
}


}


#else    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...


namespace Arageli
{

// PLACE ALL NOT TEMPLATE NOT INLINE IMPLEMENTATIONS HERE

}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
