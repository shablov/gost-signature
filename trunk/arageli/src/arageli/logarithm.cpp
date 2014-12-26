/*****************************************************************************

    logarithm.cpp

    This file is a part of the Arageli library.

    Copyright (C) 2005--2006 Alexander Pshenichnikov
    Copyright (C) 2005--2006 Nikolay Santalov

    23.12.2005

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

#include "config.hpp"

#include <cstddef>
#include <cmath>

#include "big_int.hpp"
#include "big_const.hpp"
#include "powerest.hpp"

#define SPARE 0xA    // WARNING! Macro!!!

namespace Arageli
{

// Returns  [de*log2_10]
big_int entier ( const big_int &de )
{
    if ( !de.length() )
        return 0;

    std::size_t counter = 0;
    std::size_t factor = 1;
    std::size_t spare_bits;
    std::size_t len = de.length ();
    big_int L;
    int fl = 1;

    while ( fl )
    {
        L = log2_10_u ( len + SPARE * factor  ) * de;
        //if ( L >> len + SPARE * factor + 1 == ( L + de << 1 ) >> len + SPARE * factor + 1 );
        //    break;
        spare_bits = SPARE * factor;
        while ( counter < spare_bits )
        {
            if ( ! L[ counter + len ] )
            {
                fl = 0;
                break;
            }
            counter++;
        }
        if ( counter == spare_bits )
        {
            factor++;
            counter = 0;
        }
    }
    if ( de > 0 )
        return ( L >> ( len + SPARE * factor + 1 ) )  + 3 * de;
    return ( L >> ( len + SPARE * factor + 1 ) )  + 3 * de - 1;
}
//Returns 2^{de*log2_10} with kbits precision
big_int frac ( const big_int &de, std::size_t kbits)
{
    std::size_t n = ( kbits - 1 ) / 3 + 2;
    kbits += n + 4  ;
    big_int L;
    std::size_t len = de.length ( );
    //std::size_t spare_bits;

    do
    {
        L = log2_10_u ( len + kbits ) * de;
        L = - (( L >> ( len + kbits + 1 ) ) << ( len + kbits + 1)) + L;
        len += SPARE;
    }while
    (
        (
            (L + pow2<big_int> ( kbits + 1 )) >>
            (len - SPARE + kbits + 1)
        ) == 1
    );

    if ( L < 0 )
        L = L + pow2<big_int> ( len + kbits + 1 - SPARE ) - 2;
    L = L >> len - SPARE + 4;

    //assumption that k>=56
    kbits -= 3 ;
    std::size_t counter = 1;
    big_int s;
    big_int a = pow2<big_int> (  kbits  );
    len = (kbits  << 1) + 1;
    L = L * ln2_u ( kbits );
    //cout << L << endl;
    //cout << len << endl;
    s = s + a;
    while ( counter < n )
    {
        a = ( ( a * L ) / counter ) >> len;//Teylor series
        s = s + a;
        counter++;
    }
    return ( s >> n + 1 ) + 1;
}

/// Returns  lg (b)
std::size_t lg10 ( const big_int &b )
{
    std::size_t res = 0;
    big_int temp ( b );

    while ( ( temp = temp / 10 ) != 0 )
        res++;

    return res;
}
/// Returns  lg (b)
std::size_t lg10 ( std::size_t b )
{
    std::size_t res = 0;
    std::size_t temp ( b );

    while ( ( temp /= 10 ) != 0 )
        res++;

    return res;
}

/// Returns  10^pow
big_int power_of_ten ( std::size_t pow )
{
    big_int res ( 1 );

    while ( pow-- )
        res = res * 10;
    return res;
}

//Returns [be / log2_10] = [be *lg(2)]
big_int entier_1 ( const big_int &be )
{
    if ( be == 0 )
        return 0;

    int s = 0;
    std::size_t kbits ( be.length () + SPARE );
    big_int L ( be );
    if ( L < 0 )
    {
        L = -L;
        s = - 1;
    }
    big_int three ( 3 );
    do
    {
        L = ( L << ( kbits * 2 + 2) ) / (( three << kbits )+ log2_10_o ( kbits - 1 )) ;
        kbits += SPARE;
    } while ( ( ( L + 2 ) >>  kbits + 1 )  != ( L >>  kbits + 1 ) );

    if ( s )
        return - ( L >>  kbits - SPARE + 2 ) - 1;
    return L >> kbits - SPARE + 2;
}

/// Returns 10 ^ {de/log2_10} with kbits - 1  precision
big_int frac_1 ( const big_int &de, std::size_t kbits)
{
    if ( de == 0 )
        return pow2<big_int> ( kbits ) ;

    std::size_t prec = kbits + 8;
    std::size_t l = de.length() + prec + 1;
    big_int L;

    do
    {
        L = de * lg_2_u( l );       // L ~ de * lg2 with prec + 1 precision de * lg2  = L / 2 ^ (l + 1)
        L = L - ( (L >> l + 1) << l + 1); //{ de * lg2 } ~ L / 2 ^ ( l + 1 )
        if ( L < 0 ) L = L + pow2<big_int> ( l + 1 ) - 2 ; //{ -x } = 1 - { x }
        l += SPARE;
    } while ( (L +  pow2<big_int> ( de.length() + 1 ) ).length() > l);
    L = L >> ( l - SPARE - prec );
    /* now L = { de * lg ( 2 ) } * 2 ^ ( prec + 1 ) with prec precision */

    //( ln1_25_u  ( prec ) + 3 * ln2_u ( prec ) )  ln10 with prec - 2 precision  ( ln10 = L / 2 ^ prec )
    L = L * ( ln1_25_u ( prec ) + 3 * ln2_u ( prec ) ); // L = { de * lg ( 2 )} * ln10 with prec - 3 precision { de * lg ( 2 )} * ln10 = L / 2 ^ ( 2 * prec + 2 )

    /*
        exp ( underflow )
    */

    std::size_t n = ( kbits + 1 ) / 3 + 2;
    n = (n < 20 ) ? 20 : n;
    std::size_t pos = log2floor(n) + 2;//log2( n ) + 2
    std::size_t counter = 1;
    big_int s;
    big_int a = pow2<big_int> ( kbits +  pos );
    std::size_t len = 2 * prec + 2;

    s = s + a;
    while ( counter < n )
    {
        a = ( (a * L) / counter) >> len ;//Teylor series
        s = s + a;
        counter++;
    }
    // now s ~= 10 ^ {de/log2_10} * 2 ^ kbits . It means that value  10 ^ {de/log2_10} * 2 ^ kbits is between   s and s + 2
    return  ( s >> pos ) + 1; //see do_dec_convert;
}

/*
*this function convert decimal presentation : dm * 10 ^ de
*to binary presentation:  bm * 2 ^ dm
*it calculates bm and be using dm and de.
*non-integer number bm is rounded to integer in range [ 2^p, 2^{ p + 1 } )
*/
void do_bin_convert ( const big_int &dm, const big_int &de, std::size_t p, big_int &bm, big_int &be )
{
    if ( dm == 0 )
    {
        bm = be = 0;
        return;
    }
    //cout << "de = " << entier ( de ) << endl;
    be = entier ( de ) + log2 ( dm ) - p;
    //cout << be << endl;
    //cout << " de = " << de << "   p = " << p;
    bm = frac ( de, p );
    //cout << "frac() = " << bm << endl;
    bm = bm * dm / pow2<big_int> ( log2(dm));
    //cout << bm << endl;
    //cout << p << endl;
    if ( bm.length () - p )
    {
        be = be + 1;
        bm = bm >> 1;
    }
}

/*
*this function convert binary presentation : bm * digit ^ be
*to decimal presentation:  dm * 10 ^ de
*it calculates dm and de using bm and be.
*non-integer number dm is rounded to integer in range [ 10^p, 10^{ p + 1 } )
*/
std::size_t do_dec_convert ( big_int &dm, big_int &de, std::size_t p, const big_int & bm, const big_int & be )
{
    //cout << "be = " << be << "bm = "<< bm << endl;
    //cout << entier_1 ( be  ) << endl;
    //cout << lg10 ( bm ) << endl;
    de = entier_1 ( be  ) + lg10 ( bm ) - p;
    //cout << "de =" << de << endl;
    //cout << "be = " << be << "\tp = " << p <<endl;
    std::size_t bp = std::size_t (( double )( p + 1 ) * ( log ( 10.0l ) / log ( 2.0l ) ) + 4.0);
    dm = frac_1 ( be, bp );
    //cout << p << endl;
    //cout << "dm = " << dm << "  bp = "<< bp << endl;
    //cout << "dm =" << dm << endl;

    //dm =  (( bm * dm ) >> bp) * power_of_ten ( p + 1 ) / power_of_ten ( lg10 (bm) ) ;
    dm =  ( bm * dm * power_of_ten ( p + 1 ) >> bp )/ power_of_ten ( lg10 (bm) ) ;
    //cout << " dm = " << dm << endl;
    dm = ( dm + 5 * dm.sign () ) / 10;

    //cout << "dm = "<< dm <<"   lg (dm) =" << lg10 ( dm ) << "   p = "<< p << endl;
    if ( ! (lg10 ( dm ) - p - 1)  )
    {
        de = de + 1;
        dm = (dm + 5 * dm.sign ()) / 10;

    }
    if ( ! ( lg10 ( dm ) - p + 1 )    )
    {
        de = de - 1;
        dm = dm * 10;
    }
    //cout << "dm = "<< dm <<"   lg (dm) =" << lg10 ( dm ) << "   p = "<< p << endl;
    return lg10 ( de );
}

}

//#endif
