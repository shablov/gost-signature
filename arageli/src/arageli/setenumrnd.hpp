/*****************************************************************************

    setenumrnd.hpp

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
    \file setenumrnd.hpp
    \brief Initial declarations for sets, enumerators and random generators.

    This file contains an initial declararion of template classes for
    representing sets, enumerating elements from those sets and random
    generating elements from them.
*/


#ifndef _ARAGELI_setenumrnd_hpp_
#define _ARAGELI_setenumrnd_hpp_

#include "config.hpp"
#include "rndbase.hpp"
#include "vector.hpp"


namespace Arageli
{


/// Representations for a set of objects as one thing.
/** Usual sets representations from this namespace has ways to enumerate
    objects (in enumer namespace) and to generate random elements
    (in rnd namespace). */
namespace set
{


/// An empty set.
/** Set without any elements. */
struct empty
{};


/// All subsets of set ST.
/** We suppose that ST is some set, e.g. 'empty', 'all' or
    even 'subsets' itself. This class identifies all subsets of
    that set. */
template <typename ST, typename SST = vector<typename ST::value_type> >
struct subsets
{
    /// The base set for subsets.
    typedef ST base_set_type;

    /// The class representing a particular subset.
    typedef SST value_type;
};


/// All subsets of set ST with the particular number of elements.
/** We suppose that ST is some set, e.g. 'empty', 'all' or
    even 'subsets_fixnum' itself. This class identifies all subsets
    with fixed number of elements of that set. It is in contrast
    to 'subsets' that represents ALL subsets of the particular set.

    WARNING! Partially implemented.
*/
template
<
    typename ST,
    typename SST = vector<typename ST::value_type>,
    typename N = typename vector<typename ST::value_type>::size_type
>
struct subsets_fixnum
{
    /// The base set for subsets.
    typedef ST base_set_type;

    /// The class representing a particular subset.
    typedef SST value_type;

    /// Initializes as all subsets of cardinality of n.
    subsets_fixnum (const base_set_type& bs, const N& n) :
        base_set_m(bs),
        subset_cardinality_m(n)
    {}

    /// Returns cardinality of each subset.
    const N& subset_cardinality () const
    {
        return subset_cardinality_m;
    }

    /// Returns cardinality of each subset.
    const base_set_type& base_set () const
    {
        return base_set_m;
    }

private:

    base_set_type base_set_m;
    N subset_cardinality_m;
};


/// All values of type T.
/** This class identifies all values representable as a value of type T.
    Object equivalence depends on T nature. */
template <typename T>
struct all
{
    /// Type of each element from the set.
    typedef T value_type;
};


} // namespace set


/// Enumerators for sets enumerating in set namespace.
/** This namespace allows to enumerate all elements in particular set.
    Each set represented as a sequence of its values.
    Each enumerator Enum from this namespace should have the following
    members:
    - typename set_type --- the set which enumerator works on
    - whole interface of the Standard C++ input iterator
    - constructor Enum(...) that creates an iterator on begin of the sequence
    - consturctor Enum(begin, ...) --- the same as the default constructor
    - constructor Enum(end, ...) --- iterator on the end of the sequence
    - bool is_end () const --- to check if the enumeration process is over;
        x.is_end() is equivalent to x == Enum(end).

    Notation "..." above means additional parameters depended on base set
    type. */
namespace enumer
{

struct begin_t {};

/// To create enumerator on the begin of the sequence.
static const begin_t begin = begin_t();

struct end_t {};

/// To create enumerator on the end of the sequence.
static const end_t end = end_t();


/// Default fast iterator for enumerates set without repetition.
/** All correct specialization of this template should enumerate all values
    from the set ST. An order of enumerating is undefined. */
template <typename ST>
class fast;


/// Default iterator that enumerates set without repetition in usual for ST order.
/** Meaning of "usual order" is determined for particular ST in
    corresponding specialization of this template. */
template <typename ST>
class ordered;

} // namespace enumer


namespace rnd
{


/// Random generator with pseudo-uniform disribution on continuous set ST.
/** This generator is usual for continuous sets, not discrete.
    Not all elements from ST is generated. Actually the generated
    values belong to some grid builded on ST in way that depends on
    ST nature and cardinality of Base generator. */
template <typename ST, typename Base = lcseq_fine1>
class uniform;


/// Random generator with pseudo-normal disribution on continuous set ST.
/** This generator is usual for continuous sets, not discrete.
    Not all elements from ST is generated. Actually the generated
    values belong to some discrete subset of ST that depends on
    ST nature and cardinality of Base generator. */
template <typename ST, typename Base = lcseq_fine1>
class normal;


/// Generates each element from ST with equiprobable outcome.
/** This generator is usual for finite sets, not continuous. */
template <typename ST, typename Base = lcseq_fine1>
class equiprob;


/// Generates each element from ST with equiprobable outcome.
/** This generator is usual for finite sets, not continuous.
    The difference from equiprob class is that an object of
    this class gives the parameters of the base set in moment
    of operator() call. The concrete collection of the parameters
    changes from one set to another. */
template <typename ST, typename Base = lcseq_fine1>
class equiprob_alter;


/// Holds a reference to another generator and give an access to it.
/** Gives an access by the usual interface of a random generator.
    There is no a reference counting, only one pointer.
    An original generator (that this class points to) must be alive
    until ref object will completely die. */
template <typename Base>
class ref
{
public:

    /// Type of base random generator.
    typedef Base base_type;

    /// Type of generated values.
    typedef typename Base::value_type value_type;

    /// Type of seed for random generator.
    typedef typename Base::seed_type seed_type;

    /// Type representing the cardinality.
    typedef typename Base::cardinality_type cardinality_type;

    /// Type representing the period.
    typedef typename Base::period_type period_type;

    ref (Base& base_a) :
        base_m(&base_a)
    {}

    const Base& base () const
    {
        return *base_m;
    }

    Base& base ()
    {
        return *base_m;
    }

    cardinality_type cardinality () const
    {
        return base_m->cardinality();
    }

    period_type period () const
    {
        return base_m->period();
    }

    value_type min () const
    {
        return base_m->min();
    }

    value_type max () const
    {
        return base_m->max();
    }

    void reset (const seed_type& seed)
    {
        base_m->reset(seed);
    }

    void reset ()
    {
        base_m->reset();
    }

    value_type operator() () const
    {
        return (*base_m)();
    }

private:

    Base* base_m;
};


} // namespace rnd


} // namesapce Arageli


#if 0 // UNCOMMENT THE FOLLOWING FIVE LINES IF THERE IS setenumrnd.cpp FILE
#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_setenumrnd
    #include "setenumrnd.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_setenumrnd
#endif
#endif

#endif    // #ifndef _ARAGELI_setenumrnd_hpp_
