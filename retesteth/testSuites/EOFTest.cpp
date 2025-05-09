#include "EOFTest.h"
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/testStructures/types/EOFTests/EOFTestFiller.h>
#include <retesteth/testStructures/types/EOFTests/EOFTest.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/Options.h>
#include <retesteth/session/SessionInterface.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/ExitHandler.h>

using namespace std;
using namespace test;
using namespace test::session;
using namespace test::teststruct;
using namespace dataobject;
namespace fs = boost::filesystem;

spDataObject FillTest(EOFTestInFiller const& _test)
{
    spDataObject filledTest;
    TestOutputHelper::get().setCurrentTestName(_test.testName());
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());
    if (_test.hasInfo())
        (*filledTest).atKeyPointer("_info") = _test.Info().rawData();
    (*filledTest).atKeyPointer("vectors") = sDataObject(DataType::Object);

    size_t dataInd = 0;
    for (auto const& testVector : _test.Vectors())
    {
        spDataObject vectorOut;
        (*vectorOut).setKey(_test.testName() + "_" + test::fto_string(dataInd));
        (*vectorOut)["code"] = testVector.data().asString();
        (*vectorOut).atKeyPointer("results") = sDataObject(DataType::Object);
        for(auto const& fork : _test.Forks())
        {
            if (networkSkip(fork, _test.testName()))
                continue;

            spDataObject forkResult;
            (*forkResult).setKey(fork.asString());
            auto const& dataOpt = Options::get().trData;
            if (dataOpt.initialized() && dataOpt.index != -1 && (size_t)dataOpt.index != dataInd)
                continue;

            TestInfo errorInfo(fork.asString(), (int)dataInd, -1, -1);
            errorInfo.setTrDataDebug(testVector.dataPreview());
            TestOutputHelper::get().setCurrentTestInfo(errorInfo);

            string res = session.test_rawEOFCode(testVector.data(), fork);
            string const& testException = testVector.getExpectException(fork);
            compareEOFException(testVector.data(), res, testException);
            if (testException.empty())
                (*forkResult)["result"].setBool(true);
            else
            {
                (*forkResult)["result"].setBool(false);
                (*forkResult)["exception"] = testException;
            }

            (*vectorOut)["results"].addSubObject(forkResult);
        }
        (*filledTest)["vectors"].addSubObject(vectorOut);
        dataInd++;
    }
    return filledTest;
}

size_t RunTest(EOFTestInFilled const& _test)
{
    if (ExitHandler::receivedExitSignal())
        return 0;

    TestOutputHelper::get().setCurrentTestName(_test.testName());
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    size_t executedVectors = 0;
    for (auto const& [vectorName, vec] : _test.getVectors())
    {
        auto const& singletest = Options::get().singletest;
        if (singletest.initialized() && vectorName != singletest.subname && !singletest.subname.empty())
            continue;

        for (auto const& [fork, exception] : vec.getResultForks())
        {
            if (networkSkip(fork, _test.testName()))
                continue;

            ETH_DC_MESSAGE(debug::DC::STATS2, "Running vector: " + vectorName);
            TestOutputHelper::get().setCurrentTestInfo(TestInfo(vectorName + "/" + fork.asString()));
            string const res = session.test_rawEOFCode(vec.data(), fork);
            compareEOFException(vec.data(), res, exception);
            executedVectors++;
        }
    }
    return executedVectors;
}

spDataObject EOFTestSuite::doTests(spDataObject& _input, TestSuiteOptions& _opt) const
{
    TestOutputHelper::get().setCurrentTestInfo(TestInfo("EOFSuite::doTests init"));
    if (_opt.doFilling)
    {
        spDataObject filledTest;
        EOFTestFiller filler(_input);
        EOFTestInFiller const& test = filler.tests().at(0);
        checkTestNameIsEqualToFileName(test.testName());

        auto const filled = FillTest(test);
        if (filled->type() != DataType::Null)
            (*filledTest).addSubObject(test.testName(), filled);
        else
            return filled;

        TestOutputHelper::get().registerTestRunSuccess();
        return filledTest;
    }
    else
    {
        TestOutputHelper::get().setCurrentTestInfo(TestInfo("Parsing EOF test"));
        TestOutputHelper::get().setCurrentTestName("----");
        EOFTest filledTest(_input);

        // Just check the test structure if running with --checkhash
        if (Options::get().checkhash)
            return spDataObject();

        for (auto const& test : filledTest.tests())
        {
            size_t executedVectors = RunTest(test);
            if (executedVectors != 0)
                TestOutputHelper::get().registerTestRunSuccess();
        }
    }
    return spDataObject(0);
}
