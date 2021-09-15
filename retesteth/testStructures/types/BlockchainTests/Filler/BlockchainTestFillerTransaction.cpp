#include "BlockchainTestFillerTransaction.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

#include <libdevcrypto/Common.h>

namespace test
{
namespace teststruct
{
BlockchainTestFillerTransaction::BlockchainTestFillerTransaction(spDataObjectMove _data, NonceMap& _nonceMap)
{
    try
    {
        m_rawData = _data.getPointer();
        if (m_rawData->count("expectException"))
            readExpectExceptions(m_rawData->atKey("expectException"), m_expectExceptions);

        // Convert nonce = "auto"
        if (m_rawData->count("secretKey") && m_rawData->atKey("nonce").asString() == "auto")
        {
            dev::Secret priv(m_rawData->atKey("secretKey").asString());
            dev::Address key = dev::toAddress(priv);
            if (_nonceMap.count("0x" + key.hex()))
            {
                (*m_rawData).atKeyUnsafe("nonce").setString(_nonceMap.at("0x" + key.hex())->asDecString());
                _nonceMap["0x" + key.hex()].getContent()++;
            }
            else
                ETH_ERROR_MESSAGE("Parsing tx.nonce `auto` can't find tx.origin in nonce map!");
        }

        (*m_rawData).removeKey("expectException");
        convertDecTransactionToHex(m_rawData);
        m_transaction = readTransaction(m_rawData);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(
            string("BlockchainTestFillerTransaction convertion error: ") + _ex.what() + _data.getPointer()->asJson());
    }
}

}  // namespace teststruct
}  // namespace test
