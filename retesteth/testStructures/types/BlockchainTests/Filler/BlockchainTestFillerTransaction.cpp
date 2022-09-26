#include "BlockchainTestFillerTransaction.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

#include <libdevcrypto/Common.h>
using namespace std;

namespace test::teststruct
{
BlockchainTestFillerTransaction::BlockchainTestFillerTransaction(spDataObjectMove _data, NonceMap& _nonceMap)
{
    try
    {
        spDataObject rawData;
        rawData = _data.getPointer();
        if (rawData->count("expectException"))
            readExpectExceptions(rawData->atKey("expectException"), m_expectExceptions);

        // Convert nonce = "auto"
        if (rawData->count("secretKey") && rawData->atKey("nonce").asString() == "auto")
        {
            dev::Secret priv(rawData->atKey("secretKey").asString());
            dev::Address key = dev::toAddress(priv);
            if (_nonceMap.count("0x" + key.hex()))
            {
                (*rawData).atKeyUnsafe("nonce").setString(_nonceMap.at("0x" + key.hex())->asDecString());
                _nonceMap["0x" + key.hex()].getContent()++;
            }
            else
                ETH_ERROR_MESSAGE("Parsing tx.nonce `auto` can't find tx.origin in nonce map!");
        }

        (*rawData).removeKey("expectException");
        convertDecTransactionToHex(rawData);

        // Translate secretKey to public key before parsing the transaction
        if (rawData->count("secretKey") && !rawData->count("sender"))
        {
            // TODO if FH32 initialization throw here, it wont end up with the correct message in catch
            spFH32 secret(new FH32(rawData->atKey("secretKey").asString()));
            (*rawData)["sender"] = convertSecretToPublic(secret)->asString();
        }

        m_transaction = readTransaction(dataobject::move(rawData));
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(
            string("BlockchainTestFillerTransaction convertion error: ") + _ex.what() + _data.getPointer()->asJson());
    }
}

}  // namespace teststruct
