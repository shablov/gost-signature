/*****************************************************************************

    _utility.hpp

    This file is a part of the Arageli library.

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

/**
    \file _utility.hpp
    \brief Several internal utilites (an internal file of the library).

    Do not include this file directrly if you are external user of the library.
*/

#ifndef _ARAGELI__utility_hpp_
#define _ARAGELI__utility_hpp_

#include "config.hpp"

#include <limits.h> //GCC 4.3 fix
#include <cstring> //GCC 4.3 fix

#include <iostream>
#include <algorithm>
#include <utility>
#include <string>

#include "type_opers.hpp"
#include "factory.hpp"
#include "functional.hpp"
#include "std_import.hpp"
#include "interval.hpp"
#include "powerest.hpp"


namespace Arageli
{

namespace _Internal
{

/// Set std::ios_base stream state and automaticaly changes it back when destroys.
/** When an object of this class is being constructing, it give a reference to
    a stream and flags value, it's applaying those flags on that stream.
    The old flags is keept inside the object. When the object is destroing,
    it set the old flags back.

    Usefull in streamed IO function development. */
class auto_stream_state
{
    std::ios_base& stream;
    std::ios_base::fmtflags oldflags;

public:

    auto_stream_state
    (
        std::ios_base& stream_a,
        std::ios_base::fmtflags newflags
    ) :
        stream(stream_a)
    {
        oldflags = stream.flags(newflags);
    }

    ~auto_stream_state ()
    {
        stream.flags(oldflags);
    }
};


// Ищет в s символы разделители, если они есть возвращает false, иначе -- true.
bool is_not_contains_spaces (const char* s);


// Пытается прочесть из in строку s. Если строка прочитана успешно, то
// возвращается true, иначе -- false. Ошибочно считанные символы запихиваются
// обратно в поток. Возможно выйдет исключение (зависит от потока).
// Первые символы разделители пропускаются или не пропускаются (зависит от
// потока), внутренние символы разделители не пропускаются.
bool read_literal (std::istream& in, const char* s);


template <typename In, typename Str>
inline bool is_bad_read_literal (In& in, const Str& s)
{
    if(!read_literal(in, s))
    {
        in.clear(std::ios_base::badbit);
        return true;
    }

    return false;
}


// Checks stream's state and if it's not good, sets badbit as a state and returns true,
// otherwise returns false.
template <typename In>
inline bool is_bad_input (In& in)
{
    if(!in)
    {
        in.clear(std::ios_base::badbit);
        return true;
    }

    return false;
}


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4800)
#endif


/// Implements functionality of std::auto_ptr for arrays.
template <typename T> class auto_array
{
public:

    typedef T element_type;

    explicit auto_array (T* p = 0) throw() :
        owner(p),
        pointer(p)
    {}

    auto_array (const auto_array<T>& q) throw() :
        owner(q.owner),
        pointer(q.release())
    {}

    auto_array<T>& operator= (const auto_array<T>& q) throw()
    {
        if(this != &q)
        {
            if(pointer != q.get())
            {
                if (owner)
                delete [] pointer;
                owner = q.owner;
            }
            else if(q.owner)
                owner = true;

            pointer = q.release();
        }
        return *this;
    }

    ~auto_array ()
    {
        if(owner)
            delete [] pointer;
    }

    T& operator* () const throw()
    {
        return *get();
    }

    T* operator-> () const throw()
    {
        return get();
    }

    T* get () const throw()
    {
        return pointer;
    }

    T* release () const throw()
    {
        owner = false;
        return pointer;
    }

private:

    mutable bool owner;

    T* pointer;
};


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif


template <typename A, typename B, typename Store_A, typename Store_B>
inline void swap_help_1 (A& a, B& b, Store_A& sa, Store_B& sb, true_type)
{
    sa.swap(sb);
}

template <typename A, typename B, typename Store_A, typename Store_B>
void swap_help_1 (A& a, B& b, Store_A& sa, Store_B& sb, false_type);

template <typename In_a, typename In_b, typename Cmp>
int aggregate_cmp (In_a ai, In_a aend, In_b bi, In_b bend, Cmp cmp);

template <typename In_a, typename In_b>
inline int aggregate_cmp (In_a ai, In_a aend, In_b bi, In_b bend)
{
    // WARNING! gfunc::cmp
    return aggregate_cmp(ai, aend, bi, bend, gfunc::cmp());
}

template <typename T>
std::reverse_iterator<T> make_reverse_iterator (const T& x)
{
    return std::reverse_iterator<T>(x);
}


/// 2^p - 1 module. WARNING! TEMPORARY IMPLEMENTATION.
template <typename D = unsigned int, typename T = big_int>
class module_2pm1
{
public:    // WARNING!

    D degree_a;

public:

    module_2pm1 () :
        degree_a(0)
    {}

    module_2pm1 (const D& x) :
        degree_a(x)
    {}

    operator T () const
    {
        return (unit<T>() << degree_a) - unit<T>();
    }
};


template <typename D1, typename T1, typename D2, typename T2>
inline bool operator== (const module_2pm1<D1, T1>& a, const module_2pm1<D2, T2>& b)
{
    return a.degree_a == b.degree_a;
}

template <typename D1, typename T1, typename D2, typename T2>
inline bool operator!= (const module_2pm1<D1, T1>& a, const module_2pm1<D2, T2>& b)
{
    return !(a == b);
}


template <typename In, typename Val>
Val content (In begin, In end, const Val*)
{
    typedef typename std::iterator_traits<In>::value_type T;
    T res = null<T>();
    for(; begin != end; ++begin)
        res = gcd(res, *begin);
    return res;
}

template <typename In>
typename std::iterator_traits<In>::value_type content_float (In begin, In end)
{
    typedef typename std::iterator_traits<In>::value_type Val;
    return
        begin == end ?
        null<Val>() :
        unit<Val>();
}

template <typename In>
float content (In begin, In end, const float*)
{
    return content_float(begin, end);
}

template <typename In>
double content (In begin, In end, const double*)
{
    return content_float(begin, end);
}

template <typename In>
long double content (In begin, In end, const long double*)
{
    return content_float(begin, end);
}

/// Returns content of a number sequence.
template <typename In>
typename std::iterator_traits<In>::value_type content (In begin, In end)
{
    return content(begin, end, begin.operator->());  // WARNING! operator-> isn't always return necessary type
}


/// Imbue new locale object for an input stream to avoid problem with comma.
/** The problem has been firstly discovered in MSVC++ 2005.
    Discussion on the topic is availible at
    http://groups.google.com/group/arageli/browse_thread/thread/51bfdf6b5ea57604
    (in Russian). */
template <typename Stream>
class turn_off_comma_as_separator
{
    // WARNING! Temporary 'char'.
    struct nocommapunct : public std::numpunct<char>
    {
        protected:

            // WARNING! Temporary 'char'.
            char do_thousands_sep() const
            {
                return 0;
            }
    };

    Stream& in;
    bool activated;
    std::locale oldloc;
    nocommapunct* ncp;

public:

    turn_off_comma_as_separator (Stream& ina) :
        in(ina),
        activated(false)
    {}

    void activate ()
    {
        oldloc = in.getloc();
        in.imbue(std::locale(oldloc, ncp = new nocommapunct()));
        activated = true;
    }

    ~turn_off_comma_as_separator ()
    {
        if(activated)
            in.imbue(oldloc);
    }
};


/// Width in symbols of the texts s with line breaks.
/** It's correct if only s doesn't contain any special symbols exept '\n'
    that takes not 1 printed symbol. */
template <typename S>
typename S::size_type text_width (const S& s)
{
    typedef typename S::size_type size_type;
    size_type w = 0, res = 0;
    for(size_type i = 0; i < s.length(); ++i)
        if(s[i] == '\n')
        {
            res = std::max(res, w);
            w = 0;
        }
        else ++w;

    return std::max(res, w);
}


/// Hight in lines of the texts s with line breaks.
template <typename S>
typename S::size_type text_hight (const S& s)
{
    return std::count(s.begin(), s.end(), '\n') + 1;
}

///// Hight in lines of the texts s with line breaks without one optional terminator '\n'.
//template <typename S>
//typename S::size_type text_hight_wo1eol (const S& s)
//{
//    return text_hight(s) -
//        (!s.empty() && s[s.length() - 1] == '\n');
//}

} // namespace _Internal

template <typename D1, typename T1>
inline bool is_null (const _Internal::module_2pm1<D1, T1>& a)
{
    return is_null(a.degree_a);
}

template <typename D1, typename T1>
inline bool is_unit (const _Internal::module_2pm1<D1, T1>& a)
{
    return is_unit(a.degree_a);
}


template <typename T1, typename D2, typename T2>
inline T1 prrem (const T1& aa, const _Internal::module_2pm1<D2, T2>& b)
{
    T1 a = aa;

    ARAGELI_DEBUG_EXEC_1(T1 _debug_result = prrem(a, T1(b)));

    T2 m2p = unit<T2>() << b.degree_a;
    T2 m2pm1 = m2p - unit<T2>();
    if(is_negative(a))a += m2pm1;

    //std::cout << "\nm2p = " << m2p;
    //std::cout << "\nm2pm1 = " << m2pm1;
    //std::cout << "\na = " << a;

    ARAGELI_ASSERT_0(!is_negative(a));

    if(a < m2pm1)return a;

    a -= m2pm1;

    if(a < m2pm1)return a;

    //std::cout << "\na = " << a;

    T1 q = a >> b.degree_a;
    ++q;

    //std::cout << "\nq = " << q;

    ARAGELI_ASSERT_1(q < m2pm1);

    //std::cout << "\n(a & m2pm1) = " << (a & m2pm1);

    T1 r = (a & m2pm1) + q;

    //std::cout << "\nr = " << r;

    if(r < m2p)--r;
    else r -= m2p;

    ARAGELI_ASSERT_1(!is_negative(r));
    ARAGELI_ASSERT_1(r < m2pm1);
    //std::cout << "\n_debug_result = " << _debug_result;
    //std::cout << "\nr = " << r;
    ARAGELI_ASSERT_1(_debug_result == r);

    return r;
}


template <typename Out, typename D, typename T>
inline Out& operator<< (Out& out, const _Internal::module_2pm1<D, T>& x)
{
    out << "2^" << x.degree_a << "-1";
    return out;
}


template <typename In, typename D, typename T>
inline In& operator>> (In& in, _Internal::module_2pm1<D, T>& x)
{
    // WARNING!
    ARAGELI_ASSERT_ALWAYS(!"Ha-ha-ha:)!");
    return in;
}


// WARNING! MOVE THE FOLLOWING DEFINITION TO SEPARATE FILE
template <typename Stream, typename T>
inline void output_aligned (Stream& s, const T& x)
{
    s << x;
}


/// Case-insensitive string comparison.
/** Originally at http://lafstern.org/matt/col2_new.pdf, p. 6.
    See also in Russian: Скотт Мейерс. Эффективное использование STL, с. 213. */
struct lt_str_1 :
    public std::binary_function<std::string, std::string, bool>
{
    struct lt_char
    {
        const std::ctype<char>& ct;

        lt_char(const std::ctype<char>& c) :
            ct(c)
        {}

        bool operator()(char x, char y) const
        {
            return ct.toupper(x) < ct.toupper(y);
        }
    };

    std::locale loc;
    const std::ctype<char>& ct;

    lt_str_1(const std::locale& L = std::locale::classic()) :
        loc(L),
        ct(std::use_facet<std::ctype<char> >(loc))
    {}

    bool operator()(const std::string& x, const std::string& y) const
    {
        return
            std::lexicographical_compare
            (
                x.begin(), x.end(),
                y.begin(), y.end(),
                lt_char(ct)
            );
    }
};


/// Case-insensitive string comparison.
/** Originally at http://lafstern.org/matt/col2_new.pdf, p. 6--7.
    See also in Russian: Скотт Мейерс. Эффективное использование STL, с. 214. */
struct lt_str_2 :
    public std::binary_function<std::string, std::string, bool>
{
    struct lt_char
    {
        const char* tab;

        lt_char(const char* t) :
            tab(t)
        {}

        bool operator()(char x, char y) const
        {
            return tab[x - CHAR_MIN] < tab[y - CHAR_MIN];
        }
    };

    char tab[CHAR_MAX - CHAR_MIN + 1];

    lt_str_2 (const std::locale& L = std::locale::classic())
    {
        const std::ctype<char>& ct =
            std::use_facet<std::ctype<char> >(L);
        for(int i = CHAR_MIN; i <= CHAR_MAX; ++i)
            tab[i - CHAR_MIN] = (char)i;
        ct.toupper(tab, tab + (CHAR_MAX - CHAR_MIN + 1));
    }

    bool operator()(const std::string& x, const std::string& y) const
    {
        return
            std::lexicographical_compare
            (
                x.begin(), x.end(),
                y.begin(), y.end(),
                lt_char(tab)
            );
    }
};


/// Case-insensitive C-style comparision for std::string.
/** It's based on lt_str_2.
    WARNING! It can be 2x faster. It isn't dealing with different cultures. */
inline int stricmp (const std::string& x, const std::string& y)
{
    static lt_str_2 sc;    // default locale
    ARAGELI_ASSERT_1(!(sc(y, x) && sc(x, y)));  // both cannot be true
    return sc(y, x)-sc(x, y);   // magic? ;) no! it's simple!
}


template <typename T1, typename T2>
inline bool is_converted_to (const T2& x)
{
    // WARNING! IT IS SOMETIMES INCORRECT: CONSIDER TWO 32-BIT INTEGERS, T1 IS SIGNED, T2 IS UNSIGNED,
    // BOTH ARE THE SAME SIZE, AND x = MAX(T2)
    return T2(T1(x)) == x;
}


} // namespace Arageli


#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE__UTILITY
    #include "_utility.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE__UTILITY
#endif


#endif  //  #ifndef _ARAGELI__utility_hpp_
