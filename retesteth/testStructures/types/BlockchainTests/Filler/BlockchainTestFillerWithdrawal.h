#pragma once
#include <libdataobj/DataObject.h>
#include <retesteth/testStructures/types/Ethereum/Withdrawals.h>
namespace test::teststruct
{
struct BlockchainTestFillerWithdrawal
{
    BlockchainTestFillerWithdrawal(){};
    BlockchainTestFillerWithdrawal(dataobject::spDataObjectMove);
    Withdrawals const& withdrawals() const { return m_withdrawals; }
private:
    spWithdrawals m_withdrawals;
};
}  // namespace test
