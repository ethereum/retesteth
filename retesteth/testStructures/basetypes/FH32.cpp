#include "FH32.h"
#include "Constants.h"
using namespace test::teststruct;

FH32 const& FH32::zero()
{
    return C_FH32_ZERO;
}

FH32* FH32::copy() const
{
    return new FH32(asString());
}
