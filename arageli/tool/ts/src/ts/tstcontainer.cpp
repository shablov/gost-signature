/* /////////////////////////////////////////////////////////////////////////////
//
//  File:       tstcontainer.cpp
//  Created:    2005/7/3    10:03
//
//  Author: Andrey Somsikov
//  Modification: Sergey S. Lyalin
*/

#include <ctime>
#include <iostream>

#include "tstcontainer.hpp"
#include "tsstream.hpp"

#ifndef TS_WITHOUT_BOOST
    #include <boost/regex.hpp>
#endif

#include <cstdlib>

//using namespace std;
#ifndef TS_WITHOUT_BOOST
    using namespace boost;
#endif

int CTestCollection::seed(0);
CTestCollection::CTestFunctions *CTestCollection::testsPtr(NULL);

CTestCollection::CTestFunctions& CTestCollection::getTests()
{
    if (NULL == testsPtr)
        testsPtr = new CTestFunctions();
    return *testsPtr;
}

bool CTestCollection::registerTest
(
    FTestFunction testFunction,
    const std::string& className,
    const std::string& functionName,
    const std::string& description
)
{
    CTestFunctions& tests = getTests();
    tests.push_back(STest(testFunction, className, functionName, description));
    return true;
}

void CTestCollection::runTest(std::ostream& os, const STest& test, SRunStatistic& rstat)
{
    TestResult result;
    double duration = 0;    // test function work duration, sec

    srand(CTestCollection::seed);
    os << "TEST";
    os << "\n  class = \"" << test.className << "\"";
    os << "\n  function = \"" << test.functionName << "\"";
    os << "\n  description = \"" << test.description << "\"";
    os << std::endl;

#ifndef TS_DONT_CATCH_EXCEPTIONS_FROM_TEST
    try
    {
#endif //TS_DONT_CATCH_EXCEPTIONS_FROM_TEST
        rstat.totalCount++;
        std::clock_t start_time = std::clock();
        result = test.testFunction();
        std::clock_t end_time = std::clock();
        duration = double(end_time - start_time)/CLOCKS_PER_SEC;
#ifndef TS_DONT_CATCH_EXCEPTIONS_FROM_TEST
    }
    catch(...)
    {
        result = resEXCEPT;
    }
#endif //TS_DONT_CATCH_EXCEPTIONS_FROM_TEST
    os << "  duration = " << duration << " sec";
    os << "\n  result = ";
    std::string status;
    switch(result)
    {
    case resOK:
        status = "PASSED";
        rstat.passCount++;
        break;
    case resFAIL:
        status = "FAILED";
        rstat.failCount++;
        break;
    case resHANG:
        status = "HANGED";
        rstat.hangCount++;
        break;
    case resEXCEPT:
        status = "EXCEPT";
        rstat.exceptCount++;
        break;
    default:
        std::cerr << "\nUnknown type of the result. Aborting.\n";
        std::abort();
    }

    os << status;

    if(result != resOK)
        rstat.not_passed.push_back(status + "  " + test.className + "::" + test.functionName);

    if(!tout.str().empty())
    {
        os << "\n  output =\n\"";
        os << tout.str();
        if(tout.str()[tout.str().length() - 1] != '\n')
            os << '\n';
        os << "\"";
    }
    os << std::endl;

    tout.str(""); // reset tout
}

#ifndef TS_WITHOUT_BOOST

SRunStatistic CTestCollection::run
(
    std::ostream& os,
    const std::string& className,
    const std::string& functionName
)
{
    SRunStatistic rstat;

    CTestFunctions& tests = getTests();

    cmatch what;
    regex classNameRegex(className);
    regex functionNameRegex(functionName);

    for (CTestFunctions::iterator it = tests.begin();
        it != tests.end();
        it++)
    {
        if (regex_match(it->className.c_str(), what, classNameRegex) &&
            regex_match(it->functionName.c_str(), what, functionNameRegex))
            runTest(os, *it, rstat);
    }

    TS_ASSERT(rstat.passCount+rstat.failCount+rstat.exceptCount+rstat.hangCount == rstat.totalCount);

    return rstat;
}

#endif

SRunStatistic CTestCollection::runAll(std::ostream& os)
{
    SRunStatistic rstat;

    CTestFunctions& tests = getTests();

    std::clock_t start_time = std::clock();

    for (CTestFunctions::iterator it = tests.begin();
        it != tests.end();
        it++)
        runTest(os, *it, rstat);

    std::clock_t end_time = std::clock();
    rstat.all_duration = double(end_time - start_time)/CLOCKS_PER_SEC;

    TS_ASSERT(rstat.passCount+rstat.failCount+rstat.exceptCount+rstat.hangCount == rstat.totalCount);

    return rstat;
}

/* End of file tstcontainer.cpp */
