#pragma once
#include "Account.h"
#include "Base/StateBase.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
// Marks that State is made of full account objects
struct State : StateBase
{
    State(spDataObjectMove);
    State(std::vector<spAccount>&);

    std::map<FH20, spAccountBase> const& accounts() const { return m_accounts; }
    Account const& getAccount(FH20 const& _address) const;
    bool hasAccount(Account const& _account) const;
    bool hasAccount(FH20 const& _address) const;

    spDataObject asDataObject(ExportOrder order = ExportOrder::Default) const override;

private:
    spDataObject m_raw;
    std::vector<FH20> m_order;
    State() {}
};

typedef GCP_SPointer<State> spState;

}  // namespace teststruct
}  // namespace test
