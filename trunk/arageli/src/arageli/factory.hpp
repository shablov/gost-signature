/*****************************************************************************

    factory.hpp

    This file is a part of the Arageli library.

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

/**
    \file factory.hpp
    \brief Default factory template class.

    Default factory template class and relative functions.
    A specialization of factory for std::complex.
    Factory makes unit, null and opposite unite by default as well as
    by the particular pattern.
*/


#ifndef _ARAGELI_factory_hpp_
#define _ARAGELI_factory_hpp_

#include "config.hpp"

#include <limits>
#include <complex>
#include <vector>

#include "exception.hpp"
#include "basefuncs.hpp"
#include "std_import.hpp"


namespace Arageli
{


/// Common implementation of factory for the specific type.
/** This default implementaion uses literals 1, 0 and -1 to construct
    unit, null and opposite_unit respectively. */
template <typename T>
struct factory
{
    /// True iff the functions of this class has a meaning.
    static const bool is_specialized = true;

    /// Unit element (1).
    static const T& unit ()
    {
        static const T unit_s = T(1);
        return unit_s;
    }

    /// Unit element (1) by pattern.
    static const T& unit (const T& x)
    {
        return unit();
    }

    /// Minus unit element (-1).
    static const T& opposite_unit ()
    {
        static const T opposite_unit_s = T(-1);
        return opposite_unit_s;
    }

    /// Minus unit element (-1) by pattern.
    static const T& opposite_unit (const T& x)
    {
        return opposite_unit();
    }

    /// Null element (0).
    static const T& null ()
    {
        static const T null_s = T(0);
        return null_s;
    }

    /// Null element by pattern (0).
    static const T& null (const T& x)
    {
        return null();
    }

};

/// @name Helper functions for the factory class.

//@{

/// Unit element.
template <typename T>
inline const T& unit ()
{
    return factory<T>::unit();
}

/// Unit element by pattern.
template <typename T>
inline T unit (const T& x)
{
    // WARNING.  Making a copy.
    return factory<T>::unit(x);
}

/// True iff x is one of the unit elements.
template <typename T>
inline bool is_unit (const T& x)
{
    return x == factory<T>::unit(x);
}

/// Minus unit element.
template <typename T>
inline const T& opposite_unit ()
{
    return factory<T>::opposite_unit();
}

/// Minus unit element by pattern.
template <typename T>
inline T opposite_unit (const T&x)
{
    // WARNING.  Making a copy.
    return factory<T>::opposite_unit(x);
}

/// True iff x is one of the minus unit elements.
template <typename T>
inline bool is_opposite_unit (const T& x)
{
    return x == factory<T>::opposite_unit(x);
}

/// Null element.
template <typename T>
inline const T& null ()
{
    return factory<T>::null();
}

/// Null element by pattern.
template <typename T>
inline T null (const T& x)
{
    // WARNING.  Making a copy.
    return factory<T>::null(x);
}

/// True iff x is one of the null elements.
template <typename T>
inline bool is_null (const T& x)
{
    return x == factory<T>::null(x);
}


#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (push)
    #pragma warning (disable : 4146)
#endif

/// Opposite element. Mathematically returns (-x).
template <typename T>
inline T opposite (const T& x)
{
    ARAGELI_ASSERT_0
    (
        !std::numeric_limits<T>::is_specialized ||
        std::numeric_limits<T>::is_signed ||
        is_null(x)
    );

    return -x;
}

/// Opposite element. Mathematically returns (*y = -x).
template <typename T>
inline T& opposite (const T& x, T* y)
{
    ARAGELI_ASSERT_0
    (
        !std::numeric_limits<T>::is_specialized ||
        std::numeric_limits<T>::is_signed ||
        is_null(x)
    );

    return *y = -x;
}

/// Opposite element itself. Mathematically returns (*x) = -(*x).
template <typename T>
inline T& opposite (T* x)
{
    ARAGELI_ASSERT_0
    (
        !std::numeric_limits<T>::is_specialized ||
        std::numeric_limits<T>::is_signed ||
        is_null(x)
    );

    return *x = -*x;
}

#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (pop)
#endif

/// Inverse element. Mathematically returns 1/x.
template <typename T>
inline T inverse (const T& x)
{
    return unit(x) / x;
}

/// Inverse element. Mathematically returns (*y = 1/x).
template <typename T>
inline T& inverse (const T& x, T* y)
{
    return *y = unit(x) / x;
}

/// Inverse element. Mathematically returns (*x) = 1/(*x).
template <typename T>
inline T& inverse (T* x)
{
    return *x = unit(*x) / *x;
}


/// Specialization of inverse for std::vector<bool>::pointer
/** We suppose that std::vector<bool, A>::pointer is the same for any A. */
inline std::vector<bool>::reference inverse (std::vector<bool>::pointer x)
{
    ARAGELI_ASSERT_0(!*x);
    return *x;
}

// Uder tested Lunux platforms we had std::vector<bool, A>::reference*
// is equal to std::vector<bool, A>::pointer, but under Microsoft Visual C++
// they are different types. So, we are switching off the redifinition
// in case of Linux.
#ifndef __linux

/// Specialization of inverse for std::vector<bool>::reference*.
/** We suppose that std::vector<bool, A>::reference are the same for any A. */
inline std::vector<bool>::reference inverse (std::vector<bool>::reference* x)
{
    ARAGELI_ASSERT_0(!*x);
    return *x;
}

#endif

//@}


/// Specialization of common factory template for std::complex.
template <typename T2>
struct factory<std::complex<T2> >
{
private:

    typedef std::complex<T2> T;
    typedef factory<T2> TT2;

public:

    static const bool is_specialized = true;

    static const T& unit ()
    {
        static const T unit_s(TT2::unit());
        return unit_s;
    }

    static T unit (const T& x)
    {
        return T(TT2::unit(x.real()));
    }

    static const T& opposite_unit ()
    {
        static const T opposite_unit_s(TT2::opposite_unit());
        return opposite_unit_s;
    }

    static T opposite_unit (const T& x)
    {
        return T(TT2::opposite_unit(x.real()));
    }

    static const T& null ()
    {
        static const T null_s;
        return null_s;
    }

    static T null (const T& x)
    {
        return T(TT2::null(x.real()));
    }

};

}


//#ifdef ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE
//    #define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_FACTORY
//    #include "factory.cpp"
//    #undef  ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE_FACTORY
//#endif


#endif  //  #ifndef _ARAGELI_factory_hpp_
