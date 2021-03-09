#define BOOST_TEST_MODULE EthereumTests
#define BOOST_TEST_NO_MAIN
#include <AllTestNames.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/testSuites/StateTests.h>
#include <retesteth/testSuites/blockchain/BlockchainTests.h>
#include <boost/test/included/unit_test.hpp>
#include <thread>

using namespace boost::unit_test;
static std::ostringstream strCout;
std::streambuf* oldCoutStreamBuf;
std::streambuf* oldCerrStreamBuf;
void printTestSuiteSuggestions(string const& _sMinusTArg);

void customTestSuite()
{
    // restore output for creating test
    std::cout.rdbuf(oldCoutStreamBuf);
    std::cerr.rdbuf(oldCerrStreamBuf);
    test::Options const& opt = test::Options::get();

    // if generating a random test
    /*if (opt.createRandomTest)
    {
        if (!dev::test::createRandomTest())
            throw framework::internal_error(
                "Create random test error! See std::error for more details.");
    }*/

    // if running a testfile
    if (opt.singleTestFile.is_initialized())
    {
        boost::filesystem::path file(opt.singleTestFile.get());
        if (opt.rCurrentTestSuite.find_first_of("GeneralStateTests") != std::string::npos)
        {
            test::StateTestSuite suite;
            suite.runTestWithoutFiller(file);
        }
        else if (opt.rCurrentTestSuite.find_first_of("BlockchainTests") != std::string::npos)
        {
            test::BlockchainTestValidSuite suite;
            suite.runTestWithoutFiller(file);
        }
        else if (opt.rCurrentTestSuite.find_first_of("TransitionTests") != std::string::npos)
        {
            // dev::test::TransitionTestsSuite suite;
            // suite.runTestWithoutFiller(file);
        }
        else if (opt.rCurrentTestSuite.find_first_of("VMtests") != std::string::npos)
        {
            // dev::test::VmTestSuite suite;
            // suite.runTestWithoutFiller(file);
        }
        else if (opt.rCurrentTestSuite.find_first_of("TransactionTests") != std::string::npos)
        {
            // dev::test::TransactionTestSuite suite;
            // suite.runTestWithoutFiller(file);
        }
    }
}

void travisOut(std::atomic_bool* _stopTravisOut)
{
    int tickCounter = 0;
    while (!*_stopTravisOut)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++tickCounter;
        if (tickCounter % 10 == 0)
            std::cout << ".\n" << std::flush;  // Output dot every 10s.
        if (ExitHandler::receivedExitSignal())
            break;
    }
}

/*
The equivalent of setlocale(LC_ALL, “C”) is called before any user code is run.
If the user has an invalid environment setting then it is possible for the call
to set locale to fail, so there are only two possible actions, the first is to
throw a runtime exception and cause the program to quit (default behaviour),
or the second is to modify the environment to something sensible (least
surprising behaviour).

The follow code produces the least surprising behaviour. It will use the user
specified default locale if it is valid, and if not then it will modify the
environment the process is running in to use a sensible default. This also means
that users do not need to install language packs for their OS.
*/
void setDefaultOrCLocale()
{
#if __unix__
    if (!std::setlocale(LC_ALL, ""))
    {
        setenv("LC_ALL", "C", 1);
    }
#endif
}

// Custom Boost Unit Test Main
int main(int argc, const char* argv[])
{
    std::string const dynamicTestSuiteName = "customTestSuite";
    setDefaultOrCLocale();
    signal(SIGABRT, &ExitHandler::exitHandler);
    signal(SIGTERM, &ExitHandler::exitHandler);
    signal(SIGINT, &ExitHandler::exitHandler);

    // Initialize options
    try
    {
        test::Options::get(argc, argv);
    }
    catch (test::Options::InvalidOption const& e)
    {
        std::cerr << e.what() << "\n";
        exit(1);
    }

    test::Options const& opt = test::Options::get();

    // Special UnitTest
    for (int i = 0; i < argc; i++)
    {
        // replace test suite to custom tests
        std::string arg = std::string{argv[i]};
        if (arg == "-t" && i + 1 < argc && string(argv[i + 1]) == "UnitTests")
        {
            argv[i + 1] =
                "LLLCSuite,SOLCSuite,DataObjectTestSuite,EthObjectsSuite,OptionsSuite,TestHelperSuite,ExpectSectionSuite,"
                "trDataCompileSuite";
            break;
        }
    }

    if (opt.createRandomTest || opt.singleTestFile.is_initialized())
    {
        bool testSuiteFound = false;
        for (int i = 0; i < argc; i++)
        {
            // replace test suite to custom tests
            std::string arg = std::string{argv[i]};
            if (arg == "-t" && i + 1 < argc)
            {
                testSuiteFound = true;
                argv[i + 1] = (char*)dynamicTestSuiteName.c_str();
                break;
            }
        }

        // BOOST ERROR could not be used here because boost main is not initialized
        if (!testSuiteFound && opt.createRandomTest)
        {
            std::cerr << "createRandomTest requires a test suite to be set -t <TestSuite>\n";
            return -1;
        }
        if (!testSuiteFound && opt.singleTestFile.is_initialized())
        {
            std::cerr << "testfile <file>  requires a test suite to be set -t <TestSuite>\n";
            return -1;
        }

        // Disable initial output as the random test will output valid json to std
        oldCoutStreamBuf = std::cout.rdbuf();
        oldCerrStreamBuf = std::cerr.rdbuf();
        std::cout.rdbuf(strCout.rdbuf());
        std::cerr.rdbuf(strCout.rdbuf());

        // add custom test suite
        test_suite* ts1 = BOOST_TEST_SUITE("customTestSuite");
        ts1->add(BOOST_TEST_CASE(&customTestSuite));
        framework::master_test_suite().add(ts1);
    }

    string sMinusTArg;
    // unit_test_main delete this option from _argv
    for (int i = 0; i < argc; i++)  // find -t boost arg
    {
        std::string const arg = std::string{argv[i]};
        if (arg == "-t" && i + 1 < argc)
        {
            sMinusTArg = std::string{argv[i + 1]};
            break;
    }
    }

    if (!sMinusTArg.empty())
    {
        bool requestSuiteNotFound = false;
        std::vector<std::string> requestedSuites = test::explode(sMinusTArg, ',');
        for (auto const& suite : requestedSuites)
        {
            if (!test::inArray(c_allTestNames, suite))
            {
                requestSuiteNotFound = true;
                sMinusTArg = suite;
                break;
            }
        }

        // Print suggestions of a test case if test suite not found
        if (requestSuiteNotFound && sMinusTArg != "customTestSuite")
        {
            std::cerr << "Error: '" + sMinusTArg + "' suite not found! \n";
            printTestSuiteSuggestions(sMinusTArg);
            return -1;
        }
    }

    int result = 0;
    try
    {
        auto fakeInit = [](int, char* []) -> boost::unit_test::test_suite* { return nullptr; };
        if (opt.jsontrace || opt.vmtrace || opt.statediff || opt.createRandomTest || !opt.travisOutThread)
        {
            // Do not use travis '.' output thread if debug is defined
            result = unit_test_main(fakeInit, argc, const_cast<char**>(argv));
        }
        else
        {
            // Initialize travis '.' output thread for log activity
            std::atomic_bool stopTravisOut{false};
            std::thread outputThread(travisOut, &stopTravisOut);
            result = unit_test_main(fakeInit, argc, const_cast<char**>(argv));
            stopTravisOut = true;
            outputThread.join();
        }
    }
    catch (test::UpwardsException const& _ex)
    {
        ETH_STDERROR_MESSAGE(string("Error: ") + _ex.what());
    }

    ExitHandler::doExit();
    return result;
}

void printTestSuiteSuggestions(string const& _sMinusTArg)
{
    auto const testList = test::levenshteinDistance(_sMinusTArg, c_allTestNames);
    std::cerr << "Did you mean: \n";
    for (auto const& element : testList)
        std::cerr << "-t " << element << "\n";
}
