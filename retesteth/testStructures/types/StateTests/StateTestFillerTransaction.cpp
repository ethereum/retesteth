#include "StateTestFillerTransaction.h"

namespace test
{
namespace teststruct
{
StateTestFillerTransaction::StateTestFillerTransaction(DataObject const& _data)
  : m_gasPrice(_data.atKey("gasPrice")),
    m_nonce(_data.atKey("nonce")),
    m_secretKey(_data.atKey("secretKey")),
    m_to(_data.atKey("to"))
{
    for (auto const& el : _data.atKey("data").getSubObjects())
        m_data.push_back(el);
    for (auto const& el : _data.atKey("gasLimit").getSubObjects())
        m_gasLimit.push_back(el);
    for (auto const& el : _data.atKey("value").getSubObjects())
        m_value.push_back(el);
}

}  // namespace teststruct
}  // namespace test
