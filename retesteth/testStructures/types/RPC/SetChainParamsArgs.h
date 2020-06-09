#pragma once
#include "../Ethereum/BlockHeader.h"
#include "../Ethereum/State.h"
#include <retesteth/dataObject/DataObject.h>

using namespace dataobject;

namespace test
{
namespace teststruct
{
struct SetChainParamsArgs
{
    SetChainParamsArgs(DataObject const& _data);
    DataObject asDataObject() const;
    BlockHeader const& genesis() const { return m_genesis.getCContent(); }
    State const& state() const { return m_preState.getCContent(); }
    SealEngine sealEngine() const { return m_sealEngine; }
    FORK fork() const { return FORK(m_params.atKey("fork").asString()); }

private:
    SetChainParamsArgs() {}
    spState m_preState;
    SealEngine m_sealEngine;
    spBlockHeader m_genesis;
    DataObject m_params;
};

}  // namespace teststruct
}  // namespace test
