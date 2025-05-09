#pragma once
#include "AccountBase.h"
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
struct StateBase : GCP_SPointerBase
{
    std::map<FH20, spAccountBase> const& accounts() const { return m_accounts; }
    virtual spDataObject asDataObject() const = 0;
    virtual ~StateBase() {}

protected:
    StateBase(){};
    std::map<FH20, spAccountBase> m_accounts;
};

}  // namespace teststruct
}  // namespace test
