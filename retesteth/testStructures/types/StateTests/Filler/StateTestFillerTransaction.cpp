#include "StateTestFillerTransaction.h"
#include <retesteth/TestHelper.h>
#include <retesteth/ethObjects/object.h>

namespace test
{
namespace teststruct
{
StateTestFillerTransaction::StateTestFillerTransaction(DataObject const& _data)
{
    m_to = GCP_SPointer<FH20>(new FH20(_data.atKey("to")));
    m_secretKey = GCP_SPointer<FH32>(new FH32(_data.atKey("secretKey")));

    // -- Explicitly convert transaction dec fields into hex values
    DataObject tmpD = _data;
    tmpD.performModifier(mod_valueToCompactEvenHexPrefixed);
    m_gasPrice = GCP_SPointer<VALUE>(new VALUE(tmpD.atKey("gasPrice")));
    m_nonce = GCP_SPointer<VALUE>(new VALUE(tmpD.atKey("nonce")));
    // --

    for (auto const& el : _data.atKey("data").getSubObjects())
    {
        // -- Compile LLL in transaction data into byte code if not already
        DataObject tmpD = el;
        tmpD.setKey("`data` array element in General Transaction Section");  // Hint
        tmpD.setString(test::replaceCode(tmpD.asString()));
        // ---

        m_data.push_back(tmpD);
    }
    for (auto const& el : tmpD.atKey("gasLimit").getSubObjects())
        m_gasLimit.push_back(el);
    for (auto const& el : tmpD.atKey("value").getSubObjects())
        m_value.push_back(el);
}

DataObject StateTestFillerTransaction::asDataObject() const
{
    DataObject out;
    for (BYTES const& el : m_data)
        out["data"].addArrayObject(el.asString());
    for (VALUE const& el : m_gasLimit)
        out["gasLimit"].addArrayObject(el.asString());
    for (VALUE const& el : m_value)
        out["gasLimit"].addArrayObject(el.asString());
    out["gasPrice"] = m_gasPrice.getCPtr()->asString();
    out["nonce"] = m_nonce.getCPtr()->asString();
    out["to"] = m_to.getCPtr()->asString();
    out["secretKey"] = m_secretKey.getCPtr()->asString();
    return out;
}

}  // namespace teststruct
}  // namespace test
