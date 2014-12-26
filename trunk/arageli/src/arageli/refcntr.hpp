/*****************************************************************************

    refcntr.hpp -- The implementation of reference counter class.

    Copyright (C) 2005--2007 Sergey S. Lyalin

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

#ifndef _ARAGELI_refcntr_hpp_
#define _ARAGELI_refcntr_hpp_

#include "config.hpp"

#include <cstddef>
#include <memory>
#include <algorithm>
#include <limits>

#include "exception.hpp"
#include "type_opers.hpp"

#include "std_import.hpp"

namespace Arageli
{



template <typename T, typename TC = std::size_t>
class refcntr
{
public:

    typedef T value_type;
    typedef TC counter_type;

    refcntr (const counter_type& refs_a = counter_type(1)) :
        refs_m(refs_a)
    {}

    refcntr
    (
        const value_type& value_a,
        const counter_type& refs_a = counter_type(1)
    ) :
        value_m(value_a),
        refs_m(refs_a)
    {}

    value_type& value ()
    {
        return value_m;
    }

    const value_type& value () const
    {
        return value_m;
    }

    const counter_type& refs () const
    {
        return refs_m;
    }

    void link (const counter_type& n = counter_type(1))
    {
        ARAGELI_ASSERT_0(n >= 1);

        ARAGELI_ASSERT_0
        (
            !std::numeric_limits<TC>::is_bounded ||
            std::numeric_limits<TC>::max() - n >= refs_m
        );

        refs_m += n;
    }

    bool unlink (const counter_type& n = counter_type(1))
    {
        ARAGELI_ASSERT_0(n >= 1);
        ARAGELI_ASSERT_0(refs_m >= n);

        return !(refs_m -= n);
    }

    void unlink_all ()
    {
        refs_m = counter_type(0);
    }

private:

    counter_type refs_m;
    value_type value_m;
};


// 'refcntr_proxy' служит интерфейсом для отключаемого счётчика ссылок
// и держит в себе значение. Нет общей реализации этого шаблона, предоставлены
// две частичные специализации: для включённого счётчика ссылок (on_a == true)
// и отключённого счётчика ссылок (on_a == false).
template
<
    typename T,
    bool on_a = true,
    typename TC = typename refcntr<T>::counter_type,
    typename A = std::allocator<T>
>
class refcntr_proxy;


// Объект со счётчиком ссылок, держит значение и счётчик в памяти получаемой
// с помощью распределителя A.
template <typename T, typename TC, typename A>
class refcntr_proxy<T, true, TC, A>
{

    /*
    template <typename, bool, typename, typename>
    friend class refcntr_proxy;
    */

    typedef typename A::template rebind<refcntr<T, TC> >::other Ref_alloc;

public:

    typedef T value_type;
    typedef TC counter_type;
    typedef A Allocator;
    static const bool COUNTING = true;


    refcntr_proxy ()
    {
        refcntr_m = alloc.allocate(1);
        new (refcntr_m) refcntr<T>();
    }

    refcntr_proxy (const value_type& value_a)
    {
        refcntr_m = alloc.allocate(1);
        new (refcntr_m) refcntr<T>(value_a);
    }

    refcntr_proxy (const refcntr_proxy& x) :
        refcntr_m(x.refcntr_m)
    {
        refcntr_m->link();
    }

    ~refcntr_proxy ()
    {
        destroy();
    }

    refcntr_proxy& operator= (const refcntr_proxy& x)
    {
        if(&x != this && refcntr_m != x.refcntr_m)
        {
            destroy();
            (refcntr_m = x.refcntr_m)->link();
        }
        return *this;
    }

    value_type& value ()
    {
        return refcntr_m->value();
    }

    const value_type& value () const
    {
        return refcntr_m->value();
    }

    const counter_type& refs () const
    {
        return refcntr_m->refs();
    }

    // возвращает true, если значение осталось прежним,
    // false, если была сделана новая копия
    bool unique ()
    {
        if(refs() == 1)
            return true;
        do_unique();
        return false;
    }

    void do_unique ()
    {
        typename Ref_alloc::pointer nt = alloc.allocate(1);
        new (nt) refcntr<T>(value());    // копия старого значения
        refcntr_m->unlink();
        refcntr_m = nt;
    }

    // возвращает true, если значение осталось прежним,
    // false, если был сделан новый чистый объект
    bool unique_clear ()
    {
        if(refs() == 1)
            return true;
        do_unique_clear();
        return false;
    }

    void do_unique_clear ()
    {
        typename Ref_alloc::pointer nt = alloc.allocate(1);
        new (nt) refcntr<T>;    // новый чистый объект
        refcntr_m->unlink();
        refcntr_m = nt;
    }

    bool counting () const
    {
        return COUNTING;
    }

    // ПРЕДУПРЕЖДЕНИЕ. Нужно ещё добавить swap в std.

    template <typename T1, typename TC1, typename A1>
    void swap (refcntr_proxy<T1, false, TC1, A1>& x)
    {
        swap_values(x);
    }

    template <typename T1, typename TC1, typename A1>
    void swap (refcntr_proxy<T1, true, TC1, A1>& x)
    {
        swap_help_1(x, equal_types<refcntr<T, TC>, refcntr<T1, TC1> >::value);
    }

private:

    void destroy ()
    {
        if(refcntr_m->unlink())
        {
            alloc.destroy(refcntr_m);
            alloc.deallocate(refcntr_m, 1);
        }
    }

    template <typename T1, typename TC1, typename A1>
    void swap_help_1 (refcntr_proxy<T1, true, TC1, A1>& x, true_type)
    {
        ARAGELI_ASSERT_1
        (
            (equal_types<refcntr<T, TC>, refcntr<T1, TC1> >::bvalue)
        );

        if(alloc == x.alloc)
            std::swap(refcntr_m, x.refcntr_m);
        else
            swap_values(x);
    }

    template <typename T1, typename TC1, typename A1>
    void swap_help_1 (refcntr_proxy<T1, true, TC1, A1>& x, false_type)
    {
        swap_values(x);
    }

    template <typename T1, bool REFCNT, typename TC1, typename A1>
    void swap_values (refcntr_proxy<T1, REFCNT, TC1, A1>& x)
    {
        unique(); x.unique();
        std::swap(value(), x.value());
    }

    typename Ref_alloc::pointer refcntr_m;
    Ref_alloc alloc;

};


// Значение без счётчика ссылок, содержит в себе объект типа T без счётчиков.
// Распределитель памяти A в процессе работы не используется.
template <typename T, typename TC, typename A>
class refcntr_proxy<T, false, TC, A>
{
    /*
    template <typename, bool, typename, typename>
    friend class refcntr_proxy;
    */

public:

    typedef T value_type;
    typedef TC counter_type;
    static const bool COUNTING = false;
    typedef A Allocator;

    refcntr_proxy () {}

    refcntr_proxy (const T& value_a) :
        value_m(value_a)
    {}

    value_type& value ()
    {
        return value_m;
    }

    const value_type& value () const
    {
        return value_m;
    }

    const counter_type& refs () const
    {
        return one_always;
    }

    bool unique_clear () const
    {
        return true;
    }

    bool unique () const
    {
        return true;
    }

    bool counting () const
    {
        return COUNTING;
    }

    template <typename T1, bool REFCNT1, typename TC1, typename A1>
    void swap (refcntr_proxy<T1, REFCNT1, TC1, A1>& x)
    {
        std::swap(value(), x.value());
    }

private:

    value_type value_m;
    static const counter_type one_always = counter_type(1);

};

template <typename T, typename TC, typename A>
const TC refcntr_proxy<T, false, TC, A>::one_always;


}


namespace std
{

template
<
    typename T1,
    bool REFCNT1,
    typename TC1,
    typename A1,
    typename T2,
    bool REFCNT2,
    typename TC2,
    typename A2
>
inline void swap
(
    Arageli::refcntr_proxy<T1, REFCNT1, TC1, A1>& a,
    Arageli::refcntr_proxy<T2, REFCNT2, TC2, A2>& b
)
{
    a.swap(b);
}


// Without this specialization ambigues call betwin standard swap and
// swap above is appeared.
template <typename T1, bool REFCNT1, typename TC1, typename A1>
inline void swap
(
    Arageli::refcntr_proxy<T1, REFCNT1, TC1, A1>& a,
    Arageli::refcntr_proxy<T1, REFCNT1, TC1, A1>& b
)
{
    a.swap(b);
}


}


#endif  //  _ARAGELI_refcntr_hpp_
