/*****************************************************************************

    misc.hpp -- miscellaneous features

    This file is part of the Arageli library.

    Copyright (C) 1999--2006 Nikolai Yu. Zolotykh
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

/** \file

    Miscellaneous features, routines, classes etc.
*/


#ifndef _ARAGELI_misc_hpp_
#define _ARAGELI_misc_hpp_

#include "config.hpp"


namespace Arageli
{


/// Makes and returns a temporary copy of the argument.
/** This function is useful to dealing with our container-liked
    structures when an element of structure is used in operation
    with whole its owner.  For example, such operations are
    the dividing a row of a matrix by the element in that row,
    the dividing a polynomial by the leading coefficient and so on. */
template <typename T>
T safe_reference (const T& x)
{
    return x;
}


template <typename T1, typename T2, typename Outiter>
void generate_range_helper (T1& t1, const T2& t2, Outiter outiter)
{
    //std::cerr << "\nt1 = " << t1 << ", t2 = " << t2 << ":\n\t";

    if(t1 < t2)
        for(; t1 != t2; ++t1/*, std::cerr << t1 << "  "*/)
            *outiter++ = t1;
    else
        for(; t1 != t2; --t1/*, std::cerr << t1 << "  "*/)
            *outiter++ = t1;

    *outiter = t1;
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4804)
#endif

template <typename T1, typename T2, typename T3, typename Outiter>
void generate_range_helper (T1& t1, const T2& t2, const T3& t3, Outiter outiter)
{
    for(; t1 != t3; t1 += t2, ++outiter)
        *outiter = t1;

    *outiter = t1;
}

template <typename T1, typename T2, typename Outiter>
void generate_range_helper_wo_inc (T1& t1, const T2& t2, Outiter outiter)
{
    //std::cerr << "\nt1 = " << t1 << ", t2 = " << t2 << ":\n\t";

    if(t1 < t2)
        for(; t1 != t2; t1 += unit(t1)/*, std::cerr << t1 << "  "*/)
            *outiter++ = t1;
    else
        for(; t1 != t2; t1 -= unit(t1)/*, std::cerr << t1 << "  "*/)
            *outiter++ = t1;

    *outiter = t1;
}


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

template <typename Outiter>
inline void generate_range_helper (bool& t1, bool t2, Outiter outiter)
{
    generate_range_helper_wo_inc(t1, t2, outiter);
}

template <typename Outiter>
inline void generate_range_helper (float& t1, float t2, Outiter outiter)
{
    generate_range_helper_wo_inc(t1, t2, outiter);
}

template <typename Outiter>
inline void generate_range_helper (double& t1, double t2, Outiter outiter)
{
    generate_range_helper_wo_inc(t1, t2, outiter);
}

template <typename Outiter>
inline void generate_range_helper (long double& t1, long double t2, Outiter outiter)
{
    generate_range_helper_wo_inc(t1, t2, outiter);
}


template <typename T> struct cnc_value_type
{
    typedef typename T::value_type type;
};

template <typename T> struct cnc_value_type<const T>
{
    typedef typename T::value_type const type;
};


template <typename T> struct cnc_reference
{
    typedef typename T::reference type;
};

template <typename T> struct cnc_reference<const T>
{
    typedef typename T::const_reference type;
};


template <typename T> struct cnc_pointer
{
    typedef typename T::pointer type;
};

template <typename T> struct cnc_pointer<const T>
{
    typedef typename T::const_pointer type;
};


template <typename T> struct cnc_iterator
{
    typedef typename T::iterator type;
};

template <typename T> struct cnc_iterator<const T>
{
    typedef typename T::const_iterator type;
};


template <typename R, typename T>
inline R noncopy_cast (const T& x)
{
    return R(x);
}

template <typename R>
inline const R& noncopy_cast (const R& x)
{
    return x;
}

template <typename R>
inline R& noncopy_cast (R& x)
{
    return x;
}

template <typename R, typename T>
inline R copy_cast (const T& x)
{
    return noncopy_cast<R>(x);
}


} // namespace Arageli


#if 0
#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_MISC
    #include "misc.cpp"
    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_MISC
#endif
#endif

#endif  //  #ifndef _ARAGELI_misc_hpp_
