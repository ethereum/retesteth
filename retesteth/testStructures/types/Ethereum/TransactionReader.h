#include "Transaction.h"
#include "TransactionAccessList.h"

using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
spTransaction readTransaction(spDataObjectMove _filledData);
spTransaction readTransaction(BYTES const& _rlp);
spTransaction readTransaction(dev::RLP const& _rlp);

}  // namespace teststruct
}  // namespace test
