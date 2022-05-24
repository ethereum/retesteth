#pragma once
#include "../../StateTests/Base/StateTestEnvBase.h"
#include <retesteth/dataObject/DataObject.h>
using namespace dataobject;
using namespace test::teststruct;

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

struct BlockchainTestFillerEnvMerge : BlockchainTestFillerEnv, StateTestEnvBaseMerge
{
    TestEnvClass type() const override { return TestEnvClass::MERGE; }
    BlockchainTestFillerEnvMerge(spDataObjectMove _data, SealEngine _sEngine);

protected:
    void initializeMergeFields(DataObject const&) override;
private:
    void define() const override {}
};

struct BlockchainTestFillerEnv1559 : BlockchainTestFillerEnv, StateTestEnvBase1559
{
    TestEnvClass type() const override { return TestEnvClass::EIP1559; }
    BlockchainTestFillerEnv1559(spDataObjectMove _data, SealEngine _sEngine);

protected:
    void initialize1559Fields(DataObject const&) override;
private:
    void define() const override {}

};

struct BlockchainTestFillerEnvLegacy : BlockchainTestFillerEnv, StateTestEnvBaseLegacy
{
    TestEnvClass type() const override { return TestEnvClass::LEGACY; }
    BlockchainTestFillerEnvLegacy(spDataObjectMove _data, SealEngine _sEngine);

protected:
    void initializeLegacyFields(DataObject const&) override;
private:
    void define() const override {}

};


typedef GCP_SPointer<BlockchainTestFillerEnv> spBlockchainTestFillerEnv;


}  // namespace teststruct
}  // namespace test
