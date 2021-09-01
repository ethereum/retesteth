#pragma once
#include "../StateTests/Filler/InfoIncomplete.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
#include <retesteth/testStructures/configs/FORK.h>
#include <retesteth/testStructures/types/Ethereum/Transaction.h>
#include <map>

namespace test
{
namespace teststruct
{
struct TransactionTestInFilled : GCP_SPointerBase
{
    TransactionTestInFilled(spDataObject&);
    string const& testName() const { return m_name; }
    BYTES const& rlp() const { return m_rlp; }
    spTransaction const& transaction() const { return m_readTransaction; }
    string const& getExpectException(FORK const& _net) const
    {
        static string emptyString = string();
        if (m_expectExceptions.count(_net))
            return m_expectExceptions.at(_net);
        return emptyString;
    }
    tuple<spFH32, spFH20> const& getAcceptedTransaction(FORK const& _net) const
    {
        static HashSender empty = {0, 0};
        if (m_acceptedTransactions.count(_net))
            return m_acceptedTransactions.at(_net);
        return empty;
    }

private:
    TransactionTestInFilled() {}

    string m_name;
    spBYTES m_rlp;
    spTransaction m_readTransaction;
    std::map<FORK, std::string> m_expectExceptions;

    typedef tuple<spFH32, spFH20> HashSender;
    std::map<FORK, HashSender> m_acceptedTransactions;
};

struct TransactionTest
{
    TransactionTest(spDataObject&);
    std::vector<TransactionTestInFilled> const& tests() const { return m_tests; }

private:
    TransactionTest() {}
    std::vector<TransactionTestInFilled> m_tests;
};


}  // namespace teststruct
}  // namespace test
