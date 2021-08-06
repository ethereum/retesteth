#pragma once
#include "AccountBase.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
struct StateBase : GCP_SPointerBase
{
    std::map<FH20, spAccountBase> const& accounts() const { return m_accounts; }
    virtual spDataObject asDataObject(ExportOrder order = ExportOrder::Default) const = 0;
    virtual ~StateBase() {}

protected:
    StateBase(){};
    std::map<FH20, spAccountBase> m_accounts;
};

}  // namespace teststruct
}  // namespace test
