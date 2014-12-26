/*****************************************************************************

    polynom.hpp

    This file is a part of the Arageli library.

    Created 2005/9/30 8:38 by Andrey Somsikov.
    Significantly rewrited by Sergey Lyalin.

    Copyright (C) 2005 Andrey Somsikov
    Copyright (C) 2006--2007, 2010 Sergey S. Lyalin

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
    \file polynom.hpp
    \brief Polynom implementation.

    This module contains implementation of polynomial with dense representation.
*/

#ifndef _ARAGELI_polynom_hpp_
#define _ARAGELI_polynom_hpp_

#include <cstddef>
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cmath>

#include "config.hpp"
#include "type_opers.hpp"
#include "type_traits.hpp"
#include "factory.hpp"
#include "powerest.hpp"
#include "exception.hpp"
#include "frwrddecl.hpp"
#include "rational.hpp"
#include "vector.hpp"
#include "sparse_polynom.hpp"


namespace Arageli
{

/// Default set of configuration switches for polynom template class.
template <typename F, bool REFCNT>
struct polynom_config_default
{
    typedef vector<F, REFCNT> storage_type;

    template <typename F1> struct other_coef
    {
        typedef polynom_config_default<F1, REFCNT> type;
    };
};


template <typename F, typename Config>
class io_binary<polynom<F, Config> >;   // see implementation below in this file


/// Polynomial template class with the dense representation.
/** Implementation of a dense representation for polynomials.
    An instance of this type is a polynom where the
    coefficients are of type F. */
template <typename F, typename Config>
class polynom
{
    template <typename F1, typename Config1>
    friend class polynom;

    typedef typename Config::storage_type storage_type;

    friend class io_binary<polynom<F, Config> >;


public:

    /// Structure with set of configuration switches.
    /** The default type for this is polynom_config_default. */
    typedef Config config_type;

    /// Coefficient type.
    typedef F coef_type;

    /// Type for representing size of polynomial.
    typedef typename storage_type::size_type size_type;

    /// Degree type.
    typedef typename signed_type<size_type>::type degree_type;

    /// Defines a polynomial with different coefficient type.
    template <typename F1> struct other_coef
    {
        typedef
            polynom
            <
                F1,
                typename Config::template other_coef<F1>::type
            >
                type;
    };


    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    /// @name Constructors.

    //@{

    /// Zero polynomial. A polynomial with zero length.
    polynom ()
    {}

    /// Polynomial as a constant val.
    /** If is_null(val) than the result is equalent to polynom(). */
    template <typename Val>
    polynom (const Val& val, const fromval_t&)
    {
        if(!Arageli::is_null(val))
            store.assign_fromval(1, val);
    }

    /// Polynomial consisted one monom -- val*x^p.
    /** If is_null(x) than the result is equalent to polynom(). */
    template <typename Val, typename Deg>
    polynom (const Val& val, const Deg& p, const fromval_t&)
    {
        if(!Arageli::is_null(val))
        {
            store.reserve(p+1);
            store.assign_fromsize(p);
            store.push_back(val);
        }
    }

    /// Creates polynomial from a vector of coefficients.
    template <typename Vec>
    polynom (const Vec& vec, const fromvec_t&) :
        store(vec, fromvec)
    {}

    /// Creates polynomial from a sequence of coefficients from lower degrees to greater.
    template <typename Seq>
    polynom (size_type size_a, const Seq& seq, const fromvec_t&) :
        store(size_a, seq, fromseq)
    {}

    /// TEMPORARY not mixed copy constructor.
    template <typename T1, typename Config1>
    polynom (const polynom<T1, Config1>& x) :
        store(x.store)
    {}

    /// TEMPORARY not mixed convertion constructor from sparse_polynom.
    template <typename T1, typename D1, bool REFCNT1>
    polynom (const sparse_polynom<T1, D1, REFCNT1>& x) :
        store(x.degree() + 1)
    {
        ARAGELI_ASSERT_0(x.is_normal());
        typedef typename sparse_polynom<T1, D1, REFCNT1>::monom_const_iterator Mci;

        for(Mci i = x.monoms_begin(); i != x.monoms_end(); ++i)
            store[i->degree()] = i->coef();
    }

    polynom (const char* str);

    //@}


    /// TEMPORARY not mixed assign operator.
    template <typename T1, typename Config1>
    polynom& operator= (const polynom<T1, Config1>& x)
    {
        store = x.store;
        return *this;
    }


    /// Assign polynomial from vector.
    template <typename Vec>
    void assign_fromvec (const Vec& vec)
    {
        store = vec;
    }


    /// Returns true iff this is zeroth polynomial.
    /** I.e. returns true iff polynomial does not containe any monoms. */
    bool is_null () const
    {
        return store.is_null();
    }


    /// Returns true if polynomial has only absolute term that equals to 1.
    bool is_unit () const
    {
        return
            !store.is_empty() &&
            Arageli::is_unit(store.front()) &&
            only_lead_nulls(1);
    }


    /// Returns true if polynomial has only absolute term that equal to -1.
    bool is_opposite_unit () const
    {
        return
            !store.is_empty() &&
            Arageli::is_opposite_unit(store.front()) &&
            only_lead_nulls(1);
    }


    /// Returns true if polynomial have no monoms with positive degree of x.
    bool is_const () const
    {
        return
            !store.is_empty() &&
            only_lead_nulls(1);
    }


    /// Returns true if polynomial is "x".
    /** Returns true if polynomial contains only one monom with degree = 1
        and coefficient = 1. */
    bool is_x () const
    {
        return
            !store.is_empty() &&
            Arageli::is_null(store[0]) &&
            Arageli::is_unit(store[1]) &&
            only_lead_nulls(2);
    }


    /// Returns degree of polynomial (greatest degree value).
    /** For zeroth polynomial returns -1. */
    degree_type degree () const
    {
        degree_type i;

        for
        (
            i = store.size() - 1;
            i >= 0 && Arageli::is_null(store[i]);
            --i
        );

        return i;
    }


    /// Returns size of allocated vector with coefficients.
    /** Several leading coefficients can be equal to zero.
        To obtain degree of polynomial use degree function. */
    size_type size () const
    {
        return store.size();
    }


    /// Returns true iff size() == 0.
    bool is_empty () const
    {
        return size() == 0;
    }


    /// Returns reference to nonzero coefficient of monom with the greatest degree.
    /** Requirement: !is_null(). */
    coef_type& leading_coef ()
    {
        degree_type deg = degree();
        ARAGELI_ASSERT_0(deg > -1);
        return store[deg];
    }


    /// Returns reference to nonzero coefficient of monom with the greatest degree.
    /** Requirement: !is_null(). */
    const coef_type& leading_coef () const
    {
        degree_type deg = degree();
        ARAGELI_ASSERT_0(deg > -1);
        return store[deg];
    }

    /// Returns a copy of coefficient of monom with the greatest degree.
    /** For zeroth polynomial it returns 0. */
    coef_type leading_coef_cpy () const
    {
        degree_type deg = degree();
        if(deg > -1)return store[deg];
        else return null<coef_type>();
    }

    /// Returns a copy of coefficient of monom with the greatest degree.
    /** For zeroth polynomial returns zero from factory. */
    template <typename Factory_coef>
    coef_type leading_coef_cpy (const Factory_coef& fctr) const
    {
        degree_type deg = degree();
        if(deg > -1)return store[deg];
        else return fctr.null();
    }


    /// Returns reference to this polynomial. For sameness only.
    const polynom& operator+ () const
    {
        return *this;
    }

    /// Opposites all coefficients of polynomial.
    polynom& opposite ()
    {
        store.opposite();
        return *this;
    }

    /// Returns opposite polynomial.
    polynom operator- () const
    {
        polynom t(*this);
        return t.opposite();
    }


    /// For this polynomial f, performs f = f + 1.
    polynom& operator++ ()
    {
        if(is_empty())
            store.push_back(Arageli::unit<coef_type>());
        else
            ++store.front();
        return *this;
    }

    /// For this polynomial f, performs f = f + 1.
    polynom operator++ (int)
    {
        polynom t = *this;
        operator++();
        return t;
    }

    /// For this polynomial f, performs f = f - 1.
    polynom& operator-- ()
    {
        if(is_empty())
            store.push_back(Arageli::opposite_unit<coef_type>());
        else
            --store.front();
        return *this;
    }

    /// For this polynomial f, performs f = f - 1.
    polynom operator-- (int)
    {
        polynom t = *this;
        operator--();
        return t;
    }


    /// @name Standard mathematical operations with two polynomials.
    /** Each function returns reference to this polynomial. */
    // @{

    template <typename F1, typename Config1>
    polynom& operator+= (const polynom<F1, Config1>& x)
    {
        if(x.is_empty())
            return *this;

        store.resize(std::max(x.size(), size()), null<coef_type>());
        for(size_type i = 0; i < x.size(); ++i)
            store[i] += x.store[i];

        erase_leading_nulls();
        return *this;
    }

    template <typename F1, typename Config1>
    polynom& operator-= (const polynom<F1, Config1>& x)
    {
        if(x.is_empty())
            return *this;

        store.resize(std::max(x.size(), size()), null<coef_type>());
        for(size_type i = 0; i < x.size(); ++i)
            store[i] -= x.store[i];

        erase_leading_nulls();
        return *this;
    }

    template <typename F1, typename Config1>
    polynom& operator*= (const polynom<F1, Config1>& x)
    {
        // Classical n^2 algorithm.

        erase_leading_nulls();
        if(is_null() || x.is_null())
        {
            store.assign();
            return *this;
        }

        polynom res;
        res.store.assign_fromsize(size() + x.size());

        for(size_type i = 0; i < size(); ++i)
            for(size_type j = 0; j < x.size(); ++j)
                res.store[i+j] += store[i]*x.store[j];

        res.erase_leading_nulls();
        return *this = res;
    }

    template <typename F1, typename Config1>
    polynom& operator/= (const polynom<F1, Config1>& x);

    template <typename F1, typename Config1>
    polynom& operator%= (const polynom<F1, Config1>& x);

    // @}


    void erase_leading_nulls ()
    {
        typedef typename storage_type::iterator Iter;
        typedef std::reverse_iterator<Iter> RIter;

        Iter i =
            std::find_if
            (
                RIter(store.end()),
                RIter(store.begin()),
                std::not1(func::is_null<F>())
            ).base();

        ARAGELI_ASSERT_1(store.begin() <= i);
        ARAGELI_ASSERT_1(i <= store.end());

        store.erase(i, store.end());
    }


    void resize (size_type size_a)
    {
        store.resize(size_a);
    }

    template <typename Val>
    void resize (size_type size_a, const Val& val)
    {
        store.resize(size_a, val);
    }


    coef_type& operator[] (size_type i)
    {
        ARAGELI_ASSERT_0(i < size());
        return store[i];
    }

    const coef_type& operator[] (size_type i) const
    {
        ARAGELI_ASSERT_0(i < size());
        return store[i];
    }


    template <typename Vec>
    Vec& coef_vector (Vec& vec) const
    {
        vec = store;
        return vec;
    }

    vector<coef_type> coef_vector () const
    {
        return store;
    }


private:

    bool only_lead_nulls (size_type i) const
    {
        return
            std::find_if
            (
                store.begin() + i,
                store.end(),
                std::not1(func::is_null<F>())
            ) == store.end();
    }

    storage_type store;
};


template <typename F, typename Config>
struct type_traits<polynom<F, Config> > :
    public type_traits
    <
        sparse_polynom<F, typename polynom<F, Config>::degree_type>
    >
{
    typedef type_category::dense_polynom category_type;
    static const category_type category_value;
};


template <typename F, typename Config>
const type_category::dense_polynom
type_traits<polynom<F, Config> >::category_value =
    type_category::dense_polynom();


template <typename TT, typename Config>
struct factory<polynom<TT, Config> >
{
private:

    typedef polynom<TT, Config> T;

public:
    static const bool is_specialized = true;

    static const T& unit ()
    {
        static const T unit_s =
            T(Arageli::unit<TT>(), fromval);
        return unit_s;
    }

    static const T& unit (const T& x)
    {
        return unit();
    }

    static const T& opposite_unit ()
    {
        static const T opposite_unit_s =
            T(Arageli::opposite_unit<TT>(), fromval);
        return opposite_unit_s;
    }

    static const T& opposite_unit (const T& x)
    {
        return opposite_unit();
    }

    static const T& null ()
    {
        static const T null_s;
        return null_s;
    }

    static const T& null (const T& x)
    {
        return null();
    }

};


template
<
    typename F1, typename Config1,
    typename F2, typename Config2,
    typename Q, typename R
>
void divide
(
    polynom<F1, Config1> x1,
    polynom<F2, Config2> x2,
    Q& q, R& r
)
{
    x1.erase_leading_nulls();
    x2.erase_leading_nulls();

    if(x1.size() < x2.size())
    {
        q = null<Q>();
        r = x1;
        return;
    }

    q.resize(x1.size() - x2.size() + 1, null<typename Q::coef_type>());

    r = x1;

    typedef typename R::size_type size_type;
    typedef F1 F;

    size_type rSize = r.size();

    while(rSize >= x2.size())
    {
        F qValue = r[rSize - 1] / x2[x2.size()-1];
        q[rSize - x2.size()] = qValue;
        for(size_type i = 0, j = rSize - x2.size(); i < x2.size(); i++, j++)
            r[j] -= qValue * x2[i];
        ARAGELI_ASSERT_0(is_null(r[rSize - 1]))
        rSize--;
    }

    q.erase_leading_nulls();
    r.erase_leading_nulls();
}


template <typename F, typename Config>
template <typename F1, typename Config1>
inline polynom<F, Config>& polynom<F, Config>::operator/=
(const polynom<F1, Config1>& x)
{
    polynom<F, Config> q, r;
    divide(*this, x, q, r);
    return *this = q;
}


template <typename F, typename Config>
template <typename F1, typename Config1>
inline polynom<F, Config>& polynom<F, Config>::operator%=
(const polynom<F1, Config1>& x)
{
    polynom<F, Config> q, r;
    divide(*this, x, q, r);
    return *this = r;
}


template <typename In, typename F, typename Config>
In& input_list
(
    In& in,
    polynom<F, Config>& x,
    const char* first_bracket = monom_input_list_first_bracket_default,
    const char* second_bracket = monom_input_list_second_bracket_default,
    const char* separator = monom_input_list_separator_default
)
{
    vector<F, false> t;
    input_list(in, t, first_bracket, second_bracket, separator);
    if(in)x = polynom<F, Config>(t, fromvec);
    return in;
}


template <typename Out, typename F, typename Config>
Out& output_list
(
    Out& out,
    const polynom<F, Config>& x,
    const char* first_bracket = monom_input_list_first_bracket_default,
    const char* second_bracket = monom_input_list_second_bracket_default,
    const char* separator = monom_input_list_separator_default
)
{
    output_list(out, x.coef_vector(), first_bracket, second_bracket, separator);
    return out;
}


template <typename In, typename F, typename Config, typename Coef_factory>
In& input_var
(
    In& in,
    polynom<F, Config>& x,
    const Coef_factory& fctr,
    const char* var = monom_input_var_var_default,
    const char* mul = monom_input_var_mul_default,
    const char* pow = monom_input_var_pow_default
)
{
    // WARNING! Temporary implementation through sparse_polynom.
    sparse_polynom<F, typename polynom<F, Config>::degree_type, false> t;
    input_var(in, t, fctr, var, mul, pow);
    if(in)x = t;
    return in;
}


template <typename In, typename F, typename Config>
In& input_var
(
    In& in,
    polynom<F, Config>& x,
    const char* var = monom_input_var_var_default,
    const char* mul = monom_input_var_mul_default,
    const char* pow = monom_input_var_pow_default
)
{
    return input_var(in, x, factory<F>(), var, mul, pow);
}


/// Outputting of polynomial with variable symbol.
/** Выводит полином в привычном виде, с указанием формальной переменной. */
template <typename Out, typename F, typename Config>
Out& output_var
(
    Out& out,
    const polynom<F, Config>& x,
    monoms_order mo = mo_inc,
    const char* var = monom_output_var_var_default,
    const char* mul = monom_output_var_mul_default,
    const char* pow = monom_output_var_pow_default
)
{
    // WARNING! Temporary implementation through sparse_polynom.
    output_var
    (
        out,
        sparse_polynom<F, typename polynom<F, Config>::degree_type>(x),
        mo,
        var,
        mul,
        pow
    );

    return out;
}


/// Standard way to output polynom. It's the same as polynom_output_var.
template <typename Out, typename F, typename Config>
Out& operator<< (Out& out, const polynom<F, Config>& x)
{
    return output_var(out, x);
}

/// Standard way to input polynom. It's the same as polynom_input_var.
template <typename In, typename F, typename Config>
In& operator>> (In& in, polynom<F, Config>& x)
{
    return input_var(in, x);
}


/// Specialization of io_binary for polynom.
template <typename F, typename Config>
class io_binary<polynom<F, Config> > :
    public io_binary_base<polynom<F, Config> >
{
public:

    using io_binary_base<polynom<F, Config> >::output_stream;
    using io_binary_base<polynom<F, Config> >::input_stream;
    using io_binary_base<polynom<F, Config> >::calc;
    using io_binary_base<polynom<F, Config> >::input_mem;
    using io_binary_base<polynom<F, Config> >::output_mem;

    /// Stores polynom object state to a binary stream. Seft-delimeted binary serialization.
    /** This functions just stores an object x as x.store
        in The Simple Binary format. */
    template <typename Stream>
    static inline Stream& output_stream (Stream& out, const polynom<F, Config>& x)
    {
        output_binary_stream(out, x.store);
        return out;
    }


    /// Loads polynom object state from a binary stream. Compatible with output_stream.
    /** See output_stream(stream, polynom) function for detailes on the format.
        If the function fails to read some of state components, an old value of x
        may be lost. All depends on input_binary_stream function for x.store.
        So, do not relay on the value of x when a given stream is not in a good state
        after call returns.

        The function takes input in The Simple Binary format.
    */
    template <typename Stream>
    static Stream& input_stream (Stream& in, polynom<F, Config>& x)
    {
        input_binary_stream(in, x.store);
        return in;
    }


    /// Calculates the number of chars required to store a given polynom object in The Simple Binary form.
    /** This function calculates precise number of chars that will emit
        any function outputs in The Simple Binary format for one rational object,
        for example, output_binary_mem function. */
    static inline std::size_t calc (const polynom<F, Config>& x)
    {
        return calc_binary(x.store);
    }
};


template <typename F, typename Config>
polynom<F, Config>::polynom (const char* str)
{
    std::istringstream buf(str);
    buf >> *this;
}


#define _ARAGELI_POLYNOM_BINOPER(AOPER, BOPER)    \
    template    \
    <    \
        typename F1,    \
        typename Config1,    \
        typename F2,    \
        typename Config2    \
    >    \
    inline polynom<F1, Config1> operator BOPER    \
    (    \
        const polynom<F1, Config1>& x,    \
        const polynom<F2, Config2>& y    \
    )    \
    {    \
        polynom<F1, Config1> t(x);    \
        return t AOPER y;    \
    }    \
    \
    template    \
    <    \
        typename F1,    \
        typename Config1,    \
        typename F2    \
    >    \
    inline polynom<F1, Config1> operator BOPER    \
    (    \
        polynom<F1, Config1> x,    \
        const F2& y    \
    )    \
    {    \
        polynom<F1, Config1> t(y, fromval);    \
        return x AOPER t;    \
    }    \
    \
    template    \
    <    \
        typename F1,    \
        typename F2,    \
        typename Config2    \
    >    \
    inline polynom<F2, Config2> operator BOPER    \
    (    \
        const F1& x,    \
        const polynom<F2, Config2>& y    \
    )    \
    {    \
        polynom<F2, Config2> t(x, fromval);    \
        return t AOPER y;    \
    }    \
    /* BEGIN OF TEMPORARY EXTENTION */    \
    template <typename F1, typename Config1>    \
    inline polynom<rational<F1>, Config1> operator BOPER    \
    (    \
        polynom<rational<F1>, Config1> x,    \
        const rational<F1>& y    \
    )    \
    {    \
        polynom<rational<F1>, Config1> t(y, fromval);    \
        return x AOPER t;    \
    }    \
    \
    template <typename F1, typename Config1>    \
    inline polynom<rational<F1>, Config1> operator BOPER    \
    (    \
        const rational<F1>& x,    \
        const polynom<rational<F1>, Config1>& y    \
    )    \
    {    \
        polynom<rational<F1>, Config1> t(x, fromval);    \
        return t AOPER y;    \
    }


_ARAGELI_POLYNOM_BINOPER(+=, +)
_ARAGELI_POLYNOM_BINOPER(-=, -)
_ARAGELI_POLYNOM_BINOPER(*=, *)
_ARAGELI_POLYNOM_BINOPER(/=, /)
_ARAGELI_POLYNOM_BINOPER(%=, %)



template
<
    typename F1,
    typename Config1,
    typename F2,
    typename Config2
>
inline int cmp
(
    polynom<F1, Config1> a,
    polynom<F2, Config2> b
)
{
    vector<F1> va = a.coef_vector();
    vector<F2> vb = b.coef_vector();
    if(va.size() > vb.size())
        vb.resize(va.size(), null<F2>());
    else if(va.size() < vb.size())
        va.resize(vb.size(), null<F1>());

    typedef std::reverse_iterator<typename vector<F1>::const_iterator> RI_a;
    typedef std::reverse_iterator<typename vector<F2>::const_iterator> RI_b;

    int res = _Internal::aggregate_cmp
    (
        RI_a(va.end()), RI_a(va.begin()),
        RI_b(vb.end()), RI_b(vb.begin())
    );

    return res;
}


#define _ARAGELI_POLYNOMIAL_CMP(OPER)    \
    template    \
    <    \
        typename F1,    \
        typename Config1,    \
        typename F2,    \
        typename Config2    \
    >    \
    inline bool operator OPER    \
    (    \
        const polynom<F1, Config1>& a,    \
        const polynom<F2, Config2>& b    \
    )    \
    {    \
        return cmp(a, b) OPER 0;    \
    }    \
    \
    template    \
    <    \
        typename F1,    \
        typename Config1,    \
        typename F2    \
    >    \
    inline bool operator OPER    \
    (    \
        const polynom<F1, Config1>& a,    \
        const F2& b    \
    )    \
    {    \
        return cmp(a, polynom<F2, Config1>(b, fromval)) OPER 0;    \
    }    \
    \
    template    \
    <    \
        typename F1,    \
        typename F2,    \
        typename Config2    \
    >    \
    inline bool operator OPER    \
    (    \
        const F1& a,    \
        const polynom<F2, Config2>& b    \
    )    \
    {    \
        return cmp(polynom<F1, Config2>(a, fromval), b) OPER 0;    \
    }


/// @name Standard comparision operators that based on cmp function.
//@{

_ARAGELI_POLYNOMIAL_CMP(==)
_ARAGELI_POLYNOMIAL_CMP(!=)
_ARAGELI_POLYNOMIAL_CMP(<=)
_ARAGELI_POLYNOMIAL_CMP(>=)
_ARAGELI_POLYNOMIAL_CMP(<)
_ARAGELI_POLYNOMIAL_CMP(>)

//@}


#if 0

template <typename T, typename FT>
std::istream& operator>> (std::istream& out, polynom<T>& x)
{
    return input_list(out, x);
}


template <typename F, typename FT>
std::ostream& operator << (std::ostream & s, const polynom<F> &v)
{
    std::size_t i = v.deg() + 1;
    s << "(";
    do
    {
        i--;
        s << v[i] << ", ";
    } while(i != 0);
    s << ")";
    /*
    if (s.fail ())
    matrix_error (st_output_error, "polynom::output error");
    */
    return s;
}

template <typename F, typename FT>
polynom<F> abs(const polynom<F> &maxVal)
{
    polynom<F> res(factory<coef_type>::unit(), maxVal.deg());
    for (std::size_t i = 0; i < res.size(); i++)
        res[i] = abs(maxVal[i]);
    return res;
}

/** Normalize polynom
    Normalize polynom to ensure that coefficient with high degree equal to identity.
    The resulting polynom is equal to the given polynom.
    @param value the polynom to normalize
    @return normalized polynom equal to the value
*/
template <typename F, typename FT>
polynom<F> normalize(const polynom<F> &val)
{
    polynom<F> res(val);
    F d = val[val.size()-1];
    for (std::size_t i = 0; i < val.size(); i++)
        res[i] = val[i] / d;
    return res;
}

#endif

} // namespace Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_POLYNOM
    #include "polynom.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_POLYNOM
#endif

#endif
