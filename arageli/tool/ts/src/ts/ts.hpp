/* /////////////////////////////////////////////////////////////////////////////
//
//  File:       ts.hpp
//  Created:    2005/3/22    22:25
//
//  Author: Andrey Somsikov
*/

#ifndef __ts_hpp__
#define __ts_hpp__

#include "config.hpp"
#include "tsstream.hpp"
#include "tsowndefs.hpp"
#include "tstcontainer.hpp"

#if !defined(TS_SOURCE)
//#pragma comment(lib, "ts.lib")    // WARNING! TEMPORARY DISABLED
#endif

/*! \file ts.h
\brief A Documented file.

Details.
*/

/*!
This macro is used for test registration
*/
#define TEST(className, functionName, description)    \
    TestResult t_##className##_##functionName();    \
    static bool t_##className##_##functionName##_value = CTestCollection::registerTest(t_##className##_##functionName, #className, #functionName, description);    \
    TestResult t_##className##_##functionName()

#define TEST_CLASS(className, description)    \
    TestResult t_##className();    \
    static bool t_##className##_value = CTestCollection::registerTest(t_##className, #className, ".*", description);    \
    TestResult t_##className()

#define TEST_FUNCTION(functionName, description)    \
    TestResult t_func_##functionName();    \
    static bool t_func_##functionName##_value = CTestCollection::registerTest(t_func_##functionName, "", #functionName, description);    \
    TestResult t_func_##functionName()

const int ARAGELI_TESTSYS_COVERAGE_DEFAULT = 1000;

#endif /*__ts_hpp__*/
/* End of file ts.hpp */
