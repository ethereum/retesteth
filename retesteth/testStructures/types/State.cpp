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

}  // namespace teststruct
}  // namespace test
