/*****************************************************************************

    setenumrnd/integer.hpp

    This file is a part of the Arageli library.

    Copyright (C) 2006--2008 Sergey S. Lyalin

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
    \file integer.hpp
    \brief Several simple integer sets (ranges) with random generators.
*/


#ifndef _ARAGELI_setenumrnd_integer_hpp_
#define _ARAGELI_setenumrnd_integer_hpp_

#include "../config.hpp"

#include <map>
#include <utility>

#include "../type_pair_traits.hpp"
#include "../misc.hpp"
#include "../rndbase.hpp"
#include "../setenumrnd.hpp"
#include "../big_int.hpp"   // for temporary solution with large random numbers only

namespace Arageli
{

namespace set
{

/// All natural numbers from 1 to particular value.
/** To represent an empty set, T should be able to represent 0. */
template <typename T>
class ipositive
{
public:

    /// Type of values.
    typedef T value_type;

    typedef T cardinality_type;

    /// Construct an empty set. Equivalent to ipositive(0).
    ipositive () :
        max_m(null<T>())
    {}

    /// Set of naturals from 1 to max_a (included).
    /** If 0 passed, it's equivalent to ipositive() constructor, i.e.
        constructs an empty set. */
    ipositive (const value_type& max_a) :
        max_m(max_a)
    {}

    /// Maximum number. If 0 then the set is empty.
    const T& max () const
    {
        return max_m;
    }

    const cardinality_type& cardinality () const
    {
        return max_m;
    }

private:

    T max_m;
};


template <typename T>
inline ipositive<T> make_ipositive (const T& max)
{
    return ipositive<T>(max);
}


/// All integer numbers from 0 to particular value.
/** To represent an empty set, T should be able to represent -1. */
template <typename T>
class inonnegative
{
public:

    /// Type of values.
    typedef T value_type;

    typedef T cardinality_type;

    /// Construct an empty set. Equivalent to inonnegative(-1).
    inonnegative () :
        max_m(opposite_unit<T>())
    {}

    /// Set of integers from 0 to max_a (included).
    /** If -1 passed, it's equivalent to inonnegative() constructor, i.e.
        constructs an empty set. */
    inonnegative (const value_type& max_a) :
        max_m(max_a)
    {}

    /// Maximum number. If -1 then the set is empty.
    const T& max () const
    {
        return max_m;
    }

    cardinality_type cardinality () const
    {
        return max_m+1;
    }

private:

    T max_m;
};


template <typename T>
inline inonnegative<T> make_inonnegative (const T& max)
{
    return inonnegative<T>(max);
}


/// All natural numbers representable as a given type.
template <typename T>
class natural
{
public:

    /// Type of values.
    typedef T value_type;
};


#if 0

    // TODO Move this definition to setenumrnd/interval header.
    /// All elements from some range [min, max].
    /** All values x that is satisfied expression min <= x <= max, where
        min and max are some values of type T. An empty set is represented
        of the case when a > b. Type of values can be different:
        from integers to some partially ordered set. */
    template <typename T>
    class segment
    {
    public:

        /// Type of values.
        typedef T value_type;

        /// Construct an empty set. Equivalent to segment(1, 0).
        segment () :
            min_m(unit<T>()),
            max_m(null<T>())
        {}

        /// Set of values from [0, max_a].
        segment (const value_type& max_a) :
            min_a(null<T>()),
            max_m(max_a)
        {}

        /// Set of values from [min_a, max_a].
        segment (const value_type& min_a, const value_type& max_a) :
            min_m(min_a),
            max_m(max_a)
        {}

        /// Minimum element.
        const T& min () const
        {
            return max_m;
        }

        /// Maximum element.
        const T& max () const
        {
            return max_m;
        }

    private:

        T min_m, max_m;
    };

#endif

} // namespace set


namespace enumer
{

/// Enumerates first natural numbers.
/** In addition to set::ipositive requirements T should can
    represent max + 1 value. */
template <typename T>
class fast<set::ipositive<T> > :
    public std::iterator
    <
        std::bidirectional_iterator_tag,    // WARNING! TEMPORARY.
        const T,
        typename signed_type<T>::type,
        typename unsigned_type<T>::type
    >
{
public:

    typedef set::ipositive<T> set_type;

    /// Creates iterator on 1.
    fast (const set::ipositive<T>& set_a) :
        set_m(&set_a),
        cur(unit<T>())
    {}

    /// Creates iterator on 1.
    fast (const begin_t&, const set::ipositive<T>& set_a) :
        set_m(&set_a),
        cur(unit<T>())
    {}

    /// Creates iterator on max+1 number.
    fast (const end_t&, const set::ipositive<T>& set_a) :
        set_m(&set_a),
        cur(set_a.max() + 1)
    {}

    /// Returns true if this iterator points max+1 value.
    bool is_end () const
    {
        return cur == set_m->max() + 1;
    }

    /// Poses this iterator at the begin.
    void reset ()
    {
        cur = unit<T>();
    }

    fast& operator++ ()
    {
        ++cur;
        return *this;
    }

    fast& operator-- ()
    {
        --cur;
        return *this;
    }

    fast operator++ (int)
    {
        fast t = *this;
        operator++();
        return t;
    }

    fast operator-- (int)
    {
        fast t = *this;
        operator--();
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

private:

    const set_type* set_m;
    T cur;    ///< current value
};


template <typename T>
inline bool operator==
(
    const fast<set::ipositive<T> >& a,
    const fast<set::ipositive<T> >& b
)
{
    return *a == *b;
}

template <typename T>
inline bool operator!=
(
    const fast<set::ipositive<T> >& a,
    const fast<set::ipositive<T> >& b
)
{
    return *a != *b;
}


/// Enumerates first natural numbers from 1 to max in usual order.
template <typename T>
class ordered<set::ipositive<T> > :
    public fast<set::ipositive<T> >
{
public:

    /// Creates iterator on 1.
    ordered (const set::ipositive<T>& set_a) :
        fast<set::ipositive<T> >(set_a)
    {}

    /// Creates iterator on 1.
    ordered (const begin_t&, const set::ipositive<T>& set_a) :
        fast<set::ipositive<T> >(begin, set_a)
    {}

    /// Creates iterator on max+1 number.
    ordered (const end_t&, const set::ipositive<T>& set_a) :
        fast<set::ipositive<T> >(end, set_a)
    {}

    ordered& operator++ ()
    {
        fast<set::ipositive<T> >::operator++();
        return *this;
    }

    ordered& operator-- ()
    {
        fast<set::ipositive<T> >::operator--();
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


/// Enumerates first nonnegative integer numbers.
/** In addition to set::inonnegative requirements T should can
    represent max + 1 value. */
template <typename T>
class fast<set::inonnegative<T> > :
    public std::iterator
    <
        std::bidirectional_iterator_tag,    // WARNING! TEMPORARY.
        const T,
        typename signed_type<T>::type,
        typename unsigned_type<T>::type
    >
{
public:

    typedef set::inonnegative<T> set_type;

    /// Creates iterator on 0.
    fast (const set::inonnegative<T>& set_a) :
        set_m(&set_a),
        cur(null<T>())
    {}

    /// Creates iterator on 1.
    fast (const begin_t&, const set::inonnegative<T>& set_a) :
        set_m(&set_a),
        cur(null<T>())
    {}

    /// Creates iterator on max+1 number.
    fast (const end_t&, const set::inonnegative<T>& set_a) :
        set_m(&set_a),
        cur(set_a.max() + 1)
    {}

    /// Returns true if this iterator points max+1 value.
    bool is_end () const
    {
        return cur == set_m->max() + 1;
    }

    /// Poses this iterator at the begin.
    void reset ()
    {
        cur = null<T>();
    }

    fast& operator++ ()
    {
        ++cur;
        return *this;
    }

    fast& operator-- ()
    {
        --cur;
        return *this;
    }

    fast operator++ (int)
    {
        fast t = *this;
        operator++();
        return t;
    }

    fast operator-- (int)
    {
        fast t = *this;
        operator--();
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

private:

    const set_type* set_m;
    T cur;    ///< current value
};


template <typename T>
inline bool operator==
(
    const fast<set::inonnegative<T> >& a,
    const fast<set::inonnegative<T> >& b
)
{
    return *a == *b;
}

template <typename T>
inline bool operator!=
(
    const fast<set::inonnegative<T> >& a,
    const fast<set::inonnegative<T> >& b
)
{
    return *a != *b;
}


/// Enumerates first nonnegative integer numbers.
template <typename T>
class ordered<set::inonnegative<T> > :
    public fast<set::inonnegative<T> >
{
public:

    /// Creates iterator on 1.
    ordered (const set::inonnegative<T>& set_a) :
        fast<set::inonnegative<T> >(set_a)
    {}

    /// Creates iterator on 1.
    ordered (const begin_t&, const set::inonnegative<T>& set_a) :
        fast<set::inonnegative<T> >(begin, set_a)
    {}

    /// Creates iterator on max+1 number.
    ordered (const end_t&, const set::inonnegative<T>& set_a) :
        fast<set::inonnegative<T> >(end, set_a)
    {}

    ordered& operator++ ()
    {
        fast<set::inonnegative<T> >::operator++();
        return *this;
    }

    ordered& operator-- ()
    {
        fast<set::inonnegative<T> >::operator--();
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

/// For internal use.
namespace _Internal
{

template <class Gen, class BRGen, class Power>
struct Comppars
{
    Comppars () {}

    Comppars (const Power& pw, const Gen& A, const Gen& B)
    {
        assign(pw, A, B);
    }

    void assign (const Power& pw, const Gen& A, const Gen& B)
    {
        BRGen _Rp1 = pw % (BRGen(B - A) + 1) + 1;
        Z = pw - ( (_Rp1 == (BRGen(B - A) + 1)) ? 0 : _Rp1 );
        Q = pw / BRGen(B - A + 1) + (_Rp1 == (BRGen(B - A) + 1));
    }

    BRGen _Rp1, Z, Q;
};

} // namespace _Internal

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4715)
#endif

/// Generates natural number x <= max with equiprobable outcome.
/** Set cannot be empty. If the cardinality of the base generator is less
    than the target cardinality, current implementation cannot guarantee that
    all target values will be generated. */
template <typename T, typename Base>
class equiprob<set::ipositive<T>, Base>
{
    typedef typename power_union_type
    <
        T,
        typename Base::value_type
    >::type PT;

public:

    /// Type of base random generator.
    typedef Base base_type;

    /// Type of set from which elements is being generated.
    typedef set::ipositive<T> set_type;

    /// Type of generated values.
    typedef T value_type;

    /// Type of seed for random generator.
    typedef typename Base::seed_type seed_type;

    /// Type representing the cardinality.
    typedef T cardinality_type;

    /// Type representing the period.
    typedef typename Base::period_type period_type;

    equiprob (const set_type& set_a) :
        set_m(&set_a)
        /*,cp(base_m.cardinality(), unit<T>(), set_a.max())*/
    {
        ARAGELI_ASSERT_0(!is_null(set_m->max()));
        // WARNING! TEMPORARY LIMITATION
        ARAGELI_ASSERT_0(is_null(base_m.min()));

        relation_init();
    }

    equiprob (seed_type seed, const set_type& set_a) :
        base_m(seed),
        set_m(&set_a)
        /*,cp(base_m.cardinality(), unit<T>(), set_a.max())*/
    {
        ARAGELI_ASSERT_0(!is_null(set_m->max()));
        // WARNING! TEMPORARY LIMITATION
        ARAGELI_ASSERT_0(is_null(base_m.min()));

        relation_init();
    }

    equiprob (const base_type& base_a, const set_type& set_a) :
        base_m(base_a),
        set_m(&set_a)
    {
        ARAGELI_ASSERT_0(!is_null(set_m->max()));
        // WARNING! TEMPORARY LIMITATION
        ARAGELI_ASSERT_0(is_null(base_m.min()));

        relation_init();
    }

    cardinality_type cardinality () const
    {
        return set_m->max();
    }

    period_type period () const
    {
        return base_m.period();
    }

    value_type min () const
    {
        return unit<value_type>();
    }

    value_type max () const
    {
        return set_m->max();
    }

    value_type operator() () const
    {
        switch(relation)
        {
            case BASE_EQUAL:
                return value_type(base_m()) + 1;
            case BASE_GREATER:
            {
                if(is_unit(set_m->max()))
                    return set_m->max();
                typename Base::value_type val = base_m();
                while(val > cp.Z)
                    val = base_m();
                return value_type(val / cp.Q) + 1;
            }
            case BASE_LESS:
            {
                // WARNING! TEMPORARY SOLUTION.
                // We just call random generator for big_int and convert the result to type T.

                return value_type(big_int::random_in_range(big_int(max()) - 1) + 1);
            }
            default:
                ARAGELI_ASSERT_1(!"It's impossible.");
        }
    }

    void reset ()
    {
        base_m.reset();
    }

    void reset (const seed_type& seed)
    {
        base_m.reset(seed);
    }

private:

    Base base_m;
    const set_type* set_m;
    enum
    {
        BASE_LESS,
        BASE_EQUAL,
        BASE_GREATER
    } relation;

    _Internal::Comppars
    <
        value_type,
        typename Base::value_type,
        typename Base::cardinality_type
    > cp;

    void relation_init ()
    {
        T maxm1 = set_m->max() - 1;
        if(base_m.max() > maxm1)
        {
            relation = BASE_GREATER;
            cp.assign(base_m.cardinality(), unit<T>(), set_m->max());
        }
        else if(base_m.max() == maxm1)
            relation = BASE_EQUAL;
        else
            relation = BASE_LESS;
    }
};

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4715)
#endif

/// Generates integer number 0 <= x <= max with equiprobable outcome.
/** Set cannot be empty. If the cardinality of the base generator is less
    than the target cardinality, current implementation cannot guarantee that
    all target values will be generated. */
template <typename T, typename Base>
class equiprob<set::inonnegative<T>, Base>
{
    typedef typename power_union_type
    <
        T,
        typename Base::value_type
    >::type PT;

public:

    /// Type of base random generator.
    typedef Base base_type;

    /// Type of set from which elements is being generated.
    typedef set::inonnegative<T> set_type;

    /// Type of generated values.
    typedef T value_type;

    /// Type of seed for random generator.
    typedef typename Base::seed_type seed_type;

    /// Type representing the cardinality.
    typedef T cardinality_type;

    /// Type representing the period.
    typedef typename Base::period_type period_type;

    equiprob (const set_type& set_a) :
        set_m(&set_a)
        /*,cp(base_m.cardinality(), null<T>(), set_a.max())*/
    {
        ARAGELI_ASSERT_0(!is_opposite_unit(set_m->max()));
        // WARNING! TEMPORARY LIMITATION
        ARAGELI_ASSERT_0(is_null(base_m.min()));

        relation_init();
    }

    equiprob (seed_type seed, const set_type& set_a) :
        base_m(seed),
        set_m(&set_a)
        /*,cp(base_m.cardinality(), null<T>(), set_a.max())*/
    {
        ARAGELI_ASSERT_0(!is_opposite_unit(set_m->max()));
        // WARNING! TEMPORARY LIMITATION
        ARAGELI_ASSERT_0(is_null(base_m.min()));

        relation_init();
    }

    equiprob (const Base& base_a, const set_type& set_a) :
        base_m(base_a),
        set_m(&set_a)
        /*,cp(base_m.cardinality(), null<T>(), set_a.max())*/
    {
        ARAGELI_ASSERT_0(!is_opposite_unit(set_m->max()));
        // WARNING! TEMPORARY LIMITATION
        ARAGELI_ASSERT_0(is_null(base_m.min()));

        relation_init();
    }

    cardinality_type cardinality () const
    {
        return set_m->max();
    }

    period_type period () const
    {
        return base_m.period();
    }

    value_type min () const
    {
        return null<value_type>();
    }

    value_type max () const
    {
        return set_m->max();
    }

    value_type operator() () const
    {
        switch(relation)
        {
            case BASE_EQUAL:
                return base_m();
            case BASE_GREATER:
            {
                if(is_null(set_m->max()))
                    return set_m->max();
                typename Base::value_type val = base_m();
                while(val > cp.Z)
                    val = base_m();
                return val / cp.Q;
            }
            case BASE_LESS:
            {
                // WARNING! TEMPORARY SOLUTION.
                // We just call random generator for big_int and convert the result to type T.

                return value_type(big_int::random_in_range(max()));
            }
            default:
                ARAGELI_ASSERT_1(!"It's impossible.");
        }
    }

    void reset ()
    {
        base_m.reset();
    }

    void reset (const seed_type& seed)
    {
        base_m.reset(seed);
    }

private:

    Base base_m;
    const set_type* set_m;
    enum
    {
        BASE_LESS,
        BASE_EQUAL,
        BASE_GREATER
    } relation;

    _Internal::Comppars
    <
        value_type,
        typename Base::value_type,
        typename Base::cardinality_type
    > cp;

    void relation_init ()
    {
        if(base_m.max() > set_m->max())
        {
            relation = BASE_GREATER;
            cp.assign(base_m.cardinality(), null<T>(), set_m->max());
        }
        else if(base_m.max() == set_m->max())
            relation = BASE_EQUAL;
        else
        {
            relation = BASE_LESS;
            // WARNING! IT IS NOT IMPLEMENTED!!!
        }
    }
};


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

/// Generates integer number 0 <= x <= max with equiprobable outcome.
/** Parameters of the set from which the numbers are generated are set
    when the generator calls. If the cardinality of the base generator is less
    than target cardinality, current implementation cannot guarantee that
    all target values will be generated. */
template <typename T, typename Base>
class equiprob_alter<set::inonnegative<T>, Base>
{
    typedef typename power_union_type
    <
        T,
        typename Base::value_type
    >::type PT;

public:

    /// Type of base random generator.
    typedef Base base_type;

    /// Type of set from which elements are being generated.
    typedef set::inonnegative<T> set_type;

    /// Type of generated values.
    typedef T value_type;

    /// Type of seed for random generator.
    typedef typename Base::seed_type seed_type;

    /// Type representing the cardinality.
    typedef T cardinality_type;

    /// Type representing the period.
    typedef typename Base::period_type period_type;

    equiprob_alter () {}

    equiprob_alter (seed_type seed) :
        base_m(seed)
    {
        // WARNING! TEMPORARY LIMITATION
        ARAGELI_ASSERT_0(is_null(base_m.min()));
    }

    equiprob_alter (const Base& base_a) :
        base_m(base_a)
    {
        // WARNING! TEMPORARY LIMITATION
        ARAGELI_ASSERT_0(is_null(base_m.min()));
    }

    period_type period ()
    {
        return base_m.period();
    }

    value_type min ()
    {
        return null<value_type>();
    }

    value_type operator() (const value_type& max) const
    {
        ARAGELI_ASSERT_0(!is_negative(max));
        ARAGELI_ASSERT_0(base_m.max() >= max);

        if(is_null(max))return max;

        if(base_m.max() > max)
        {
            _Internal::Comppars
            <
                value_type,
                typename Base::value_type,
                typename Base::cardinality_type
            > cp;

            cp.assign(base_m.cardinality(), null<T>(), max);
            typename Base::value_type val = base_m();
            while(val > cp.Z)
                val = base_m();
            return val / cp.Q;
        }

        ARAGELI_ASSERT_1(base_m.max() == max);
        return base_m();
    }

    void reset ()
    {
        base_m.reset();
    }

    void reset (const seed_type& seed)
    {
        base_m.reset(seed);
    }

private:

    Base base_m;
};


/////////////////
// TODO: Relocate!
template <typename T1>
struct pair_first_cmp
{
    T1 x;

    pair_first_cmp (const T1& xa) :
        x(xa)
    {}

    bool operator() (const std::pair<T1, T1>& x1) const
    {
        return x1.first >= x;
    }
};
/////////////////


/// Random subset with fixed number of elements.
/** WARNING! Partially implemented. */
template <typename T, typename SST, typename N, typename Base>
class equiprob<set::subsets_fixnum<set::inonnegative<T>, SST, N>, Base>
{
    // To choose the indeces for elements in set::inonnegative.
    typedef equiprob_alter<set::inonnegative<T>, Base> Base2;

public:

    /// Type of base random generator.
    typedef Base base_type;

    /// Type of set from which elements are being generated.
    typedef set::subsets_fixnum<set::inonnegative<T>, SST> set_type;

    /// Type of generated values.
    typedef SST value_type;

    /// Type of seed for random generator.
    typedef typename Base2::seed_type seed_type;

    equiprob (const set_type& set_a) :
        set_m(&set_a)
    {}

    equiprob (seed_type seed, const set_type& set_a) :
        base2(seed),
        set_m(&set_a)
    {}

    equiprob (const Base& base_a, const set_type& set_a) :
        base2(base_a),
        set_m(&set_a)
    {}

    value_type operator() () const
    {
        // We suppose value_type is a vector.

        N n = set_m->subset_cardinality();
        value_type res(n);

#if 0    // AN OLD VARIANT
        // The elements in 'selected' represent already selected
        // elements (first) in base set and the number of selected elements
        // that less (second) for each element.
        typedef std::map<T, N> Selected;
        Selected selected;
        typedef typename Selected::iterator SelI;

        for(N i = null<N>(); i < n; ++i)
        {
            T cur = base2(set_m->base_set().max() - i);
            ARAGELI_ASSERT_1(cur <= set_m->base_set().max() - i);
            ARAGELI_ASSERT_1(!is_negative(cur));
            N prev = null<N>();

            if(i == n/2)
            {
                std::cout << "STOP: i = " << i;
                std::cin.get();
            }

            int inbits = Arageli::nbits(i);
            int ratio = inbits;
            SelI iter = selected.begin();
            if(!selected.empty())for(;;)
            {
                if(ratio >= inbits)
                    iter = selected.lower_bound(cur + prev);
                else
                    iter = std::find_if
                    (
                        iter,
                        selected.end(),
                        pair_first_cmp<int>(cur + prev)
                    );

                ARAGELI_ASSERT_1(iter != selected.begin() || is_null(prev));

                if
                (
                    iter == selected.begin() &&
                    iter->first != cur
                )
                {
                    ARAGELI_ASSERT_1(cur < iter->first);
                    break;
                }

                if(iter == selected.end() || iter->first != cur + prev)
                    --iter;

                if(iter->second + unit<N>() == prev)
                    break;

                ratio = iter->second + unit<N>() - prev;
                ARAGELI_ASSERT_1(ratio >= 0);
                prev = iter->second + unit<N>();
            }

            cur += prev;
            ARAGELI_ASSERT_1(cur <= set_m->base_set().max());
            std::pair<Selected::iterator, bool> insinfo =
                selected.insert(std::make_pair(cur, prev));
            ARAGELI_ASSERT_1(insinfo.second);    // sure the insert occurs

            // increase the second component for each greater
            ++insinfo.first;
            for(; insinfo.first != selected.end(); ++insinfo.first)
                insinfo.first->second++;
        }

        ARAGELI_ASSERT_1(selected.size() == n);

        SelI seli = selected.begin();
        for
        (
            typename value_type::size_type i = 0;
            i < res.size();
            ++i, ++seli
        )
        {
            res[i] = seli->first;
            ARAGELI_ASSERT_1(seli->second == i);    // WARNING: are types in == compatible?
        }

#else

        int m = 0;    // the number of already selected elements
        for(int i = 0; i <= set_m->base_set().max() && m < n; ++i)
            if(base2(set_m->base_set().max() - i) < n - m)    // WARNING! PROOF!
                res[m++] = i;

#endif

        ARAGELI_ASSERT_1(m == n && res.size() == n);
        return res;
    }

private:

    Base2 base2;
    const set_type* set_m;
};



} // namespace rnd


//namespace set
//{
//
///// All subsets of nonpositive integers with the const number of elements.
//template <typename T, typename SST>
//struct subsets_fixnum<set::inonnegative<T>, SST>
//{
//    /// The base set for subsets.
//    typedef ST base_set_type;
//
//    /// The class representing a particular subset.
//    typedef SST value_type;
//};
//
//
//}


} // namesapce Arageli


// UNCOMMENT THE FOLLOWING FIVE LINES IF THERE IS integer.cpp FILE
//#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
//    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_setenumrnd_integer
//    #include "integer.cpp"
//    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_setenumrnd_integer
//#endif

#endif    // #ifndef _ARAGELI_setenumrnd_integer_hpp_
