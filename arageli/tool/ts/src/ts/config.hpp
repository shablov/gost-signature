/* /////////////////////////////////////////////////////////////////////////////
//
//
//  File:       config.hpp
//  Created:    2005/9/30    20:48
//
*/

#ifndef __config_hpp__
#define __config_hpp__

// if the macro is defined, The Boost Library isn't used by TS.
#define TS_WITHOUT_BOOST

// if the macro is defined, the program is configured for lib-target,
// otherwise --- for dll-tagret.
// WARNING! I cannot build such dll-target that would be successfuly builded with
// arageli/tests together. So, now target is lib.
#define TS_LIB_TARGET

#ifndef TS_LIB_TARGET
    // export if this is our own source, otherwise import:
    #ifdef TS_SOURCE
        #if defined(TS_WINDOWS) && (defined(_MSC_VER) || defined(__ICL))
            #define TS_DECL __declspec(dllexport)
        #else
            #define TS_DECL
        #endif
    #else
        #if defined(TS_WINDOWS) && (defined(_MSC_VER) || defined(__ICL))
            #define TS_DECL __declspec(dllimport)
        #else
            #define TS_DECL
        #endif
    #endif                // #ifdef TS_SOURCE
#else                    // #ifndef TS_LIB_TARGET
        #define TS_DECL
#endif                    // #ifndef TS_LIB_TARGET

#ifndef TS_WITHOUT_BOOST
#ifndef BOOST_PROGRAM_OPTIONS_DECL
#define BOOST_PROGRAM_OPTIONS_DECL
#endif
#endif // #ifndef TS_WITHOUT_BOOST


#endif /*__config_hpp__*/
/* End of file config.hpp */
