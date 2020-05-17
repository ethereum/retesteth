#pragma once
#include "../../../basetypes/BYTES.h"
#include "../../../basetypes/FH20.h"
#include "../../../basetypes/FH32.h"
#include "../../../basetypes/VALUE.h"
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
    FH20 const& currentCoinbase() const { return *m_currentCoinbase.getCPtr(); }
    VALUE const& currentDifficulty() const { return *m_currentDifficulty.getCPtr(); }
    VALUE const& currentGasLimit() const { return *m_currentGasLimit.getCPtr(); }
    VALUE const& currentNumber() const { return *m_currentNumber.getCPtr(); }
    VALUE const& currentTimestamp() const { return *m_currentTimestamp.getCPtr(); }
    FH32 const& previousHash() const { return *m_previousHash.getCPtr(); }
    DataObject asDataObject() const;

private:
    StateTestFillerEnv() {}
    GCP_SPointer<FH20> m_currentCoinbase;
    GCP_SPointer<VALUE> m_currentDifficulty;
    GCP_SPointer<VALUE> m_currentGasLimit;
    GCP_SPointer<VALUE> m_currentNumber;
    GCP_SPointer<VALUE> m_currentTimestamp;
    GCP_SPointer<FH32> m_previousHash;
};


}  // namespace teststruct
}  // namespace test
