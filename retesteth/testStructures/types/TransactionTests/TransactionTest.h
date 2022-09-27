#pragma once
#include <libdataobj/DataObject.h>
#include <retesteth/testStructures/configs/FORK.h>
#include <retesteth/testStructures/types/Ethereum/Transaction.h>
#include <map>

namespace test::teststruct
{
struct TransactionResult
{
    TransactionResult(){};
    TransactionResult(spFH32 const& _hash, spFH20 const& _sender, spVALUE const& _intrGas)
      : m_intrinsicGas(_intrGas), m_hash(_hash), m_sender(_sender)
    {}
    std::string m_exception;
    spVALUE m_intrinsicGas;
    spFH32 m_hash;
    spFH20 m_sender;
};


struct TransactionTestInFilled : GCP_SPointerBase
{
    TransactionTestInFilled(spDataObject&);
    std::string const& testName() const { return m_name; }
    BYTES const& rlp() const { return m_rlp; }
    spTransaction const& transaction() const { return m_readTransaction; }
    std::set<FORK> const& allForks() const { return m_forks; }
    std::string const& getExpectException(FORK const& _net) const
    {
        static std::string emptyString = std::string();
        if (m_expectExceptions.count(_net))
            return m_expectExceptions.at(_net);
        return emptyString;
    }
    TransactionResult const& getAcceptedTransaction(FORK const& _net) const
    {
        static TransactionResult empty;
        if (m_acceptedTransactions.count(_net))
            return m_acceptedTransactions.at(_net);
        return empty;
    }

private:
    TransactionTestInFilled() {}

    std::string m_name;
    spBYTES m_rlp;
    spTransaction m_readTransaction;
    std::set<FORK> m_forks;
    std::map<FORK, std::string> m_expectExceptions;

    std::map<FORK, TransactionResult> m_acceptedTransactions;
    std::map<FORK, TransactionResult> m_rejectedTransactions;
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
