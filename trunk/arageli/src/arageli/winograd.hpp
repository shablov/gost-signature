/*****************************************************************************

    winograd.hpp

    This file is a part of the Arageli library.

    Copyright (C) 2007 Sergey V. Lobanov

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
    \file winograd.hpp
    \brief Alternate algorithms for matrix multiplication

    <!--ADD WHOLE FILE DESCRIPTION HERE-->
*/


#ifndef _ARAGELI_winograd_hpp_
#define _ARAGELI_winograd_hpp_

#include "config.hpp"
#include "matrix.hpp"
#include "big_int.hpp"


namespace Arageli
{
template <typename T1,typename T2,typename T3> void
mm_winograd_fund (const T1& G,const T2& H, T3& R);

/// Winograd's Matrix Multiplication algorithm
/** Matrix R=G*H, where size(G)=[a b] and size(H)=[b c].<br/>
    Count of multiplications = <i>O</i>(1/2*a*b*c+1/2*b*a+1/2*a*c).<br/>
    Count of additions = <i>O</i>(3/2*a*b*c+1/2*b*a+1/2*a*c).<br/>
    Advanced memory use <i>O</i>(a+c)*/
template <typename T1,typename T2,typename T3> void
mm_winograd (const T1& G,const T2& H, T3& R);

template <typename T1,typename T2,bool REFCNT3>
inline void mm_winograd
(
    const T1& G,
    const T2& H,
    matrix<signed int,REFCNT3>& R
)
{
    mm_winograd_fund (G,H,R);
}

template <typename T1,typename T2,bool REFCNT3>
inline void mm_winograd
(
    const T1& G,
    const T2& H,
    matrix<unsigned int,REFCNT3>& R
)
{
    mm_winograd_fund (G,H,R);
}

template <typename T1,typename T2,bool REFCNT3>
inline void mm_winograd
(
    const T1& G,
    const T2& H,
    matrix<signed short,REFCNT3>& R
)
{
    mm_winograd_fund (G,H,R);
}

template <typename T1,typename T2,bool REFCNT3>
inline void mm_winograd
(
    const T1& G,
    const T2& H,
    matrix<unsigned short,REFCNT3>& R
)
{
    mm_winograd_fund (G,H,R);
}

template <typename T1,typename T2,bool REFCNT3>
inline void mm_winograd
(
    const T1& G,
    const T2& H,
    matrix<signed long,REFCNT3>& R
)
{
    mm_winograd_fund (G,H,R);
}

template <typename T1,typename T2,bool REFCNT3>
inline void mm_winograd
(
    const T1& G,
    const T2& H,
    matrix<unsigned long,REFCNT3>& R
)
{
    mm_winograd_fund (G,H,R);
}

template <typename T1,typename T2,bool REFCNT3>
inline void mm_winograd
(
    const T1& G,
    const T2& H,
    matrix<float,REFCNT3>& R
)
{
    mm_winograd_fund (G,H,R);
}

template <typename T1,typename T2,bool REFCNT3>
inline void mm_winograd
(
    const T1& G,
    const T2& H,
    matrix<double,REFCNT3>& R
)
{
    mm_winograd_fund (G,H,R);
}

template <typename T1,typename T2,bool REFCNT3>
inline void mm_winograd
(
    const T1& G,
    const T2& H,
    matrix<bool,REFCNT3>& R
)
{
    mm_winograd_fund (G,H,R);
}


#ifdef ARAGELI_INT64_SUPPORT

template <typename T1,typename T2,bool REFCNT3>
inline void mm_winograd
(
    const T1& G,
    const T2& H,
    matrix<signed __int64,REFCNT3>& R
)
{
    mm_winograd_fund (G,H,R);
}

template <typename T1,typename T2,bool REFCNT3>
inline void mm_winograd
(
    const T1& G,
    const T2& H,
    matrix<unsigned __int64,REFCNT3>& R
)
{
    mm_winograd_fund (G,H,R);
}

#endif

#ifdef ARAGELI_LONG_LONG_SUPPORT
template <typename T1,typename T2,bool REFCNT3>
inline void mm_winograd
(
    const T1& G,
    const T2& H,
    matrix<signed long long,REFCNT3>& R
)
{
    mm_winograd_fund (G,H,R);
}

template <typename T1,typename T2,bool REFCNT3>
inline void mm_winograd
(
    const T1& G,
    const T2& H,
    matrix<unsigned long long,REFCNT3>& R
)
{
    mm_winograd_fund (G,H,R);
}

#endif

template <typename T1,typename T2,bool REFCNT3>
inline void mm_winograd
(
    const T1& G,
    const T2& H,
    matrix<long double,REFCNT3>& R
)
{
    mm_winograd_fund (G,H,R);
}

/// Winograd's matrix multiplication only for signed types(for example matrix<signed int>)
template <typename T1,typename T2,typename T3> void
mm_winograd_unsafe (const T1& G,const T2& H, T3& R);

template <typename T1,typename T2,bool REFCNT3>
inline void mm_winograd
(
    const T1& G,
    const T2& H,
    matrix<big_int,REFCNT3>& R
)
{
    mm_winograd_unsafe (G,H,R);
}

} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_winograd
    #include "winograd.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_winograd
#endif

#endif    // #ifndef _ARAGELI_winograd_hpp_
