#include "State.h"
#include "Account.h"

namespace test
{
namespace teststruct
{
State::State(DataObject const& _data)
{
    for (auto const& el : _data.getSubObjects())
        m_accounts.push_back(Account(el));
}

DataObject State::asDataObject() const
{
    DataObject out;
    for (Account const& el : m_accounts)
        out.addSubObject(el.asDataObject());
    return out;
}

}  // namespace teststruct
}  // namespace test
