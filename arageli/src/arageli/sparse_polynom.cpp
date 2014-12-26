/*****************************************************************************

    sparse_polynom.cpp -- See description in file sparse_polynom.hpp.

    This file is a part of Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
    Copyright (C) 2005--2010 Sergey S. Lyalin

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

#include "config.hpp"

#if !defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE) ||    \
    defined(ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_SPARSE_POLYNOM)

#include <iomanip>
#include <algorithm>
#include <algorithm>
#include <cstring>
#include <cctype>
#include <iostream>
#include <sstream>

#include "sparse_polynom.hpp"
#include "vector.hpp"
#include "_utility.hpp"
#include "functional.hpp"

//****************************************************************************

namespace Arageli
{


template <typename F, typename I>
monom<F, I>::monom (const char* s)
{
    std::stringstream buf(s);
    // WARNING. Correct if there are no virtual function.
    buf >> *this;
    if(!buf && !buf.eof())
        throw incorrect_string(s);
}


template
<
    typename Ch,
    typename ChT,
    typename F,
    typename I
>
std::basic_ostream<Ch, ChT>& output_var
(
    std::basic_ostream<Ch, ChT>& out,
    const monom<F, I>& x,
    bool first,
    const char* var,
    const char* mul,
    const char* pow
)
{
    if(x.is_const())
        if(first)
            output_polynom_first(out, x.coef());
        else
            output_polynom_internal(out, x.coef());
    else
    {
        if(is_opposite_unit(x.coef()))
            out << '-';
        else if(is_unit(x.coef()))
        {
            if(!first)
                out << '+';
        }
        else if(first)
            output_polynom_first(out, x.coef()) << mul;
        else
            output_polynom_internal(out, x.coef()) << mul;
        out << var;
        if(!is_unit(x.degree()))
        {
            out << pow;
            output_pow(out, x.degree());
        }
    }
    return out;
}


template
<
    typename Ch,
    typename ChT,
    typename F,
    typename I
>
std::basic_istream<Ch, ChT>& input_list
(
    std::basic_istream<Ch, ChT>& in,
    monom<F, I>& x,
    const char* fb,
    const char* sb,
    const char* sep
)
{
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(fb));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(sb));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(sep));

    monom<F, I> tmp;
    if
    (
        !(
            !_Internal::read_literal(in, fb)  ||
            !input_polynom_first(in, tmp.coef())  ||
            !_Internal::read_literal(in, sep)  ||
            !input_pow(in, tmp.degree())  ||
            !_Internal::read_literal(in, sb)
        )
    )
        x = tmp;

    return in;
}


template
<
    typename Ch,
    typename ChT,
    typename F,
    typename I,
    typename Factory_coef
>
std::basic_istream<Ch, ChT>& input_var
(
    std::basic_istream<Ch, ChT>& in,
    monom<F, I>& x,
    bool first_a,
    const Factory_coef& fctr,
    const char* var,
    const char* mul,
    const char* pow
)
{
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(var));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(mul));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(pow));

    monom<F, I> res;
    char ch, ch1 = 0;

    in >> ch1;
    if(!in)
        return in;

    if(ch1 != '-' && ch1 != '+')
    {
        in.putback(ch1);

        if(!first_a)
        {
            in.clear(std::ios_base::failbit);
            return in;
        }
    }

    if(_Internal::read_literal(in, var))
    {
        if(ch1 == '-')
            res.coef() = fctr.opposite_unit(x.coef());
        else
            res.coef() = fctr.unit(x.coef());

        ch = in.get();
        in.clear();
        in.putback(ch);

        if(ch != '\n' && _Internal::read_literal(in, pow))
        {
            input_pow(in, res.degree());
            if(!in && !in.eof())
            {
                ARAGELI_ASSERT_1(!in.good());
                return in;
            }
        }
        else
            res.degree() = unit(x.degree());

        x = res;
        in.clear();
        return in;
    }

    //std::cerr << "\n" << in.good();
    if(ch1 == '-' || ch1 == '+')
        in.putback(ch1);
    in.clear();
    //std::cerr << "\n" << in.good();

    if(first_a)
        input_polynom_first(in, res.coef());
    else
        input_polynom_internal(in, res.coef());

    if(!in)
    {
        if(!in.eof())
        {
            ARAGELI_ASSERT_1(!in.good());
            return in;
        }
    }
    else
    {
        ch = in.get();
        in.putback(ch);
        in.clear();

        if(!in.eof() && ch != '\n')
        {
            if(_Internal::read_literal(in, mul))
            {
                if(!_Internal::read_literal(in, var))
                {
                    in.clear(std::ios_base::badbit);
                    return in;
                }

                ch = in.get();
                in.clear();
                in.putback(ch);

                if(ch != '\n' && _Internal::read_literal(in, pow))
                {
                    input_pow(in, res.degree());
                    if(!in && !in.eof())
                    {
                        ARAGELI_ASSERT_1(!in.good());
                        return in;
                    }
                }
                else
                    res.degree() = unit(x.degree());
            }
        }
    }

    x = res;

    in.clear();
    return in;
}


template <typename F, typename I, bool REFCNT>
sparse_polynom<F, I, REFCNT>::sparse_polynom (const char* str)
{
    std::istringstream buf(str);
    // WARNING. It's correct if and only if sparse_polynom has no virtual functions.
    static_cast<std::istream&>(buf) >> *this;
    if(!buf && !buf.eof())
        throw incorrect_string(str);
    //ARAGELI_ASSERT_0(buf.good() || buf.eof());
}


template <typename F, typename I, bool REFCNT>
bool sparse_polynom<F, I, REFCNT>::is_normal () const
{
    // WARNING! PERFORMANCE!
    // Next two checks (for degree order and zero coefficients) are made
    // in two separate passes through the monomial sequence. It should be done
    // in only one pass.
    return
        // find if there are any degree strong order violations and ...
        std::adjacent_find
        (
            rep().begin(),
            rep().end(),
            std::not2(monom_degree_less<monom>())
        ) == rep().end()
        &&
        // ... and if there are any zero coefficients
        std::find_if
        (
            rep().begin(),
            rep().end(),
            std::mem_fun_ref(&monom::is_null)
        ) == rep().end();
}


template <typename F, typename I, bool REFCNT>
sparse_polynom<F, I, REFCNT>& sparse_polynom<F, I, REFCNT>::opposite ()
{
    ARAGELI_ASSERT_EX1_0(is_normal(), *this);

    if(is_null())
        return *this;
    store.unique();

    std::for_each
    (
        rep().begin(),
        rep().end(),
        std::mem_fun_ref(&monom::opposite)
    );

    return *this;
}


template <typename F, typename I, bool REFCNT>
template <typename F1>
sparse_polynom<F, I, REFCNT>& sparse_polynom<F, I, REFCNT>::add_scalar
(const F1& x)
{
    ARAGELI_ASSERT_EX1_0(is_normal(), *this);

    if(Arageli::is_null(x))return *this;
    store.unique();

    if(rep().empty() || !rep().front().is_const())
        rep().push_front(monom(x));
    else if((rep().front() += x).is_null())
        rep().pop_front();

    return *this;
}


template <typename F, typename I, bool REFCNT>
template <typename F1>
sparse_polynom<F, I, REFCNT>& sparse_polynom<F, I, REFCNT>::sub_scalar
(const F1& x)
{
    ARAGELI_ASSERT_EX1_0(is_normal(), *this);

    if(Arageli::is_null(x))
        return *this;
    store.unique();

    if(rep().empty() || !rep().front().is_const())
        rep().push_front(-monom(x));
    else if((rep().front() -= x).is_null())
        rep().pop_front();

    return *this;
}


#define _ARAGELI_SPARSE_POLYNOM_OPERATOR_ASSIGN_1(MNEM, OPER)    \
    template <typename F, typename I, bool REFCNT>    \
    template <typename F1>    \
    sparse_polynom<F, I, REFCNT>&    \
    sparse_polynom<F, I, REFCNT>::MNEM##_scalar    \
    (const F1& x)    \
    {    \
        ARAGELI_ASSERT_EX1_0(is_normal(), *this);    \
        if(is_null())    \
            return *this;    \
    \
        for    \
        (    \
            coef_iterator i = coefs_begin(), iend = coefs_end();    \
            i != iend;    \
            ++i    \
        )    \
            /*if(Arageli::is_null(*i OPER x))    \
                i = erase(i.base());*/    \
            *i OPER x;    \
    \
        normalize();    /* WARNING! We don't need full normalize! */    \
        return *this;    \
    }

_ARAGELI_SPARSE_POLYNOM_OPERATOR_ASSIGN_1(mul, *=);
_ARAGELI_SPARSE_POLYNOM_OPERATOR_ASSIGN_1(div, /=);
_ARAGELI_SPARSE_POLYNOM_OPERATOR_ASSIGN_1(mod, %=);
_ARAGELI_SPARSE_POLYNOM_OPERATOR_ASSIGN_1(bitor, |=);
_ARAGELI_SPARSE_POLYNOM_OPERATOR_ASSIGN_1(bitand, &=);
_ARAGELI_SPARSE_POLYNOM_OPERATOR_ASSIGN_1(bitxor, ^=);
_ARAGELI_SPARSE_POLYNOM_OPERATOR_ASSIGN_1(shl, <<=);
_ARAGELI_SPARSE_POLYNOM_OPERATOR_ASSIGN_1(shr, >>=);


template <typename F, typename I, bool REFCNT>
template <typename F1, typename I1>
sparse_polynom<F, I, REFCNT>& sparse_polynom<F, I, REFCNT>::operator+=
(const Arageli::monom<F1, I1>& x)
{
    ARAGELI_ASSERT_EX1_0(is_normal(), *this);

    if(x.is_null())
        return *this;
    store.unique();
    typename Rep::iterator i = find_pos(x);

    if(i != rep().end() && x.can_add(*i))
    {
        if((*i += x).is_null())
            rep().erase(i);
    }
    else
        rep().insert(i, x);

    return *this;
}


template <typename F, typename I, bool REFCNT>
template <typename F1, typename I1>
sparse_polynom<F, I, REFCNT>& sparse_polynom<F, I, REFCNT>::operator-=
(const Arageli::monom<F1, I1>& x)
{
    ARAGELI_ASSERT_EX1_0(is_normal(), *this);

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.

    return operator+=(-x);
}


template <typename F, typename I, bool REFCNT>
template <typename F1, typename I1>
sparse_polynom<F, I, REFCNT>& sparse_polynom<F, I, REFCNT>::operator*=
(const Arageli::monom<F1, I1>& x)
{
    ARAGELI_ASSERT_EX1_0(is_normal(), *this);

    if(x.is_unit())
        return *this;

    if(x.is_null())
    {
        if(store.unique_clear())
            rep().clear();
        return *this;
    }

    store.unique();

    // WARNING!!!
    Arageli::monom<F1, I1> xx = x;    // copy because x maybe part of this polynomial

    for(typename Rep::iterator i = rep().begin(); i != rep().end();)
        if((*i *= xx).is_null())
            i = rep().erase(i);
        else
            ++i;

    return *this;
}


template <typename F, typename I, bool REFCNT>
template <typename T1>
sparse_polynom<F, I, REFCNT>&
sparse_polynom<F, I, REFCNT>::add_sparse_polynom
(const T1& x)
{
    ARAGELI_ASSERT_EX1_0(is_normal(), *this);
    ARAGELI_ASSERT_EX1_0(x.is_normal(), x);

    if(x.is_null())
        return *this;

    store.unique();
    if
    (
        reinterpret_cast<const void*>(this) ==
        reinterpret_cast<const void*>(&x))
    {
        ARAGELI_ASSERT_0(!is_null());
        for(typename Rep::iterator i = rep().begin(); i != rep().end();)
            if((*i += *i).is_null())
                i = rep().erase(i);
            else
                ++i;
    }
    else
    {
        monom_iterator i1 = monoms_begin(), i1end = monoms_end();
        typename T1::monom_const_iterator i2 = x.monoms_begin(),
            i2end = x.monoms_end();

        while(i1 != i1end && i2 != i2end)
        {
            if(i1->degree() < i2->degree())
                ++i1;
            else if(i1->degree() == i2->degree())
            {
                if((*i1 += *i2).is_null())
                    i1 = rep().erase(i1);
                else
                    ++i1;
                ++i2;
            }
            else
            { // i1->degree() > i2->degree()
                rep().insert(i1, *i2);
                ++i2;
            }
        }

        if(i2 != x.rep().end())
        {
            ARAGELI_ASSERT_1(i1 == monoms_end());
            rep().insert(i1, i2, x.monoms_end());
        }
    }

    return *this;
}


template <typename F, typename I, bool REFCNT>
template <typename T1>
sparse_polynom<F, I, REFCNT>&
sparse_polynom<F, I, REFCNT>::sub_sparse_polynom
(const T1& x)
{
    ARAGELI_ASSERT_EX1_0(is_normal(), *this);
    ARAGELI_ASSERT_EX1_0(x.is_normal(), x);

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.

    return operator+=(-x);
}


template <typename F, typename I, bool REFCNT>
template <typename T1>
sparse_polynom<F, I, REFCNT>&
sparse_polynom<F, I, REFCNT>::mul_sparse_polynom
(const T1& x)
{
    ARAGELI_ASSERT_EX1_0(is_normal(), *this);
    ARAGELI_ASSERT_EX1_0(x.is_normal(), x);

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.

    if(is_null())
        return *this;
    if(x.is_null())
    {
        if(store.unique_clear())
            rep().clear();
        return *this;
    }

    sparse_polynom res;
    for
    (
        typename T1::monom_const_iterator i =
            x.monoms_begin(), iend = x.monoms_end();
        i != iend;
        ++i
    )
    {
        sparse_polynom t(*this);
        t *= *i;
        res += t;
    }

    *this = res;
    return *this;
}


template <typename F, typename I, bool REFCNT>
template <typename T1>
sparse_polynom<F, I, REFCNT>&
sparse_polynom<F, I, REFCNT>::div_sparse_polynom
(const T1& x)
{
    ARAGELI_ASSERT_EX1_0(x.is_normal(), x);
    ARAGELI_ASSERT_EX1_0(is_normal(), *this);

    if(is_null())
        return *this;
    if(x.is_null())
        throw division_by_zero();

    if(degree() < x.degree())
    {
        if(store.unique_clear())
            rep().clear();
        return *this;
    }

    assign_div(*this, x);
    return *this;
}


// WARNING! The second arg should more common.
template <typename F, typename I, bool REFCNT>
template <typename F1, typename I1, bool REFCNT1>
void sparse_polynom<F, I, REFCNT>::assign_div_common
(
    sparse_polynom& x,
    const sparse_polynom<F1, I1, REFCNT1>& y
)
{
    ARAGELI_ASSERT_EX1_1(x.is_normal(), x);
    ARAGELI_ASSERT_1(y.is_normal());

#if 1

    sparse_polynom pq, pr;
    polynom_divide(x, y, pq, pr);
    x = pq;

#else

    ARAGELI_ASSERT_1(!x.is_null());
    ARAGELI_ASSERT_1(!y.is_null());

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.
    // WARNING. This implementation is simple but is slow, may be quicker.

    // WARNING. This algorithm works only if F and F1 are fields.

    sparse_polynom t(x);
    sparse_polynom res;

    while(!t.is_null() && t.degree() >= y.degree())
    {
        ARAGELI_ASSERT_0(!t.is_null());
        monom mt = t.leading_monom();
        mt /= y.leading_monom();    // maybe integer division
        if(mt.is_null())
            break;
        sparse_polynom<F1, I1, REFCNT1> tx(y);
        //std::cout << "\ntx before = " << tx;
        tx *= mt;
        //std::cout << "\nmt sumtraction = " << mt;
        //std::cout << "\ntx after = " << tx;
        //std::cout << "\nt before sumtraction = " << t;
        ARAGELI_ASSERT_0(!tx.is_null());
        t -= tx;
        //std::cout << "\nt after sumtraction = " << t;
        res += mt;
    }

    x = res;

#endif
}


template <typename F, typename I, bool REFCNT>
template <typename T1>
sparse_polynom<F, I, REFCNT>&
sparse_polynom<F, I, REFCNT>::mod_sparse_polynom
(const T1& x)
{
    ARAGELI_ASSERT_EX1_0(is_normal(), *this);
    ARAGELI_ASSERT_EX1_0(x.is_normal(), x);

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.
    // WARNING. This implementation is simple but is slow, maybe quicker.

    if(is_null())
        return *this;
    if(x.is_null())
        throw division_by_zero();

#if 1

    sparse_polynom pq, pr;
    polynom_divide(*this, x, pq, pr);
    *this = pr;

#else

    if(degree() < x.degree())
        return *this;
    sparse_polynom res(*this);

    while(!res.is_null() && res.degree() >= x.degree())
    {
        monom mt = res.leading_monom();
        mt /= x.leading_monom();
        if(mt.is_null())
            break;
        T1 tx(x);
        tx *= mt;
        res -= tx;
    }

    *this = res;

#endif

    return *this;
}


template <typename F, typename I, bool REFCNT>
template <typename Iter>
void sparse_polynom<F, I, REFCNT>::add
(Iter first, Iter last, const any_monom_seq_t&)
{
    // ПРЕДУПРЕЖДЕНИЕ. 'first' и 'last' не должны быть итераторами
    // для данного объекта полинома.

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.

    while(first != last)
        *this += *first++;
}


template <typename F, typename I, bool REFCNT>
template <typename Iter>
void sparse_polynom<F, I, REFCNT>::add
(Iter first, Iter last, const norm_monom_seq_t&)
{
    // ПРЕДУПРЕЖДЕНИЕ. 'first' и 'last' не должны быть итераторами
    // для данного объекта полинома.

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.

    while(first != last)
        *this += *first++;
}


template <typename F, typename I, bool REFCNT>
template <typename Iter>
void sparse_polynom<F, I, REFCNT>::sub
(Iter first, Iter last, const any_monom_seq_t&)
{
    // ПРЕДУПРЕЖДЕНИЕ. 'first' и 'last' не должны быть итераторами
    // для данного объекта полинома.

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.

    while(first != last)
        *this -= *first++;
}


template <typename F, typename I, bool REFCNT>
template <typename Iter>
void sparse_polynom<F, I, REFCNT>::sub
(Iter first, Iter last, const norm_monom_seq_t&)
{
    // ПРЕДУПРЕЖДЕНИЕ. 'first' и 'last' не должны быть итераторами
    // для данного объекта полинома.

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.

    while(first != last)
        *this -= *first++;
}


template <typename F, typename I, bool REFCNT>
template <typename F1, typename I1, bool REFCNT1>
void sparse_polynom<F, I, REFCNT>::addsub
(
    sparse_polynom<F1, I1, REFCNT1>& x,
    typename sparse_polynom<F1, I1, REFCNT1>::monom_iterator pos
)
{
    // Предполагается, что F1 != F или I1 != I.
    // В противном случае должна вызываться другая специализация этой функции.
    // Типы 'Rep' для *this и для x различны, поэтому мы не можем перемещать
    // мономы между этими списками без копирования.

    ARAGELI_ASSERT_0(pos != x.rep().end());

    *this += *pos;
    x.erase(pos);
}


template <typename F, typename I, bool REFCNT>
template <bool REFCNT1>
void sparse_polynom<F, I, REFCNT>::addsub
(
    sparse_polynom<F, I, REFCNT1>& x,
    typename sparse_polynom<F, I, REFCNT1>::monom_iterator pos
)
{
    // Так как F и I для обоих полиномов (и для '*this' и для 'x') одинаковы,
    // то типы 'Rep' для них должны быть эквивалентны.
    // Поэтому применяем обычные операции со списками, где это возможно.

    ARAGELI_ASSERT_0(pos != x.rep().end());

    unique();

    typename Rep::iterator i = find_pos(*pos);
    if(i != rep().end() && pos->can_add(*i))
    {
        if((*i += *pos).is_null())
            rep().erase(i);
        x.erase(pos);
    }
    else
        rep().splice(i, x.rep(), pos);
}


template <typename F, typename I, bool REFCNT>
template <typename F1, typename I1, bool REFCNT1>
void sparse_polynom<F, I, REFCNT>::addsub
(
    sparse_polynom<F1, I1, REFCNT1>& x,
    typename sparse_polynom<F1, I1, REFCNT1>::monom_iterator first,
    typename sparse_polynom<F1, I1, REFCNT1>::monom_iterator last,
    const any_monom_seq_t&
)
{
    // Предполагается, что F1 != F или I1 != I.
    // В противном случае должна вызываться другая специализация этой функции.
    // Типы 'Rep' для *this и для x различны, поэтому мы не можем перемещать
    // мономы между этими списками без копирования.

    ARAGELI_ASSERT_1(static_cast<sparse_polynom_base*>(&x) != static_cast<sparse_polynom_base*>(this));

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.

    while(first != last)
        addsub(x, first++);
}


template <typename F, typename I, bool REFCNT>
template <bool REFCNT1>
void sparse_polynom<F, I, REFCNT>::addsub
(
    sparse_polynom<F, I, REFCNT1>& x,
    typename sparse_polynom<F, I, REFCNT1>::monom_iterator first,
    typename sparse_polynom<F, I, REFCNT1>::monom_iterator last,
    const any_monom_seq_t&
)
{
    // Предполагается, что F1 != F или I1 != I.
    // В противном случае должна вызываться другая специализация этой функции.
    // Типы 'Rep' для *this и для x различны, поэтому мы не можем перемещать
    // мономы между этими списками без копирования.

    ARAGELI_ASSERT_0(static_cast<sparse_polynom_base*>(&x) != static_cast<sparse_polynom_base*>(this));

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.

    while(first != last)
        addsub(x, first++);
}


template <typename F, typename I, bool REFCNT>
template <typename F1, typename I1, bool REFCNT1>
void sparse_polynom<F, I, REFCNT>::addsub
(
    sparse_polynom<F1, I1, REFCNT1>& x,
    typename sparse_polynom<F1, I1, REFCNT1>::monom_iterator first,
    typename sparse_polynom<F1, I1, REFCNT1>::monom_iterator last,
    const norm_monom_seq_t&
)
{
    // Предполагается, что F1 != F или I1 != I.
    // В противном случае должна вызываться другая специализация этой функции.
    // Типы 'Rep' для *this и для x различны, поэтому мы не можем перемещать
    // мономы между этими списками без копирования.

    ARAGELI_ASSERT_1(static_cast<sparse_polynom_base*>(&x) != static_cast<sparse_polynom_base*>(this));

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.

    while(first != last)
        addsub(x, first++);
}


template <typename F, typename I, bool REFCNT>
template <bool REFCNT1>
void sparse_polynom<F, I, REFCNT>::addsub
(
    sparse_polynom<F, I, REFCNT1>& x,
    typename sparse_polynom<F, I, REFCNT1>::monom_iterator first,
    typename sparse_polynom<F, I, REFCNT1>::monom_iterator last,
    const norm_monom_seq_t&
)
{
    // Предполагается, что F1 != F или I1 != I.
    // В противном случае должна вызываться другая специализация этой функции.
    // Типы 'Rep' для *this и для x различны, поэтому мы не можем перемещать
    // мономы между этими списками без копирования.

    ARAGELI_ASSERT_0(static_cast<sparse_polynom_base*>(&x) != static_cast<sparse_polynom_base*>(this));

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.

    while(first != last)
        addsub(x, first++);
}


template <typename F, typename I, bool REFCNT>
template <typename F1, typename I1, bool REFCNT1>
void sparse_polynom<F, I, REFCNT>::addsub
(
    sparse_polynom<F1, I1, REFCNT1>& x,
    const any_monom_seq_t&
)
{
    // Предполагается, что F1 != F или I1 != I.
    // В противном случае должна вызываться другая специализация этой функции.
    // Типы 'Rep' для *this и для x различны, поэтому мы не можем перемещать
    // мономы между этими списками без копирования.

    ARAGELI_ASSERT_1(static_cast<sparse_polynom_base*>(&x) != static_cast<sparse_polynom_base*>(this));

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.

    addsub(x, x.monoms_begin(), x.monoms_end(), any_monom_seq);
}


template <typename F, typename I, bool REFCNT>
template <bool REFCNT1>
void sparse_polynom<F, I, REFCNT>::addsub
(
    sparse_polynom<F, I, REFCNT1>& x,
    const any_monom_seq_t&
)
{
    // Предполагается, что F1 != F или I1 != I.
    // В противном случае должна вызываться другая специализация этой функции.
    // Типы 'Rep' для *this и для x различны, поэтому мы не можем перемещать
    // мономы между этими списками без копирования.

    ARAGELI_ASSERT_0(static_cast<sparse_polynom_base*>(&x) != static_cast<sparse_polynom_base*>(this));

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.

    addsub(x, x.monoms_begin(), x.monoms_end(), any_monom_seq);
}


template <typename F, typename I, bool REFCNT>
template <typename F1, typename I1, bool REFCNT1>
void sparse_polynom<F, I, REFCNT>::addsub
(
    sparse_polynom<F1, I1, REFCNT1>& x,
    const norm_monom_seq_t&
)
{
    // Предполагается, что F1 != F или I1 != I.
    // В противном случае должна вызываться другая специализация этой функции.
    // Типы 'Rep' для *this и для x различны, поэтому мы не можем перемещать
    // мономы между этими списками без копирования.

    ARAGELI_ASSERT_0(static_cast<sparse_polynom_base*>(&x) != static_cast<sparse_polynom_base*>(this));

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.

    addsub(x, x.monoms_begin(), x.monoms_end(), norm_monom_seq);
}


template <typename F, typename I, bool REFCNT>
template <bool REFCNT1>
void sparse_polynom<F, I, REFCNT>::addsub
(
    sparse_polynom<F, I, REFCNT1>& x,
    const norm_monom_seq_t&
)
{
    // Предполагается, что F1 != F или I1 != I.
    // В противном случае должна вызываться другая специализация этой функции.
    // Типы 'Rep' для *this и для x различны, поэтому мы не можем перемещать
    // мономы между этими списками без копирования.

    ARAGELI_ASSERT_0(static_cast<sparse_polynom_base*>(&x) != static_cast<sparse_polynom_base*>(this));

    // ПРЕДУПРЕЖДЕНИЕ. Простая, но медленная реализация, можно быстрее.

    addsub(x, x.monoms_begin(), x.monoms_end(), norm_monom_seq);
}


namespace _Internal
{

template <typename M>
struct is_monom_null
{
    bool operator() (const M& m)
    {
        return m.is_null();
    }
};

}


template
<typename F, typename I, bool REFCNT>
void sparse_polynom<F, I, REFCNT>::normalize ()
{
    //std::cerr << "\nPre unique: " << *this;
    store.unique();
    //std::cerr << "\nPre remove_if: " << *this;
    rep().remove_if(_Internal::is_monom_null<monom>());
    //std::cerr << "\nPre sort: " << *this;
    rep().sort(monom_degree_less<monom>());
    //std::cerr << "\nPre removing duplicated degrees: " << *this;

    for(typename Rep::iterator i = rep().begin();;)
    {
        i = std::adjacent_find
            (
                i,
                rep().end(),
                monom_degree_equal<monom>()
            );

        if(i == rep().end())
            break;

        typename Rep::iterator j = i;
        ++j;
        ARAGELI_ASSERT_1(j != rep().end());
        ARAGELI_ASSERT_1(!Arageli::is_null(*i));
        ARAGELI_ASSERT_1(!Arageli::is_null(*j));

        if((*j += *i).is_null())
            i = rep().erase(i);
        i = rep().erase(i);
    }

    //std::cerr << "\nAfter all: " << *this;

    ARAGELI_ASSERT_2(is_normal());
}


// WARNING!!!
template <typename T1, typename T2>
T1 sparse_polynom_subs_helper (T1 a, const T2& b)
{
    return a *= b;
}


// WARNING!!!
template <typename T1, bool REFCNT, typename T2>
vector<T1, REFCNT> sparse_polynom_subs_helper (vector<T1, REFCNT> a, const T2& b)
{
    return a = a*T1(b);
}


// WARNING!!!
template <typename T1, bool REFCNT, typename T2>
matrix<T1, REFCNT> sparse_polynom_subs_helper (matrix<T1, REFCNT> a, const T2& b)
{
    return a = a*T1(b);
}


// WARNING!!!
template <typename T1, bool REFCNT, typename T2, bool REFCNT2>
matrix<T1, REFCNT> sparse_polynom_subs_helper
(matrix<T1, REFCNT> a, const matrix<T2, REFCNT2>& b)
{
    return a = a*b;
}





template <typename F, typename I, bool REFCNT>
template <typename F1, typename Coef_factory>
F1 sparse_polynom<F, I, REFCNT>::subs (const F1& x, const Coef_factory& fctr) const
{
    ARAGELI_ASSERT_EX1_0(is_normal(), *this);

    // ПРЕДУПРЕЖДЕНИЕ. Это не схема Горнера.

    typedef typename Rep::const_iterator Iter;
    F1 xx = fctr.unit(x);
    F1 res = fctr.null(x);
    //std::cout << "\nxx = " << xx << "\nres = " << res;
    degree_type j = null<degree_type>();    // степень элемента x хранится в xx

    // ВНИМАНИЕ! КОНЦЕПТУАЛЬНОЕ ЗАМЕЧАНИЕ. При вычислении по схеме Горнера
    // требуется реверсивный обход по отношению к тому, который принят для
    // полиномов. Он будет не возможен, если будет использоваться
    // одноносвязный контейнер (на что указывается при определении Rep).

    for(Iter i = rep().begin(); i != rep().end(); ++i)
    {
        if(j < i->degree())
        {
            xx *= power(x, i->degree() - j, fctr);
            j = i->degree();
        }

        // WARNING!!!
        //F1 xxx = xx;
        //if(type_traits<F1>::is_rational)
        //    xxx *= i->coef();
        //else
        //    xxx *= (typename type_traits<F1>::element_type)i->coef();
        //res += /*F1(*/i->coef()/*)*/ * xx;
        res += sparse_polynom_subs_helper(xx, i->coef());
        //res += xxx;
        //std::cout << "\nxx = " << xx << "\nres = " << res;
    }

    return res;
}


template <typename F, typename I, bool REFCNT>
sparse_polynom<F, I, REFCNT> diff
(const sparse_polynom<F, I, REFCNT>& x)
{
    ARAGELI_ASSERT_EX1_0(x.is_normal(), x);

    typedef sparse_polynom<F, I, REFCNT> P;
    typedef typename P::monom_const_iterator Pmi;
    typedef typename P::monom Pm;
    typedef typename Pm::coef_type PC;

    if(x.is_null() || x.is_const())
        return P();

    P res;
    Pmi i = x.monoms_begin();
    if(is_null(i->degree()))
        ++i;
    for(; i != x.monoms_end(); ++i)
    {
        ARAGELI_ASSERT_1(!is_null(i->degree()));
        res += Pm(i->coef() * PC(i->degree()), i->degree() - 1);
    }

    return res;
}


template
<
    typename F,
    typename I,
    bool REFCNT,
    typename Ch,
    typename ChT
>
std::basic_ostream<Ch, ChT>& output_list
(
    std::basic_ostream<Ch, ChT>& out,
    const sparse_polynom<F, I, REFCNT>& x,
    monoms_order mo,
    const char* first_bracket,
    const char* second_bracket,
    const char* separator
)
{
    switch(mo)
    {
        case mo_inc:
            return polynom_output_list
            (
                out,
                _Internal::make_reverse_iterator(x.monoms_end()),
                _Internal::make_reverse_iterator(x.monoms_begin()),
                first_bracket,
                second_bracket,
                separator
            );
        case mo_dec:
            return polynom_output_list
            (
                out,
                x.monoms_begin(),
                x.monoms_end(),
                first_bracket,
                second_bracket,
                separator
            );
    }
}


template
<
    typename F,
    typename I,
    bool REFCNT,
    typename Ch,
    typename ChT
>
std::basic_ostream<Ch, ChT>& output_var
(
    std::basic_ostream<Ch, ChT>& out,
    const sparse_polynom<F, I, REFCNT>& x,
    monoms_order mo,
    const char* var,
    const char* mul,
    const char* pow
)
{
    ARAGELI_ASSERT_0(mo == mo_inc || mo == mo_dec);

    switch(mo)
    {
        case mo_inc:
            return polynom_output_var
            (
                out,
                _Internal::make_reverse_iterator(x.monoms_end()),
                _Internal::make_reverse_iterator(x.monoms_begin()),
                mo,
                var,
                mul,
                pow
            );
        case mo_dec:
            return polynom_output_var
            (
                out,
                x.monoms_begin(),
                x.monoms_end(),
                mo,
                var,
                mul,
                pow
            );
        default:
            return out;
    }
}


template
<
    typename F,
    typename I,
    bool REFCNT,
    typename Ch,
    typename ChT
>
std::basic_istream<Ch, ChT>& input_list
(
    std::basic_istream<Ch, ChT>& in,
    sparse_polynom<F, I, REFCNT>& x,
    const char* fb,
    const char* sb,
    const char* sep
)
{
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(fb));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(sb));
    ARAGELI_ASSERT_0(_Internal::is_not_contains_spaces(sep));
    ARAGELI_ASSERT_0(*sep);

    if(!_Internal::read_literal(in, fb))
        return in;
    typedef sparse_polynom<F, I, REFCNT> P;
    P tmp;
    typename P::monom m;
    if(!input_list(in, m))
        return in;
    tmp += m;

    while(_Internal::read_literal(in, sep))    // поэтому sep не должен быть ""
    {
        if(!input_list(in, m))
            return in;
        tmp += m;
    }

    if(!_Internal::read_literal(in, sb))
        return in;
    x = tmp;

    return in;
}


template
<
    typename F,
    typename I,
    bool REFCNT,
    typename Ch,
    typename ChT,
    typename Coef_factory
>
std::basic_istream<Ch, ChT>& input_var
(
    std::basic_istream<Ch, ChT>& in,
    sparse_polynom<F, I, REFCNT>& x,
    const Coef_factory& fctr,
    const char* var,
    const char* mul,
    const char* pow
)
{
    sparse_polynom<F, I, REFCNT> res;

    char ch = 0;
    in >> ch;
    bool brackets = false;
    if(ch == '(')
        brackets = true;
    else
        in.putback(ch);

    typedef typename sparse_polynom<F, I, REFCNT>::monom monom;
    monom m;
    input_var(in, m, true, fctr, var, mul, pow);

    if(!in)
    {
        if(in.eof())
        {
            if(brackets)
                in.clear(std::ios_base::badbit);
            else
                x = m;
        }

        return in;
    }

    ch = in.get();

    if(in.eof())
    {
        if(brackets)
            in.clear(std::ios_base::badbit);
        else
        {
            x = m;
            in.clear();
        }

        return in;
    }

    in.clear();
    in.putback(ch);

    if(ch == '\n')
    {
        if(brackets)
            in.clear(std::ios_base::badbit);
        else
        {
            x = m;
            in.clear();
        }
        return in;
    }

    res += m;

    for(;;)
    {
        m = monom();
        input_var(in, m, false, fctr, var, mul, pow);

        if(!in)
        {
            if(in.eof())
                res += m;
            break;
        }

        ch = in.get();

        if(in.eof())
        {
            if(brackets)
            {
                in.clear(std::ios_base::badbit);
                return in;
            }

            res += m;
            break;
        }

        in.clear();
        in.putback(ch);

        if(ch == '\n')
        {
            if(brackets)
            {
                in.clear(std::ios_base::badbit);
                return in;
            }

            res += m;
            break;
        }

        res += m;
    }

    in.clear();
    if(brackets && !_Internal::read_literal(in, ")"))
        in.clear(std::ios_base::badbit);
    else
    {
        x = res;
        in.clear();
    }

    return in;
}


template <typename F, typename I>
std::istream& input_polynom_first
(std::istream& in, monom<F, I>& x)
{
    char fb = 0, sb = 0;
    monom<F, I> res;
    in >> fb >> res >> sb;
    if(fb != '(' || sb != ')')
    {
        in.clear(std::istream::badbit);
        return in;
    }

    x = res;
    return in;
}


template <typename F, typename I>
std::istream& input_polynom_internal
(std::istream& in, monom<F, I>& x)
{
    char sign = 0, fb = 0, sb = 0;
    monom<F, I> res;
    in >> sign >> fb >> res >> sb;
    if(sign != '+' || sign != '-' || fb != '(' || sb != ')')
    {
        in.clear(std::istream::badbit);
        return in;
    }

    x = res;
    if(sign == '-')
        x.opposite();
    return in;
}


template <typename F, typename I, bool REFCNT>
std::istream& input_polynom_first
(std::istream& in, sparse_polynom<F, I, REFCNT>& x)
{
    char fb = 0, sb = 0;
    sparse_polynom<F, I, REFCNT> res;
    in >> fb >> res >> sb;
    if(fb != '(' || sb != ')')
    {
        in.clear(std::istream::badbit);
        return in;
    }

    x = res;
    return in;
}


template <typename F, typename I, bool REFCNT>
std::istream& input_polynom_internal
(std::istream& in, sparse_polynom<F, I, REFCNT>& x)
{
    char sign = 0, fb = 0, sb = 0;
    sparse_polynom<F, I, REFCNT> res;
    in >> sign >> fb >> res >> sb;
    if((sign != '+' && sign != '-') || fb != '(' || sb != ')')
    {
        in.clear(std::istream::badbit);
        return in;
    }

    x = res;
    if(sign == '-')
        x.opposite();
    return in;
}


template <typename F, typename I, bool REFCNT>
template <typename Stream>
Stream& io_binary<sparse_polynom<F, I, REFCNT> >::output_stream
(
    Stream& out,
    const sparse_polynom<F, I, REFCNT>& x
)
{
    const std::size_t size = x.size();
    output_binary_stream(out, size);

    if(size)
    {
        // Output all monomials.

        typedef typename sparse_polynom<F, I, REFCNT>::monom_const_iterator mci;
        mci begin = x.monoms_begin(), end = x.monoms_end();

        // WARNING! Use some special iterator and a standard algorithm
        // instead of the loop here.

        for(mci i = begin; i != end; ++i)
            output_binary_stream(out, *i);
    }

    return out;
}


template <typename F, typename I, bool REFCNT>
template <typename Stream>
Stream& io_binary<sparse_polynom<F, I, REFCNT> >::input_stream
(
    Stream& in,
    sparse_polynom<F, I, REFCNT>& x
)
{
    // If an error occurs when the size is being read,
    // we won't lose an old value of x.
    std::size_t size;
    if(!input_binary_stream(in, size))
        return in;

    // The following statement loses an old value of x, and
    // if an error occure during next reads we will return
    // a meaningless value in x.

    x = null(x);    // WARNING! Use some special function instead.

    // Read all monomials numbered `size' and insert them all
    // to x in the same order.

    typedef typename sparse_polynom<F, I, REFCNT>::monom_iterator mi;
    mi end = x.monoms_end();

    for(std::size_t i = 0; i < size; ++i)
    {
        typedef typename sparse_polynom<F, I, REFCNT>::monom monom;
        monom m;
        if(!input_binary_stream(in, m))
            return in;  // reading fails
        x.insert(end, m);
    }

    return in;
}


template <typename F, typename I, bool REFCNT>
std::size_t io_binary<sparse_polynom<F, I, REFCNT> >::calc
(const sparse_polynom<F, I, REFCNT>& x)
{
    std::size_t size = x.size();
    std::size_t res = calc_binary(size);

    if(size)
    {
        // Calculate for all monomials.

        typedef typename sparse_polynom<F, I, REFCNT>::monom_const_iterator mci;
        mci begin = x.monoms_begin(), end = x.monoms_end();

        // WARNING! Use some special iterator and a standard algorithm
        // instead of the loop here.

        for(mci i = begin; i != end; ++i)
            res += calc_binary(*i);
    }

    return res;
}


} // namespace Arageli


#else


#include "sparse_polynom.hpp"

namespace Arageli
{

const char* monom_input_list_first_bracket_default = "(";
const char* monom_output_list_first_bracket_default = "(";
const char* monom_input_list_second_bracket_default = ")";
const char* monom_output_list_second_bracket_default = ")";
const char* monom_input_list_separator_default = ",";
const char* monom_output_list_separator_default = ", ";
const char* monom_input_var_mul_default = "*";
const char* monom_output_var_mul_default = "*";
const char* monom_input_var_var_default = "x";
const char* monom_output_var_var_default = "x";
const char* monom_input_var_pow_default = "^";
const char* monom_output_var_pow_default = "^";

}

#endif
