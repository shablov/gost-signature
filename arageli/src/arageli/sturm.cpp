/*****************************************************************************

    sturm.cpp -- See declarations in sturm.hpp.

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

#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_STURM)

#include <iostream>    // only for debug output
#include <list>
#include <cstdlib>

#include "interval.hpp"
#include "cmp.hpp"
#include "gcd.hpp"
#include "polyalg.hpp"

#include "sturm.hpp"


namespace Arageli
{

namespace _Internal
{

template <typename T, typename N>
struct sturm_segment
{
    sturm_segment ()
    {}

    sturm_segment
    (
        const T& limfirst,
        const T& limsecond,
        const N& changefirst,
        const N& changesecond
    ) :
        lims(limfirst, limsecond),
        changes(changefirst, changesecond)
    {}

    interval<T> lims;
    interval<N> changes;
};

template <typename Out, typename T, typename N>
Out& operator<< (Out& out, const sturm_segment<T, N>& x)
{
    out << "(" << x.lims << ", " << x.changes << ")";
    return out;
}

}


template <typename P, typename SS>
void sturm_diff_sys (const P& p, SS& ss)
{
    ss.reserve(p.degree() + 1);

    ss.push_back(p);
    ss.push_back(polynom_primpart(diff(p)));

    for(std::size_t i = 2; !ss[i-1].is_const(); ++i)
    {
        P pr, pq;
        typename P::coef_type mult;
        polynom_pseudodivide_simple(-ss[i-2], ss[i-1], pq, pr, mult);
        ss.push_back(polynom_primpart(pr));
        //ss.push_back(-ss[i-2] % ss[i-1]);

        if(ss[i].is_null())
        {
            ss.pop_back();
            ARAGELI_ASSERT_2(is_null(ss % safe_reference(ss.back())));
            ss /= safe_reference(ss.back());
            break;
        }
    }
}


template <typename SS, typename T>
typename SS::size_type sturm_sign_changes (const SS& ss, const T& x, int signpx)
{
    ARAGELI_ASSERT_0(!ss.is_empty());

    if(ss.size() == 1)
        return 0;

    typename SS::size_type changes = 0;

    for(typename SS::const_iterator i = ss.begin() + 1; i != ss.end(); ++i)
        if(int cursign = sign(i->subs(x)))
        {
            if(signpx != 0 && cursign != signpx)
                changes++;
            signpx = cursign;
        }

    return changes;
}


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4715)
#endif

template <typename SS, typename Seg>
typename SS::size_type sturm_number_roots (const SS& ss, const Seg& seg)
{
    ARAGELI_ASSERT_0(!ss.is_empty());

    typedef typename SS::size_type size_type;

    switch(seg.kind())
    {
        case interval_empty:
            return 0;
        case interval_point:
            return is_null(ss.front().subs(seg.left()));
        case interval_length:
        {
            int lsign = sign(ss.front().subs(seg.left()));
            size_type lchanges = sturm_sign_changes(ss, seg.left(), lsign);
            if(lchanges == 0)
                return is_null(lsign);
            size_type rchanges = sturm_sign_changes(ss, seg.right());
            ARAGELI_ASSERT_0(lchanges >= rchanges);
            return lchanges - rchanges + is_null(lsign);
        }
        default:
            ARAGELI_ASSERT_1(!"It is impossible.");
    }
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

template <typename SS, typename SegT, bool SegREFCNT>
vector<typename SS::size_type> sturm_number_roots
(
    const SS& ss,
    const vector<SegT, SegREFCNT>& lims
)
{
    vector<typename SS::size_type> res(lims.size());
    for(std::size_t i = 0; i < res.size(); ++i)
        res[i] = sturm_number_roots(ss, lims[i]);
    return res;
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4018)
    #pragma warning (disable : 4288)
#endif

template <typename T, typename P, typename LIMS, typename SegBound>
void sturm (const P& p, LIMS& lims, SegBound bs)
{
    if(p.degree() <= 0)
    {
        lims.resize(0);
        return;
    }

    // Building of the Sturm system.

    typedef vector<P, false> SS;
    SS ss;
    sturm_diff_sys(p, ss);

    //output_aligned(cout << "\nSturm's system:\n", ss);

    typedef _Internal::sturm_segment<T, std::size_t> SSEG;
    SSEG t1;

    if(bs.first() > bs.second())
    {
        t1.lims.second() = root_upper_bound_cauchy<T>(p);
        t1.lims.first() = -t1.lims.second();
    }
    else
    {
        t1.lims.second() = bs.second();
        t1.lims.first() = bs.first();
    }

    // Copmutes sign changes at the ends of investigated segment.

    t1.changes.first() = sturm_sign_changes(ss, t1.lims.first());
    t1.changes.second() = sturm_sign_changes(ss, t1.lims.second());

    ARAGELI_ASSERT_1(t1.changes.first() >= t1.changes.second());

    if(t1.changes.first() == t1.changes.second())
    {
        // polynomial hasn't real roots
        lims.resize(0);
        return;
    }

    if(t1.changes.first() - t1.changes.second() == 1)
    {
        // polynomial has only one root
        lims.assign(1, t1.lims);
        return;
    }

    typedef std::list<SSEG> LSSEG;
    LSSEG lsseg;

    lsseg.push_back(t1);


    for(typename LSSEG::iterator lssegi = lsseg.begin(); lssegi != lsseg.end();)
    {
        if
        (
            lssegi->lims.first() == lssegi->lims.second() ||    // exact root
            lssegi->changes.first() - lssegi->changes.second() == 1    // one root
        )
        {
            ++lssegi;
            continue;
        }

        //std::cout << "\nlssegi->lims = " << lssegi->lims;
        //std::cout << "\nlssegi->changes = " << lssegi->changes;

        ARAGELI_ASSERT_1(lssegi->lims.first() < lssegi->lims.second());
        ARAGELI_ASSERT_1(lssegi->changes.first() - lssegi->changes.second() >= 2);

        T mid = (lssegi->lims.first() + lssegi->lims.second())/2;

        //cout << "\nmid = " << mid;

        ARAGELI_ASSERT_1(lssegi->lims.first() < mid);
        ARAGELI_ASSERT_1(mid < lssegi->lims.second());

        bool exact_root = is_null(ss[0].subs(mid));
        int changes = sturm_sign_changes(ss, mid);

        typename LSSEG::iterator new_lssegi = lssegi;

        if(lssegi->changes.first() == changes)
        {
            // there are no roots
            lssegi->lims.first() = mid;
        }
        else
        {
            new_lssegi = lsseg.insert
            (
                lssegi,
                SSEG
                (
                    lssegi->lims.first(), mid,
                    lssegi->changes.first(), changes
                )
            );

            lssegi->lims.first() = mid;
            lssegi->changes.first() = changes;
        }

        if(lssegi->changes.first() - changes == 1)
            new_lssegi = lssegi;    // pass segment with one root

        if(exact_root)
            lsseg.insert(lssegi, SSEG(mid, mid, changes, changes));

        if(changes == lssegi->changes.second())
        {
            if(new_lssegi == lssegi)
                new_lssegi = lsseg.erase(lssegi);
            else
                lsseg.erase(lssegi);
        }

        lssegi = new_lssegi;
    }

    // Here segments can overlapped and share roots.
    // Exact root location.

    {
        typename LSSEG::iterator
            lssegi = lsseg.begin();

        for(;;)
        {
            typename LSSEG::iterator lssegend = lsseg.end();
            if(lssegi == lssegend)
                break;
            --lssegend;
            if(lssegi == lssegend)
                break;

            ARAGELI_ASSERT_1
            (
                !is_null(p.subs(lssegi->lims.first())) ||
                lssegi->changes.is_point()
            );

            if(lssegi->changes.is_point())
            {
                typename LSSEG::iterator next = lssegi;
                ++next;
                ARAGELI_ASSERT_1(next != lsseg.end());
                if(next->lims.first() == lssegi->lims.second())
                    if(next->changes.is_point())
                        lsseg.erase(next);
                    else
                    {
                        int lsign = -sign(p.subs(next->lims.second()));
                        ARAGELI_ASSERT_1(lsign);

                        do
                        {
                            if(interval_root_dichotomy(p, lsign, next->lims))
                                next->changes.first() = next->changes.second();
                        }while(next->lims.first() == lssegi->lims.second());
                    }
            }
            else
            {
                typename LSSEG::iterator next = lssegi;
                ++next;
                ARAGELI_ASSERT_1(next != lsseg.end());
                if(next->lims.first() == lssegi->lims.second())
                    if(is_null(p.subs(lssegi->lims.second())))
                    {
                        ARAGELI_ASSERT_1(next->changes.is_point());
                        lssegi = lsseg.erase(lssegi);
                        continue;
                    }
                    else
                    {
                        int lsign = sign(p.subs(lssegi->lims.first()));
                        ARAGELI_ASSERT_1(lsign);

                        do
                        {
                            //std::cout << " " << lssegi->lims << " ";
                            if(interval_root_dichotomy(p, lsign, lssegi->lims))
                                lssegi->changes.first() = lssegi->changes.second();
                        }while(next->lims.first() == lssegi->lims.second());
                    }
            }

            ++lssegi;
        }

    }

    ARAGELI_ASSERT_2(p.degree() >= 0);
    ARAGELI_ASSERT_1(lsseg.size() <= p.degree());
    lims.resize(lsseg.size());

    typename LSSEG::const_iterator lssegi = lsseg.begin();
    for(std::size_t i = 0; i < lims.size(); ++i, ++lssegi)
        lims[i] = lssegi->lims;

    ARAGELI_ASSERT_2(is_unit(sturm_number_roots(ss, lims)));
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

template <typename P, typename Lims>
bool interval_root_dichotomy (const P& p, int lsign, Lims& lims)
{
    typedef typename Lims::value_type T;

    T mid = (lims.first() + lims.second())/2;
    ARAGELI_ASSERT_0(lims.first() < mid);
    ARAGELI_ASSERT_0(mid < lims.second());
    int msign = sign(p.subs(mid));

    if(is_null(msign))
    {// exact root
        lims.first() = lims.second() = mid;
        return true;
    }
    else if(lsign == msign)
        lims.first() = mid;
    else
        lims.second() = mid;

    return false;
}


template <typename P, typename Lims, typename T>
bool interval_root_precise (const P& p, Lims& lims, const T& e)
{
    T re = lims.length();    // real error
    if(is_null(re))
        return true;

    if(lims.length() > e)
    {
        int lsign = sign(p.subs(lims.first()));

        do
        {
            if(interval_root_dichotomy(p, lsign, lims))
                return true;
        }
        while(lims.length() > e);
    }

    return false;
}


template <typename T, typename P, typename Roots, typename Prec>
void roots_poly_real
(
    const P& p,
    Roots& roots,
    Prec& prec,
    const T& e
)
{
    ARAGELI_ASSERT_0(!p.is_null());
    typedef vector<interval<T> > Lims;
    Lims lims;
    sturm<T>(p, lims);

    // computes roots more exactly

    if(!is_null(e))
        for(typename Lims::iterator i = lims.begin(); i != lims.end(); ++i)
            interval_root_precise(p, *i, e);

    roots.resize(lims.size());
    prec.resize(lims.size());

    for(std::size_t i = 0; i < lims.size(); ++i)
    {
        roots[i] = lims[i].first();
        prec[i] = lims[i].length();
    }
}


}

#endif // #ifndef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
