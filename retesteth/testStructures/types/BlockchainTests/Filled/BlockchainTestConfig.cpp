#include "BlockchainTestConfig.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>

using namespace std;
using namespace dataobject;
using namespace test::teststruct;
using namespace test::teststruct::constnames;


namespace
{
void requireBlockchainTestConfigScheme(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "BlockchainTestConfig " + _data.getKey(),
        {
            {"chainid", {{DataType::String}, jsonField::Required}},
            {"network", {{DataType::String}, jsonField::Required}},
            {"blobSchedule", {{DataType::Object}, jsonField::Optional}},
        });
}

}  // namespace



namespace test::teststruct
{
void BlockchainTestConfig::initializeFields(DataObject const& _data, std::set<FORK> const& _allForks)
{
    m_chainid = sVALUE(_data.atKey("chainid"));
    m_network = spFORK(new FORK(_data.atKey("network")));

    std::set<FORK> scheduleForks = {FORK("Cancun"), FORK("Prague")};
    for (auto const& scheduleFork : scheduleForks)
    {
        if (!_allForks.count(scheduleFork))
            continue;

        if (!_data.count("blobSchedule"))
        {
            std::string const comment = "Expected field '" + string("blobSchedule") + "' not found in config: " + "BlockchainTestConfig->" + _data.getKey();
            throw test::UpwardsException(comment + "\n" + _data.asJson());
        }

        if (!_data.atKey("blobSchedule").count(scheduleFork.asString()))
        {
            std::string const comment = "Expected field '" + scheduleFork.asString() + "' not found in config: " + "BlockchainTestConfig->blobSchedule";
            throw test::UpwardsException(comment + "\n" + _data.asJson());
        }

        spBlobSchedule schedule = GCP_SPointer<BlobSchedule>(
            new BlobSchedule(_data.atKey("blobSchedule").atKey(scheduleFork.asString()))
            );
        m_blobSchedules[scheduleFork] = schedule;
    }
}

BlockchainTestConfig::BlockchainTestConfig(DataObject const& _data, FORK _fork)
  : StateTestConfigBase({})
{
    try
    {
        requireBlockchainTestConfigScheme(_data);
        initializeFields(_data, {_fork});
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestConfig parse error: ") + _ex.what() + _data.asJson());
    }
}
}
