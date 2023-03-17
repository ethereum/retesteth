#include "FH256.h"
#include "Constants.h"
using namespace test::teststruct;

FH256 const& FH256::zero()
{
    return C_FH256_ZERO;
}

FH256* FH256::copy() const
{
    return new FH256(asString());
}
