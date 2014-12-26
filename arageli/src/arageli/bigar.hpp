/*****************************************************************************

    bigar.hpp

    This file is a part of the Arageli library.

    WARNING. This is internal header of Arageli library.
    Do not use it directly in you code other then Arageli code.

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
    \file bigar.hpp
    \brief Dealing with chains of big digits as a big integer numbers.

    A lot of internal constants. Type for the big digit is being choosen.
    The four basic operations on big digits sequences (big integers).

    The implementation of the four basic arithmetic operations based on
    an assumption that there are doubledigit type and extendeddigit type
    that have double size of the digit and size greater by 1 bit then
    the digit correspondingly.
*/

#ifndef __ARAGELI_bigar_hpp__
#define __ARAGELI_bigar_hpp__

#include "config.hpp"

#include <cstddef>
#include <cstdlib>
#include <limits>

#include "std_import.hpp"


namespace Arageli { namespace _Internal
{

    // WARNING for the whole file!
    // Sizes, types and masks can faile on some platforms.
    // This is a temporary implementation.

#if defined(_MSC_VER) && defined (_WIN32) || 0 && defined(__BORLANDC__) && defined(__WIN32__)
    // The following works well for Ms C (Win32)
    // Also you can use these for Borland C 5.5 compiler.
    // but surprisingly the code will be more slower than for settings by default (see below)
    // For Microsoft Visual C++ use /O2 optimizing option
    typedef unsigned long digit;
    typedef unsigned __int64 doubledigit;
    typedef unsigned __int64 extendeddigit;
    typedef unsigned short int bit;
    const digit max_digit = 0xFFFFFFFF;
    const extendeddigit BASE = 0x100000000ull;
    const int bits_per_digit = 32;
#elif defined ARAGELI_LONG_LONG_SUPPORT
    typedef unsigned int digit;
    typedef unsigned long long doubledigit;
    typedef unsigned long long extendeddigit;
    typedef unsigned short int bit;
    const digit max_digit = 0xFFFFFFFF;
    const extendeddigit BASE = 0x100000000ull;
    const int bits_per_digit = 32;
#else
    // For others compilers we can also TRY this
    // These setting works well for a lot of compilers (Microsoft, Borland, gcc)
    // For Borlan C++ use -O2 optimizing option
    // For gcc use /O3 optimizing option
    typedef unsigned short digit;
    typedef unsigned long doubledigit;
    typedef unsigned long extendeddigit;
    typedef unsigned short bit;
    const digit max_digit = 0xFFFF;
    const extendeddigit BASE = 0x10000l;
    const int bits_per_digit = 16;
#endif


std::size_t do_big_int_to_bdn
(
    digit* a,
    digit* b,
    std::size_t n,
    digit bdn_radix
);

std::size_t do_bdn_to_big_int
(
    digit* a,
    digit* b,
    std::size_t n,
    digit bdn_radix
);

std::size_t do_add
(
    digit* p1,
    const digit* p2,
    std::size_t m,
    std::size_t n
);

int do_sub
(
    digit* p1,
    const digit* p2,
    std::size_t m,
    std::size_t n
);

std::size_t do_optimize
(
    const digit* a,
    std::size_t n
);

std::size_t do_mult_classic
(
    const digit* u,
    const digit* v,
    digit* w,
    std::size_t m,
    std::size_t n
);

std::size_t do_mult
(
    const digit* u,
    const digit* v,
    digit* w,
    std::size_t m,
    std::size_t n
);

digit do_divide_by_digit
(
    const digit* a,
    digit* p,
    std::size_t n,
    digit d
);

std::size_t do_divide
(
    digit* u,
    digit* v,
    digit* q,
    std::size_t m,
    std::size_t n
);

}

#if 0

namespace Bigar
{

template <typename D> struct doubledigit;
template <typename D> struct extendeddigit;
template <typename D> struct bits_per_digit;
template <typename D> struct max_digit;
template <typename D> struct base;

// temporary definitions
template <> struct doubledigit<unsigned long>
{
    typedef unsigned __int64 type;
};

template <> struct extendeddigit<unsigned long>
{
    typedef unsigned __int64 type;
};

template <> struct bits_per_digit<unsigned long>
{
    static const unsigned int value =
        std::numeric_limits<unsigned long>::digits;
};

template <> struct max_digit<unsigned long>
{
    static const unsigned long value;
};

const unsigned long max_digit<unsigned long>::value =
    std::numeric_limits<unsigned long>::max();

template <> struct base<unsigned long>
{
private:
    typedef extendeddigit<unsigned long>::type exdig;
public:
    static const exdig value
        /* = static_cast<exdig>(max_digit<unsigned long>::value) + 1*/;
};

const extendeddigit<unsigned long>::type
base<unsigned long>::value =
    static_cast<exdig>(max_digit<unsigned long>::value) + 1;

typedef unsigned char bit;

template <typename D> struct digit_info
{
    typedef typename Bigar::doubledigit<D>::type doubledigit;
    typedef typename Bigar::extendeddigit<D>::type extendeddigit;
    static const unsigned int bits_per_digit = Bigar::bits_per_digit<D>::value;
    static const D max_digit = Bigar::max_digit<D>::value;
    static const extendeddigit base = Bigar::base<D>::value;
};


template <typename In, typename Out>
typename std::iterator_traits<Out>::difference_type
addseq
(
    Out res,
    In in,
    typename std::iterator_traits<Out>::difference_type size_res,
    typename std::iterator_traits<Out>::difference_type size_in
);

template <typename In, typename Out>
typename std::iterator_traits<Out>::difference_type
addseq
(
    Out res,
    In in,
    typename std::iterator_traits<Out>::difference_type size_res,
    typename std::iterator_traits<Out>::difference_type size_in
)
{
    ARAGELI_ASSERT_1(size_res >= size_in);

    typedef std::iterator_traits<Out> Out_traits;
    typedef typename Out_traits::difference_type pos_type;
    typedef digit_info<typename Out_traits::value_type> digit_info;

    // actually, length of p1 must be m+1 - reserved for carry
    // do_add returns amount of main digits (m without carry or m+1 with carry)

    ARAGELI_ASSERT_1(m >= n);

    bit carry = 0;

    // 1. loop for 0 to n-1 - add digits of p2 to p1

    for(pos_type i = 0; i < n; i++)
    {
        typename digit_info::doubledigit tmp =
            static_cast<typename digit_info::doubledigit>(p1[i]) + p2[i] + carry;

        res[i] = digit(tmp % digit_info::base);
        carry = digit(tmp / digit_info::base);
    }

    // 2. loop for add carry

    for(pos_type i = n; i < m; i++)
    {
        doubledigit tmp = doubledigit(p1[i]) + carry;
        p1[i] = digit(tmp % BASE);
        carry = digit(tmp / BASE);

        if(carry == 0)
            break;
    }

    if(carry)
    {
        p1[m] = 1;
        return m + 1;
    }
    else
        return m;
}

}

#endif

}

#endif
