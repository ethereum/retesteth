#pragma once
#include "../../../basetypes.h"
#include <libdataobj/DataObject.h>

namespace test::teststruct
{

// Base logic for State Test Env section
struct StateTestEnvBase : GCP_SPointerBase
{
    FH20 const& currentCoinbase() const { return m_currentCoinbase; }
    VALUE const& currentGasLimit() const { return m_currentGasLimit; }
    VALUE const& currentNumber() const { return m_currentNumber; }

    // In StateTests timestamp for genesis block should be overrided with 0x00
    // In BlockchainTests currentTimestamp is m_currentTimestamp constructed from genesis blockHeader
    virtual VALUE const& currentTimestamp() const { return m_currentTimestamp; }

    BYTES const& currentExtraData() const { return m_currentExtraData; }
    FH8 const& currentNonce() const { return m_currentNonce; }
    FH32 const& currentMixHash() const { return m_currentMixHash; }
    FH32 const& previousHash() const { return m_previousHash; }
    virtual spDataObject const& asDataObject() const;
    virtual ~StateTestEnvBase() {}

    VALUE const& currentBaseFee() const { return m_currentBaseFee; }
    FH32 const& currentRandom() const { return m_currentRandom; }
    VALUE const& currentDifficulty() const { return m_currentDifficulty; }

    FH32 const& currentWithdrawalsRoot() const { return m_currentWithdrawalsRoot; }
    VALUE const& currentExcessBlobGas() const { return m_currentExcessBlobGas; }
    VALUE const& currentBlobGasUsed() const { return m_currentBlobGasUsed; }
    FH32 const& currentBeaconRoot() const { return m_currentBeaconRoot; }

protected:
    StateTestEnvBase() {}
    spDataObject m_raw;

    spFH20 m_currentCoinbase;
    spVALUE m_currentGasLimit;
    spVALUE m_currentNumber;
    spVALUE m_currentTimestamp;
    spFH32 m_previousHash;
    spBYTES m_currentExtraData;
    spFH8 m_currentNonce;
    spFH32 m_currentMixHash;

    // 1559
    spVALUE m_currentDifficulty;
    spVALUE m_currentBaseFee;

    // Merge
    spFH32 m_currentRandom;

    // Shaghai
    spFH32 m_currentWithdrawalsRoot;

    // 4844
    spVALUE m_currentBlobGasUsed;
    spVALUE m_currentExcessBlobGas;
    spFH32 m_currentBeaconRoot;
};


}  // namespace teststruct
