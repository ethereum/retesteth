#include "BlockHeaderReader.h"
#include <retesteth/EthChecks.h>
#include <retesteth/Constants.h>

using namespace std;
using namespace dataobject;
using namespace test;
using namespace test::teststruct;
using namespace test::teststruct::constnames;

namespace  {

bool isHeaderPrague(DataObject const& _filledData)
{
    if (_filledData.count(c_requestsHash))
        return true;
    return false;
}

bool isHeader4844(DataObject const& _filledData)
{
    if (_filledData.count(c_excessBlobGas) && !_filledData.count(c_requestsHash))
        return true;
    return false;
}

bool isHeaderShanghai(DataObject const& _filledData)
{
    if (_filledData.count(c_withdrawalsRoot) && !_filledData.count(c_excessBlobGas))
        return true;
    return false;
}

bool isHeaderParis(DataObject const& _filledData)
{
    if (!_filledData.count(c_baseFeePerGas) || _filledData.count(c_withdrawalsRoot))
        return false;

    string uncleHashName = "sha3Uncles";
    if (!_filledData.count(uncleHashName))
        uncleHashName = "uncleHash";

    // https://eips.ethereum.org/EIPS/eip-3675
    static const FH32 parisUncleHash(C_EMPTY_LIST_HASH);
    if (VALUE(_filledData.atKey(c_difficulty)) == 0
        && FH32(_filledData.atKey(uncleHashName)) == parisUncleHash
        && FH8(_filledData.atKey(c_nonce)) == FH8::zero())
    {
        return true;
    }
    return false;
}

bool isHeader1559(DataObject const& _filledData)
{
    if (_filledData.count(c_baseFeePerGas) && !isHeaderParis(_filledData)
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

bool isHeaderPrague(dev::RLP const& _rlp)
{
    return (_rlp.itemCount() == 21);
}


bool isHeaderParis(dev::RLP const& _rlp)
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
    return (_rlp.itemCount() == 16 && !isHeaderParis(_rlp));
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

    if (isHeaderParis(_rlp))
        return spBlockHeader(new BlockHeaderParis(_rlp));

    if (isHeaderShanghai(_rlp))
        return spBlockHeader(new BlockHeaderShanghai(_rlp));

    if (isHeader4844(_rlp))
        return spBlockHeader(new BlockHeader4844(_rlp));

    if (isHeaderPrague(_rlp))
        return spBlockHeader(new BlockHeaderPrague(_rlp));

    throw test::UpwardsException("readBlockHeader(RLP): unknown block type! \n" + dev::asString(_rlp.toBytes()));
}

spBlockHeader readBlockHeader(DataObject const& _filledData)
{
    if (isHeaderLegacy(_filledData))
        return spBlockHeader(new BlockHeaderLegacy(_filledData));

    if (isHeader1559(_filledData))
        return spBlockHeader(new BlockHeader1559(_filledData));

    if (isHeaderParis(_filledData))
        return spBlockHeader(new BlockHeaderParis(_filledData));

    if (isHeaderShanghai(_filledData))
        return spBlockHeader(new BlockHeaderShanghai(_filledData));

    if (isHeader4844(_filledData))
        return spBlockHeader(new BlockHeader4844(_filledData));

    if (isHeaderPrague(_filledData))
        return spBlockHeader(new BlockHeaderPrague(_filledData));

    ETH_ERROR_MESSAGE("readBlockHeader(DATA): unknown block type! \n" + _filledData.asJson());
    return spBlockHeader(new BlockHeaderLegacy(_filledData));
}

bool isBlockPoS(BlockHeader const& _header)
{
    return isBlockExportCurrentRandom(_header);
}

bool isBlockExportCurrentRandom(BlockHeader const& _header)
{
    return _header.type() == BlockType::BlockHeaderParis
           || _header.type() == BlockType::BlockHeaderShanghai
           || _header.type() == BlockType::BlockHeader4844
           || _header.type() == BlockType::BlockHeaderPrague;
}

bool isBlockExportRequestHash(BlockHeader const& _header)
{
    return _header.type() == BlockType::BlockHeaderPrague;
}

bool isBlockExportExcessBlobGas(BlockHeader const& _header)
{
    return _header.type() == BlockType::BlockHeader4844
        || _header.type() == BlockType::BlockHeaderPrague;
}

bool isBlockExportWithdrawals(BlockHeader const& _header)
{
    return _header.type() == BlockType::BlockHeaderShanghai
            || _header.type() == BlockType::BlockHeader4844
            || _header.type() == BlockType::BlockHeaderPrague;
}

bool isBlockExportBasefee(BlockHeader const& _header)
{
    return _header.type() == BlockType::BlockHeader1559
           || _header.type() == BlockType::BlockHeaderParis
           || _header.type() == BlockType::BlockHeaderShanghai
           || _header.type() == BlockType::BlockHeader4844
           || _header.type() == BlockType::BlockHeaderPrague;
}

bool isBlockExportDifficulty(BlockHeader const& _header)
{
    return _header.type() == BlockType::BlockHeaderLegacy
           || _header.type() == BlockType::BlockHeader1559;
}

}  // namespace teststruct
