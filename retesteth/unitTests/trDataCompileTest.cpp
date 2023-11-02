#include <libdataobj/ConvertFile.h>
#include <retesteth/Options.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <testStructures/types/StateTests/Filler/StateTestFillerTransaction.h>

using namespace std;
using namespace test;
using namespace dataobject;

class Initializer : public TestOutputHelperFixture
{
public:
    Initializer()
    {
        for (auto const& config : Options::getDynamicOptions().getClientConfigs())
        {
            Options::getDynamicOptions().setCurrentConfig(config);
            break;
        }
    }
};

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

BOOST_FIXTURE_TEST_SUITE(trDataCompileSuite, Initializer)

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
    "0x608060405234801561000f575f80fd5b5060043610610029575f3560e01c806373027f6d1461002d575b5f80fd5b6100476"
    "004803603810190610042919061019c565b610049565b005b5f808273ffffffffffffffffffffffffffffffffffffffff1660"
    "40516024016040516020818303038152906040527fb28175c4000000000000000000000000000000000000000000000000000"
    "000007bffffffffffffffffffffffffffffffffffffffffffffffffffffffff19166020820180517bffffffffffffffffffff"
    "ffffffffffffffffffffffffffffffffffff83818316178352505050506040516100f29190610233565b5f604051808303815"
    "f865af19150503d805f811461012b576040519150601f19603f3d011682016040523d82523d5f602084013e610130565b6060"
    "91505b509150915081600155505050565b5f80fd5b5f73ffffffffffffffffffffffffffffffffffffffff821690509190505"
    "65b5f61016b82610142565b9050919050565b61017b81610161565b8114610185575f80fd5b50565b5f813590506101968161"
    "0172565b92915050565b5f602082840312156101b1576101b061013e565b5b5f6101be84828501610188565b9150509291505"
    "0565b5f81519050919050565b5f81905092915050565b5f5b838110156101f85780820151818401526020810190506101dd56"
    "5b5f8484015250505050565b5f61020d826101c7565b61021781856101d1565b93506102278185602086016101db565b80840"
    "191505092915050565b5f61023e8284610203565b91508190509291505056fea26469706673582212206b2f2cd16e1520078e"
    "62358a8db43c71155a7c7c30b0502d25d6e97e20f5a29564736f6c63430008150033";

BOOST_AUTO_TEST_CASE(compileSOLC)
{
    StateTestFillerTransaction tr = makeTransaction({solContract});
    bool condition = tr.asDataObject()->atKey("data").at(0).asString().substr(0, 50) == solContractC.substr(0, 50);
    BOOST_CHECK(condition);
    if (!condition)
        BOOST_ERROR(tr.asDataObject()->atKey("data").at(0).asString());
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
