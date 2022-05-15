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
enum class TestEnvClass
{
    LEGACY,
    EIP1559,
    MERGE,
    UNDEFINED
};

// Base logic for State Test Env section
struct StateTestEnvBase : GCP_SPointerBase
{
    virtual TestEnvClass type() const { return TestEnvClass::UNDEFINED; }

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
    spDataObject const& asDataObject() const;
    virtual ~StateTestEnvBase() {}

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
};

struct StateTestEnvBaseMerge
{
    spVALUE const& currentBaseFee() const { return m_currentBaseFee; }
    spFH32 const& currentRandom() const { return m_currentRandom; }
    spVALUE const& currentDifficultyForOther() const { return m_currentDifficultyForOtherNets; }
    static StateTestEnvBaseMerge const* castFrom(StateTestEnvBase const* _from);

protected:
    virtual void initializeMergeFields(DataObject const&) = 0;
    spVALUE m_currentDifficultyForOtherNets;
    spVALUE m_currentBaseFee;
    spFH32 m_currentRandom;
};

struct StateTestEnvBase1559
{
    spVALUE const& currentBaseFee() const { return m_currentBaseFee; }
    VALUE const& currentDifficulty() const { return m_currentDifficulty; }
    static StateTestEnvBase1559 const* castFrom(StateTestEnvBase const* _from);

protected:
    virtual void initialize1559Fields(DataObject const&) = 0;
    spVALUE m_currentBaseFee;
    spVALUE m_currentDifficulty;
};

struct StateTestEnvBaseLegacy
{
    VALUE const& currentDifficulty() const { return m_currentDifficulty; }
    static StateTestEnvBaseLegacy const* castFrom(StateTestEnvBase const* _from);

protected:
    virtual void initializeLegacyFields(DataObject const&) = 0;
    spVALUE m_currentDifficulty;
};


}  // namespace teststruct
}  // namespace test
