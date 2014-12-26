/*****************************************************************************

    big_float.cpp

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
    \file big_float.cpp
    \brief The big_float.hpp file stuff implementation.

    Big Float Numbers implementation.
*/


#include "config.hpp"

#define CHECK_PREC(p)    \
    ARAGELI_ASSERT_0(p >= PREC_MIN && p <= PREC_MAX && "Precision is out of range")

#define CHECK_MODE(m)    \
    ARAGELI_ASSERT_0(m >= EXACT && m <= TO_INF && "Try to use incorrect rounding mode")

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_BIG_FLOAT)

#include <cmath>
#include "big_float.hpp"
#include "_utility.hpp"


namespace Arageli
{

template <typename T>
void big_float::from_native_float(const T &f)
{
    typedef std::numeric_limits<T> Nl;

    //using namespace _Internal;

    ARAGELI_ASSERT_1(Nl::is_specialized);
    ARAGELI_ASSERT_1(!Nl::is_integer);
    ARAGELI_ASSERT_0(!(Nl::has_infinity && f == Nl::infinity()));
    ARAGELI_ASSERT_0(!(Nl::has_quiet_NaN && f == Nl::quiet_NaN()));
    ARAGELI_ASSERT_0(!(Nl::has_signaling_NaN && f == Nl::signaling_NaN()));
    ARAGELI_ASSERT_0(!(Nl::has_denorm && f == Nl::denorm_min()));

    if ( !f )
    {
        big_float ();
        return;
    }

    int digits_need = (Nl::digits - 1) / _Internal::bits_per_digit;
    _Internal::digit *man;
    _Internal::digit *p = (_Internal::digit *)&f;

    man = new _Internal::digit [ digits_need + 1 ];
    ARAGELI_ASSERT_0(man && "the heap overflow");

    for ( int i = 0; i < digits_need; i++,p++ )
        man [ i ] = *p;

    int bits_remain = (Nl::digits - 1) % _Internal::bits_per_digit;

    man [ digits_need ] =
        (
            (*p << (_Internal::bits_per_digit - bits_remain)) >>
            (_Internal::bits_per_digit - bits_remain)
        ) | (1U << bits_remain);

    prec = Nl::digits;
    mode = big_float::global_mode;

    s.free_mem_and_alloc_number( f > 0 ? 1 : -1, man, digits_need + 1 );

    int ex;
    frexp (f, &ex);
    e = ex - Nl::digits;
}

template <typename T>
void big_float::from_native_int (const T& i)
{
    s = big_int(i);
    prec =
        (big_float::global_prec < std::numeric_limits<T>::digits) ?
        std::numeric_limits<T>::digits :
        big_float::global_prec;

    CHECK_PREC(prec)
    mode = big_float::global_mode;
}

template <typename T>
T big_float::to_native_int () const
{
    return
        s.sign() > 0 ?
        ifloor(*this).to_native_int<int>() :
        iceil (*this).to_native_int<int>();
}

template <typename T>
T big_float::to_native_float () const
{
    typedef std::numeric_limits<T> Nl;

    ARAGELI_ASSERT_1(Nl::is_specialized);
    ARAGELI_ASSERT_1(!Nl::is_integer);
    ARAGELI_ASSERT_0(Nl::has_infinity);

    big_int temp(s);
    int shift = temp.length() - Nl::digits;
    if (shift > 0 )
        temp >>= shift;

    /*!! */
    int tlen = temp.length();
    int digits_need = tlen / _Internal::bits_per_digit + 1;
    _Internal::digit *p = new _Internal::digit [ digits_need ];
    ARAGELI_ASSERT_0(p && "the heap overflow");

    std::memmove( p, temp.number->data, digits_need * sizeof (_Internal::digit));
    p [digits_need - 1] /*&= ~(_Internal::digit(1) << tlen %_Internal::bits_per_digit - 1)*/;
    T ret = s.sign() * ldexp (*((T*)p), e + (shift + Nl::max_exponent + Nl::digits - 3));
    delete [] p;
    return ret;
}

}//namespace Arageli

#else

#include <sstream>

#include "powerest.hpp"
#include "logarithm.hpp"
#include "big_float.hpp"
#include <cmath>


namespace Arageli
{


// here: conversion from long double to size_t,
// possible loss of data
#pragma warning(disable : 4244)


//a-la bigarith
void big_float_warning ( const char *s )
{
    std::cerr << "Big float warning: " << s << "\n";
}

void big_float_error ( const char *s )
{
    std::cerr << "Big float error: " << s << "\n";
}

void big_float_fatal_error(const char *s)
{
    std::cerr << "Big float fatal error: \n " << s << "\n";
    exit(1);
}

/*
* Constructors
*/
//default constructor
big_float::big_float(void) :
    prec( big_float::global_prec ),
    mode ( big_float::global_mode )
{}

//Sets prec to prec and mode to mode
big_float::big_float ( long prec, long mode ) :
    prec ( prec ),
    mode ( mode )
{}

//copy constructor
big_float::big_float ( const big_float &b ) :
    e(b.e),
    s(b.s),
    prec(b.prec),
    mode(b.mode)
{}

//constructor (from string to big_float)
big_float::big_float ( const char *str, long p ) :
    prec (p),
    mode ( big_float::global_mode )
{
    CHECK_PREC(prec)
    std::istringstream s ( str );
    s >> *this;
}

//constructor (from string to big_float)
big_float::big_float ( const char *str ) :
    mode ( big_float::global_mode )
{
    //first evaluate significant digits (mantissa) and set appropriate precision
    size_t l;
    size_t space = 0;

    while ( isspace(str[space]) )
        space++;
    if ( str[space] == '+' || str [space] == '-' )
        space++;

    l = space;
    while ( isdigit (str[l]) )
        l++;
    if ( str [l] == '.' )
    {
        while ( isdigit (str[++l]) );
        l--;
    }
    l = l - space;
    if ( !l )
        *this = big_float(); //zero
    else
    {
        prec = (long ) ((long double) l * log (10.0l)/log(2.0l) + 1.0);
        if ( prec < big_float::global_prec )
            prec = big_float::global_prec;
        *this = big_float (str, prec);
    }
}

//constructor  ( from two big_int to big_float )
big_float::big_float ( const big_int &s, const big_int &e, long mode ) :
    e ( e ),
    s (s),
    mode ( mode )
{
    ARAGELI_ASSERT_1(is_converted_to<long>(s.length()));
    prec = static_cast<long>(s.length());
    ARAGELI_ASSERT_0( prec >= PREC_MIN && prec <= PREC_MAX )
}

//constructor from big_int
big_float::big_float (const big_int& i)
{
    s = big_int(i);
    ARAGELI_ASSERT_1(is_converted_to<long>(s.length()));
    prec = static_cast<long>(i.length());
    CHECK_PREC(prec)
    mode = big_float::global_mode;
}

//returns |n| mod m
unsigned fracsion ( int n, unsigned m )
{
    if ( n < 0 )
        return (m-((-n)%m))%m;
    else
        return (n%m);
}

/*
//swap  a  and b
inline void swap( big_int &a, big_int &b )
{
    a = a + b;
    b = a - b;
    a = a - b;
}

// Unused now
inline long get_number_of_digits ( big_int b )
{
    return (b.length() - 1 ) / _Internal::bits_per_digit + 1;
}
*/

big_float & big_float::operator = ( const big_int &b )
{
    s = b;
    normalize_1 ( prec, mode );
    return *this;
}

// Normalization for big_float number
void big_float::normalize_1 ( long prec, long mode )
{
    this->prec = prec;
    this->mode = mode;
    if ( !s.sign() ) //significant s = 0, then exponenta = 0
    {
        e = 0;
        return;
    }

    int s_len = s.length() - prec;

    if ( mode == EXACT && s.length() <= PREC_MAX ||  (!s_len)  )
        return;// do nothing - mantissa has apropriate length
    if ( s_len < 0  )
    {
        s = s << -s_len;
        e = e + s_len;
        return;
    }//write zero at the end and return
    ARAGELI_ASSERT_0
    (
        mode!= EXACT &&
        "Impossibly to perfom operarion with EXACT rounding mode ( length too long )"
    );

    /*
    if ( mode == EXACT )
    {
        mode = TO_NEAREST; // s.lenght > PREC_MAX
        big_float_error ( "can't perfom operarion with EXACT rounding mode" );
    }
    */

    int is_digit = 0; // needs then rounds to +-infinity
    is_digit = ( (s >> s_len - 1) << s_len - 1 != s );
    s = s >> s_len - 1;
    e = e + s_len - 1;

    switch (mode)
    {
        case TO_NEAREST:
                        s = ( s + s.sign() ) >> 1;
                        e = e + 1;
                        if ( s.length() > prec )
                        {
                            s = s >> 1;
                            e = e + 1;
                        }
                        break;
        case TO_ZERO:
                        s = s >> 1;
                        e = e + 1;
                        break;
        case TO_P_INF:
                        if ( s.sign() < 0 )
                            s = s >> 1;
                        else
                        {
                            if ( is_digit )
                                s = ( s >> 1) + 1;
                            else
                                s = s + 1 >> 1;
                        }
                        e = e + 1;

                        if ( s.length() > prec )
                        {
                            s = s >> 1;
                            e = e + 1;
                        }
                        break;
        case TO_M_INF:
                        if ( s.sign() > 0  )
                            s = s >> 1;
                        else
                        {
                            if ( is_digit )
                                s = ( s >> 1) - 1;
                            else
                                s = s - 1 >> 1;
                        }
                        e = e + 1;

                        if ( s.length() > prec )
                        {
                            s = s >> 1;
                            e = e + 1;
                        }
                        break;
        case TO_INF:
                        //s=s/(max_digit+1);
                        if ( is_digit )
                            s = ( s >> 1 ) + s.sign();
                        else
                        {
                            if ( s [ 0 ] )
                                s = ( s >> 1 ) + s.sign();
                            else
                                s = s >> 1;
                        }
                        e = e + 1;

                        if ( s.length() > prec )
                        {
                            s = s >> 1;
                            e = e + 1;
                        }
                        break;
        default:
                        break;

    }//switch(mode)
    return;
}


//destructor
big_float::~big_float()
{}

/*
*  compare operations
*/

int big_float::sign () const
{
    return s.sign();
}

int cmp ( const big_float & a, const big_float & b )
{
    big_float temp = a - b;
    return temp.sign();
}

bool operator == ( const big_float & a, const big_float & b )
{
    return  cmp ( a, b ) == 0;
}

bool operator != ( const big_float & a, const big_float & b )
{
    return cmp ( a, b ) != 0;
}

bool operator > (const big_float & a, const big_float & b)
{
    return cmp ( a, b ) == 1;
}

bool operator >= (const big_float & a, const big_float & b)
{
    return cmp ( a, b ) != -1;
}

bool operator < (const big_float & a, const big_float & b)
{
    return cmp ( a, b ) == -1;
}

bool operator <= (const big_float & a, const big_float & b)
{
    return cmp ( a, b ) != 1;
}

//set bits precision
void big_float::set_precision ( long p )
{
    CHECK_PREC(p)
    prec = p;
}

//set rounding mode
void big_float::set_round_mode ( long m )
{
    CHECK_MODE(m)
    mode = m;
}

//set default global bits precision
void big_float::set_global_precision ( long p )
{
    CHECK_PREC(p)
    global_prec = p;
}

//set global rounding mode
void big_float::set_global_round_mode ( int m )
{
    CHECK_MODE(m)
    global_mode = m;
}

//unary plus
big_float operator + ( const big_float &a )
{
    return a;
}

//unary minus
big_float operator - ( const big_float &a )
{
    big_float b(-a.s, a.e, a.mode);
    return b;
}

//operator +
big_float operator + (const big_float & b, const big_float & c)
{
    ARAGELI_ASSERT_0
    (
        !(b.mode-c.mode) &&
        "Try to perfom adding two big_float numbers with different "
            "rounding modes"
    );//or may be use global_mode

    return add
    (
        b,
        c,
        b.prec < c.prec? b.prec : c.prec,
        b.mode
    );
}

//operator -
big_float operator - (const big_float & b, const big_float & c)
{
    ARAGELI_ASSERT_0
    (
        !(b.mode-c.mode) &&
        "Try to subtraction two big_float numbers with different "
            "rounding modes"
    );

    return
        sub
        (
            b,
            c,
            b.prec < c.prec ? b.prec : c.prec,
            b.mode
        );
}

//operator *
big_float operator * (const big_float & b, const big_float & c)
{
    ARAGELI_ASSERT_0
    (
        !(b.mode-c.mode) &&
        "Try to subtraction two big_float numbers with different "
            "rounding modes"
    );

    return
        mul
        (
            b,
            c,
            b.prec < c.prec ? b.prec : c.prec,
            b.mode
        );
}

//operator /
big_float operator / (const big_float & b, const big_float & c)
{
    ARAGELI_ASSERT_0
    (
        !(b.mode-c.mode) &&
        "Try to perfom division two big_float numbers with different "
            "rounding modes"
    );

    return
        div
        (
            b,
            c,
            b.prec < c.prec ? b.prec : c.prec,
            b.mode
        );
}

// adding ( the same as operator + )
big_float add(const big_float & b, const big_float & c )
{
    ARAGELI_ASSERT_0
    (
        !(b.mode-c.mode) &&
        "Try to perfom adding two big_float numbers with different "
            "rounding modes"
    );//or may be use global_mode

    return
        add
        (
            b,
            c,
            b.prec < c.prec ? b.prec : c.prec,
            b.mode
        );
}

// subtraction ( the same as operator - )
big_float sub (const big_float & b, const big_float & c )
{
    ARAGELI_ASSERT_0
    (
        !(b.mode-c.mode) &&
        "Try to subtraction two big_float numbers with different "
            "rounding modes"
    );

    return
        sub
        (
            b,
            c,
            b.prec < c.prec ? b.prec : c.prec,
            b.mode
        );
}

//multiplying ( the same as operator * )
big_float mul( const big_float & b, const big_float & c )
{
    ARAGELI_ASSERT_0
    (
        !(b.mode-c.mode) &&
        "Try to subtraction two big_float numbers with different "
            "rounding modes"
    );

    return
        mul
        (
            b,
            c,
            b.prec < c.prec ? b.prec : c.prec,
            b.mode
        );
}

// division ( the same as operator / )
big_float div ( const big_float &b, const big_float & c )
{
    ARAGELI_ASSERT_0
    (
        !(b.mode-c.mode) &&
        "Try to perfom division two big_float numbers with different "
            "rounding modes"
    );

    return
        div
        (
            b,
            c,
            b.prec < c.prec ? b.prec : c.prec,
            b.mode
        );
}

//adding with precision prec
big_float add(const big_float & b, const big_float & c, long prec)
{
    ARAGELI_ASSERT_0
    (
        !(b.mode-c.mode) &&
        "Try to perfom adding two big_float numbers with different "
            "rounding modes"
    );

    return add ( b, c, prec, b.mode );
}

//subtraction with precision prec
big_float sub(const big_float & b, const big_float & c, long prec)
{
    ARAGELI_ASSERT_0
    (
        !(b.mode-c.mode) &&
        "Try to perfom subtraction two big_float numbers with different "
            "rounding modes"
    );

    return sub( b, c, prec, b.mode );
}

//multiplying with precision prec
big_float mul(const big_float & b, const big_float & c, long prec)
{
    ARAGELI_ASSERT_0
    (
        !(b.mode-c.mode) &&
        "Try to perfom multiplying two big_float numbers with different "
            "rounding modes"
    );

    return mul ( b, c, prec, b.mode );
}

//division with precision prec
big_float div(const big_float & b, const big_float & c, long prec )
{
    ARAGELI_ASSERT_0
    (
        !(b.mode-c.mode) &&
        "Try to perfom multiplying two big_float numbers with different "
            "rounding modes"
    );

    return div ( b, c, prec, b.mode);
}

//adding with precision prec, rounding mode is mode
//(it is all the same the b and c are normlized or not)
//TODO remove WARNINGS related with EXACT mode
big_float add ( const big_float & b, const big_float & c, long prec, int mode )
{
    CHECK_PREC(prec)
    CHECK_MODE(mode)
    big_int b_e(b.e), c_e(c.e), b_s(b.s), c_s(c.s);

    big_float temp;

    // some checks
    if ( !c_s.length() ) // c == 0, so b + c == b
    {
        temp = b;
        temp.normalize_1 ( prec, mode );
        return temp;
    }
    if ( !b_s.length() ) // b == 0, so b + c == c
    {
        temp = c;
        temp.normalize_1 ( prec, mode );
        return temp;
    }

    // followed if ( ... )... needed for adding no normalized numbers, make equal length
    long delta = b_s.length() - c_s.length();

    if ( delta > 0 )
    {
        c_s = c_s << delta;
        c_e = c_e - delta;
    }
    else
    {
        b_s = b_s << -delta;
        b_e = b_e + delta;
    }

    if ( b_e < c_e )
    {
        swap ( b_e, c_e );
        swap ( b_s, c_s );
    }

    big_int ed = b_e - c_e;//or three times to compute or once to save

    if ( ed > prec  && mode != EXACT/*+1 */) // ������� ����b_e - c_e �� ��� �mode == EXACT !!!
    //  �������, ��� �� mode == TO_NAREST
    {
        temp.s = b_s;//�� mode=EXACT, ��� ������, ������ � ��� ��
        temp.e = b_e;
        if ( mode != TO_NEAREST )
        {
            temp.s = ( temp.s << 1 ) + c_s.sign();
            temp.e = temp.e - 1;
        }
    }
    else
    {
        if ( ed.length() > _Internal::bits_per_digit /* PREC_MAX - b_s.length()*/ ) //ed > PREC_MAX and mode == EXACT
            big_float_fatal_error( "can't perfom adding with EXACT rounding mode" );

        b_s = b_s << ed/*.to_digit ()*/; //������� ����, �� mode == EXCACT � b_e - c_e �� ���

        temp.s = c_s + b_s;
        temp.e = c_e;
    }
    temp.normalize_1 ( prec, mode );
    return temp;
}

//subtraction
big_float sub(const big_float & b, const big_float & c, long prec, int mode)
{
    big_float temp(c);
    temp.s = -temp.s;
    temp = add ( b, temp, prec, mode);
    return temp;
}

//myltiplying with precision prec, rounding mode is mode
//(it is all the same are the b and c normlized or not)
big_float mul( const big_float & b, const big_float & c, long prec, int mode )
{
    //if ( b.prec != c.prec )
    //{    std::cout << "Warning! Perform mul with different precisions\n" << b.prec << " vs " << c.prec;}


    CHECK_PREC(prec)
    CHECK_MODE(mode)
    big_float temp;
    //TODO The way to simplify computation is check prec and then truncate b or/and c
    temp.s = b.s * c.s;
    temp.e = b.e + c.e;
    temp.normalize_1 ( prec, mode );
    return temp;
}

//division (temporary version)
big_float div ( const big_float & b, const big_float & c, long prec, int mode)
{
    if ( c.s==0 )
        return big_float();//
    big_float x;
    big_int temp = pow2<big_int> ( prec << 1 );

    x.s = temp / c.s;
    x.e = -c.e - prec - prec;

    x.normalize_1 (prec,mode );
    return mul( x, b, prec, mode );
}
//not used for the time present
big_float divnu ( const big_float & b, const big_float & c, long prec, int mode)
{
    //��������1/� ��������� b
    //� ��, �� ���� ����� �� ��� ���� �� ���1/2 �1
    short k = 0;
    big_float x;
    big_float temp = 1;
    temp.out ( std::cout, 'd');
    x.s = c.s;
    while ( x.s.length() % _Internal::bits_per_digit > 0 )
    {
        x.s = x.s << 1;
        k++;
    }
    x.e = -( int )x.s.length();// / bits_per_digit;
    for ( int i=0; i < prec + 1; i++ )
    {
        temp = mul (2,temp,prec,mode) - mul ( mul ( temp,temp,prec,mode ),x,prec,mode );
    }
    for (int i = 0; i < k; i++)
    {
        temp = temp *big_float(2.0);
    }
    temp.e = temp.e + x.e - c.e;
    temp = mul ( b, temp, prec, mode );
    return temp;
}
//experimental method
big_float div_i ( const big_int & c )
{
    std::size_t l  = c.length();

    std::size_t step = 1;
    big_float res;

    res.s = 1;

    for ( step = 1 ; step < 32; step <<= 1 )
    {
        res.s = res.s * ( pow2<big_int> ( l * step + 1  ) - c * res.s );
    }
    //res.e =- l * ( step );
    std::cout << 10000 * res.s / pow2<big_int> ( 500)<< std::endl;
    //std::cout << res.s << std::endl << res.e << std::endl;
    return res;
}

/*
* stream operators
*/
// the name is self-explaining ( e.g  123.123000000 to 123.123 etc )
static void reduce_final_zeros ( std::basic_string<char> &str )
{
    std::size_t last_zero = str.length() - 1;

    while ( str[ last_zero ] == '0' )
        last_zero--;
    str.erase(last_zero + 1);
    if ( str[last_zero] == '.' )
        str.erase(last_zero--);
    if ( str[last_zero] == '.' )
        str.erase(last_zero);
}

//static big_float::special_numbers big_float::get_special_from_string ( const std::stringstream &str )
//{
//    /if ( str ")
//}

std::istream & operator >> ( std::istream &is , big_float &fnum )
{
    // is >> fnum.special;
    //if ( fnum.is_special() ) return is;
    char simbol;
    big_int dm, de, bm,be;
    long add = 0;

    std::stringstream bufferm; //integer part of mantissa
    std::stringstream buffere; //exponenta

    do
    {
        is.get( simbol );
    } while ( isspace ( simbol ) && !is.fail() );

    if ( simbol == '-' || simbol == '+' ) // signum may be
    {
        bufferm << simbol;
        is.get ( simbol );
    }

    if ( isdigit ( simbol ) ) // mantissa
    {
        bufferm << simbol;
        is.get ( simbol );
        while ( isdigit ( simbol ) && !is.fail() )
        {
            bufferm << simbol;
            is.get ( simbol );
        }
    }

    if ( simbol == '.' )//decimal point
    {
        simbol = is.get ();
        while ( isdigit (simbol) && !is.fail() )
        {
            bufferm << simbol;
            simbol = is.get();
            add++;
        }
    }

    if ( simbol == 'e' || simbol == 'E')//exponenta
    {
        is.get ( simbol );
        if ( simbol == '-' || simbol == '+') // signum may be
        {
            buffere << simbol;
            is.get ( simbol );
        }
        while ( isdigit (simbol)  && !is.fail() )
        {
            buffere << simbol;
            is.get ( simbol );
        }
    }

    if ( bufferm.str().empty() || bufferm.str() == "+" || bufferm.str() == "-")
    {
        fnum = big_float ();//zero
    }
    else
    {
        bufferm >> dm;
        if ( !bufferm.str().empty() && buffere.str() != "+" && buffere.str() != "-" )
            buffere >> de;
        de = de - add;
        do_bin_convert (dm, de, fnum.prec, bm, be);
        /*
        if ( be.get_sign() != -1 )
        {
            bm = bm << (be - (be / bits_per_digit) * bits_per_digit);
            be =  be  / bits_per_digit;
        }
        else
        {
            bm = bm << bits_per_digit + be - (be / bits_per_digit) * bits_per_digit;
            be = be / bits_per_digit - 1;
        }
        std::cout << "bm = " << bm.length() << "    be = "  << be.length() << std::endl;
        */
        fnum = big_float ( bm, be, fnum.mode );
        //std::cout << fnum.get_significant()<< std::endl;
        //std::cout << fnum.get_exponent()<< std::endl;
    }

    is.putback ( simbol );
    return is;
}

std::ostream & operator << ( std::ostream &os , const big_float &fnum )
{
    if ( fnum.is_inf() || fnum.is_nan() )
        os << fnum.special;

    long flags = os.flags( );

    if ( flags & std::ios::scientific )
        fnum.out ( os, 'e' );
    else if ( flags & std::ios :: fixed )
        fnum.out ( os, 'f');
    else
        fnum.out(os, 'a');
    return os;
}

/// Input number
void big_float :: in ( std::istream & ins, char c )
{
    switch  ( c )
    {
        case 'f':
            ins >> *this;
            break;
        case 'd':
            ins >> s >> e;
            normalize_1 ();
            break;
        default:
            big_float_warning ( "unrecognized input format: " );
            std::cerr << c << '\n';
            ins >> *this;
    }
}
//TODO Add rounding instead truncation
void big_float :: out ( std::ostream & os, char c  ) const
{
    using namespace _Internal;

    if ( !s.sign() ) // show zero
    {
        switch( c )
        {
            case 'f': // 0.00...
                os << "0.";
                os.width( os.precision() );
                os.fill('0');
                os << "0";
                break;
            case 'e': // 0.000...e0 or 0.000...E0
                os << "0.";
                os.width( os.precision() );
                os.fill('0');
                os << "0";
                if ( os.flags() & std::ios_base::uppercase )
                    os << "E";
                else
                    os << "e";
                os << "0";
                break;
            default: // just 0
                os << "0";
        }
        return;
    }

    std::stringstream str;                  // for temporary output
    std::ios_base::fmtflags prev_flags;     // ostream flags
    big_int dm, de;                         // mantissa and exponenta in decimal representation
    int  position;                          // decimal point position
    std::basic_string<char> temp_str;       // for temporary output

    if ( os.precision() > D_PREC_MAX )
        os.precision( D_PREC_MAX );
    prev_flags = os.flags ();

    //don't take a lot of trouble over s    ignums
    if ( s.sign() == -1 )
        os << "-";

    else if ( prev_flags & std::ios::showpos )
        os << "+";

    str.setf( std::ios::dec | std::ios::showpos ); // always out signum see below
    ARAGELI_ASSERT_1(is_converted_to<int>(s.length())); // to be sure with the following conversions

    switch ( c )
    {
        case 'f': //floating point mode e.g 123.456
                position = static_cast<int>(s.length()) + e.to_native_int<int>(); // Check this conversion good way to improve this function

                //approximately evaluate floating point position in decimal representation
                if ( position >= 0 )
                    position = 1 + ( int )((position + 1) * log (2.0l) / log ( 10.0l ));
                else
                    position = (int)( (long double) position * log (2.0l) / log ( 10.0l ) );

                if ( position + os.precision() > 0 ) //there is significant digits (probably)
                {
                    // convert to decimal and out in string stream
                    do_dec_convert(dm, de, os.precision() + position, s, e);
                    str << dm;
                    temp_str = str.str();// save dm in non-const string variable for manipulation
                    temp_str.erase(0,1); // erase signum ( it have been printed already )

                    if ( position > 0 )
                    {
                        //insert decimal point in right position
                        temp_str.insert( position = static_cast<int>(temp_str.length()) + de.to_native_int<int>(), "." );
                    }
                    else //there is leading zeros
                    {
                        os << "0.";
                        if ( position ) //one zero is produced already
                        {
                            os.width( -position );
                            os.fill ( '0' );
                            os << "0";
                        }
                        if (temp_str.length() <= os.precision() + position )
                            position--, os << "0";
                    }

                    //erase excess digits and final zeros //without rounding!!
                    temp_str.erase( position + os.precision() );
                    reduce_final_zeros ( temp_str );
                    os << temp_str;
                }
                else // no significant digits on this precision (only zeros)
                    os << "0.0";

                break;

        case 'e':// exponential mode  e.g 1.23e456

                do_dec_convert ( dm, de, os.precision(), s, e );
                str << dm;
                temp_str = str.str();
                temp_str.erase(0,1); // erase signum ( it have been printed already )
                temp_str.insert(1,".");
                reduce_final_zeros ( temp_str );

                //out
                if ( prev_flags & std::ios::uppercase )
                    os << temp_str << 'E' << de + os.precision();
                else
                    os << temp_str << 'e' << de + os.precision();

                break;

        case 'a': // auto format ( if neither scientific notation and not fixed notation are used )
                do_dec_convert ( dm, de, os.precision(), s, e );
                str << dm;
                temp_str = str.str();
                temp_str.erase(0,1);// erase signum ( it have been printed already )

                if ( de.sign() < 0 && de.to_native_int<int>() > -4 - os.precision () ) // show number in fixed format
                {

                    position = static_cast<int>(temp_str.length()) + de.to_native_int<int>(); //determine floating point position
                    if ( position <= 0 ) //there is leading zeros
                    {
                        os << "0.";
                        if ( position ) //one zero is produced already
                        {
                            os.width( -position );
                            os.fill ( '0' );
                            os << "0";
                        }
                    }
                    else if ( position != os.precision() )
                        //insert decimal point in right position
                        temp_str.insert ( position, "." );
                    else
                        temp_str.insert ( position, ".." ); //will be erased in followed

                    //erase excess digits and //without rounding!!
                    temp_str.erase( temp_str.length()-1 );
                    reduce_final_zeros ( temp_str );
                    os << temp_str;
                }
                else // use scientific format
                {
                    temp_str.insert( 1,"." );
                    //erase excess digits //without rounding!!
                    temp_str.erase( temp_str.length() - 1 );
                    reduce_final_zeros ( temp_str );

                    //out
                    if ( prev_flags & std::ios::uppercase )
                        os << temp_str << 'E' << de + os.precision();
                    else
                        os << temp_str << 'e' << de + os.precision();
                }

            break;

        case 'b'://binary ( zero and one ). out significant and exponent separately
            if ( s.sign() == -1 )
                os << '-';
            else if ( prev_flags & std::ios::showpos )
                os << '+';
            for ( int counter = s.length () - 1; counter >= 0 ; counter-- )
                os << s [ counter ];

            os << '\t';

            if ( e.sign() == -1 )
                os << '-';
            else if ( prev_flags & std::ios::showpos )
                os << '+';

            for ( int counter = e.length () - 1; counter >= 0 ; counter-- )
                os << e [ counter ];
            break;

        case 'd': // out significant and exponent separately, in decimal mode
            os.setf( std::ios :: dec, std::ios::hex );
            os << s << '\t' << e;
            break;

        case 'h': // out significant and exponent separately, in hex mode
            os.setf( std::ios::hex, std::ios::dec );
            os << s << '\t' << e;
            break;

        case 'o': // out significant and exponent separately, in oct mode
            os.setf( std::ios :: oct, std::ios::dec | std::ios::hex );
            os << s << '\t' << e;
            break;

        default:
            std::cerr << "Warning!: unrecognized output format:" << c << std::endl;
            os.setf( std::ios::dec);
            os << s << '\t' << e;
    }
    //restore flags
    os.setf ( prev_flags, os.flags() );

}

/*
* miscellaneous functions
*/
//returns a floating-point value representing the smallest integer that is greater than or equal to a
big_float ceil  (const big_float & a)
{
    big_float res ( a );
    big_int e = a.e;
    big_int temp;

    if ( e >=  0 )
        return res;
    if ( e <= -big_int(a.s.length()) )
    {
        if ( a.s > 0 )
            return 1.0;
        else
            return 0.0;
    }
    temp = ( res.s >> -e) << -e;
    if ( res.s != temp && res.s > 0 )
        res.s = temp + pow2<big_int>( -e/*.to_digit ()*/ );
    else
        res.s = temp;
    return res;
}

// returns a floating-point value representing the largest integer that is less than or equal to a
big_float floor (const big_float & a)
{
    big_float res ( a );
    big_int e = a.e;
    big_int temp;

    if ( e >=  0 )
        return res;
    if ( e <= -big_int(a.s.length()) )
    {
        if ( a.s > 0 )
            return 1.0;
        else
            return 0.0;
    }
    e = -e;
    temp = ( res.s >> e) << e;
    if ( res.s != temp && res.s < 0 )
        res.s = temp - pow2<big_int>( e );
    else
        res.s = temp;
    return res;

}

// Returns the fractal part of the number
big_float frac  (const big_float & a)
{
    if ( a.e >= 0 )
        return 0.0;

    big_float res ( a );
    if ( res.e <= -big_int(res.s.length()) )
        return res;

    res.s = res.s - ((res.s >> -res.e) << -res.e);
    return res;
}

// returns an big-integer value representing the smallest integer that is greater than or equal to a
big_int iceil (const big_float & a)
{
    std::size_t l = a.s.length();
    if ( !l )
        return l;

    big_int temp ( a.s );
    if ( a.e > 0 )
    {
        //if ( a.e + a.s.length() > SHRT_MAX/*??*/  )
        //    big_float_fatal_error ( "Can't convert from big_float to big_int ( exponent too large )..." );
        return a.s <<a.e;
    }
    if ( a.e <= -big_int(a.s.length()) )
        return ( a.s > 0 ) ? 1 : 0;
    temp = temp >> -a.e;
    if ( temp << -a.e != a.s && temp > 0)
        return temp + 1;
    return temp;
}

// returns a big-integer  value representing the largest integer that is less than or equal to a
big_int ifloor(const big_float & a)
{
    std::size_t l = a.s.length();
    if ( !l )
        return l;

    big_int temp ( a.s );
    if ( a.e > 0 )
    {
        //if ( a.e + a.s.length() > SHRT_MAX/*??*/  )
        //    big_float_fatal_error ( "Can't convert from big_float to big_int ( exponent too large )..." );
        return a.s <<a.e;
    }
    if ( a.e <= -big_int(a.s.length()) )
        return ( a.s > 0 ) ? 0 : -1;
    temp = temp >> -a.e;
    if ( temp << -a.e != a.s && temp > 0)
        return temp;
    return temp - 1;
}
//sqrt
big_float fsqrt( const big_float & b )
{
    return fsqrt( b, big_float :: global_prec, big_float :: global_mode);
}

//user must call fsqrt( const big_float & b ) function instead two forward
//functions ( because EXACT mode not supported yet )
big_float fsqrt(const big_float & bf, long prec)
{
    return fsqrt( bf, prec, big_float :: global_mode);
}

big_float fsqrt(const big_float & bf, long prec, int mode)
{
    if ( bf.s == 0 )
        return big_float (prec,mode);
    big_float res ( bf );
    int l;

    if ( res.e [ 0 ] )
    {
        res.s = res.s << 1;
        res.e = res.e - 1;
    }

    ARAGELI_ASSERT_1(is_converted_to<int>(res.s.length()));
    l = (static_cast<int>(res.s.length()) - 2 * prec - 1);
    l = (l / 2) * 2;

    if ( l > 0 )
    {
        res.s = res.s >> l;
        res.e = res.e + l;
    }
    else
    {
        res.s = res.s << (-l) + 2;
        res.e = res.e + l - 2;
    }
    res.s = sqrt ( res.s );
    res.e = res.e >> 1;
    res.normalize_1 ( prec, mode );//error may be if mode == TO_INF

    return res;
}
//Newton fsqrt
big_float nfsqrt ( const big_float & bf, long prec, int mode )
{
    big_float res;
    //std::cout << bf << std::endl;
    res.e = ((( bf.get_exponent() ) + bf.get_significant().length() - 1) >> 1) + 1;
    res.s = 1;
    //res.normalize_1();
    std::cout << "x0 "<<res << std::endl;
    //res.out ( std::cout, 'd' ) ;
    //std::cout << std::endl;
    std::size_t n = (long) (log ( (long double)prec + 1 ) / log ( 2.0l ) + 1.0);

    for ( std::size_t counter = 1; counter <= 2 * n; counter ++ )
    {
        std::cout << div( bf, res, counter * 2, EXACT ) << std::endl;
        res = add ( res,  div( bf, res, counter * 2, EXACT ), counter * 2, TO_NEAREST );
        std::cout << res << std::endl;
        res.e = res.e - 1;
        std::cout << counter << " iter \t";
        std::cout << res << std::endl;
        //res.out( std::cout, 'd' );
        //std::cout << std::endl;
    }
    res.normalize_1 ( prec, mode );//error may be if mode == TO_INF

    return res;
}
//Random numbers
big_float frandom  ( long prec )
{
    CHECK_PREC(prec)//
    return big_float ( big_int::random_with_length_or_less( prec ), big_int (-prec) );
}
//Random numbers
big_float frandom  ( )
{
    return big_float ( big_int::random_with_length_or_less( big_float::global_prec ), big_int (-big_float::global_prec) );
}

big_float frandom1 ( long bits, const big_int & exp)
{
    CHECK_PREC(bits)//
    return big_float (  big_int::random_with_length_or_less( bits ) + pow2<big_int>( bits - 1 ) , exp );
}

#if 0
const std::string big_float::get_lower_string ( const special_numbers sn )
{
    switch (sn)
    {
        case NAN    : return "nan";
        case P_INF  : return "inf";
        case M_INF  : return "+inf";
        case P_ZERO : return "0";
        case M_ZERO : return "-0";
        case FINITE :
        default     : return "finite";
    }
}

const std::string big_float::get_upper_string ( const special_numbers sn )
{
    switch (sn)
    {
        case NAN    : return "NAN";
        case P_INF  : return "INF";
        case M_INF  : return "-INF";
        case P_ZERO : return "0";
        case M_ZERO : return "-0";
        case FINITE :
        default     : return "FINITE";
    }
}

std::ostream & operator << ( std::ostream &os, const big_float::special_numbers &sn )
{
    return os << ( os.flags() & std::ios_base::uppercase  ?
        big_float::get_upper_string( sn ) : big_float::get_lower_string( sn ));
}

std::istream & operator >> ( std::istream &is, big_float::special_numbers &sn )
{
    char buf [5]  = {0};

    is.getline(buf,4);
    sn = big_float::special_numbers(big_float::FINITE);

    //if ( !sn.is_special () )
    {
        is.putback(buf[3]);
        is.putback(buf[2]);
        is.putback(buf[1]);
        is.putback(buf[0]);
    }
    return is;
}
#endif

long big_float::global_prec = 350; //bits precision
long big_float::global_mode = TO_NEAREST; //rounding mode

}
#endif
