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
    BlockchainTestEnv(DataObject const& _data);
};

typedef GCP_SPointer<BlockchainTestEnv> spBlockchainTestEnv;


}  // namespace teststruct
}  // namespace test
