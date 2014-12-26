/*****************************************************************************

    iteradapt.hpp

    Copyright (C) 2005--2007 Sergey S. Lyalin

    16.04.2005

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
    \file iteradapt.hpp
    \brief Iterator utilities.

    WARNING. Implementation of 'iterpair' is temporary limited.
*/


#ifndef _ARAGELI_UTILS_iteradapt_hpp_
#define _ARAGELI_UTILS_iteradapt_hpp_

#include "config.hpp"

#include <iterator>


namespace Arageli
{


// Iterator adapter with base iterator type TB and adaptation functor type F
// Functor F must operate with reference on base element type of base iterator.
// T -- is value type for new iterator type
template <typename T, typename TB, typename F>
class apply_iterator :
    public std::iterator
    <
        typename std::iterator_traits<TB>::iterator_category,
        T,
        typename std::iterator_traits<TB>::difference_type,
        T*,
        T&
    >
{
    typedef std::iterator_traits<TB> Iterbase;

    template <typename T1, typename TB1, typename F1>
    friend class apply_iterator;

public:

    typedef TB base_iterator;
    typedef F function;

    apply_iterator ()
    {}

    template <typename T1, typename TB1, typename F1>
    apply_iterator (const apply_iterator<T1, TB1, F1>& x) :
        fu(x.fu),
        iter(x.iter)
    {}

    explicit apply_iterator
    (
        const base_iterator& iter_a,
        const function& fu_a = function()
    ) :
        iter(iter_a),
        fu(fu_a)
    {}

    explicit apply_iterator (const function& fu_a) :
        fu(fu_a)
    {}

    const base_iterator& base () const
    {
        return iter;
    }

    base_iterator& base ()
    {
        return iter;
    }

    const function& adapter () const
    {
        return fu;
    }

    function& adapter ()
    {
        return fu;
    }

    T* operator-> () const
    {
        return &fu(*iter);
    }

    T& operator* () const
    {
        return fu(*iter);
    }

    apply_iterator& operator++ ()
    {
        ++iter; return *this;
    }

    apply_iterator& operator-- ()
    {
        --iter; return *this;
    }

    apply_iterator operator++ (int)
    {
        apply_iterator t = *this;
        operator++();
        return t;
    }

    apply_iterator operator-- (int)
    {
        apply_iterator t = *this;
        operator--();
        return t;
    }

    apply_iterator& operator+= (typename Iterbase::difference_type n)
    {
        iter += n; return *this;
    }

    apply_iterator& operator-= (typename Iterbase::difference_type n)
    {
        iter += n; return *this;
    }

    T& operator[] (typename Iterbase::difference_type n) const
    {
        return fu(iter[n]);
    }

private:

    function fu;
    base_iterator iter;

};


template <typename T, typename TB, typename F>
apply_iterator<T, TB, F> operator+
(
    apply_iterator<T, TB, F> x,
    typename apply_iterator<T, TB, F>::difference_type n
)
{
    return x += n;
}


template <typename T, typename TB, typename F>
apply_iterator<T, TB, F> operator-
(
    apply_iterator<T, TB, F> x,
    typename apply_iterator<T, TB, F>::difference_type n
)
{
    return x -= n;
}


template <typename T, typename TB, typename F>
inline typename std::iterator_traits<TB>::difference_type operator-
(
    const apply_iterator<T, TB, F>& a,
    const apply_iterator<T, TB, F>& b
)
{
    return a.base() - b.base();
}


#define _ARAGELI_ITERADAPT_CMP_OPER(OPER)    \
    template <typename T, typename TB, typename F>    \
    inline bool operator OPER    \
    (    \
        const apply_iterator<T, TB, F>& a,    \
        const apply_iterator<T, TB, F>& b    \
    )    \
    {    \
        return a.base() OPER b.base();    \
    }


_ARAGELI_ITERADAPT_CMP_OPER(==)
_ARAGELI_ITERADAPT_CMP_OPER(!=)
_ARAGELI_ITERADAPT_CMP_OPER(<)
_ARAGELI_ITERADAPT_CMP_OPER(>)
_ARAGELI_ITERADAPT_CMP_OPER(<=)
_ARAGELI_ITERADAPT_CMP_OPER(>=)


template <typename T1, typename T2>
struct min_iterator_traits;


// Iterator pair incapsulates two iterators and represented virtual sequence as
// element sequence generated by functor F object. Suppose TB2 iterator is
// more 'flexible' iterator then TB1 iterator. That's if some operation is applicable
// for type TB1 then one is applicable for type TB2. This nonsymmetrical situation is
// temporary.
template <typename T, typename TB1, typename TB2, typename F>
class iterpair :
    public std::iterator
    <
        //typename min_iterator_traits<TB1, TB2>::iterator_category,
        typename std::iterator_traits<TB1>::iterator_category,
        T,
        //typename min_iterator_traits<TB1, TB2>::difference_type,
        typename std::iterator_traits<TB1>::difference_type,
        T*,
        T&
    >
{
    //typedef min_iterator_traits<TB1, TB2> Iterbase;
    typedef std::iterator_traits<TB1> Iterbase;

public:

    typedef TB1 Base_first;
    typedef TB2 Base_second;
    typedef F function;    // REMARK: may be another name is better for this type

    iterpair ()
    {}

    iterpair
    (
        const Base_first& iterf_a,
        const Base_second& iters_a,
        const function& fu_a = function()
    ) :
        iterf(iterf_a),
        iters(iters_a),
        fu(fu_a)
    {}

    iterpair (const function& fu_a) :
        fu(fu_a)
    {}

    const Base_first& base_first () const
    {
        return iterf;
    }

    Base_first& base_first ()
    {
        return iterf;
    }

    const Base_second& base_second () const
    {
        return iters;
    }

    Base_second& base_second ()
    {
        return iters;
    }

    // WARNING! Why name is `unitor'? Recall the renaming history.
    const function& unitor () const
    {
        return fu;
    }

    function& unitor ()
    {
        return fu;
    }

    T* operator-> () const
    {
        return &fu(*iterf, *iters);
    }    // may be problems

    typename F::result_type operator* () const
    {
        return fu(*iterf, *iters);
    }

    iterpair& operator++ ()
    {
        ++iterf;
        ++iters;
        return *this;
    }

    iterpair& operator-- ()
    {
        --iterf;
        --iters;
        return *this;
    }

    iterpair& operator+= (typename Iterbase::difference_type n)
    {
        iterf += n;
        iters += n;
        return *this;
    }

    iterpair& operator-= (typename Iterbase::difference_type n)
    {
        iterf += n;
        iters += n;
        return *this;
    }

    typename F::result_type operator[] (typename Iterbase::difference_type n) const
    {
        return fu(iterf[n], iters[n]);
    }

    iterpair operator+ (typename Iterbase::difference_type n)
    {
        return iterpair(iterf + n, iters + n, fu);
    }

    iterpair operator- (typename Iterbase::difference_type n)
    {
        return iterpair(iterf - n, iters - n, fu);
    }

private:

    function fu;
    Base_first iterf;
    Base_second iters;

};


template <typename T, typename TB1, typename TB2, typename F>
inline typename std::iterator_traits<TB1>::difference_type operator-
(
    const iterpair<T, TB1, TB2, F>& a,
    const iterpair<T, TB1, TB2, F>& b
)
{
    return a.base_first() - b.base_first();
}


template <typename T, typename TB1, typename TB2, typename F>
inline bool operator==
(
    const iterpair<T, TB1, TB2, F>& a,
    const iterpair<T, TB1, TB2, F>& b
)
{
    return a.base_first() == b.base_first();
}

template <typename T, typename TB1, typename TB2, typename F>
inline bool operator!=
(
    const iterpair<T, TB1, TB2, F>& a,
    const iterpair<T, TB1, TB2, F>& b
)
{
    return a.base_first() != b.base_first();
}

template <typename T, typename TB1, typename TB2, typename F>
inline bool operator<
(
    const iterpair<T, TB1, TB2, F>& a,
    const iterpair<T, TB1, TB2, F>& b
)
{
    return a.base_first() < b.base_first();
}

template <typename T, typename TB1, typename TB2, typename F>
inline bool operator>
(
    const iterpair<T, TB1, TB2, F>& a,
    const iterpair<T, TB1, TB2, F>& b
)
{
    return a.base_first() > b.base_first();
}

template <typename T, typename TB1, typename TB2, typename F>
inline bool operator<=
(
    const iterpair<T, TB1, TB2, F>& a,
    const iterpair<T, TB1, TB2, F>& b
)
{
    return a.base_first() == b.base_first();
}

template <typename T, typename TB1, typename TB2, typename F>
inline bool operator>=
(
    const iterpair<T, TB1, TB2, F>& a,
    const iterpair<T, TB1, TB2, F>& b
)
{
    return a.base_first() >= b.base_first();
}


// -------------------------------------------------------


/*
template <typename Iter_base>
class slice_iterator :
    public std::iterator
    <
        typename std::iterator_traits<Iter_base>::iterator_category,
        typename std::iterator_traits<Iter_base>::value_type,
        typename std::iterator_traits<Iter_base>::difference_type,
        typename std::iterator_traits<Iter_base>::pointer,
        typename std::iterator_traits<Iter_base>::reference
    >
{
public:
    typedef typename std::iterator_traits<Iter_base>::value_type value_type;
    typedef typename std::iterator_traits<Iter_base>::value_type difference_type;


};
*/

} // namespace Arageli


#endif // #ifndef _ARAGELI_UTILS_iteradapt_hpp_
