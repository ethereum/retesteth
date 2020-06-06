#pragma once
#include "../../basetypes.h"
#include <libdevcore/RLP.h>
#include <retesteth/dataObject/DataObject.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
struct Transaction : GCP_SPointerBase
{
    Transaction(DataObject const&);
    BYTES const& data() const { return m_data.getCContent(); }
    VALUE const& gasLimit() const { return m_gasLimit.getCContent(); }
    VALUE const& gasPrice() const { return m_gasPrice.getCContent(); }
    VALUE const& nonce() const { return m_nonce.getCContent(); }
    VALUE const& value() const { return m_value.getCContent(); }
    FH20 const& to() const
    {
        assert(!m_creation);
        return m_to.getCContent();
    }

    VALUE const& v() const { return m_v.getCContent(); }
    VALUE const& r() const { return m_r.getCContent(); }
    VALUE const& s() const { return m_s.getCContent(); }
    FH32 hash() const;

    BYTES const getSignedRLP() const;
    dev::RLPStream const asRLPStream() const;
    DataObject const asDataObject(ExportOrder _order = ExportOrder::Default) const;

    bool operator==(Transaction const& _rhs) const;

private:
    Transaction() {}
    spBYTES m_data;
    spVALUE m_gasLimit;
    spVALUE m_gasPrice;
    spVALUE m_nonce;
    spVALUE m_value;
    spFH20 m_to;
    bool m_creation;

    void buildVRS(VALUE const& _secret);
    void streamHeader(dev::RLPStream& _stream) const;
    spVALUE m_v;
    spVALUE m_r;
    spVALUE m_s;
};

typedef GCP_SPointer<Transaction> spTransaction;

}  // namespace teststruct
}  // namespace test
