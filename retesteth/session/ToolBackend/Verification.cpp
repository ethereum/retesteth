#include "Verification.h"
using namespace toolimpl;

namespace
{
void verifyLegacyBlock(spBlockHeader const& _header, ToolChain const& _chain)
{
    (void)_chain;
    BlockHeader const& header = _header.getCContent();
    if (header.difficulty() < dev::bigint("0x20000"))
        throw test::UpwardsException("Invalid difficulty: header.difficulty < 0x20000");
}

void verify1559Block(spBlockHeader const& _header, ToolChain const& _chain)
{
    (void)_chain;
    BlockHeader const& headerLegacy = _header.getCContent();
    if (headerLegacy.difficulty() < dev::bigint("0x20000"))
        throw test::UpwardsException("Invalid difficulty: header.difficulty < 0x20000");

    if (_header->type() != BlockType::BlockHeader1559)
        ETH_FAIL_MESSAGE("verify1559Block got block of another type!");
    BlockHeader1559 const& header = BlockHeader1559::castFrom(_header);

    // Check if the block used too much gas
    if (header.gasUsed() > header.gasLimit())
        throw test::UpwardsException() << "Invalid block1559: Invalid gasUsed: too much gas used!";

    // Check first ever EIP1559 gasLimit
    if (header.number() == 5 && _chain.fork() == "BerlinToLondonAt5")
    {
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

        VALUE const TTD = _chain.params()->params().atKey("terminalTotalDifficulty");
        if (_chain.lastBlock().totalDifficulty() >=  TTD)
            throw test::UpwardsException() << "Invalid block1559: Chain switched to PoS!";
    }
}

void verifyMergeBlock(spBlockHeader const& _header, ToolChain const& _chain)
{
    (void)_chain;
    if (_header->type() != BlockType::BlockHeaderMerge)
        ETH_FAIL_MESSAGE("verifyMergeBlock got block of another type!");
    BlockHeaderMerge const& header = BlockHeaderMerge::castFrom(_header);

    /// Verify rules
    /// https://eips.ethereum.org/EIPS/eip-3675
    if (header.uncleHash().asString() != "0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347")
        throw test::UpwardsException("Merge block.uncleHash != empty \n" + header.asDataObject()->asJson());
    if (header.difficulty() != 0)
        throw test::UpwardsException("Merge block.difficulty must be 0! ");
    if (header.nonce().asString() != "0x0000000000000000")
        throw test::UpwardsException("Merge block nonce != 0x00..00 \n" + header.asDataObject()->asJson());
    if ((header.extraData().asString().size()-2) / 2 > MAX_EXTRADATA_SIZE_IN_MERGE)
        throw test::UpwardsException("Merge block extraDataSize > 32bytes \n" + header.asDataObject()->asJson());
}

void verifyLegacyParent(spBlockHeader const& _header, spBlockHeader const& _parent, ToolChain const& _chain)
{
    // Validate block difficulty delta
    ChainOperationParams params = ChainOperationParams::defaultParams(_chain.toolParams());
    VALUE newDiff = calculateEthashDifficulty(params, _header, _parent);
    if (_header.getCContent().difficulty() != newDiff)
        throw test::UpwardsException("Invalid difficulty: " + _header.getCContent().difficulty().asDecString() +
                                     ", retesteth want: " + VALUE(newDiff).asDecString());

    if (_chain.fork().asString() == "BerlinToLondonAt5" && _parent->number() == 4)
        throw test::UpwardsException("Legacy block import is impossible on BerlinToLondonAt5 after block#4!");

    if (_header->type() != BlockType::BlockHeaderLegacy)
        ETH_FAIL_MESSAGE("verifyLegacyBlock got block of another type!");

    if (_parent->type() != BlockType::BlockHeaderLegacy)
        throw test::UpwardsException("Legacy block can only be on top of LegacyBlock!");
}

void verify1559Parent_private(spBlockHeader const& _header, spBlockHeader const& _parent, ToolChain const& _chain)
{
    BlockHeader1559 const& header = BlockHeader1559::castFrom(_header);
    if (_parent->type() != BlockType::BlockHeader1559)
        throw test::UpwardsException() << "verify1559Parent 1559 block must be on top of 1559 block!";
    BlockHeader1559 const& parent = BlockHeader1559::castFrom(_parent);

    // Check if the base fee is correct
    ChainOperationParams params = ChainOperationParams::defaultParams(_chain.toolParams());
    VALUE newBaseFee = calculateEIP1559BaseFee(params, _header, _parent);
    if (header.baseFee() != newBaseFee)
        throw test::UpwardsException() << "Invalid block1559: base fee not correct! Expected: `" + newBaseFee.asDecString() +
                                              "`, got: `" + header.baseFee().asDecString() + "`";

    (void) parent;
}

void verify1559Parent(spBlockHeader const& _header, spBlockHeader const& _parent, ToolChain const& _chain)
{
    if (_parent->type() == BlockType::BlockHeaderMerge)
        throw test::UpwardsException("Trying to import 1559 block on top of PoS block!");

    // Validate block difficulty delta
    ChainOperationParams params = ChainOperationParams::defaultParams(_chain.toolParams());
    VALUE newDiff = calculateEthashDifficulty(params, _header, _parent);
    if (_header.getCContent().difficulty() != newDiff)
        throw test::UpwardsException("Invalid difficulty: " + _header.getCContent().difficulty().asDecString() +
                                     ", retesteth want: " + VALUE(newDiff).asDecString());

    if (_header->type() != BlockType::BlockHeader1559)
        ETH_FAIL_MESSAGE("verify1559Parent got block of another type!");

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

void verifyMergeParent(spBlockHeader const& _header, spBlockHeader const& _parent, ToolChain const& _chain, VALUE const& _parentTD)
{
    /// Verify the rules
    /// https://eips.ethereum.org/EIPS/eip-3675

    if (_header->type() != BlockType::BlockHeaderMerge)
        ETH_FAIL_MESSAGE("verifyMergeParent got block of another type!");
    bool isTTDDefined = _chain.params()->params().count("terminalTotalDifficulty");
    if (!isTTDDefined)
        ETH_WARNING("terminalTotalDifficulty is not defined in chain params: \n" + _chain.params()->params().asJson());


    if (_parent->type() != BlockType::BlockHeaderMerge)
    {
        VALUE const TTD = isTTDDefined ? _chain.params()->params().atKey("terminalTotalDifficulty") : VALUE (DataObject("0xffffffffffffffffffffffffffff"));
        if (_parentTD < TTD)
            throw test::UpwardsException("Parent (transition) block has not reached TTD (" + _parentTD.asString() +
                                         " < " + TTD.asString() + ") but current block set to PoS format! \nParent: \n" +
                                         _parent->asDataObject()->asJson() + "\nCurrent: " + _header->asDataObject()->asJson());

        // Check if the base fee is correct
        ChainOperationParams params = ChainOperationParams::defaultParams(_chain.toolParams());
        VALUE newBaseFee = calculateEIP1559BaseFee(params, _header, _parent);
        BlockHeader1559 const& header = BlockHeader1559::castFrom(_header);
        if (header.baseFee() != newBaseFee)
            throw test::UpwardsException() << "Invalid blockMerge: base fee not correct! Expected: `" + newBaseFee.asDecString() +
                                                  "`, got: `" + header.baseFee().asDecString() + "`";
    }
    if (_parent->type() == BlockType::BlockHeaderMerge)
    {
        ETH_TEST_MESSAGE("Verifying Merge Block Parent");

        // Check if the base fee is correct
        ChainOperationParams params = ChainOperationParams::defaultParams(_chain.toolParams());
        VALUE newBaseFee = calculateEIP1559BaseFee(params, _header, _parent);
        BlockHeaderMerge const& header = BlockHeaderMerge::castFrom(_header);
        if (header.baseFee() != newBaseFee)
            throw test::UpwardsException() << "Invalid blockMerge: base fee not correct! Expected: `" + newBaseFee.asDecString() +
                                                  "`, got: `" + header.baseFee().asDecString() + "`";

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
    if (header.gasUsed() > header.gasLimit())
        throw test::UpwardsException("Invalid gasUsed: header.gasUsed > header.gasLimit");
}


void verifyCommonParent(spBlockHeader const& _header, spBlockHeader const& _parent, ToolChain const& _chain)
{
    BlockHeader const& header = _header.getCContent();
    BlockHeader const& parent = _parent.getCContent();
    if (header.number() != parent.number() + 1)
        throw test::UpwardsException("BlockHeader number != parent.number + 1 (" + header.number().asDecString() +
                                     " != " + parent.number().asDecString() + ")");

    if (parent.timestamp() >= header.timestamp())
        throw test::UpwardsException("BlockHeader timestamp is less or equal then it's parent block! (" +
            header.timestamp().asDecString() + " <= " + parent.timestamp().asDecString() + ")");

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
            default:
                throw test::UpwardsException("Unhandled block type check!");
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
    default:
        throw test::UpwardsException("Unhandled block type check!");
    }
}

}  // namespace toolimpl
