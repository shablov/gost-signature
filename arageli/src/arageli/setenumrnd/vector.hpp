/*****************************************************************************

    setenumrnd/vector.hpp

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
    \file vector.hpp
    \brief Set of vector with particular characteristics.
*/


#ifndef _ARAGELI_setenumrnd_vector_hpp_
#define _ARAGELI_setenumrnd_vector_hpp_

#include <cstddef>
#include <algorithm>

#include <iostream>    // WARNING! FOR DEBUGGING ONLY!

#include "../config.hpp"

#include "../type_pair_traits.hpp"
#include "../misc.hpp"
#include "../powerest.hpp"
#include "../rndbase.hpp"
#include "../setenumrnd.hpp"
#include "integer.hpp"
#include "../vector.hpp"
#include "integer.hpp"


namespace Arageli
{

namespace set
{

/// Set of vectors of some fixed size with elements from specified set.
/** An empty set cannon be represented as this structure.
    And the set which elements from cannon be empty too. */
template <typename V, typename ElSet>
class vector_fixed_size
{
public:

    typedef V value_type;
    typedef typename V::size_type size_type;
    typedef ElSet element_set_type;

    typedef typename power_power_type
        <size_type, typename ElSet::cardinality_type>::type
            cardinality_type;

    /// Creates set with only one element --- empty vector.
    vector_fixed_size () :
        size_m(0)
    {}

    /// Creates set with all vectors of given size.
    vector_fixed_size (const size_type& size_a) :
        size_m(size_a)
    {}

    /// Creates set with all vectors of given size.
    vector_fixed_size (const size_type& size_a, const element_set_type& es) :
        size_m(size_a),
        element_set_m(es)
    {}

    const element_set_type& element_set () const
    {
        return element_set_m;
    }

    size_type size () const
    {
        return size_m;
    }

    cardinality_type cardinality () const
    {
        return power
        (
            cardinality_type(element_set_m.cardinality()),
            size_m
        );
    }

private:

    element_set_type element_set_m;
    size_type size_m;
};


template <typename V, typename ElSet>
inline vector_fixed_size<V, ElSet> make_vector_fixed_size
(const typename V::size_type& size, const ElSet& es)
{
    return vector_fixed_size<V, ElSet>(size, es);
}


/// Set of vectors sized upto particular size with elements from specified set.
/** An empty set cannon be represented as this structure.
    And the set which elements from cannon be empty too. */
template <typename V, typename ElSet>
class vector_upto_size
{
public:

    typedef V value_type;
    typedef typename V::size_type size_type;
    typedef ElSet element_set_type;

    typedef typename power_power_type
        <size_type, typename ElSet::cardinality_type>::type
            cardinality_type;

    /// Creates set with only one element --- empty vector.
    vector_upto_size () :
        size_m(0)
    {}

    /// Creates set with all vectors sized upto given size.
    vector_upto_size (const size_type& size_a) :
        size_m(size_a)
    {}

    /// Creates set with all vectors sized upto given size.
    vector_upto_size (const size_type& size_a, const element_set_type& es) :
        size_m(size_a),
        element_set_m(es)
    {}

    const element_set_type& element_set () const
    {
        return element_set_m;
    }

    size_type size () const
    {
        return size_m;
    }

    cardinality_type cardinality () const
    {
        cardinality_type esc = element_set_m.cardinality();
        if(is_null(esc))
            return unit<cardinality_type>();
        else if(is_unit(esc))
            return size_m + 1;
        else
            return (power(esc, size_m + 1) - 1)/(esc - 1);
    }

private:

    element_set_type element_set_m;
    size_type size_m;
};


template <typename V, typename ElSet>
inline vector_upto_size<V, ElSet> make_vector_upto_size
(const typename V::size_type& size, const ElSet& es)
{
    return vector_upto_size<V, ElSet>(size, es);
}


/// Creates set of given type from set of vectors.
template <typename T, typename VSet>
class byvector
{
public:

    typedef T value_type;
    typedef VSet base_set_type;
    typedef typename VSet::cardinality_type cardinality_type;

    byvector () {}

    byvector (const VSet& base_set_a) :
        base_set_m(base_set_a)
    {}

    const base_set_type& base_set () const
    {
        return base_set_m;
    }

    cardinality_type cardinality () const
    {
        return base_set_m.cardinality();
    }

private:

    base_set_type base_set_m;
};


template <typename T, typename VSet>
inline byvector<T, VSet> make_byvector (const VSet& vs)
{
    return byvector<T, VSet>(vs);
}



} // namespace set


namespace enumer
{

template <typename V, typename ElSet>
class fast<set::vector_fixed_size<V, ElSet> > :
    public std::iterator
    <
        std::input_iterator_tag,
        const V,
        typename signed_type
        <
            typename set::vector_fixed_size<V,ElSet>::cardinality_type
        >::type,
        typename unsigned_type
        <
            typename set::vector_fixed_size<V, ElSet>::cardinality_type
        >::type
    >
{
public:

    typedef set::vector_fixed_size<V, ElSet> set_type;

    fast (const set_type& set_a) :
        set_m(&set_a),
        cur(set_a.size()),
        icur(set_a.size(), set_a.element_set(), fromval),
        is_end_m(false)
    {
        if(!cur.is_empty())
            update(cur.size() - 1);
    }

    fast (const begin_t&, const set_type& set_a) :
        set_m(&set_a),
        cur(set_a.size()),
        icur(set_a.size(), set_a.element_set(), fromval),
        is_end_m(false)
    {
        if(!cur.is_empty())
            update(cur.size() - 1);
    }

    fast (const end_t&, const set_type& set_a) :
        set_m(&set_a),
        is_end_m(true)
    {}

    bool is_end () const
    {
        return is_end_m;
    }

    fast& operator++ ()
    {
        ARAGELI_ASSERT_0(!is_end());

        if(cur.is_empty())
            is_end_m = true;
        else next();

        return *this;
    }

    fast operator++ (int)
    {
        fast t = *this;
        operator++();
        return t;
    }

    const V* operator->() const
    {
        return &cur;
    }

    const V& operator*() const
    {
        return cur;
    }

    void reset ()
    {
        reset_all();
    }

private:

    typedef typename V::size_type size_type;

    void update (size_type s)
    {
        // Update all entries in cur up to index s by values from icur.
        for(size_type i = 0; i <= s; ++i)
            cur[i] = *icur[i];
    }

    void next ()
    {
        size_type i = 0;
        for(; i < icur.size(); ++i)
        {
            ARAGELI_ASSERT_1(!icur[i].is_end());
            ++icur[i];
            if(!icur[i].is_end())break;
            icur[i].reset();
        }

        if(i == icur.size())
            is_end_m = true;
        else
            update(i);
    }

    void reset_all ()
    {
        is_end_m = false;
        for(size_type i = 0; i < icur.size(); ++i)
            icur[i].reset();
        if(!cur.is_empty())
            update(cur.size() - 1);
    }

    const set_type* set_m;
    V cur;
    typename V::template other_element_type<fast<ElSet> >::type icur;
    bool is_end_m;
};

// TODO IMPLEMENT ordered specialization.


template <typename V, typename ElSet>
class fast<set::vector_upto_size<V, ElSet> > :
    public std::iterator
    <
        std::input_iterator_tag,
        const V,
        typename signed_type
        <
            typename set::vector_fixed_size<V,ElSet>::cardinality_type
        >::type,
        typename unsigned_type
        <
            typename set::vector_fixed_size<V, ElSet>::cardinality_type
        >::type
    >
{
    typedef set::vector_fixed_size<V, ElSet> VFS;

public:

    typedef set::vector_upto_size<V, ElSet> set_type;

    fast (const set_type& set_a) :
        size_m(set_a.size()),
        vfs(VFS(0, set_a.element_set())),
        icur(vfs.value())    // makes iteraton on empty vector
    {}

    fast (const begin_t&, const set_type& set_a) :
        size_m(set_a.size()),
        vfs(VFS(0, set_a.element_set())),
        icur(vfs.value())    // makes iteraton on empty vector
    {}

    fast (const end_t&, const set_type& set_a) :
        size_m(set_a.size()),
        icur(end, vfs.value())
    {}

    bool is_end () const
    {
        return icur.is_end();
    }

    fast& operator++ ()
    {
        ARAGELI_ASSERT_0(!is_end());
        if((++icur).is_end())
            next_size();
        return *this;
    }

    fast operator++ (int)
    {
        fast t = *this;
        operator++();
        return t;
    }

    const V* operator->() const
    {
        return icur.operator->();
    }

    const V& operator*() const
    {
        return *icur;
    }

    void reset ()
    {
        // WARNING! WE ALTERNATE SET WHEN THERE IS ITERATOR WITH IT.
        vfs = VFS(vfs.value().element_set());
        icur = fast<VFS>(vfs.value());
    }

private:

    typedef typename V::size_type size_type;

    void next_size ()
    {
        ARAGELI_ASSERT_1(icur.is_end());
        if(vfs.value().size() != size_m)
        {
            // WARNING! WE ALTERNATE SET WHEN THERE IS ITERATOR WITH IT.
            vfs = VFS
            (
                vfs.value().size() + 1,
                vfs.value().element_set()
            );

            icur = fast<VFS>(vfs.value());
        }
    }

    size_type size_m;
    refcntr_proxy<VFS> vfs;
    fast<VFS> icur;
};

// TODO IMPLEMENT ordered specialization.


template <typename T, typename VSet>
class fast<set::byvector<T, VSet> > :
    public std::iterator
    <
        std::input_iterator_tag,
        const T
        // WARNING! Temporary! Need to set other types, not the default
    >
{
public:

    typedef set::byvector<T, VSet> set_type;

    fast (const set_type& set_a) :
        base_m(set_a.base_set())
    {
        if(!is_end())update();
    }

    fast (const begin_t&, const set_type& set_a) :
        base_m(set_a.base_set())
    {
        if(!is_end())update();
    }

    fast (const end_t&, const set_type& set_a) :
        base_m(set_a.base_set())
    {
        if(!is_end())update();
    }

    bool is_end () const
    {
        return base_m.is_end();
    }

    fast& operator++ ()
    {
        ++base_m;
        if(!is_end())update();
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
        base_m.reset_all();
    }

private:

    void update ()
    {
        cur.assign_fromvec(*base_m);
    }

    fast<VSet> base_m;
    T cur;
};


} // namespace enumer


namespace rnd
{


template <typename V, typename ElSet, typename Base>
class equiprob<set::vector_fixed_size<V, ElSet>, Base>
{
    typedef equiprob<ElSet, Base> Base2;

public:

    /// Type of base random generator.
    typedef Base base_type;

    /// Type of set from which elements is being generated.
    typedef set::vector_fixed_size<V, ElSet> set_type;

    /// Type of generated values.
    typedef V value_type;

    /// Type of seed for random generator.
    typedef typename Base2::seed_type seed_type;

    /// Type representing the cardinality.
    typedef
        typename set::vector_fixed_size<V, ElSet>::cardinality_type
            cardinality_type;

    /// Type representing the period.
    typedef typename Base2::period_type period_type;

    equiprob (const set_type& set_a) :
        set_m(&set_a),
        base2(set_m->element_set())
    {}

    equiprob (seed_type seed, const set_type& set_a) :
        set_m(&set_a),
        base2(seed, set_m->element_set())
    {}

    cardinality_type cardinality ()
    {
        return set_m->cardinality();
    }

    period_type period ()
    {
        return base2.period();
    }

    value_type operator() () const
    {
        V res(set_m->size());
        for(typename V::size_type i = 0; i < set_m->size(); ++i)
            res[i] = base2();
        return res;
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
    Base2 base2;
};


template <typename V, typename ElSet, typename Base>
class equiprob<set::vector_upto_size<V, ElSet>, Base>
{
    // WARNING! Objects of types Base2 and Base3 are statistically dependent.

    typedef equiprob<ElSet, Base> Base2;
    typedef typename set::vector_upto_size<V, ElSet>::cardinality_type CT;
    typedef equiprob<set::inonnegative<CT>, Base> Base3;
    typedef typename V::size_type size_type;

public:

    /// Type of base random generator.
    typedef Base base_type;

    /// Type of set from which elements is being generated.
    typedef set::vector_upto_size<V, ElSet> set_type;

    /// Type of generated values.
    typedef V value_type;

    /// Type of seed for random generator.
    typedef typename Base::seed_type seed_type;

    /// Type representing the cardinality.
    typedef CT cardinality_type;

    /// Type representing the period.
    typedef typename Base2::period_type period_type;

    equiprob (const set_type& set_a) :
        set_m(&set_a),
        base2(set_m->element_set()),
        size_set_m(set_m->cardinality() - 1),
        base3(size_set_m)
    {
        prepare_table();
    }

    equiprob (seed_type seed, const set_type& set_a) :
        set_m(&set_a),
        base2(seed, set_m->element_set()),
        size_set_m(set_m->cardinality() - 1),
        base3(seed, size_set_m)
    {
        prepare_table();
    }

    cardinality_type cardinality ()
    {
        return set_m->cardinality();
    }

    period_type period ()
    {
        return base2.period();
    }

    value_type operator() () const
    {
        size_type size =
            std::upper_bound
            (
                tab.begin(), tab.end(),
                base3()
            ) - tab.begin();

        V res(size);
        for(typename V::size_type i = 0; i < size; ++i)
            res[i] = base2();
        return res;
    }

    void reset ()
    {
        base2.reset();
        base3.reset();
    }

    void reset (const seed_type& seed)
    {
        base2.reset(seed);
        base3.reset(seed);
    }

private:

    void prepare_table ()
    {
        tab.resize(set_m->size());
        if(tab.is_empty())return;
        tab[0] = unit<CT>();

        CT
            c1 = unit<CT>(),
            c2 = unit<CT>(),
            cbt = set_m->element_set().cardinality();

        for(size_type i = 1; i < tab.size(); ++i)
            tab[i] = (c1 += (c2 *= cbt));

        //std::cout
        //    << "\ncardinality = " << cardinality()
        //    << "\nc1 + c2*cbt = " << c1 + c2*cbt
        //    << "\ntab = " << tab << std::endl;

        ARAGELI_ASSERT_1(c1 + c2*cbt == cardinality());
    }

    // Do not change an order of the following declarations.
    const set_type* set_m;
    Base2 base2;
    set::inonnegative<CT> size_set_m;
    Base3 base3;
    vector<CT> tab;
};


template <typename T, typename VSet, typename Base>
class equiprob<set::byvector<T, VSet>, Base>
{
    typedef equiprob<VSet, Base> Base2;

public:

    /// Type of base random generator.
    typedef Base base_type;

    /// Type of set from which elements is being generated.
    typedef set::byvector<T, VSet> set_type;

    /// Type of generated values.
    typedef T value_type;

    /// Type of seed for random generator.
    typedef typename Base2::seed_type seed_type;

    /// Type representing the cardinality.
    // WARNING! Need to choose.
    typedef typename Base2::cardinality_type cardinality_type;

    /// Type representing the period.
    typedef typename Base2::period_type period_type;

    equiprob (const set_type& set_a) :
        base2(set_a.base_set())
    {}

    equiprob (seed_type seed, const set_type& set_a) :
        base2(seed, set_a.base_set())
    {}

    cardinality_type cardinality ()
    {
        // WARNING! INCORRECT!
        return base2->cardinality();
    }

    period_type period ()
    {
        return base2.period();
    }

    value_type operator() () const
    {
        return T(base2(), fromvec);
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

    Base2 base2;
};


} // namespace rnd

} // namesapce Arageli


// UNCOMMENT THE FOLLOWING FIVE LINES IF THERE IS vector.cpp FILE
//#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
//    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_setenumrnd_vector
//    #include "vector.cpp"
//    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_setenumrnd_vector
//#endif

#endif    // #ifndef _ARAGELI_setenumrnd_vector_hpp_
