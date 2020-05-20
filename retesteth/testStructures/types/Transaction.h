#pragma once
#include "../basetypes/BYTES.h"
#include "../basetypes/FH20.h"
#include "../basetypes/FH32.h"
#include "../basetypes/VALUE.h"
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
    BYTES const& data() const { return *m_data.getCPtr(); }
    VALUE const& gasLimit() const { return *m_gasLimit.getCPtr(); }
    VALUE const& gasPrice() const { return *m_gasPrice.getCPtr(); }
    VALUE const& nonce() const { return *m_nonce.getCPtr(); }
    VALUE const& value() const { return *m_value.getCPtr(); }
    FH20 const& to() const { return *m_to.getCPtr(); }

    //FH32 const& secretKey() const { return *m_secretKey.getCPtr(); }
    VALUE const& v() const { return *m_v.getCPtr(); }
    VALUE const& r() const { return *m_r.getCPtr(); }
    VALUE const& s() const { return *m_s.getCPtr(); }

    std::string const getSignedRLP() const;
    DataObject asDataObject() const;

private:
    Transaction() {}
    spBYTES m_data;
    spVALUE m_gasLimit;
    spVALUE m_gasPrice;
    spVALUE m_nonce;
    spVALUE m_value;
    spFH20 m_to;

    void buildVRS(VALUE const& _secret);
    void streamHeader(dev::RLPStream& _stream) const;
    spVALUE m_v;
    spVALUE m_r;
    spVALUE m_s;
};

typedef GCP_SPointer<Transaction> spTransaction;

}  // namespace teststruct
}  // namespace test
