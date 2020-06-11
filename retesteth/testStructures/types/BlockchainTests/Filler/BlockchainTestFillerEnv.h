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
    BlockchainTestFillerEnv(DataObject const& _data, SealEngine _sEngine);
};

typedef GCP_SPointer<BlockchainTestFillerEnv> spBlockchainTestFillerEnv;


}  // namespace teststruct
}  // namespace test
