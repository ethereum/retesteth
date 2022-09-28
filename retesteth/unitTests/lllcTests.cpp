#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::compiler;

BOOST_FIXTURE_TEST_SUITE(LLLCSuite, TestOutputHelperFixture)

BOOST_AUTO_TEST_CASE(lllcVersionString)
{
    string version = prepareLLLCVersionString();
    std::cout << "Got LLLC version string: '" + version + "'" << std::endl;
    ETH_FAIL_REQUIRE_MESSAGE(version.find("Error") == string::npos, "Test lllcVersionString failed with warning: " + version);
}

BOOST_AUTO_TEST_CASE(lllcCompile_0_10)
{
    string lllccode = "{ (ADD 1 1) (MUL 1 1) (SUB 1 1) (DIV 1 1) (SDIV 1 1) (MOD 1 1) (SMOD 1 1) (ADDMOD 1 2 3) (MULMOD 1 2 3) (EXP 1 2) (SIGNEXTEND 1 2) }";
    lllccode = replaceCode(lllccode);
    ETH_FAIL_REQUIRE(lllccode == "0x60016001015060016001025060016001035060016001045060016001055060016001065060016001075060036002600108506003600260010950600260010a50600260010b00");
}

BOOST_AUTO_TEST_CASE(lllcCompile_10_20)
{
    string lllccode = "{ (LT 1 2) (GT 1 2) (SLT 1 2) (SGT 1 2) (EQ 1 2) (ISZERO 1) (AND 1 2) (OR 1 2) (XOR 12 2) (NOT 1) (BYTE 1 2) }";
    lllccode = replaceCode(lllccode);
    ETH_FAIL_REQUIRE(lllccode == "0x600260011050600260011150600260011250600260011350600260011450600115506002600116506002600117506002600c185060011950600260011a00");
}

BOOST_AUTO_TEST_CASE(lllcCompile_30_40)
{
    string lllccode =
        "{ (KECCAK256 0x00 0xff) (ADDRESS) (BALANCE 1) (ORIGIN) (CALLER) (CALLVALUE) (CALLDATALOAD 0) (CALLDATASIZE) "
        "(CALLDATACOPY 1 2 3) (CODESIZE) (CODECOPY 1 2 3) (GASPRICE) (EXTCODESIZE 1) (EXTCODECOPY 1 2 3 4) (RETURNDATASIZE) "
        "(RETURNDATACOPY 1 2 3)}";
    lllccode = replaceCode(lllccode);
    ETH_FAIL_REQUIRE(lllccode ==
                     "0x60ff60002050305060013150325033503450600035503650600360026001373850600360026001393a5060013b5060046003600"
                     "260013c3d506003600260013e00");
}

BOOST_AUTO_TEST_CASE(lllcCompile_40_50)
{
    string lllccode = "{ (BLOCKHASH 1) (COINBASE) (TIMESTAMP) (NUMBER) (DIFFICULTY) (GASLIMIT) (CHAINID) (SELFBALANCE) }";
    lllccode = replaceCode(lllccode);
    ETH_FAIL_REQUIRE(lllccode == "0x600140504150425043504450455046504700");
}

BOOST_AUTO_TEST_CASE(lllcCompile_50_60)
{
    // (JUMPDEST) ??
    string lllccode = "{ (POP 1) (MLOAD 1) (MSTORE 1 2) (MSTORE8 1 2) (SLOAD 1) (SSTORE 1 2) (JUMP 1) (JUMPI 1 2) (PC) (MSIZE) (GAS) }";
    lllccode = replaceCode(lllccode);
    ETH_FAIL_REQUIRE(lllccode == "0x60015060015150600260015260026001536001545060026001556001566002600157585059505a00");
}

/*
BOOST_AUTO_TEST_CASE(lllcCompile_60_80)
{
    string lllccode = string() + "{ (PUSH1) (PUSH2) (PUSH3) (PUSH4) (PUSH5) (PUSH6) (PUSH7) (PUSH8) (PUSH9) (PUSH10) (PUSH11) (PUSH12) " +
            "(PUSH13) (PUSH14) (PUSH15) (PUSH16) (PUSH17) (PUSH18) (PUSH19) (PUSH20) (PUSH21) (PUSH22) (PUSH23)" +
            "(PUSH24) (PUSH25) (PUSH26) (PUSH27) (PUSH28) (PUSH29) (PUSH30) (PUSH31) (PUSH32) }";
    lllccode = replaceCode(lllccode);
    std::cerr << lllccode << std::endl;
    ETH_FAIL_REQUIRE(lllccode == "0x60015060015150600260015260026001536001545060026001556001566002600157585059505a00");
}
*/

/*
BOOST_AUTO_TEST_CASE(lllcCompile_80_90)
{
    string lllccode = string() + "{ " +
            "(DUP1 1) (DUP2 1 2) (DUP3 1 2 3) (DUP4 1 2 3 4) (DUP5 1 2 3 4 5) (DUP6 1 2 3 4 5 6) (DUP7 1 2 3 4 5 6 7) " +
            "(DUP8 1 2 3 4 5 6 7 8) (DUP9 1 2 3 4 5 6 7 8 9) (DUP10 1 2 3 4 5 6 7 8 9 10) (DUP11 1 2 3 4 5 6 7 8 9 10 11) " +
            "(DUP12 1 2 3 4 5 6 7 8 9 10 11 12) (DUP13 1 2 3 4 5 6 7 8 9 10 11 12 13) (DUP14 1 2 3 4 5 6 7 8 9 10 11 12 13 14) " +
            "(DUP15 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15) (DUP16 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16) " +
          "}";
    lllccode = replaceCode(lllccode);
    std::cerr << lllccode << std::endl;
    ETH_FAIL_REQUIRE(lllccode == "0x60015060015150600260015260026001536001545060026001556001566002600157585059505a00");
}
*/

/*
BOOST_AUTO_TEST_CASE(lllcCompile_90_100)
{
    string lllccode = string() + "{" +
           " (SWAP1 1 2) (SWAP2 1 2 3) (SWAP3 1 2 3 4) (SWAP4 1 2 3 4 5) (SWAP5 1 2 3 4 5 6) (SWAP6 1 2 3 4 5 6 7) " +
           " (SWAP7 1 2 3 4 5 6 7 8) (SWAP7 1 2 3 4 5 6 7 8) (SWAP8 1 2 3 4 5 6 7 8 9) (SWAP9 1 2 3 4 5 6 7 8 9 10) " +
           " (SWAP10 1 2 3 4 5 6 7 8 9 10 11) (SWAP10 1 2 3 4 5 6 7 8 9 10 11) (SWAP11 1 2 3 4 5 6 7 8 9 10 11 12) " +
           " (SWAP12 1 2 3 4 5 6 7 8 9 10 11 12 13) (SWAP13 1 2 3 4 5 6 7 8 9 10 11 12 13 14) " +
           " (SWAP14 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15) (SWAP15 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16) " +
           " (SWAP16 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16) " +
         "}";
    lllccode = replaceCode(lllccode);
    std::cerr << lllccode << std::endl;
    ETH_FAIL_REQUIRE(lllccode == "0x60015060015150600260015260026001536001545060026001556001566002600157585059505a00");
}
*/

BOOST_AUTO_TEST_CASE(lllcCompile_a0_b0)
{
    string lllccode = string() + "{ (LOG0 0 1) (LOG1 0 1 2) (LOG2 0 1 2 3) (LOG3 0 1 2 3 4) (LOG4 0 1 2 3 4 5) }";
    lllccode = replaceCode(lllccode);
    ETH_FAIL_REQUIRE(lllccode == "0x60016000a0600260016000a16003600260016000a260046003600260016000a3600560046003600260016000a400");
}

BOOST_AUTO_TEST_CASE(lllcCompile_f0_ff)
{
    string lllccode = string() + "{ (CREATE 1 2 3) (CALL 1 2 3 4 5 6 7) (CALLCODE 1 2 3 4 5 6 7) (RETURN 1 2) (DELEGATECALL 1 2 3 4 5 6) (STATICCALL 1 2 3 4 5 6) (REVERT 1 2) (SELFDESTRUCT 1) }";
    lllccode = replaceCode(lllccode);
    ETH_FAIL_REQUIRE(lllccode == "0x600360026001f0506007600660056004600360026001f1506007600660056004600360026001f25060026001f3600660056004600360026001f450600660056004600360026001fa5060026001fd6001ff00");
}

BOOST_AUTO_TEST_SUITE_END()

