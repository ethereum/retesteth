#include "retesteth/helpers/TestHelper.h"
#include "retesteth/helpers/TestOutputHelper.h"
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

#define EIPSUITE_FOLDER_OVERRIDE(SUITE, FOLDER)   \
    TestSuite::TestPath SUITE::suiteFolder() const  \
    {                                               \
        if (Options::get().fillchain)               \
            return TestSuite::TestPath(fs::path(string("EIPTests/BlockchainTests") + string(FOLDER + m_fillerPathAdd))); \
        return TestSuite::TestPath(fs::path(string("EIPTests") + string(FOLDER + m_fillerPathAdd))); \
    }                                               \
                                                    \
    TestSuite::FillerPath SUITE::suiteFillerFolder() const \
    {                                               \
        return TestSuite::FillerPath(fs::path(string("src/EIPTestsFiller") + string(FOLDER + m_fillerPathAdd))); \
    }

STATESUITE_FOLDER_OVERRIDE(StateTestSuite, "")
STATESUITE_FOLDER_OVERRIDE(StateTestVMSuite, "/VMTests")
STATESUITE_FOLDER_OVERRIDE(StateTestShanghaiSuite, "/Shanghai")
EIPSUITE_FOLDER_OVERRIDE(EIPStateTestSuite, "/StateTests")
EIPSUITE_FOLDER_OVERRIDE(EIPStateTestEOFSuite, "/StateTests/stEOF")


// Legacy Constantinople
TestSuite::TestPath LegacyConstantinopleStateTestSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("LegacyTests") / "Constantinople" / "GeneralStateTests");
}

TestSuite::FillerPath LegacyConstantinopleStateTestSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / "LegacyTests" / "Constantinople" / "GeneralStateTestsFiller");
}


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

// Shanghai tests suite
using GeneralStateTestsShanghaiFixture = TestFixture<StateTestShanghaiSuite, DefaultFlags>;
ETH_REGISTER_DYNAMIC_TEST_SEARCH(GeneralStateTestsShanghaiFixture, "GeneralStateTests/Shanghai")
BOOST_FIXTURE_TEST_SUITE(Shanghai, GeneralStateTestsShanghaiFixture)
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


#include <retesteth/testSuites/blockchain/BlockchainTests.h>
#define BCEIPSUITE_FOLDER_OVERRIDE(SUITE, FOLDER, FILLER)   \
    TestSuite::TestPath SUITE::suiteFolder() const       \
    {                                                    \
        return TestSuite::TestPath(fs::path(string("EIPTests" + string(FOLDER) + m_fillerPathAdd))); \
    }                                                    \
                                                         \
    TestSuite::FillerPath SUITE::suiteFillerFolder() const   \
    {                                                    \
        return TestSuite::FillerPath(fs::path(string("src/EIPTestsFiller" + string(FILLER) + m_fillerPathAdd)));  \
    }

BCEIPSUITE_FOLDER_OVERRIDE(BCEIPStateTestsSuite, "/BlockchainTests/StateTests", "/StateTests")
BCEIPSUITE_FOLDER_OVERRIDE(BCEIPStateTestsEOFSuite, "/BlockchainTests/StateTests/stEOF", "/StateTests/stEOF")
BCEIPSUITE_FOLDER_OVERRIDE(BlockchainTestEIPSuite, "/BlockchainTests", "/BlockchainTests")

BCEIPSUITE_FOLDER_OVERRIDE(BlockchainTestEIPPyspecSuite, "/Pyspecs", "/Pyspecs")
    BCEIPSUITE_FOLDER_OVERRIDE(BlockchainTestEIPPyspecSuite_frontier, "/Pyspecs/frontier", "/Pyspecs/frontier")
    BCEIPSUITE_FOLDER_OVERRIDE(BlockchainTestEIPPyspecSuite_homestead, "/Pyspecs/homestead", "/Pyspecs/homestead")
    BCEIPSUITE_FOLDER_OVERRIDE(BlockchainTestEIPPyspecSuite_istanbul, "/Pyspecs/istanbul", "/Pyspecs/istanbul")
    BCEIPSUITE_FOLDER_OVERRIDE(BlockchainTestEIPPyspecSuite_berlin, "/Pyspecs/berlin", "/Pyspecs/berlin")
    BCEIPSUITE_FOLDER_OVERRIDE(BlockchainTestEIPPyspecSuite_merge, "/Pyspecs/merge", "/Pyspecs/merge")
    BCEIPSUITE_FOLDER_OVERRIDE(BlockchainTestEIPPyspecSuite_shanghai, "/Pyspecs/shanghai", "/Pyspecs/shanghai")
    BCEIPSUITE_FOLDER_OVERRIDE(BlockchainTestEIPPyspecSuite_cancun, "/Pyspecs/cancun", "/Pyspecs/cancun")

//EIP TESTS
BOOST_AUTO_TEST_SUITE(EIPTests)

using EIPPyspecFixture = TestFixture<BlockchainTestEIPPyspecSuite, DefaultFlags>;
//ETH_REGISTER_DYNAMIC_TEST_SEARCH(EIPPyspecFixture, "EIPTests/Pyspecs")
BOOST_FIXTURE_TEST_SUITE(Pyspecs, EIPPyspecFixture)
    using EIPPyspecSuiteFixture_frontier = TestFixture<BlockchainTestEIPPyspecSuite_frontier, DefaultFlags>;
    ETH_REGISTER_DYNAMIC_TEST_SEARCH(EIPPyspecSuiteFixture_frontier, "EIPTests/Pyspecs/frontier")
    BOOST_FIXTURE_TEST_SUITE(frontier, EIPPyspecSuiteFixture_frontier)
    BOOST_AUTO_TEST_SUITE_END()
    using EIPPyspecSuiteFixture_homestead = TestFixture<BlockchainTestEIPPyspecSuite_homestead, DefaultFlags>;
    ETH_REGISTER_DYNAMIC_TEST_SEARCH(EIPPyspecSuiteFixture_homestead, "EIPTests/Pyspecs/homestead")
    BOOST_FIXTURE_TEST_SUITE(homestead, EIPPyspecSuiteFixture_homestead)
    BOOST_AUTO_TEST_SUITE_END()
    using EIPPyspecSuiteFixture_istanbul = TestFixture<BlockchainTestEIPPyspecSuite_istanbul, DefaultFlags>;
    ETH_REGISTER_DYNAMIC_TEST_SEARCH(EIPPyspecSuiteFixture_istanbul, "EIPTests/Pyspecs/istanbul")
    BOOST_FIXTURE_TEST_SUITE(istanbul, EIPPyspecSuiteFixture_istanbul)
    BOOST_AUTO_TEST_SUITE_END()
    using EIPPyspecSuiteFixture_berlin = TestFixture<BlockchainTestEIPPyspecSuite_berlin, DefaultFlags>;
    ETH_REGISTER_DYNAMIC_TEST_SEARCH(EIPPyspecSuiteFixture_berlin, "EIPTests/Pyspecs/berlin")
    BOOST_FIXTURE_TEST_SUITE(berlin, EIPPyspecSuiteFixture_berlin)
    BOOST_AUTO_TEST_SUITE_END()
    using EIPPyspecSuiteFixture_merge = TestFixture<BlockchainTestEIPPyspecSuite_merge, DefaultFlags>;
    ETH_REGISTER_DYNAMIC_TEST_SEARCH(EIPPyspecSuiteFixture_merge, "EIPTests/Pyspecs/merge")
    BOOST_FIXTURE_TEST_SUITE(merge, EIPPyspecSuiteFixture_merge)
    BOOST_AUTO_TEST_SUITE_END()
    using EIPPyspecSuiteFixture_shanghai = TestFixture<BlockchainTestEIPPyspecSuite_shanghai, DefaultFlags>;
    ETH_REGISTER_DYNAMIC_TEST_SEARCH(EIPPyspecSuiteFixture_shanghai, "EIPTests/Pyspecs/shanghai")
    BOOST_FIXTURE_TEST_SUITE(shanghai, EIPPyspecSuiteFixture_shanghai)
    BOOST_AUTO_TEST_SUITE_END()
    using EIPPyspecSuiteFixture_cancun = TestFixture<BlockchainTestEIPPyspecSuite_cancun, DefaultFlags>;
    ETH_REGISTER_DYNAMIC_TEST_SEARCH(EIPPyspecSuiteFixture_cancun, "EIPTests/Pyspecs/cancun")
    BOOST_FIXTURE_TEST_SUITE(cancun, EIPPyspecSuiteFixture_cancun)
    BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()


using EIPTestsFixture = TestFixture<EIPStateTestSuite, DefaultFlags>;
ETH_REGISTER_DYNAMIC_TEST_SEARCH(EIPTestsFixture, "EIPTests/StateTests")
BOOST_FIXTURE_TEST_SUITE(StateTests, EIPTestsFixture)
    using EIPStateTestsEOFFixture = TestFixture<EIPStateTestEOFSuite, DefaultFlags>;
    ETH_REGISTER_DYNAMIC_TEST_SEARCH(EIPStateTestsEOFFixture, "EIPTests/StateTests/stEOF")
    BOOST_FIXTURE_TEST_SUITE(stEOF, EIPStateTestsEOFFixture)
        BOOST_AUTO_TEST_CASE(stEIP3540) {}
    BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()


using BCEIPSuiteFixture = TestFixture<BlockchainTestEIPSuite, DefaultFlags>;
ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCEIPSuiteFixture, "EIPTests/BlockchainTests")
BOOST_FIXTURE_TEST_SUITE(BlockchainTests, BCEIPSuiteFixture)
    using BCEIPStateSuiteFixture = TestFixture<BCEIPStateTestsSuite, RequireOptionAllNotRefillable>;
    ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCEIPStateSuiteFixture, "EIPTests/BlockchainTests/StateTests")
    BOOST_FIXTURE_TEST_SUITE(StateTests, BCEIPStateSuiteFixture)
    BOOST_AUTO_TEST_CASE(stEIP3855) {}
    BOOST_AUTO_TEST_CASE(stEIP3860) {}
        using BCEIPStateTestsEOFFixture = TestFixture<BCEIPStateTestsEOFSuite, RequireOptionAll>;
        ETH_REGISTER_DYNAMIC_TEST_SEARCH(BCEIPStateTestsEOFFixture, "EIPTests/BlockchainTests/StateTests/stEOF")
        BOOST_FIXTURE_TEST_SUITE(stEOF, BCEIPStateTestsEOFFixture)
        BOOST_AUTO_TEST_CASE(stEIP3540) {}
        BOOST_AUTO_TEST_SUITE_END()
    BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END() // EIPTESTS

