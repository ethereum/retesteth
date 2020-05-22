#pragma once
#include "../../basetypes.h"
#include <retesteth/dataObject/DataObject.h>

using namespace dataobject;

namespace test
{
namespace teststruct
{
// Transaction Structure inside RPC response eth_getBlockByHash/eth_getBlockByNumber
struct EthGetBlockByTransaction
{
    EthGetBlockByTransaction(DataObject const&);
    FH32 const& hash() const { return m_hash.getCContent(); }

private:
    spFH32 m_blockHash;
    spVALUE m_blockNumber;
    spFH20 m_from;
    spVALUE m_gas;
    spVALUE m_gasPrice;
    spFH32 m_hash;
    spBYTES m_input;
    spVALUE m_nonce;
    spFH20 m_to;
    spVALUE m_transactionIndex;
    spVALUE m_value;
    spVALUE m_v;
    spVALUE m_r;
    spVALUE m_s;
};

}  // namespace teststruct
}  // namespace test
