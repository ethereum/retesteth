#include "TransactionLegacy.h"
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
#include <libdevcrypto/Common.h>
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/Constants.h>

using namespace std;
using namespace dev;
using namespace test::teststruct::constnames;

namespace test::teststruct
{

void TransactionLegacy::checkDataScheme(DataObject const& _data) const
{
    REQUIRE_JSONFIELDS(_data, "TransactionLegacy " + _data.getKey(),
        {
            {c_data, {{DataType::String}, jsonField::Required}},
            {c_gasLimit, {{DataType::String}, jsonField::Required}},
            {c_gasPrice, {{DataType::String}, jsonField::Required}},
            {c_nonce, {{DataType::String}, jsonField::Required}},
            {c_value, {{DataType::String}, jsonField::Required}},
            {c_to, {{DataType::String, DataType::Null}, jsonField::Required}},
            {c_secretKey, {{DataType::String}, jsonField::Optional}},
            {c_sender, {{DataType::String}, jsonField::Optional}},
            {c_v, {{DataType::String}, jsonField::Optional}},
            {c_r, {{DataType::String}, jsonField::Optional}},
            {c_s, {{DataType::String}, jsonField::Optional}},

            {"publicKey", {{DataType::String}, jsonField::Optional}},  // Besu EthGetBlockBy transaction
            {"raw", {{DataType::String}, jsonField::Optional}},        // Besu EthGetBlockBy transaction
            {"chainId", {{DataType::String}, jsonField::Optional}},    // Besu EthGetBlockBy transaction
            {"type", {{DataType::String}, jsonField::Optional}},       // Besu EthGetBlockBy transaction

            {"blockHash", {{DataType::String}, jsonField::Optional}},         // EthGetBlockBy transaction
            {"blockNumber", {{DataType::String}, jsonField::Optional}},       // EthGetBlockBy transaction
            {"from", {{DataType::String}, jsonField::Optional}},              // EthGetBlockBy transaction
            {"hash", {{DataType::String}, jsonField::Optional}},              // EthGetBlockBy transaction
            {"transactionIndex", {{DataType::String}, jsonField::Optional}},  // EthGetBlockBy transaction
            {"expectException", {{DataType::Object}, jsonField::Optional}}    // BlockchainTest filling
        });
}

void TransactionLegacy::_fromData(DataObject const& _data)
{
    m_data = sBYTES(_data.atKey(c_data));
    m_gasLimit = sVALUE(_data.atKey(c_gasLimit));
    m_gasPrice = sVALUE(_data.atKey(c_gasPrice));
    m_nonce = sVALUE(_data.atKey(c_nonce));
    m_value = sVALUE(_data.atKey(c_value));

    if (_data.count(c_sender))
        m_sender = sFH20(_data.atKey(c_sender));

    if (_data.count("chainId"))
        m_chainID = sVALUE(_data.atKey("chainId"));

    auto const& to = _data.atKey(c_to);
    if (to.type() == DataType::Null || to.asString().empty())
        m_creation = true;
    else
    {
        m_creation = false;
        m_to = sFH20(to);
    }
}

void TransactionLegacy::fromRLP(dev::RLP const& _rlp)
{
    if (_rlp.itemCount() != _rlpHeaderSize())
        throw test::UpwardsException(TransactionTypeToString(type())
                                     + "::fromRLP(RLP) expected to have exactly "
                                     + test::fto_string(_rlpHeaderSize()) + " elements!");

    // 0 - nonce        3 - to      6 - v
    // 1 - gasPrice     4 - value   7 - r
    // 2 - gasLimit     5 - data    8 - s
    size_t i = 0;
    m_nonce = sVALUE(_rlp[i++]);
    m_gasPrice = sVALUE(_rlp[i++]);
    m_gasLimit = sVALUE(_rlp[i++]);

    auto const r = _rlp[i++];
    std::ostringstream stream;
    stream << r.toBytes();
    m_creation = false;
    if (stream.str() == "0x")
        m_creation = true;
    else
        m_to = sFH20(r);

    m_value = sVALUE(_rlp[i++]);
    m_data = sBYTES(_rlp[i++]);
    m_v = sVALUE(_rlp[i++]);
    m_r = sVALUE(_rlp[i++]);
    m_s = sVALUE(_rlp[i++]);

    if (m_v.getCContent() == 27 || m_v.getCContent() == 28)
        m_chainID = sVALUE(1);
    else
    {
        int chainID = std::floor((double)(m_v.getCContent().asBigInt() - 35) / 2);
        if (chainID < 0)
        {
            if (Options::get().filltests)
                ETH_WARNING("Error decoding chainID from transaction RLP: " + test::fto_string(chainID));
        }
        else
            m_chainID = sVALUE(chainID);
    }

    m_secretKey = sVALUE(0);
    rebuildRLP();
}

TransactionLegacy::TransactionLegacy(dev::RLP const& _rlp) : Transaction()
{
    fromRLP(_rlp);
}

TransactionLegacy::TransactionLegacy(BYTES const& _rlp) : Transaction()
{
    dev::bytes decodeRLP = sfromHex(_rlp.asString());
    dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);
    fromRLP(rlp);
}

void TransactionLegacy::streamHeader(dev::RLPStream& _s) const
{
    _s << nonce().serializeRLP();
    _s << gasPrice().serializeRLP();
    _s << gasLimit().serializeRLP();
    if (m_creation)
        _s << C_EMPTY_STR;
    else
        _s << to().serializeRLP();
    _s << value().serializeRLP();
    _s << test::sfromHex(data().asString());
}

dev::h256 TransactionLegacy::buildVRSHash() const
{
    dev::RLPStream stream;

    // https://eips.ethereum.org/EIPS/eip-155
    bool legacyEncoding;
    if (m_v.isEmpty())
        legacyEncoding = m_chainID.getCContent() == 1;
    else
        legacyEncoding = m_v->asBigInt() < 37;

    stream.appendList(legacyEncoding ? 6 : 9);
    streamHeader(stream);
    if (!legacyEncoding)
    {
        stream << m_chainID->serializeRLP();
        stream << VALUE(0).serializeRLP();
        stream << VALUE(0).serializeRLP();
    }
    return dev::sha3(stream.out());
}

void TransactionLegacy::buildVRS()
{
    const dev::h256 hash = buildVRSHash();
    const dev::Secret secret(m_secretKey->asString());
    dev::Signature sig = dev::sign(secret, hash);
    dev::SignatureStruct sigStruct = *(dev::SignatureStruct const*)&sig;
    ETH_FAIL_REQUIRE_MESSAGE(
        sigStruct.isValid(), TestOutputHelper::get().testName() + " Could not construct transaction signature!");

    // 27 because devcrypto signing donesn't count chain id
    // https://github.com/ethereum/EIPs/blob/master/EIPS/eip-155.md

    bigint v;
    if (m_chainID.getCContent() == 1)
        v = bigint(dev::toCompactHexPrefixed(dev::u256(sigStruct.v + 27)));
    else
    {
        // {0,1} + CHAIN_ID * 2 + 35
        v = bigint(dev::toCompactHexPrefixed(dev::u256(sigStruct.v + m_chainID->asBigInt() * 2 + 35)));
    }

    const bigint r (dev::toCompactHexPrefixed(dev::u256(sigStruct.r)));
    const bigint s (dev::toCompactHexPrefixed(dev::u256(sigStruct.s)));
    m_v = sVALUE(v);
    m_r = sVALUE(r);
    m_s = sVALUE(s);
    rebuildRLP();
}


const spDataObject TransactionLegacy::asDataObject(ExportOrder _order) const
{
    spDataObject out;
    (*out)[c_data] = m_data->asString();
    (*out)[c_gasLimit] = m_gasLimit->asString();
    (*out)[c_gasPrice] = m_gasPrice->asString();
    (*out)[c_nonce] = m_nonce->asString();
    if (m_creation)
        (*out)[c_to] = C_EMPTY_STR;
    else
        (*out)[c_to] = m_to->asString();
    (*out)[c_value] = m_value->asString();
    (*out)[c_v] = m_v->asString();
    (*out)[c_r] = m_r->asString();
    (*out)[c_s] = m_s->asString();
    if (!m_sender.isEmpty())
        (*out)[c_sender] = m_sender->asString();

    if (_order == ExportOrder::ToolStyle)
    {
        (*out).performModifier(mod_removeLeadingZerosFromHexValues, DataObject::ModifierOption::RECURSIVE, {"data", "to"});
        (*out).renameKey(c_gasLimit, "gas");
        (*out).renameKey(c_data, "input");
        if (!m_secretKey.isEmpty() && m_secretKey.getCContent() != 0)
            (*out)[c_secretKey] = m_secretKey->asString();
        (*out).performModifier(mod_removeBigIntHint);
    }

    return out;
}

void TransactionLegacy::rebuildRLP()
{
    dev::RLPStream out;
    out.appendList(_rlpHeaderSize());
    streamHeader(out);
    out << v().serializeRLP();
    out << r().serializeRLP();
    out << s().serializeRLP();
    m_outRlpStream = out;
    m_rawRLPdata = sBYTES(dev::toHexPrefixed(out.out()));
    m_hash = sFH32("0x" + dev::toString(dev::sha3(out.out())));
}

}  // namespace teststruct
