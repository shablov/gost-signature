/*****************************************************************************

    karatsuba_vs_classic.cpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    Copyright (C) 2005--2007 Sergey S. Lyalin
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
    \file karatsuba_vs_classic.cpp
    \brief Utility to determine threshold value.
    This value is minimum number length for which we karatsuba multiplication algorithm.
    Classic multiplication algorithm is used for numbers with length less than threshold value.

*/


#include <arageli/arageli.hpp>

using namespace Arageli;
// We need internal namespace because of using interal big_int representation.
using namespace Arageli::_Internal;

double pollard(big_int a, big_int b)
{
    int i;
    const unsigned digit_len = sizeof(_Internal::digit)*8;
    unsigned mask = 0;
    for (i = 0; i < digit_len; ++i)
    {
        mask |= 1 << i;
    }
    unsigned a_len = /*a.length()>>digit_len*/0;
    unsigned b_len = /*b.length()>>digit_len*/0;
    big_int rt = a;
    for (; rt != 0; ++a_len)
    {
        rt >>= digit_len;
    }
    rt = b;
    for (; rt != 0; ++b_len)
    {
        rt >>= digit_len;
    }
    _Internal::digit *a_digits = new _Internal::digit[a_len];
    _Internal::digit *b_digits = new _Internal::digit[b_len];
    rt = a;
    for (i = 0; i < a_len; ++i)
    {
        a_digits[i] = _Internal::digit(rt & big_int(mask));
        rt >>= digit_len;
    }
    rt = b;
    for (i = 0; i < b_len; ++i)
    {
        b_digits[i] = _Internal::digit(rt & big_int(mask));
        rt >>= digit_len;
    }
    big_int out = 0;
    _Internal::digit *w_digits = new _Internal::digit[a_len+b_len];
    w_digits[a_len+b_len-1] = 0;
    timer t(false);
    t.start();
    do_mult_pollard<_Internal::digit,unsigned>(a_digits, b_digits, w_digits, a_len, b_len);
    t.stop();
    delete []a_digits;
    delete []b_digits;
    delete []w_digits;
    return t.time();
}

int main ()
{
    unsigned int lower_bound = 150;
    unsigned int upper_bound = 3000;
    unsigned int repeat = 80;
    unsigned int tolerance = 20;
    unsigned int init_step = 100;
    // Create mask for extracting digits list from big_int.
    unsigned mask = BASE-1;
    int i;

    do
    {
        double t1_l = 0.0, t1_m = 0.0, t1_u = 0.0, t2_l = 0.0, t2_m = 0.0, t2_u = 0.0;
        std::cout << "Lower bound = " << lower_bound << "; Upper bound = " << upper_bound << ";\n";
        big_int bt1, bt2;
        big_int a_l = big_int::random_with_length(lower_bound);
        big_int b_l = big_int::random_with_length(lower_bound);
        bt1 = a_l;
        bt2 = b_l;
        unsigned int l_len = lower_bound>>5+1;
        digit *a_l_digits = new digit[l_len];
        digit *b_l_digits = new digit[l_len];
        for (i = 0; i < l_len; ++i)
        {
            a_l_digits[i] = digit(bt1 & big_int(mask));
            bt1 >>= bits_per_digit;
            b_l_digits[i] = digit(bt2 & big_int(mask));
            bt2 >>= bits_per_digit;
        }
        big_int a_m = big_int::random_with_length((upper_bound+lower_bound)/2);
        big_int b_m = big_int::random_with_length((upper_bound+lower_bound)/2);
        bt1 = a_m;
        bt2 = b_m;
        unsigned int m_len = ((upper_bound+lower_bound)/2)>>5+1;
        digit *a_m_digits = new digit[m_len];
        digit *b_m_digits = new digit[m_len];
        for (i = 0; i < m_len; ++i)
        {
            a_m_digits[i] = digit(bt1 & big_int(mask));
            bt1 >>= bits_per_digit;
            b_m_digits[i] = digit(bt2 & big_int(mask));
            bt2 >>= bits_per_digit;
        }
        big_int a_u = big_int::random_with_length(upper_bound);
        big_int b_u = big_int::random_with_length(upper_bound);
        bt1 = a_u;
        bt2 = b_u;
        unsigned int u_len = upper_bound>>5+1;
        digit *a_u_digits = new digit[u_len];
        digit *b_u_digits = new digit[u_len];
        digit *w_u_digits = new digit[2*u_len];
        digit *t_digits = new digit[4*u_len];
        for (i = 0; i < u_len; ++i)
        {
            a_u_digits[i] = digit(bt1 & big_int(mask));
            bt1 >>= bits_per_digit;
            b_u_digits[i] = digit(bt2 & big_int(mask));
            bt2 >>= bits_per_digit;
        }
        for (int i = 0; i < repeat; ++i)
        {
            unsigned int res = 30;
            w_u_digits[2*u_len - 1] = 0;
            {
                timer t(false);
                t.start();
                do_mult_classic(a_l_digits, b_l_digits, w_u_digits, l_len, l_len);
                t.stop();
                t1_l += t.time();
            }
            w_u_digits[2*u_len - 1] = 0;
            {
                timer t(false);
                t.start();
                do_mult_karatsuba(a_l_digits, b_l_digits, w_u_digits, t_digits, l_len, l_len, res);
                t.stop();
                t2_l += t.time();
            }
            w_u_digits[2*u_len - 1] = 0;
            {
                timer t(false);
                t.start();
                do_mult_classic(a_m_digits, b_m_digits, w_u_digits, m_len, m_len);
                t.stop();
                t1_m += t.time();
            }
            w_u_digits[2*u_len - 1] = 0;
            {
                timer t(false);
                t.start();
                do_mult_karatsuba(a_m_digits, b_m_digits, w_u_digits, t_digits, m_len, m_len, res);
                t.stop();
                t2_m += t.time();
            }
            w_u_digits[2*u_len - 1] = 0;
            {
                timer t(false);
                t.start();
                do_mult_classic(a_u_digits, b_u_digits, w_u_digits, u_len, u_len);
                t.stop();
                t1_u += t.time();
            }
            w_u_digits[2*u_len - 1] = 0;
            {
                timer t(false);
                t.start();
                do_mult_karatsuba(a_u_digits, b_u_digits, w_u_digits, t_digits, u_len, u_len, res);
                t.stop();
                t2_u += t.time();
            }
        }
        t1_l /= repeat; t1_m /= repeat; t1_u /= repeat;
        t2_l /= repeat; t2_m /= repeat; t2_u /= repeat;
        if (t1_u < t2_u)
        {
            upper_bound += init_step;
            continue;
        }
        if (t2_l < t1_l)
        {
            lower_bound -= init_step;
            if (lower_bound < 0) lower_bound = 100;
            continue;
        }
        if (t1_m < t2_m)
        {
            lower_bound = (lower_bound + upper_bound) / 2;
        }
        if (t2_m < t1_m)
        {
            upper_bound -= (upper_bound - lower_bound) / 2;
        }
        if (t2_m == t1_m)
        {
            lower_bound += (upper_bound - lower_bound) / 4;
            upper_bound -= (upper_bound - lower_bound) / 4;
        }
        delete [] a_l_digits;
        delete [] b_l_digits;
        delete [] a_m_digits;
        delete [] b_m_digits;
        delete [] a_u_digits;
        delete [] b_u_digits;
        delete [] w_u_digits;
        delete [] t_digits;
    } while ((upper_bound - lower_bound) > tolerance);
    std::cout << "Threshold value = " << (upper_bound + lower_bound) / 2 << " or " << (upper_bound + lower_bound) / 64 << " digits" << std::endl;
    return (upper_bound + lower_bound) / 2;
}

