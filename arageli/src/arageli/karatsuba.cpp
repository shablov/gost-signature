/*****************************************************************************

    karatsuba.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
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

/**
    \file karatsuba.cpp
    \brief The karatsuba.hpp file stuff implementation
*/


#include "config.hpp"
#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_karatsuba)
#include "karatsuba.hpp"
#include "bigar.hpp"
#include "_utility.hpp"
#include <iostream>

namespace Arageli
{

/**
Preliminary conditions: w[n+m+1] = 0!
                        m >= n
*/
template <typename N,typename T>
T do_mult_karatsuba(const N *u, const N *v, N *w, N *t, T m, T n, T threshold)
{
    ARAGELI_ASSERT_0(m>=n);

    T k = m >> 1;

    if (!is_null(k) && n > k && n > ARAGELI_KARATSUBA_THRESHOLD)
    {
        T k2 = k << 1;
        N *W0 = w+k2;
        N *W1 = w+k;
        N *W2 = w;
        const N *U0 = u+k;
        const N *U1 = u;
        const N *V0 = v+k;
        const N *V1 = v;

        T Clen, C1len, C2len, UV0len, UV1len;

        // allocate temprary space
        // N *C1 = new N[k+2];
        N *C1 = t;
        // N *C2 = new N[k+2];
        N *C2 = t+k+2;
        N *C = t+m+4;
        t += 2*m+6;

        UV0len = do_mult_karatsuba(U0, V0, W0, t, m-k, n-k); // mult U0*V0
        UV1len = do_mult_karatsuba(U1, V1, W2, t, k, k);     // mult U1*V1

        memcpy(C1, U0, sizeof(N)*(m-k));
        C1len = _Internal::do_add(C1, U1, m-k, k); // U0+U1
        if (n>=k2)    // V0+V1
        {
            // here n = m or n = m-1 and (n-k) = k or (n-k) = k+1
            memcpy(C2, V0, sizeof(N)*(n-k));
            C2len = _Internal::do_add(C2, V1, n-k, k);
        }
        else
        {
            memcpy(C2, V1, sizeof(N)*k);
            C2len = _Internal::do_add(C2, V0, k, n-k);
        }

        if (C2len > C1len)
        {
            Clen = do_mult_karatsuba(C2, C1, C, t, C2len, C1len); // mult C = C1*C2
        }
        else
        {
            Clen = do_mult_karatsuba(C1, C2, C, t, C1len, C2len); // mult C = C1*C2
        }

        _Internal::do_sub(C, W0, Clen, UV0len);
        _Internal::do_sub(C, W2, Clen, UV1len);

        _Internal::do_add(W1, C, m+n-k, Clen);    // W += C

        t -= 2*m+6;
    }
    else
    {
        // no sence to use karatsuba algorithm in this case
        return _Internal::do_mult_classic(u, v, w, m, n);
    };

    return (w[m+n-1] != 0) ? (m+n): (m+n-1);
};

}
#endif
