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
/** @file
 * Base functions for all test suites
 */

#include <dataObject/ConvertFile.h>
#include <dataObject/ConvertYaml.h>
#include <dataObject/DataObject.h>
#include <json/reader.h>
#include <json/value.h>
#include <json/version.h>
#include <libdevcore/CommonIO.h>
#include <libdevcore/Log.h>
#include <libdevcore/SHA3.h>
#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/RPCSession.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/TestSuite.h>
#include <boost/test/unit_test.hpp>
#include <string>
#include <thread>


using namespace std;
using namespace dev;
namespace fs = boost::filesystem;

//Helper functions for test proccessing
namespace {
struct TestFileData
{
    DataObject data;
    h256 hash;
};

TestFileData readTestFile(fs::path const& _testFileName)
{
    TestFileData testData;

    // Check that file is not empty
    string const s = dev::contentsString(_testFileName);
    ETH_ERROR_REQUIRE_MESSAGE(
        s.length() > 0, "Contents of " + _testFileName.string() + " is empty.");

    if (_testFileName.extension() == ".json")
        testData.data = dataobject::ConvertJsoncppStringToData(s, string(), true);
    else if (_testFileName.extension() == ".yml")
        testData.data = dataobject::ConvertYamlToData(YAML::Load(s));
    else
        ETH_ERROR_MESSAGE(
            "Unknown test format!" + test::TestOutputHelper::get().testFile().string());

    string srcString = testData.data.asJson(0, false); //json_spirit::write_string(testData.data, false);
    if (test::Options::get().showhash)
    {
        std::string output = "Not a Json object!";
        if (_testFileName.extension() == ".json")
        {
            Json::FastWriter fastWriter;
            Json::Value v = readJson(_testFileName);
            output = fastWriter.write(v);
            output = output.substr(0, output.size() - 1);
        }
        std::cerr << "JSON: '" << std::endl << output << "'" << std::endl;
        std::cerr << "DATA: '" << std::endl << srcString << "'" << std::endl;
    }
    testData.hash = sha3(srcString);
    return testData;
}

void removeComments(dataobject::DataObject& _obj)
{
    if (_obj.type() == dataobject::DataType::Object)
    {
		list<string> removeList;
        for (auto& i: _obj.getSubObjectsUnsafe())
		{
            if (i.getKey().substr(0, 2) == "//")
			{
                removeList.push_back(i.getKey());
				continue;
			}
            removeComments(i);
		}
        for (auto const& i: removeList)
            _obj.removeKey(i);
	}
    else if (_obj.type() == dataobject::DataType::Array)
    {
        for (auto& i: _obj.getSubObjectsUnsafe())
			removeComments(i);
    }
}

void addClientInfo(
    dataobject::DataObject& _v, fs::path const& _testSource, h256 const& _testSourceHash)
{
  RPCSession &session = RPCSession::instance(TestOutputHelper::getThreadID());
  for (auto& o : _v.getSubObjectsUnsafe())
  {
      string comment;
      dataobject::DataObject clientinfo;
      if (o.count("_info"))
      {
          dataobject::DataObject const& existingInfo = o.atKey("_info");
          if (existingInfo.count("comment"))
              comment = existingInfo.atKey("comment").asString();
      }

      clientinfo.setKey("_info");
      clientinfo["comment"] = comment;
      clientinfo["filling-rpc-server"] = session.web3_clientVersion();
      clientinfo["filling-tool-version"] = test::prepareVersionString();
      clientinfo["lllcversion"] = test::prepareLLLCVersionString();
      clientinfo["source"] = _testSource.string();
      clientinfo["sourceHash"] = toString(_testSourceHash);

      o["_info"].replace(clientinfo);
      o.setKeyPos("_info", 0);
    }
}

void checkFillerHash(fs::path const& _compiledTest, fs::path const& _sourceTest)
{
    dataobject::DataObject v =
        dataobject::ConvertJsoncppStringToData(dev::contentsString(_compiledTest), "_info");
    TestFileData fillerData = readTestFile(_sourceTest);
    for (auto const& i: v.getSubObjects())
    {
        try
        {
            // use eth object _info section class here !!!!!
            ETH_ERROR_REQUIRE_MESSAGE(i.type() == dataobject::DataType::Object,
                i.getKey() + " should contain an object under a test name.");
            ETH_ERROR_REQUIRE_MESSAGE(
                i.count("_info") > 0, "_info section not set! " + _compiledTest.string());
            dataobject::DataObject const& info = i.atKey("_info");
            ETH_ERROR_REQUIRE_MESSAGE(info.count("sourceHash") > 0,
                "sourceHash not found in " + _compiledTest.string() + " in " + i.getKey());
            h256 const sourceHash = h256(info.atKey("sourceHash").asString());
            ETH_ERROR_REQUIRE_MESSAGE(sourceHash == fillerData.hash,
                "Test " + _compiledTest.string() + " in " + i.getKey() +
                    " is outdated. Filler hash is different! ( '" + sourceHash.hex().substr(0, 4) +
                    "' != '" + fillerData.hash.hex().substr(0, 4) + "') ");
        }
        catch (test::BaseEthException const&)
        {
            continue;
        }
    }
}

void joinThreads(vector<thread>& _threadVector, bool _all)
{
    if (_all)
    {
        for (auto& th : _threadVector)
        {
            string id = toString(th.get_id());
            th.join();
            // A thread with exception thrown still being joined here!
            RPCSession::sessionEnd(id, RPCSession::SessionStatus::Available);
        }
        _threadVector.clear();
        if (ExitHandler::receivedExitSignal())
        {
            // if one of the tests threads failed with fatal exception
            // stop retesteth execution
            TestOutputHelper::get().finishTest();
            ExitHandler::doExit();
        }
        return;  // otherwise continue test execution
    }

    bool finished = false;
    while (!finished)
    {
        for (vector<thread>::iterator it = _threadVector.begin(); it != _threadVector.end(); it++)
        {
            finished =
                (RPCSession::sessionStatus(toString((*it).get_id())) == RPCSession::HasFinished);
            if (finished)
            {
                string id = toString((*it).get_id());
                (*it).join();
                RPCSession::sessionEnd(id, RPCSession::SessionStatus::Available);
                _threadVector.erase(it);
                return;
            }
        }
    }
}
}

namespace test
{
string const c_fillerPostf = "Filler";
string const c_copierPostf = "Copier";

void TestSuite::runTestWithoutFiller(boost::filesystem::path const& _file) const
{
    // Allow to execute a custom test .json file on any test suite
    auto& testOutput = test::TestOutputHelper::get();
    testOutput.initTest(1);
    executeFile(_file);
    testOutput.finishTest();
}

string TestSuite::checkFillerExistance(string const& _testFolder) const
{
    string filter = test::Options::get().singleTestName.empty() ?
                        string() :
                        test::Options::get().singleTestName;
    std::cout << "Filter: '" << filter << "'" << std::endl;
    AbsoluteTestPath testsPath = getFullPath(_testFolder);
    if (!fs::exists(testsPath.path()))
    {
        ETH_STDERROR_MESSAGE("Tests folder does not exists, creating test folder: '" +
                             string(testsPath.path().c_str()) + "'");
        fs::create_directories(testsPath.path());
    }
    vector<fs::path> compiledFiles = test::getFiles(testsPath.path(), {".json", ".yml"}, filter);
    AbsoluteFillerPath fullPathToFillers = getFullPathFiller(_testFolder);

    bool checkFillerWhenFilterIsSetButNoTestsFilled = false;
    if (compiledFiles.size() == 0)
    {
        if (filter.empty())
        {
            // No tests generated, check at least one filler existence
            vector<fs::path> existingFillers =
                test::getFiles(fullPathToFillers.path(), {".json", ".yml"});
            for (auto const& filler : existingFillers)
            {
                // put filler names as if it was actual tests
                string fillerName(filler.stem().c_str());
                string fillerSuffix = fillerName.substr(fillerName.size() - 6);
                if (fillerSuffix == c_fillerPostf || fillerSuffix == c_copierPostf)
                    compiledFiles.push_back(fillerName.substr(0, fillerName.size() - 6));
            }
        }
        else
        {
            // No tests generated and filter is set, check that filler for filter is exist
            compiledFiles.push_back(fs::path(filter));  // put the test name as if it was compiled.
            checkFillerWhenFilterIsSetButNoTestsFilled = true;
        }
    }

    for (auto const& file : compiledFiles)
    {
        fs::path const expectedFillerName =
            fullPathToFillers.path() / fs::path(file.stem().string() + c_fillerPostf + ".json");
        fs::path const expectedFillerName2 =
            fullPathToFillers.path() / fs::path(file.stem().string() + c_fillerPostf + ".yml");
        fs::path const expectedCopierName =
            fullPathToFillers.path() / fs::path(file.stem().string() + c_copierPostf + ".json");

        string exceptionStr;
        if (checkFillerWhenFilterIsSetButNoTestsFilled)
            exceptionStr = "Could not find a filler for provided --singletest filter: '" +
                           file.filename().string() +
                           "', or no tests were found in the test suite folder!";
        else
            exceptionStr =
                "Compiled test folder contains test without Filler: " + file.filename().string();
        ETH_ERROR_REQUIRE_MESSAGE(fs::exists(expectedFillerName) ||
                                      fs::exists(expectedFillerName2) ||
                                      fs::exists(expectedCopierName),
            exceptionStr);
        ETH_ERROR_REQUIRE_MESSAGE(
            !(fs::exists(expectedFillerName) && fs::exists(expectedFillerName2) &&
                fs::exists(expectedCopierName)),
            "Src test could either be Filler.json, Filler.yml or Copier.json: " +
                file.filename().string());

        // Check that filled tests created from actual fillers depenging on a test type
        if (fs::exists(expectedFillerName))
        {
            if (Options::get().filltests == false)  // If we are filling the test it is probably
                                                    // outdated/being updated. no need to check.
                checkFillerHash(file, expectedFillerName);
            if (!filter.empty())
                return filter + c_fillerPostf;
        }
        if (fs::exists(expectedFillerName2))
        {
            if (Options::get().filltests == false)
                checkFillerHash(file, expectedFillerName2);
            if (!filter.empty())
                return filter + c_fillerPostf;
        }
        if (fs::exists(expectedCopierName))
        {
            if (Options::get().filltests == false)
                checkFillerHash(file, expectedCopierName);
            if (!filter.empty())
                return filter + c_copierPostf;
        }
    }

    // No compiled test files. Filter is empty
    return filter;
}

void TestSuite::runAllTestsInFolder(string const& _testFolder) const
{
    if (ExitHandler::receivedExitSignal())
    {
        auto& testOutput = test::TestOutputHelper::get();
        testOutput.finishTest();
        return;
    }

    // check that destination folder test files has according Filler file in src folder
    string const filter = checkFillerExistance(_testFolder);

    // run all tests
    AbsoluteFillerPath fillerPath = getFullPathFiller(_testFolder);
    vector<fs::path> const files = test::getFiles(fillerPath.path(), {".json", ".yml"}, filter);

    // repeat this part for all connected clients
    auto thisPart = [this, &files, &_testFolder]() {
        auto& testOutput = test::TestOutputHelper::get();
        vector<thread> threadVector;
        testOutput.initTest(files.size());
        for (auto const& file : files)
        {
            if (ExitHandler::receivedExitSignal())
            {
                testOutput.finishTest();
                break;
            }
            testOutput.showProgress();

            // If debugging, already there is an open instance of a client.
            // Only one thread allowed to connect to it;
            size_t maxAllowedThreads = Options::get().threadCount;
            ClientConfig const& currConfig = Options::get().getDynamicOptions().getCurrentConfig();
            Socket::SocketType socType = currConfig.getSocketType();
            if (socType == Socket::SocketType::IPCDebug)
                maxAllowedThreads = 1;
            // If connecting to TCP sockets. Max threads are limited with tcp ports provided
            if (socType == Socket::SocketType::TCP)
                maxAllowedThreads = min(maxAllowedThreads, currConfig.getAddressObject().getSubObjects().size());

            if (threadVector.size() == maxAllowedThreads)
                joinThreads(threadVector, false);
            thread testThread(&TestSuite::executeTest, this, _testFolder, file);
            threadVector.push_back(std::move(testThread));
        }
        joinThreads(threadVector, true);
        testOutput.finishTest();
    };
    runFunctionForAllClients(thisPart);
}


void TestSuite::runFunctionForAllClients(std::function<void()> _func)
{
    for (auto const& config : Options::getDynamicOptions().getClientConfigs())
    {
        Options::getDynamicOptions().setCurrentConfig(config);
        std::cout << "Running tests for config '" << config.getName() << "' " << config.getId().id()
                  << std::endl;
        _func();
        // leave current connections open for the next tests (if commented)
        // RPCSession::clear();
    }
}

TestSuite::AbsoluteFillerPath TestSuite::getFullPathFiller(string const& _testFolder) const
{
    return TestSuite::AbsoluteFillerPath(
        test::getTestPath() / suiteFillerFolder().path() / _testFolder);
}

TestSuite::AbsoluteTestPath TestSuite::getFullPath(string const& _testFolder) const
{
    return TestSuite::AbsoluteTestPath(test::getTestPath() / suiteFolder().path() / _testFolder);
}

void TestSuite::executeTest(string const& _testFolder, fs::path const& _testFileName) const
{
    RPCSession::sessionStart(TestOutputHelper::getThreadID());
    TestOutputHelper::get().setCurrentTestFile(_testFileName);
    fs::path const boostRelativeTestPath = fs::relative(_testFileName, getTestPath());
    string testname = _testFileName.stem().string();
    bool isCopySource = false;
    size_t pos = testname.rfind(c_fillerPostf);
    if (pos != string::npos)
        testname = testname.substr(0, pos);
    else
    {
        pos = testname.rfind(c_copierPostf);
        if (pos != string::npos)
        {
            testname = testname.substr(0, pos);
            isCopySource = true;
        }
        else
            ETH_FAIL_REQUIRE_MESSAGE(
                false, "Incorrect file suffix in the filler folder! " + _testFileName.string());
    }

    ETH_LOG("Running " + testname + ": ", 3);
    // Filename of the test that would be generated
    AbsoluteTestPath const boostTestPath =
        getFullPath(_testFolder).path() / fs::path(testname + ".json");

    TestSuiteOptions opt;
    if (Options::get().filltests)
    {
        if (isCopySource)
        {
            clog << "Copying " << _testFileName.string() << "\n";
            clog << " TO " << boostTestPath.path().string() << "\n";
            assert(_testFileName.string() != boostTestPath.path().string());
            TestOutputHelper::get().showProgress();
            test::copyFile(_testFileName, boostTestPath.path());
            ETH_FAIL_REQUIRE_MESSAGE(boost::filesystem::exists(boostTestPath.path().string()),
                "Error when copying the test file!");

            // Update _info and build information of the copied test
            /*Json::Value v;
            string const s = asString(dev::contents(boostTestPath));
            json_spirit::read_string(s, v);
            addClientInfo(v, boostRelativeTestPath, sha3(dev::contents(_testFileName)));
            writeFile(boostTestPath, asBytes(json_spirit::write_string(v, true)));*/
        }
        else
        {
            TestFileData testData = readTestFile(_testFileName);
            removeComments(testData.data);
            opt.doFilling = true;

            try
            {
                DataObject output = doTests(testData.data, opt);
                if (!opt.wasErrors)
                {
                    // Add client info for all of the tests in output
                    addClientInfo(output, boostRelativeTestPath, testData.hash);
                    writeFile(boostTestPath.path(), asBytes(output.asJson()));
                }
            }
            catch (test::BaseEthException const&)
            {
                // Something went wrong inside the test. skip it.
                // (error message is stored at TestOutputHelper)
                opt.wasErrors = true;
            }
            catch (std::exception const& _ex)
            {
                ETH_ERROR_MESSAGE("ERROR OCCURED FILLING TESTS: " + string(_ex.what()));
                RPCSession::sessionEnd(TestOutputHelper::getThreadID(), RPCSession::SessionStatus::HasFinished);
            }
        }
    }

    if (!opt.wasErrors && !opt.disableSecondRun)
    {
        try
        {
            TestOutputHelper::get().setCurrentTestFile(boostTestPath.path());
            executeFile(boostTestPath.path());
        }
        catch (test::BaseEthException const&)
        {
            // Something went wrong inside the test. skip it. (error message is stored at
            // TestOutputHelper)
            opt.wasErrors = true;
        }
        catch (std::exception const& _ex)
        {
            ETH_ERROR_MESSAGE("ERROR OCCURED RUNNING TESTS: " + string(_ex.what()));
            RPCSession::sessionEnd(TestOutputHelper::getThreadID(), RPCSession::SessionStatus::HasFinished);
        }
    }
    RPCSession::sessionEnd(TestOutputHelper::getThreadID(), RPCSession::SessionStatus::HasFinished);
}

void TestSuite::executeFile(boost::filesystem::path const& _file) const
{
    TestSuiteOptions opt;
    doTests(dataobject::ConvertJsoncppStringToData(dev::contentsString(_file)), opt);
}

}

