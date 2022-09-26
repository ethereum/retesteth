#pragma once
#include "../../Ethereum/Transaction.h"
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>
namespace test
{
namespace teststruct
{
// NonceMap;
typedef std::map<std::string, spVALUE> NonceMap;

struct BlockchainTestFillerTransaction : GCP_SPointerBase
{
    BlockchainTestFillerTransaction(spDataObjectMove, NonceMap& _nonceMap);
    Transaction const& tr() const { return m_transaction; }
    spTransaction const& trPointer() const { return m_transaction; }

    // Test functions
    std::string const& getExpectException(FORK const& _net) const
    {
        static std::string emptyString = std::string();  // mutex ??
        if (m_expectExceptions.count(_net))
            return m_expectExceptions.at(_net);
        return emptyString;
    }

private:
    BlockchainTestFillerTransaction() {}
    spTransaction m_transaction;
    std::map<FORK, std::string> m_expectExceptions;
};

}  // namespace teststruct
}  // namespace test
