#include "BlockchainTestFillerConfig.h"

namespace test::teststruct
{
BlockchainTestFillerConfig::BlockchainTestFillerConfig(std::set<FORK> const& _testNetwork)
  : StateTestConfigBase(_testNetwork)
{}

}
