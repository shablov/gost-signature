/*****************************************************************************

    residue.hpp -- Residue representation with dynamically set module.

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
    This file contains declaration of class template residue for representation
    residue. The module for each object set dynamically when object will
    be created.

    WARNING. It's implementation in "old style" because it doesn't use
    mixing computations. Be careful: in the neares future we plan to replace
    it to modern one with full support of the mixing computation engine.
*/

#ifndef _ARAGELI_residue_hpp_
#define _ARAGELI_residue_hpp_

#include "config.hpp"

#include <iostream>
#include <sstream>
#include <limits>

#include "_utility.hpp"
#include "factory.hpp"
#include "intalg.hpp"

#include "std_import.hpp"


namespace Arageli
{


template <typename T, typename M>
struct residue_config_default
{
    static void init (T& x, const M& m)
    {
        normalize(x, m);
    }

    static void get_value (const T& x, const M& m)
    {}

    static void before_oper (const T& x, const M& m)
    {}

    static void after_oper (T& x, const M& m)
    {
        normalize(x, m);
    }

    static bool is_normal (const T& x, const M& m)
    {
        return is_null(m) || is_null(x/m);
    }

    static void normalize (T& x, const M& m)
    {
        if(!is_null(m))x = prrem(x, m);
    }
};


/// Residue class.
/** If T is a number, it should be a signed type. */
template
<
    typename T,
    typename M = T,
    typename Config = residue_config_default<T, M>
>
class residue
{
    mutable T value_m;    // resudue value itself
    M module_m;    // module value
    Config config_m;    // configurator

    template <typename T2, typename M2, typename Config2>
    friend class residue;

public:

    typedef T value_type;
    typedef M module_type;
    typedef Config config_type;

    /// Creates common zero. Without module assigned.
    residue () :
        value_m(null<T>()),
        module_m(null<M>())
    {
        ARAGELI_ASSERT_0
        (
            !std::numeric_limits<T>::is_specialized ||
            std::numeric_limits<T>::is_signed
        );
    }

    /// Creates common object of type T. Without module assigned.
    template <typename T1>
    residue (const T1& x) :
        value_m(x),
        module_m(null<M>())
    {
        ARAGELI_ASSERT_0
        (
            !std::numeric_limits<T>::is_specialized ||
            std::numeric_limits<T>::is_signed
        );
    }

    // Special copy constructor.
    template <typename T1>
    residue (const residue<T1>& x) :
        value_m(x.value()),
        module_m(x.module())
    {
        ARAGELI_ASSERT_0
        (
            !std::numeric_limits<T>::is_specialized ||
            std::numeric_limits<T>::is_signed
        );

        config_m.init(value_m, module_m);
    }

    // Creates residue from the division of x by m.
    residue (const T& x, const M& m) : value_m(x), module_m(m)
    {
        config_m.init(value_m, module_m);
    }


    residue (const char* s);


    /// Access to the residue value itself. Read only.
    const T& value () const
    {
        config_m.get_value(value_m, module_m);
        return value_m;
    }

    /// Access to the module value. Read only.
    const M& module () const
    {
        return module_m;
    }

    /// Access to the residue value itself.
    T& value ()
    {
        config_m.get_value(value_m, module_m);
        return value_m;
    }

    /// Access to the module value.
    M& module ()
    {
        return module_m;
    }


    /// Check if value is correct residue value modulo module.
    bool is_normal () const
    {
        return config_m.is_normal(value_m, module_m);
    }

    /// Performs normalization of residue value.
    void normalize () const
    {
        config_m.normalize(value_m, module_m);
    }


    residue operator- () const
    {
        config_m.before_oper(value_m, module_m);
        return residue(-value_m, module_m);
    }

    const residue& operator+ () const
    {
        return *this;
    }

    residue& operator++ ()
    {
        config_m.before_oper(value_m, module_m);
        ++value_m;
        config_m.after_oper(value_m, module_m);
        return *this;
    }

    residue& operator-- ()
    {
        config_m.before_oper(value_m, module_m);
        --value_m;
        config_m.after_oper(value_m, module_m);
        return *this;
    }

    residue operator++ (int)
    {
        residue t = *this;
        operator++();
        return t;
    }

    residue operator-- (int)
    {
        residue t = *this;
        operator--();
        return t;
    }

    template <typename T1, typename M1, typename Config1>
    residue& operator+= (const residue<T1, M1, Config1>& x)
    {
        ARAGELI_ASSERT_0
        (
            module_m == x.module_m ||
            is_null(x.module_m) ||
            is_null(module_m)
        );

        config_m.before_oper(value_m, module_m);

        value_m += x.value_m;
        if(is_null(module_m) && !is_null(x.module_m))
            module_m = x.module_m;

        config_m.after_oper(value_m, module_m);
        return *this;
    }

    template <typename T1, typename M1, typename Config1>
    residue& operator-= (const residue<T1, M1, Config1>& x)
    {
        ARAGELI_ASSERT_0
        (
            module_m == x.module_m ||
            is_null(x.module_m) ||
            is_null(module_m)
        );

        config_m.before_oper(value_m, module_m);

        value_m -= x.value_m;
        if(is_null(module_m) && !is_null(x.module_m))
            module_m = x.module_m;

        config_m.after_oper(value_m, module_m);
        return *this;
    }

    template <typename T1, typename M1, typename Config1>
    residue& operator*= (const residue<T1, M1, Config1>& x)
    {
        ARAGELI_ASSERT_0
        (
            module_m == x.module_m ||
            is_null(x.module_m) ||
            is_null(module_m)
        );

        config_m.before_oper(value_m, module_m);

        value_m *= x.value_m;
        if(is_null(module_m) && !is_null(x.module_m))
            module_m = x.module_m;

        config_m.after_oper(value_m, module_m);
        return *this;
    }

    template <typename T1, typename M1, typename Config1>
    residue& operator/= (const residue<T1, M1, Config1>& x)
    {
        ARAGELI_ASSERT_0
        (
            module_m == x.module_m ||
            is_null(x.module_m) ||
            is_null(module_m)
        );

        config_m.before_oper(value_m, module_m);

        if(is_null(module_m) && !is_null(x.module_m))
            module_m = x.module_m;

        if(is_null(module_m))
            value_m /= x.value_m;
        else
        {
            residue tmp(x.value_m, module_m);
            value_m *= inverse_mod(tmp.value(), T1(module_m));    // WARNING! A factory needed.
        }

        config_m.after_oper(value_m, module_m);
        return *this;
    }

    template <typename T1, typename M1, typename Config1>
    residue& operator%= (const residue<T1, M1, Config1>& x)
    {
        return residue(null(value_m), module_m);
    }

    bool operator! () const
    {
        return is_null(value_m);
    }

    operator bool () const
    {
        return !!(*this);
    }

    operator T () const
    {
        return value();
    }
};


/// Specialization of a common factory template for residue.
/** All members with additional optional parameter gives
    an object with the same module value as the argument has. */
template <typename T, typename M, typename Config>
struct factory<residue<T, M, Config> >
{
private:
    typedef residue<T, M, Config> TT;
public:

    /// True iff the functions of this class has a meaning.
    static const bool is_specialized = true;

    /// Unit element (1).
    static const TT& unit ()
    {
        static const TT unit_s = TT(Arageli::unit<T>());
        return unit_s;
    }

    /// Unit element (1) by a pattern.
    static TT unit (const TT& x)
    {
        return TT(Arageli::unit(x.value()), x.module());
    }

    /// Minus unit element (-1).
    static const TT& opposite_unit ()
    {
        static const TT opposite_unit_s = TT(Arageli::opposite_unit<T>());
        return opposite_unit_s;
    }

    /// Minus unit element (-1) by a pattern.
    static TT opposite_unit (const TT& x)
    {
        return TT(Arageli::opposite_unit(x.value()), x.module());
    }

    /// Null element (0).
    static const TT& null ()
    {
        static const TT null_s = TT(Arageli::null<T>());
        return null_s;
    }

    /// Null element (0) by a pattern.
    static TT null (const TT& x)
    {
        return TT(Arageli::null(x.value()), x.module());
    }
};


#define ARAGELI_RESIDUE_BINARY_OP(OP, OPASS)    \
    template    \
    <    \
        typename T1, typename M1, typename Config1,    \
        typename T2, typename M2, typename Config2    \
    >    \
    inline residue<T1, M1, Config1> operator OP    \
    (residue<T1, M1, Config1> a, const residue<T2, M2, Config2>& b)    \
    {    \
        return a OPASS b;    \
    }

ARAGELI_RESIDUE_BINARY_OP(+, +=)
ARAGELI_RESIDUE_BINARY_OP(-, -=)
ARAGELI_RESIDUE_BINARY_OP(*, *=)
ARAGELI_RESIDUE_BINARY_OP(/, /=)
ARAGELI_RESIDUE_BINARY_OP(%, %=)


#define ARAGELI_RESIDUE_LOGIC_OP(OP)    \
    template    \
    <    \
        typename T1,    \
        typename M1,    \
        typename Config1,    \
        typename T2,    \
        typename M2,    \
        typename Config2    \
    >    \
    inline bool operator OP    \
    (    \
        const residue<T1, M1, Config1>& a,    \
        const residue<T2, M2, Config2>& b    \
    )    \
    {    \
        return a.value() OP b.value();    \
    }    \
    \
    template    \
    <    \
        typename T1,    \
        typename M1,    \
        typename Config1,    \
        typename T2    \
    >    \
    inline bool operator OP    \
    (const residue<T1, M1, Config1>& a, const T2& b)    \
    {    \
        return a.value() OP b;    \
    }    \
    \
    template    \
    <    \
        typename T1,    \
        typename T2,    \
        typename M2,    \
        typename Config2    \
    >    \
    inline bool operator OP    \
    (const T1& a, const residue<T2, M2, Config2>& b)    \
    {    \
        return a OP b.value();    \
    }



ARAGELI_RESIDUE_LOGIC_OP(==)
ARAGELI_RESIDUE_LOGIC_OP(!=)
ARAGELI_RESIDUE_LOGIC_OP(<)
ARAGELI_RESIDUE_LOGIC_OP(<=)
ARAGELI_RESIDUE_LOGIC_OP(>)
ARAGELI_RESIDUE_LOGIC_OP(>=)


template
<
    typename T1,
    typename M1,
    typename Config1,
    typename T2,
    typename M2,
    typename Config2
>
inline int cmp
(residue<T1, M1, Config1> a, const residue<T2, M2, Config2>& b)
{
    return cmp(a.value(), b.value());
}


template <typename T, typename M, typename Config>
inline std::ostream& operator<< (std::ostream& out, const residue<T, M, Config>& x)
{
    return out << x.value() << "(mod " << x.module() << ")";
}


template <typename T, typename M, typename Config>
std::istream& operator>> (std::istream& in, residue<T, M, Config>& x)
{
    char ch = 0;
    in >> ch;
    bool brackets = false;
    if(ch == '(')brackets = true;
    else in.putback(ch);

    T value;
    in >> value;

    if(!in)
    {
        in.clear(std::ios_base::badbit);
        return in;
    }

    M module;

    if(_Internal::read_literal(in, "("))
    {
        if(!_Internal::read_literal(in, "mod"))
        {
            in.clear(std::ios_base::badbit);
            return in;
        }

        in >> module;

        if(!in)
        {
            in.clear(std::ios_base::badbit);
            return in;
        }

        if(!_Internal::read_literal(in, ")"))
        {
            in.clear(std::ios_base::badbit);
            return in;
        }
    }
    else
        module = null<M>(value);

    if(brackets && !_Internal::read_literal(in, ")"))
    {
        in.clear(std::ios_base::badbit);
        return in;
    }

    x = residue<T, M, Config>(value, module);

    return in;
}


template <typename T, typename M, typename Config>
residue<T, M, Config>::residue (const char* s)
{
    std::istringstream buf(s);
    buf >> *this;
}


template <typename T, typename M, typename Config>
const residue<T, M, Config>& abs (const residue<T, M, Config>& x)
{
    return x;
}



} // namesapce Arageli


#if 0
#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_RESIDUE
    #include "residue.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_RESIDUE
#endif
#endif

#endif
