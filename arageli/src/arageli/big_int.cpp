/*****************************************************************************

    big_int.cpp -- see the big_int.hpp file for description.

    This file is a part of the Arageli library.

    WARNIG. This file has no complate implementation.

    Copyright (C) 1999 -- 2005, 2008 Nikolai Yu. Zolotykh

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
    \file big_int.cpp
    \brief The big_int.hpp file stuff implementation.
*/

#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_BIG_INT)


#include <cstddef>
#include <cstdlib>  // it maybe for std::rand only
#include <limits>
#include <cmath>

#include "big_int.hpp"
#include "_utility.hpp"


namespace Arageli
{


namespace _Internal
{

template <typename T> struct Unsigned { typedef T Type; };

#define _ARAGELI_IMPL_UNSIGNED_SIGNED(TYPE)    \
    template <> struct Unsigned<TYPE> { typedef unsigned TYPE Type; }

_ARAGELI_IMPL_UNSIGNED_SIGNED(char);
_ARAGELI_IMPL_UNSIGNED_SIGNED(short);
_ARAGELI_IMPL_UNSIGNED_SIGNED(int);
_ARAGELI_IMPL_UNSIGNED_SIGNED(long);

#ifdef ARAGELI_INT64_SUPPORT
    _ARAGELI_IMPL_UNSIGNED_SIGNED(__int64);
#endif

template <> struct Unsigned<signed char> { typedef unsigned char Type; };

}


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4146)
    #pragma warning (disable : 4244)
    #pragma warning (disable : 4804)
    #pragma warning (disable : 4800)
    #pragma warning (disable : 4806)
    #pragma warning (disable : 4305)
    #pragma warning (disable : 4293)
#endif


template <typename T>
void big_int::from_native_int (const T& x)
{
    typedef std::numeric_limits<T> Nl;
    ARAGELI_ASSERT_1(Nl::is_specialized);
    ARAGELI_ASSERT_1(Nl::is_integer);
    free_mem_and_alloc_zero();

    if(Arageli::is_null(x))
        return;
    else if(is_negative(x))
    {
        // WARNING. The following expression is not portable.
        from_native_int(static_cast<typename _Internal::Unsigned<T>::Type>(-x));
        number->sign = -1;
    }
    else if(Nl::digits <= _Internal::bits_per_digit)
    {
        _Internal::digit* mem = big_int::get_mem_for_data(1);

        try
        {
            mem[0] = _Internal::digit(x);
            free_mem_and_alloc_number(1, mem, 1);
        }
        catch(...)
        {
            free_data(mem);
            throw;
        }
    }
    else
    {
        std::size_t n =
            Nl::digits / _Internal::bits_per_digit +
            bool(Nl::digits % _Internal::bits_per_digit);

        _Internal::digit* mem = get_mem_for_data(n);

        try
        {
            T xx = x;

            for(std::size_t i = 0; i < n; ++i, xx >>= _Internal::bits_per_digit)
                mem[i] = xx & _Internal::max_digit;

            ARAGELI_ASSERT_1(Arageli::is_null(xx));

            std::size_t newlen;
            mem = optimize(newlen, mem, n);
            ARAGELI_ASSERT_1(newlen);
            free_mem_and_alloc_number(1, mem, newlen);
        }
        catch(...)
        {
            free_data(mem);
            throw;
        }

        number->sign = +1;
    }
}


template <typename T>
void big_int::from_native_float (const T& x)
{
    typedef std::numeric_limits<T> Nl;

    ARAGELI_ASSERT_1(Nl::is_specialized);
    ARAGELI_ASSERT_1(!Nl::is_integer);
    ARAGELI_ASSERT_0(!(Nl::has_infinity && x == Nl::infinity()));
    ARAGELI_ASSERT_0(!(Nl::has_quiet_NaN && x == Nl::quiet_NaN()));
    ARAGELI_ASSERT_0(!(Nl::has_signaling_NaN && x == Nl::signaling_NaN()));
    ARAGELI_ASSERT_0(!(Nl::has_denorm && x == Nl::denorm_min()));

    free_mem_and_alloc_zero();

    if(x > opposite_unit(x) && x < unit(x))
        return;
    else if(is_negative(x))
    {
        from_native_float(-x);
        number->sign = -1;
    }
    else
    {
        T xx = std::floor(x);
        int expon;
        xx = std::frexp(xx, &expon);

        ARAGELI_ASSERT_1(expon > 0);

        xx *= std::pow(2 * unit(x), Nl::digits + 1);
        expon -= Nl::digits + 1;

        ARAGELI_ASSERT_1(std::floor(xx) == xx);

        std::size_t n =
            (Nl::digits + 1) / _Internal::bits_per_digit +
            bool((Nl::digits + 1) % _Internal::bits_per_digit);

        _Internal::digit* mem = get_mem_for_data(n);

        try
        {
            T module = _Internal::max_digit;
            module += unit(module);

            for(std::size_t i = 0; i < n; ++i)
            {
                T remaind = std::fmod(xx, module);

                ARAGELI_ASSERT_1(std::floor(remaind) == remaind);
                ARAGELI_ASSERT_1(_Internal::digit(remaind) == remaind);

                mem[i] = _Internal::digit(remaind);
                xx = (xx - remaind) / module;

                ARAGELI_ASSERT_1(std::floor(xx) == xx);
            }

            ARAGELI_ASSERT_1(Arageli::is_null(xx));

            std::size_t newlen;
            mem = big_int::optimize(newlen, mem, n);
            ARAGELI_ASSERT_1(newlen);
            free_mem_and_alloc_number(1, mem, newlen);
        }
        catch(...)
        {
            free_data(mem);
            throw;
        }

        number->sign = +1;

        if(expon > 0)
            *this <<= expon;
        else
            *this >>= -expon;
    }
}


template <typename T>
T big_int::to_native_int () const
{
    typedef std::numeric_limits<T> Nl;
    ARAGELI_ASSERT_1(Nl::is_specialized);
    ARAGELI_ASSERT_1(Nl::is_integer);

    ARAGELI_ASSERT_0(big_int(Nl::min()) <= *this && *this <= big_int(Nl::max()));

    if(is_null())return factory<T>::null();
    else if(sign() < 0)
    {
        // WARNING. The following expression is not portable.
        return
            -static_cast<T>
            (to_native_int_without_sign<typename _Internal::Unsigned<T>::Type>());
    }
    else return to_native_int_without_sign<T>();
}


template <typename T>
T big_int::to_native_int_without_sign () const
{
    typedef std::numeric_limits<T> Nl;
    ARAGELI_ASSERT_1(Nl::is_specialized);
    ARAGELI_ASSERT_1(Nl::is_integer);
    ARAGELI_ASSERT_1(*this <= big_int(Nl::max()));
    ARAGELI_ASSERT_1(!is_null());

    T res = number->data[0];

    for(std::size_t i = 1; i < number->len; ++i)
        res |= number->data[i] << (i * _Internal::bits_per_digit);

    return res;
}


template <typename T>
T big_int::to_native_float () const
{
    typedef std::numeric_limits<T> Nl;

    ARAGELI_ASSERT_1(Nl::is_specialized);
    ARAGELI_ASSERT_1(!Nl::is_integer);

    if(is_null())
        return factory<T>::null();
    if(*this < big_int(-Nl::max()))
    {
        ARAGELI_ASSERT_0(Nl::has_infinity);
        return -Nl::infinity();
    }
    else if(*this > big_int(Nl::max()))
    {
        ARAGELI_ASSERT_0(Nl::has_infinity);
        return Nl::infinity();
    }
    else
    {
        big_int t = *this;
        int expon = 0;
        std::size_t blen = t.length();

        if(blen - 1 > Nl::digits)
        {
            ARAGELI_ASSERT_1(size_t(int(blen - 1 - Nl::digits)) == blen - 1 - Nl::digits);
            expon = static_cast<int>(blen - 1 - Nl::digits);
            t >>= expon;
        }

        ARAGELI_ASSERT_1(t.length() - 1 <= Nl::digits);

        T res = factory<T>::null();

        T module = _Internal::max_digit;
        module += unit(module);
        T curscale = unit(module);

        for(std::size_t i = 0; i < t.number->len; ++i)
        {
            res += t.number->data[i]*curscale;
            curscale *= module;
        }

        res *= std::pow(2*unit(res), expon);
        if(sign() < 0)opposite(&res);

        ARAGELI_DEBUG_EXEC_1(big_int backres = res);
        ARAGELI_ASSERT_1(backres.length() == length());

        ARAGELI_ASSERT_1
        (
            (length() <= Nl::digits && backres - *this == 0)  ||
            (
                length() > Nl::digits &&
                (backres - *this).length() <= length() - Nl::digits
            )
        );

        return res;
    }
}


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif


template <typename Stream>
Stream& io_binary<big_int>::output_stream (Stream& out, const big_int& x)
{
    int sign = x.number->sign;
    output_binary_stream(out, sign);
    if(sign)
    {
        std::size_t len = x.number->len;    // length in limbs
        output_binary_stream(out, len);
        output_binary_stream(out, x.number->data, len);
    }

    return out;
}


template <typename Stream>
Stream& io_binary<big_int>::input_stream (Stream& in, big_int& x)
{
    // The following first reads of SIGN and LEN can't break x value.

    int sign;
    if(!input_binary_stream(in, sign))
        return in;
    ARAGELI_ASSERT_ALWAYS(sign == 0 || sign == -1 || sign == +1);

    if(sign)
    {
        // The number isn't zero. Read LEN and DIGITS.

        std::size_t len;
        if(!input_binary_stream(in, len))
            return in;
        ARAGELI_ASSERT_ALWAYS(len > 0);

        if(x.number->refs == 1 && x.number->len == len)
            x.number->sign = sign;
        else
            x.free_mem_and_alloc_number(sign, x.get_mem_for_data(len), len);

        // Load DIGITS.
        if(!input_binary_stream(in, x.number->data, len))
        {
            // A new value load fails and an old value is lost.
            // Make sure that x object is in correct state.
            x.number->data[len - 1] = 1;    // kills all leading zeros
        }
    }
    else
    {
        // The number is zero.
        x = big_int();  // WARNING! replace by big_int::assign_null()
    }

    return in;
}


}


#else


#include <cstdlib>
#include <malloc.h>
#include <sstream>
#include <limits>
#include <cctype>

#include "big_int.hpp"
#include "rational.hpp"


namespace
{
    typedef Arageli::_Internal::digit digit;
}


namespace Arageli
{

void big_arith_error(const char *s)
{
    throw big_int::exception(std::string("Big arith error: ") + s);
}


/***************************/
/*                         */
/*    low level memory     */
/*   managment routines    */
/*                         */
/***************************/


digit* big_int::get_mem_for_data (std::size_t nitems)
{
    digit* p = reinterpret_cast<digit*>(std::malloc(nitems * sizeof(digit)));
    if(!p)
        big_arith_error("the heap overflow");
    return p;
}


void big_int::free_data (digit *p)
{
    free(p);
}


digit* big_int::realloc_data (digit* p, std::size_t newnitems)
{
    if(newnitems)
    {
        p = reinterpret_cast<digit*>(realloc(p, newnitems * sizeof(digit)));
        if(!p)
            big_arith_error("the heap overflow");
    }
    else
    {
        free_data(p);
        p = 0;
    }

    return p;
}


void big_int::copy_data
(digit* dest, const digit* source, std::size_t newnitems)
{
    memmove(dest, source, newnitems * sizeof(digit));
}

/***************************/
/*                         */
/*    number allocation    */
/*         routines        */
/*                         */
/***************************/

void big_int::alloc_number
(int new_sign, digit * new_data, std::size_t new_len)
{
    ARAGELI_ASSERT_1(new_sign == -1 || new_sign == 0 || new_sign == 1);

    number = new big_struct;

    number->sign = new_sign;
    number->data = new_data;
    number->len = new_len;
    number->refs = 1;
}

void big_int::free_number()
{
    if(!number)return;
    number->refs--;
    if(number->refs)return;
    free_data(number->data);
    delete number;
    number = 0;
}

void big_int::free_mem_and_alloc_number
(int new_sign, digit* new_data, std::size_t new_len)
{
    free_number();
    alloc_number(new_sign, new_data, new_len);
}


void big_int::free_mem_and_alloc_zero ()
{
    free_number();
    alloc_zero();
}

/***************************/
/*                         */
/*      calc_bdn_radix     */
/*                         */
/***************************/

void calc_bdn_radix (digit radix, digit& bdn_radix, std::size_t& chars_per_digit)
{
    //bdn_radix = maximal power of radix fit in digit;

    bdn_radix = 1;
    chars_per_digit = 0;
    digit t = _Internal::max_digit/radix;

    while(t)
    {
        t /= radix;
        bdn_radix *= radix;
        chars_per_digit++;
    }
}


/***************************/
/*                         */
/*         optimize        */
/*                         */
/***************************/

digit* big_int::optimize (std::size_t& new_len, digit * p, std::size_t len)
{
    // deleting leading zeros

    new_len = _Internal::do_optimize(p, len);

    if(new_len != len)
        if(new_len)
        {
            digit* new_p = get_mem_for_data(new_len);
            copy_data(new_p, p, new_len);
            free_data(p);
            p = new_p;
        }
        else
        {
            free_data(p);
            p = 0;
        }

    return p;
}

/***************************/
/*                         */
/*      constructors       */
/*    and destructors      */
/*                         */
/***************************/

big_int::big_int (const char* str)
{
    std::istringstream s(str);
    big_int b;
    s >> b;
    if(!s && !s.eof())
        throw incorrect_string(str);
    alloc_zero();

    try
    {
        *this = b;
    }
    catch(...)
    {
        free_number(); throw;
    }
}


/***************************/
/*                         */
/*        operator =       */
/*                         */
/***************************/


big_int& big_int::operator= (const big_int & b)
{
    ARAGELI_ASSERT_1(b.number->sign == -1 || b.number->sign == 0 || b.number->sign == 1);

    // make a copy of a number, just increments the reference count
    if(number == b.number)
        return *this;
    free_number();
    b.number->refs++;
    number = b.number;
    return *this;
}


/***************************/
/*                         */
/*      Unary operators    */
/*                         */
/***************************/


big_int big_int::operator- () const    // unary minus
{
    big_int a;
    digit *result;
    std::size_t blen;

    blen = number->len;

    if(number->sign == 0)
        a.free_mem_and_alloc_zero();
    else
    {
        result = get_mem_for_data(blen);
        copy_data(result, number->data, blen);      // copy data
        a.free_mem_and_alloc_number(-number->sign, result, blen);
    }

    return a;
}

/***************************/
/*                         */
/*        operator +       */
/*                         */
/***************************/

big_int operator+ (const big_int& b, const big_int& c)
{
    ARAGELI_ASSERT_1(b.number->sign == -1 || b.number->sign == 0 || b.number->sign == 1);
    ARAGELI_ASSERT_1(c.number->sign == -1 || c.number->sign == 0 || c.number->sign == 1);

    digit *sum;
    std::size_t sumlen;
    big_int a;
    int bsign = b.number->sign;
    int csign = c.number->sign;
    int sumsign;
    std::size_t blen = b.number->len;
    std::size_t clen = c.number->len;
    digit *bdata = b.number->data;
    digit *cdata = c.number->data;
    big_int::big_struct *u, *v;

    if(bsign == 0)
        a = c;
    else if(csign == 0)
        a = b;
    else
    {
        if(bsign == csign)
        {
            sumsign = bsign;
            if (blen >= clen)
            {
                u = b.number;
                v = c.number;
            }
            else
            {
                u = c.number;
                v = b.number;
            }
            sum = big_int::get_mem_for_data(u->len + 1);
            big_int::copy_data(sum, u->data, u->len);
            sumlen = _Internal::do_add(sum, v->data, u->len, v->len);
        }
        else // bsign != csign
        {
            if(blen >= clen)
            {
                sum = big_int::get_mem_for_data(blen);
                sumsign = bsign;
                big_int::copy_data(sum, bdata, blen);
                if(_Internal::do_sub(sum, cdata, blen, clen))     // был заем
                    goto c_ge_b;                // yes, it is
                sum = big_int::optimize(sumlen, sum, blen);
            }
            else
            {
                sum = big_int::get_mem_for_data(clen);
            c_ge_b:
                sumsign = csign;
                big_int::copy_data(sum, cdata, clen);
                _Internal::do_sub(sum, bdata, clen, blen);
                sum = big_int::optimize(sumlen, sum, clen);
            }
        }

        if(sumlen)
            a.free_mem_and_alloc_number(sumsign, sum, sumlen);
        else
            a.free_mem_and_alloc_zero();
    }

    return a;
}


/***************************/
/*                         */
/*        operator -       */
/*                         */
/***************************/


big_int operator- (const big_int& b, const big_int& c)
{
    return b + (-c);
}


/***************************/
/*                         */
/*        operator *       */
/*                         */
/***************************/


big_int operator* (const big_int& b, const big_int& c)
{
    ARAGELI_ASSERT_1(b.number->sign == -1 || b.number->sign == 0 || b.number->sign == 1);
    ARAGELI_ASSERT_1(c.number->sign == -1 || c.number->sign == 0 || c.number->sign == 1);

    digit *result;
    std::size_t blen, clen, resultlen;
    int bsign, csign;
    big_int a;

    blen = b.number->len;
    clen = c.number->len;
    bsign = b.number->sign;
    csign = c.number->sign;

    if((bsign == 0) || (csign == 0))
        a.free_mem_and_alloc_zero();
    else
    {
        resultlen = blen + clen;
        result = big_int::get_mem_for_data(resultlen);
        resultlen = _Internal::do_mult
        (
            b.number->data,
            c.number->data,
            result,
            blen,
            clen
        );
        a.free_mem_and_alloc_number(bsign * csign, result, resultlen);
    }

    return a;
}


/***************************/
/*                         */
/*         xdivide          */
/*                         */
/***************************/


void _Internal::xdivide (big_int& a, const big_int& b, const big_int& c, big_int& res)
{
    // a = b / c; res = b % c

    digit *u, *v, *q, *r;
    std::size_t alen, blen, clen, rlen;
    digit runint;
    int bsign, csign;

    blen = b.number->len;
    clen = c.number->len;
    bsign = b.number->sign;
    csign = c.number->sign;

    if(!csign)
        big_arith_error("divide by zero");
    else if(!bsign)
    {
        a.free_mem_and_alloc_zero();
        res.free_mem_and_alloc_zero();
    }
    else if (blen < clen)
    {
        a.free_mem_and_alloc_zero();
        res = b;
    }
    else if (clen == 1)
    {
        q = big_int::get_mem_for_data(blen);
        runint =
            _Internal::do_divide_by_digit
            (
                b.number->data,
                q,
                blen,
                c.number->data[0]
            );

        if(q[blen - 1])
            alen = blen;
        else
            alen = blen - 1;

        if(alen)
            a.free_mem_and_alloc_number(bsign * csign, q, alen);
        else
        {
            a.free_mem_and_alloc_zero();
            big_int::free_data(q);
        }

        if(runint == 0)
            res.free_mem_and_alloc_zero();
        else
        {
            u = big_int::get_mem_for_data(1);
            u[0] = runint;
            res.free_mem_and_alloc_number(bsign, u, 1);
        }

    }
    else
    {
        u = big_int::get_mem_for_data(blen + 1);
        v = big_int::get_mem_for_data(clen);
        q = big_int::get_mem_for_data(blen - clen + 1);
        big_int::copy_data(u, b.number->data, blen);
        big_int::copy_data(v, c.number->data, clen);

        rlen = _Internal::do_divide(u, v, q, blen, clen);
        if(q[blen - clen])
            alen = blen - clen + 1;
        else
            alen = blen - clen;

        big_int::free_data(v);
        if(rlen == 0)
            res.free_mem_and_alloc_zero();
        else
        {
            r = big_int::get_mem_for_data(rlen);
            big_int::copy_data(r, u, rlen);
            res.free_mem_and_alloc_number(bsign, r, rlen);
        }

        big_int::free_data(u);
        if(alen != 0)
            a.free_mem_and_alloc_number(bsign * csign, q, alen);
        else
        {
            a.free_mem_and_alloc_zero();
            big_int::free_data(q);
        }
    }

    ARAGELI_ASSERT_1(b == c*a + res);
}


big_int operator% (const big_int& b, const big_int& c)
{
    big_int a, r;
    _Internal::xdivide(a, b, c, r);
    return r;
}


big_int operator/ (const big_int& b, const big_int& c)
{
    big_int a, r;
    _Internal::xdivide(a, b, c, r);
    return a;
}


/***************************/
/*                         */
/*    Compare functions    */
/*                         */
/***************************/


int cmp (const big_int & a, const big_int & b)
{
    //sign(a-b)

    int result;

    int asign, bsign;
    std::size_t alen, blen;
    big_int c;

    asign = a.number->sign;
    bsign = b.number->sign;
    alen = a.number->len;
    blen = b.number->len;

    if(asign < bsign)
        result = -1;
    else if(asign > bsign)
        result = 1;
    else if(asign == 0)
        result = 0;            // asign == bsign == 0
    else if(alen < blen)
        result = -asign;    // asign == bsign != 0
    else if(alen > blen)
        result = asign;
    else
    { // asign == bsign != 0, alen == blen
        c = a - b;
        result = c.number->sign;
    }

    return result;
}


/***************************/
/*                         */
/*      Random numbers     */
/*                         */
/***************************/

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4800)
#endif

digit random_digit ()
{
    if(_Internal::max_digit <= RAND_MAX)return std::rand();

    static const std::size_t rand_max_len = big_int(RAND_MAX).length();    // WARNING! It is not efficient.
    const std::size_t n =
        _Internal::bits_per_digit / rand_max_len +
        bool(_Internal::bits_per_digit % rand_max_len);

    digit res = 0;
    for(std::size_t i = 0; i < n; ++i)
        (res <<= rand_max_len) |= std::rand();    // WARNING! Lower bits from rand is
                                            // placed to higher bits of the result.
    return res;
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif


big_int big_int::random_in_range (const big_int& max)
{
    if(max.is_null())
        return max;

    std::size_t len = max.length();

    for(;;)
    {
        big_int res = random_with_length_or_less(len);
        if(res <= max)
            return res;
    }
}


big_int big_int::random_with_length_or_less (std::size_t length) //bits
{
    int bits_in_highest = length % _Internal::bits_per_digit;
    std::size_t len = length/_Internal::bits_per_digit + 1;
    digit *p = big_int::get_mem_for_data(len);

    try
    {
        for(std::size_t i = 0; i < len - 1; i++)
            p[i] = random_digit();

        if(bits_in_highest)
            p[len - 1] = random_digit() >>
                (_Internal::bits_per_digit - bits_in_highest);
        else
            p[len - 1] = 0;

        std::size_t new_len;

        p = big_int::optimize(new_len, p, len);
        big_int a;

        if(new_len)
            a.free_mem_and_alloc_number(1, p, new_len);

        return a;
    }
    catch(...)
    {
        big_int::free_data(p);
        throw;
    }
}

/***************************/
/*                         */
/*     Stream operators    */
/*                         */
/***************************/

digit stream_radix(std::ios & s)
{
    if(s.flags() & std::ios::dec)
        return 10;
    if(s.flags() & std::ios::hex)
        return 16;
    if(s.flags() & std::ios::oct)
        return 8;
    else
        return 10; // it is very strange
}


std::ostream& operator<< (std::ostream& s, const big_int& x)
{
    if(s.flags() & std::ios_base::showpos && x.sign() > 0)
        s << '+';

    if(!x.number->sign)return s << "0";

    digit bdn_radix;
    std::size_t chars_per_block;
    calc_bdn_radix(stream_radix(s), bdn_radix, chars_per_block);

    std::size_t bdnlen = x.number->len;
    digit *bdn = big_int::get_mem_for_data(2 * bdnlen); //bdnlen + bdnlen/chars_per_block

    try
    {
        digit *numberdata = big_int::get_mem_for_data(x.number->len);

        try
        {
            big_int::copy_data(numberdata, x.number->data, x.number->len);
            bdnlen = _Internal::do_big_int_to_bdn(numberdata, bdn, x.number->len, bdn_radix);
        }
        catch(...)
        {
            big_int::free_data(numberdata);
            throw;
        }

        big_int::free_data(numberdata);

        _Internal::auto_stream_state _ass(s, s.flags() & ~std::iostream::showpos);

        if(x.number->sign == -1)
            s << '-';
        s << bdn[bdnlen - 1];

        for (std::size_t i = bdnlen - 1; i > 0; i--)
        {
            s.width(chars_per_block);
            s.fill('0');
            s << bdn[i - 1];
        }
    }
    catch(...)
    {
        big_int::free_data(bdn);
        throw;
    }

    big_int::free_data(bdn);
    return s;
}


inline void set_stream_radix (std::ios& s, digit radix)
{
    switch(radix)
    {
        case 10:
            s.setf(std::ios::dec, std::ios::basefield);
            break;
        case 16:
            s.setf(std::ios::hex, std::ios::basefield);
            break;
        case 8:
            s.setf(std::ios::oct, std::ios::basefield);
            break;
        default:
            throw big_int::exception("Value for radix is invalid");
    }
}


std::istream& operator>> (std::istream& s, big_int& x)
{
#if 0    // old version

    // WARNING!!! This function is incorrect!

    char ch;
    int sign = 1;
    digit radix = 10;
    bool brackets = false;

    while(s.get(ch) && isspace(ch));    // pass leading spaces

    if(!s)
        big_arith_error("empty string for reading as big int");

    if(ch == '-')
    {
        sign = -1;
        s.get(ch);
    }
    else if(ch == '+')
        s.get(ch);
    else if(ch == '(')
    {
        s.get(ch);
        brackets = true;
    }

    if(!s)big_arith_error("invalid string format for reading as big int");

    // now in ch the first digit of number notation

    int zero = 0;
    if (ch == '0')
    {
        zero = 1;
        s.get(ch);
        switch (ch)
        {
            case 'o':
                radix = 8;
                break;
            case 'x':
                radix = 16;
                break;
            default:
                s.putback(ch);
        }
    }
    else
        s.putback(ch);

    do    // pass non significant first zeros
    {
        s.get(ch);
    }while (ch == '0');
    s.putback(ch);

    std::ostringstream buffer;
    std::size_t tellp = 0;

    while(s.get(ch))
    {
        if
        (
            radix == 10 && isdigit(ch) ||
            radix == 16 && isxdigit(ch) ||
            radix == 8  && isdigit(ch) && ch < '8'
        )
        {
            buffer << ch;
            ++tellp;
        }
        else
        {
            s.putback(ch);
            break;
        }
    }

    if(tellp)
    {
        if(!zero)
            big_arith_error("empty string for reading as big int");
        x = 0;
        return s;
    }

    std::string buffer_cpp_str = buffer.str();
    const char* buffer_str = buffer_cpp_str.c_str();

    digit bdn_radix;
    std::size_t chars_per_block;
    calc_bdn_radix(radix, bdn_radix, chars_per_block);

    std::size_t length_of_str = buffer_cpp_str.length();
    std::size_t number_of_blocks = (length_of_str - 1)/chars_per_block + 1;
    digit* bdn = big_int::get_mem_for_data(number_of_blocks);
    digit* data = 0;
    std::size_t len = 0;

    try
    {
        std::size_t length_of_block = length_of_str % chars_per_block;
                                // for the first block
        if(!length_of_block)
            length_of_block = chars_per_block;

        const char * buffer_str_cur = buffer_str;
        for(std::size_t j = number_of_blocks; j > 0; j--)
        {
            std::istringstream
                stream_block
                    (std::string(buffer_str_cur).substr(0, length_of_block));
            set_stream_radix(stream_block, radix);
            stream_block >> bdn[j-1];
            buffer_str_cur += length_of_block;
            length_of_block = chars_per_block;  // for the last blocks
        }

        data = big_int::get_mem_for_data(number_of_blocks);
        len = _Internal::do_bdn_to_big_int(bdn, data, number_of_blocks, bdn_radix);
    }
    catch(...)
    {
        big_int::free_data(bdn); big_int::free_data(data);
        throw;
    }

    big_int::free_data(bdn);

    try
    {
        data = big_int::realloc_data(data, len);

        if(data)
            x.free_mem_and_alloc_number(sign, data, len);
        else
            x.free_mem_and_alloc_zero();
    }
    catch(...)
    {
        big_int::free_data(data);
        throw;
    }

    if(brackets)
    {
        s >> ch;
        if(ch != ')')
            big_arith_error("invalid string format for reading as big int");
    }

    s.clear();
    return s;

#else

    char ch = 0;
    int sign = 1;
    digit radix = 10;
    bool brackets = false;

    std::size_t len;

    //do s.get(ch); while (isspace(ch));
    s >> ch;    // pass leading spaces (if this feature is turn on for s)
                // and read first non space character
    if(!s)
    {
        s.clear(std::ios_base::badbit);
        return s;
    }

    if(ch == '(')
    {
        brackets = true;
        s >> ch;
    }

    if(ch == '-')sign = -1;
    else if(ch != '+')s.putback(ch);

    s.get(ch);

    int zero = 0;
    if(ch == '0')
    {
        zero = 1;
        s.get(ch);

        switch (ch)
        {
            case 'o':
                radix = 8;
                break;
            case 'x':
                radix = 16;
                break;
            default:
                s.putback(ch);
        }
    }
    else s.putback(ch);

    s.clear();
    do
    {
        s.get(ch);
    }while (ch == '0' && s);

    if(s)
        s.putback(ch);
    else
    {
        x = big_int();
        s.clear();
        return s;
    }

    std::string buffer;

    while(s.get(ch))
    {
        if
        (
            radix == 10 && isdigit(ch) ||
            radix == 16 && isxdigit(ch) ||
            radix == 8  && isdigit(ch) && ch < '8'
        )
            buffer += ch;
        else
        {
            s.putback(ch);
            break;
        }
    }

    if(buffer.empty())
    {
        if(!zero)
            throw big_int::incorrect_string(std::string(1, s.peek()));

        if(brackets && !_Internal::read_literal(s, ")"))
            throw big_int::incorrect_string(std::string(1, s.peek()));

        x = big_int();
        s.clear();
        return s;
    }

    digit bdn_radix;
    std::size_t chars_per_block;
    calc_bdn_radix(radix, bdn_radix, chars_per_block);

    std::size_t length_of_str = buffer.length();
    std::size_t number_of_blocks = (length_of_str - 1)/chars_per_block + 1;
    _Internal::auto_array<digit> bdn(big_int::get_mem_for_data(number_of_blocks));
    std::size_t length_of_block = length_of_str % chars_per_block;
                            // for the first block
    if(!length_of_block)
        length_of_block = chars_per_block;

    std::size_t buffer_cur = 0;
    for(std::size_t j = number_of_blocks; j > 0; j--)
    {
        std::string block = buffer.substr(buffer_cur, length_of_block);
        std::istringstream stream_block(block);
        set_stream_radix(stream_block, radix);
        stream_block >> bdn.get()[j-1];
        buffer_cur += length_of_block;
        length_of_block = chars_per_block;  // for the last blocks
    }

    _Internal::auto_array<digit> data(big_int::get_mem_for_data(number_of_blocks));

    len = _Internal::do_bdn_to_big_int
    (
        bdn.get(),
        data.get(),
        number_of_blocks,
        bdn_radix
    );

    big_int::free_data(bdn.release());
    digit* d = big_int::realloc_data(data.release(), len);

    if(d)
        x.free_mem_and_alloc_number(sign, d, len);
    else
        x.free_mem_and_alloc_zero();

    if(brackets && !_Internal::read_literal(s, ")"))
        throw big_int::incorrect_string(std::string(1, s.peek()));

    s.clear();
    return s;

#endif
}


std::size_t big_int::length () const
{
    if(!number->len)
        return 0;
    std::size_t l = (number->len - 1) * _Internal::bits_per_digit;
    digit highest = number->data[number->len - 1];
    while(highest >>= 1)
        l++;
    return l + 1;
}


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4800)
#endif

bool big_int::operator[] (std::size_t k) const
{
    ARAGELI_ASSERT_0(k < length());
    return (number->data[k / _Internal::bits_per_digit] >>
        (k % _Internal::bits_per_digit)) % 2;
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif



big_int operator<< (const big_int& a, std::size_t n)
{
    std::size_t a_len = a.number->len;
    if(!n || !a_len)
        return a;
    std::size_t l = a.length() + n;
    std::size_t res_len = l/_Internal::bits_per_digit;
    if (l%_Internal::bits_per_digit)
        res_len++;
    std::size_t m = n/_Internal::bits_per_digit;
    n %= _Internal::bits_per_digit;
    digit t = 0;
    std::size_t k = _Internal::bits_per_digit - n;
    digit* res_data = big_int::get_mem_for_data(res_len);
    const digit* data = a.number->data;

    //  memset(res_data, 0, m * sizeof(digit));
    std::fill_n(res_data, m, digit(0));

    if(n)
    {
        for(std::size_t i = 0; i < a_len; i++)
        {
            res_data[m + i] = (data[i] << n) | t;
            t = data[i] >> k;
        }
        if(t)
            res_data[m + a_len] = t;
    }
    else
    {
        std::copy(data, data + a_len, res_data + m);
    }

    big_int res;
    res.free_mem_and_alloc_number(a.number->sign, res_data, res_len);
    return res;
}


big_int operator>> (const big_int& a, std::size_t n)
{
    std::size_t a_len = a.number->len;
    if (!n || !a_len)
        return a;
    std::size_t l = a.length();
    if (l <= n)
        return big_int();
    l -= n;
    std::size_t res_len = l/_Internal::bits_per_digit;
    if (l % _Internal::bits_per_digit)
        res_len++;
    std::size_t m = n / _Internal::bits_per_digit;
    n %= _Internal::bits_per_digit;
    std::size_t k = _Internal::bits_per_digit - n;
    digit t = 0;
    digit* res_data = big_int::get_mem_for_data(res_len);
    const digit* data = a.number->data;

    if(n)
    {
        if(a_len > res_len + m)
            t = data[a_len - 1] << k;
        for(std::size_t i = res_len; i > 0; i--)
        {
            res_data[i - 1] = (data[i + m - 1] >> n) | t;
            t = data[i + m - 1] << k;
        }
    }
    else
    {
        std::copy(data + m, data + m + res_len, res_data);
    }

    big_int res;
    res.free_mem_and_alloc_number(a.number->sign, res_data, res_len);
    return res;
}


big_int operator& (const big_int& a, const big_int& b)
{
    std::size_t reslen = std::min(a.number->len, b.number->len);
    if(reslen == 0)
        return big_int();
    digit *res = big_int::get_mem_for_data(reslen);

    // the current size of nonzero part of the number in res
    std::size_t lastnz = 0;

    for(std::size_t i = 0; i < reslen; ++i)
    {
        res[i] = a.number->data[i] & b.number->data[i];
        if(res[i])
            lastnz = i+1;
    }

    if(lastnz)
    {
        big_int resbi;
        resbi.free_mem_and_alloc_number(1, res, lastnz);
        return resbi;
    }
    else
    {
        big_int::free_data(res);
        return big_int();
    }
}

big_int operator| (const big_int& a, const big_int& b)
{
    std::size_t reslen = std::max(a.number->len, b.number->len);
    std::size_t resmin = std::min(a.number->len, b.number->len);
    if(reslen == 0)
        return big_int();
    digit *res = big_int::get_mem_for_data(reslen);

    // the current size of nonzero part of the number in res
    std::size_t lastnz = 0;

    for(std::size_t i = 0; i < resmin; ++i)
    {
        res[i] = a.number->data[i] | b.number->data[i];
        if(res[i])
            lastnz = i+1;
    }
    
    big_int tmp = a;
    if (b.number->len == reslen)
    {
	tmp = b;
    }
    
    for(std::size_t i = resmin; i < reslen; ++i)
    {
        res[i] = tmp.number->data[i];
        if(res[i])
            lastnz = i+1;
    }

    if(lastnz)
    {
        big_int resbi;
        resbi.free_mem_and_alloc_number(1, res, lastnz);
        return resbi;
    }
    else
    {
        big_int::free_data(res);
        return big_int();
    }
}

big_int operator^ (const big_int& a, const big_int& b)
{
    std::size_t reslen = std::max(a.number->len, b.number->len);
    std::size_t resmin = std::min(a.number->len, b.number->len);
    if(reslen == 0)
        return big_int();
    digit *res = big_int::get_mem_for_data(reslen);

    // the current size of nonzero part of the number in res
    std::size_t lastnz = 0;

    for(std::size_t i = 0; i < resmin; ++i)
    {
        res[i] = a.number->data[i] ^ b.number->data[i];
        if(res[i])
            lastnz = i+1;
    }
    
    big_int tmp = a;
    if (b.number->len == reslen)
    {
	tmp = b;
    }
    
    for(std::size_t i = resmin; i < reslen; ++i)
    {
        res[i] = tmp.number->data[i];
        if(res[i])
            lastnz = i+1;
    }

    if(lastnz)
    {
        big_int resbi;
        resbi.free_mem_and_alloc_number(1, res, lastnz);
        return resbi;
    }
    else
    {
        big_int::free_data(res);
        return big_int();
    }
}

std::size_t ndigits (big_int x, std::size_t r)
{
    ARAGELI_ASSERT_0(r >= 2);

    if(is_negative(x))
        opposite(&x);   // Is it really necessary?

    std::size_t res = 0;
    big_int br = r; // as we do not have optimized operations with the built-ins
    while(!is_null(x))
    {
        x /= br;
        ++res;
    }

    return res;
}


std::size_t io_binary<big_int>::calc (const big_int& x)
{
    if(x.number->sign)
        return
            sizeof(int) +
            sizeof(std::size_t) +
            x.number->len * sizeof(big_int::digit);
    else
        return sizeof(int);
}


}


#endif
