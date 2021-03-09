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
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/TestSuite.h>
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/testStructures/types/VMTests/VMTestFiller.h>
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
// Use this genesis to generate a blockchain test
BlockHeader getGenesisTemplate()
{
    DataObject genesisBlockHeader;
    genesisBlockHeader["coinbase"] = "0x1122334455667788991011121314151617181920";
    genesisBlockHeader["difficulty"] = "0x11223344";
    genesisBlockHeader["gasLimit"] = "0x11223344";
    genesisBlockHeader["timestamp"] = "0x11223344";
    genesisBlockHeader["number"] = "0x00";
    genesisBlockHeader["parentHash"] = "0x0000000000000000000000000000000000000000000000000000000000000000";
    genesisBlockHeader["bloom"] =
        "0x000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000000000000000000000000000";
    genesisBlockHeader["extraData"] = "0x42";
    genesisBlockHeader["gasUsed"] = "0x00";
    genesisBlockHeader["mixHash"] = "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421";
    genesisBlockHeader["nonce"] = "0x0102030405060708";
    genesisBlockHeader["receiptTrie"] = "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421";
    genesisBlockHeader["stateRoot"] = "0xf99eb1626cfa6db435c0836235942d7ccaa935f1ae247d3f1c21e495685f903a";
    genesisBlockHeader["transactionsTrie"] = "0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421";
    genesisBlockHeader["uncleHash"] = "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347";
    return BlockHeader(genesisBlockHeader);
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
    DataObject res;
    if (_opt.doFilling)  // convert vmTestFiller into StateTestFiller
    {
        VMTestFiller vmtest(_input);
        for (VMTestInFiller const& test : vmtest.tests())
        {
            TestOutputHelper::get().setCurrentTestName(test.testName());
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("Converting to BlockchainTestFiller"));

            DataObject bcTestFiller;
            string const comment = "Converted from VMTest source. ";
            if (test.hasInfo())
                bcTestFiller["_info"]["comment"] = comment + test.Info().comment();
            else
                bcTestFiller["_info"]["comment"] = comment;

            // Prepare genesisBlockHeader
            bcTestFiller["genesisBlockHeader"] = getGenesisTemplate().asDataObject();
            bcTestFiller["genesisBlockHeader"]["coinbase"] = test.Env().currentCoinbase().asString();
            bcTestFiller["genesisBlockHeader"]["difficulty"] = test.Env().currentDifficulty().asString();
            bcTestFiller["genesisBlockHeader"]["gasLimit"] = test.Env().currentGasLimit().asString();
            bcTestFiller["genesisBlockHeader"]["timestamp"] = VALUE(0).asString();

            // Disable mining
            bcTestFiller["sealEngine"] = sealEngineToStr(SealEngine::NoProof);

            // Pre state
            bcTestFiller["pre"] = test.Pre().asDataObject();
            for (auto& acc : bcTestFiller["pre"].getSubObjectsUnsafe())
            {
                const string val = acc.atKey("code").asString();
                if (val.size() > 2)
                    acc.atKeyUnsafe("code").setString(":raw " + val);
            }

            // Insert sender account
            FH20 const sender(DataObject("0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b"));
            if (!test.Pre().hasAccount(sender))
            {
                bcTestFiller["pre"][sender.asString()]["balance"] = "0x7ffffffffffffff0";
                bcTestFiller["pre"][sender.asString()]["nonce"] = "0";
                bcTestFiller["pre"][sender.asString()]["code"] = "";
                bcTestFiller["pre"][sender.asString()]["storage"] = DataObject(DataType::Object);
            }

            // Prepare block with transaction
            DataObject blockInfo;
            blockInfo["blockHeader"]["timestamp"] = test.Env().firstBlockTimestamp().asString();
            blockInfo["blockHeader"]["difficulty"] = test.Env().currentDifficulty().asString();
            blockInfo["blockHeader"]["gasLimit"] = test.Env().currentGasLimit().asString();
            blockInfo["transactions"].addArrayObject(test.Tr().asDataObject());
            blockInfo["uncleHeaders"] = DataObject(DataType::Array);
            bcTestFiller["blocks"].addArrayObject(blockInfo);

            // Construct expect section
            DataObject expectSection;
            expectSection["network"].addArrayObject(DataObject(">=Istanbul"));
            if (test.hasExpect())
                expectSection["result"] = test.Expect().asDataObject();
            else
            {
                ETH_WARNING("VMTest filler missing expect section! Empty section will be used. " +
                            TestOutputHelper::get().testInfo().errorDebug());
                expectSection["result"] = DataObject(DataType::Object);
            }
            bcTestFiller["expect"].addArrayObject(expectSection);
            res[test.testName()] = bcTestFiller;

            try
            {
                BlockchainTestValidSuite blockSuite;
                TestSuiteOptions stateOpt;
                stateOpt.doFilling = true;
                return blockSuite.doTests(res, stateOpt);
            }
            catch (std::exception const& _ex)
            {
                ETH_LOG("Error when filling the bc test! (" + test.testName() + "): \n" + _ex.what(), 0);
            }
        }
    }
    else  // run tests
    {
        // Execute block tests filled from VMTests as blockchain
        BlockchainTestValidSuite blockSuite;
        blockSuite.doTests(_input, _opt);
    }
    return res;
}

}  // namespace test
