#include "VMTestFiller.h"
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/testStructures/Common.h>
#include "../StateTests/Filler/StateTestFillerTransaction.h"

using namespace test::teststruct;
namespace  {
    spDataObject translateExecToTransaction(DataObject const& _exec)
    {
        spDataObject _gtransaction(new DataObject());
        DataObject& gtransaction = _gtransaction.getContent();
        requireJsonFields(_exec, "vmTestFiller exec",
            {{"address", {{DataType::String}, jsonField::Required}},
                {"caller", {{DataType::String}, jsonField::Required}},
                {"data", {{DataType::String}, jsonField::Required}},
                {"code", {{DataType::String}, jsonField::Optional}},
                {"gas", {{DataType::String}, jsonField::Required}},
                {"gasPrice", {{DataType::String}, jsonField::Required}},
                {"origin", {{DataType::String}, jsonField::Required}},
                {"value", {{DataType::String}, jsonField::Required}}});
        gtransaction["data"].addArrayObject(spDataObject(new DataObject(_exec.atKey("data").asString())));
        gtransaction["gasLimit"].addArrayObject(spDataObject(new DataObject(_exec.atKey("gas").asString())));
        gtransaction["gasPrice"] = _exec.atKey("gasPrice").asString();
        gtransaction["nonce"] = "0";
        gtransaction["secretKey"] = "45a915e4d060149eb4365960e6a7a45f334393093061116b197e3240065ff2d8";
        gtransaction["to"] = _exec.atKey("address").asString();
        gtransaction["value"].addArrayObject(spDataObject(new DataObject(_exec.atKey("value").asString())));
        return _gtransaction;
    }
}
VMTestFiller::VMTestFiller(spDataObject& _data)
{
    try
    {
        ETH_ERROR_REQUIRE_MESSAGE(_data->type() == DataType::Object,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain an object value (json/yaml).");
        ETH_ERROR_REQUIRE_MESSAGE(_data->getSubObjects().size() == 1,
            TestOutputHelper::get().get().testFile().string() + " A test file must contain exactly one test!");
        for (auto& el : _data.getContent().getSubObjectsUnsafe())
        {
            TestOutputHelper::get().setCurrentTestInfo(TestInfo("VMTestFiller", el->getKey()));
            m_tests.push_back(VMTestInFiller(el));
        }
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}

VMTestInFiller::VMTestInFiller(spDataObject& _data)
{
    try
    {
        requireJsonFields(_data, "vmTestFiller " + _data->getKey(),
            {{"_info", {{DataType::Object}, jsonField::Optional}},
                {"env", {{DataType::Object}, jsonField::Required}},
                {"pre", {{DataType::Object}, jsonField::Required}},
                {"exec", {{DataType::Object}, jsonField::Required}},
                {"expect", {{DataType::Object}, jsonField::Optional}},
                {"expectOut", {{DataType::String}, jsonField::Optional}}});
        if (_data->count("expectOut"))
            ETH_WARNING("Unable to verify `expectOut` when creating a stateTest from VMTest " +
                        TestOutputHelper::get().testInfo().errorDebug());

        if (_data->count("_info"))
            m_info = GCP_SPointer<InfoIncomplete>(new InfoIncomplete(dataobject::move((*_data).atKeyPointerUnsafe("_info"))));
        m_env = GCP_SPointer<StateTestFillerEnv>(new StateTestFillerEnv(_data->atKey("env")));
        convertDecStateToHex((*_data).atKeyPointerUnsafe("pre"));
        m_pre = spState(new State(MOVE(_data, "pre")));
        m_name = _data->getKey();

        StateTestFillerTransaction stateTx(translateExecToTransaction(_data->atKey("exec")));
        m_transaction = spTransaction(new TransactionLegacy(stateTx.buildTransactions().at(0).transaction()->asDataObject()));
        if (_data->count("expect"))
        {
            convertDecStateToHex((*_data).atKeyPointerUnsafe("expect"), solContracts(), StateToHex::NOCOMPILECODE);
            m_expect = spStateIncomplete(new StateIncomplete(MOVE(_data, "expect")));
        }
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("VMTestFiller convertion error: ") + _ex.what());
    }
}

