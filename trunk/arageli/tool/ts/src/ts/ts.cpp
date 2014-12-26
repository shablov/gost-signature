/* /////////////////////////////////////////////////////////////////////////////
//
//  File:       ts.cpp
//  Created:    2005/3/22    22:24
//
//  Author: Andrey Somsikov
*/
#include "ts.hpp"

#ifndef TS_WITHOUT_BOOST
    #include <boost/program_options.hpp>
#endif

#include "config.hpp"
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <iterator>

using namespace std;

ostringstream tout;

#ifndef TS_WITHOUT_BOOST
    namespace po = boost::program_options;
#endif

TS_DECL int main(int argc, char* argv[]);
int main(int argc, char* argv[])
{

    SRunStatistic rstat;

    try
    {

    #ifndef TS_WITHOUT_BOOST

        string cName;
        string fName;
        bool hasRegexp = false;
        bool hasOutStream = false;
        ofstream outStream;
        int seed;

        // define options
        po::options_description generic("Generic options");
        generic.add_options()
            ("help,h", "Output this help message")
            ("ini,i", po::value<string>(), "Configuration file")
            ;
        po::options_description config("Config options");
        config.add_options()
            ("class,c", po::value<string>(), "Define regular expression used"
                " to choose tests with appropriate class name")
            ("func,f", po::value<string>(), "Define regular expression used"
                " to choose tests with appropriate function name")
            ("out,o", po::value<string>(), "Output file (standard output"
                " used if options not specified)")
            ("seed", po::value<int>(), "Seed value used to initialize random"
                " number generator for each test (0 used if not specified)")
            ;

        // get option values
        po::options_description cmdline_options;
        cmdline_options.add(generic).add(config);

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
        po::notify(vm);

        if (vm.count("ini"))
        {
            string iniFile = vm["ini"].as<string>();
            ifstream ifs(iniFile.c_str());
            store(parse_config_file(ifs, config), vm);
            notify(vm);
        }

        if (vm.count("help")) {
            cout << cmdline_options << "\n";
            return 1;
        }

        if (vm.count("class"))
        {
            cName = vm["class"].as<string>();
            hasRegexp = true;
        }
        else
            cName = ".*";
        if (vm.count("function"))
        {
            fName = vm["function"].as<string>();
            hasRegexp = true;
        }
        else
            fName = ".*";

        if (vm.count("seed"))
            seed = vm["seed"].as<int>();
        else
            seed = 0;

        if (vm.count("out"))
        {
            string outFileName = vm["out"].as<string>();
            outStream.open(outFileName.c_str());
            if (!outStream.is_open())
                cerr << "Can't open " << outFileName << "\n";
            hasOutStream = true;
        }

        // setup & run tests
        CTestCollection::seed = seed;
        if (hasRegexp)
        {
            if(hasOutStream)
                rstat = CTestCollection::run(outStream, cName, fName);
            else
                rstat = CTestCollection::run(cout, cName, fName);
        }
        else
        {
            if(hasOutStream)
                rstat = CTestCollection::runAll(outStream);
            else
                rstat = CTestCollection::runAll(cout);
        }

    #else

        CTestCollection::seed = 0;
        rstat = CTestCollection::runAll(cout);

    #endif // #ifndef TS_WITHOUT_BOOST

    }
    catch(exception& e)
    {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        cerr << "Exception of unknown type!\n";
    }

    cout << "\n-------------\nTest finished\n";
    cout << "total duration = " << rstat.all_duration << " sec\n";
    cout << "Results:\n";
    cout << "TOTAL  = " << rstat.totalCount << "\n";
    cout << "PASSED = " << rstat.passCount << "   (" << 100.0*rstat.passCount/rstat.totalCount << "%)\n";
    cout << "FAILED = " << rstat.failCount << "   (" << 100.0*rstat.failCount/rstat.totalCount << "%)\n";
    cout << "EXCEPT = " << rstat.exceptCount << "   (" << 100.0*rstat.exceptCount/rstat.totalCount << "%)\n";
    cout << "HANGED = " << rstat.hangCount << "   (" << 100.0*rstat.hangCount/rstat.totalCount << "%)\n";

    if(!rstat.not_passed.empty())
    {
        cout << "-----------------\nNot passed tests:\n";
        copy
        (
            rstat.not_passed.begin(),
            rstat.not_passed.end(),
            ostream_iterator<string>(cout, "\n")
        );
    }

    if (rstat.totalCount != rstat.passCount)
        return 1;

    return 0;
}

/* End of file ts.cpp */
