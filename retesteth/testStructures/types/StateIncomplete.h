#pragma once
#include "AccountIncomplete.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
struct StateIncomplete : GCP_SPointerBase
{
    StateIncomplete(DataObject const&);
    std::vector<AccountIncomplete> const& accounts() const { return m_accounts; }

private:
    std::vector<AccountIncomplete> m_accounts;
};

}  // namespace teststruct
}  // namespace test
