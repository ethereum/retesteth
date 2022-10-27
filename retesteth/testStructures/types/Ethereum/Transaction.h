#pragma once
#include "../../basetypes.h"
#include <libdevcore/RLP.h>
#include <libdataobj/DataObject.h>

namespace test
{
namespace teststruct
{

enum class TransactionType
{
    LEGACY,
    ACCESSLIST,
    BASEFEE
};

struct Transaction : GCP_SPointerBase
{
    // Transaction Interface
    virtual TransactionType type() const = 0;
    virtual const spDataObject asDataObject(ExportOrder _order = ExportOrder::Default) const = 0;
    virtual ~Transaction(){ /* all fields are smart pointers */ };

    // Common transaction data
    BYTES const& data() const { return m_data; }
    VALUE const& gasLimit() const { return m_gasLimit; }
    VALUE const& nonce() const { return m_nonce; }
    VALUE const& value() const { return m_value; }
    bool isCreation() const { return m_creation; }
    FH20 const& to() const { assert(!m_creation); return m_to; }
    VALUE const& v() const { return m_v; }
    VALUE const& r() const { return m_r; }
    VALUE const& s() const { return m_s; }

    FH32 const& hash() const { return m_hash; }
    BYTES const& getRawBytes() const { return m_rawRLPdata; }
    dev::RLPStream const& asRLPStream() const { return m_outRlpStream; }

    /// Debug transaction data for retesteth
    std::string const& dataLabel() const { return m_dataLabel; }
    std::string const& dataRawPreview() const { return m_dataRawPreview; }
    void setDataLabel(std::string const& _label) { m_dataLabel = _label; }
    void setDataRawPreview(std::string const& _dataRawPreview) { m_dataRawPreview = _dataRawPreview; }

    /// Debug transaction data for t8ntool wrapper
    void setSecret(VALUE const& _secret) { m_secretKey = spVALUE(_secret.copy()); }
    VALUE const& getSecret() const { return m_secretKey; }
    void setChainID(VALUE const& _chainID);
    VALUE const& getChainID() const { return m_chainID; }

protected:
    // Potected transaction interface
    virtual void fromDataObject(DataObject const&) = 0;
    virtual void fromRLP(dev::RLP const&) = 0;
    virtual void buildVRS() = 0;
    virtual void streamHeader(dev::RLPStream& _stream) const = 0;
    virtual void rebuildRLP() = 0;

protected:
    Transaction();
    spBYTES m_data;
    spVALUE m_gasLimit;
    spVALUE m_nonce;
    spVALUE m_value;
    spFH20 m_to;
    bool m_creation;

    spVALUE m_v;
    spVALUE m_r;
    spVALUE m_s;

    // Variable
    mutable spVALUE m_chainID;

    // Debug
    std::string m_dataRawPreview;  // Attached data raw preview before code compilation
    std::string m_dataLabel;       // Attached data Label from filler

    // Optimization
    spFH32 m_hash;
    dev::RLPStream m_outRlpStream;
    spBYTES m_rawRLPdata;  // raw transaction data without rlp header (for typed transactions)
    spVALUE m_secretKey = spVALUE(new VALUE(0));   // Additional info for t8ntool transaction wrapper
};

typedef GCP_SPointer<Transaction> spTransaction;

}  // namespace teststruct
}  // namespace test
