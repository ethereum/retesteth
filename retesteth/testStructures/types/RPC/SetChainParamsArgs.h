#pragma once
#include "../Ethereum/BlockHeader.h"
#include "../Ethereum/BlockHeader1559.h"
#include "../Ethereum/State.h"
#include <retesteth/dataObject/DataObject.h>

using namespace dataobject;

namespace test
{
namespace teststruct
{
struct SetChainParamsArgs : GCP_SPointerBase
{
    SetChainParamsArgs(spDataObject& _data);

    spDataObject asDataObject() const;
    spBlockHeader const& genesis() const { return m_genesis; }
    spState const& state() const { return m_preState; }
    SealEngine sealEngine() const { return m_sealEngine; }
    DataObject const& params() const { return m_params; }

protected:
    SetChainParamsArgs() {}
    spState m_preState;
    SealEngine m_sealEngine;
    spBlockHeader m_genesis;
    spDataObject m_params;
};

typedef GCP_SPointer<SetChainParamsArgs> spSetChainParamsArgs;

}  // namespace teststruct
}  // namespace test
