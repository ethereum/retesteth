#include "GeneralStateTestsFiller.h"
#include "../../Common.h"
#include <retesteth/EthChecks.h>

using namespace test::teststruct;
GeneralStateTestFiller::GeneralStateTestFiller(DataObject const& _data)
{
    checkOnlyOneTest(_data);
    m_test = GCP_SPointer<StateTestInFiller>(new StateTestInFiller(_data.at(0)));
}

StateTestInFiller::StateTestInFiller(DataObject const& _data)
{
    (void)_data;
    try
    {
        m_env = GCP_SPointer<StateTestFillerEnv>(new StateTestFillerEnv(_data.atKey("env")));
        m_pre = GCP_SPointer<State>(new State(_data.atKey("pre")));
        m_transaction = GCP_SPointer<StateTestFillerTransaction>(
            new StateTestFillerTransaction(_data.atKey("transaction")));
        for (auto const& el : _data.atKey("expect").getSubObjects())
            m_expectSections.push_back(StateTestFillerExpectSection(el));
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}

StateTestFillerEnv::StateTestFillerEnv(DataObject const& _data)
{
    try
    {
        m_currentCoinbase = FH20(_data.atKey("currentCoinbase"));
        m_currentDifficulty = VALUE(_data.atKey("currentDifficulty"));
        m_currentGasLimit = VALUE(_data.atKey("currentGasLimit"), dev::u256("0x7fffffffffffffff"));
        m_currentNumber = VALUE(_data.atKey("currentNumber"));
        m_currentTimestamp = VALUE(_data.atKey("currentTimestamp"));
        ;
        m_previousHash = FH32(_data.atKey("previousHash"));
    }
    catch (DataObjectException const& _ex)
    {
        ETH_ERROR_MESSAGE(_ex.what());
    }
}
