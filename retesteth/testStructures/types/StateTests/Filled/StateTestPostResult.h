#pragma once
#include "../../../basetypes.h"
#include "../../../configs/FORK.h"
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/dataObject/SPointer.h>
using namespace dataobject;

namespace test
{
namespace teststruct
{
struct StateTestPostResult : GCP_SPointerBase
{
    StateTestPostResult(DataObject const&);
    bool checkIndexes(size_t _dataInd, size_t _gasInd, size_t _valInd) const
    {
        return m_dataInd == (int)_dataInd && m_gasInd == (int)_gasInd && m_valInd == (int)_valInd;
    }
    FH32 const& hash() const { return m_hash.getCContent(); }
    FH32 const& logs() const { return m_log.getCContent(); }
    spBYTES const& bytesPtr() const { return m_txbytes; }
    DataObject const asDataObject() const;

private:
    StateTestPostResult() {}
    int m_dataInd;
    int m_gasInd;
    int m_valInd;
    spFH32 m_hash;
    spFH32 m_log;
    spBYTES m_txbytes;
};

typedef std::vector<StateTestPostResult> StateTestPostResults;

}  // namespace teststruct
}  // namespace test
