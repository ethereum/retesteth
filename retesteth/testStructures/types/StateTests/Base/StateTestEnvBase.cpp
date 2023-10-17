#include "StateTestEnvBase.h"
namespace test::teststruct
{

spDataObject const& StateTestEnvBase::asDataObject() const
{
    // This method is called too many times. Output what we parsed from input file.
    return m_raw;
}

}  // namespace teststruct
