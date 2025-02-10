#pragma once
#include <retesteth/testStructures/types/StateTests/Base/StateTestConfigBase.h>
#include <libdataobj/DataObject.h>

namespace test::teststruct
{

struct BlockchainTestConfig : StateTestConfigBase
{
    BlockchainTestConfig(DataObject const&, FORK _fork);
    ~BlockchainTestConfig() override {}
private:
    void initializeFields(DataObject const& _data, std::set<FORK> const&);
};
typedef GCP_SPointer<BlockchainTestConfig> spBlockchainTestConfig;


}  // namespace teststruct
