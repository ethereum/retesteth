#include <libdataobj/ConvertFile.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <testStructures/types/StateTests/Filler/StateTestFillerExpectSection.h>
#include <testStructures/types/StateTests/Filler/StateTestFillerTransaction.h>

using namespace std;
using namespace test;
using namespace dataobject;

string const transactionCommon = R"(
    "gasLimit" : ["400000"],
    "gasPrice" : "1",
    "nonce" : "0",
    "secretKey" : "45a915e4d060149eb4365960e6a7a45f334393093061116b197e3240065ff2d8",
    "to" : "095e7baea6a6c7c4c2dfeb977efac326af552d87",
    "value" : ["100000"]
)";

StateTestFillerTransaction makeTransaction(std::vector<string> const& _data)
{
    string str = "{ ";
    str += "\"data\" : [";
    for (vector<string>::const_iterator it = _data.begin(); it != _data.end(); it++)
    {
        str += "\"" + *it + "\"";
        if (it + 1 != _data.end())
            str += ", ";
    }
    str += "]," + transactionCommon + "}";
    auto p = ConvertJsoncppStringToData(str);
    return StateTestFillerTransaction(dataobject::move(p));
}

BOOST_FIXTURE_TEST_SUITE(trDataCompileSuite, TestOutputHelperFixture)

BOOST_AUTO_TEST_CASE(compileRaw)
{
    StateTestFillerTransaction tr = makeTransaction({":raw 0x1234"});
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(0).asString() == "0x1234");
}

BOOST_AUTO_TEST_CASE(compileRawLabel)
{
    StateTestFillerTransaction tr = makeTransaction({":label sample :raw 0x1234"});
    BOOST_CHECK(tr.buildTransactions().at(0).transaction()->dataLabel() == ":label sample");
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(0).asString() == "0x1234");
}

BOOST_AUTO_TEST_CASE(compileRawArray)
{
    StateTestFillerTransaction tr = makeTransaction({":raw 0x1234", ":raw 0x2244"});
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(0).asString() == "0x1234");
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(1).asString() == "0x2244");
}

BOOST_AUTO_TEST_CASE(compileRawArrayLabel)
{
    StateTestFillerTransaction tr = makeTransaction({":raw 0x1234", ":label sample :raw 0x2244"});
    BOOST_CHECK(tr.buildTransactions().at(0).transaction()->dataLabel().empty());
    BOOST_CHECK(tr.buildTransactions().at(1).transaction()->dataLabel() == ":label sample");
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(0).asString() == "0x1234");
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(1).asString() == "0x2244");
}

BOOST_AUTO_TEST_CASE(compileLLL)
{
    StateTestFillerTransaction tr = makeTransaction({"{ [[1]] 1 }"});
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(0).asString() == "0x600160015500");
}

BOOST_AUTO_TEST_CASE(compileLLLLabel)
{
    StateTestFillerTransaction tr = makeTransaction({":label sample { [[1]] 1 }"});
    BOOST_CHECK(tr.buildTransactions().at(0).transaction()->dataLabel() == ":label sample");
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(0).asString() == "0x600160015500");
}

BOOST_AUTO_TEST_CASE(compileLLLArray)
{
    StateTestFillerTransaction tr = makeTransaction({"{ [[1]] 1 }", "{ [[1]] 2 }"});
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(0).asString() == "0x600160015500");
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(1).asString() == "0x600260015500");
}

BOOST_AUTO_TEST_CASE(compileLLLArrayLabel)
{
    StateTestFillerTransaction tr = makeTransaction({"{ [[1]] 1 }", ":label sample { [[1]] 2 }"});
    BOOST_CHECK(tr.buildTransactions().at(0).transaction()->dataLabel().empty());
    BOOST_CHECK(tr.buildTransactions().at(1).transaction()->dataLabel() == ":label sample");
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(0).asString() == "0x600160015500");
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(1).asString() == "0x600260015500");
}

BOOST_AUTO_TEST_CASE(compileABI)
{
    StateTestFillerTransaction tr = makeTransaction({":abi f(uint) 12"});
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(0).asString() ==
                "0x693c6139000000000000000000000000000000000000000000000000000000000000000c");
}

BOOST_AUTO_TEST_CASE(compileABILabel)
{
    StateTestFillerTransaction tr = makeTransaction({":label sample :abi f(uint) 12"});
    BOOST_CHECK(tr.buildTransactions().at(0).transaction()->dataLabel() == ":label sample");
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(0).asString() ==
                "0x693c6139000000000000000000000000000000000000000000000000000000000000000c");
}

BOOST_AUTO_TEST_CASE(compileABIArray)
{
    StateTestFillerTransaction tr = makeTransaction({":abi f(uint) 12", ":abi f(uint) 13"});
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(0).asString() ==
                "0x693c6139000000000000000000000000000000000000000000000000000000000000000c");
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(1).asString() ==
                "0x693c6139000000000000000000000000000000000000000000000000000000000000000d");
}

BOOST_AUTO_TEST_CASE(compileABIArrayLabel)
{
    StateTestFillerTransaction tr = makeTransaction({":abi f(uint) 13", ":label sample :abi f(uint) 12"});
    BOOST_CHECK(tr.buildTransactions().at(0).transaction()->dataLabel().empty());
    BOOST_CHECK(tr.buildTransactions().at(1).transaction()->dataLabel() == ":label sample");
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(0).asString() ==
                "0x693c6139000000000000000000000000000000000000000000000000000000000000000d");
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(1).asString() ==
                "0x693c6139000000000000000000000000000000000000000000000000000000000000000c");
}

string const solContract = R"(
   // SPDX-License-Identifier: GPL-3.0
   pragma solidity >=0.4.16 <0.9.0;
   contract B {
     function callback(address x) public {
         (bool success, bytes memory data) = x.call(abi.encodeWithSignature('stor()'));
         assembly { sstore(1,success) }
         success;
         data;
     }
   }
)";

string const solContractC =
    "0x608060405234801561001057600080fd5b506004361061002b5760003560e01c806373027f6d14610030575b600080fd5b6100726004803603602081"
    "101561004657600080fd5b81019080803573ffffffffffffffffffffffffffffffffffffffff169060200190929190505050610074565b005b60006060"
    "8273ffffffffffffffffffffffffffffffffffffffff166040516024016040516020818303038152906040527fb28175c4000000000000000000000000"
    "000000000000000000000000000000007bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19166020820180517bffffffffffffff"
    "ffffffffffffffffffffffffffffffffffffffffff83818316178352505050506040518082805190602001908083835b60208310610145578051825260"
    "2082019150602081019050602083039250610122565b6001836020036101000a0380198251168184511680821785525050505050509050019150506000"
    "604051808303816000865af19150503d80600081146101a7576040519150601f19603f3d011682016040523d82523d6000602084013e6101ac565b6060"
    "91505b50915091508160015550505056fea2646970667358221220a3bfc993384abb4d5ed57244caedd74028e6c86f863f7b27bb5353572b7890fa6473"
    "6f6c63430007000033";

BOOST_AUTO_TEST_CASE(compileSOLC)
{
    StateTestFillerTransaction tr = makeTransaction({solContract});
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(0).asString().substr(0, 50) == solContractC.substr(0, 50));
}

BOOST_AUTO_TEST_CASE(compileSOLCLabel)
{
    StateTestFillerTransaction tr = makeTransaction({":label sample " + solContract});
    BOOST_CHECK(tr.buildTransactions().at(0).transaction()->dataLabel() == ":label sample");
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(0).asString().substr(0, 50) == solContractC.substr(0, 50));
}

BOOST_AUTO_TEST_CASE(compileSOLCArray)
{
    StateTestFillerTransaction tr = makeTransaction({solContract, solContract});
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(0).asString().substr(0, 50) == solContractC.substr(0, 50));
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(1).asString().substr(0, 50) == solContractC.substr(0, 50));
}

BOOST_AUTO_TEST_CASE(compileSOLCArrayLabel)
{
    StateTestFillerTransaction tr = makeTransaction({solContract, ":label sample " + solContract});
    BOOST_CHECK(tr.buildTransactions().at(0).transaction()->dataLabel().empty());
    BOOST_CHECK(tr.buildTransactions().at(1).transaction()->dataLabel() == ":label sample");
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(0).asString().substr(0, 50) == solContractC.substr(0, 50));
    BOOST_CHECK(tr.asDataObject()->atKey("data").at(1).asString().substr(0, 50) == solContractC.substr(0, 50));
}

BOOST_AUTO_TEST_SUITE_END()
