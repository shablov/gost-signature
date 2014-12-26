/* /////////////////////////////////////////////////////////////////////////////
//
//  File:       tstcontainer.hpp
//  Created:    2005/7/3    9:57
//
//  Author: Andrey Somsikov
*/

#ifndef __tstcontainer_hpp__
#define __tstcontainer_hpp__

#include "config.hpp"
#include "tsowndefs.hpp"

#include <list>
#include <string>

typedef enum TestResult
{
    resOK,
    resFAIL,
    resEXCEPT,
    resHANG
};

typedef struct SRunStatistic
{
    size_t totalCount;
    size_t passCount;
    size_t failCount;
    size_t exceptCount;
    size_t hangCount;

    double all_duration;

    std::list<std::string> not_passed;

    SRunStatistic()
    {
        totalCount = 0;
        passCount = 0;
        failCount = 0;
        exceptCount = 0;
        hangCount = 0;
    }

} SRunStatistic;

//!  A test class.
/*!
A more elaborate class description.
*/
class TS_DECL CTestCollection
{
public:
    typedef TestResult (*FTestFunction)(void);
    typedef struct STest
    {
        FTestFunction testFunction;
        std::string className;
        std::string functionName;
        std::string description;
        STest():
        testFunction(NULL)
        { }
        STest(FTestFunction _testFunction,
            const std::string& _className,
            const std::string& _functionName,
            const std::string& _description):
        testFunction(_testFunction),
            className(_className),
            functionName(_functionName),
            description(_description)
        { }

    } STest;
    typedef std::list<STest> CTestFunctions;
public:
    CTestCollection() {};
    ~CTestCollection() {};
    static CTestFunctions& getTests();
    /*!
    This function is used for test registartion. Use
    TEST(className, functionName, description) macro instead
    */
    static bool registerTest(FTestFunction testFunction,
        const std::string& className,
        const std::string& functionName,
        const std::string& description);
    static SRunStatistic runAll(std::ostream& os);

    #ifndef TS_WITHOUT_BOOST
        static SRunStatistic run(std::ostream& os,
            const std::string& className,
            const std::string& functionName);
    #endif

    static int seed;
protected:
    static void runTest(std::ostream& os, const STest& test, SRunStatistic& rstat);

protected:
    static CTestFunctions *testsPtr;
};

#endif /*__tstcontainer_h__*/
/* End of file tstcontainer.hpp */
