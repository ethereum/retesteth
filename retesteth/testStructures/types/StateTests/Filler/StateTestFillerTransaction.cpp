#include "StateTestFillerTransaction.h"
#include <retesteth/TestHelper.h>
#include <retesteth/ethObjects/object.h>

namespace test
{
namespace teststruct
{
StateTestFillerTransaction::StateTestFillerTransaction(DataObject const& _data)
{
    // Read data from JSON in test filler
    // The VALUE fields can be decimal -> convert it to hex
    // The data field can be LLL or other code -> compile it to BYTES

    m_to = spFH20(new FH20(_data.atKey("to")));
    m_secretKey = spFH32(new FH32(_data.atKey("secretKey")));

    // -- Explicitly convert transaction dec fields into hex values
    DataObject tmpD;
    tmpD["gasLimit"] = _data.atKey("gasLimit");
    tmpD["gasPrice"] = _data.atKey("gasPrice");
    tmpD["nonce"] = _data.atKey("nonce");
    tmpD["value"] = _data.atKey("value");
    tmpD.performModifier(mod_valueToCompactEvenHexPrefixed);
    m_gasPrice = spVALUE(new VALUE(tmpD.atKey("gasPrice")));
    m_nonce = spVALUE(new VALUE(tmpD.atKey("nonce")));
    // --

    for (auto const& el : _data.atKey("data").getSubObjects())
    {
        // -- Compile LLL in transaction data into byte code if not already
        DataObject dataInKey = el;
        dataInKey.setKey("`data` array element in General Transaction Section");  // Hint
        dataInKey.setString(test::replaceCode(dataInKey.asString()));
        // ---

        m_data.push_back(dataInKey);
    }
    for (auto const& el : tmpD.atKey("gasLimit").getSubObjects())
        m_gasLimit.push_back(el);
    for (auto const& el : tmpD.atKey("value").getSubObjects())
        m_value.push_back(el);
}

DataObject StateTestFillerTransaction::asDataObject() const
{
    // Serialize data back to JSON
    DataObject out;
    for (BYTES const& el : m_data)
        out["data"].addArrayObject(el.asString());
    for (VALUE const& el : m_gasLimit)
        out["gasLimit"].addArrayObject(el.asString());
    for (VALUE const& el : m_value)
        out["value"].addArrayObject(el.asString());
    out["gasPrice"] = m_gasPrice.getCPtr()->asString();
    out["nonce"] = m_nonce.getCPtr()->asString();
    out["to"] = m_to.getCPtr()->asString();
    out["secretKey"] = m_secretKey.getCPtr()->asString();
    return out;
}

std::vector<TransactionInGeneralSection> StateTestFillerTransaction::buildTransactions() const
{
    // Construct vector of all transactions that are described int data
    std::vector<TransactionInGeneralSection> out;
    for (size_t dIND = 0; dIND < m_data.size(); dIND++)
    {
        for (size_t gIND = 0; gIND < m_gasLimit.size(); gIND++)
        {
            for (size_t vIND = 0; vIND < m_value.size(); vIND++)
            {
                DataObject trData;
                trData["data"] = m_data.at(dIND).asString();
                trData["gasLimit"] = m_gasLimit.at(gIND).asString();
                trData["value"] = m_value.at(vIND).asString();

                trData["gasPrice"] = m_gasPrice.getCPtr()->asString();
                trData["nonce"] = m_nonce.getCPtr()->asString();
                trData["to"] = m_to.getCPtr()->asString();
                trData["secretKey"] = m_secretKey.getCPtr()->asString();
                out.push_back(TransactionInGeneralSection(spTransaction(new Transaction(trData)), dIND, gIND, vIND));
            }
        }
    }
    return out;
}

TransactionInGeneralSection::TransactionInGeneralSection(spTransaction const& _tr, size_t _dInd, size_t _gInd, size_t _vInd)
    : m_tr(_tr), m_dInd(_dInd), m_gInd(_gInd), m_vInd(_vInd), m_executed(false), m_skipped(false)
{}

}  // namespace teststruct
}  // namespace test
