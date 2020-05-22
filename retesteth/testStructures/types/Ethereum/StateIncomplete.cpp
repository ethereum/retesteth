#include "StateIncomplete.h"
#include "AccountIncomplete.h"

namespace test
{
namespace teststruct
{
StateIncomplete::StateIncomplete(DataObject const& _data)
{
    for (auto const& el : _data.getSubObjects())
        m_accounts[FH20(el.getKey())] = spAccountIncomplete(new AccountIncomplete(el));
}

DataObject StateIncomplete::asDataObject() const
{
    DataObject out;
    for (auto const& el : m_accounts)
        out.addSubObject(el.second.getCContent().asDataObject());
    return out;
}

}  // namespace teststruct
}  // namespace test
