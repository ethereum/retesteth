#define BOOST_TEST_MODULE EthereumTests
#define BOOST_TEST_NO_MAIN
#include <mainHelper.h>
#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <boost/test/included/unit_test.hpp>
#include <thread>

using namespace std;
using namespace boost::unit_test;
using namespace test::main;

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

    initializeOptions(argc, argv);
    expandUnitTestsArg(argc, argv);
    makeSingleTestFileSuite(argc, argv);

    lookForUnregisteredTestFolders();
    if (!checkTestSuiteIsKnown(argc, argv))
        return -1;

    int result = runTheBoostTests(argc, argv);

    ExitHandler::doExit();
    return result;
}


/*std::pair<string, string> getTestSuiteAndTestCase(std::string const& _suite)
{
    std::vector<std::string> testSuitePath = test::explode(_suite, '/');
    string const testCaseName = testSuitePath.at(testSuitePath.size() - 1);
    string testSuiteName = testSuitePath.at(0);
    for (size_t i = 1; i < testSuitePath.size() - 1; i++)
        testSuiteName += "/" + testSuitePath.at(i);
    return std::make_pair(testSuiteName, testCaseName);
}

bool tryDynamicallyAddTestSuite(std::string const& _suite)
{
    // Try dynamically load the suite
    auto suiteCase = getTestSuiteAndTestCase(_suite);
    if (suiteCase.first != suiteCase.second)
    {
        auto suiteid = framework::master_test_suite().get(suiteCase.first);
        if (suiteid != INV_TEST_UNIT_ID)
        {
            auto& suite = framework::get<test_suite>(suiteid);
            auto caseid = suite.get("stDynamicName");
            if (caseid != INV_TEST_UNIT_ID)
            {
                test_case& tcase = framework::get<test_case>(caseid);
                tcase.p_name.value = suiteCase.second;
            }
            return true;
        }
    }
    return false;
}*/
