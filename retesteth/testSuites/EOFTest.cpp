#include "EOFTest.h"
#include <retesteth/TestOutputHelper.h>
#include <retesteth/testStructures/types/EOFTests/EOFTestFiller.h>
#include <retesteth/testStructures/types/EOFTests/EOFTest.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/Options.h>
#include <retesteth/session/SessionInterface.h>
#include <retesteth/TestOutputHelper.h>
#include <retesteth/TestHelper.h>
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
            spDataObject forkResult;
            (*forkResult).setKey(fork.asString());
            auto const& dataOpt = Options::get().trData;
            if (dataOpt.initialized() && dataOpt.index != -1 && (size_t)dataOpt.index != dataInd)
                continue;

            TestInfo errorInfo(fork.asString(), (int)dataInd, -1, -1);
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

void RunTest(EOFTestInFilled const& _test)
{
    if (ExitHandler::receivedExitSignal())
        return;

    TestOutputHelper::get().setCurrentTestName(_test.testName());
    SessionInterface& session = RPCSession::instance(TestOutputHelper::getThreadID());

    for (auto const& [vectorName, vec] : _test.getVectors())
    {
        for (auto const& [fork, exception] : vec.getResultForks())
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo(vectorName + "/" + fork.asString()));
            string const res = session.test_rawEOFCode(vec.data(), fork);
            compareEOFException(vec.data(), res, exception);
        }
    }
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
            RunTest(test);
            TestOutputHelper::get().registerTestRunSuccess();
        }
    }
    return spDataObject(0);
}
