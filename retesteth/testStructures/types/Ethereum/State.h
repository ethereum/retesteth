#pragma once
#include "Base/StateBase.h"
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
using namespace dataobject;

// Marks that State is made of full account objects
struct State : StateBase
{

public:
    // Mark that account has internal data of State and editing pointer of one will edit the state
    struct Account;
    typedef GCP_SPointer<Account> spAccount;

public:
    State(spDataObjectMove);
    State(std::map<FH20, spAccountBase>&);

    std::map<FH20, spAccountBase> const& accounts() const { return m_accounts; }
    Account const& getAccount(FH20 const& _address) const;
    bool hasAccount(Account const& _account) const;
    bool hasAccount(FH20 const& _address) const;

    spDataObject const& asDataObject() const override;

private:
    spDataObject m_raw;
    State() {}

public:
    // Ethereum account description
    // Describe the strict account defenition
    // Constructor require all fields to be set
    struct Account : AccountBase
    {
        Account(spDataObject&);
        Account(FH20 const& _addr, spVALUE& _balance, spVALUE& _nonce, spBYTES& _code, spStorage& _storage);
        spDataObject const& asDataObject() const override;
        AccountType type() const override { return AccountType::FullAccount; }

    private:
        Account() {}
    };

};

typedef GCP_SPointer<State> spState;

}  // namespace teststruct
