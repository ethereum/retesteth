#pragma once
#include "../Ethereum/Blocks/BlockHeader.h"
#include "../Ethereum/Blocks/BlockHeader1559.h"
#include "../Ethereum/State.h"
#include <libdataobj/DataObject.h>

namespace test::teststruct
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

struct SetChainParamsArgsGenesis : GCP_SPointerBase
{
    SetChainParamsArgsGenesis(DataObject const&);
    spBlockHeader constructBlockHeader() const;
    virtual ~SetChainParamsArgsGenesis() {}

protected:
    SetChainParamsArgsGenesis(DataObject const& _data, bool) : m_dataRef(_data){}
    DataObject const& m_dataRef;
    spDataObject buildCommonBlockHeader() const;
    virtual spDataObject _constructBlockHeader() const;
};
struct SetChainParamsArgsGenesis1559 : SetChainParamsArgsGenesis
{
    SetChainParamsArgsGenesis1559(DataObject const&);
protected:
    virtual spDataObject _constructBlockHeader() const override;
};
struct SetChainParamsArgsGenesisMerge : SetChainParamsArgsGenesis
{
    SetChainParamsArgsGenesisMerge(DataObject const&);
protected:
    virtual spDataObject _constructBlockHeader() const override;
};
struct SetChainParamsArgsGenesisShanghai : SetChainParamsArgsGenesis
{
    SetChainParamsArgsGenesisShanghai(DataObject const&);
protected:
    virtual spDataObject _constructBlockHeader() const override;
};
struct SetChainParamsArgsGenesis4844 : SetChainParamsArgsGenesis
{
    SetChainParamsArgsGenesis4844(DataObject const&);
protected:
    virtual spDataObject _constructBlockHeader() const override;
};

typedef GCP_SPointer<SetChainParamsArgs> spSetChainParamsArgs;
typedef GCP_SPointer<SetChainParamsArgsGenesis> spSetChainParamsArgsGenesis;

}  // namespace teststruct
