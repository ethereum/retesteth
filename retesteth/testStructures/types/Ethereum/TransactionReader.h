#include "Transaction.h"

namespace test::teststruct
{
spTransaction readTransaction(spDataObjectMove _filledData);
spTransaction readTransaction(BYTES const& _rlp);
spTransaction readTransaction(dev::RLP const& _rlp);

}  // namespace teststruct
