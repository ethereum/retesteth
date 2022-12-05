#include "retesteth/TestHelper.h"
#include "retesteth/TestOutputHelper.h"
#include "retesteth/testSuites/TestFixtures.h"
#include "retesteth/testSuites/statetests/StateTests.h"
#include "libdataobj/DataObject.h"
#include <retesteth/Options.h>
#include <functional>
#include <iostream>

using namespace std;
using namespace dev;
using namespace test;
using namespace dataobject;
using namespace boost::unit_test;
namespace fs = boost::filesystem;

#define STATESUITE_FOLDER_OVERRIDE(SUITE, FOLDER)   \
    TestSuite::TestPath SUITE::suiteFolder() const  \
    {                                               \
        if (Options::get().fillchain)               \
            return TestSuite::TestPath(fs::path(string("BlockchainTests/GeneralStateTests") + string(FOLDER + m_fillerPathAdd))); \
        return TestSuite::TestPath(fs::path(string("GeneralStateTests") + string(FOLDER + m_fillerPathAdd))); \
    }                                               \
                                                    \
    TestSuite::FillerPath SUITE::suiteFillerFolder() const \
    {                                               \
        return TestSuite::FillerPath(fs::path(string("src/GeneralStateTestsFiller") + string(FOLDER + m_fillerPathAdd))); \
    }

#define EIPSTATESUITE_FOLDER_OVERRIDE(SUITE, FOLDER)   \
    TestSuite::TestPath SUITE::suiteFolder() const  \
    {                                               \
        if (Options::get().fillchain)               \
            return TestSuite::TestPath(fs::path(string("BlockchainTests/EIPStateTests") + string(FOLDER + m_fillerPathAdd))); \
        return TestSuite::TestPath(fs::path(string("EIPStateTests") + string(FOLDER + m_fillerPathAdd))); \
    }                                               \
                                                    \
    TestSuite::FillerPath SUITE::suiteFillerFolder() const \
    {                                               \
        return TestSuite::FillerPath(fs::path(string("src/EIPStateTestsFiller") + string(FOLDER + m_fillerPathAdd))); \
    }

#define STATESUITE_DO_OVERRIDE(SUITE, CODE) \
spDataObject SUITE::doTests(spDataObject& _input, TestSuite::TestSuiteOptions& _opt) const \
{                                           \
    CODE                                    \
    return DoTests(_input, _opt);           \
}

#define MARKFINISHED(CASE) \
    test::TestOutputHelper::get().markTestFolderAsFinished(getFullPathFiller(CASE).parent_path(), CASE);

STATESUITE_FOLDER_OVERRIDE(StateTestSuite, "")
STATESUITE_DO_OVERRIDE(StateTestSuite, MARKFINISHED("VMTests"))
STATESUITE_FOLDER_OVERRIDE(StateTestVMSuite, "/VMTests")
STATESUITE_DO_OVERRIDE(StateTestVMSuite,)
STATESUITE_FOLDER_OVERRIDE(StateTestShanghaiSuite, "/Shanghai")
STATESUITE_DO_OVERRIDE(StateTestShanghaiSuite,)


// Legacy Constantinople
TestSuite::TestPath LegacyConstantinopleStateTestSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("LegacyTests") / "Constantinople" / "GeneralStateTests");
}

TestSuite::FillerPath LegacyConstantinopleStateTestSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / "LegacyTests" / "Constantinople" / "GeneralStateTestsFiller");
}


EIPSTATESUITE_FOLDER_OVERRIDE(EIPStateTestSuite, "")
STATESUITE_DO_OVERRIDE(EIPStateTestSuite, MARKFINISHED("stEOF"))
EIPSTATESUITE_FOLDER_OVERRIDE(EIPStateTestEOFSuite, "/stEOF")
STATESUITE_DO_OVERRIDE(EIPStateTestEOFSuite,)


using EIPStateTestsFixture = TestFixture<EIPStateTestSuite, DefaultFlags>;
ETH_REGISTER_DYNAMIC_TEST_SEARCH(EIPStateTestsFixture, "EIPStateTests")
BOOST_FIXTURE_TEST_SUITE(EIPStateTests, EIPStateTestsFixture)

using EIPStateTestsEOFFixture = TestFixture<EIPStateTestEOFSuite, DefaultFlags>;
ETH_REGISTER_DYNAMIC_TEST_SEARCH(EIPStateTestsEOFFixture, "EIPStateTests/stEOF")
BOOST_FIXTURE_TEST_SUITE(stEOF, EIPStateTestsEOFFixture)

BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()


// latest version StateTests
using GeneralStateTestsFixture = TestFixture<StateTestSuite, DefaultFlags>;
ETH_REGISTER_DYNAMIC_TEST_SEARCH(GeneralStateTestsFixture, "GeneralStateTests")
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
BOOST_AUTO_TEST_CASE(stEIP3670) {}
BOOST_AUTO_TEST_CASE(stEIP3860) {}

// Heavy
BOOST_AUTO_TEST_CASE(stTimeConsuming) {}

// Retesteth unit tests
using StateTestsRetestethUnit = TestFixture<StateTestSuite, RequireOptionFill>;
BOOST_FIXTURE_TEST_SUITE(Retesteth, StateTestsRetestethUnit)
BOOST_AUTO_TEST_CASE(stExpectSection) {}
BOOST_AUTO_TEST_SUITE_END()

// Shanghai tests suite
using GeneralStateTestsShanghaiFixture = TestFixture<StateTestShanghaiSuite, DefaultFlags>;
ETH_REGISTER_DYNAMIC_TEST_SEARCH(GeneralStateTestsShanghaiFixture, "GeneralStateTests/Shanghai")
BOOST_FIXTURE_TEST_SUITE(Shanghai, GeneralStateTestsShanghaiFixture)
BOOST_AUTO_TEST_CASE(stEIP3540) {}
BOOST_AUTO_TEST_SUITE_END()

// Converted VMTests
using GeneralStateTestsVMFixture = TestFixture<StateTestVMSuite, DefaultFlags>;
ETH_REGISTER_DYNAMIC_TEST_SEARCH(GeneralStateTestsVMFixture, "GeneralStateTests/VMTests")
BOOST_FIXTURE_TEST_SUITE(VMTests, GeneralStateTestsVMFixture)
BOOST_AUTO_TEST_CASE(vmArithmeticTest) {}
BOOST_AUTO_TEST_CASE(vmBitwiseLogicOperation) {}
BOOST_AUTO_TEST_CASE(vmIOandFlowOperations) {}
BOOST_AUTO_TEST_CASE(vmLogTest) {}
BOOST_AUTO_TEST_CASE(vmPerformance) {}
BOOST_AUTO_TEST_CASE(vmTests) {}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
