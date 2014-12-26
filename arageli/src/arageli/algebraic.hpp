/*****************************************************************************

    algebraic.hpp

    This file is a part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2006--2007 Sergey S. Lyalin

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
    \file algebraic.hpp
    \brief Algebraic number declaration.

    This file contains declaration of class template `algebraic' for
    representation any algebraic number over rational field.

    WARNING. It's implementation in "old style" because it doesn't use
    mixing computations. Be careful: in the neares future we plan to replace
    it to modern one with full support of the mixing computation mechanism.

    WARNING! It's very slow implementaion. Do not use it if you need an efficient
    in computation. It's an experimental class.
*/

#ifndef _ARAGELI_algebraic_hpp_
#define _ARAGELI_algebraic_hpp_

#include "config.hpp"

#include <iostream>
#include <sstream>

#include "_utility.hpp"
#include "factory.hpp"
#include "intalg.hpp"
#include "polyalg.hpp"
#include "sturm.hpp"
#include "algebrslt.hpp"
#include "sparse_polynom.hpp"
#include "rational.hpp"
#include "interval.hpp"

#include "std_import.hpp"


namespace Arageli
{


/// Default set of configuration switches for algebraic template class.
/** This structure includes method to control representation normality:
    while initialization, before and after operations. */
template <typename TP, typename TS, typename Poly, typename Seg>
struct algebraic_config_default
{
    /// This method should be called by algebraic when initialization.
    static void init (Poly& poly, Seg& seg)
    {
        normalize(poly, seg);
    }

    /// This method should be called by algebraic just before retriving value to the user.
    static void get_value (const Poly& poly, const Seg& seg)
    {}

    /// This method should be called by algebraic just before performing some operation.
    static void before_oper (Poly& poly, Seg& seg)
    {}

    /// This method should be called by algebraic just after performing some operation.
    static void after_oper (Poly& poly, Seg& seg)
    {
        /*normalize(poly, seg);*/
    }

    /// Retruns if the representation is normal.
    static bool is_normal (const Poly& poly, const Seg& seg)
    {
        vector<Poly, false> ss;
        sturm_diff_sys(poly, ss);
        return sturm_number_roots(ss, seg) == 1;
    }

    /// Normalization of the representation.
    static void normalize (Poly& poly, Seg& seg);
};


/// Algebraic number class.
/** Represents algebraic algebraic closure of a set represented by TP.
    Representaion of each such element is a polynomial and a segment where
    only one root of given polynomial is located. */
template
<
    typename TP = rational<>,
    typename TS = TP,
    typename Poly = sparse_polynom<TP>,
    typename Seg = interval<TS>,
    typename Config = algebraic_config_default<TP, TS, Poly, Seg>
>
class algebraic
{
    Poly poly_m;    ///< polynomial with one root in seg segment that identifies the number
    Seg seg_m;    ///< segment where the number located
    Config config_m;    ///< configurator

    template
    <
        typename TP2,
        typename TS2,
        typename Poly2,
        typename Seg2,
        typename Cfg2
    >
    friend class algebraic;

public:

    /// Type for represenation of the base set.
    /** Coefficients of the polynomial. */
    typedef TP base_type;

    /// Type for representation of the segment limits.
    /** It can be different type that base_type, for example, base_type
        can be an integer type, but interval_limit_type is a rational. */
    typedef TS interval_limit_type;

    /// Type for representaion of the polynomial.
    typedef Poly polynom_type;

    /// Type for representation of the segment.
    typedef Seg interval_type;

    /// Structure with set of configuration switches.
    /** The default type for this is algebraic_config_default. */
    typedef Config config_type;

    /// Creates zero.
    /** Represents as a zero polynomial and segment (0, 0). */
    algebraic () :
        poly_m(null<TP>()),
        seg_m(null<TS>())
    {}

    /// Creates number as just a copy of given value.
    /** Passed value should be convertible to interval_limit_type.
        Represents as a polynomial "y - x" and segment (x, x)
        where 'x' is passed value and 'y' is a variable. */
    algebraic (const TS& x) :
        seg_m(x)
    {
        poly_m += typename Poly::monom(unit<TP>(), 1);
        poly_m -= TP(x);
    }

    /// Creates number as the minimal root of given polynomial.
    /** Finds the minimal by value root of given polynomial with
        segment obtained from strum function. The given polynomial
        must have at least one root. */
    algebraic (const Poly& px);

    // Creates number as the root of px on the segment s.
    algebraic (const Poly& px, const Seg& s) :
        poly_m(px),
        seg_m(s)
    {
        config_m.init(poly_m, seg_m);
    }


    //algebraic (const char* s);


    /// Temporaty implementation for converting to double.
    operator double () const
    {
        // WARNING! TEMPORARY!
        interval<rational<> > fseg = seg_m;
        interval_root_precise(poly_m, fseg, rational<>("1/100000000000"));
        return fseg.first();
    }


    /// Access to the polynomial.
    const Poly& polynom () const
    {
        config_m.get_value(poly_m, seg_m);
        return poly_m;
    }

    /// Access to the boundary segment.
    const Seg& segment () const
    {
        return seg_m;
    }

    /// Access to the polynomial.
    Poly& polynom ()
    {
        config_m.get_value(poly_m, seg_m);
        return poly_m;
    }

    /// Access to the boundary segment.
    Seg& segment ()
    {
        return seg_m;
    }


    /// Check if representation of the number is valid.
    bool is_normal () const
    {
        return config_m.is_normal(poly_m, seg_m);
    }

    /// Performs normalization of the number.
    void normalize () const
    {
        config_m.normalize(poly_m, seg_m);
    }

    algebraic operator- () const;

    const algebraic& operator+ () const
    {
        return *this;
    }

    algebraic& operator++ ()
    {
        *this += algebraic(unit<TP>());
        return *this;
    }

    algebraic& operator-- ()
    {
        *this -= algebraic(unit<TP>());
        return *this;
    }

    algebraic operator++ (int)
    {
        algebraic t = *this;
        operator++();
        return t;
    }

    algebraic operator-- (int)
    {
        algebraic t = *this;
        operator--();
        return t;
    }

    template
    <
        typename TP2,
        typename TS2,
        typename Poly2,
        typename Seg2,
        typename Cfg2
    >
    algebraic& operator+= (const algebraic<TP2, TS2, Poly2, Seg2, Cfg2>& x);

    template
    <
        typename TP2,
        typename TS2,
        typename Poly2,
        typename Seg2,
        typename Cfg2
    >
    algebraic& operator-= (const algebraic<TP2, TS2, Poly2, Seg2, Cfg2>& x);

    template
    <
        typename TP2,
        typename TS2,
        typename Poly2,
        typename Seg2,
        typename Cfg2
    >
    algebraic& operator*= (const algebraic<TP2, TS2, Poly2, Seg2, Cfg2>& x);

    template
    <
        typename TP2,
        typename TS2,
        typename Poly2,
        typename Seg2,
        typename Cfg2
    >
    algebraic& operator/= (const algebraic<TP2, TS2, Poly2, Seg2, Cfg2>& x);

    /// Returns true iff the number is zero.
    bool is_null () const
    {
        return is_equal_const(null<TP>());
    }

    /// Returns true iff the number is unit.
    bool is_unit () const
    {
        return is_equal_const(unit<TP>());
    }

    /// Returns true iff the number is opposite unit.
    bool is_opposite_unit () const
    {
        return is_equal_const(opposite_unit<TP>());
    }

    /// Return true iff the number is zero.
    bool operator! () const
    {
        return is_null();
    }

    /// Return true iff the number is non-zero.
    operator bool () const
    {
        return !is_null();
    }

private:

    /// Tests equality with a constant.
    bool is_equal_const (const TP& x) const
    {
        ARAGELI_ASSERT_0(is_normal());
        return
            x >= seg_m.first() && x <= seg_m.second() &&
            Arageli::is_null(poly_m.subs(x));
    }
};


template
<
    typename TP,
    typename TS,
    typename Poly,
    typename Seg,
    typename Cfg
>
struct factory<algebraic<TP, TS, Poly, Seg, Cfg> >
{
private:

    typedef algebraic<TP, TS, Poly, Seg, Cfg> T;

public:

    /// This structure is specialized for given types.
    static const bool is_specialized = true;

    /// Returns 1.
    static const T& unit ()
    {
        static const T unit_s = T(Arageli::unit<TS>());
        return unit_s;
    }

    /// Returns 1 by the pattern.
    static const T& unit (const T& x)
    {
        return unit();
    }

    /// Returns -1.
    static const T& opposite_unit ()
    {
        static const T opposite_unit_s = T(Arageli::opposite_unit<TS>());
        return opposite_unit_s;
    }

    /// Returns -1 by the pattern.
    static const T& opposite_unit (const T& x)
    {
        return opposite_unit();
    }

    /// Returns 0.
    static const T& null ()
    {
        static const T null_s = T(Arageli::null<TS>());
        return null_s;
    }

    /// Returns 0 by the pattern.
    static const T& null (const T& x)
    {
        return null();
    }

};


template
<
    typename TP,
    typename TS,
    typename Poly,
    typename Seg,
    typename Cfg
>
inline bool is_null (const algebraic<TP, TS, Poly, Seg, Cfg>& x)
{
    return x.is_null();
}


template
<
    typename TP,
    typename TS,
    typename Poly,
    typename Seg,
    typename Cfg
>
inline bool is_unit (const algebraic<TP, TS, Poly, Seg, Cfg>& x)
{
    return x.is_unit();
}


template
<
    typename TP,
    typename TS,
    typename Poly,
    typename Seg,
    typename Cfg
>
inline bool is_opposite_unit (const algebraic<TP, TS, Poly, Seg, Cfg>& x)
{
    return x.is_opposite_unit();
}


#define ARAGELI_ALGEBRAIC_BINARY_OP(OP, OPASS)    \
    template    \
    <    \
        typename TP1, typename TS1, typename Poly1, typename Seg1, typename Cfg1,    \
        typename TP2, typename TS2, typename Poly2, typename Seg2, typename Cfg2    \
    >    \
    inline algebraic<TP1, TS1, Poly1, Seg1, Cfg1> operator OP    \
    (    \
        algebraic<TP1, TS1, Poly1, Seg1, Cfg1> a,    \
        const algebraic<TP2, TS2, Poly2, Seg2, Cfg2>& b    \
    )    \
    {    \
        return a OPASS b;    \
    }

ARAGELI_ALGEBRAIC_BINARY_OP(+, +=)
ARAGELI_ALGEBRAIC_BINARY_OP(-, -=)
ARAGELI_ALGEBRAIC_BINARY_OP(*, *=)
ARAGELI_ALGEBRAIC_BINARY_OP(/, /=)
ARAGELI_ALGEBRAIC_BINARY_OP(%, %=)


#define ARAGELI_ALGEBRAIC_LOGIC_OP(OP)    \
    template    \
    <    \
        typename TP1,    \
        typename TS1,    \
        typename Poly1,    \
        typename Seg1,    \
        typename Cfg1,    \
    \
        typename TP2,    \
        typename TS2,    \
        typename Poly2,    \
        typename Seg2,    \
        typename Cfg2    \
    >    \
    inline bool operator OP    \
    (    \
        const algebraic<TP1, TS1, Poly1, Seg1, Cfg1>& a,    \
        const algebraic<TP2, TS2, Poly2, Seg2, Cfg2>& b    \
    )    \
    {    \
        return cmp(a, b) OP 0;    \
    }    \
    \
    template    \
    <    \
        typename TP1,    \
        typename TS1,    \
        typename Poly1,    \
        typename Seg1,    \
        typename Cfg1,    \
        typename TS2    \
    >    \
    inline bool operator OP    \
    (    \
        const algebraic<TP1, TS1, Poly1, Seg1, Cfg1>& a,    \
        const TS2& b    \
    )    \
    {    \
        return cmp(a, algebraic<TP1, TS2, Poly1, Seg1, Cfg1>(b)) OP 0;    \
    }    \
    \
    template    \
    <    \
        typename TS1,    \
        typename TP2,    \
        typename TS2,    \
        typename Poly2,    \
        typename Seg2,    \
        typename Cfg2    \
    >    \
    inline bool operator OP    \
    (    \
        const TS1& a,    \
        const algebraic<TP2, TS2, Poly2, Seg2, Cfg2>& b    \
    )    \
    {    \
        return cmp(algebraic<TP2, TS1, Poly2, Seg2, Cfg2>(a), b) OP 0;    \
    }



ARAGELI_ALGEBRAIC_LOGIC_OP(==)
ARAGELI_ALGEBRAIC_LOGIC_OP(!=)
ARAGELI_ALGEBRAIC_LOGIC_OP(<)
ARAGELI_ALGEBRAIC_LOGIC_OP(<=)
ARAGELI_ALGEBRAIC_LOGIC_OP(>)
ARAGELI_ALGEBRAIC_LOGIC_OP(>=)


template
<
    typename TP1,
    typename TS1,
    typename Poly1,
    typename Seg1,
    typename Cfg1,

    typename TP2,
    typename TS2,
    typename Poly2,
    typename Seg2,
    typename Cfg2
>
int cmp
(
    const algebraic<TP1, TS1, Poly1, Seg1, Cfg1>& a,
    const algebraic<TP2, TS2, Poly2, Seg2, Cfg2>& b
);


template
<
    typename TP1,
    typename TS1,
    typename Poly1,
    typename Seg1,
    typename Cfg1,

    typename TP2
>
inline int cmp
(
    const algebraic<TP1, TS1, Poly1, Seg1, Cfg1>& a,
    const TP2& b
)
{
    return cmp(a, algebraic<TP2, TS1, Poly1, Seg1, Cfg1>(b));
}


template
<
    typename TP1,

    typename TP2,
    typename TS2,
    typename Poly2,
    typename Seg2,
    typename Cfg2
>
inline int cmp
(
    const TP1& a,
    const algebraic<TP2, TS2, Poly2, Seg2, Cfg2>& b
)
{
    return cmp(algebraic<TP1, TS2, Poly2, Seg2, Cfg2>(a), b);
}


template
<
    typename TP,
    typename TS,
    typename Poly,
    typename Seg,
    typename Cfg
>
inline algebraic<TP, TS, Poly, Seg, Cfg> abs
(const algebraic<TP, TS, Poly, Seg, Cfg>& b)
{
    return (b < null(b)) ? -b : b;
}


/// Outputs algebraic number as in the format "(polynomial, segment)".
template
<
    typename Out,

    typename TP1,
    typename TS1,
    typename Poly1,
    typename Seg1,
    typename Cfg1
>
Out& operator<< (Out& out, const algebraic<TP1, TS1, Poly1, Seg1, Cfg1>& x);


//template <typename TP1, typename TS1, typename Poly1, typename Seg1, typename Cfg1>
//algebraic<TP1, TS1, Poly1, Seg1, Cfg1>::algebraic (const char* s)
//{
//    std::istringstream buf(s);
//    buf >> *this;
//}




} // namesapce Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_algebraic
    #include "algebraic.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_algebraic
#endif

#endif
