#pragma once
#include "../../basetypes.h"
#include <libdevcore/RLP.h>
#include <retesteth/dataObject/DataObject.h>
#include <retesteth/testStructures/types/StateTests/Base/AccessList.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
enum class TransactionType
{
    LEGACY,
    ACCESSLIST
};

struct Transaction : GCP_SPointerBase
{
    Transaction(DataObject const&, string const& _dataRawPreview = string(), string const& _dataLabel = string());
    Transaction(BYTES const&);
    Transaction(dev::RLP const&);
    BYTES const& data() const { return m_data.getCContent(); }
    VALUE const& gasLimit() const { return m_gasLimit.getCContent(); }
    VALUE const& gasPrice() const { return m_gasPrice.getCContent(); }
    VALUE const& nonce() const { return m_nonce.getCContent(); }
    VALUE const& value() const { return m_value.getCContent(); }
    bool isCreation() const { return m_creation; }
    FH20 const& to() const
    {
        assert(!m_creation);
        return m_to.getCContent();
    }

    VALUE const& v() const { return m_v.getCContent(); }
    VALUE const& r() const { return m_r.getCContent(); }
    VALUE const& s() const { return m_s.getCContent(); }
    FH32 const& hash() const;
    virtual TransactionType type() const { return TransactionType::LEGACY; }

    BYTES const& getSignedRLP() const;
    dev::RLPStream const& asRLPStream() const;
    virtual DataObject const asDataObject(ExportOrder _order = ExportOrder::Default) const;

    virtual ~Transaction(){};

    /// Debug
    string const& dataLabel() const { return m_dataLabel; }
    string const& dataRawPreview() const { return m_dataRawPreview; }

private:
    virtual void fromRLP(dev::RLP const&);

    spBYTES m_data;
    spVALUE m_gasLimit;
    spVALUE m_gasPrice;
    spVALUE m_nonce;
    spVALUE m_value;
    spFH20 m_to;
    bool m_creation;

    virtual void buildVRS(VALUE const& _secret);
    virtual void streamHeader(dev::RLPStream& _stream) const;
    spVALUE m_v;
    spVALUE m_r;
    spVALUE m_s;

    // Debug
    string m_dataRawPreview;  // Attached data raw preview before code compilation
    string m_dataLabel;       // Attached data Label from filler

protected:
    Transaction() {}
    virtual void fromDataObject(DataObject const&);
    void assignV(spVALUE const);
    void assignR(spVALUE const);
    void assignS(spVALUE const);

    // Optimization
    spFH32 m_hash;
    dev::RLPStream m_outRlpStream;
    spBYTES m_signedRLPdata;
    virtual void rebuildRLP();
};

typedef GCP_SPointer<Transaction> spTransaction;

}  // namespace teststruct
}  // namespace test
