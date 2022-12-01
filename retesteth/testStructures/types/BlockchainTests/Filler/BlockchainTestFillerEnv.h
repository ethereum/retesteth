#pragma once
#include <retesteth/testStructures/types/StateTests/Base/StateTestEnvBase.h>
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{

// Blockchain test does not have Env section
// This represent env section consructed from blockchain test genesis header
// Filler marks that fields are converted from dec to hex explicitly
struct BlockchainTestFillerEnv : StateTestEnvBase
{
protected:
    void initializeCommonFields(spDataObject const&, SealEngine);
private:
    virtual void define() const = 0; //BlockchainTestFillerEnv is abstract

};

struct BlockchainTestFillerEnvMerge : BlockchainTestFillerEnv
{
    BlockchainTestFillerEnvMerge(spDataObjectMove _data, SealEngine _sEngine);

protected:
    void initializeMergeFields(DataObject const&);
private:
    void define() const override {}
};

struct BlockchainTestFillerEnv1559 : BlockchainTestFillerEnv
{
    BlockchainTestFillerEnv1559(spDataObjectMove _data, SealEngine _sEngine);

protected:
    void initialize1559Fields(DataObject const&);
private:
    void define() const override {}

};

struct BlockchainTestFillerEnvLegacy : BlockchainTestFillerEnv
{
    BlockchainTestFillerEnvLegacy(spDataObjectMove _data, SealEngine _sEngine);

protected:
    void initializeLegacyFields(DataObject const&);
private:
    void define() const override {}

};


typedef GCP_SPointer<BlockchainTestFillerEnv> spBlockchainTestFillerEnv;


}  // namespace teststruct
}  // namespace test
