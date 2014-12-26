/*****************************************************************************

    big_const.cpp -- See big_const.hpp file.

    This file is a part of the Arageli library.

    Copyright (C) 2005--2006 Alexander Pshenichnikov
    Copyright (C) 2005--2006 Nikolay Santalov

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
    \file big_const.cpp
    \brief The big_const.hpp file stuff implementation.
*/

#include "config.hpp"

#include <cstddef>
#include <cmath>

#include "big_const.hpp"


#pragma warning(disable : 4244)     // here: conversion from long double to size_t,
                                    // possible loss of data


namespace Arageli
{

/***************************** ln(1.25)****************************************/
big_int ln1_25 ( std::size_t kbits ) //calculate ln(1.25)
{
    std::size_t n = (int) (kbits - 4) / 4 + 1;
    if ( n <= 1)
        n++;
    std::size_t pos = log2floor(n) + 3;//log2( 2n ) + 2

    big_int s(0);
    big_int a;
    unsigned counter = 1;

    kbits +=1 ;
    n <<= 1;
    while (counter <= n) //Taylor series
    {
        a = pow2<big_int>(kbits + pos - (counter<<1))/counter;//(1/4^n)*(1/n)
        counter++;
        s = s + a;
        a = pow2<big_int>(kbits + pos - (counter<<1))/counter;//(1/4^n)*(1/n)
        counter++;
        s = s - a - 1;
    }
    return (s >> pos ) + 1;//delete excess bits
}

big_int ln1_25_o(std::size_t kbits)//calculate ln(1.25) with overflow
{
    return ln1_25 ( kbits ) + 1;
}

big_int ln1_25_u ( std::size_t kbits )//calculate ln(1.25) with underflow
{
    return ln1_25 ( kbits ) - 1;
}

/***********************************ln2*****************************************/
big_int ln2 ( std::size_t kbits )//calculate ln(2)
{
    std::size_t n = ( kbits - 1 )/ 3 + 1;
    std::size_t pos = log2floor(n) + 2;//log2( n ) + 2
    big_int s(0);
    big_int a,b;
    unsigned counter = 1;

    kbits++;
    a = pow2<big_int>( kbits + pos ) / 3;
    s = s + a;
    while ( counter < n ) //Taylor series
    {
        //b = pow2<big_int>(kbits + n) / 81; it is good to verify that always a/(b*c)==(a/b)/c
        a = (a * ((counter << 1) - 1))/(((counter << 1) + 1) * 9);
        s = s + a;
        counter++;
    }

    return (s >> pos - 1) + 1 ;//don't forget to multiply by two
}

big_int ln2_u ( std::size_t kbits )//calculate ln(2) with underflow
{
    return ln2 ( kbits ) - 1;
}

big_int ln2_o(std::size_t kbits)//calculate ln(1.25) with overflow
{
    return ln2 ( kbits ) + 1;
}


/******************************log2(10)*****************************************/
big_int log2_10 ( std::size_t nbits ) //calculate log2_10 with nbits significant bits
{
    big_int numerator   = ln1_25_u ( nbits + 3 );
    big_int denumerator = ln2_o ( nbits + 3 );
    return ( numerator << ( nbits + 1 ))/ denumerator + 1;
}
big_int log2_10_o ( std::size_t nbits ) //calculate log2_10 with nbits significant bits with overflow
{
    return log2_10 ( nbits ) + 1;
}

big_int log2_10_u ( std::size_t nbits ) //calculate log2_10 with nbits significant bits with underflow
{
    return log2_10 ( nbits ) + 1;
}

/******************************lg(2)*****************************************/

big_int lg_2_o ( std::size_t nbits ) //calculate lg2 with nbits significant bits with overflow
{
    return lg_2 ( nbits ) + 1;
}

big_int lg_2_u ( std::size_t nbits ) //calculate lg2 with nbits significant bits with underflow
{
    return lg_2 ( nbits ) - 1;
}

big_int lg_2 ( std::size_t nbits ) //calculate lg2 with nbits significant bits
{
    big_int numerator   = ln2_u ( nbits + 3 );
    big_int denumerator = 3 * ( numerator + 2 ) + ln1_25_o ( nbits + 3 );
    return ( numerator << ( nbits + 1 ))/ denumerator + 1;
}


/*********************************e^x*********************************************/

// calculate exp ( x ) with kbits significant bits with underflow
big_int exp_u (big_int arg, std::size_t kbits, int factor)
{
    return exp ( arg, kbits, factor) - 1;
}


// calculate exp ( x ) with k_bits significant bits with overflow
big_int exp_o (big_int arg, std::size_t kbits, int factor)
{
    return exp ( arg,  kbits, factor) + 1;
}


// calculate exp ( x ) with k_bits significant bits with overflow
big_int exp (big_int arg, std::size_t kbits, int factor)
{
    // there 0 < x < 1 x =  arg/2^(arg.length() + factor )

    std::size_t n = int ( kbits - 2 ) / 3 + 2;
    if ( n <= 19 ) n = 20;
    std::size_t pos = log2floor(n) + 2;//log2( n ) + 2
    std::size_t counter = 1;
    big_int s;

    kbits++;
    big_int a = pow2<big_int> ( kbits + pos );
    std::size_t len = arg.length ( ) + factor;

    s = s + a;
    while ( counter < n )
    {
        a = ( (a * arg) / counter) >> len ;//Taylor series
        s = s + a;
        counter++;
    }
    return ( s >> pos ) + 1  ;
}


/*****************************2^x***********************************************/

big_int exp_2_u ( big_int arg, std::size_t kbits, int factor)
{
    return exp_2 ( arg,  kbits, factor) -1;
}

big_int exp_2_o ( big_int arg, std::size_t kbits, int factor)
{
    return exp_2 ( arg, kbits, factor) + 1;
}

big_int exp_2 ( big_int arg, std::size_t kbits, int factor)
{
    std::size_t n = ( kbits - 1 ) / 3 + 2;
    if ( n <= 19 ) n = 20;
    kbits++;
    std::size_t counter = 1;
    big_int s;
    big_int a = pow2<big_int> ( kbits + n);
    std::size_t len = kbits + n + arg.length ( ) + factor;
    arg = arg * ln2_u ( kbits + n );

    s = s + a;
    while ( counter < n )
    {
        a = ( ( a * arg ) / counter ) >> len ;//Taylor series
        s = s + a;
        counter++;
    }
    return (s >> n + 1)  +  1;
}


/***************************e ****************************************************/

big_int e_u (std::size_t kbits  )//e number with underflow
{
    return e ( kbits ) - 1;
}

big_int e_o (std::size_t kbits  )//e number with overflow
{
    return e ( kbits ) + 1;
}


big_int e ( std::size_t kbits )//e number with overflow
{
    std::size_t n = int ( kbits - 2 ) / 3 + 1;
    if ( n <= 19 ) n = 20;

    std::size_t pos = log2floor(n) + 2;//log2( n ) + 2
    std::size_t counter = 1;
    big_int s;

    kbits++;
    big_int a = pow2<big_int> ( kbits + pos );

    s = s + a;
    while ( counter < n && a != 0 )
    {
        a = ( a / counter);//Taylor series
        s = s + a;
        counter++;
    }
    return ( s >> pos ) + 1;
}
/**********************************************************************************/

}
