/*****************************************************************************

    pollard_mult.hpp

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
\file pollard_mult.hpp
\brief Multiplication algorithm for two polynoms

ADD WHOLE FILE DESCRIPTION HERE
*/


#ifndef _ARAGELI_pollard_mult_hpp_
#define _ARAGELI_pollard_mult_hpp_

#include "config.hpp"
#include "fft.hpp"

namespace Arageli
{

template<typename ET, typename MT>
inline void add_int128
(
    typename doubled_type<ET>::d_value a_low,
    typename doubled_type<ET>::d_value a_high,
    typename doubled_type<ET>::d_value b_low,
    typename doubled_type<ET>::d_value b_high,
    typename doubled_type<ET>::d_value *result_low,
    typename doubled_type<ET>::d_value *result_high
)
{
    union
    {
        typename doubled_type<ET>::d_value i64[2];
        ET    i32[4];
    } _a, _b, _result;

    typename doubled_type<ET>::d_value tmp, carry;

    /* store operands into unions */
    _a.i64[0] = a_low;
    _a.i64[1] = a_high;

    _b.i64[0] = b_low;
    _b.i64[1] = b_high;

    /* add operands using 320bit parts (digit by digit)*/

    /* 1st digit */
    tmp  = _a.i32[0];
    tmp += _b.i32[0];
    _result.i32[0] = (ET) (tmp & 0xFFFFFFFF);
    carry = tmp >> 32;

    /* 2nd digit */
    tmp  = _a.i32[1];
    tmp += _b.i32[1];
    tmp += carry;
    _result.i32[1] = (ET) (tmp & 0xFFFFFFFF);
    carry = tmp >> 32;

    /* 3rd digit */
    tmp  = _a.i32[2];
    tmp += _b.i32[2];
    tmp += carry;
    _result.i32[2] = (ET) (tmp & 0xFFFFFFFF);
    carry = tmp >> 32;

    /* 4th digit */
    tmp  = _a.i32[3];
    tmp += _b.i32[3];
    tmp += carry;
    _result.i32[3] = (ET) (tmp & 0xFFFFFFFF);

    /* return result */
    *result_low  = _result.i64[0];
    *result_high = _result.i64[1];
}

template<typename ET, typename MT>
inline void mult_int64_by_int32
(
    typename doubled_type<ET>::d_value a,
    ET b,
    typename doubled_type<ET>::d_value *result_low,
    typename doubled_type<ET>::d_value *result_high
)
{
    union
    {
        typename doubled_type<ET>::d_value i64[2];
        ET    i32[4];
    } _a, _result;

    typename doubled_type<ET>::d_value tmp, carry;

    /* store operands into unions */
    _a.i64[0] = a;
    _a.i64[1] = 0;

    /* start to multiplicate */
    /* 1st digit */
    tmp  = _a.i32[0];
    tmp *= b;
    _result.i32[0] = (ET) (tmp & 0xFFFFFFFF);
    carry = tmp >> 32;

    /* 2nd digit */
    tmp  = _a.i32[1];
    tmp *= b;
    tmp += carry;
    _result.i32[1] = (ET) (tmp & 0xFFFFFFFF);

    carry = tmp >> 32;

    /* highest 64 bits contains carry only */
    *result_low = _result.i64[0];
    *result_high = carry;
}

template<typename ET, typename MT>
bool multiplication_check
(
    const ET *u,
    const ET *v,
    const ET *w,
    const MT n,
    const MT m,
    const MT p
)
{
    typedef typename doubled_type<ET>::d_value dd;
    bool ret = true;
    ET *tmp = new ET[n+m];
    unsigned long i,j;
    for (i = 0; i < n+m; ++i)
    {
        tmp[i] = 0;
    }
    for(i = 0; i < n; ++i)
    {
        for(j = 0; j < m; ++j)
        {
            tmp[i+j] += dd(u[i])*v[j] % p;
            tmp[i+j] %= p;
        }
    }
    for (j = 0; j < n+m; ++j)
    {
        if (tmp[j] != w[j])
            ret = false;
    }
    return ret;
}

const _Internal::digit pollard_p1 = 469762049; // 7*2^26+1
const _Internal::digit pollard_root1 = 3;
const _Internal::digit pollard_p2 = 167772161; // 5*2^25+1
const _Internal::digit pollard_root2 = 3;
const _Internal::digit pollard_p3 = 754974721; // 45*2^24+1
const _Internal::digit pollard_root3 = 11;
const _Internal::digit C2 = 130489458;
const _Internal::digit C3 = 190329765;
const _Internal::doubledigit N3 = _Internal::doubledigit(pollard_p1)*pollard_p2; /* POLARD_P1*POLARD_P2 */

// prepare objects for pollard multiplication.
#ifdef BARRETT
extern const CFFT_mult<_Internal::digit,unsigned long,pollard_p1,p1_inv,32> t1;
extern const CFFT_mult<_Internal::digit,unsigned long,pollard_p2,p2_inv,30> t2;
extern const CFFT_mult<_Internal::digit,unsigned long,pollard_p3,p3_inv,32> t3;
#else
extern const CFFT_mult<_Internal::digit,unsigned long> t1;
extern const CFFT_mult<_Internal::digit,unsigned long> t2;
extern const CFFT_mult<_Internal::digit,unsigned long> t3;
#endif

template<typename ET, typename MT>
MT do_mult_pollard(const ET *u, const ET *v, ET *w, const MT n, const MT m)
{
    static _Internal::digit u_buffer[n_max];
    static _Internal::digit v_buffer[n_max];
    static _Internal::digit w_buffer1[n_max];
    static _Internal::digit w_buffer2[n_max];
    static _Internal::digit w_buffer3[n_max];

    //N3 = pollard_p1;
    //N3 *= pollard_p2;
    MT i;
    for (i = 0; i < n; ++i)
    {
#ifdef BARRETT
        u_buffer[i] = barrett_reduction<pollard_p1, p1_inv, 32>(u[i]);
#else
        u_buffer[i] = u[i] % pollard_p1;
#endif
    }
    for (i = 0; i < m; ++i)
    {
#ifdef BARRETT
        v_buffer[i] = barrett_reduction<pollard_p1, p1_inv, 32>(v[i]);
#else
        v_buffer[i] = v[i] % pollard_p1;
#endif
    }
    t1.poli_multiply(u_buffer, n, v_buffer, m, w_buffer1);
    ARAGELI_ASSERT_2((multiplication_check<ET, MT>(u_buffer, v_buffer, w_buffer1, n, m, pollard_p1)));

    for (i = 0; i < n; ++i)
    {
#ifdef BARRETT
        u_buffer[i] = barrett_reduction<pollard_p2, p2_inv, 30>(u[i]);
#else
        u_buffer[i] = u[i] % pollard_p2;
#endif
    }
    for (i = 0; i < m; ++i)
    {
#ifdef BARRETT
        v_buffer[i] = barrett_reduction<pollard_p2, p2_inv, 30>(v[i]);
#else
        v_buffer[i] = v[i] % pollard_p2;
#endif
    }
    t2.poli_multiply(u_buffer, n, v_buffer, m, w_buffer2);
    ARAGELI_ASSERT_2((multiplication_check<ET, MT>(u_buffer, v_buffer, w_buffer2, n, m, pollard_p2)));

    for (i = 0; i < n; ++i)
    {
#ifdef BARRETT
        u_buffer[i] = barrett_reduction<pollard_p3, p3_inv, 32>(u[i]);
#else
        u_buffer[i] = u[i] % pollard_p3;
#endif
    }
    for (i = 0; i < m; ++i)
    {
#ifdef BARRETT
        v_buffer[i] = barrett_reduction<pollard_p3, p3_inv, 32>(v[i]);
#else
        v_buffer[i] = v[i] % pollard_p3;
#endif
    }
    t3.poli_multiply(u_buffer, n, v_buffer, m, w_buffer3);
    ARAGELI_ASSERT_2((multiplication_check<ET, MT>(u_buffer, v_buffer, w_buffer3, n, m, pollard_p3)));

    //__int64 y2, y3;
    signed long long y2, y3;
    typename doubled_type<ET>::d_value y3_low, y3_high, betta_low, betta_high;
    for (i=0; i < m+n; ++i)
    {
        /* y1 = W1 mod n1 = W1*/

        y2 = w_buffer2[i];
        y2 -= w_buffer1[i];
        y2 %= pollard_p2;
        y2 *= C2;
        y2 %= pollard_p2;
        if (y2 < 0)
        {
            y2 += pollard_p2;
        }

        y2 *= pollard_p1;   // *N2
        y2 += w_buffer1[i]; // +y1

        // y3 = C3*(W3 - y2) % n3;

        y3 = w_buffer3[i];
        y3 -= y2;
        y3 %= pollard_p3;
        if (y3 < 0)
        {
            y3 += pollard_p3;
        }
        y3 *= C3;
        y3 %= pollard_p3;

        /* if (y3 < 0) y3 += n3; */
        /* y3 *= N3; */

        mult_int64_by_int32<ET, MT>(N3, ET(y3), &y3_low, &y3_high);
        add_int128<ET, MT>(y3_low, y3_high, y2, 0, &y3_low, &y3_high);

        if (i == 0)
        {
            betta_low = y3_low;
            betta_high = y3_high;
        }
        else
        {
            /* make carry from prevous betta value */
            betta_low = (betta_low >> 32) | (betta_high << 32);
            betta_high = 0;
            /* add y3 to betta */
            add_int128<ET, MT>(y3_low, y3_high, betta_low, betta_high, &betta_low, &betta_high);
        }
        w[i] = (ET) (betta_low & 0xFFFFFFFF);
    }
    return (w[m+n-1] != 0) ? (m+n): (m+n-1);
}


} // namesapce Arageli


#if 0 //UNCOMMENT THE FOLLOWING FIVE LINES IF THERE IS pollard_mult.cpp FILE
#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_pollard_mult
    #include "pollard_mult.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_pollard_mult
#endif
#endif

#endif    // #ifndef _ARAGELI_pollard_mult_hpp_
