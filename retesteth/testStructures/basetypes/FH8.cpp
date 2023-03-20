#include "FH8.h"
#include "Constants.h"
using namespace test::teststruct;

FH8 const& FH8::zero()
{
    return C_FH8_ZERO;
}

FH8* FH8::copy() const
{
    return new FH8(asString());
}
