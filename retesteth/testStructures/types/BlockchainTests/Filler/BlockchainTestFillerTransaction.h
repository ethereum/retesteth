#pragma once
#include "../../Ethereum/Transaction.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
struct BlockchainTestFillerTransaction : GCP_SPointerBase
{
    BlockchainTestFillerTransaction(DataObject const&);
    Transaction const& tr() const { return m_transaction.getCContent(); }
    bool isMarkedInvalid() const { return m_expectInvalid; }

private:
    BlockchainTestFillerTransaction() {}
    spTransaction m_transaction;
    bool m_expectInvalid;
};

}  // namespace teststruct
}  // namespace test
