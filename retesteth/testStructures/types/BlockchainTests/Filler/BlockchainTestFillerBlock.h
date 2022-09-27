#pragma once
#include "../../../basetypes.h"
#include "../../../configs/FORK.h"
#include "../../Ethereum/Transaction.h"
#include "BlockchainTestFillerBlockHeaderOverwrite.h"
#include "BlockchainTestFillerTransaction.h"
#include "BlockchainTestFillerUncle.h"
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{
// BlockchainTestFiller block instructions
struct BlockchainTestFillerBlock : GCP_SPointerBase
{
    BlockchainTestFillerBlock(spDataObject&, NonceMap&);

    // Block can be represented as raw RLP without any of other fields
    // other then BlockHeader with expected exceptions
    bool isRawRLP() const { return !m_rlp.isEmpty(); }
    BYTES const& rawRLP() const { return m_rlp; }

    // Block can have chainName explicitly defined
    std::string const& chainName() const { return m_chainName; }

    // Block can have chainNext explicitly defined to generate blocks on different networks
    bool hasChainNet() const { return !m_network.isEmpty(); }
    FORK const& chainNet() const { return m_network; }

    // Block can be marked with number. If number sequence changes, a fork chain can be created
    bool hasNumber() const { return !m_blockNumber.isEmpty(); }
    VALUE const& number() const { return m_blockNumber; }

    // BlockHeader overwrite section to replace some fields in the header for testing purposes
    bool hasBlockHeaderOverwrite(FORK const& _fork) const { return m_overwriteHeaderByForkMap.count(_fork); }
    BlockHeaderOverwrite const& getHeaderOverwrite(FORK const& _fork) const
    {
        return m_overwriteHeaderByForkMap.at(_fork);
    }

    // Transaction in block filler. Can be marked invalid (expected to fail)
    std::vector<BlockchainTestFillerTransaction> const& transactions() const { return m_transactions; }
    size_t invalidTransactionCount(FORK const& _net) const
    {
        size_t count = 0;
        for (auto const& el : m_transactions)
            count += el.getExpectException(_net).empty() ? 0 : 1;
        return count;
    }

    // Uncle generate instruction section
    std::vector<BlockchainTestFillerUncle> const& uncles() const { return m_uncles; }

    // Test Functions
    // Block can have exceptions expected to thrown by the client upon generation of the block
    std::string const& getExpectException(FORK const& _net) const
    {
        static std::string emptyString = std::string();  // mutex ??
        if (m_expectExceptions.count(_net))
            return m_expectExceptions.at(_net);
        return emptyString;
    }

    static std::string const& defaultChainName()
    {
        // Mutex lock static defenition?
        static std::string defaultChainName = "default";
        return defaultChainName;
    }

    bool isDoNotImportOnClient() const { return m_doNotImportOnClient; }

private:
    BlockchainTestFillerBlock() {}
    std::string m_chainName;
    spBYTES m_rlp;
    spVALUE m_blockNumber;
    spFORK m_network;
    bool m_doNotImportOnClient = false;

    std::vector<BlockchainTestFillerUncle> m_uncles;
    std::vector<BlockchainTestFillerTransaction> m_transactions;
    std::map<FORK, std::string> m_expectExceptions;

    std::map<FORK, spBlockHeaderOverwrite> m_overwriteHeaderByForkMap;
};

}  // namespace teststruct
}  // namespace test
