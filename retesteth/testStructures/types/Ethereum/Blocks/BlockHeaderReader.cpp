#include "BlockHeaderReader.h"
#include <retesteth/EthChecks.h>
#include <retesteth/Constants.h>

using namespace std;
using namespace dataobject;
using namespace test;
using namespace test::teststruct;
using namespace test::teststruct::constnames;

namespace  {


bool isHeader4844(DataObject const& _filledData)
{
    if (_filledData.count(c_excessBlobGas))
        return true;
    return false;
}

bool isHeaderShanghai(DataObject const& _filledData)
{
    if (_filledData.count(c_withdrawalsRoot) && !_filledData.count(c_excessBlobGas))
        return true;
    return false;
}

bool isHeaderMerge(DataObject const& _filledData)
{
    if (!_filledData.count(c_baseFeePerGas) || _filledData.count(c_withdrawalsRoot))
        return false;

    string uncleHashName = "sha3Uncles";
    if (!_filledData.count(uncleHashName))
        uncleHashName = "uncleHash";

    // https://eips.ethereum.org/EIPS/eip-3675
    static const FH32 mergeUncleHash(C_EMPTY_LIST_HASH);
    if (VALUE(_filledData.atKey(c_difficulty)) == 0
        && FH32(_filledData.atKey(uncleHashName)) == mergeUncleHash
        && FH8(_filledData.atKey(c_nonce)) == FH8::zero())
    {
        return true;
    }
    return false;
}

bool isHeader1559(DataObject const& _filledData)
{
    if (_filledData.count(c_baseFeePerGas) && !isHeaderMerge(_filledData)
        && !_filledData.count(c_withdrawalsRoot))
        return true;
    return false;
}

bool isHeaderLegacy(DataObject const& _filledData)
{
    if (!_filledData.count(c_baseFeePerGas))
        return true;
    return false;
}

bool isHeaderShanghai(dev::RLP const& _rlp)
{
    return (_rlp.itemCount() == 17);
}

bool isHeader4844(dev::RLP const& _rlp)
{
    return (_rlp.itemCount() == 20);
}


bool isHeaderMerge(dev::RLP const& _rlp)
{
    if (_rlp.itemCount() != 16)
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
    static const FH32 mergeUncleHash(C_EMPTY_LIST_HASH);
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
    return (_rlp.itemCount() == 16 && !isHeaderMerge(_rlp));
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

    if (isHeaderShanghai(_rlp))
        return spBlockHeader(new BlockHeaderShanghai(_rlp));

    if (isHeader4844(_rlp))
        return spBlockHeader(new BlockHeader4844(_rlp));

    throw test::UpwardsException("readBlockHeader(RLP): unknown block type! \n" + dev::asString(_rlp.toBytes()));
}

spBlockHeader readBlockHeader(DataObject const& _filledData)
{
    if (isHeaderLegacy(_filledData))
        return spBlockHeader(new BlockHeaderLegacy(_filledData));

    if (isHeader1559(_filledData))
        return spBlockHeader(new BlockHeader1559(_filledData));

    if (isHeaderMerge(_filledData))
        return spBlockHeader(new BlockHeaderMerge(_filledData));

    if (isHeaderShanghai(_filledData))
        return spBlockHeader(new BlockHeaderShanghai(_filledData));

    if (isHeader4844(_filledData))
        return spBlockHeader(new BlockHeader4844(_filledData));

    ETH_ERROR_MESSAGE("readBlockHeader(DATA): unknown block type! \n" + _filledData.asJson());
    return spBlockHeader(new BlockHeaderLegacy(_filledData));
}

bool isBlockPoS(BlockHeader const& _header)
{
    return isBlockExportCurrentRandom(_header);
}

bool isBlockExportCurrentRandom(BlockHeader const& _header)
{
    return _header.type() == BlockType::BlockHeaderMerge
           || _header.type() == BlockType::BlockHeaderShanghai
           || _header.type() == BlockType::BlockHeader4844;
}

bool isBlockExportExcessBlobGas(BlockHeader const& _header)
{
    return _header.type() == BlockType::BlockHeader4844;
}

bool isBlockExportWithdrawals(BlockHeader const& _header)
{
    return _header.type() == BlockType::BlockHeaderShanghai
           || _header.type() == BlockType::BlockHeader4844;
}

bool isBlockExportBasefee(BlockHeader const& _header)
{
    return _header.type() == BlockType::BlockHeader1559
           || _header.type() == BlockType::BlockHeaderMerge
           || _header.type() == BlockType::BlockHeaderShanghai
           || _header.type() == BlockType::BlockHeader4844;
}

bool isBlockExportDifficulty(BlockHeader const& _header)
{
    return _header.type() == BlockType::BlockHeaderLegacy
           || _header.type() == BlockType::BlockHeader1559;
}

}  // namespace teststruct
