#include "retesteth/ExitHandler.h"
#include "retesteth/TestOutputHelper.h"
#include "retesteth/testSuites/StateTests.h"
#include "retesteth/testSuites/TestFixtures.h"
#include <retesteth/testSuiteRunner/TestSuite.h>
#include <retesteth/Options.h>
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace dev;
using namespace test;
namespace fs = boost::filesystem;

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

StateTestSuite::StateTestSuite()
{
    // Register subtest as finished test case. because each folder is treated as test case folder
    test::TestOutputHelper::get().markTestFolderAsFinished(getFullPathFiller("VMTests").parent_path(), "VMTests");
}

TestSuite::TestPath StateTestVMSuite::suiteFolder() const
{
    if (Options::get().fillchain)
        return TestSuite::TestPath(fs::path("BlockchainTests") / "GeneralStateTests" / "VMTests");
    return TestSuite::TestPath(fs::path("GeneralStateTests") / "VMTests");
}

TestSuite::FillerPath StateTestVMSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / "GeneralStateTestsFiller" / "VMTests");
}

// Legacy Constantinople
TestSuite::TestPath LegacyConstantinopleStateTestSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("LegacyTests") / "Constantinople" / "GeneralStateTests");
}

TestSuite::FillerPath LegacyConstantinopleStateTestSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(
        fs::path("src") / "LegacyTests" / "Constantinople" / "GeneralStateTestsFiller");
}

// latest version StateTests
using GeneralStateTestsFixture = TestFixture<StateTestSuite, DefaultFlags>;
BOOST_FIXTURE_TEST_SUITE(GeneralStateTests, GeneralStateTestsFixture)

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

// Recent Tests
BOOST_AUTO_TEST_CASE(stArgsZeroOneBalance) {}
BOOST_AUTO_TEST_CASE(stSLoadTest) {}
BOOST_AUTO_TEST_CASE(stChainId) {}
BOOST_AUTO_TEST_CASE(stSelfBalance) {}
BOOST_AUTO_TEST_CASE(stStaticFlagEnabled) {}
BOOST_AUTO_TEST_CASE(stSubroutine) {}
BOOST_AUTO_TEST_CASE(stEIP2537) {}
BOOST_AUTO_TEST_CASE(stEIP2930) {}
BOOST_AUTO_TEST_CASE(stEIP1559) {}
BOOST_AUTO_TEST_CASE(stEIP3607) {}

// Heavy
BOOST_AUTO_TEST_CASE(stTimeConsuming) {}

// Retesteth unit tests
using StateTestsRetestethUnit = TestFixture<StateTestSuite, RequireOptionFill>;
BOOST_FIXTURE_TEST_SUITE(Retesteth, StateTestsRetestethUnit)
BOOST_AUTO_TEST_CASE(stExpectSection) {}
BOOST_AUTO_TEST_SUITE_END()

// Converted VMTests
using GeneralStateTestsVMFixture = TestFixture<StateTestVMSuite, DefaultFlags>;
BOOST_FIXTURE_TEST_SUITE(VMTests, GeneralStateTestsVMFixture)
BOOST_AUTO_TEST_CASE(vmArithmeticTest) {}
BOOST_AUTO_TEST_CASE(vmBitwiseLogicOperation) {}
BOOST_AUTO_TEST_CASE(vmIOandFlowOperations) {}
BOOST_AUTO_TEST_CASE(vmLogTest) {}
BOOST_AUTO_TEST_CASE(vmPerformance) {}
BOOST_AUTO_TEST_CASE(vmTests) {}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
