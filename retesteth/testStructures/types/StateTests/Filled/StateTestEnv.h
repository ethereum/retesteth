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
struct StateTestEnv : GCP_SPointerBase
{
    StateTestEnv(DataObject const& _data);
    FH20 const& currentCoinbase() const { return m_currentCoinbase.getCContent(); }
    VALUE const& currentDifficulty() const { return m_currentDifficulty.getCContent(); }
    VALUE const& currentGasLimit() const { return m_currentGasLimit.getCContent(); }
    VALUE const& currentNumber() const { return m_currentNumber.getCContent(); }
    VALUE const& currentTimestamp() const { return m_currentTimestamp.getCContent(); }
    FH32 const& previousHash() const { return m_previousHash.getCContent(); }
    DataObject asDataObject() const;

private:
    StateTestEnv() {}
    spFH20 m_currentCoinbase;
    spVALUE m_currentDifficulty;
    spVALUE m_currentGasLimit;
    spVALUE m_currentNumber;
    spVALUE m_currentTimestamp;
    spFH32 m_previousHash;
};


}  // namespace teststruct
}  // namespace test
