/*****************************************************************************

    pollard_mult.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    Copyright (C) 2007 Aleksey Bader

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
    \file pollard_mult.cpp
    \brief The pollard_mult.hpp file stuff implementation.

    ADD ADDITIONAL FILE DESCRIPTION HERE
*/


#include "config.hpp"
#include "pollard_mult.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_pollard_mult)

namespace Arageli
{

}


#else    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...


namespace Arageli
{
// prepare objects for pollard multiplication.
#ifdef BARRETT
    const CFFT_mult<_Internal::digit,unsigned long,pollard_p1,p1_inv,32>
        t1(pollard_p1, pollard_root1);
    const CFFT_mult<_Internal::digit,unsigned long,pollard_p2,p2_inv,30>
        t2(pollard_p2, pollard_root2);
    const CFFT_mult<_Internal::digit,unsigned long,pollard_p3,p3_inv,32>
        t3(pollard_p3, pollard_root3);
#else
    const CFFT_mult<_Internal::digit,unsigned long>
        t1(pollard_p1, pollard_root1);
    const CFFT_mult<_Internal::digit,unsigned long>
        t2(pollard_p2, pollard_root2);
    const CFFT_mult<_Internal::digit,unsigned long>
        t3(pollard_p3, pollard_root3);
#endif

}


#endif    // #if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) || ...
