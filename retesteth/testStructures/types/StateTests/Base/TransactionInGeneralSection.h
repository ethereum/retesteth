#pragma once
#include "../../Ethereum/Transaction.h"
#include "../../Ethereum/TransactionAccessList.h"
#include "../../Ethereum/TransactionBaseFee.h"
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
    TransactionInGeneralSection(
        spDataObjectMove, size_t _dInd, size_t _gInd, size_t _vInd, string const& _dataRawPreview, string const& _dataLabel);
    size_t dataInd() const { return m_dataIndexBox.m_dInd; }
    string const& dataIndS() const { return m_dataIndexBox.m_dIndS; }
    size_t gasInd() const { return m_dataIndexBox.m_gInd; }
    string const& gasIndS() const { return m_dataIndexBox.m_gIndS; }
    size_t valueInd() const { return m_dataIndexBox.m_vInd; }
    string const& valueIndS() const { return m_dataIndexBox.m_vIndS; }
    bool getExecuted() const { return m_executed; }
    bool getSkipped() const { return m_skipped; }
    void markExecuted() { m_executed = true; }
    void markSkipped() { m_skipped = true; }
    spTransaction const& transaction() const { return m_tr; }
    void assignTransactionLabel(string const& _label);

private:
    spTransaction m_tr;
    struct DataIndex
    {
        // Keep the index box size_t and string representations for faster debug
        // Not converting to string every time
        DataIndex(size_t _dind, string const& _dinds, size_t _gind, string const& _ginds, size_t _vind, string const& _vinds)
          : m_dInd(_dind), m_dIndS(_dinds), m_gInd(_gind), m_gIndS(_ginds), m_vInd(_vind), m_vIndS(_vinds)
        {}
        size_t m_dInd;
        string m_dIndS;
        size_t m_gInd;
        string m_gIndS;
        size_t m_vInd;
        string m_vIndS;
    };
    DataIndex m_dataIndexBox;
    bool m_executed;
    bool m_skipped;
};

}  // namespace teststruct
}  // namespace test
