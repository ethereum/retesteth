#include "StateTestsHelper.h"
#include "StateTestFillerRunnerEEST.h"
#include <retesteth/session/Session.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/testSuites/Common.h>
#include <retesteth/testStructures/PrepareChainParams.h>
#include <retesteth/ExitHandler.h>
#include <retesteth/Options.h>
#include <libdevcore/CommonIO.h>
using namespace std;
using namespace test;
using namespace test::debug;
using namespace test::teststruct;
using namespace test::session;
namespace fs = boost::filesystem;

namespace test::statetests
{

StateTestFillerRunnerEEST::StateTestFillerRunnerEEST(StateTestInFiller const& _test, TestSuite::TestSuiteOptions const& _opt)
  : StateTestFillerRunner(_test, _opt)
{
    m_finalFilled_test = sDataObject(DataType::Object);
}

void StateTestFillerRunnerEEST::performTransactionOnExpect(TransactionInGeneralSection& _tx, StateTestFillerExpectSection const& _expect, FORK const& _fork)
{
    StateTestFillerRunner::performTransactionOnExpect(_tx, _expect, _fork);
    constructTestVector(_fork, _tx);
}

spDataObject StateTestFillerRunnerEEST::getFilledTest() const
{
    return m_finalFilled_test;
}

void StateTestFillerRunnerEEST::constructTestVector(FORK const& _network, TransactionInGeneralSection& _tx)
{
    spDataObject m_eestTestVector = sDataObject(DataType::Object);

    string testName = m_testSuiteOpt.relativePathToFilledTest.string();
    testName += "::" + m_test.testName() + "-fork_" + _network.asString();
    testName += "-d" + _tx.dataIndS() + "g" + _tx.gasIndS() + "v" + _tx.valueIndS();
    // <relative-path-of-fixture-json>::<fixture-name>-fork_<fork-name>-d<X>g<Y>v<Z>

    spDataObject info = sDataObject(DataType::Object);
    if (m_test.hasInfo())
        (*info)["comment"] = m_test.Info().rawData()->atKey("comment").asString();
    if (!_tx.transaction()->dataLabel().empty())
    {
        static string const removePrefix = ":label ";
        string label = _tx.transaction()->dataLabel();
        size_t const pos = label.find(removePrefix);
        if (pos != string::npos)
            label.erase(pos, removePrefix.length());
        (*info)["labels"]["0"] = label;
    }
    (*m_eestTestVector).atKeyPointer("_info") = info;

    (*m_eestTestVector).atKeyPointer("env") = m_test.Env().asDataObject();
    (*m_eestTestVector).atKeyPointer("pre") = m_test.Pre().asDataObject();
    if (m_test.hasConfig())
        (*m_eestTestVector).atKeyPointer("config") = m_test.Config().asDataObject(_network);

    spDataObject data = m_test.GeneralTr().asDataObject(_tx.dataInd(), _tx.gasInd(), _tx.valueInd());
    (*m_eestTestVector).atKeyPointer("transaction") = data;

    // Fill post
    spDataObject postRecord = sDataObject(DataType::Object);
    spDataObject forkRecord = sDataObject(DataType::Array);

    spDataObject postData = m_forkResults->getSubObjects().at(m_forkResults->getSubObjects().size() - 1);

    // Test vector have only 1 transaction, overwrite indexes
    spDataObject indexes;
    (*indexes).setKey("indexes");
    (*indexes)["data"] = 0;
    (*indexes)["gas"] = 0;
    (*indexes)["value"] = 0;
    (*postData).atKeyPointer("indexes") = indexes;

    (*forkRecord).addArrayObject(postData);

    (*postRecord).atKeyPointer(_network.asString()) = forkRecord;
    (*m_eestTestVector).atKeyPointer("post") =  postRecord;

    for (auto const& ex : m_test.unitTestExceptions())
        (*m_eestTestVector)["exceptions"].addArrayObject(spDataObject(new DataObject(ex)));

    m_finalFilled_test.getContent().atKeyPointer(testName) = m_eestTestVector;
}
}
