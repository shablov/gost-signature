/*****************************************************************************

    bigar.cpp -- see the bigar.hpp file for description.

    This file is a part of the Arageli library.

    Copyright (C) 1999 -- 2005 Nikolai Yu. Zolotykh

    Updated by Agafonov E.A. 2000

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
    \file bigar.cpp
    \brief The bigar.hpp file stuff implementation.
*/


#include "config.hpp"

#include <cstddef>

#include "exception.hpp"
#include "bigar.hpp"
#include "karatsuba.hpp"
#include "pollard_mult.hpp"

namespace Arageli
{
namespace _Internal
{


// divides two integers and returns both the quotient and the remainder
// NOTE. Inline specificator makes this function is internal liked.
inline void divide
(
    digit& quotient, digit& remainder,
    doubledigit numerator, extendeddigit denominator
)
{
    quotient = digit(numerator / (doubledigit)denominator);
    remainder = digit(numerator % (doubledigit)denominator);
}


std::size_t do_big_int_to_bdn (digit* a, digit* b, std::size_t n, digit bdn_radix)
{
    // BASE to bdn_radix convertion
    // BASE > bdn_radix

    // input:  a[n-1],...,a[0], n
    // output: b[m-1],...,b[0], m
    // returns m
    // a[n-1],...,a[0] are not const

    ARAGELI_ASSERT_1(BASE > bdn_radix);

    std::size_t m = 0;

    while (n > 0)
    {
        // division
        digit carry = 0;
        for(std::size_t j = n; j > 0; j--)
        {
            doubledigit tmp = doubledigit(carry) * BASE + a[j - 1];
            carry = digit(tmp % bdn_radix);
            a[j - 1] = digit(tmp / bdn_radix);
        }

        b[m] = carry;
        m++;

        if(!a[n - 1])
            n--;
    }

    return m;
}


std::size_t do_bdn_to_big_int (digit* a, digit* b, std::size_t n, digit bdn_radix)
{
    // bdn_radix to BASE convertion
    // BASE > bdn_radix

    // input:  a[n-1],...,a[0], n
    // output: b[m-1],...,b[0], m
    // returns m
    // a[n-1],...,a[0] are not const

    ARAGELI_ASSERT_1(BASE > bdn_radix);

    std::size_t m = 0;
    for(std::size_t i = n; i > 0; i--)
    {
        // (b[m-1],...,b[0]) = (b[m-1],...,b[0]) * bdn_radix + a[i - 1]
        digit carry = a[i - 1];

        for(std::size_t j = 0; j < m; j++)
        {
            doubledigit tmp = doubledigit(b[j]) * bdn_radix + carry;
            carry = digit(tmp / BASE);
            b[j] = digit(tmp % BASE);
        }

        if(carry)
        {
            b[m] = carry; // new digit
            m++;
        }
    }

    return m;
}


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4244)
#endif


std::size_t do_add (digit* p1, const digit* p2, std::size_t m, std::size_t n)
{
    // m - length of p1
    // n - length of p2
    // m must be >= n

    // actually, length of p1 must be m+1 - reserved for carry
    // do_add returns amount of main digits (m without carry or m+1 with carry)

    ARAGELI_ASSERT_1(m >= n);

    bit carry = 0;

    // 1. loop for 0 to n-1 - add digits of p2 to p1

    for(std::size_t i = 0; i < n; i++)
    {
        doubledigit tmp = doubledigit(p1[i]) + p2[i] + carry;
        p1[i] = digit(tmp % BASE);
        carry = digit(tmp / BASE);
    }

    // 2. loop for add carry

    for(std::size_t i = n; i < m; i++)
    {
        doubledigit tmp = doubledigit(p1[i]) + carry;
        p1[i] = digit(tmp % BASE);
        carry = digit(tmp / BASE);
        if(carry == 0) break;
    }

    if(carry)
    {
        p1[m] = 1;
        return m + 1;
    }
    else
        return m;
}


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif


int do_sub (digit* p1, const digit* p2, std::size_t m, std::size_t n)
{
    // m - length of p1
    // n - length of p2
    // m must be >= n
    //
    // do_sub returns borrow (0 or 1)

    ARAGELI_ASSERT_1(m >= n);

    bit borrow = 0;

    // 1. loop for 0 to n-1 - digits of p2 sub from digits of p1

    for(std::size_t i = 0; i < n; i++)
    {
        if(extendeddigit(p1[i]) >= extendeddigit(p2[i]) + borrow)
        {
            p1[i] = p1[i] - p2[i] - borrow;
            borrow = 0;
        }
        else
        {
            p1[i] = digit(BASE + p1[i] - p2[i] - borrow);
            borrow = 1;
        }
    }

    if(borrow == 0)return 0; // no borrow

    // 2. loop for borrow

    for(std::size_t i = n; i < m; i++)
    {
        if(p1[i] >= borrow)
        {
            p1[i] = p1[i] - borrow;
            return 0; // borrow = 0;
        }
        else
        {
            p1[i] = digit(BASE + p1[i] - borrow);
            borrow = 1;
        }
    }

    return 1; // borrow was accuped -> p2 > p1
}


std::size_t do_optimize (const digit* a, std::size_t n)
{
    // returns real length of a,
    // i.e. the number of digits except leading zeros.

    for(std::size_t i = n; i > 0; i--)
        if(a[i - 1])return i;

    return 0;
}

/// Performs multiplication of 2 sequences of digits by applying different methods.
/** Different algorithms including karatsuba and pollard methods are used
    depending on arguments lenghts.
    @param u    First number.
    @param v    Second number.
    @param w    Result of multiplication.
                The length of w = m + n or m + n - 1.
    @param m    Length of u.
    @param n    Length of v.
    @return     Multiplication result length (length of w).
*/
std::size_t do_mult (const digit* u, const digit* v, digit* w, std::size_t m, std::size_t n)
{
#ifdef ARAGELI_ENABLE_POLLARD_MULT
    if
    (
        m > ARAGELI_POLLARD_THRESHOLD &&
        n > ARAGELI_POLLARD_THRESHOLD &&
        m <= ARAGELI_POLLARD_UPPER_BOUND &&
        n <= ARAGELI_POLLARD_UPPER_BOUND
    )
    {
        return do_mult_pollard<digit,unsigned long>(u, v, w, m, n);
    }
#endif
#ifdef ARAGELI_ENABLE_KARATSUBA_MULT
    if
    (
        m > ARAGELI_KARATSUBA_THRESHOLD &&
        n > ARAGELI_KARATSUBA_THRESHOLD
    )
    {
        digit *t = new digit[3 * (n + m)];
        std::size_t ret = 0;
        //// make m >= n
        if (m < n)
        {
            std::size_t temp = m;
            m = n;
            n = temp;
            const digit *tmp = u;
            u = v;
            v = tmp;
        };
        if (m/n > 1)
        {
            for (std::size_t i = n; i < m + n; ++i)
            {
                w[i] = 0;
            }
            do_mult_karatsuba<digit, std::size_t>(u, v, w, t, n, n);
            for (std::size_t i = 1; i < m/n; ++i)
            {
                std::size_t temp_len = do_mult_karatsuba<digit, std::size_t>(&u[i*n], v, &t[2*(n+m)], t, n, n);
                do_add(&w[i*n], &t[2*(n+m)], temp_len, temp_len);
            }
            if (m - (m/n)*n)
            {
                std::size_t temp_len = do_mult(v, &u[(m/n)*n], t, n, m - (m/n)*n);
                do_add(&w[(m/n)*n], t, temp_len, temp_len);
            }
            ret = (w[m + n - 1]) ?  m + n :  m + n - 1;
        }
        else
        {
            ret = do_mult_karatsuba<digit, std::size_t>(u, v, w, t, m, n);
        }
        delete[] t;
        return ret;
    }
#endif
    return do_mult_classic(u, v, w, m, n);
}

/// Performs multiplication of 2 sequences of digits by classic method.
/** Performs multiplication of 2 sequences of digits by classic method.
    @param u    First number.
    @param v    Second number.
    @param w    Result of multiplication.
                The length of w = m + n or m + n - 1.
    @param m    Length of u.
    @param n    Length of v.
    @return     Multiplication result length (length of w).
*/
std::size_t do_mult_classic (const digit* u, const digit* v, digit* w, std::size_t m, std::size_t n)
{
    for(std::size_t i = 0; i < m; i++)
        w[i] = 0;

    for(std::size_t j = 0; j < n; j++) // v[j]
    {
        digit carry = 0;

        for(std::size_t i = 0; i < m; i++)  // u[i]
        {
            doubledigit tmp = doubledigit(u[i]) * v[j] + w[i + j] + carry;
            w[i + j] = digit(tmp % BASE);
            carry = digit(tmp / BASE);
        }

        w[j + m] = carry;
    }

    return
        (w[m + n - 1]) ?
        m + n :
        m + n - 1;
}

digit do_divide_by_digit (const digit* a, digit* p, std::size_t n, digit d)
{
    // p = a / d;
    // returns a % d
    // the real length of p = m, if p[m - 1] != 0
    //                      = m - 1, if p[m - 1] = 0

    // if d == 1 ...

    digit carry = 0;

    for(std::size_t i = n; i > 0; i--)
    {
        doubledigit tmp = doubledigit(carry) * BASE + a[i - 1];
        p[i - 1] = digit(tmp / d);
        carry = digit(tmp % d);
    }

    return carry;
}


std::size_t do_divide (digit* u, digit* v, digit* q, std::size_t m, std::size_t n)
{
    // D1 Normalization

    digit d = digit(BASE / (extendeddigit(v[n-1]) + 1));

    // (v[n-1], v[n-2],..., v[0]) = d * (v[n-1], v[n-2],..., v[0])

    digit carry = 0;
    for(std::size_t j = 0; j < n; j++)
    {
        doubledigit tmp = doubledigit(v[j]) * d + carry;
        carry = digit(tmp/BASE);
        v[j] = digit(tmp%BASE);
    }

    // (u[m], u[m-1],..., u[0]) = d * (u[m-1], u[m-2],..., u[0])

    carry = 0;
    for(std::size_t j = 0; j < m; j++)
    {
        doubledigit tmp = doubledigit(u[j]) * d + carry;
        carry = digit(tmp / BASE);
        u[j] = digit(tmp % BASE);
    }
    u[m] = carry;

    // D2
    // D3

    for(std::size_t j = m; j >= n; j--)
    {
        // main loop: one iteration in the loop
        // makes one digit in q

        digit q_hat, r_hat; // approximation for current digit
        // in the quotient and residue
        int overflow = 0;

        if(u[j] >= v[n-1])
        {
            q_hat = digit(BASE - 1);
            if(doubledigit(v[n-1]) + u[j-1] >= BASE)
                overflow = 1;
            else
                r_hat = v[n-1] + u[j-1];
        }
        else
            divide(q_hat, r_hat, doubledigit(u[j]) * BASE + u[j - 1], v[n - 1]);

        if(!overflow)
            while
            (
                doubledigit(v[n - 2]) * q_hat >
                doubledigit(r_hat) * BASE + u[j - 2]
            )
            { // loop at most two times
                q_hat--;
                if(doubledigit(r_hat) + v[n - 1] >= BASE)
                    break; // if overflow, break
                r_hat += v[n-1];
            }

        // D4 Multiply and subtract
        // (u[j],...,u[j-n]) = (u[j],...,u[j-n]) - (v[n-1],...,v[0]) * q_hat
        // with borrow if it is required

        digit carry = 0;
        for(std::size_t i = 0; i < n; i++)
        {
            digit higher, lower;

            //#ifdef __WATCOMC__
            //// the following is very strange because of watcomc error maybe
            //higher = ((doubledigit)v[i]*q_hat + carry)/BASE;
            //lower = ((doubledigit)v[i]*q_hat + carry)%BASE;
            //#else

            divide(higher, lower, doubledigit(v[i]) * q_hat + carry, BASE);

            //#endif

            if(u[j + i - n] >= lower)
                u[j + i - n] -= lower;
            else
            {
                higher++;
                u[j + i - n] = digit(BASE - lower + u[j + i - n]);
            }

            carry = higher;
        }

        int borrow;
        if(u[j] >= carry)
        {
            borrow = 0;
            u[j] -= carry;
        }
        else
        {
            borrow = 1;
            u[j] = digit(BASE - carry + u[j]);
        }

        // D5

        if(borrow)
        {
            // D6 addition
            // It is very rare event
            // (u[j],...,u[j-n]) = (u[j],...,u[j-n]) + (v[n-1],...,v[0])
            // we don't consider the highest borrow

            q_hat--;
            digit carry = 0;
            for(std::size_t i = 0; i < n; i++)
            {
                divide
                (
                    carry, u[j + i - n],
                    doubledigit(u[j + i - n]) + v[i] + carry,
                    extendeddigit(BASE)
                );
            }
        }

        // The quotient digit:
        q[j - n] = q_hat;
    }

    // D8 denormalization
    // (u[n-1],...,u[0]) = (u[n-1],...,u[0]) / d

    carry = 0;
    for(std::size_t j = n; j > 0; j--)
        divide(u[j - 1], carry, doubledigit(carry) * BASE + u[j - 1], d);

    // optimize u --- residue
    std::size_t u_len = n;

    for(std::size_t j = n; j > 0; j--)
        if(u[j - 1])
            break;
        else
            u_len--;

    return u_len;
}


}

}
