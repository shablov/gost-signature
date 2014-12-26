/*****************************************************************************

    berlekamp_hensel.hpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh

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
    \file berlekamp_hensel.hpp
    \brief Factoring univariate polynomials using Berlekamp's algorithm.

    \todo
    - Dense polynomials
    - Polynomial factorization over finite field with big prime (see Shoup)
    - Multivariate polynomials factorization
    - Modular algorihm for polynomials GCD over integers
*/


#ifndef _ARAGELI_berlekamp_hensel_hpp_
#define _ARAGELI_berlekamp_hensel_hpp_

#include "config.hpp"

#include "vector.hpp"


namespace Arageli
{

namespace ctrl
{

/// Temporary idle-controller for factorize_berlekamp function.
struct factorize_berlekamp_idler
{
    class abort : public ctrl::abort {};
};

}


/// Factorize polynomial over finite number field.
/** @param p univariate polynomial over integers
    @param m prime integer, identifies target finite field GF(m)
    @param res the resulting vector with factors
    @param ctrler a controller */
template <typename P, typename M, typename VP, typename Ctrler>
void factorize_berlekamp
(
    const P& p,
    const M& m,
    VP& res,
    Ctrler ctrler
);


/// Factorize polynomial over finite number field.
/** @param p univariate polynomial over integers
    @param m prime integer, identifies target finite field GF(m)
    @param res the resulting vector with factors */
template <typename P, typename M, typename VP>
inline void factorize_berlekamp
(
    const P& p,
    const M& m,
    VP& res
)
{
    factorize_berlekamp(p, m, res, ctrl::factorize_berlekamp_idler());
}


/// Factorize polynomial over finite number field.
/** @param p univariate polynomial over integers
    @param m prime integer, identifies target finite field GF(m)
    The resulting vector with factors is a returned value. */
template <typename P, typename M>
inline vector<P> factorize_berlekamp (const P& p, const M& m)
{
    vector<P> res;
    factorize_berlekamp(p, m, res);
    return res;
}


/// Factorize polynomial over finite number field.
/** @param p univariate polynomial over residue integer class
    The resulting vector with factors is a returned value. */
template <typename P>
vector<P> factorize_berlekamp (const P& p);


namespace ctrl
{

/// Temporary idle-controller for factorize_berlekamp_msf function.
struct factorize_berlekamp_msf_idler
{
    class abort : public ctrl::abort {};
};

}


/// Factorize monic square-free polynomial over finite number field.
/** @param p univariate monic square-free polynomial over integers
    @param m prime integer, identifies target finite field GF(m)
    @param res the resulting vector with factors
    @param ctrler a controller */
template <typename P, typename M, typename VP, typename Ctrler>
void factorize_berlekamp_msf
(
    const P& p,
    const M& m,
    VP& res,
    Ctrler ctrler
);


/// Factorize monic square-free polynomial over finite number field.
/** @param p univariate monic square-free polynomial over integers
    @param m prime integer, identifies target finite field GF(m)
    @param res the resulting vector with factors */
template <typename P, typename M, typename VP>
inline void factorize_berlekamp_msf
(
    const P& p,
    const M& m,
    VP& res
)
{
    factorize_berlekamp_msf(p, m, res, ctrl::factorize_berlekamp_msf_idler());
}


/// Factorize monic square-free polynomial over finite number field.
/** @param p univariate monic square-free polynomial over integers
    @param m prime integer, identifies target finite field GF(m)
    The resulting vector with factors is a returned value. */
template <typename P, typename M>
inline vector<P> factorize_berlekamp_msf (const P& p, const M& m)
{
    vector<P> res;
    factorize_berlekamp_msf(p, m, res);
    return res;
}


/// Factorize monic square-free polynomial over finite number field.
/** @param p univariate monic square-free polynomial over residue integer class
    The resulting vector with factors is a returned value. */
template <typename P>
vector<P> factorize_berlekamp_msf (const P& p);



namespace ctrl
{

/// Temporary idle-controller for factorize_berlekamp_hensel function.
struct factorize_berlekamp_hensel_idler
{
    class abort : public ctrl::abort {};
};

}


/// Factorize polynomial over integer ring.
/** @param p univariate polynomial over integers
    @param res the resulting vector with factors
    @param ctrler a controller */
template <typename P, typename VP, typename Ctrler>
void factorize_berlekamp_hensel
(
    const P& p,
    VP& res,
    Ctrler ctrler
);


/// Factorize polynomial over integer ring.
/** @param p univariate polynomial over integers
    @param res the resulting vector with factors */
template <typename P, typename VP>
inline void factorize_berlekamp_hensel
(
    const P& p,
    VP& res
)
{
    factorize_berlekamp_hensel
        (p, res, ctrl::factorize_berlekamp_hensel_idler());
}


/// Factorize polynomial over integer ring.
/** @param p univariate polynomial over integers
    The resulting vector with factors is a returned value. */
template <typename P>
inline vector<P> factorize_berlekamp_hensel (const P& p)
{
    vector<P> res;
    factorize_berlekamp_hensel(p, res);
    return res;
}


namespace ctrl
{

/// Temporary idle-controller for factorize_berlekamp_hensel_sf function.
struct factorize_berlekamp_hensel_sf_idler
{
    class abort : public ctrl::abort {};
};

}


/// Factorize square-free polynomial over integer ring.
/** @param p univariate square-free polynomial over integers
    @param res the resulting vector with factors
    @param ctrler a controller */
template <typename P, typename VP, typename Ctrler>
void factorize_berlekamp_hensel_sf
(
    const P& p,
    VP& res,
    Ctrler ctrler
);


/// Factorize square-free polynomial over integer ring.
/** @param p univariate square-free polynomial over integers
    @param res the resulting vector with factors */
template <typename P, typename VP>
inline void factorize_berlekamp_hensel_sf
(
    const P& p,
    VP& res
)
{
    factorize_berlekamp_hensel_sf
        (p, res, ctrl::factorize_berlekamp_hensel_sf_idler());
}


/// Factorize square-free polynomial over integer ring.
/** @param p univariate square-free polynomial over integers
    The resulting vector with factors is a returned value. */
template <typename P>
inline vector<P> factorize_berlekamp_hensel_sf (const P& p)
{
    vector<P> res;
    factorize_berlekamp_hensel_sf(p, res);
    return res;
}



} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_berlekamp_hensel
    #include "berlekamp_hensel.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_berlekamp_hensel
#endif

#endif    // #ifndef _ARAGELI_berlekamp_hensel_hpp_
