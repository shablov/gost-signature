/*****************************************************************************

    algebrslt.cpp

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
    \file algebrslt.cpp
    \brief The algebrslt.hpp file stuff implementation.
*/


#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_algebrslt)

#include <sstream>

#include "exception.hpp"
#include "cmp.hpp"
#include "vector.hpp"
#include "sparse_polynom.hpp"
#include "polyalg.hpp"
#include "sturm.hpp"
#include "resultant.hpp"
#include "berlekamp_hensel.hpp"

#include "algebrslt.hpp"


namespace Arageli
{


template
<
    typename PolyVec,
    typename SegVec,
    typename Rslt,
    typename P,
    typename Seg,
    typename F
>
void algebraic_func_rslt
(
    const PolyVec& polyvec, // source polynomials
    SegVec segvec, // source segments
    Rslt rslt,  // source resultant
    P& p,       // the result: polynomial
    Seg& seg,   // the result: segment where root is located
    F f    // the function that is performing on the source algebraic numbers
)
{
    // Implementation of this function is based on Loos, 1973. Source: Buhberger

    ARAGELI_ASSERT_0(polyvec.size() == segvec.size());

    //std::cout << "\n++++++++++ algebraic_func_rslt +++++++++++++\n";

    //typedef vector<Rslt, false> Rslt_factors;
    //typedef Rslt_factors::size_type size_type;

    //std::cout << "\nrslt = " << rslt;

    //// Make squre-free resultant.    // WARNING! Is it needed? See sturm specification.
    //Rslt gcd_rslt_df = gcd(rslt, diff(rslt));
    //gcd_rslt_df /= gcd_rslt_df.leading_coef_cpy();    // WARNING! Is it needed?
    //rslt /= gcd_rslt_df;


    //std::cout << "\nsf rslt = " << rslt;

    //rslt = polynom_primpart(rslt);

    //std::cout << "\nprimpart rslt = " << rslt;

    //std::cout << "\n\nBEGIN\nrslt = " << rslt;
    rslt = sqrfree_poly_rational(rslt);
    //std::cout << "\nsqrfree rslt = " << rslt;

    typedef typename Rslt::coef_type CT;
    typedef typename Rslt::template other_coef<big_int>::type IRslt;
    big_int mult;
    polynom_rational_to_int_value(rslt, mult);
    IRslt irslt;
    polynom_rational_to_int(rslt, irslt, mult);

    vector<Rslt> vrslt =
        factorize_berlekamp_hensel_sf(polynom_primpart(irslt));

    //output_aligned(std::cout << "\nvrslt = \n", vrslt);

    typedef typename vector<Rslt, false>::size_type size_type;

    for(;;)
    {
        size_type gsnr = 0;
        size_type iglast = 0;

        for(size_type i = 0; i < vrslt.size();)
        {
            vector<Rslt, false> ss;

            // TODO Move the following line to exterior loop
            sturm_diff_sys(vrslt[i], ss);

            //output_aligned(std::cout << "\nrslt Sturm system =\n", ss);
            //std::cout << "\ni = " << i << "\nvrslt[i] = " << vrslt[i];

            for(;;)
            {
                seg = f(segvec);

                //output_aligned(std::cout << "\nsegvec =\n", segvec);
                //std::cout << "\nseg = " << seg << "\n";

                size_type snr = sturm_number_roots(ss, seg);

                //std::cout << "\nseg = " << seg << "\nsnr = " << snr;

                if(snr == 0)
                {
                    vrslt.erase(i);    // this factor doesn't have the root
                    break;
                }
                else if(snr == 1)    // Congratulations! The interval is found for this factor.
                {
                    ++gsnr;
                    iglast = i;
                    ++i;
                    break;
                }
                else    // Make intervals more precise.
                    for(size_type j = 0; j < polyvec.size(); ++j)
                        if(!segvec[j].is_negligible())
                            interval_root_dichotomy
                            (
                                polyvec[j],
                                sign(polyvec[j].subs(segvec[j].first())),
                                segvec[j]
                            );
            }
        }

        if(gsnr == 0)
        {
            invalid_argument e;
            std::ostringstream msg;
            msg
                << "An operation cannot be performed on specified "
                << "arguments in algebraic number field."
                << " polyvec = " << polyvec
                << ", segvec = " << segvec
                << ", rslt = " << rslt
                << ", vrslt = " << vrslt;

            ARAGELI_EXCEPT_LOC_DESC(e, msg.str());
            throw e;
        }
        else if(gsnr == 1)
        {
            p = vrslt[iglast];
            //std::cout << "\np.degree() = " << p.degree() << "\n";
            break;
        }
        else
            // WARNING! COPY-PASTE!
            for(size_type j = 0; j < polyvec.size(); ++j)
                if(!segvec[j].is_negligible())
                    interval_root_dichotomy
                    (
                        polyvec[j],
                        sign(polyvec[j].subs(segvec[j].first())),
                        segvec[j]
                    );

    }

    //p = rslt;
    // WARNING! This is a place where the degree of the polynomial
    // is groing exponentially. Actually we need not p = rslt but
    // store only that irreducible factor of rslt which has
    // given root.


    //std::cout << "\np = " << p;

    //std::cout << "\nprimpart p = " << p;
    //std::cout << "\n++++++++ END algebraic_func_rslt +++++++++++\n";
}


namespace _Internal
{

template <typename T>
struct vector_2_plus_algebraic_helper
{
    typename T::value_type operator() (const T& x) const
    {
        ARAGELI_ASSERT_0(x.size() == 2);
        return x.front() + x.back();
    }
};


template <typename T>
struct vector_2_minus_algebraic_helper
{
    typename T::value_type operator() (const T& x) const
    {
        ARAGELI_ASSERT_0(x.size() == 2);
        return x.front() - x.back();
    }
};


template <typename T>
struct vector_2_multiplies_algebraic_helper
{
    typename T::value_type operator() (const T& x) const
    {
        ARAGELI_ASSERT_0(x.size() == 2);
        return x.front() * x.back();
    }
};


template <typename T>
struct vector_2_divides_algebraic_helper
{
    typename T::value_type operator() (const T& x) const
    {
        ARAGELI_ASSERT_0(x.size() == 2);
        return x.front() / x.back();
    }
};


} // namespace _Internal


template
<
    typename P1,
    typename Seg1,
    typename P2,
    typename Seg2,
    typename P3,
    typename Seg3
>
void algebraic_plus
(
    const P1& p1,
    const Seg1& seg1,
    const P2& p2,
    const Seg2& seg2,
    P3& p3,
    Seg3& seg3
)
{
    typedef sparse_polynom<P1> Pxy;
    typedef vector<P1, false> Polyvec;
    typedef vector<Seg1, false> Segvec;

    //std::cout << "\np1 = " << p1 << "\nseg1 = " << seg1;
    //std::cout << "\np2 = " << p2 << "\nseg2 = " << seg2;

    Polyvec polyvec;
    polyvec.push_back(p1);
    polyvec.push_back(p2);

    Segvec segvec;
    segvec.push_back(seg1);
    segvec.push_back(seg2);

    P1 rslt = resultant(p1.subs(Pxy("((x)-x)")), p2.subs(Pxy("x")));

    ARAGELI_EXCEPT_LOCCTRL_REGION_BEGIN
        algebraic_func_rslt
        (
            polyvec,
            segvec,
            rslt,
            p3,
            seg3,
            _Internal::vector_2_plus_algebraic_helper<Segvec>()
        );
    ARAGELI_EXCEPT_LOCCTRL_REGION_END
}


// WARNING! TEMPORARY.
template
<
    typename P1,
    typename Seg1,
    typename P2,
    typename Seg2,
    typename P3,
    typename Seg3
>
void algebraic_minus
(
    const P1& p1,
    const Seg1& seg1,
    const P2& p2,
    const Seg2& seg2,
    P3& p3,
    Seg3& seg3
)
{
    typedef sparse_polynom<P1> Pxy;
    typedef vector<P1, false> Polyvec;
    typedef vector<Seg1, false> Segvec;

    //std::cout << "\np1 = " << p1 << "\nseg1 = " << seg1;
    //std::cout << "\np2 = " << p2 << "\nseg2 = " << seg2;

    Polyvec polyvec;
    polyvec.push_back(p1);
    polyvec.push_back(p2);

    Segvec segvec;
    segvec.push_back(seg1);
    segvec.push_back(seg2);

    P1 rslt = resultant(p1.subs(Pxy("((x)+x)")), p2.subs(Pxy("x")));

    ARAGELI_EXCEPT_LOCCTRL_REGION_BEGIN
        algebraic_func_rslt
        (
            polyvec,
            segvec,
            rslt,
            p3,
            seg3,
            _Internal::vector_2_minus_algebraic_helper<Segvec>()
        );
    ARAGELI_EXCEPT_LOCCTRL_REGION_END
}


// WARNING! TEMPORARY.
template
<
    typename P1,
    typename Seg1,
    typename P2,
    typename Seg2,
    typename P3,
    typename Seg3
>
void algebraic_multiplies
(
    const P1& p1,
    const Seg1& seg1,
    const P2& p2,
    const Seg2& seg2,
    P3& p3,
    Seg3& seg3
)
{
    typedef sparse_polynom<P1> Pxy;
    typedef vector<P1, false> Polyvec;
    typedef vector<Seg1, false> Segvec;

    //std::cout << "\np1 = " << p1 << "\nseg1 = " << seg1;
    //std::cout << "\np2 = " << p2 << "\nseg2 = " << seg2;

    Polyvec polyvec;
    polyvec.push_back(p1);
    polyvec.push_back(p2);

    Segvec segvec;
    segvec.push_back(seg1);
    segvec.push_back(seg2);

    Pxy p1n = reciprocal_poly(p1).subs(Pxy("x"));
    typedef typename P1::monom monom;
    for
    (
        typename Pxy::monom_iterator i = p1n.monoms_begin();
        i != p1n.monoms_end();
        ++i
    )
        i->coef() *=
            monom
            (
                unit<typename monom::coef_type>(),
                p1n.degree() - i->degree()
            );
    //std::cout << "\np1n = " << p1n;

    P1 rslt = resultant(p1n, p2.subs(Pxy("x")));

    ARAGELI_EXCEPT_LOCCTRL_REGION_BEGIN
        algebraic_func_rslt
        (
            polyvec,
            segvec,
            rslt,
            p3,
            seg3,
            _Internal::vector_2_multiplies_algebraic_helper<Segvec>()
        );
    ARAGELI_EXCEPT_LOCCTRL_REGION_END
}


// WARNING! TEMPORARY.
template
<
    typename P1,
    typename Seg1,
    typename P2,
    typename Seg2,
    typename P3,
    typename Seg3
>
void algebraic_divides
(
    const P1& p1,
    const Seg1& seg1,
    const P2& p2,
    Seg2 seg2,
    P3& p3,
    Seg3& seg3
)
{
    typedef sparse_polynom<P1> Pxy;
    typedef vector<P1, false> Polyvec;
    typedef vector<Seg1, false> Segvec;

    //std::cout << "\np1 = " << p1 << "\nseg1 = " << seg1;
    //std::cout << "\np2 = " << p2 << "\nseg2 = " << seg2;

    Polyvec polyvec;
    polyvec.push_back(p1);
    polyvec.push_back(p2);

    if(sign(seg2.first())*sign(seg2.second()) <= 0)
    {
        ARAGELI_ASSERT_0(!is_null(p2.subs(null<typename Seg2::value_type>())));
        int lsign = sign(p2.subs(seg2.first()));

        do
        {
            interval_root_dichotomy(p2, lsign, seg2);
        }while(sign(seg2.first())*sign(seg2.second()) <= 0);
    }

    Segvec segvec;
    segvec.push_back(seg1);
    segvec.push_back(seg2);

    P1 rslt = resultant(p1.subs(Pxy("((x)*x)")), p2.subs(Pxy("x")));

    ARAGELI_EXCEPT_LOCCTRL_REGION_BEGIN
        algebraic_func_rslt
        (
            polyvec,
            segvec,
            rslt,
            p3,
            seg3,
            _Internal::vector_2_divides_algebraic_helper<Segvec>()
        );
    ARAGELI_EXCEPT_LOCCTRL_REGION_END
}


}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
