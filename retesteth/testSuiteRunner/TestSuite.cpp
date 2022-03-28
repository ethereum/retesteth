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

#include "TestSuiteHelperFunctions.h"
#include <dataObject/DataObject.h>
#include <libdevcore/CommonIO.h>
#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/session/Session.h>
#include <retesteth/session/ThreadManager.h>
#include <retesteth/testSuiteRunner/TestSuite.h>
#include <retesteth/testSuites/TestFixtures.h>
#include <boost/test/unit_test.hpp>
#include <string>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::testsuite;

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
                    spDataObject output = doTests(testData.data, opt);
                    addClientInfo(output.getContent(), _file, testData.hash, outPath);
                    (*output).performModifier(mod_sortKeys, DataObject::ModifierOption::NONRECURSIVE);
                    writeFile(outPath, asBytes(output->asJson()));
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
    {
        TestOutputHelper::get().currentTestRunPP();
        ETH_WARNING(_testFolder + " no tests detected in folder!");
    }


    // repeat this part for all connected clients
    auto thisPart = [this, &files, &_testFolder]() {
        auto& testOutput = test::TestOutputHelper::get();

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
            if (ExitHandler::receivedExitSignal())
                break;

            auto job = [this, &_testFolder, &file]() { executeTest(_testFolder, file); };
            ThreadManager::addTask(job);
        }
        ThreadManager::joinThreads();
        testOutput.finishTest();
    };
    runFunctionForAllClients(thisPart);
}


void TestSuite::runFunctionForAllClients(std::function<void()> _func)
{
    for (auto const& config : Options::getDynamicOptions().getClientConfigs())
    {
        Options::getDynamicOptions().setCurrentConfig(config);
        std::cout << "Running tests for config '" << config.cfgFile().name() << "' " << config.getId().id() << std::endl;

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
    return TestSuite::AbsoluteFillerPath(test::getTestPath() / suiteFillerFolder().path() / _testFolder);
}

TestSuite::AbsoluteFilledTestPath TestSuite::getFullPathFilled(string const& _testFolder) const
{
    return TestSuite::AbsoluteFilledTestPath(test::getTestPath() / suiteFolder().path() / _testFolder);
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
        AbsoluteFilledTestPath const boostTestPath = getFullPathFilled(_testFolder).path() / fs::path(testname + ".json");

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
                addClientInfo(testData.data.getContent(), boostRelativeTestPath, testData.hash, boostTestPath.path());
                writeFile(boostTestPath.path(), asBytes(testData.data->asJson()));
                ETH_FAIL_REQUIRE_MESSAGE(
                    boost::filesystem::exists(boostTestPath.path().string()), "Error when copying the test file!");
            }
            else
            {
                removeComments(testData.data);
                opt.doFilling = true;

                try
                {
                    spDataObject output = doTests(testData.data, opt);

                    bool update =
                        addClientInfo(output.getContent(), boostRelativeTestPath, testData.hash, boostTestPath.path());
                    if (update)
                    {
                        (*output).performModifier(mod_sortKeys, DataObject::ModifierOption::NONRECURSIVE);
                        writeFile(boostTestPath.path(), asBytes(output->asJson()));
                    }

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
    static bool isLegacy = Options::get().rCurrentTestSuite.find("LegacyTests") != string::npos;
    opt.isLegacyTests = isLegacy || legacyTestSuiteFlag();

    if (_file.extension() != ".json")
        ETH_ERROR_MESSAGE("The generated test must have `.json` format!");

    ETH_LOG("Read json structure " + string(_file.filename().c_str()), 5);
    spDataObject res = test::readJsonData(_file);
    ETH_LOG("Read json finish", 5);
    doTests(res, opt);
}


}  // namespace test
