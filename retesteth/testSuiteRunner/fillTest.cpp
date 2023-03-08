#include "TestHelper.h"
#include "TestOutputHelper.h"
#include "TestSuiteHelperFunctions.h"
#include "testSuiteRunner/TestSuite.h"
#include <Options.h>
#include <libdevcore/CommonIO.h>
#include <retesteth/EthChecks.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/session/Session.h>
#include <retesteth/testStructures/Common.h>
#include <boost/filesystem.hpp>
#include <libdataobj/ConvertFile.h>

using namespace std;
using namespace dev;
using namespace test;
using namespace test::testsuite;
using namespace test::debug;
using namespace test::session;
namespace fs = boost::filesystem;

namespace
{
void checkFileIsFiller(fs::path const& _file)
{
    string fileName = _file.stem().c_str();
    if (fileName.find("Filler") == string::npos)
        ETH_ERROR_MESSAGE("Trying to fill `" + string(_file.c_str()) + "`, but file does not have Filler suffix!");
}

fs::path prepareOutputFileName(fs::path const& _file)
{
    string fileName = _file.stem().c_str();

    // output filename. substract Filler suffix
    fileName = fileName.substr(0, fileName.length() - 6) + ".json";

    fs::path outPath;
    if (Options::get().singleTestOutFile.initialized())
        outPath = fs::path(Options::get().singleTestOutFile);
    else
        outPath = _file.parent_path() / fileName;
    return outPath;
}

void updatePythonTestInfo(TestFileData& _testData, fs::path const& _pythonFiller, fs::path const& _filledFolder)
{
    // TODO double calling this function (getGeneratedTestNames)
    // can make a global map
    auto const testNames = getGeneratedTestNames(_pythonFiller);
    for (auto const& generatedTest : testNames)
    {
        fs::path const outputTestFilePath = _filledFolder / (generatedTest + ".json");
        string const res = dev::contentsString(outputTestFilePath);
        if (res.empty())
            ETH_ERROR_MESSAGE("Can't open expected python produced test: " + outputTestFilePath.string());
        spDataObject output = dataobject::ConvertJsoncppStringToData(res);
        bool update =
            addClientInfoIfUpdate(output.getContent(), _pythonFiller, _testData.hash, outputTestFilePath);
        if (update)
        {
            (*output).performModifier(mod_sortKeys, DataObject::ModifierOption::NONRECURSIVE);
            writeFile(outputTestFilePath, asBytes(output->asJson()));
        }
    }
}

}  // namespace

namespace test
{
string const c_fillerPostf = "Filler";
string const c_copierPostf = "Copier";
string const c_pythonPostf = ".py";

bool TestSuite::_fillPython(TestFileData& _testData, fs::path const& _fillerTestFilePath, AbsoluteFilledTestPath const& _filledPath) const
{
    bool wereErrors = false;
    auto const& currentConfig = Options::getCurrentConfig();
    auto const& specsScript = currentConfig.getPySpecsStartScript();
    if (fs::exists(specsScript))
    {
        string runcmd = specsScript.c_str();
        runcmd += " " + _fillerTestFilePath.parent_path().parent_path().string();
        runcmd += " " + _fillerTestFilePath.stem().string();
        runcmd += " " + _filledPath.path().parent_path().string();
        ETH_DC_MESSAGEC(DC::STATS, string("Generate Python test: ") + _fillerTestFilePath.stem().string(), LogColor::YELLOW);
        ETH_DC_MESSAGE(DC::RPC, string("Generate Python test: ") + runcmd);

        int exitcode;
        string out = test::executeCmd(runcmd, exitcode, ExecCMDWarning::NoWarningNoError);
        fs::remove_all(_fillerTestFilePath.parent_path() / "__pycache__");
        fs::remove_all(_filledPath.path().parent_path() / "__pycache__");
        ETH_DC_MESSAGE(DC::RPC, out);
        if (exitcode != 0)
        {
            wereErrors = true;
            ETH_ERROR_MESSAGE("Python spec failed filling the test: \n" + out);
            return wereErrors;
        }
        else
            updatePythonTestInfo(_testData, _fillerTestFilePath, _filledPath.path().parent_path());
        return wereErrors;
    }
    else
        return wereErrors;
}

void TestSuite::_fillCopier(
    TestFileData& _testData, fs::path const& _fillerTestFilePath, AbsoluteFilledTestPath const& _outputTestFilePath) const
{
    ETH_DC_MESSAGE(DC::TESTLOG, "Copying " + _fillerTestFilePath.string());
    ETH_DC_MESSAGE(DC::TESTLOG, " TO " + _outputTestFilePath.path().string());
    assert(_fillerTestFilePath.string() != _outputTestFilePath.path().string());
    addClientInfoIfUpdate(_testData.data.getContent(), _fillerTestFilePath, _testData.hash, _outputTestFilePath.path());
    writeFile(_outputTestFilePath.path(), asBytes(_testData.data->asJson()));
    ETH_FAIL_REQUIRE_MESSAGE(
        boost::filesystem::exists(_outputTestFilePath.path().string()), "Error when copying the test file!");
}

bool TestSuite::_fillJsonYml(TestFileData& _testData, fs::path const& _fillerTestFilePath,
    AbsoluteFilledTestPath const& _outputTestFilePath, TestSuite::TestSuiteOptions& _opt) const
{
    bool wereErrors = true;
    removeComments(_testData.data);
    try
    {
        spDataObject output = doTests(_testData.data, _opt);
        if (output->type() != DataType::Null)
        {
            bool update =
                addClientInfoIfUpdate(output.getContent(), _fillerTestFilePath, _testData.hash, _outputTestFilePath.path());
            if (update)
            {
                (*output).performModifier(mod_sortKeys, DataObject::ModifierOption::NONRECURSIVE);
                writeFile(_outputTestFilePath.path(), asBytes(output->asJson()));
            }
        }
        wereErrors = false;
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
        // Low level error occured in tests
        ETH_MARK_ERROR("ERROR OCCURED FILLING TESTS: " + string(_ex.what()));
        RPCSession::sessionEnd(TestOutputHelper::getThreadID(), RPCSession::SessionStatus::HasFinished);
    }
    return wereErrors;
}

bool TestSuite::_fillTest(TestSuite::TestSuiteOptions& _opt, fs::path const& _fillerTestFilePath,
    AbsoluteFilledTestPath const& _outputTestFilePath) const
{
    bool wereErrors = false;
    _opt.doFilling = true;
    TestOutputHelper::get().setCurrentTestFile(_fillerTestFilePath);

    // TODO we already read this file when checking filler hash updated
    // maybe store it in some map ?? but then a lot of tests in memory
    TestFileData testData = readTestFile(_fillerTestFilePath);

    bool const isPy = (_fillerTestFilePath.extension() == ".py");
    bool const isCopier = (_fillerTestFilePath.stem().string().rfind(c_copierPostf) != string::npos);
    if (isPy)
        return _fillPython(testData, _fillerTestFilePath, _outputTestFilePath);

    fs::path const testFillerPathRelative =
        _opt.calculateRelativeSrcPath ? fs::relative(_fillerTestFilePath, getTestPath()) : _fillerTestFilePath;
    if (isCopier)
    {
        _fillCopier(testData, testFillerPathRelative, _outputTestFilePath);
        wereErrors = false;
    }
    else
        wereErrors = _fillJsonYml(testData, testFillerPathRelative, _outputTestFilePath, _opt);
    return wereErrors;
}

void TestSuite::runTestWithoutFiller(boost::filesystem::path const& _file) const
{
    try
    {
        for (auto const& config : Options::getDynamicOptions().getClientConfigs())
        {
            Options::getDynamicOptions().setCurrentConfig(config);

            ETH_DC_MESSAGE(DC::STATS,
                "Running tests for config '" + config.cfgFile().name() + "' " + test::fto_string(config.getId().id()));
            ETH_DC_MESSAGE(DC::TESTLOG, "Running " + _file.filename().string() + ": ");

            // Allow to execute a custom test .json file on any test suite
            auto& testOutput = test::TestOutputHelper::get();
            testOutput.initTest(1);

            if (Options::get().filltests)
            {
                checkFileIsFiller(_file);
                fs::path outPath = prepareOutputFileName(_file);
                TestSuite::TestSuiteOptions _opt;
                _opt.allowInvalidBlocks = true;
                _opt.calculateRelativeSrcPath = false;
                _fillTest(_opt, _file, outPath);
            }
            else
                _runTest(_file);

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

}  // namespace test
