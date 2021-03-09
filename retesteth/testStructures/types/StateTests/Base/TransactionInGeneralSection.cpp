#include "TransactionInGeneralSection.h"
#include <retesteth/EthChecks.h>

using namespace test::teststruct;

TransactionInGeneralSection::TransactionInGeneralSection(
    DataObject const& _tr, size_t _dInd, size_t _gInd, size_t _vInd, string const& _dataRawPreview, string const& _dataLabel)
  : m_dInd(_dInd), m_gInd(_gInd), m_vInd(_vInd), m_executed(false), m_skipped(false)
{
    if (_tr.count("accessList"))
        m_tr = spTransaction(new TransactionAccessList(_tr, _dataRawPreview, _dataLabel));
    else
        m_tr = spTransaction(new Transaction(_tr, _dataRawPreview, _dataLabel));
}
