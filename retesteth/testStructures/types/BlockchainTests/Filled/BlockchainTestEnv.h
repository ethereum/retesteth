#pragma once
#include <retesteth/testStructures/types/StateTests/Base/StateTestEnvBase.h>
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
// Blockchain test does not have Env section
// This represent env section consructed from blockchain test genesis header
struct BlockchainTestEnv : StateTestEnvBase
{
protected:
    void initializeCommonFields(DataObject const&);
private:
    virtual void define() const = 0; // BlockchainTestEnv is abstract
};

struct BlockchainTestEnvLegacy : BlockchainTestEnv
{
    BlockchainTestEnvLegacy(DataObject const& _data);

protected:
    void initializeLegacyFields(DataObject const&);
private:
    void define() const override {}
};

struct BlockchainTestEnv1559 : BlockchainTestEnv
{
    BlockchainTestEnv1559(DataObject const& _data);

protected:
    void initialize1559Fields(DataObject const&);
private:
    void define() const override {}
};

struct BlockchainTestEnvMerge : BlockchainTestEnv
{
    BlockchainTestEnvMerge(DataObject const& _data);

protected:
    void initializeMergeFields(DataObject const&);

private:
    void define() const override {}
};

typedef GCP_SPointer<BlockchainTestEnv> spBlockchainTestEnv;


}  // namespace teststruct
}  // namespace test
