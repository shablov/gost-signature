/*****************************************************************************

    resultant.cpp -- See declarations in resultant.hpp.

    This file is a part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2005--2007, 2010 Sergey S. Lyalin

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
    See description for resultant.hpp file.
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_resultant)

#include "vector.hpp"

#include "resultant.hpp"


namespace Arageli
{


template <typename P1, typename P2, typename M>
M& sylvester_fill (const P1& p1, const P2& p2, M& res)
{
    typedef typename M::size_type size_type;
    typedef typename P1::monom_const_iterator P1I;
    typedef typename P2::monom_const_iterator P2I;

    size_type
        d1 = p1.degree(),
        d2 = p2.degree(),
        s = d1 + d2;

    res.resize(s, s);

    size_type i = 0;
    for(; i != d2; ++i)
    {
        size_type margin = i + d1;
        for
        (
            P1I p1i = p1.monoms_begin(), p1end = p1.monoms_end();
            p1i != p1end;
            ++p1i
        )
            res(i, margin - p1i->degree()) = p1i->coef();
    }

    for(; i != s; ++i)
        for
        (
            P2I p2i = p2.monoms_begin(), p2end = p2.monoms_end();
            p2i != p2end;
            ++p2i
        )
            res(i, i - p2i->degree()) = p2i->coef();

    return res;
}


template <typename P1, typename P2, typename M, typename MTFactory>
M& sylvester (const P1& p1, const P2& p2, M& res, MTFactory fctr)
{
    res.assign
    (
        p1.degree() + p2.degree(),
        p1.degree() + p2.degree(),
        res.is_empty() ? fctr.null() : fctr.null(res(0, 0))
    );

    return sylvester_fill(p1, p2, res);
}


template <typename P1, typename P2, typename SRChain, typename PCFactory>
void subresultants_nonmodular
(const P1& p1, const P2& p2, SRChain& s, PCFactory fctr)
{
    // Source: Buhberger and others, p. 168, alg. 4.1
    // We have a problem with sign of the result subresultants.

    ARAGELI_ERROR
    (
        "It is known that there are inputs for which this function works incorrectly: subresultants_nonmodular.\n"
        "If you are sure that you need to call this function anyway, please comment the lines that produces "
        "this error message out. "
    );

    ARAGELI_ASSERT_0(!is_null(p1));
    ARAGELI_ASSERT_0(!is_null(p2));

    typedef typename SRChain::difference_type index;
    typedef typename SRChain::element_type P;    // maybe there is better chose
    typedef typename P::coef_type Coef;
    typedef typename P::degree_type Degree;

    typename P1::degree_type m = p1.degree();
    typename P2::degree_type n = p2.degree();

    index j;
    if(m > n)
        j = m-1;
    else
        j = n;

    s.resize(j+2);

    s[j+1] = p1;
    s[j] = p2;

    vector<Coef, false> rr(j+2);

    rr[j+1] = fctr.unit();

    for(;;)
    {
        P& sj = s[j];
        index r = sj.degree();    // P should return -1 when polynomial is null.

        for(index k = j-1; k >= r+1; --k)
            s[k] = null(s[k]);

        if(j > r && r >= 0)
            s[r] = (sj*pow(sj.leading_coef(), j-r)) / pow(rr[j+1], j-r);

        if(r <= 0)break;

        {
            P pq; Coef mult;
            polynom_pseudodivide_simple(s[j+1], sj, pq, s[r-1], mult);
        }

        s[r-1] /= pow(-rr[j+1], j-r+2);
        j = r-1;
        rr[j+1] = s[j+1].leading_coef();
    }

    //output_aligned(std::cout << "\nSub Resultants =\n", s);
}


template <typename P1, typename P2, typename PCFactory>
typename P1::coef_type resultant_nonmodular
(const P1& p1, const P2& p2, PCFactory fctr)
{
    {
        // WARNING! THIS IS WORKAROUND FOR BUG IN subresultants_nonmodular, see bug #2655678.

        typedef typename P1::coef_type T;
        matrix<T, false> m;
        sylvester(p1, p2, m);
        return det(m);
    }

    // WARNING! Need to choose from P1::coef_type and P2::coef_type
    // insted just P1::coef_type as a type of the return value.

    vector<P1, false> s;    // WARNING! Need to choose from P1 and P2 insted just P1.
    if(p1.is_const())
        if(p2.is_const())
            return fctr.unit();
        else
            return power(p1.leading_coef_cpy(), p2.degree());
    else if(p2.is_const())
        return power(p2.leading_coef_cpy(), p1.degree());

    subresultants_nonmodular(p1, p2, s, fctr);
    ARAGELI_ASSERT_2(s[0].degree() <= 0);
    return s[0].leading_coef_cpy();
}


}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
