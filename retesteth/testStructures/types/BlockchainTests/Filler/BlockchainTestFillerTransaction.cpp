#include "BlockchainTestFillerTransaction.h"

namespace test
{
namespace teststruct
{
BlockchainTestFillerTransaction::BlockchainTestFillerTransaction(DataObject const& _data)
{
    m_expectInvalid = _data.count("invalid");
    m_transaction = spTransaction(new Transaction(_data));
}

}  // namespace teststruct
}  // namespace test
