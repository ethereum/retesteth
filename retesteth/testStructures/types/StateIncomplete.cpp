#include "StateIncomplete.h"
#include "AccountIncomplete.h"

namespace test
{
namespace teststruct
{
StateIncomplete::StateIncomplete(DataObject const& _data)
{
    for (auto const& el : _data.getSubObjects())
        m_accounts.push_back(AccountIncomplete(el));
}

}  // namespace teststruct
}  // namespace test
