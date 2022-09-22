#pragma once
#include "../../Ethereum/Transaction.h"
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>
using namespace dataobject;
namespace test
{
namespace teststruct
{
// NonceMap;
typedef std::map<string, spVALUE> NonceMap;

struct BlockchainTestFillerTransaction : GCP_SPointerBase
{
    BlockchainTestFillerTransaction(spDataObjectMove, NonceMap& _nonceMap);
    Transaction const& tr() const { return m_transaction; }
    spTransaction const& trPointer() const { return m_transaction; }

    // Test functions
    string const& getExpectException(FORK const& _net) const
    {
        static string emptyString = string();  // mutex ??
        if (m_expectExceptions.count(_net))
            return m_expectExceptions.at(_net);
        return emptyString;
    }

private:
    BlockchainTestFillerTransaction() {}
    spTransaction m_transaction;
    std::map<FORK, string> m_expectExceptions;
};

}  // namespace teststruct
}  // namespace test
