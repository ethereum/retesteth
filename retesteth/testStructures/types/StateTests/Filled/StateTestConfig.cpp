#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>
#include "StateTestConfig.h"

using namespace std;
using namespace dataobject;
using namespace test::teststruct;
using namespace test::teststruct::constnames;

namespace
{
void requireStateTestConfigScheme(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "StateTestConfig " + _data.getKey(),
        {{"chainid", {{DataType::String}, jsonField::Required}},
         {"blobSchedule", {{DataType::Object}, jsonField::Optional}},
        });
}

}  // namespace


namespace test::teststruct
{
void StateTestConfig::initializeFields(DataObject const& _data, std::set<FORK> const& _allForks)
{
    m_chainid = sVALUE(_data.atKey("chainid"));

    std::set<FORK> scheduleForks = {FORK("Cancun"), FORK("Prague")};
    for (auto const& scheduleFork : scheduleForks)
    {
        if (!_allForks.count(scheduleFork))
            continue;

        if (!_data.count("blobSchedule"))
        {
            std::string const comment = "Expected field '" + string("blobSchedule") + "' not found in config: " + "StateTestConfig->" + _data.getKey();
            throw test::UpwardsException(comment + "\n" + _data.asJson());
        }

        if (!_data.atKey("blobSchedule").count(scheduleFork.asString()))
        {
            std::string const comment = "Expected field '" + scheduleFork.asString() + "' not found in config: " + "StateTestConfig->blobSchedule";
            throw test::UpwardsException(comment + "\n" + _data.asJson());
        }

        spBlobSchedule schedule = GCP_SPointer<BlobSchedule>(
            new BlobSchedule(_data.atKey("blobSchedule").atKey(scheduleFork.asString()))
        );
        m_blobSchedules[scheduleFork] = schedule;
    }
}


StateTestConfig::StateTestConfig(DataObject const& _data, std::set<FORK> const& _allForks)
  : StateTestConfigBase({})
{
    try
    {
        requireStateTestConfigScheme(_data);
        initializeFields(_data, _allForks);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("StateTestConfig parse error: ") + _ex.what() + _data.asJson());
    }
}

}  // namespace teststruct
