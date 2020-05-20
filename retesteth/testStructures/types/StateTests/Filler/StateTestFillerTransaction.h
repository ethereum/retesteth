#pragma once
#include "../../../basetypes/BYTES.h"
#include "../../../basetypes/FH20.h"
#include "../../../basetypes/FH32.h"
#include "../../../basetypes/VALUE.h"
#include "../../Transaction.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{

struct TransactionInGeneralSection;

// Sructure to store JSON data from test
struct StateTestFillerTransaction : GCP_SPointerBase
{
    StateTestFillerTransaction(DataObject const&);
    DataObject asDataObject() const;
    std::vector<TransactionInGeneralSection> buildTransactions() const;

private:
    std::vector<BYTES> m_data;
    std::vector<VALUE> m_gasLimit;
    std::vector<VALUE> m_value;
    GCP_SPointer<VALUE> m_gasPrice;
    GCP_SPointer<VALUE> m_nonce;
    GCP_SPointer<FH32> m_secretKey;
    GCP_SPointer<FH20> m_to;
};

struct TransactionInGeneralSection
{
    TransactionInGeneralSection(spTransaction const&, size_t _dInd, size_t _gInd, size_t _vInd);
    size_t dataInd() const { return m_dInd; }
    size_t gasInd() const { return m_gInd; }
    size_t valueInd() const { return m_vInd; }
    bool getExecuted() const { return m_executed; }
    bool getSkipped() const { return m_skipped; }
    void markExecuted() { m_executed = true; }
    void markSkipped() { m_skipped = true; }
    Transaction const& transaction() const { return *m_tr.getCPtr();}
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
