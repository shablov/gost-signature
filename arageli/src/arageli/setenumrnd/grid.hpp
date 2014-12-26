/*****************************************************************************

    setenumrnd/grid.hpp

    This file is a part of the Arageli library.

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
    \file grid.hpp
    \brief Grid on a continuous set as a set.
*/


#ifndef _ARAGELI_setenumrnd_grid_hpp_
#define _ARAGELI_setenumrnd_grid_hpp_

#include <cstddef>

#include "../config.hpp"

#include "../type_pair_traits.hpp"
#include "../misc.hpp"
#include "../rndbase.hpp"
#include "../setenumrnd.hpp"
#include "integer.hpp"


namespace Arageli
{

namespace set
{

/// One dimensional grid.
/** WARNING! Temporary parameter N is std::size_t by the default.
    In the correct implementation it depends on T also. */
template <typename T, typename N = std::size_t>
class grid1
{
public:

    typedef T value_type;

    /// Creates an empty set.
    grid1 () :
        min_m(unit<T>()),
        max_m(null<T>()),
        step_m(null<T>())
    {}

    typedef N cardinality_type;

    /// Creates grid { x | x = min_a + i*step_a, x <= max_a, i in Z+ }.
    grid1
    (
        const value_type& min_a,
        const value_type& max_a,
        const value_type& step_a = unit<value_type>()
    ) :
        min_m(min_a),
        max_m(max_a),
        step_m(step_a)
    {}

    const value_type& min () const
    {
        return min_m;
    }

    const value_type& max () const
    {
        return max_m;
    }

    const value_type& step () const
    {
        return step_m;
    }

    cardinality_type cardinality () const
    {
        return (max_m - min_m)/step_m + unit<N>();
    }

private:

    value_type min_m, max_m, step_m;
};


template <typename T>
inline grid1<T> make_grid1
(
    const T& min,
    const T& max,
    const T& step = unit<T>()
)
{
    return grid1<T>(min, max, step);
}


} // namespace set


namespace enumer
{

// WARNING! JUST COPY-PASTE FROM integer.hpp FILE WITH LITTLE MODIFICATION

/// Enumerates elements in grid.
template <typename T, typename N>
class fast<set::grid1<T, N> > :
    public std::iterator
    <
        std::input_iterator_tag,
        const T,
        typename signed_type<N>::type,
        typename unsigned_type<N>::type
    >
{
public:

    typedef set::grid1<T> set_type;

    /// Creates iterator at the begin.
    fast (const set::grid1<T>& set_a) :
        set_m(&set_a),
        cur(set_a.min()),
        is_end_m(set_a.min() > set_a.max())
    {}

    /// Creates iterator at the begin.
    fast (const begin_t&, const set::grid1<T>& set_a) :
        set_m(&set_a),
        cur(set_a.min()),
        is_end_m(set_a.min() > set_a.max())
    {}

    /// Creates iterator at the end.
    fast (const end_t&, const set::grid1<T>& set_a) :
        set_m(&set_a),
        is_end_m(true)
    {}

    /// Returns true if this iterator points max+1 value.
    bool is_end () const
    {
        if(!is_end_m)
            is_end_m = (cur > set_m->max());
        return is_end_m;
    }

    fast& operator++ ()
    {
        cur += set_m->step();
        return *this;
    }

    fast operator++ (int)
    {
        fast t = *this;
        operator++();
        return t;
    }

    const T* operator->() const
    {
        return &cur;
    }

    const T& operator*() const
    {
        return cur;
    }

    void reset ()
    {
        cur = set_m->min();
        is_end_m = (set_m->min() > set_m->max());
    }

private:

    const set_type* set_m;
    T cur;    ///< current value
    mutable bool is_end_m;

};


// TODO Complete iterator implementation.

template <typename T, typename N>
inline bool operator==
(
    const fast<set::grid1<T, N> >& a,
    const fast<set::grid1<T, N> >& b
)
{
    return
        a.is_end() ?
        b.is_end() :
        (b.is_end() ? a.is_end() : *a == *b);
}


template <typename T, typename N>
inline bool operator!=
(
    const fast<set::grid1<T, N> >& a,
    const fast<set::grid1<T, N> >& b
)
{
    return !(a == b);
}


/// Enumerates first natural numbers from 1 to max in usual order.
template <typename T, typename N>
class ordered<set::grid1<T, N> > :
    public fast<set::grid1<T, N> >
{
public:

    /// Creates iterator on 1.
    ordered (const set::grid1<T, N>& set_a) :
        fast<set::grid1<T, N> >(set_a)
    {}

    /// Creates iterator on 1.
    ordered (const begin_t&, const set::grid1<T, N>& set_a) :
        fast<set::grid1<T, N> >(begin, set_a)
    {}

    /// Creates iterator on max+1 number.
    ordered (const end_t&, const set::grid1<T, N>& set_a) :
        fast<set::grid1<T, N> >(end, set_a)
    {}

    // TODO Complete iterator implementation.

    ordered& operator++ ()
    {
        fast<set::grid1<T, N> >::operator++();
        return *this;
    }

    ordered& operator-- ()
    {
        fast<set::grid1<T, N> >::operator--();
        return *this;
    }

    ordered operator++ (int)
    {
        ordered t = *this;
        operator++();
        return t;
    }

    ordered operator-- (int)
    {
        ordered t = *this;
        operator--();
        return t;
    }
};


} // namespace enumer


namespace rnd
{

/// Generates element from the grid with equiprobable outcome.
/** Set cannot be empty. */
template <typename T, typename N, typename Base>
class equiprob<set::grid1<T, N>, Base>
{
    typedef equiprob<set::inonnegative<N>, Base> Base2;

public:

    /// Type of base random generator.
    typedef Base base_type;

    /// Type of set from which elements is being generated.
    typedef set::grid1<T, N> set_type;

    /// Type of generated values.
    typedef T value_type;

    /// Type of seed for random generator.
    typedef typename Base2::seed_type seed_type;

    /// Type representing the cardinality.
    typedef N cardinality_type;

    /// Type representing the period.
    typedef typename Base2::period_type period_type;

    equiprob (const set_type& set_a) :
        set_m(&set_a),
        base2_set(init_value()),
        base2(base2_set)
    {
        ARAGELI_ASSERT_0(set_m->min() <= set_m->max());
        ARAGELI_ASSERT_0(std::numeric_limits<N>::is_integer);
    }

    equiprob (seed_type seed, const set_type& set_a) :
        set_m(&set_a),
        base2_set(init_value()),
        base2(seed, base2_set)
    {
        ARAGELI_ASSERT_0(set_m->min() <= set_m->max());
        ARAGELI_ASSERT_0(std::numeric_limits<N>::is_integer);
    }

    equiprob (const Base& base_a, const set_type& set_a) :
        set_m(&set_a),
        base2_set(init_value()),
        base2(base_a, base2_set)
    {
        ARAGELI_ASSERT_0(set_m->min() <= set_m->max());
        ARAGELI_ASSERT_0(std::numeric_limits<N>::is_integer);
    }

    cardinality_type cardinality ()
    {
        return set_m->max();
    }

    period_type period ()
    {
        return base2.period();
    }

    value_type min ()
    {
        return set_m->min();
    }

    value_type max ()
    {
        return set_m->max();
    }

    value_type operator() () const
    {
        return set_m->min() + base2()*set_m->step();
    }

    void reset ()
    {
        base2.reset();
    }

    void reset (const seed_type& seed)
    {
        base2.reset(seed);
    }

private:

    // Do not change an order of the following declarations.
    const set_type* set_m;
    set::inonnegative<N> base2_set;
    Base2 base2;

    N init_value () const
    { return set_m->cardinality() - unit<N>(); }

};


} // namespace rnd

} // namesapce Arageli


// UNCOMMENT THE FOLLOWING FIVE LINES IF THERE IS grid.cpp FILE
//#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
//    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_setenumrnd_grid
//    #include "grid.cpp"
//    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_setenumrnd_grid
//#endif

#endif    // #ifndef _ARAGELI_setenumrnd_grid_hpp_
