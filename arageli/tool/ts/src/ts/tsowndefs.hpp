/* /////////////////////////////////////////////////////////////////////////////
//
//  File:       tsowndefs.hpp
//  Created:    2005/7/3    10:17
//
//  Author: Andrey Somsikov
*/

#ifndef __tsowndefs_hpp__
#define __tsowndefs_hpp__

#define COUNT_OF(a) (sizeof(a)/sizeof(a[0]))

#ifndef TS_ASSERT
#ifndef _DEBUG
#define TS_ASSERT(expr) ((void)0)
#else
#include <cassert>
//#define TS_ASSERT(exp) (void)( (exp) || (_assert(#exp, __FILE__, __LINE__), 0) ) // It's not portable.
#define TS_ASSERT(exp) assert(exp)
#endif
#endif

#endif /*__tsowndefs_hpp__*/
/* End of file tsowndefs.hpp */
