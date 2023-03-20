#pragma once
#include <libdataobj/DataObject.h>
#include <retesteth/testStructures/types/Ethereum/Withdrawals.h>
namespace test::teststruct
{
struct BlockchainTestFillerWithdrawal
{
    BlockchainTestFillerWithdrawal(){};
    BlockchainTestFillerWithdrawal(dataobject::spDataObjectMove);
    spWithdrawal const& withdrawal() const { return m_withdrawal; }
private:
    spWithdrawal m_withdrawal;
};
}  // namespace test
