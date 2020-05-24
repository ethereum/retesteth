#include "StateTestTransaction.h"
#include <retesteth/TestHelper.h>
#include <retesteth/ethObjects/object.h>

namespace test
{
namespace teststruct
{
StateTestTransaction::StateTestTransaction(DataObject const& _data)
{
    if (_data.atKey("to").asString().empty())
        m_creation = true;
    else
    {
        m_creation = false;
        m_to = spFH20(new FH20(_data.atKey("to")));
    }
    m_secretKey = spFH32(new FH32(_data.atKey("secretKey")));
    m_gasPrice = spVALUE(new VALUE(_data.atKey("gasPrice")));
    m_nonce = spVALUE(new VALUE(_data.atKey("nonce")));

    for (auto const& el : _data.atKey("data").getSubObjects())
        m_data.push_back(el);
    for (auto const& el : _data.atKey("gasLimit").getSubObjects())
        m_gasLimit.push_back(el);
    for (auto const& el : _data.atKey("value").getSubObjects())
        m_value.push_back(el);
}

}  // namespace teststruct
}  // namespace test
