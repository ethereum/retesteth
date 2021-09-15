#include "StateTestEnvBase.h"
#include <retesteth/EthChecks.h>
namespace test
{
namespace teststruct
{

spDataObject const& StateTestEnvBase::asDataObject() const
{
    return m_raw;
}

}  // namespace teststruct
}  // namespace test
