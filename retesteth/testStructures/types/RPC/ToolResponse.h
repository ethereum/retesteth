#pragma once
#include "../../basetypes.h"
#include "../Ethereum/StateIncomplete.h"
#include <retesteth/dataObject/DataObject.h>

using namespace dataobject;

namespace test
{
namespace teststruct
{
struct ToolResponse
{
    ToolResponse(DataObject const& _data);
    FH32 const& stateRoot() const { return m_stateRoot.getCContent(); }
    FH32 const& txRoot() const { return m_txRoot.getCContent(); }
    FH32 const& receiptRoot() const { return m_receiptRoot.getCContent(); }
    FH32 const& logsHash() const { return m_logsHash.getCContent(); }
    StateIncomplete const& state() const { return m_stateResponse.getCContent(); }

    // Tool export the state separately
    void attachState(StateIncomplete const& _state) { m_stateResponse = spStateIncomplete(new StateIncomplete(_state)); }

private:
    ToolResponse() {}
    spFH32 m_stateRoot;
    spFH32 m_txRoot;
    spFH32 m_receiptRoot;
    spFH32 m_logsHash;
    // Array of receipts

    spStateIncomplete m_stateResponse;
};

}  // namespace teststruct
}  // namespace test
