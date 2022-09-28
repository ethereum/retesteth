#define BOOST_TEST_MODULE EthereumTests
#define BOOST_TEST_NO_MAIN
#include <AllTestNames.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/testSuites/StateTests.h>
#include <retesteth/testSuites/blockchain/BlockchainTests.h>
#include <retesteth/testSuites/TransactionTest.h>
#include <retesteth/testSuites/DifficultyTest.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <boost/test/included/unit_test.hpp>

using namespace std;
using namespace boost::unit_test;
static std::ostringstream strCout;
std::streambuf* oldCoutStreamBuf;
std::streambuf* oldCerrStreamBuf;
std::string const c_sDynamicTestSuiteName = "customTestSuite";

void customTestSuite();
void travisOut(std::atomic_bool* _stopTravisOut);
void timeoutThread(std::atomic_bool* _stopTimeout);
void printTestSuiteSuggestions(string const& _sMinusTArg);
bool checkTestSuiteIsKnown(int argc, const char* argv[], string sMinusTArg = string());
void disableOutput()
{
    oldCoutStreamBuf = std::cout.rdbuf();
    oldCerrStreamBuf = std::cerr.rdbuf();
    std::cout.rdbuf(strCout.rdbuf());
    std::cerr.rdbuf(strCout.rdbuf());
}
void enableOutput()
{
    std::cout.rdbuf(oldCoutStreamBuf);
    std::cerr.rdbuf(oldCerrStreamBuf);
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
                "trDataCompileSuite,StructTest,MemoryLeak";
            break;
        }
    }

    if (opt.singleTestFile.initialized() || opt.customTestFolder.initialized())
    {
        bool testSuiteFound = false;
        for (int i = 0; i < argc; i++)
        {
            // replace test suite to custom tests
            std::string arg = std::string{argv[i]};
            if (arg == "-t" && i + 1 < argc)
            {
                testSuiteFound = true;
                argv[i + 1] = (char*)c_sDynamicTestSuiteName.c_str();
                break;
            }
        }

        // BOOST ERROR could not be used here because boost main is not initialized
        if (!testSuiteFound)
        {
            if (opt.singleTestFile.initialized())
            {
                std::cerr << "testfile <file>  requires a test suite to be set -t <TestSuite>\n";
                return -1;
            }
            else if (opt.customTestFolder.initialized())
            {
                std::cerr << "testfolder <subfolder>  requires a test suite to be set -t <TestSuite>\n";
                return -1;
            }
        }

        // add custom test suite
        test_suite* ts1 = BOOST_TEST_SUITE(c_sDynamicTestSuiteName);
        ts1->add(BOOST_TEST_CASE(&customTestSuite));
        framework::master_test_suite().add(ts1);
    }

    if (!checkTestSuiteIsKnown(argc, argv))
        return -1;

    int result = 0;
    try
    {
        auto fakeInit = [](int, char* []) -> boost::unit_test::test_suite* { return nullptr; };
        if (opt.vmtrace || !opt.travisOutThread)
        {
            std::atomic_bool stopTimeout{false};
            std::thread timeout(timeoutThread, &stopTimeout);
            // Do not use travis '.' output thread if debug is defined
            result = unit_test_main(fakeInit, argc, const_cast<char**>(argv));
            stopTimeout = true;
            timeout.join();
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

void timeoutThread(std::atomic_bool* _stopTimeout)
{
    uint tickCounter = 0;
    const uint C_MAX_TESTEXEC_TIMEOUT = 36000;
    while (!*_stopTimeout)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ++tickCounter;
        if (tickCounter > C_MAX_TESTEXEC_TIMEOUT)
        {
            test::TestOutputHelper::get().setCurrentTestInfo(test::TestInfo("Timeout"));
            ETH_FAIL_MESSAGE("Test execution timeout reached! " + test::fto_string(C_MAX_TESTEXEC_TIMEOUT) + "sec");
        }
        if (ExitHandler::receivedExitSignal())
            break;
    }
}

void runCustomTestFileOrFolder();
void customTestSuite()
{
    test::Options const& opt = test::Options::get();
    if (opt.singleTestFile.initialized() || opt.customTestFolder.initialized())
        runCustomTestFileOrFolder();
}

void runCustomTestFileOrFolder()
{
    test::Options const& opt = test::Options::get();
    auto runSuite = [&opt](test::TestSuite* _suite){
        if (opt.singleTestFile.initialized())
        {
            boost::filesystem::path file(opt.singleTestFile);
            _suite->runTestWithoutFiller(file);
        }
        else if (opt.customTestFolder.initialized())
        {
            _suite->runAllTestsInFolder(opt.customTestFolder);
        }
    };

    if (opt.rCurrentTestSuite.find("GeneralStateTests") != std::string::npos)
    {
        test::StateTestSuite suite;
        runSuite(&suite);
    }
    else if (opt.rCurrentTestSuite.find("BlockchainTests") != std::string::npos)
    {
        if (opt.rCurrentTestSuite.find("InvalidBlocks") != std::string::npos)
        {
            test::BlockchainTestInvalidSuite suite;
            runSuite(&suite);
        }
        else if (opt.rCurrentTestSuite.find("ValidBlocks") != std::string::npos)
        {
            test::BlockchainTestValidSuite suite;
            runSuite(&suite);
        }
        else if (opt.rCurrentTestSuite.find("TransitionTests") != std::string::npos)
        {
            test::BlockchainTestTransitionSuite suite;
            runSuite(&suite);
        }
        else
        {
            test::BlockchainTestInvalidSuite suite;
            runSuite(&suite);
        }
    }
    else if (opt.rCurrentTestSuite.find("DifficultyTests") != std::string::npos)
    {
        test::DifficultyTestSuite suite;
        runSuite(&suite);
    }
    else if (opt.rCurrentTestSuite.find("TransactionTests") != std::string::npos)
    {
        test::TransactionTestSuite suite;
        runSuite(&suite);
    }
    else
    {
        string sMinusTArg = opt.rCurrentTestSuite;
        std::cerr << "Error: '" + sMinusTArg + "' suite not found! \n";
        printTestSuiteSuggestions(sMinusTArg);
    }
}

bool checkTestSuiteIsKnown(int argc, const char* argv[], string sMinusTArg)
{
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
        if (requestSuiteNotFound && sMinusTArg != c_sDynamicTestSuiteName)
        {
            std::cerr << "Error: '" + sMinusTArg + "' suite not found! \n";
            printTestSuiteSuggestions(sMinusTArg);
            return false;
        }
    }
    return true;
}
