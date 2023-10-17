#include "Verification.h"
#include "retesteth/Options.h"
#include <retesteth/Constants.h>
using namespace toolimpl;
using namespace test::debug;
using namespace std;
using namespace dev;
using namespace test;

namespace {

void check_timestamp(BlockHeader const& _header, BlockHeader const& _parent)
{
    if (_parent.timestamp() >= _header.timestamp())
        throw test::UpwardsException("BlockHeader timestamp is less or equal then it's parent block! (" +
                                     _header.timestamp().asDecString() + " <= " + _parent.timestamp().asDecString() + ")");
}

void check_difficultyDelta(ToolChain const& _chain, BlockHeader const& _header, BlockHeader const& _parent)
{
    if (!test::Options::getCurrentConfig().cfgFile().checkDifficulty())
        return;
    // Validate block difficulty delta
    ChainOperationParams params = ChainOperationParams::defaultParams(_chain.toolParams());
    VALUE newDiff = calculateEthashDifficulty(params, _header, _parent);
    if (_header.difficulty() != newDiff)
        throw test::UpwardsException("Invalid difficulty: " + _header.difficulty().asDecString() +
                                     ", retesteth want: " + VALUE(newDiff).asDecString());
}

void check_baseFeeDelta(ToolChain const& _chain, spBlockHeader const& _header, spBlockHeader const& _parent)
{
    if (!test::Options::getCurrentConfig().cfgFile().checkBasefee())
        return;
    // Check if the base fee is correct
    ChainOperationParams params = ChainOperationParams::defaultParams(_chain.toolParams());
    VALUE newBaseFee = calculateEIP1559BaseFee(params, _header, _parent);
    BlockHeader1559 const& header = BlockHeader1559::castFrom(_header);
    if (header.baseFee() != newBaseFee)
        throw test::UpwardsException() << "Invalid block: Error in field: baseFeePerGas! Expected: `" + newBaseFee.asDecString() +
                                              "`, got: `" + header.baseFee().asDecString() + "`";
}

void check_difficultyMin(BlockHeader const& _header)
{
    if (_header.difficulty() < dev::bigint("0x20000"))
        throw test::UpwardsException("Invalid difficulty: header.difficulty < 0x20000");
}

void check_blockType(BlockType const& _t, BlockType const& _expected, string const& _error)
{
    if (_t != _expected)
        ETH_FAIL_MESSAGE(_error + " got block of another type!");
}

void check_gasUsed(BlockHeader const& _header, string const& _error)
{
    if (_header.gasUsed() > _header.gasLimit())
        throw test::UpwardsException() << _error + " Invalid gasUsed: header.gasUsed > header.gasLimit!";
}

}

namespace
{
void verifyLegacyBlock(spBlockHeader const& _header, ToolChain const& _chain)
{
    (void)_chain;
    check_difficultyMin(_header);
}

void verify1559Block(spBlockHeader const& _header, ToolChain const& _chain)
{
    check_difficultyMin(_header);
    check_blockType(_header->type(), BlockType::BlockHeader1559, "verify1559Block");
    check_gasUsed(_header, "Invalid block1559:");

    BlockHeader1559 const& header = BlockHeader1559::castFrom(_header);
    if (header.number() == 5 && _chain.fork() == "BerlinToLondonAt5")
    {
        // Check first ever EIP1559 gasLimit
        /* https://eips.ethereum.org/EIPS/eip-1559
           if INITIAL_FORK_BLOCK_NUMBER == block.number:
           expected_base_fee_per_gas = INITIAL_BASE_FEE
        */

        if (header.baseFee().asBigInt() != INITIAL_BASE_FEE)
            throw test::UpwardsException(
                "Invalid block1559: Initial baseFee must be 1000000000, got: " + header.baseFee().asDecString());
    }

    if (_chain.fork() == "ArrowGlacierToMergeAtDiffC0000")
    {
        bool isTTDDefined = _chain.params()->params().count("terminalTotalDifficulty");
        if (!isTTDDefined)
            throw test::UpwardsException("terminalTotalDifficulty is not defined in chain params: \n" + _chain.params()->params().asJson());

        VALUE const TTD (_chain.params()->params().atKey("terminalTotalDifficulty"));
        if (_chain.lastBlock().totalDifficulty() >=  TTD)
            throw test::UpwardsException() << "Invalid block1559: Chain switched to PoS!";
    }
}

void verifyCommonMergeRules(spBlockHeader const& _header, string const& _msg)
{
    BlockHeaderMerge const& header = BlockHeaderMerge::castFrom(_header);

    /// Verify rules
    /// https://eips.ethereum.org/EIPS/eip-3675
    if (header.uncleHash().asString() != C_EMPTY_LIST_HASH)
        throw test::UpwardsException(_msg + " block.uncleHash != empty \n" + header.asDataObject()->asJson());
    if (header.difficulty() != 0)
        throw test::UpwardsException(_msg + " block.difficulty must be 0! ");
    if (header.nonce().asString() != "0x0000000000000000")
        throw test::UpwardsException(_msg + " block nonce != 0x00..00 \n" + header.asDataObject()->asJson());
    if ((header.extraData().asString().size()-2) / 2 > MAX_EXTRADATA_SIZE_IN_MERGE)
        throw test::UpwardsException(_msg + " block extraDataSize > 32bytes \n" + header.asDataObject()->asJson());
}

void verifyShanghaiBlock(spBlockHeader const& _header, ToolChain const& _chain)
{
    (void)_chain;
    check_blockType(_header->type(), BlockType::BlockHeaderShanghai, "verifyShanghaiBlock");

    //BlockHeaderShanghai const& header = BlockHeaderShanghai::castFrom(_header);
    verifyCommonMergeRules(_header, "Shanghai");

    /// Verify shanghai rules
    // Withdrawals root is just a hash. no changes
}

void verify4844Block(spBlockHeader const& _header, ToolChain const& _chain)
{
    (void)_chain;
    check_blockType(_header->type(), BlockType::BlockHeader4844, "verify4844Block");

    //BlockHeaderShanghai const& header = BlockHeaderShanghai::castFrom(_header);
    verifyCommonMergeRules(_header, "4844");

    /// Verify 4844 rules
}

void verifyMergeBlock(spBlockHeader const& _header, ToolChain const& _chain)
{
    (void)_chain;
    check_blockType(_header->type(), BlockType::BlockHeaderMerge, "verifyMergeBlock");
    verifyCommonMergeRules(_header, "Merge");
}

void verifyLegacyParent(spBlockHeader const& _header, spBlockHeader const& _parent, ToolChain const& _chain)
{
    check_difficultyDelta(_chain, _header, _parent);

    if (_chain.fork().asString() == "BerlinToLondonAt5" && _parent->number() == 4)
        throw test::UpwardsException("Legacy block import is impossible on BerlinToLondonAt5 after block#4!");

    check_blockType(_header->type(), BlockType::BlockHeaderLegacy, "verifyLegacyBlock");

    if (_parent->type() != BlockType::BlockHeaderLegacy)
        throw test::UpwardsException("Legacy block can only be on top of LegacyBlock!");
}

void verify1559Parent_private(spBlockHeader const& _header, spBlockHeader const& _parent, ToolChain const& _chain)
{
    if (_parent->type() != BlockType::BlockHeader1559)
        throw test::UpwardsException() << "verify1559Parent 1559 block must be on top of 1559 block!";
    check_baseFeeDelta(_chain, _header, _parent);
}

void verify1559Parent(spBlockHeader const& _header, spBlockHeader const& _parent, ToolChain const& _chain)
{
    if (isBlockPoS(_parent))
        throw test::UpwardsException("Trying to import 1559 block on top of PoS block!");

    check_blockType(_header->type(), BlockType::BlockHeader1559, "verify1559Parent");
    check_difficultyDelta(_chain, _header, _parent);

    BlockHeader1559 const& header = BlockHeader1559::castFrom(_header);
    if (header.number() == 5 && _chain.fork() == "BerlinToLondonAt5")
    {
        if (_parent->type() != BlockType::BlockHeaderLegacy)
            ETH_FAIL_MESSAGE("verify1559Parent first 1559 block must be on top of legacy block!");

        spDataObject parentData = _parent->asDataObject();

        // fake legacy block gasLimit for delta validation
        // https://eips.ethereum.org/EIPS/eip-1559
        // if INITIAL_FORK_BLOCK_NUMBER == block.number:
        //            parent_gas_target = self.parent(block).gas_limit
        //            parent_gas_limit = self.parent(block).gas_limit * ELASTICITY_MULTIPLIER
        (*parentData)["gasLimit"] = (_parent->gasLimit() * ELASTICITY_MULTIPLIER).asString();

        // https://eips.ethereum.org/EIPS/eip-1559
        // INITIAL_BASE_FEE = 1000000000
        // fake legacy block baseFee for delta validation
        VALUE genesisBaseFee = INITIAL_BASE_FEE * 8 / 7;
        (*parentData)["baseFeePerGas"] = genesisBaseFee.asString();

        spBlockHeader fixedParent = spBlockHeader(new BlockHeader1559(parentData));
        verify1559Parent_private(_header, fixedParent, _chain);
    }
    else
        verify1559Parent_private(_header, _parent, _chain);
}

void verifyShanghaiParent(spBlockHeader const& _header, spBlockHeader const& _parent, ToolChain const& _chain)
{
    check_blockType(_header->type(), BlockType::BlockHeaderShanghai, "verifyShanghaiParent");
    if (_parent->type() == BlockType::BlockHeaderShanghai)
        verifyShanghaiBlock(_parent, _chain);
    else
    {
        if (_header->timestamp() >= 15000 && _chain.fork() == "MergeToShanghaiAtTime15k")
        {
            if (_parent->type() != BlockType::BlockHeaderMerge)
                throw test::UpwardsException("Trying to import Shanghai block on top of Merge block before transition!!");
        }
        else
            throw test::UpwardsException("Trying to import Shanghai block on top of block that is not Shanghai!!");
    }
}

void verify4844Parent(spBlockHeader const& _header, spBlockHeader const& _parent, ToolChain const& _chain)
{
    check_blockType(_header->type(), BlockType::BlockHeader4844, "verify4844Parent");
    if (_parent->type() == BlockType::BlockHeader4844)
        verify4844Block(_parent, _chain);
    else
    {
        if (_header->timestamp() >= 15000 && _chain.fork() == "ShanghaiToCancunAtTime15k")
        {
            if (_parent->type() != BlockType::BlockHeaderShanghai)
                throw test::UpwardsException("Trying to import Cancun block on top of Shanghai block before transition!!");
        }
        else
            throw test::UpwardsException("Trying to import Cancun block on top of block that is not Cancun!!");
    }
}

void verifyMergeParent(spBlockHeader const& _header, spBlockHeader const& _parent, ToolChain const& _chain, VALUE const& _parentTD)
{
    /// Verify the rules
    /// https://eips.ethereum.org/EIPS/eip-3675

    check_blockType(_header->type(), BlockType::BlockHeaderMerge, "verifyMergeParent");
    bool isTTDDefined = _chain.params()->params().count("terminalTotalDifficulty");
    if (!isTTDDefined)
        ETH_WARNING("terminalTotalDifficulty is not defined in chain params: \n" + _chain.params()->params().asJson());


    check_baseFeeDelta(_chain, _header, _parent);
    if (_parent->type() == BlockType::BlockHeaderShanghai)
        throw test::UpwardsException("Trying to import Merge block on top of Shanghai block after transition!!");

    if (_parent->type() != BlockType::BlockHeaderMerge)
    {
        VALUE const TTD = isTTDDefined ? VALUE(_chain.params()->params().atKey("terminalTotalDifficulty")) : VALUE ("0xffffffffffffffffffffffffffff");
        if (_parentTD < TTD)
            throw test::UpwardsException("Parent (transition) block has not reached TTD (" + _parentTD.asString() +
                                         " < " + TTD.asString() + ") but current block set to PoS format! \nParent: \n" +
                                         _parent->asDataObject()->asJson() + "\nCurrent: " + _header->asDataObject()->asJson());
    }
    if (_parent->type() == BlockType::BlockHeaderMerge)
    {
        ETH_DC_MESSAGE(DC::TESTLOG, "Verifying Merge Block Parent");
        verifyMergeBlock(_parent, _chain);
    }
}

void verifyCommonBlock(spBlockHeader const& _header, ToolChain const& _chain)
{
    BlockHeader const& header = _header.getCContent();
    if (header.extraData().asString().size() > 32 * 2 + 2)
        throw test::UpwardsException("Header extraData > 32 bytes");

    // Check DAO extraData
    if (_chain.fork().asString() == "HomesteadToDaoAt5" && header.number() > 4 && header.number() <= 5 + 9 &&
        header.extraData().asString() != "0x64616f2d686172642d666f726b")
        throw test::UpwardsException("BlockHeader require Dao ExtraData! (0x64616f2d686172642d666f726b)");

    // Check gasLimit
    if (header.gasLimit() > dev::bigint("0x7fffffffffffffff"))
        throw test::UpwardsException("Header gasLimit > 0x7fffffffffffffff");

    check_gasUsed(header, "verifyCommonBlock: ");
}


void verifyCommonParent(spBlockHeader const& _header, spBlockHeader const& _parent, ToolChain const& _chain)
{
    BlockHeader const& header = _header.getCContent();
    BlockHeader const& parent = _parent.getCContent();
    if (header.number() != parent.number() + 1)
        throw test::UpwardsException("BlockHeader number != parent.number + 1 (" + header.number().asDecString() +
                                     " != " + parent.number().asDecString() + ")");

    check_timestamp(_header, _parent);
    bigint parentGasLimit = parent.gasLimit().asBigInt();
    if (header.number() == 5 && _chain.fork() == "BerlinToLondonAt5")
        parentGasLimit = parentGasLimit * ELASTICITY_MULTIPLIER;

    // Verify delta gas (legacy formula)
    VALUE const deltaGas = parent.gasLimit() / VALUE(1024);
    if (header.gasLimit().asBigInt() >= parentGasLimit + deltaGas.asBigInt() ||
        header.gasLimit().asBigInt() <= parentGasLimit - deltaGas.asBigInt())
        throw test::UpwardsException("Invalid gaslimit: " + header.gasLimit().asDecString() + ", want " +
                                     parent.gasLimit().asDecString() + " +/- " + deltaGas.asDecString());
}

void verifyBlockParent(spBlockHeader const& _header, ToolChain const& _chain)
{
    bool found = false;
    for (auto const& parentBlock : _chain.blocks())
    {
        // See if uncles not already in chain
        if (parentBlock.header()->hash() == _header->hash())
            throw test::UpwardsException("Block is already in chain!");
        for (auto const& un : parentBlock.uncles())
        {
            if (un->hash() == _header->hash())
                throw test::UpwardsException("Block is already in chain!");
        }

        if (parentBlock.header()->hash() == _header->parentHash())
        {
            found = true;
            verifyCommonParent(_header, parentBlock.header(), _chain);
            switch (_header->type())
            {
            case BlockType::BlockHeaderLegacy:
                verifyLegacyParent(_header, parentBlock.header(), _chain);
                break;
            case BlockType::BlockHeader1559:
                verify1559Parent(_header, parentBlock.header(), _chain);
                break;
            case BlockType::BlockHeaderMerge:
                verifyMergeParent(_header, parentBlock.header(), _chain, parentBlock.totalDifficulty());
                break;
            case BlockType::BlockHeaderShanghai:
                verifyShanghaiParent(_header, parentBlock.header(), _chain);
                break;
            case BlockType::BlockHeader4844:
                verify4844Parent(_header, parentBlock.header(), _chain);
                break;
            default:
                throw test::UpwardsException("verifyBlockParent::Unhandled block type check!");
            }
        }
    }
    if (!found)
        throw test::UpwardsException(
            "verifyEthereumBlockHeader:: Parent block hash not found: " + _header->parentHash().asString());
}
}  // namespace

namespace toolimpl
{
// Blockchain logic validator
void verifyEthereumBlockHeader(spBlockHeader const& _header, ToolChain const& _chain)
{
    // Check Ethereum rules
    verifyBlockParent(_header, _chain);
    verifyCommonBlock(_header, _chain);
    switch (_header->type())
    {
    case BlockType::BlockHeaderLegacy:
        verifyLegacyBlock(_header, _chain);
        break;
    case BlockType::BlockHeader1559:
        verify1559Block(_header, _chain);
        break;
    case BlockType::BlockHeaderMerge:
        verifyMergeBlock(_header, _chain);
        break;
    case BlockType::BlockHeaderShanghai:
        verifyShanghaiBlock(_header, _chain);
        break;
    case BlockType::BlockHeader4844:
        verify4844Block(_header, _chain);
        break;
    default:
        throw test::UpwardsException("verifyEthereumBlockHeader::Unhandled block type check!");
    }
}

void verifyWithdrawalsRLP(dev::RLP const& _rlp)
{
    if (!_rlp.isList())
        throw dev::RLPException("Withdrawals RLP is expected to be list");
    for (auto const& wt : _rlp)
    {
        if (!wt.isList())
            throw dev::RLPException("Withdrawals RLP is expected to be list");

        for (size_t i = 0; i < 4; i++)
        {
            if (!wt[i].isData())
                throw dev::RLPException("Rlp structure is wrong: Withdrawals RLP field is not data!");
        }
    }
}

void verifyWithdrawalRecord(spWithdrawal const& _wtRecord)
{
    if (_wtRecord->index.getCContent() >= POW2_64)
        throw test::UpwardsException("Withdrawals Index field >= 2**64");
    if (_wtRecord->amount.getCContent() >= POW2_64)
        throw test::UpwardsException("Withdrawals Amount field >= 2**64");
    if (_wtRecord->validatorIndex.getCContent() >= POW2_64)
        throw test::UpwardsException("Withdrawals validatorIndex field >= 2**64");
    auto const& address = _wtRecord->address.getCContent().asString();
    if (address.find("bigint") != string::npos)
        throw test::UpwardsException("Withdrawals address field is not a valid address! " + address);
}

}  // namespace toolimpl
