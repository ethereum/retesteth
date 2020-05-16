#pragma once
#include "../../Common.h"
#include "StateTestFillerExpectSection.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>

using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
struct StateTestFillerEnv : GCP_SPointerBase
{
    StateTestFillerEnv(DataObject const& _data);
    FH20 const& currentCoinbase() const { return m_currentCoinbase; }
    VALUE const& currentDifficulty() const { return m_currentDifficulty; }
    VALUE const& currentGasLimit() const { return m_currentGasLimit; }
    VALUE const& currentNumber() const { return m_currentNumber; }
    VALUE const& currentTimestamp() const { return m_currentTimestamp; }
    FH32 const& previousHash() const { return m_previousHash; }

private:
    FH20 m_currentCoinbase;
    VALUE m_currentDifficulty;
    VALUE m_currentGasLimit;
    VALUE m_currentNumber;
    VALUE m_currentTimestamp;
    FH32 m_previousHash;
};

struct StateTestInFiller : GCP_SPointerBase
{
    StateTestInFiller(DataObject const&);

    StateTestFillerEnv const& Env() const { return *m_env.getPointer(); }
    State const& Pre() const { return *m_pre.getPointer(); }
    StateTestFillerTransaction const& GTr() const { return *m_transaction.getPointer(); }
    std::vector<StateTestFillerExpectSection> const& Expects() const { return m_expectSections; }

private:
    GCP_SPointer<StateTestFillerEnv> m_env;
    GCP_SPointer<State> m_pre;
    GCP_SPointer<StateTestFillerTransaction> m_transaction;
    std::vector<StateTestFillerExpectSection> m_expectSections;
};

struct GeneralStateTestFiller
{
    GeneralStateTestFiller(DataObject const&);
    StateTestInFiller const& test() const { return *m_test.getPointer(); }

private:
    GCP_SPointer<StateTestInFiller> m_test;
};


}  // namespace teststruct
}  // namespace test
