#include "BlockchainTestFillerTransaction.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

#include <libdevcrypto/Common.h>

namespace test
{
namespace teststruct
{
BlockchainTestFillerTransaction::BlockchainTestFillerTransaction(DataObject const& _data, NonceMap& _nonceMap)
{
    try
    {
        if (_data.count("expectException"))
            readExpectExceptions(_data.atKey("expectException"), m_expectExceptions);

        DataObject tmpD = _data;

        if (tmpD.count("secretKey") && tmpD.atKey("nonce").asString() == "auto")
        {
            dev::Secret priv(tmpD.atKey("secretKey").asString());
            dev::Address key = dev::toAddress(priv);
            if (_nonceMap.count("0x" + key.hex()))
            {
                tmpD.atKeyUnsafe("nonce").setString(_nonceMap.at("0x" + key.hex())->asDecString());
                _nonceMap["0x" + key.hex()].getContent()++;
            }
            else
                ETH_ERROR_MESSAGE("Parsing tx.nonce `auto` can't find tx.origin in nonce map!");
        }

        tmpD.removeKey("data");
        tmpD.removeKey("to");
        tmpD.removeKey("expectException");
        if (tmpD.count("accessList"))
            tmpD.removeKey("accessList");
        tmpD.performModifier(mod_valueToCompactEvenHexPrefixed);

        // fix 0x prefix on 'to' key
        DataObject dTo = _data.atKey("to");
        string const& to = _data.atKey("to").asString();
        if (to.size() > 1 && to[1] != 'x')
            dTo = "0x" + _data.atKey("to").asString();
        tmpD["to"] = dTo;

        // Compile LLL in transaction data into byte code if not already
        tmpD["data"] = test::compiler::replaceCode(_data.atKey("data").asString());

        if (_data.count("accessList"))
            tmpD["accessList"] = _data.atKey("accessList");

        m_transaction = readTransaction(tmpD);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestFillerTransaction convertion error: ") + _ex.what() + _data.asJson());
    }
}

}  // namespace teststruct
}  // namespace test
