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
    State(std::map<FH20, spAccount> const&);

    std::map<FH20, spAccount> const& accounts() const { return m_accounts; }
    Account const& getAccount(FH20 const& _address) const;
    bool hasAccount(Account const& _account) const;
    bool hasAccount(FH20 const& _address) const;

    DataObject asDataObject() const;

private:
    State(){};
    // Map require spAccount() constructor
    std::map<FH20, spAccount> m_accounts;
};

typedef GCP_SPointer<State> spState;

}  // namespace teststruct
}  // namespace test
