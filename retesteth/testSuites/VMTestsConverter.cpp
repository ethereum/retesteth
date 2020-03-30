/*
    This file is part of cpp-ethereum.

    cpp-ethereum is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    cpp-ethereum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/

/** @file VMTestToStateConverterSuite.cpp
 * @author Dimitry Khokhlov <dimitry@ethereum.org>
 * @date 2020
 * VM Tests parser.
 */

#include <boost/filesystem/operations.hpp>
#include <boost/test/unit_test.hpp>
#include <mutex>
#include <thread>

#include <dataObject/DataObject.h>
#include <libdevcore/CommonData.h>
#include <libdevcore/CommonIO.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/RPCSession.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/TestSuite.h>
#include <retesteth/ethObjects/common.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/testSuites/StateTests.h>
#include <retesteth/testSuites/TestFixtures.h>
#include <retesteth/testSuites/VMTestsConverter.h>
#include <retesteth/testSuites/blockchain/BlockchainTests.h>

using namespace std;
using namespace dev;
namespace fs = boost::filesystem;

namespace
{
DataObject getGenesisTemplate()
{
    // Blockchain Test Template
    DataObject genesisBlockHeader;
    genesisBlockHeader["number"] = "0";
    genesisBlockHeader["parentHash"] =
        "0x0000000000000000000000000000000000000000000000000000000000000000";
    genesisBlockHeader["bloom"] =
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000000000000000000000";
    genesisBlockHeader["extraData"] = "0x42";
    genesisBlockHeader["gasUsed"] = "0";
    genesisBlockHeader["mixHash"] =
        "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421";
    genesisBlockHeader["nonce"] = "0x0102030405060708";
    genesisBlockHeader["receiptTrie"] =
        "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421";
    genesisBlockHeader["stateRoot"] =
        "0xf99eb1626cfa6db435c0836235942d7ccaa935f1ae247d3f1c21e495685f903a";
    genesisBlockHeader["transactionsTrie"] =
        "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421";
    genesisBlockHeader["uncleHash"] =
        "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347";
    return genesisBlockHeader;
}

}  // namespace

namespace test
{
// Most Recent StateTestSuite
TestSuite::TestPath VMTestConverterSuite::suiteFolder() const
{
    return TestSuite::TestPath(fs::path("BlockchainTests/ValidBlocks/VMTests"));
}

TestSuite::FillerPath VMTestConverterSuite::suiteFillerFolder() const
{
    return TestSuite::FillerPath(fs::path("src") / "VMTestsFiller");
}

DataObject VMTestConverterSuite::doTests(DataObject const& _input, TestSuiteOptions& _opt) const
{
    DataObject obj;
    checkAtLeastOneTest(_input);
    if (_opt.doFilling)  // convert vmTestFiller into StateTestFiller
    {
        DataObject blockFiller;

        DataObject const& vmFiller = _input.getSubObjects().at(0);
        string const& testname = vmFiller.getKey();
        TestOutputHelper::get().setCurrentTestName(testname);
        TestOutputHelper::get().setCurrentTestInfo(TestInfo("Filler init"));

        scheme_vmTestFiller vmTestFiller(vmFiller);
        TestOutputHelper::get().setCurrentTestInfo(TestInfo("Converting to BlockchainTestFiller"));

        // Prepare _info comment
        string comment;
        if (vmFiller.count("_info"))
            blockFiller["_info"] = vmFiller.atKey("_info");
        if (blockFiller["_info"].count("comment"))
            comment = blockFiller["_info"]["comment"].asString();
        blockFiller["_info"]["comment"] = "Converted from VMTest source. " + comment;

        // Prepare genesisBlockHeader
        blockFiller["genesisBlockHeader"] = getGenesisTemplate();
        blockFiller["genesisBlockHeader"]["coinbase"] =
            vmFiller.atKey("env").atKey("currentCoinbase");
        blockFiller["genesisBlockHeader"]["difficulty"] =
            vmFiller.atKey("env").atKey("currentDifficulty");
        blockFiller["genesisBlockHeader"]["gasLimit"] =
            vmFiller.atKey("env").atKey("currentGasLimit");
        blockFiller["genesisBlockHeader"]["timestamp"] = "0";
        blockFiller["sealEngine"] = "NoProof";  // Disable mining

        // Prepare pre section
        blockFiller["pre"] = vmFiller.atKey("pre");
        // Insert sender account
        string const sender = "a94f5374fce5edbc8e2a8697c15331677e6ebf0b";
        if (!blockFiller["pre"].count(sender))
        {
            blockFiller["pre"][sender]["balance"] = "0x7ffffffffffffff0";
            blockFiller["pre"][sender]["nonce"] = "0";
            blockFiller["pre"][sender]["code"] = "";
            blockFiller["pre"][sender]["storage"] = DataObject(DataType::Object);
        }

        // Prepare block with transaction
        DataObject blockInfo;
        blockInfo["blockHeader"]["timestamp"] = vmFiller.atKey("env").atKey("currentTimestamp");
        blockInfo["blockHeader"]["difficulty"] = vmFiller.atKey("env").atKey("currentDifficulty");
        blockInfo["blockHeader"]["gasLimit"] = vmFiller.atKey("env").atKey("currentGasLimit");

        scheme_transaction const& trInTest =
            vmTestFiller.getTransaction().getTransactions().at(0).transaction;
        blockInfo["transactions"].addArrayObject(trInTest.getData());
        blockInfo["uncleHeaders"] = DataObject(DataType::Array);
        blockFiller["blocks"].addArrayObject(blockInfo);

        // Construct expect section
        DataObject expectSection;
        expectSection["network"].addArrayObject(DataObject(">=Istanbul"));
        if (vmFiller.count("expect"))
            expectSection["result"] = vmFiller.atKey("expect");
        else
        {
            ETH_WARNING("VMTest filler missing expect section! Empty section will be used. " +
                        TestOutputHelper::get().testInfo().getMessage());
            expectSection["result"] = DataObject(DataType::Object);
        }
        blockFiller["expect"].addArrayObject(expectSection);
        obj[testname] = blockFiller;

        try
        {
            BlockchainTestValidSuite blockSuite;
            TestSuiteOptions stateOpt;
            stateOpt.doFilling = true;
            return blockSuite.doTests(obj, stateOpt);
        }
        catch (std::exception const& _ex)
        {
            ETH_LOG("Error when filling the bc test! (" + testname + "): \n" + _ex.what(), 0);
        }
    }
    else  // run tests
    {
        // Execute block tests filled from VMTests
        BlockchainTestValidSuite blockSuite;
        blockSuite.doTests(_input, _opt);
    }
    return obj;
    }

    }  // namespace test
