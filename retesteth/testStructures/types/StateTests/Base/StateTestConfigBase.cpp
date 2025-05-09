#include "StateTestConfigBase.h"
#include <retesteth/testStructures/Common.h>

namespace test::teststruct
{

BlobSchedule::BlobSchedule(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "TestConfig::BlobSchedule " + _data.getKey(),
        {
            {"baseFeeUpdateFraction", {{DataType::String}, jsonField::Required}},
            {"max", {{DataType::String}, jsonField::Required}},
            {"target", {{DataType::String}, jsonField::Required}}
        });
}

spDataObject BlobSchedule::asDataObject() const
{
    spDataObject data;
    (*data)["baseFeeUpdateFraction"] = baseFeeUpdateFraction->asString();
    (*data)["max"] = max->asString();
    (*data)["target"] = target->asString();
    return data;
}

StateTestConfigBase::StateTestConfigBase(std::set<FORK> const& _testForks)
{
    m_chainid = sVALUE(1);
    if (_testForks.count(FORK("Cancun")))
    {
        spBlobSchedule cancunSchedule = GCP_SPointer<BlobSchedule>(new BlobSchedule("0x32f0ed", "0x06", "0x03"));
        m_blobSchedules.emplace(FORK("Cancun"), cancunSchedule);
    }
    if (_testForks.count(FORK("Prague")))
    {
        spBlobSchedule pragueSchedule = GCP_SPointer<BlobSchedule>(new BlobSchedule("0x4c6964", "0x09", "0x06"));
        m_blobSchedules.emplace(FORK("Prague"), pragueSchedule);
    }
}

spDataObject StateTestConfigBase::asDataObject(FORK const& _scheduleForForkOnly) const
{
    spDataObject data;
    (*data)["chainid"] = m_chainid->asString();
    if (!m_network.isEmpty())
        (*data)["network"] = m_network->asString();
    if (m_blobSchedules.size() > 0)
    {
        for (auto const& [fork, schedule] : m_blobSchedules)
        {
            if (_scheduleForForkOnly == "ALL" || fork == _scheduleForForkOnly)
                (*data)["blobSchedule"].atKeyPointer(fork.asString()) = schedule->asDataObject();
        }
    }
    return data;
}

}
