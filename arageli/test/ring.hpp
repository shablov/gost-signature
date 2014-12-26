/*****************************************************************************

    test/ring.hpp

    This file is a part of the Arageli library test base.

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
    \file ring.hpp
    \brief Basic test operations for rings.

    We suppose that `+' is commutative, assotiative and inversible;
    `-' is inverse of `+'; `*' is associative; `/' is division with remainder;
    `%' is remainder after division.
*/


#ifndef _ARAGELI_tests_ta_ring_hpp_
#define _ARAGELI_tests_ta_ring_hpp_

#include "stdafx.hpp"

using namespace Arageli;


template <typename T1, typename T2>
bool is_plus_commutative (const T1& a, const T2& b)
{ return a + b == b + a; }

template <typename T1, typename T2, typename T3>
bool is_plus_associative (const T1& a, const T2& b, const T3& c)
{ return a + (b + c) == (a + b) + c; }

template <typename T1, typename T2>
bool is_multiplies_commutative (const T1& a, const T2& b)
{ return a * b == b * a; }

template <typename T1, typename T2, typename T3>
bool is_multiplies_associative (const T1& a, const T2& b, const T3& c)
{ return a * (b * c) == (a * b) * c; }

template <typename T1, typename T2>
bool is_minus_inverse_plus (const T1& a, const T2& b)
{ return (a - b) + b == a; }

template <typename T1, typename T2>
bool are_minus_null_correct ()
{ return -null<T1>() == null<T2>(); }

template <typename T1, typename T2>
bool are_minus_and_null_correct (const T2& a)
{ return null<T1>() == a - a; }




#endif
