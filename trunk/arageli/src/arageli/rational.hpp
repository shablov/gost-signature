/*****************************************************************************

    rational.hpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2007 Nikolai Yu. Zolotykh

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

#ifndef _ARAGELI_rational_hpp_
#define _ARAGELI_rational_hpp_

#include "config.hpp"

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <cctype>
#include <cmath>
#include <functional>

#include "type_traits.hpp"
#include "type_pair_traits.hpp"
#include "basefuncs.hpp"
#include "frwrddecl.hpp"
#include "exception.hpp"
#include "factory.hpp"
#include "cmp.hpp"
#include "io.hpp"
#include "powerest.hpp"
#include "big_int.hpp"

#include "std_import.hpp"

/**
    \file
    Rational numbers.
    The module containes implementation for a rational number.
*/


namespace Arageli
{



/// Base for all rationals.
class rational_base
{
public:

    /// Exception of all rationals numbers.
    class exception :
        public virtual Arageli::exception
    {
        std::string msg_m;

    public:

        exception (const std::string& msg_a) :
            msg_m(msg_a)
        {}

        virtual std::string msg () const
        {
            return msg_m;
        }
    };
};


/// Implementation of a Rational Number.
/** An instance of this type is a rational number where the numerator
    and the denominator are both of type T.
    @param T a type of numerator and denominator, by default it is big_int  */
template <typename T = big_int>
class rational : public rational_base
{
    template <typename T1>
    friend class rational;

public:

    /// Type of numerator and denominator.
    typedef T value_type;

    /// Constructs zero.
    rational () :
        p(factory<T>::null()),
        q(factory<T>::unit())
    {}

    /// Initialize rational from string notation.
    rational (const char* str);

    /// Creates a copy of the some rational.
    template <typename T1>
    rational (const rational<T1>& x)
    {
        ARAGELI_ASSERT_1
        (
            reinterpret_cast<const void*>(this) !=
            reinterpret_cast<const void*>(&x)
        );

        p = x.p;
        q = x.q;
    }

    /// Creates a rational from integer value w.
    /** After this, numerator = w, denominator = 1.  */
    template <typename T1>
    rational (const T1& w) :
        p(w),
        q(factory<T>::unit(p))
    {}

    /// Creates the rational w/v.
    /** After this, numerator = w, denominator = v.  */
    rational (const T& u, const T& v) :
        p(u),
        q(v)
    {
        ARAGELI_ASSERT_0(!Arageli::is_null(q)); normalize();
    }


    rational (double x)
    {
        if(x == 0)
        {
            p = null<T>();
            q = unit<T>();
        }
        else
        {
            int pp;
            static const double powdig2 = pow(2.0, std::numeric_limits<double>::digits);
            p = powdig2 * frexp(x, &pp);
            q = powdig2;
            if(pp > 0)
                p *= pow(T(2), pp);
            else if(pp < 0)
                q *= pow(T(2), -pp);
            normalize();
        }
    }

    #if 0
    /// Converts from rational to integer base type T by division.
    /** Returns the result of (integer) division numerator/denominator.  */
    operator T () const
    {
        ARAGELI_ASSERT_0(is_normal());
        return numerator()/denominator();
    }
    #endif

    #define _ARAGELI_RATIONAL_OPERATOR_TO(TYPE)    \
        operator TYPE () const    \
        {    \
            return double(*this);    \
        }

    /// Convertion to int. Temporary implementation.
    _ARAGELI_RATIONAL_OPERATOR_TO(int);
    _ARAGELI_RATIONAL_OPERATOR_TO(std::size_t);

    /// Convertion to big_int. Temporary implementation.
    operator big_int () const
    {
        ARAGELI_ASSERT_0(is_normal());
        return numerator()/denominator();
    }

    /// Converts from rational to float.
    /** Just returns the result of division float(numerator)/float(denominator).
        WARNING!  This is restricted implementation.  For some values of numerator
        and denominator this solution doesn't work (converting to double
        may cause overflow even if the ration numerator/denominator
        in real sense is representable with available precision as double). */
    operator float () const
    {
        ARAGELI_ASSERT_0(is_normal());
        return float(numerator())/float(denominator());
    }

    /// Converts from rational to double.
    /** Just returns the result of division double(numerator)/double(denominator).
        WARNING!  This is restricted implementation.  For some values of numerator
        and denominator this solution doesn't work (converting to double
        may cause overflow even if the ration numerator/denominator
        in real sense is representable with available precision as double). */
    operator double () const
    {
        ARAGELI_ASSERT_0(is_normal());
        return double(numerator())/double(denominator());
    }

    /// Converts from rational to long double.
    /** Just returns the result of division
        (long double)(numerator)/(long double)(denominator).
        WARNING!  This is restricted implementation.  For some values of numerator
        and denominator this solution doesn't work (converting to double
        may cause overflow even if the ration numerator/denominator
        in real sense is representable with available precision as double). */
    operator long double () const
    {
        ARAGELI_ASSERT_0(is_normal());
        return (long double)(numerator())/(long double)(denominator());
    }

    /// Returns true if the rational is nonzero.
    operator bool () const
    {
        ARAGELI_ASSERT_0(is_normal());
        return !Arageli::is_null(numerator());
    }

    bool operator! () const
    {
        return !bool(*this);
    }

    #if 0
    operator int () const
    {
        ARAGELI_ASSERT_0(is_normal());
        return numerator()/denominator();
    }
    #endif

    /// Assigns numerator and denominator from x to this rational.
    template <typename T1>
    rational& operator= (const rational<T1>& x)
    {
        ARAGELI_ASSERT_1
        (
            reinterpret_cast<const void*>(this) !=
            reinterpret_cast<const void*>(&x)
        );

        p = x.p;
        q = x.q;
        return *this;
    }

    /// Assigns this rationals with integer x.
    /** After this call numerator = x, denominator = 1.  */
    template <typename T1>
    rational& operator= (const T1& x)
    {
        p = x;
        q = unit(p);
        return *this;
    }

    rational& operator= (double x)
    {
        return *this = rational(x);
    }


    rational& operator= (const char* s)
    {
        return *this = rational(s);
    }


    /// Access to numerator.  Read-only.
    const T& numerator () const
    {
        return p;
    }

    /// Access to denominator.  Read-only.
    const T& denominator () const
    {
        return q;
    }

    /// Access to numerator (non-constant).
    T& numerator ()
    {
        return p;
    }

    /// Access to denominator (non-constant).
    T& denominator ()
    {
        return q;
    }

    /// Returns a negated rational.
    /** Returns a rational with numerator = - this numerator
        and the same denominator.  */
    rational operator- () const
    {
        return rational(Arageli::opposite(p), q);
    }

    /// Return a reference to this object.  For sameness only.
    const rational& operator+ () const
    {
        return *this;
    }


    rational& operator++ ()
    {
        p += q;
        normalize();
        return *this;
    }

    rational operator++ (int)
    {
        rational t = *this;
        operator++();
        return t;
    }

    rational& operator-- ()
    {
        p -= q;
        normalize();
        return *this;
    }

    rational operator-- (int)
    {
        rational t = *this;
        operator--();
        return t;
    }


    /// Makes opposite rational.
    /** Calls 'value_traits::opposite' for numerator of this rational.
        Returns reference to the rational. */
    rational& opposite ()
    {
        Arageli::opposite(&p);
        return *this;
    }

    /// Inverses this rational.
    /** Performs swap between numerator and denominator.
        Requirement: denominator must not be equal to zero.
        Returns reference to the rational. */
    rational& inverse ();


    ///    Sign of a number.
    /**    Returns
        -   0    if x = 0,
        -  -1    if x < 0,
        -  +1    if x > 0. */
    int sign () const;

    /// Returns true if this rational is integer.
    /** Returns true if denominator = 1.  */
    bool is_integer () const
    {
        return Arageli::is_unit(q);
    }

    /// Return true if this rational is zero.
    /** Return true if numerator = 0. */
    bool is_null () const
    {
        return Arageli::is_null(p);
    }

    /// Return true if this rational is unit.
    /** Return true if numerator = 1, denominator = 1.  */
    bool is_unit () const
    {
        return
            Arageli::is_unit(p) &&
            Arageli::is_unit(q);
    }

    /// Return true if this rational is opposite unit.
    /** Return true if numerator = -1, denominator = 1.  */
    bool is_opposite_unit () const
    {
        return
            Arageli::is_opposite_unit(p) &&
            Arageli::is_unit(q);
    }

    /// Reduces fraction numerator/denominator.
    /** This method is useful if you use non-constant form of numerator()
        and denominator() functions and "manually" alternate their values.
        After call this function is_normal() always returns true. */
    void normalize ();

    /// Returns true if the fraction numerator/denominator is irreducible.
    /** After calls of all methods (except non-constant form of numerator()
        and denominator()) the result of is_normal() is true. */
    bool is_normal () const;


    /// Swaps two rationals without copying (if possible).
    template <typename T1>
    void swap (rational<T1>& x)
    {
        std::swap(p, x.p);
        std::swap(q, x.q);
    }

private:

    T p;    // numerator
    T q;    // denominator
};


extern const char* rational_output_list_first_bracket_default;
extern const char* rational_output_list_second_bracket_default;
extern const char* rational_output_list_separator_default;
extern const char* rational_input_list_first_bracket_default;
extern const char* rational_input_list_second_bracket_default;
extern const char* rational_input_list_separator_default;
extern const char* rational_output_default_oblique_default;
extern const char* rational_input_default_oblique_default;


template <typename T, typename Ch, typename ChT>
inline std::basic_ostream<Ch, ChT>& output_list
(
    std::basic_ostream<Ch, ChT>& out,
    const rational<T>& x,
    const Ch* first_bracket = rational_output_list_first_bracket_default,
    const Ch* second_bracket = rational_output_list_second_bracket_default,
    const Ch* separator = rational_output_list_separator_default
)
{
    return
        out
            << first_bracket << x.numerator()
            << separator << x.denominator() << second_bracket;
}


template <typename T, typename Ch, typename ChT>
std::basic_istream<Ch, ChT>& input_list
(
    std::basic_istream<Ch, ChT>& in,
    rational<T>& x,
    const Ch* first_bracket = rational_input_list_first_bracket_default,
    const Ch* second_bracket = rational_input_list_second_bracket_default,
    const Ch* separator = rational_input_list_separator_default
);


template <typename T, typename Ch, typename ChT>
std::basic_ostream<Ch, ChT>& output_default
(
    std::basic_ostream<Ch, ChT>& out,
    const rational<T>& x,
    const Ch* oblique = rational_output_default_oblique_default
);


template <typename T, typename Ch, typename ChT>
std::basic_istream<Ch, ChT>& input_default
(
    std::basic_istream<Ch, ChT>& in,
    rational<T>& x,
    const Ch* oblique = rational_input_default_oblique_default
);


/// Reads a rational from a string notation.
/** Format is x or x/x where x is notation for object of type T. */
template <typename T, typename Ch, typename ChT>
inline std::basic_ostream<Ch, ChT>& operator<<
(
    std::basic_ostream<Ch, ChT>& out,
    const rational<T>& x
)
{
    return output_default(out, x);
}


/// Writes a rational to a string notation.
/** Format is x or x/x where x is notation for object of type T. */
template <typename T, typename Ch, typename ChT>
inline std::basic_istream<Ch, ChT>& operator>>
(
    std::basic_istream<Ch, ChT>& in,
    rational<T>& x
)
{
    return input_default(in, x);
}

template <typename T>
class io_binary<rational<T> > : public io_binary_base<rational<T> >
{
public:

    using io_binary_base<rational<T> >::output_stream;
    using io_binary_base<rational<T> >::input_stream;
    using io_binary_base<rational<T> >::calc;
    using io_binary_base<rational<T> >::input_mem;
    using io_binary_base<rational<T> >::output_mem;

    /// Stores rational object state to a binary stream. Seft-delimeted binary serialization.
    /** This functions uses the following format:
            NUMERATOR DENOMINATOR
        where NUMERATOR and DENOMINATOR are output by The Simple Binary
        format rules corresponding their types. */
    template <typename Stream>
    static inline Stream& output_stream (Stream& out, const rational<T>& x)
    {
        output_binary_stream(out, x.numerator());
        output_binary_stream(out, x.denominator());
        return out;
    }


    /// Loads rational object state from a binary stream. Compatible with output_stream.
    /** See output_stream(stream, rational) function for detailes on the format.
        If the function fails to read some of state components, an old value of x
        may be lost. All depends on input_binary_stream function for T.
        So, do not relay on the value of x when a given stream is not in a good state
        after call returns.

        The function takes input in The Simple Binary format.
    */
    template <typename Stream>
    static inline Stream& input_stream (Stream& in, rational<T>& x)
    {
        input_binary_stream(in, x.numerator());
        input_binary_stream(in, x.denominator());
        return in;
    }


    /// Calculates the number of chars required to store a given rational object in The Simple Binary form.
    /** This function calculates precise number of chars that will emit
        any function outputs in The Simple Binary format for one rational object,
        for example, output_binary_mem function. */
    static inline std::size_t calc (const rational<T>& x)
    {
        return calc_binary(x.numerator()) + calc_binary(x.denominator());
    }
};


/// @name Standard arithmetic operations.
// @{

template <typename T1, typename T2>
rational<T1> operator+
(const rational<T1>& b, const rational<T2>& c);

template <typename T1, typename T2>
rational<T1> operator-
(const rational<T1>& b, const rational<T2>& c);

template <typename T1, typename T2>
rational<T1> operator*
(const rational<T1>& b, const rational<T2>& c);

template <typename T1, typename T2>
rational<T1> operator/
(const rational<T1>& b, const rational<T2>& c);

template <typename T1, typename T2>
inline rational<T1> operator%
(const rational<T1>& b, const rational<T2>& c)
{
    ARAGELI_ASSERT_0(!is_null(c));
    return null(b);
}

// @}


#define _ARAGELI_RATIONAL_OPERS(OPER)    \
    template <typename T1, typename T2>    \
    inline rational<T1>& operator OPER##=    \
    (rational<T1>& a, const rational<T2>& b)    \
    {    \
        return a = a OPER b;    \
    }    \
    \
    template <typename T1, typename T2>    \
    inline rational<T1>& operator OPER##=    \
    (rational<T1>& a, const T2& b)    \
    {    \
        return a = a OPER rational<T1>(b);    \
    }    \
    \
    template <typename T1, typename T2>    \
    inline rational<T1> operator OPER    \
    (const rational<T1>& a, const T2& b)    \
    {    \
        return a OPER rational<T1>(b);    \
    }    \
    \
    template <typename T1, typename T2>    \
    inline rational<T1> operator OPER    \
    (const T2& a, const rational<T1>& b)    \
    {    \
        return rational<T1>(a) OPER b;    \
    }


/// @name Standard arithmetic operations.
// @{

_ARAGELI_RATIONAL_OPERS(+)
_ARAGELI_RATIONAL_OPERS(-)
_ARAGELI_RATIONAL_OPERS(*)
_ARAGELI_RATIONAL_OPERS(/)
_ARAGELI_RATIONAL_OPERS(%)

// @}


//template <typename T>
//int rational<T>::sign () const;


/// Comparison of two rationals.
/**    Returns
    -  0  if a == b,
    -  -1 if a < b,
    -  1  if a > b. */
template <typename T1, typename T2>
inline int cmp
(
    const rational<T1>& a,
    const rational<T2>& b
)
{
    return (a - b).sign();
}


template <typename T>
inline int sign (const rational<T>& x)
{
    return x.sign();
}

template <typename T>
inline bool is_coprime
(const rational<T>& a, const rational<T>& b)
{
    return true;
}

template <typename T>
inline rational<T> gcd
(const rational<T>& a, const rational<T>& b)
{
    if(is_null(a) && is_null(b))
        return a;    // WARNING!
    else return unit(a);
}



#define _ARAGELI_RATIONAL_CMP1(OPER)    \
    template <typename T1, typename T2>    \
    inline bool operator OPER    \
    (const rational<T1>& a, const rational<T2>& b)    \
    {    \
        return cmp(a, b) OPER 0;    \
    }    \
    \
    template <typename T1, typename T2>    \
    inline bool operator OPER    \
    (const rational<T1>& a, const T2& b)    \
    {    \
        return cmp(a, rational<T2>(b)) OPER 0;  /* WARNING! */    \
    }    \
    \
    template <typename T1, typename T2>    \
    inline bool operator OPER    \
    (const T1& a, const rational<T2>& b)    \
    {    \
        return cmp(rational<T1>(a), b) OPER 0;  /* WARNING! */    \
    }


/// @name Standard comparision operators.
// @{

_ARAGELI_RATIONAL_CMP1(<)
_ARAGELI_RATIONAL_CMP1(>)
_ARAGELI_RATIONAL_CMP1(==)
_ARAGELI_RATIONAL_CMP1(!=)
_ARAGELI_RATIONAL_CMP1(<=)
_ARAGELI_RATIONAL_CMP1(>=)

// @}


//template <typename T>
//void rational<T>::normalize ();

/// Floor of x.
/** Returns maximum integer i <= x. */
template <typename T>
T ifloor (const rational<T>& x);

/// Ceiling of x.
/** Returns minimum integer i >= x. */
template <typename T>
T iceil (const rational<T>& x);

/// Fractional part of x.
/** Return x - floor(x).  */
template <typename T>
inline rational<T> frac (const rational<T>& x)
{
    return x - ifloor(x);
}

//    WARNING! In the following selectors we have non reference as return value.

/// Unary functor: selects a numerator of an argument of type rational.
template <typename T>
struct select_numer : public std::unary_function<rational<T>, T>
{
    T operator() (const rational<T>& x) const
    {
        return x.numerator();
    }
};

/// Unary functor: selects a denominator of an argument of type rational.
template <typename T>
struct select_denom : public std::unary_function<rational<T>, T>
{
    T operator() (const rational<T>& x) const
    {
        return x.denominator();
    }
};

template <typename T1>
struct factory<rational<T1> >
{
private:

    typedef rational<T1> T;

public:

    static const bool is_specialized = true;

    static const T& unit ()
    {
        static const T unit_s(Arageli::unit<T1>());
        return unit_s;
    }

    static T unit (const T& x)
    {
        return
            T
            (
                Arageli::unit(x.numerator()),
                Arageli::unit(x.denominator())
            );
    }

    static const T& opposite_unit ()
    {
        static const T opposite_unit_s(Arageli::opposite_unit<T1>());
        return opposite_unit_s;
    }

    static T opposite_unit (const T& x)
    {
        return
            T
            (
                Arageli::opposite_unit(x.numerator()),
                Arageli::unit(x.denominator())
            );
    }

    static const T& null ()
    {
        static const T null_s(Arageli::null<T1>());
        return null_s;
    }

    static T null (const T& x)
    {
        return
            T
            (
                Arageli::null(x.numerator()),
                Arageli::unit(x.denominator())
            );
    }
};


template <typename T1>
inline bool is_null (const rational<T1>& x)
{
    return x.is_null();
}

template <typename T1>
inline rational<T1> opposite (const rational<T1>& x)
{
    return -x;
}

template <typename T1>
inline rational<T1>& opposite (const rational<T1>& x, rational<T1>* y)
{
    return *y = -x;
}

template <typename T1>
inline rational<T1>& opposite (rational<T1>* x)
{
    return x->opposite();
}

template <typename T1>
inline rational<T1> inverse (const rational<T1>& x)
{
    rational<T1> xx = x;
    return xx.inverse();
}

template <typename T1>
inline rational<T1>& inverse (const rational<T1>& x, rational<T1>* y)
{
    *y = x;
    return y->inverse();
}

template <typename T1>
inline rational<T1>& inverse (rational<T1>* x)
{
    return x->inverse();
}


template <typename T1>
inline std::ostream& output_polynom_first (std::ostream& out, const rational<T1>& x)
{
    return output_polynom_first_default(out, x);
}

template <typename T1>
inline std::ostream& output_polynom_internal
(std::ostream& out, const rational<T1>& x)
{
    return output_polynom_internal_default(out, x);
}

template <typename T1>
inline std::ostream& output_pow (std::ostream& out, const rational<T1>& x)
{
    return output_pow_default(out, x);
}

template <typename T1>
inline std::istream& input_polynom_first (std::istream& in, rational<T1>& x)
{
    return input_polynom_first_default(in, x);
}

template <typename T1>
inline std::istream& input_polynom_internal
(std::istream& in, rational<T1>& x)
{
    return input_polynom_internal_default(in, x);
}

template <typename T1>
inline std::istream& input_pow (std::istream& in, rational<T1>& x)
{
    return input_pow_default(in, x);
}

/// Absolute value of x.
template <typename T>
inline rational<T> abs (const rational<T>& x)
{
    if(x.sign() < 0)
        return -x;
    else
        return x;
}

/// Floor of x.
/** See Arageli::ifloor. */
template <typename T>
inline rational<T> floor (const rational<T>& x)
{
    return ifloor(x);
}

/// Ceiling of x.
/** See Arageli::iceil. */
template <typename T>
inline rational<T> ceil (const rational<T>& x)
{
    return iceil(x); }

} // namespace Arageli


namespace std
{

/// Absolute value of x.
template <typename T>
inline Arageli::rational<T> abs (const Arageli::rational<T>& x)
{
    return Arageli::abs(x);
}

/// Floor of x.
/** See Arageli::ifloor. */
template <typename T>
inline Arageli::rational<T> floor (const Arageli::rational<T>& x)
{
    return Arageli::floor(x);
}

/// Ceiling of x.
/** See Arageli::iceil. */
template <typename T>
inline Arageli::rational<T> ceil (const Arageli::rational<T>& x)
{
    return Arageli::ceil(x);
}


/// Specialization of standard template std::numeric_limits for rationals.
template <typename T> class numeric_limits<Arageli::rational<T> >
{
    typedef Arageli::rational<T> TTT;

public:

    static const bool is_specialized = true;

    // 'min' and 'max' return values like floating types
    // meaningfull if is_bounded only

    static TTT min () throw()
    {
        return TTT(Arageli::factory<T>::unit(), numeric_limits<T>::max());
    }

    static TTT max () throw()
    {
        return TTT(numeric_limits<T>::max, Arageli::factory<T>::unit());
    }

    static const int digits = 2*numeric_limits<T>::digits;
    static const int digits10 = 2*numeric_limits<T>::digits10;
    static const bool is_signed = numeric_limits<T>::is_signed;
    static const bool is_integer = false;
    static const bool is_exact = numeric_limits<T>::is_exact;
    static const int radix = 2;

    static TTT epsilon () throw()
    {
        return TTT();
    }

    static TTT round_error () throw()
    {
        return TTT();
    }

    static const int min_exponent = 0;
    static const int min_exponent10 = 0;
    static const int max_exponent = 0;
    static const int max_exponent10 = 0;
    static const bool has_infinity = false;
    static const bool has_quiet_NaN = false;
    static const bool has_signaling_NaN = false;
    static const float_denorm_style has_denorm = denorm_absent;
    static const bool has_denorm_loss = false;

    static TTT infinity () throw()
    {
        return TTT();
    }

    static TTT quiet_NaN () throw()
    {
        return TTT();
    }

    static TTT signaling_NaN () throw()
    {
        return TTT();
    }

    static TTT denorm_min () throw()
    {
        return TTT();
    }

    static const bool is_iec559 = false;
    static const bool is_bounded = numeric_limits<T>::is_bounded;
    static const bool is_modulo = false;
    static const bool traps = true;
    static const bool tinyness_before = false;
    static const float_round_style round = round_toward_zero;
};


/// Swaps two rationals without actully copying (if possible).
template <typename T>
void swap (Arageli::rational<T>& a, Arageli::rational<T>& b)
{
    a.swap(b);
}

/// Swaps two rationals without actully copying (if possible).
template <typename T1, typename T2>
void swap (Arageli::rational<T1>& a, Arageli::rational<T2>& b)
{
    a.swap(b);
}


}


namespace Arageli
{

/// Specialization of type_traits for rational.
template <typename T>
struct type_traits<rational<T> > :
    public type_traits_default<rational<T> >
{
    static const bool is_specialized = type_traits<T>::is_specialized;
    static const bool is_number = type_traits<T>::is_number;
    static const bool is_rational = true;
    static const bool is_integer_number = false;
    static const bool is_polynom = false;
    static const bool is_real_number = false;
    static const bool is_rational_number = type_traits<T>::is_number;
    static const bool is_complex_number = false;
    static const bool is_ring = type_traits<T>::is_ring;
    static const bool is_field = type_traits<T>::is_ring;
    static const bool is_finite = type_traits<T>::is_finite;

    static const bool is_additive_group =
        type_traits<T>::is_additive_group &&
        type_traits<T>::is_ring;

    static const bool is_multiplicative_group =
        type_traits<T>::is_additive_group &&
        type_traits<T>::is_ring;

    static const bool has_zero_divisor = type_traits<T>::has_zero_divisor;
    static const bool is_integer_modulo_ring = false;
    static const bool is_matrix = false;
    static const bool is_vector = false;

    static const bool has_commutative_multiplication =
        type_traits<T>::has_commutative_multiplication;

    static const bool has_commutative_addition =
        type_traits<T>::has_commutative_multiplication &&
        type_traits<T>::has_commutative_addition;

    static const bool has_null = type_traits<T>::has_null;
    static const bool has_unit = type_traits<T>::has_unit;

    static const bool has_opposite_unit =
        type_traits<T>::has_opposite_unit &&
        type_traits<T>::has_unit;

    static const bool is_aggregate = true;
    typedef T element_type;
    typedef type_category::rational category_type;
    static const category_type category_value;
};

template <typename T>
const type_category::rational
type_traits<rational<T> >::category_value =
    type_category::rational();


#if 0

/// Specialization of type_pair_traits for rational and unknown type.
/** This really need to be implemented some features for T2. Actually rational<T>
    is cannot converted to type that is not builtin or not T. This conversion
    will be implemented realization of T2. */
template <typename TE1, typename T2>
struct type_pair_traits<rational<TE1>, T2>
{
    static const bool is_specialized =
        type_traits<TE1>::is_specialized &&
        type_traits<T2>::is_specialized;

    static const bool is_convertible =
        type_pair_traits<TE1, T2>::is_convertible;

    static const bool is_safe_convertible =
        type_traits<T2>::is_rational &&
        type_traits<T2>::is_aggregate &&
        type_pair_traits<TE1, typename type_traits<T2>::element_type>::is_specialized &&
        type_pair_traits<TE1, typename type_traits<T2>::element_type>::is_safe_convertible;

    static const bool is_assignable = is_convertible;
    static const bool is_safe_assignable = is_safe_convertible;
    static const bool is_initializable = is_convertible;
    static const bool is_safe_initializable = is_safe_convertible;
};


/// Specialization of type_pair_traits for unknown type and rational.
template <typename T1, typename TE2>
struct type_pair_traits<T1, rational<TE2> > :
    public type_pair_traits_for_unknown_and_rational<T1, rational<TE2> >
{};


/// Specialization of type_pair_traits for two rationals.
template <typename TE1, typename TE2>
struct type_pair_traits<rational<TE1>, rational<TE2> > :
    public type_pair_traits_for_similar_aggregates<rational<TE1>, rational<TE2> >
{};


/// Specialization of type_pair_traits for two identical rationals.
template <typename TE>
struct type_pair_traits<rational<TE>, rational<TE> > :
    public type_pair_traits_for_the_same<rational<TE>, rational<TE> >
{};


/// Specialization of type_pair_traits for big_int and rational.
template <typename TE2>
struct type_pair_traits<big_int, rational<TE2> > :
    public type_pair_traits_for_unknown_and_rational<big_int, rational<TE2> >
{};


/// Specialization of type_pair_traits for rational and big_int.
template <typename TE1>
struct type_pair_traits<rational<TE1>, big_int>
{
    static const bool is_specialized =
        type_traits<TE1>::is_specialized &&
        type_traits<big_int>::is_specialized;

    static const bool is_convertible =
        type_pair_traits<TE1, big_int>::is_convertible;

    static const bool is_safe_convertible = false;
    static const bool is_assignable = is_convertible;
    static const bool is_safe_assignable = is_safe_convertible;
    static const bool is_initializable = is_convertible;
    static const bool is_safe_initializable = is_safe_convertible;
};


/// Specialization of type_pair_traits for rational and matrix.
template <typename TE1, typename TE2, bool REFCNTE2>
struct type_pair_traits<rational<TE1>, matrix<TE2, REFCNTE2> > :
    public type_pair_traits_for_unknown_and_vector_matrix
        <rational<TE1>, matrix<TE2, REFCNTE2> >
{};

/// Specialization of type_pair_traits for rational and matrix.
template <typename TE1, typename TE2, typename TD2, bool REFCNTE2>
struct type_pair_traits<rational<TE1>, sparse_polynom<TE2, TD2, REFCNTE2> > :
    public type_pair_traits_for_unknown_and_polynom
        <rational<TE1>, sparse_polynom<TE2, TD2, REFCNTE2> >
{};

#endif

/// Assignment with bitwise not for rationals --- isn't implemented.
/** I have no idea what meaning set for this operator. But there is
    this declaration to avoid some conflictes. */
template <typename T>
T& assign_bitwise_not (rational<T>& x);

}


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_RATIONAL
    #include "rational.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_RATIONAL
#endif


#endif
