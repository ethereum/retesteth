#pragma once
#include "../../Ethereum/Transaction.h"
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
// Single transaction construct from GeneralTransaction section in state tests
// State tests GeneralTransaction section define multiple transactions
// This is a single transaction representation
struct TransactionInGeneralSection
{
    TransactionInGeneralSection(spDataObjectMove, size_t _dInd, size_t _gInd, size_t _vInd, std::string const& _dataRawPreview,
        std::string const& _dataLabel);
    size_t dataInd() const { return m_dataIndexBox.m_dInd; }
    std::string const& dataIndS() const { return m_dataIndexBox.m_dIndS; }
    size_t gasInd() const { return m_dataIndexBox.m_gInd; }
    std::string const& gasIndS() const { return m_dataIndexBox.m_gIndS; }
    size_t valueInd() const { return m_dataIndexBox.m_vInd; }
    std::string const& valueIndS() const { return m_dataIndexBox.m_vIndS; }
    bool getExecuted() const { return m_executed; }
    bool getSkipped() const { return m_skipped; }
    void markExecuted() { m_executed = true; }
    void markSkipped() { m_skipped = true; }
    spTransaction const& transaction() const { return m_tr; }
    void assignTransactionLabel(std::string const& _label);

private:
    spTransaction m_tr;
    struct DataIndex
    {
        // Keep the index box size_t and string representations for faster debug
        // Not converting to string every time
        DataIndex(size_t _dind, std::string const& _dinds, size_t _gind, std::string const& _ginds, size_t _vind,
            std::string const& _vinds)
          : m_dInd(_dind), m_dIndS(_dinds), m_gInd(_gind), m_gIndS(_ginds), m_vInd(_vind), m_vIndS(_vinds)
        {}
        size_t m_dInd;
        std::string m_dIndS;
        size_t m_gInd;
        std::string m_gIndS;
        size_t m_vInd;
        std::string m_vIndS;
    };
    DataIndex m_dataIndexBox;
    bool m_executed;
    bool m_skipped;
};

}  // namespace teststruct
