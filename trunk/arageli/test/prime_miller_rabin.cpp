/*****************************************************************************

    test/prime_miller_rabin.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    Copyright (C) 2006 Aleksey Bader

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

#include "stdafx.hpp"


TEST_FUNCTION(is_pseudoprime_miller_rabin, "Test is_pseudoprime_miller_rabin function")
{
    using namespace Arageli;

    bool fail = false;

    for(int i = 3; i <= 1000; ++i)
    {
        if(is_prime(i) != is_pseudoprime_miller_rabin(i, 20))
        {
            tout << "is_pseudoprime_miller_rabin is failed on i = int(" << i << ").\n";
            fail = true;
        }

        if(is_prime(big_int(i)) != is_pseudoprime_miller_rabin(big_int(i), 20))
        {
            tout << "is_pseudoprime_miller_rabin is failed on i = big_int(" << i << ").\n";
            fail = true;
        }
    }

    if(fail)return resFAIL;
    return resOK;
}


/* End of file prime_miller_rabin.cpp */
