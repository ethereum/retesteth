#pragma once
#include "../../../basetypes.h"
#include <libdataobj/DataObject.h>

namespace test::teststruct
{

struct BlobSchedule : GCP_SPointerBase
{
    BlobSchedule(DataObject const&);
    BlobSchedule(std::string const& _baseFeeUpdateFraction, std::string const& _max, std::string const& _target)
    {
        baseFeeUpdateFraction = sVALUE(_baseFeeUpdateFraction);
        max = sVALUE(_max);
        target = sVALUE(_target);
    }
    spVALUE baseFeeUpdateFraction;
    spVALUE max;
    spVALUE target;

    spDataObject asDataObject() const;
    BlobSchedule() = delete;
};

typedef GCP_SPointer<BlobSchedule> spBlobSchedule;


struct StateTestConfigBase : GCP_SPointerBase
{
    StateTestConfigBase(std::set<FORK> const&);
    virtual spDataObject asDataObject(FORK const&) const;
    virtual ~StateTestConfigBase() {}
protected:
    spVALUE m_chainid;
    spFORK m_network;
    std::map<FORK, spBlobSchedule> m_blobSchedules;
};



}
