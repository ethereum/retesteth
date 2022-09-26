#pragma once
#include "../../../basetypes.h"
#include "../../../configs/FORK.h"
#include <libdataobj/DataObject.h>
#include <libdataobj/SPointer.h>


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
    FH32 const& hash() const { return m_hash; }
    FH32 const& logs() const
    {
        static FH32 emptyLogs("0x1dcc4de8dec75d7aab85b567b6ccd41ad312451b948a7413f0a142fd40d49347");
        if (m_log.isEmpty())
            return emptyLogs;
        return m_log;
    }
    spBYTES const& bytesPtr() const { return m_txbytes; }
    spDataObject asDataObject() const;
    std::string const& expectException() const { return m_expectException; }

private:
    StateTestPostResult() {}
    int m_dataInd;
    int m_gasInd;
    int m_valInd;
    spFH32 m_hash;
    spFH32 m_log;
    spBYTES m_txbytes;
    std::string m_expectException;
};

typedef std::vector<StateTestPostResult> StateTestPostResults;

}  // namespace teststruct
}  // namespace test
