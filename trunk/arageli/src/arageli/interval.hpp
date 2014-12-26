/*****************************************************************************

    interval.hpp -- Approches to represent intervals.

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
    \file
    This file contains declaration of class template interval for representation
    intervals. It contains two fields: left limit and right limit and declares
    some routines to dealing with those limits together.

    WARNING. It's implementation in "old style" because it doesn't use
    mixing computations. Be careful: in the neares future we plan to replace
    it to modern one with full support of the mixing computation engine.
*/

#ifndef _ARAGELI_interval_hpp_
#define _ARAGELI_interval_hpp_

#include "config.hpp"

#include <iostream>
#include <sstream>
#include <utility>

#include "frwrddecl.hpp"
#include "function_traits.hpp"
#include "factory.hpp"
#include "cmp.hpp"
//#include "vector.hpp"

#include "std_import.hpp"


namespace Arageli
{

/// A kind of interval.
enum interval_kind
{
    interval_empty = 0,    ///< empty as a segment
    interval_point = 1,    ///< point as a segment
    interval_length = 2    ///< have a nonzero length
};

/// Represents an interval (open, close or semi-open; it depends on performed operations).
/** The class holds a pair of values --- the limits of the interval: left and right
    (or first and second). This class is appropriate for doing interval arithmetic.
    Empty interval is valid state; it represents by left > right values. */
template <typename T> class interval
{
    std::pair<T, T> lims;

    template <typename T1>
    friend class interval;

public:

    /// Type of the limits.
    typedef T value_type;

    /// Type of difference between two objects of value_type.
    typedef typename binary_function_traits<function_tag::minus, T, T>::result_type difference_type;

    /// Interval with left == right == 0.
    interval () :
        lims(null<T>(), null<T>())
    {}

    /// Interval with left == right == x.
    template <typename T1>
    interval (const T1& x) :
        lims(x, x)
    {}

    /// Interval from first_a to second_a. It can be empty, that's first_a > second_a.
    template <typename T1, typename T2>
    interval (const T1& first_a, const T2& second_a) :
        lims(first_a, second_a)
    {}

    /// Interval as a copy of another interval.
    template <typename T1>
    interval (const interval<T1>& x) :
        lims(x.lims.first, x.lims.second)
    {}

    /// Initialization from a string.
    /** The interval can represented as a string in shape "x", "(x)" or "(x, x)".
        See operator>> for details. */
    interval (const char* s)
    {
        std::istringstream buf(s);
        buf >> *this;
    }

    template <typename T1>
    interval& operator= (const T1& x)
    {
        lims.first = x;
        lims.second = x;
        return *this;
    }

    template <typename T1>
    interval& operator= (const interval<T1>& x)
    {
        lims.first = x.lims.first;
        lims.second = x.lims.second;
        return *this;
    }

    interval& operator= (const char* s)
    {
        std::istringstream buf(s);
        buf >> *this;
        return *this;
    }

    const T& first () const
    {
        return lims.first;
    }

    const T& left () const
    {
        return lims.first;
    }

    const T& second () const
    {
        return lims.second;
    }

    const T& right () const
    {
        return lims.second;
    }

    T& first ()
    {
        return lims.first;
    }

    T& left ()
    {
        return lims.first;
    }

    T& second ()
    {
        return lims.second;
    }

    T& right ()
    {
        return lims.second;
    }

    /// Return true iff left == rigth.
    bool is_point () const
    {
        return lims.first == lims.second;
    }

    /// Return true iff left > rigth.
    bool is_empty () const
    {
        return lims.first > lims.second;
    }

    /// Return true iff left >= right.
    bool is_negligible () const
    {
        return lims.first >= lims.second;
    }

    interval_kind kind () const
    {
        return interval_kind(cmp(lims.second, lims.first) + 1);
    }

    /// Length of the interval as a difference between right and left limts. It can be negative.
    typename binary_function_traits<function_tag::minus, T, T>::result_type
    length () const
    {
        return lims.second - lims.first;
    }

    /// Swaps left and right.
    void turn ()
    {
        swap(lims.first, lims.second);
    }

    template <typename T1>
    bool on_limits (const T1& x) const
    {
        return lims.first == x || lims.second == x;
    }

    /// True iff x is strictly inside of the interval/segment.
    template <typename T1>
    bool inside (const T1& x) const
    {
        return lims.first < x && x < lims.second;
    }

    /// True iff x is strictly outside of the interval/segment.
    template <typename T1>
    bool outside (const T1& x) const
    {
        return x < lims.first || lims.second < x;
    }

    template <typename T1>
    interval& operator+= (const interval<T1>& x)
    {
        return *this = *this + x;
    }

    template <typename T1>
    interval& operator-= (const interval<T1>& x)
    {
        return *this = *this - x;
    }

    template <typename T1>
    interval& operator*= (const interval<T1>& x)
    {
        return *this = *this * x;
    }

    template <typename T1>
    interval& operator/= (const interval<T1>& x)
    {
        return *this = *this / x;
    }

    interval& operator++ ()
    {
        return *this += interval(unit<T>());
    }

    interval& operator-- ()
    {
        return *this -= interval(unit<T>());
    }

    interval operator++ (int)
    {
        interval t = *this; operator++(); return t;
    }

    interval operator-- (int)
    {
        interval t = *this; operator--(); return t;
    }

    const interval& operator+ () const
    {
        return *this;
    }

    interval operator- () const
    {
        return interval(null<T>()) - *this;
    }
};


template <typename T>
interval<T> operator+ (const interval<T>& a, const interval<T>& b)
{
    // WARNING! There is simpler way for reals.

    T
        p1 = a.first() + b.first(),
        p2 = a.first() + b.second(),
        p3 = a.second() + b.first(),
        p4 = a.second() + b.second();

    return interval<T>
    (
        std::min(std::min(p1, p2), std::min(p3, p4)),
        std::max(std::max(p1, p2), std::max(p3, p4))
    );
}

template <typename T>
interval<T> operator- (const interval<T>& a, const interval<T>& b)
{
    // WARNING! There is simpler way for reals.

    T
        p1 = a.first() - b.first(),
        p2 = a.first() - b.second(),
        p3 = a.second() - b.first(),
        p4 = a.second() - b.second();

    return interval<T>
    (
        std::min(std::min(p1, p2), std::min(p3, p4)),
        std::max(std::max(p1, p2), std::max(p3, p4))
    );
}

template <typename T>
interval<T> operator* (const interval<T>& a, const interval<T>& b)
{
    // WARNING! There is simpler way for reals.

    T
        p1 = a.first() * b.first(),
        p2 = a.first() * b.second(),
        p3 = a.second() * b.first(),
        p4 = a.second() * b.second();

    return interval<T>
    (
        std::min(std::min(p1, p2), std::min(p3, p4)),
        std::max(std::max(p1, p2), std::max(p3, p4))
    );
}

template <typename T>
interval<T> operator/ (const interval<T>& a, const interval<T>& b)
{
    // WARNING! There is simpler way for reals.
    ARAGELI_ASSERT_0(sign(b.first())*sign(b.second()) > 0);

    T
        p1 = a.first() / b.first(),
        p2 = a.first() / b.second(),
        p3 = a.second() / b.first(),
        p4 = a.second() / b.second();

    return interval<T>
    (
        std::min(std::min(p1, p2), std::min(p3, p4)),
        std::max(std::max(p1, p2), std::max(p3, p4))
    );
}


template <typename T1, typename T2>
inline bool are_overlap_intervals_oooo (const T1& a, const T2& b)
{
    if(a.is_negligible() || b.is_negligible())
        return false;
    else
        return
            (a.first() < b.first()) ?
            (b.first < a.second) :
            (a.first < b.second);
}

/*
template <typename T1, typename T2>
inline bool is_overlap_interval_cooo (const T1& a, const T2& b)
{
    if(a.is_negligible() || b.is_negligible())return false;
    return
        (a.first() < b.first()) ? (b.first < a.second) : (a.first < b.second);
}
*/

template <typename Seg1, typename Seg2>
inline bool is_overlap_segs (const Seg1& a, const Seg2& b)
{
    return
        (a.first() <= b.first()) ?
        (b.first() <= a.second()) :
        (a.first() <= b.second());
}


template <typename Out, typename T>
Out& operator<< (Out& out, const interval<T>& x)
{
    out << "(" << x.first() << ", " << x.second() << ")";
    return out;
}

template <typename In, typename T>
In& operator>> (In& in, interval<T>& x)
{
    vector<T, false> buf;    // WARNING! TEMPORARY!
    in >> buf;
    if(!in && !in.eof() || buf.size() != 2)
        in.clear(std::ios::badbit);
    else
    {
        x.first() = buf[0];
        x.second() = buf[1];
    }

    return in;
}

/*
template <typename T1, typename T2, typename Outiter>
void generate_range_helper
(
    interval<T1>& t1,
    const interval<T2>& t2,
    Outiter outiter
)
{
    ARAGELI_ASSERT_ALWAYS(!"generate_range_helper is not implemented for interval");
}


template <typename T1, typename T2, typename T3, typename Outiter>
void generate_range_helper
(
    interval<T1>& t1,
    const interval<T2>& t2,
    const interval<T3>& t3,
    Outiter outiter
)
{
    ARAGELI_ASSERT_ALWAYS(!"generate_range_helper is not implemented for interval");
}
*/

/// Determines if a and b are exactly comparable as two open intervals.
template <typename T1, typename T2>
bool are_comparable_oooo (const interval<T1>& a, const interval<T2>& b)
{
    return !are_overlap_intervals_oooo(a, b);
}


/// Semi-lexicographical comparision between two intervals.
/** An empty interval less than all others. */
template <typename T1, typename T2>
int cmp (const interval<T1>& a, const interval<T2>& b)
{
    if(a.is_empty())
        if(b.is_empty())
            return 0;
        else
            return -1;
    else if(b.is_empty())
        return +1;

    ARAGELI_ASSERT_1(!a.is_empty() && !b.is_empty());

    if(a.left() < b.left())
        return -1;
    else if(a.left() > b.left())
        return +1;
    else if(a.right() < b.right())
        return -1;
    else if(a.right() > b.right())
        return +1;
    else
        return 0;
}


template <typename T1, typename T2>
bool operator< (const interval<T1>& a, const interval<T2>& b)
{
    if(a.is_empty() || b.is_empty())
        return true;
    else
        return a.right() < b.left();
}


template <typename T1, typename T2>
bool operator> (const interval<T1>& a, const interval<T2>& b)
{
    if(a.is_empty() || b.is_empty())
        return true;
    else
        return b.right() < a.left();
}


template <typename T1, typename T2>
bool operator<= (const interval<T1>& a, const interval<T2>& b)
{
    if(a.is_empty() || b.is_empty())
        return true;
    else
        return a.right() <= b.left();
}


template <typename T1, typename T2>
bool operator>= (const interval<T1>& a, const interval<T2>& b)
{
    if(a.is_empty() || b.is_empty())
        return true;
    else
        return b.right() <= a.left();
}


template <typename T1, typename T2>
bool operator== (const interval<T1>& a, const interval<T2>& b)
{
    if(a.is_empty() && b.is_empty())
        return true;
    else
        return a.left() == b.left() && a.right() == b.right();
}


template <typename T1, typename T2>
inline bool operator!= (const interval<T1>& a, const interval<T2>& b)
{
    return !(a == b);
}


template <typename T>
interval<T> abs (const interval<T>& x)
{
    if(x.is_empty())
        return x;

    if(!x.outside(null<T>()))
        return
            interval<T>
            (
                null<T>(),
                std::max(abs(x.left()), abs(x.right()))
            );
    else
    {
        T
            absleft = abs(x.left()),
            absright = abs(x.right());

        return
            interval<T>
            (
                std::min(absleft, absright),
                std::max(absleft, absright)
            );
    }
}




} // namesapce Arageli

namespace std
{

template <typename T>
inline Arageli::interval<T> abs (const Arageli::interval<T>& x)
{
    return Arageli::abs(x);
}

}


#if 0
#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_INTERVAL
    #include "interval.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_INTERVAL
#endif
#endif

#endif
