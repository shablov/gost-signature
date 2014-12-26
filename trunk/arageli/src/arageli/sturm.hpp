/*****************************************************************************

    sturm.hpp -- The Sturm algorithm for location of roots.

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
    This file contains declaration of Sturm algorithm for location of roots.
    Now the location only real (not complex) roots is implemented. Also
    the file contains several routines for Sturm system building and precising
    of found roots.
*/

#ifndef _ARAGELI_sturm_hpp_
#define _ARAGELI_sturm_hpp_

#include "config.hpp"

#include "exception.hpp"
#include "factory.hpp"
#include "vector.hpp"


namespace Arageli
{


/// Builds full Sturm system for polynomial.
/** After call, ss.front() is equal to p and ss.back() is constant. */
template <typename P, typename SS>
void sturm_diff_sys (const P& p, SS& ss);


/// Number of sign changes for Sturm system in point 'x' with precomputed sign.
/** The function counts the number of sign changes for given Sturm system.
    A sign of value obtained after substitution 'x' into a polynomial ss.front()
    must be already precomputed and passed through 'signpx' parameter.
    So, the function doesn't compute an expression sign(ss.front().subs(x)) but
    uses 'signpx' value insted. */
template <typename SS, typename T>
typename SS::size_type sturm_sign_changes
(const SS& ss, const T& x, int signpx);


/// Number of sign changes for Sturm system in point 'x'.
template <typename SS, typename T>
inline typename SS::size_type sturm_sign_changes (const SS& ss, const T& x)
{
    ARAGELI_ASSERT_0(!ss.is_empty());
    return sturm_sign_changes(ss, x, sign(ss.front().subs(x)));
}


/// Number of roots of polynomial ss.front() on segment 'seg' by Sturm system 'ss'.
template <typename SS, typename Seg>
typename SS::size_type sturm_number_roots (const SS& ss, const Seg& seg);


/// Vector version of simple sturm_number_roots.
template <typename SS, typename SegT, bool SegREFCNT>
vector<typename SS::size_type> sturm_number_roots
(
    const SS& ss,
    const vector<SegT, SegREFCNT>& lims    // WARNING! We need to replace explicit vector.
);


/// Real root location for polynomial by the Sturm algorithm on segment bs.
/** The function performs a real root loocation over T for nonzero polynomial p.
    It is looking for only roots in 'bs' segment. The result segments (each
    of them contains exactly one root) are placed into 'lims' that must be
    a vector of segments. */
template <typename T, typename P, typename LIMS, typename SegBound>
void sturm (const P& p, LIMS& lims, SegBound bs);


/// All real root location for polynomial by the Sturm algorithm.
/** See sturm function with 3 arguments. */
template <typename T, typename P, typename LIMS>
inline void sturm (const P& p, LIMS& lims)
{
    sturm<T>(p, lims, interval<T>(unit<T>(), null<T>()));
}


/// Dichotomy of an interval with one root of a polynomial to double precision.
/** Returns true if exact root is found, otherwise returns false.
    'lsign' should be already precomputed sign of p.subs(lims.first()). */
template <typename P, typename Lims>
bool interval_root_dichotomy (const P& p, int lsign, Lims& lims);


/// Fits intervals that located the roots to 'e' precision.
/** After call, for all i, 0 <= i < lims.size()
    an expression lims[i].length() <= e is satisfied. */
template <typename P, typename Lims, typename T>
bool interval_root_precise (const P& p, Lims& lims, const T& e);


/// Computes all real roots of polynomial with precision 'e'.
/** If e is zero, it means that the precision doesn't matter. */
template <typename T, typename P, typename Roots, typename Prec>
void roots_poly_real
(
    const P& p, Roots& roots,
    Prec& prec, const T& e = null<T>()
);


} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_STURM
    #include "sturm.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_STURM
#endif

#endif
