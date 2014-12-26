/*****************************************************************************

    test/fft_multiplication.cpp

    This file is a part of the Arageli library test base.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh
    Copyright (C) 2006--2007 Aleksey Bader

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
    \file fft_multiplication.cpp
    \brief Test for FFT multiplication.
*/

#include "stdafx.hpp"
//#define PRINT_OUTPUTS
//#define CHECK_TIME

using namespace Arageli;

TEST_FUNCTION(do_mult_fft, "Test FFT based multiplication.")
{
    bool is_ok = true;

    try
    {
        for(int j2 = 0; j2 < 10; ++j2)
        {
            const unsigned int num_lengths = 30000*j2 + 100;
            // generate two random integers approximately with equal lengths
            big_int a = big_int::random_with_length(num_lengths),
                    b = big_int::random_with_length(num_lengths);
            // alloc memory for output number
            _Internal::digit *w_digits = new _Internal::digit[magnitude(a)+magnitude(b)];
            w_digits[magnitude(a)+magnitude(b)-1] = 0;
            unsigned long w_len =
            do_mult_pollard<_Internal::digit,unsigned long>(a._digits(), b._digits(), w_digits, magnitude(a), magnitude(b));
            _Internal::digit *t_digits = new _Internal::digit[3*(magnitude(a)+magnitude(b))];
            _Internal::digit *r_digits = new _Internal::digit[magnitude(a)+magnitude(b)];
            // compare result with trivial multiplication
            // compare result with karatsuba method
            if (do_mult_karatsuba<_Internal::digit,unsigned>(a._digits(),
                    b._digits(), r_digits, t_digits, magnitude(a), magnitude(b)) != w_len)
            {
                is_ok = false;
            }
            int i;
            for (i = 0; i < w_len; ++i)
            {
                if(w_digits[i] != r_digits[i])
                {
                    is_ok = false;
                }
            }
            delete [] w_digits;
            delete [] r_digits;
            delete [] t_digits;
        }
    }
    catch(const Arageli::exception& e)
    {
        tout << e;
        return resEXCEPT;
    }
    catch(const std::exception& e)
    {
        tout << e.what();
        return resEXCEPT;
    }

    return is_ok ? resOK : resFAIL;
}
