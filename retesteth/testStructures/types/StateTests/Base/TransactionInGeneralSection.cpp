#include "TransactionInGeneralSection.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/types/Ethereum/TransactionReader.h>

using namespace std;
using namespace test::teststruct;

TransactionInGeneralSection::TransactionInGeneralSection(
    spDataObjectMove _tr, size_t _dInd, size_t _gInd, size_t _vInd, string const& _dataRawPreview, string const& _dataLabel)
  : m_dataIndexBox(_dInd, test::fto_string(_dInd), _gInd, test::fto_string(_gInd), _vInd, test::fto_string(_vInd)),
    m_executed(false),
    m_skipped(false)
{
    m_tr = readTransaction(_tr);
    (*m_tr).setDataLabel(_dataLabel);
    (*m_tr).setDataRawPreview(_dataRawPreview);
}

void TransactionInGeneralSection::assignTransactionLabel(string const& _label)
{
    m_tr.getContent().setDataLabel(_label);
}
