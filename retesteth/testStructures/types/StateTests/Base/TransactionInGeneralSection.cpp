#include "TransactionInGeneralSection.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>

using namespace test::teststruct;

TransactionInGeneralSection::TransactionInGeneralSection(
    DataObject const& _tr, size_t _dInd, size_t _gInd, size_t _vInd, string const& _dataRawPreview, string const& _dataLabel)
  : m_dataIndexBox(_dInd, test::fto_string(_dInd), _gInd, test::fto_string(_gInd), _vInd, test::fto_string(_vInd)),
    m_executed(false),
    m_skipped(false)
{
    if (_tr.count("feeCap"))
        m_tr = spTransaction(new TransactionBaseFee(_tr, _dataRawPreview, _dataLabel));
    else if (_tr.count("accessList"))
        m_tr = spTransaction(new TransactionAccessList(_tr, _dataRawPreview, _dataLabel));
    else
        m_tr = spTransaction(new Transaction(_tr, _dataRawPreview, _dataLabel));
}

void TransactionInGeneralSection::assignTransactionLabel(string const& _label)
{
    m_tr.getContent().setDataLabel(_label);
}
