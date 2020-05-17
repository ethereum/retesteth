#pragma once
#include "Account.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
struct State : GCP_SPointerBase
{
    State(DataObject const&);
    std::vector<Account> const& accounts() const { return m_accounts; }
    DataObject asDataObject() const;

private:
    std::vector<Account> m_accounts;
};

}  // namespace teststruct
}  // namespace test
