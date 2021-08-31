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
struct TransactionTestInFiller : GCP_SPointerBase
{
    TransactionTestInFiller(spDataObject&);
    string const& testName() const { return m_name; }
    InfoIncomplete const& info() const { return m_info; }
    bool hasInfo() const { return !m_info.isEmpty(); }
    spTransaction const& transaction() const { return m_transaction; }
    string const& getExpectException(FORK const& _net) const
    {
        static string emptyString = string();
        if (m_expectExceptions.count(_net))
            return m_expectExceptions.at(_net);
        return emptyString;
    }

private:
    TransactionTestInFiller() {}

    string m_name;
    spTransaction m_transaction;
    GCP_SPointer<InfoIncomplete> m_info;
    std::map<FORK, std::string> m_expectExceptions;
};

struct TransactionTestFiller
{
    TransactionTestFiller(spDataObject&);
    std::vector<TransactionTestInFiller> const& tests() const { return m_tests; }

private:
    TransactionTestFiller() {}
    std::vector<TransactionTestInFiller> m_tests;
};


}  // namespace teststruct
}  // namespace test
