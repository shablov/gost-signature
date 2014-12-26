/*****************************************************************************

    algebraic.cpp

    This file is a part of the Arageli library.

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
    \file algebraic.cpp
    \brief The algebraic.hpp file stuff implementation.

    <!--ADD ADDITIONAL FILE DESCRIPTION HERE-->
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_algebraic)

// REFERENCE ADDITIONAL HEADERS HERE

#include "algebraic.hpp"


namespace Arageli
{


template <typename TP, typename TS, typename Poly, typename Seg>
void algebraic_config_default<TP, TS, Poly, Seg>::normalize (Poly& poly, Seg& seg)
{
    // WARNING!
    ARAGELI_ASSERT_0(is_normal(poly, seg));

    poly = sqrfree_poly_rational(poly);

    big_int mult;    // WARNING! big_int
    polynom_rational_to_int_value(poly, mult);

    typedef typename Poly::template other_coef<big_int>::type PI;
    PI ipoly;
    polynom_rational_to_int(poly, ipoly, mult);

    vector<Poly> vp = factorize_berlekamp_hensel(ipoly);
    for(std::size_t i = 0; i < vp.size(); ++i)
        if(is_normal(vp[i], seg))
        {
            poly = vp[i];
            break;
        }

    ARAGELI_ASSERT_0(is_normal(poly, seg));
}


template
<
    typename TP,
    typename TS,
    typename Poly,
    typename Seg,
    typename Config
>
algebraic<TP, TS, Poly, Seg, Config>::algebraic (const Poly& px) :
    poly_m(px)
{
    if(Arageli::is_null(poly_m))
        seg_m = null<Seg>();
    else
    {
        vector<Seg, false> lims;
        poly_m = sqrfree_poly_rational(poly_m);    // WARNING! Only for rationals.
        sturm<TS>(poly_m, lims);
        ARAGELI_ASSERT_0(!lims.is_empty());
        seg_m = lims.front();
        config_m.init(poly_m, seg_m);
    }
}


template
<
    typename TP,
    typename TS,
    typename Poly,
    typename Seg,
    typename Config
>
algebraic<TP, TS, Poly, Seg, Config>
algebraic<TP, TS, Poly, Seg, Config>::operator- () const
{
    ARAGELI_ASSERT_0(is_normal());

    if(is_null())
        return *this;

    algebraic res = *this;
    res.polynom() = res.polynom().subs(Poly("-x"));
    Arageli::opposite(&res.segment());

    ARAGELI_ASSERT_1(is_normal());
    return res;
}


template
<
    typename TP,
    typename TS,
    typename Poly,
    typename Seg,
    typename Config
>
template
<
    typename TP2,
    typename TS2,
    typename Poly2,
    typename Seg2,
    typename Cfg2
>
algebraic<TP, TS, Poly, Seg, Config>&
algebraic<TP, TS, Poly, Seg, Config>::operator+=
(const algebraic<TP2, TS2, Poly2, Seg2, Cfg2>& x)
{
    ARAGELI_ASSERT_0(is_normal() && x.is_normal());
    if(x.is_null())
        return *this;
    if(is_null())
        return *this = x;
    algebraic_plus(poly_m, seg_m, x.poly_m, x.seg_m, poly_m, seg_m);
    ARAGELI_ASSERT_0(is_normal());
    return *this;
}

template
<
    typename TP,
    typename TS,
    typename Poly,
    typename Seg,
    typename Config
>
template
<
    typename TP2,
    typename TS2,
    typename Poly2,
    typename Seg2,
    typename Cfg2
>
algebraic<TP, TS, Poly, Seg, Config>&
algebraic<TP, TS, Poly, Seg, Config>::operator-=
(const algebraic<TP2, TS2, Poly2, Seg2, Cfg2>& x)
{
    ARAGELI_ASSERT_0(is_normal() && x.is_normal());
    if(x.is_null())
        return *this;
    if(is_null())
        return *this = -x;
    algebraic_minus(poly_m, seg_m, x.poly_m, x.seg_m, poly_m, seg_m);
    ARAGELI_ASSERT_0(is_normal());
    return *this;
}

template
<
    typename TP,
    typename TS,
    typename Poly,
    typename Seg,
    typename Config
>
template
<
    typename TP2,
    typename TS2,
    typename Poly2,
    typename Seg2,
    typename Cfg2
>
algebraic<TP, TS, Poly, Seg, Config>&
algebraic<TP, TS, Poly, Seg, Config>::operator*=
(const algebraic<TP2, TS2, Poly2, Seg2, Cfg2>& x)
{
    ARAGELI_ASSERT_0(is_normal() && x.is_normal());
    if(is_null())
        return *this;
    if(x.is_null())
        return *this = x;
    algebraic_multiplies(poly_m, seg_m, x.poly_m, x.seg_m, poly_m, seg_m);
    ARAGELI_ASSERT_0(is_normal());
    return *this;
}

template
<
    typename TP,
    typename TS,
    typename Poly,
    typename Seg,
    typename Config
>
template
<
    typename TP2,
    typename TS2,
    typename Poly2,
    typename Seg2,
    typename Cfg2
>
algebraic<TP, TS, Poly, Seg, Config>&
algebraic<TP, TS, Poly, Seg, Config>::operator/=
(const algebraic<TP2, TS2, Poly2, Seg2, Cfg2>& x)
{
    ARAGELI_ASSERT_0(is_normal() && x.is_normal());
    ARAGELI_ASSERT_0(!x.is_null());
    if(is_null())
        return *this;
    algebraic_divides(poly_m, seg_m, x.poly_m, x.seg_m, poly_m, seg_m);
    ARAGELI_ASSERT_0(is_normal());
    return *this;
}


template
<
    typename TP1,
    typename TS1,
    typename Poly1,
    typename Seg1,
    typename Cfg1,

    typename TP2,
    typename TS2,
    typename Poly2,
    typename Seg2,
    typename Cfg2
>
int cmp
(
    const algebraic<TP1, TS1, Poly1, Seg1, Cfg1>& a,
    const algebraic<TP2, TS2, Poly2, Seg2, Cfg2>& b
)
{
    algebraic<TP1, TS1, Poly1, Seg1, Cfg1> c = a - b;
    if(c.is_null())
        return 0;
    int lsign = sign(c.polynom().subs(c.segment().first()));
    while(sign(c.segment().first())*sign(c.segment().second()) <= 0)
        interval_root_dichotomy(c.polynom(), lsign, c.segment());
    return sign(c.segment().first());
}


template
<
    typename Out,

    typename TP1,
    typename TS1,
    typename Poly1,
    typename Seg1,
    typename Cfg1
>
Out& operator<< (Out& out, const algebraic<TP1, TS1, Poly1, Seg1, Cfg1>& x)
{
    out << "(" << x.polynom() << ", " << x.segment() << ")";
    return out;
}


}


#else    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...


namespace Arageli
{

// PLACE ALL NOT TEMPLATE NOT INLINE IMPLEMENTATIONS HERE

}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
