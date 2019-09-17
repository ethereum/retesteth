#include "retesteth/ExitHandler.h"
#include "retesteth/TestOutputHelper.h"
#include "retesteth/testSuites/StateTests.h"
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace dev;
namespace fs = boost::filesystem;

// Run Tests using test suite and StateTest execution logic from StateTests.cpp
void runStateTests(test::TestSuite const& _suite)
{
    string casename = boost::unit_test::framework::current_test_case().p_name;
    boost::filesystem::path suiteFillerPath = _suite.getFullPathFiller(casename).parent_path();

    static vector<string> const timeConsumingTestSuites{
        string{"stTimeConsuming"}, string{"stQuadraticComplexityTest"}};
    if (test::inArray(timeConsumingTestSuites, casename) && !test::Options::get().all)
    {
        if (!ExitHandler::receivedExitSignal())
            std::cout << "Skipping " << casename << " because --all option is not specified.\n";
        test::TestOutputHelper::get().markTestFolderAsFinished(suiteFillerPath, casename);
        return;
    }
    _suite.runAllTestsInFolder(casename);
    test::TestOutputHelper::get().markTestFolderAsFinished(suiteFillerPath, casename);
}

// Most Recent StateTestSuite
TestSuite::TestPath StateTestSuite::suiteFolder() const
{
    if (Options::get().fillchain)
        return TestSuite::TestPath(fs::path("BlockchainTests") / "GeneralStateTests");
    return TestSuite::TestPath(fs::path("GeneralStateTests"));
}

TestSuite::FillerPath StateTestSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / "GeneralStateTestsFiller");
}

test::GeneralStateTestFixture::GeneralStateTestFixture()
{
    test::StateTestSuite suite;
    runStateTests(suite);
}


// Legacy Constantinople
TestSuite::TestPath LegacyConstantinopleStateTestSuite::suiteFolder() const
{
    if (Options::get().fillchain || Options::get().filltests)
        ETH_ERROR_MESSAGE("Legacy tests are sealed and not refillable!");
    return TestSuite::TestPath(fs::path("LegacyTests") / "Constantinople" / "GeneralStateTests");
}

TestSuite::FillerPath LegacyConstantinopleStateTestSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(
        fs::path("src") / "LegacyTests" / "Constantinople" / "GeneralStateTestsFiller");
}

DataObject LegacyConstantinopleStateTestSuite::doTests(
    DataObject const& _input, TestSuiteOptions& _opt) const
{
    test::StateTestSuite suite;
    return suite.doTests(_input, _opt);
}

test::LegacyConstantinopleGeneralStateTestFixture::LegacyConstantinopleGeneralStateTestFixture()
{
    test::LegacyConstantinopleStateTestSuite suite;
    runStateTests(suite);
}


// latest version StateTests
BOOST_FIXTURE_TEST_SUITE(GeneralStateTests, GeneralStateTestFixture)

// Frontier Tests
BOOST_AUTO_TEST_CASE(stCallCodes) {}
BOOST_AUTO_TEST_CASE(stCallCreateCallCodeTest) {}
BOOST_AUTO_TEST_CASE(stExample) {}
BOOST_AUTO_TEST_CASE(stInitCodeTest) {}
BOOST_AUTO_TEST_CASE(stLogTests) {}
BOOST_AUTO_TEST_CASE(stMemoryTest) {}
BOOST_AUTO_TEST_CASE(stPreCompiledContracts) {}
BOOST_AUTO_TEST_CASE(stPreCompiledContracts2) {}
BOOST_AUTO_TEST_CASE(stRandom) {}
BOOST_AUTO_TEST_CASE(stRandom2) {}
BOOST_AUTO_TEST_CASE(stRecursiveCreate) {}
BOOST_AUTO_TEST_CASE(stRefundTest) {}
BOOST_AUTO_TEST_CASE(stSolidityTest) {}
BOOST_AUTO_TEST_CASE(stSpecialTest) {}
BOOST_AUTO_TEST_CASE(stSystemOperationsTest) {}
BOOST_AUTO_TEST_CASE(stTransactionTest) {}
BOOST_AUTO_TEST_CASE(stTransitionTest) {}
BOOST_AUTO_TEST_CASE(stWalletTest) {}

// Homestead Tests
BOOST_AUTO_TEST_CASE(stCallDelegateCodesCallCodeHomestead) {}
BOOST_AUTO_TEST_CASE(stCallDelegateCodesHomestead) {}
BOOST_AUTO_TEST_CASE(stHomesteadSpecific) {}
BOOST_AUTO_TEST_CASE(stDelegatecallTestHomestead) {}

// EIP150 Tests
BOOST_AUTO_TEST_CASE(stChangedEIP150) {}
BOOST_AUTO_TEST_CASE(stEIP150singleCodeGasPrices) {}
BOOST_AUTO_TEST_CASE(stMemExpandingEIP150Calls) {}
BOOST_AUTO_TEST_CASE(stEIP150Specific) {}

// EIP158 Tests
BOOST_AUTO_TEST_CASE(stEIP158Specific) {}
BOOST_AUTO_TEST_CASE(stNonZeroCallsTest) {}
BOOST_AUTO_TEST_CASE(stZeroCallsTest) {}
BOOST_AUTO_TEST_CASE(stZeroCallsRevert) {}
BOOST_AUTO_TEST_CASE(stCodeSizeLimit) {}
BOOST_AUTO_TEST_CASE(stCreateTest) {}
BOOST_AUTO_TEST_CASE(stRevertTest) {}

// Metropolis Tests
BOOST_AUTO_TEST_CASE(stStackTests) {}
BOOST_AUTO_TEST_CASE(stStaticCall) {}
BOOST_AUTO_TEST_CASE(stReturnDataTest) {}
BOOST_AUTO_TEST_CASE(stZeroKnowledge) {}
BOOST_AUTO_TEST_CASE(stZeroKnowledge2) {}
BOOST_AUTO_TEST_CASE(stCodeCopyTest) {}
BOOST_AUTO_TEST_CASE(stBugs) {}

// Constantinople Tests
BOOST_AUTO_TEST_CASE(stShift) {}
BOOST_AUTO_TEST_CASE(stCreate2) {}
BOOST_AUTO_TEST_CASE(stExtCodeHash) {}
BOOST_AUTO_TEST_CASE(stSStoreTest) {}

// Stress Tests
BOOST_AUTO_TEST_CASE(stAttackTest) {}
BOOST_AUTO_TEST_CASE(stMemoryStressTest) {}
BOOST_AUTO_TEST_CASE(stQuadraticComplexityTest) {}

// Invalid Opcode Tests
BOOST_AUTO_TEST_CASE(stBadOpcode) {}

// New Tests
BOOST_AUTO_TEST_CASE(stArgsZeroOneBalance) {}
BOOST_AUTO_TEST_CASE(stEWASMTests) {}
BOOST_AUTO_TEST_CASE(stTimeConsuming) {}
BOOST_AUTO_TEST_CASE(stSLoadTest) {}
BOOST_AUTO_TEST_CASE(stChainId) {}
BOOST_AUTO_TEST_CASE(stSelfBalance) {}
BOOST_AUTO_TEST_SUITE_END()
