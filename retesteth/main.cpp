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

    auto argv2 = preprocessOptions(argc, argv);
    initializeOptions(argc, argv2);
    expandUnitTestsArg(argc, argv2);
    makeSingleTestFileSuite(argc, argv2);

    lookForUnregisteredTestFolders();
    if (!checkTestSuiteIsKnown(argc, argv2))
        return -1;

    int result = runTheBoostTests(argc, argv2);
    cleanMemory();
    ExitHandler::doExit();
    return result;
}
