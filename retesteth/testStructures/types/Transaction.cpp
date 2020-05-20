#include "Transaction.h"
#include <libdevcore/Address.h>
#include <libdevcore/SHA3.h>
#include <libdevcrypto/Common.h>
#include <retesteth/EthChecks.h>

namespace test
{
namespace teststruct
{

Transaction::Transaction(DataObject const& _data)
{
    m_data = spBYTES(new BYTES(_data.atKey("data").asString()));
    m_gasLimit = spVALUE(new VALUE(_data.atKey("gasLimit")));
    m_gasPrice = spVALUE(new VALUE(_data.atKey("gasPrice")));
    m_nonce = spVALUE(new VALUE(_data.atKey("nonce")));
    m_to = spFH20(new FH20(_data.atKey("to")));
    m_value = spVALUE(new VALUE(_data.atKey("value")));

    if (_data.count("secretKey"))
        buildVRS(VALUE(_data.atKey("secretKey")));
    else
    {
        m_v = spVALUE(new VALUE(_data.atKey("v"), dev::u256("0xff")));
        m_r = spVALUE(new VALUE(_data.atKey("r")));
        m_s = spVALUE(new VALUE(_data.atKey("s")));
    }
}

void Transaction::streamHeader(dev::RLPStream& _s) const
{
    _s << nonce().asU256();
    _s << gasPrice().asU256();
    _s << gasLimit().asU256();
    _s << dev::Address(to().asString());  //  s << "";  if empty
    _s << value().asU256();
    _s << dev::fromHex(data().asString());
}

void Transaction::buildVRS(VALUE const& _secret)
{
    dev::RLPStream stream;
    stream.appendList(6);
    streamHeader(stream);
    dev::h256 hash(dev::sha3(stream.out()));
    dev::Signature sig = dev::sign(dev::Secret(_secret.asString()), hash);
    dev::SignatureStruct sigStruct = *(dev::SignatureStruct const*)&sig;
    ETH_FAIL_REQUIRE_MESSAGE(sigStruct.isValid(),
        TestOutputHelper::get().testName() + " Could not construct transaction signature!");

    // 27 because devcrypto signing donesn't count chain id
    DataObject v = DataObject(dev::toCompactHexPrefixed(dev::u256(sigStruct.v + 27)));
    DataObject r = DataObject(dev::toCompactHexPrefixed(dev::u256(sigStruct.r)));
    DataObject s = DataObject(dev::toCompactHexPrefixed(dev::u256(sigStruct.s)));
    m_v = spVALUE(new VALUE(v, dev::u256("0xff")));
    m_r = spVALUE(new VALUE(r));
    m_s = spVALUE(new VALUE(s));
}

DataObject Transaction::asDataObject() const
{
    DataObject out;
    return out;
}

std::string const Transaction::getSignedRLP() const
{
    dev::RLPStream sWithSignature;
    sWithSignature.appendList(9);
    streamHeader(sWithSignature);
    sWithSignature << v().asU256().convert_to<dev::byte>();
    sWithSignature << r().asU256();
    sWithSignature << s().asU256();
    return dev::toHexPrefixed(sWithSignature.out());
}

}  // namespace teststruct
}  // namespace test
