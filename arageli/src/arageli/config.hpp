/*****************************************************************************

    config.hpp

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


#ifndef _ARAGELI_config_hpp_
#define _ARAGELI_config_hpp_

/**
    \file config.hpp
    \brief Several configuration switches.

    Some macros from this file can be defined through the compiler command
    line. If so, the values for such macros from this file don't have
    an effect.
*/

/// Version of the library.
#define ARAGELI_VERSION "2.2.9"


/// Controls whether cpp-files would be included into corresponding hpp-files.
/** If this macro is defined, cpp-files with template implementations will be
    included into corresponding hpp-files to be visible to the compiler when
    the user's code is being compiled. */
#define ARAGELI_INCLUDE_CPP_WITH_EXPORT_TEMPLATE


// #define ARAGELI_PLATFORM_OPTIMIZATION_1

// #define ARAGELI_PLATFORM_OPTIMIZATION_2



#ifndef ARAGELI_DEBUG_LEVEL
    /// The debug level.
    /** Activates assert macros with level-number less then this value.
        So, zero deactivates all assert macros and value 4 activates
        all asserts. */
    #ifdef NDEBUG
        #define ARAGELI_DEBUG_LEVEL        1
    #else
        #define ARAGELI_DEBUG_LEVEL        3
    #endif
#endif


#define ARAGELI_PLATFORM_UNKNOWN 0
#define ARAGELI_PLATFORM_WINDOWS 1


#ifndef ARAGELI_PLATFORM
    #if defined(_MSC_VER) || defined(__CYGWIN32__) || defined(__MINGW32__)
        #define ARAGELI_PLATFORM  ARAGELI_PLATFORM_WINDOWS
    #else
        #define ARAGELI_PLATFORM  ARAGELI_PLATFORM_UNKNOWN
    #endif
#endif


/// Turn on some debug output command to std::cout inlined to internal code.
//#define ARAGELI_INLINE_DEBUG_OUTPUT

// The following two defines turn on advanced multiplication methods:

/// Turns on multiplication method based on NTT.
/** This method is experimental and doesn't work properly on all the numbers.
    Do not turn it on!
    The complexity of the method is O(n*log(n)), where n is the length
    of input numbers. */
//#define ARAGELI_ENABLE_POLLARD_MULT

/// Turns on Karatsuba multiplication method.
/** This method has complexity O(n^log2(3)), where n is the length
    of input numbers. */
#define ARAGELI_ENABLE_KARATSUBA_MULT

/// Internal threshold for Karatsuba multiplication algorithm
#if defined(ARAGELI_ENABLE_KARATSUBA_MULT)
    #define ARAGELI_KARATSUBA_THRESHOLD 50
#endif

/// Internal threshold for Pollard multiplication algorithm
#define ARAGELI_POLLARD_MAX_POWER 21
#define ARAGELI_POLLARD_UPPER_BOUND (1<<ARAGELI_POLLARD_MAX_POWER)
#if defined(ARAGELI_ENABLE_POLLARD_MULT)
    #define ARAGELI_POLLARD_THRESHOLD 30000
#endif

/// If this macro is defined then assertions throw exception assert_failed.
//#define ARAGELI_ASSERT_THROW_EXCEPTION


/// Turn on tracing subsystem of the library.
/**  See exception.hpp for more details. */
//#define ARAGELI_TRACING


/// Turn on the use of __assume intrinsic instead empty ASSERT substitution.
/** Macros from the ARAGELI_ASSERT family is expanded to nothing if the debug
    level is low enough. Defining this macro we allow using __assume intrinsic
    instead an empty expantion in those cases.
    WARNING! Temporary this feature can't be turned on because of
    not compatibility with ARAGELI_DEBUG_EXEC macros family. */
//#define ARAGELI_ASSERT_ASSUME


#define ARAGELI_PERFORMANCE_TIMER


/// The number of iterations in is_prime_small_primes_division function.
#define ARAGELI_IS_PRIME_SMALL_PRIMES_DIVISION_NUMBER 20


#if defined(_MSC_VER) && defined(_WIN32)
    #define ARAGELI_INT64_SUPPORT
#endif


// Please check whether it is correct or not. And only after that uncomment.
#ifdef __linux
    #define ARAGELI_LONG_LONG_SUPPORT
#endif


/// Enables support of signed __int64 and unsigned __int64 types.
//#define ARAGELI_INT64_SUPPORT_ENABLE

/// Enables support of unsigned signed long long and unsigned long long.
//#define ARAGELI_LONG_LONG_SUPPORT_ENABLE

/// Disables support of signed __int64 and unsigned __int64 types.
//#define ARAGELI_INT64_SUPPORT_DISABLE

/// Disables support of unsigned signed long long and unsigned long long.
//#define ARAGELI_LONG_LONG_SUPPORT_DISABLE

#if defined(ARAGELI_INT64_SUPPORT_ENABLE) && defined(ARAGELI_INT64_SUPPORT_DISABLE)
    #error Both macros ARAGELI_INT64_SUPPORT_DISABLE and    \
        ARAGELI_INT64_SUPPORT_ENABLE are defined.
#endif


#if defined(ARAGELI_LONG_LONG_SUPPORT_ENABLE) && defined(ARAGELI_LONG_LONG_SUPPORT_DISABLE)
    #error Both macros ARAGELI_LONG_LONG_SUPPORT_DISABLE and    \
        ARAGELI_LONG_LONG_SUPPORT_ENABLE are defined.
#endif


#ifdef ARAGELI_INT64_SUPPORT_ENABLE
    #define ARAGELI_INT64_SUPPORT
#elif defined(ARAGELI_INT64_SUPPORT_DISABLE) && defined(ARAGELI_INT64_SUPPORT)
    #undef ARAGELI_INT64_SUPPORT
#endif


#ifdef ARAGELI_LONG_LONG_SUPPORT_ENABLE
    #define ARAGELI_LONG_LONG_SUPPORT
#elif defined(ARAGELI_LONG_LONG_SUPPORT_DISABLE) && defined(ARAGELI_LONG_LONG_SUPPORT)
    #undef ARAGELI_LONG_LONG_SUPPORT
#endif


/// Disables some negligible warnings under some systems.
/** If you modified the library, you likely want to not define
    this macro to see all warnings occur for some new feature. */
#define ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS


/// Allows the compiler automatically link the Arageli library.
/** It's useful when the user's code is being compiled.
    WARNING! This option temporary isn't allowed. */
//#define ARAGELI_USE_AUTOLINK


#ifdef ARAGELI_USE_AUTOLINK_ENABLE
    #define ARAGELI_USE_AUTOLINK
#elif defined(ARAGELI_USE_AUTOLINK_DISABLE) && defined(ARAGELI_USE_AUTOLINK)
    #undef ARAGELI_USE_AUTOLINK
#endif


#ifdef ARAGELI_USE_AUTOLINK
    #define ARAGELI_LIB_NAME arageli
    #include "auto_link.hpp"
#endif


// Be careful! The next pragmas disables some warnings for the whole library
// and for all code that uses the library.
#ifdef ARAGELI_DISABLE_PARTICULAR_COMPILER_WARNINGS
    #pragma warning (disable : 4996)
    #pragma warning (disable : 4800)
    #pragma warning (disable : 4244)
#endif


#endif  //  #ifndef _ARAGELI_config_hpp_
