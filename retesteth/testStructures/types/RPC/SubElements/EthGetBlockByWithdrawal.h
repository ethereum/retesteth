#pragma once
#include "../../../basetypes.h"
#include <retesteth/testStructures/types/Ethereum/Withdrawals.h>
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
// Withdrawals Structure inside RPC response eth_getBlockByHash/eth_getBlockByNumber
struct EthGetBlockByWithdrawal : GCP_SPointerBase
{
    EthGetBlockByWithdrawal(spDataObjectMove);
    spWithdrawal const& withdrawal() const { return m_withdrawal; }
private:
    spWithdrawal m_withdrawal;
};

typedef GCP_SPointer<EthGetBlockByWithdrawal> spEthGetBlockByWithdrawal;

}  // namespace teststruct
