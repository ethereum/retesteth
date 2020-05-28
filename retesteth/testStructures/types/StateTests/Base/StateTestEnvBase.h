#pragma once
#include "../../../basetypes.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>

using namespace dataobject;
using namespace test::teststruct;
namespace test
{
namespace teststruct
{
// Base logic for State Test Env section
struct StateTestEnvBase : GCP_SPointerBase
{
    FH20 const& currentCoinbase() const { return m_currentCoinbase.getCContent(); }
    VALUE const& currentDifficulty() const { return m_currentDifficulty.getCContent(); }
    VALUE const& currentGasLimit() const { return m_currentGasLimit.getCContent(); }
    VALUE const& currentNumber() const { return m_currentNumber.getCContent(); }

    // In StateTests timestamp for genesis block should be overrided with 0x00
    virtual VALUE const& currentTimestamp() const { return m_currentTimestamp.getCContent(); }

    BYTES const& currentExtraData() const { return m_currentExtraData.getCContent(); }
    FH8 const& currentNonce() const { return m_currentNonce.getCContent(); }
    FH32 const& currentMixHash() const { return m_currentMixHash.getCContent(); }
    FH32 const& previousHash() const { return m_previousHash.getCContent(); }
    DataObject const asDataObject() const;

    virtual ~StateTestEnvBase() {}

protected:
    StateTestEnvBase() {}
    spFH20 m_currentCoinbase;
    spVALUE m_currentDifficulty;
    spVALUE m_currentGasLimit;
    spVALUE m_currentNumber;
    spVALUE m_currentTimestamp;
    spFH32 m_previousHash;
    spBYTES m_currentExtraData;
    spFH8 m_currentNonce;
    spFH32 m_currentMixHash;
};


}  // namespace teststruct
}  // namespace test
