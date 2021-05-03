#include "Verification.h"
using namespace toolimpl;

namespace
{
void verifyLegacyBlock(spBlockHeader const& _header, ToolChain const& _chain)
{
    (void)_chain;
    if (_header.getCContent().type() != BlockType::BlockHeaderLegacy)
        ETH_FAIL_MESSAGE("verifyLegacyBlock got block of another type!");
    BlockHeaderLegacy const& header = BlockHeaderLegacy::castFrom(_header);

    if (header.gasLimit() > dev::u256("0x7fffffffffffffff"))
        throw test::UpwardsException("Header gasLimit > 0x7fffffffffffffff");
    if (header.gasUsed() > header.gasLimit())
        throw test::UpwardsException("Invalid gasUsed: header.gasUsed > header.gasLimit");
}

void verifyLegacyParent(spBlockHeader const& _header, spBlockHeader const& _parent)
{
    if (_header.getCContent().type() != BlockType::BlockHeaderLegacy)
        ETH_FAIL_MESSAGE("verifyLegacyBlock got block of another type!");

    if (_parent.getCContent().type() != BlockType::BlockHeaderLegacy)
        throw test::UpwardsException("Legacy block can only be on top of LegacyBlock!");

    BlockHeaderLegacy const& header = BlockHeaderLegacy::castFrom(_header);
    BlockHeaderLegacy const& parent = BlockHeaderLegacy::castFrom(_parent);
    VALUE deltaGas = parent.gasLimit().asU256() / 1024;
    if (header.gasLimit().asU256() >= parent.gasLimit().asU256() + deltaGas.asU256() ||
        header.gasLimit().asU256() <= parent.gasLimit().asU256() - deltaGas.asU256())
        throw test::UpwardsException("Invalid gaslimit: " + header.gasLimit().asDecString() + ", want " +
                                     parent.gasLimit().asDecString() + " +/- " + deltaGas.asDecString());
}

void verify1559Block(spBlockHeader const& _header, ToolChain const& _chain)
{
    (void)_chain;
    if (_header.getCContent().type() != BlockType::BlockHeader1559)
        ETH_FAIL_MESSAGE("verify1559Block got block of another type!");
    BlockHeader1559 const& header = BlockHeader1559::castFrom(_header);

    const size_t ELASTICITY_MULTIPLIER = 2;

    // Check if the block used too much gas
    if (header.gasUsed().asU256() > header.gasTarget().asU256() * ELASTICITY_MULTIPLIER)
        throw test::UpwardsException() << "Invalid block1559: too much gas used!";
}

void verify1559Parent(spBlockHeader const& _header, spBlockHeader const& _parent, ToolChain const& _chain)
{
    if (_header.getCContent().type() != BlockType::BlockHeader1559)
        ETH_FAIL_MESSAGE("verify1559Block got block of another type!");

    BlockHeader1559 const& header = BlockHeader1559::castFrom(_header);
    BlockHeader1559 const& parent = BlockHeader1559::castFrom(_parent);

    // Check if the block changed the gas target too much
    VALUE deltaGasT = parent.gasTarget().asU256() / 1024;
    if (header.gasTarget().asU256() > parent.gasTarget().asU256() + deltaGasT.asU256())
        throw test::UpwardsException() << "Invalid block1559: gasTarget increased too much!";
    if (header.gasTarget().asU256() < parent.gasTarget().asU256() - deltaGasT.asU256())
        throw test::UpwardsException() << "Invalid block1559: gasTarget decreased too much!";

    // Check if the base fee is correct
    ChainOperationParams params = ChainOperationParams::defaultParams(_chain.toolParams());
    VALUE newBaseFee = calculateEIP1559BaseFee(params, _header, _parent);
    if (header.baseFee() != newBaseFee)
        throw test::UpwardsException() << "Invalid block1559: base fee not correct! Expected: `" + newBaseFee.asDecString() +
                                              "`, got: `" + header.baseFee().asDecString() + "`";
}

void verifyCommonBlock(spBlockHeader const& _header, ToolChain const& _chain)
{
    BlockHeader const& header = _header.getCContent();
    if (header.difficulty() < dev::u256("0x20000"))
        throw test::UpwardsException("Invalid difficulty: header.difficulty < 0x20000");
    if (header.extraData().asString().size() > 32 * 2 + 2)
        throw test::UpwardsException("Header extraData > 32 bytes");

    // Check DAO extraData
    if (_chain.fork().asString() == "HomesteadToDaoAt5" && header.number() > 4 && header.number() <= 5 + 9 &&
        header.extraData().asString() != "0x64616f2d686172642d666f726b")
        throw test::UpwardsException("BlockHeader require Dao ExtraData! (0x64616f2d686172642d666f726b)");
}


void verifyCommonParent(spBlockHeader const& _header, spBlockHeader const& _parent, ToolChain const& _chain)
{
    BlockHeader const& header = _header.getCContent();
    BlockHeader const& parent = _parent.getCContent();
    if (header.number() != parent.number() + 1)
        throw test::UpwardsException("BlockHeader number != parent.number + 1 (" + header.number().asDecString() +
                                     " != " + parent.number().asDecString() + ")");

    if (parent.timestamp() >= header.timestamp())
        throw test::UpwardsException("BlockHeader timestamp is less then it's parent block!");

    // Validate block difficulty delta
    ChainOperationParams params = ChainOperationParams::defaultParams(_chain.toolParams());
    u256 newDiff = calculateEthashDifficulty(params, _header, _parent);
    if (header.difficulty().asU256() != newDiff)
        throw test::UpwardsException(
            "Invalid difficulty: " + header.difficulty().asDecString() + ", want: " + VALUE(newDiff).asDecString());
}

}  // namespace

namespace toolimpl
{
// Blockchain logic validator
void verifyEthereumBlockHeader(spBlockHeader const& _header, ToolChain const& _chain)
{
    // Check Ethereum rules
    verifyCommonBlock(_header, _chain);
    switch (_header.getCContent().type())
    {
    case BlockType::BlockHeaderLegacy:
        verifyLegacyBlock(_header, _chain);
        break;
    case BlockType::BlockHeader1559:
        verify1559Block(_header, _chain);
        break;
    default:
        throw test::UpwardsException("Unhandled block type check!");
    }

    bool found = false;
    for (auto const& parentBlock : _chain.blocks())
    {
        // See if uncles not already in chain
        if (parentBlock.header().getCContent().hash() == _header.getCContent().hash())
            throw test::UpwardsException("Block is already in chain!");
        for (auto const& un : parentBlock.uncles())
            if (un.getCContent().hash() == _header.getCContent().hash())
                throw test::UpwardsException("Block is already in chain!");

        if (parentBlock.header().getCContent().hash() == _header.getCContent().parentHash())
        {
            found = true;
            verifyCommonParent(_header, parentBlock.header(), _chain);
            switch (_header.getCContent().type())
            {
            case BlockType::BlockHeaderLegacy:
                verifyLegacyParent(_header, parentBlock.header());
                break;
            case BlockType::BlockHeader1559:
                verify1559Parent(_header, parentBlock.header(), _chain);
                break;
            default:
                throw test::UpwardsException("Unhandled block type check!");
            }
            break;
        }
    }
    if (!found)
        throw test::UpwardsException(
            "verifyEthereumBlockHeader:: Parent block hash not found: " + _header.getCContent().parentHash().asString());
}

}  // namespace toolimpl
