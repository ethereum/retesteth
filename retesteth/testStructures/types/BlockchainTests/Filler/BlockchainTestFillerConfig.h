#pragma once
#include <retesteth/testStructures/types/StateTests/Base/StateTestConfigBase.h>
#include <libdataobj/DataObject.h>

namespace test::teststruct
{

struct BlockchainTestFillerConfig : StateTestConfigBase
{
    BlockchainTestFillerConfig(std::set<FORK> const&);
};

typedef GCP_SPointer<BlockchainTestFillerConfig> spBlockchainTestFillerConfig;


}  // namespace teststruct
