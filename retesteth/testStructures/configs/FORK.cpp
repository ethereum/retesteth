#include "FORK.h"
#include <retesteth/EthChecks.h>
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
// Can add a validation here so not to have inalid forks ever
FORK::FORK(DataObject const& _data) : m_data(_data) {}

}  // namespace teststruct
}  // namespace test
