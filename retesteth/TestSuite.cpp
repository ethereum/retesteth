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

#include <dataObject/DataObject.h>
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testSuites/TestFixtures.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/TestSuite.h>
#include <retesteth/session/Session.h>
#include <boost/test/unit_test.hpp>
#include <string>
#include <thread>

using namespace std;
using namespace dev;
using namespace test;
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
    ETH_LOG("Read json structure " + string(_testFileName.filename().c_str()), 5);
    TestFileData testData;
    if (_testFileName.extension() == ".json")
        testData.data = test::readJsonData(_testFileName, string(), true);
    else if (_testFileName.extension() == ".yml")
        testData.data = test::readYamlData(_testFileName);
    else
        ETH_ERROR_MESSAGE(
            "Unknown test format!" + test::TestOutputHelper::get().testFile().string());
    ETH_LOG("Read json structure finish", 5);

    string srcString = testData.data.asJson(0, false); //json_spirit::write_string(testData.data, false);
    if (test::Options::get().showhash)
    {
        std::string output = "Not a Json object!";
#ifdef JSONCPP
        if (_testFileName.extension() == ".json")
        {
            Json::FastWriter fastWriter;
            Json::Value v = readJson(_testFileName);
            output = fastWriter.write(v);
            output = output.substr(0, output.size() - 1);
        }
#else
        output = "compile with -DJSONCPP flag to get the json print";
#endif

        std::cerr << "JSON: '" << std::endl << output << "'" << std::endl;
        std::cerr << "DATA: '" << std::endl << srcString << "'" << std::endl;
    }
    testData.hash = sha3(srcString);
    return testData;
}

void removeComments(DataObject& _obj)
{
    if (_obj.type() == DataType::Object)
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
    else if (_obj.type() == DataType::Array)
    {
        for (auto& i: _obj.getSubObjectsUnsafe())
			removeComments(i);
    }
}

void addClientInfo(DataObject& _v, fs::path const& _testSource, h256 const& _testSourceHash)
{
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());
    for (auto& o : _v.getSubObjectsUnsafe())
    {
        string comment;
        DataObject clientinfo;
        clientinfo.setKey("_info");
        if (o.count("_info"))
        {
            DataObject const& existingInfo = o.atKey("_info");
            if (existingInfo.count("comment"))
                comment = existingInfo.atKey("comment").asString();
            if (existingInfo.count("labels"))
                clientinfo["labels"] = existingInfo.atKey("labels");
        }

        clientinfo["comment"] = comment;
        clientinfo["filling-rpc-server"] = session.web3_clientVersion();
        clientinfo["filling-tool-version"] = test::prepareVersionString();
        clientinfo["lllcversion"] = test::prepareLLLCVersionString();
        clientinfo["source"] = _testSource.string();
        clientinfo["sourceHash"] = toString(_testSourceHash);
        if (clientinfo.count("labels"))
            clientinfo.setKeyPos("labels", clientinfo.getSubObjects().size() - 1);

        o["_info"].replace(clientinfo);
        o.setKeyPos("_info", 0);
    }
}

void checkFillerHash(fs::path const& _compiledTest, fs::path const& _sourceTest)
{
    DataObject v = test::readJsonData(_compiledTest, "_info");
    TestFileData fillerData = readTestFile(_sourceTest);
    for (auto const& i: v.getSubObjects())
    {
        try
        {
            // use eth object _info section class here !!!!!
            ETH_ERROR_REQUIRE_MESSAGE(
                i.type() == DataType::Object, i.getKey() + " should contain an object under a test name.");
            ETH_ERROR_REQUIRE_MESSAGE(
                i.count("_info") > 0, "_info section not set! " + _compiledTest.string());
            DataObject const& info = i.atKey("_info");
            ETH_ERROR_REQUIRE_MESSAGE(info.count("sourceHash") > 0,
                "sourceHash not found in " + _compiledTest.string() + " in " + i.getKey());
            h256 const sourceHash = h256(info.atKey("sourceHash").asString());
            ETH_ERROR_REQUIRE_MESSAGE(sourceHash == fillerData.hash,
                "Test " + _compiledTest.string() +
                    " is outdated. Filler hash is different! ('" + sourceHash.hex().substr(0, 4) +
                    "' != '" + fillerData.hash.hex().substr(0, 4) + "') ");
        }
        catch (test::UpwardsException const&)
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
            thread::id const id = th.get_id();
            th.join();
            // A thread with exception thrown still being joined here!
            RPCSession::sessionEnd(id, RPCSession::SessionStatus::Available);
        }
        _threadVector.clear();
        if (ExitHandler::receivedExitSignal())
        {
            // if one of the tests threads failed with fatal exception
            // stop retesteth execution
            ExitHandler::doExit();
        }
        return;  // otherwise continue test execution
    }

    bool finished = false;
    while (!finished)
    {
        for (vector<thread>::iterator it = _threadVector.begin(); it != _threadVector.end(); it++)
        {
            auto status = RPCSession::sessionStatus((*it).get_id());
            if (status == RPCSession::HasFinished)
            {
                thread::id const id = (*it).get_id();
                (*it).join();
                RPCSession::sessionEnd(id, RPCSession::SessionStatus::Available);
                _threadVector.erase(it);
                return;
            }
            else if (status == RPCSession::NotExist && ExitHandler::receivedExitSignal())
                return;
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
    try
    {
        for (auto const& config : Options::getDynamicOptions().getClientConfigs())
        {
            Options::getDynamicOptions().setCurrentConfig(config);

            std::cout << "Running tests for config '" << config.cfgFile().name() << "' " << config.getId().id() << std::endl;
            ETH_LOG("Running " + _file.filename().string() + ": ", 3);

            // Allow to execute a custom test .json file on any test suite
            auto& testOutput = test::TestOutputHelper::get();
            testOutput.initTest(1);

            try
            {
                if (Options::get().filltests)
                {
                    TestFileData testData = readTestFile(_file);
                    removeComments(testData.data);

                    string fileName = _file.stem().c_str();
                    if (fileName.find("Filler") == string::npos)
                        ETH_ERROR_MESSAGE(
                            "Trying to fill `" + string(_file.c_str()) + "`, but file does not have Filler suffix!");

                    // output filename. substract Filler suffix
                    fileName = fileName.substr(0, fileName.length() - 6) + ".json";

                    fs::path outPath;
                    if (Options::get().singleTestOutFile.is_initialized())
                        outPath = fs::path(Options::get().singleTestOutFile.get());
                    else
                        outPath = _file.parent_path() / fileName;

                    TestSuiteOptions opt;
                    opt.doFilling = true;
                    opt.allowInvalidBlocks = true;
                    DataObject output = doTests(testData.data, opt);
                    addClientInfo(output, _file, testData.hash);
                    writeFile(outPath, asBytes(output.asJson()));
                }
                else
                    executeFile(_file);
            }
            catch (test::EthError const& _ex)
            {
                // Something went wrong inside the test. skip the test.
                // (error message is stored at TestOutputHelper. EthError is via ETH_ERROR_())
            }
            catch (test::UpwardsException const& _ex)
            {
                // UpwardsException is thrown upwards in tests for debug info
                // And it should be catched on upper level for report till this point
                ETH_ERROR_MESSAGE(string("Unhandled UpwardsException: ") + _ex.what());
            }
            catch (std::exception const& _ex)
            {
                if (!ExitHandler::receivedExitSignal())
                    ETH_ERROR_MESSAGE("ERROR OCCURED TESTFILE RUN: " + string(_ex.what()));
                RPCSession::sessionEnd(TestOutputHelper::getThreadID(), RPCSession::SessionStatus::HasFinished);
            }

            testOutput.finishTest();
            // Disconnect threads from the client
            if (Options::getDynamicOptions().getClientConfigs().size() > 1)
                RPCSession::clear();
        }
    }
    catch (std::exception const&)
    {
        test::TestOutputHelper::get().finishTest();
        test::TestOutputHelper::printTestExecStats();
    }
}

string TestSuite::checkFillerExistance(string const& _testFolder) const
{
    test::Options const& opt = test::Options::get();
    string const testNameFilter = opt.singleTestName.empty() ? string() : opt.singleTestName;
    string filter = testNameFilter;
    filter += opt.singleTestNet.empty() ? string() : " " + opt.singleTestNet;
    filter += opt.getGStateTransactionFilter();
    ETH_LOG("Checking test filler hashes for " + boost::unit_test::framework::current_test_case().full_name(), 4);
    if (!filter.empty())
        ETH_LOG("Filter: '" + filter +  "'", 0);
    AbsoluteTestPath testsPath = getFullPath(_testFolder);
    if (!fs::exists(testsPath.path()))
    {
        ETH_LOG("Tests folder does not exists, creating test folder: '" + string(testsPath.path().c_str()) + "'", 2);
        fs::create_directories(testsPath.path());
    }
    vector<fs::path> compiledFiles = test::getFiles(testsPath.path(), {".json", ".yml"}, testNameFilter);
    AbsoluteFillerPath fullPathToFillers = getFullPathFiller(_testFolder);

    // Check unfilled tests
    if (Options::get().checkhash)
    {
        vector<fs::path> fillerFiles = test::getFiles(fullPathToFillers.path(), {".json", ".yml"}, testNameFilter);
        if (fillerFiles.size() > compiledFiles.size())
        {
            string message = "Tests are not generated: ";
            for (auto const& filler : fillerFiles)
            {
                bool found = false;
                for (auto const& filled : compiledFiles)
                {
                    string const fillerName = filler.stem().string();
                    if (fillerName.substr(0, fillerName.size() - 6) == filled.stem().string())
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                    message += "\n " + string(filler.c_str());
            }
            ETH_ERROR_MESSAGE(message + "\n");
        }
    }

    bool checkFillerWhenFilterIsSetButNoTestsFilled = false;
    if (compiledFiles.size() == 0)
    {
        if (testNameFilter.empty())
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
            compiledFiles.push_back(fs::path(testNameFilter));  // put the test name as if it was compiled.
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
                           file.filename().string() + "'";
        else
            exceptionStr =
                "Compiled test folder contains test without Filler: " + file.filename().string();
        {
            TestInfo errorInfo("CheckFillers", file.stem().string());
            TestOutputHelper::get().setCurrentTestInfo(errorInfo);
        }
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
            if (!testNameFilter.empty())
                return testNameFilter + c_fillerPostf;
        }
        if (fs::exists(expectedFillerName2))
        {
            if (Options::get().filltests == false)
                checkFillerHash(file, expectedFillerName2);
            if (!testNameFilter.empty())
                return testNameFilter + c_fillerPostf;
        }
        if (fs::exists(expectedCopierName))
        {
            if (Options::get().filltests == false)
                checkFillerHash(file, expectedCopierName);
            if (!testNameFilter.empty())
                return testNameFilter + c_copierPostf;
        }
    }

    // No compiled test files. Filter is empty
    return testNameFilter;
}

void TestSuite::runAllTestsInFolder(string const& _testFolder) const
{
    Options::getDynamicOptions().getClientConfigs();
    if (ExitHandler::receivedExitSignal())
        return;

    // check that destination folder test files has according Filler file in src folder
    string filter;
    try
    {
        TestOutputHelper::get().setCurrentTestInfo(TestInfo("checkFillerExistance", _testFolder));
        filter = checkFillerExistance(_testFolder);
    }
    catch (std::exception const&)
    {
        TestOutputHelper::get().initTest(1);
        TestOutputHelper::get().finishTest();
        return;
    }

    // run all tests
    AbsoluteFillerPath fillerPath = getFullPathFiller(_testFolder);
    vector<fs::path> const files = test::getFiles(fillerPath.path(), {".json", ".yml"}, filter);
    if (files.size() == 0)
        ETH_WARNING(_testFolder + " no tests detected in folder!");

    // repeat this part for all connected clients
    auto thisPart = [this, &files, &_testFolder]() {
        auto& testOutput = test::TestOutputHelper::get();
        vector<thread> threadVector;

        // If debugging, already there is an open instance of a client.
        // Only one thread allowed to connect to it;
        size_t maxAllowedThreads = Options::get().threadCount;
        ClientConfig const& currConfig = Options::get().getDynamicOptions().getCurrentConfig();
        ClientConfgSocketType socType = currConfig.cfgFile().socketType();
        if (socType == ClientConfgSocketType::IPCDebug)
            maxAllowedThreads = 1;

        // If connecting to TCP sockets. Max threads are limited with tcp ports provided
        if (socType == ClientConfgSocketType::TCP)
        {
            maxAllowedThreads = min(maxAllowedThreads, currConfig.cfgFile().socketAdresses().size());
            if (maxAllowedThreads != Options::get().threadCount)
                ETH_WARNING(
                    "Correct -j option to `" + test::fto_string(maxAllowedThreads) + "` (or provide socket ports in config)!");
        }

        if (RPCSession::isRunningTooLong() || TestChecker::isTimeConsumingTest(_testFolder.c_str()))
            RPCSession::restartScripts(true);

        testOutput.initTest(files.size());
        for (auto const& file : files)
        {
            if (ExitHandler::receivedExitSignal())
                break;
            if (Options::get().lowcpu && TestChecker::isCPUIntenseTest(file.stem().string()))
            {
                ETH_WARNING("Skipping " + file.stem().string() + " because --lowcpu option was specified.\n");
                continue;
            }

            testOutput.showProgress();
            if (threadVector.size() == maxAllowedThreads)
                joinThreads(threadVector, false);

            if (ExitHandler::receivedExitSignal())
                break;
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
        std::cout << "Running tests for config '" << config.cfgFile().name() << "' "
                  << config.getId().id() << std::endl;

        // Run tests
        _func();

        // Disconnect threads from the client
        if (Options::getDynamicOptions().getClientConfigs().size() > 1)
            RPCSession::clear();
    }
}

std::mutex g_testPathMutex;
TestSuite::TestSuite()
{
    std::lock_guard<std::mutex> lock(g_testPathMutex);
    static bool runningTestsMessage = true;
    if (runningTestsMessage)
    {
        boost::filesystem::path const testPath = test::getTestPath();
        ETH_STDOUT_MESSAGE(string("Running tests using path: ") + testPath.c_str());
        runningTestsMessage = false;
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
    try
    {
        TestOutputHelper::get().setCurrentTestInfo(TestInfo("TestSuite::executeTest"));
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
            {
                string requireStr = " require: Filler.json/Filler.yml/Copier.json";
                ETH_FAIL_REQUIRE_MESSAGE(
                    false, "Incorrect file suffix in the filler folder! " + _testFileName.string() + requireStr);
            }
        }

        if (Options::get().logVerbosity >= 3)
        {
            size_t const threadID = std::hash<std::thread::id>()(TestOutputHelper::getThreadID());
            ETH_LOG("Running " + testname + ": " + "(" + test::fto_string(threadID) + ")", 3);
        }
        // Filename of the test that would be generated
        AbsoluteTestPath const boostTestPath =
            getFullPath(_testFolder).path() / fs::path(testname + ".json");

        bool wasErrors = false;
        TestSuiteOptions opt;
        if (Options::get().filltests)
        {
            TestFileData testData = readTestFile(_testFileName);
            if (isCopySource)
            {
                ETH_LOG("Copying " + _testFileName.string(), 0);
                ETH_LOG(" TO " + boostTestPath.path().string(), 0);
                assert(_testFileName.string() != boostTestPath.path().string());
                addClientInfo(testData.data, boostRelativeTestPath, testData.hash);
                writeFile(boostTestPath.path(), asBytes(testData.data.asJson()));
                ETH_FAIL_REQUIRE_MESSAGE(boost::filesystem::exists(boostTestPath.path().string()),
                    "Error when copying the test file!");
            }
            else
            {
                removeComments(testData.data);
                opt.doFilling = true;

                try
                {
                    DataObject output = doTests(testData.data, opt);
                    // Add client info for all of the tests in output
                    addClientInfo(output, boostRelativeTestPath, testData.hash);
                    writeFile(boostTestPath.path(), asBytes(output.asJson()));

                    if (!Options::get().getGStateTransactionFilter().empty())
                    {
                        ETH_WARNING("GState transaction filter is set. Disabling generated test run!");
                        opt.disableSecondRun = true;
                    }
                }
                catch (test::EthError const& _ex)
                {
                    // Something went wrong inside the test. skip the test.
                    // (error message is stored at TestOutputHelper. EthError is via ETH_ERROR_())
                    wasErrors = true;
                }
                catch (test::UpwardsException const& _ex)
                {
                    // UpwardsException is thrown upwards in tests for debug info
                    // And it should be catched on upper level for report till this point
                    ETH_ERROR_MESSAGE(string("Unhandled UpwardsException: ") + _ex.what());
                    wasErrors = true;
                }
                catch (std::exception const& _ex)
                {
                    // Low level error occured in tests
                    ETH_MARK_ERROR("ERROR OCCURED FILLING TESTS: " + string(_ex.what()));
                    RPCSession::sessionEnd(TestOutputHelper::getThreadID(), RPCSession::SessionStatus::HasFinished);
                    wasErrors = true;
                }
            }
        }

        if (!wasErrors && !opt.disableSecondRun)
        {
            try
            {
                TestOutputHelper::get().setCurrentTestFile(boostTestPath.path());
                executeFile(boostTestPath.path());
            }
            catch (test::EthError const& _ex)
            {
                // Something went wrong inside the test. skip the test.
                // (error message is stored at TestOutputHelper. EthError is via ETH_ERROR_())
            }
            catch (test::UpwardsException const& _ex)
            {
                // UpwardsException is thrown upwards in tests for debug info
                // And it should be catched on upper level for report till this point
                ETH_ERROR_MESSAGE(string("Unhandled UpwardsException: ") + _ex.what());
            }
            catch (std::exception const& _ex)
            {
                if (!ExitHandler::receivedExitSignal())
                    ETH_ERROR_MESSAGE("ERROR OCCURED RUNNING TESTS: " + string(_ex.what()));
                RPCSession::sessionEnd(TestOutputHelper::getThreadID(), RPCSession::SessionStatus::HasFinished);
            }
        }
        RPCSession::sessionEnd(TestOutputHelper::getThreadID(), RPCSession::SessionStatus::HasFinished);
    }
    catch (std::exception const& _ex)
    {
        // there was an error close thread
        RPCSession::sessionEnd(TestOutputHelper::getThreadID(), RPCSession::SessionStatus::HasFinished);
    }
}

void TestSuite::executeFile(boost::filesystem::path const& _file) const
{
    TestSuiteOptions opt;
    opt.isLegacyTests = Options::get().rCurrentTestSuite.find("LegacyTests") != string::npos;
    opt.isLegacyTests = opt.isLegacyTests || legacyTestSuiteFlag();

    if (_file.extension() != ".json")
        ETH_ERROR_MESSAGE("The generated test must have `.json` format!");

    ETH_LOG("Read json structure " + string(_file.filename().c_str()), 5);
    auto const res = test::readJsonData(_file);
    ETH_LOG("Read json finish", 5);
    doTests(res, opt);
}
}
