#include "BlockchainTestFillerTransaction.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
BlockchainTestFillerTransaction::BlockchainTestFillerTransaction(DataObject const& _data)
{
    try
    {
        m_expectInvalid = _data.count("invalid");

        DataObject tmpD = _data;
        tmpD.removeKey("data");
        tmpD.removeKey("to");
        tmpD.performModifier(mod_valueToCompactEvenHexPrefixed);

        // fix 0x prefix on 'to' key
        DataObject dTo = _data.atKey("to");
        string const& to = _data.atKey("to").asString();
        if (to.size() > 1 && to[1] != 'x')
            dTo = "0x" + _data.atKey("to").asString();
        tmpD["to"] = dTo;

        // Compile LLL in transaction data into byte code if not already
        tmpD["data"] = test::compiler::replaceCode(_data.atKey("data").asString());

        m_transaction = spTransaction(new Transaction(tmpD));
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("BlockchainTestFillerTransaction convertion error: ") + _ex.what() + _data.asJson());
    }
}

}  // namespace teststruct
}  // namespace test
