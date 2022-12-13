#include "BlockHeaderReader.h"
#include <retesteth/EthChecks.h>

using namespace std;
using namespace dataobject;
using namespace test::teststruct;

namespace  {

bool isHeaderMerge(DataObject const& _filledData)
{
    if (!_filledData.count("baseFeePerGas"))
        return false;

    string uncleHashName = "sha3Uncles";
    if (!_filledData.count(uncleHashName))
        uncleHashName = "uncleHash";

    // https://eips.ethereum.org/EIPS/eip-3675
    static const FH32 mergeUncleHash("0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347");
    if (VALUE(_filledData.atKey("difficulty")) == 0
        && FH32(_filledData.atKey(uncleHashName)) == mergeUncleHash
        && FH8(_filledData.atKey("nonce")) == FH8::zero())
    {
        return true;
    }
    return false;
}

bool isHeader1559(DataObject const& _filledData)
{
    if (_filledData.count("baseFeePerGas") && !isHeaderMerge(_filledData))
        return true;
    return false;
}

bool isHeaderLegacy(DataObject const& _filledData)
{
    if (!_filledData.count("baseFeePerGas"))
        return true;
    return false;
}



bool isHeaderMerge(dev::RLP const& _rlp)
{
    if (_rlp.itemCount() == 15)
        return false;
    // 0 - parentHash           // 8 - number
    // 1 - uncleHash            // 9 - gasLimit
    // 2 - coinbase             // 10 - gasUsed
    // 3 - stateRoot            // 11 - timestamp
    // 4 - transactionTrie      // 12 - extraData
    // 5 - receiptTrie          // 13 - mixHash
    // 6 - bloom                // 14 - nonce
    // 7 - difficulty           // 15 - baseFee

    // https://eips.ethereum.org/EIPS/eip-3675
    static const FH32 mergeUncleHash("0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347");
    if (VALUE(_rlp[7]) == 0
        && FH32(_rlp[1]) == mergeUncleHash
        && FH8(_rlp[14]) == FH8::zero())
    {
        return true;
    }
    return false;
}

bool isHeader1559(dev::RLP const& _rlp)
{
    return (_rlp.itemCount() != 15 && !isHeaderMerge(_rlp));
}

bool isHeaderLegacy(dev::RLP const& _rlp)
{
    return (_rlp.itemCount() == 15);
}

}

namespace test::teststruct
{

spBlockHeader readBlockHeader(dev::RLP const& _rlp)
{
    if (isHeaderLegacy(_rlp))
        return spBlockHeader(new BlockHeaderLegacy(_rlp));

    if (isHeader1559(_rlp))
        return spBlockHeader(new BlockHeader1559(_rlp));

    if (isHeaderMerge(_rlp))
        return spBlockHeader(new BlockHeaderMerge(_rlp));

    ETH_ERROR_MESSAGE("readBlockHeader: unknown block type! /n" + dev::asString(_rlp.toBytes()));
    return spBlockHeader(new BlockHeaderLegacy(_rlp));
}

spBlockHeader readBlockHeader(DataObject const& _filledData)
{
    if (isHeaderLegacy(_filledData))
        return spBlockHeader(new BlockHeaderLegacy(_filledData));

    if (isHeader1559(_filledData))
        return spBlockHeader(new BlockHeader1559(_filledData));

    if (isHeaderMerge(_filledData))
        return spBlockHeader(new BlockHeaderMerge(_filledData));

    ETH_ERROR_MESSAGE("readBlockHeader: unknown block type! /n" + _filledData.asJson());
    return spBlockHeader(new BlockHeaderLegacy(_filledData));
}

}  // namespace teststruct
