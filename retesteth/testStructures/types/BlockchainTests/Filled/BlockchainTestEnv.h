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
struct BlockchainTestEnv : StateTestEnvBase
{
protected:
    void initializeCommonFields(DataObject const&);
private:
    virtual void define() const = 0; // BlockchainTestEnv is abstract
};

struct BlockchainTestEnvLegacy : BlockchainTestEnv, StateTestEnvBaseLegacy
{
    TestEnvClass type() const override { return TestEnvClass::LEGACY; }
    BlockchainTestEnvLegacy(DataObject const& _data);

protected:
    void initializeLegacyFields(DataObject const&) override;
private:
    void define() const override {}
};

struct BlockchainTestEnv1559 : BlockchainTestEnv, StateTestEnvBase1559
{
    TestEnvClass type() const override { return TestEnvClass::EIP1559; }
    BlockchainTestEnv1559(DataObject const& _data);

protected:
    void initialize1559Fields(DataObject const&) override;
private:
    void define() const override {}
};

typedef GCP_SPointer<BlockchainTestEnv> spBlockchainTestEnv;


}  // namespace teststruct
}  // namespace test
