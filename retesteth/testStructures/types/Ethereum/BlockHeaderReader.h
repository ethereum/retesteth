#include "BlockHeader.h"
#include "BlockHeader1559.h"

using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
spBlockHeader readBlockHeader(DataObject const& _data);
spBlockHeader readBlockHeader(dev::RLP const& _rlp);

}  // namespace teststruct
}  // namespace test
