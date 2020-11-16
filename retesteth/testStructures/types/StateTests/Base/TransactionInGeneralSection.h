#pragma once
#include "../../Ethereum/Transaction.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>

using namespace dataobject;
using namespace test::teststruct;
namespace test
{
namespace teststruct
{
// Single transaction construct from GeneralTransaction section in state tests
// State tests GeneralTransaction section define multiple transactions
// This is a single transaction representation
struct TransactionInGeneralSection
{
    TransactionInGeneralSection(DataObject const&, size_t _dInd, size_t _gInd, size_t _vInd, string const& _dataLabel);
    size_t dataInd() const { return m_dInd; }
    size_t gasInd() const { return m_gInd; }
    size_t valueInd() const { return m_vInd; }
    bool getExecuted() const { return m_executed; }
    bool getSkipped() const { return m_skipped; }
    void markExecuted() { m_executed = true; }
    void markSkipped() { m_skipped = true; }
    Transaction const& transaction() const { return m_tr.getCContent(); }

private:
    spTransaction m_tr;
    size_t m_dInd;
    size_t m_gInd;
    size_t m_vInd;
    bool m_executed;
    bool m_skipped;
};

}  // namespace teststruct
}  // namespace test
