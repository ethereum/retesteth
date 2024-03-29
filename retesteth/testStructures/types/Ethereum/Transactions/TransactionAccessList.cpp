#include "TransactionAccessList.h"
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
#include <libdevcrypto/Common.h>
#include <retesteth/EthChecks.h>
#include <retesteth/Options.h>
#include <retesteth/helpers/TestHelper.h>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/testStructures/Common.h>
using namespace std;
using namespace dev;

namespace test::teststruct
{

void TransactionAccessList::checkDataScheme(DataObject const& _data) const
{
    REQUIRE_JSONFIELDS(_data, "TransactionAccessList " + _data.getKey(),
        {
            {"data", {{DataType::String}, jsonField::Required}},
            {"gasPrice", {{DataType::String}, jsonField::Required}},
            {"gasLimit", {{DataType::String}, jsonField::Required}},
            {"nonce", {{DataType::String}, jsonField::Required}},
            {"value", {{DataType::String}, jsonField::Required}},
            {"to", {{DataType::String, DataType::Null}, jsonField::Required}},
            {"secretKey", {{DataType::String}, jsonField::Optional}},
            {"sender", {{DataType::String}, jsonField::Optional}},
            {"v", {{DataType::String}, jsonField::Optional}},
            {"r", {{DataType::String}, jsonField::Optional}},
            {"s", {{DataType::String}, jsonField::Optional}},

            // Transaction type 1
            {"type", {{DataType::String}, jsonField::Optional}},
            {"chainId", {{DataType::String, DataType::Null}, jsonField::Optional}},
            {"accessList", {{DataType::Array}, jsonField::Required}},

            {"publicKey", {{DataType::String}, jsonField::Optional}},  // Besu EthGetBlockBy transaction
            {"raw", {{DataType::String}, jsonField::Optional}},        // Besu EthGetBlockBy transaction

            {"blockHash", {{DataType::String}, jsonField::Optional}},         // EthGetBlockBy transaction
            {"blockNumber", {{DataType::String}, jsonField::Optional}},       // EthGetBlockBy transaction
            {"from", {{DataType::String}, jsonField::Optional}},              // EthGetBlockBy transaction
            {"hash", {{DataType::String}, jsonField::Optional}},              // EthGetBlockBy transaction
            {"transactionIndex", {{DataType::String}, jsonField::Optional}},  // EthGetBlockBy transaction
            {"expectException", {{DataType::Object}, jsonField::Optional}}    // BlockchainTest filling
        });
}

void TransactionAccessList::_fromData(DataObject const& _data)
{
    TransactionLegacy::_fromData(_data);
    m_accessList = spAccessList(new AccessList(_data.atKey("accessList")));
}

TransactionAccessList::TransactionAccessList(dev::RLP const& _rlp) : TransactionLegacy()
{
    fromRLP(_rlp);
}

TransactionAccessList::TransactionAccessList(BYTES const& _rlp) : TransactionLegacy()
{
    dev::bytes decodeRLP = sfromHex(_rlp.asString());
    dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);
    fromRLP(rlp);
}

void TransactionAccessList::fromRLP(dev::RLP const& _rlp)
{
    if (_rlp.itemCount() != _rlpHeaderSize())
        throw test::UpwardsException(TransactionTypeToString(type())
                                     + "::fromRLP(RLP) expected to have exactly "
                                     + test::fto_string(_rlpHeaderSize()) + " elements!");

    // 0 - chainID
    // 1 - nonce        4 - to      7 - v
    // 2 - gasPrice     5 - value   8 - r
    // 3 - gasLimit     6 - data    9 - s
    DataObject trData;
    size_t i = 0;
    m_chainID = spVALUE(new VALUE(_rlp[i++]));
    m_nonce = spVALUE(new VALUE(_rlp[i++]));
    m_gasPrice = spVALUE(new VALUE(_rlp[i++]));
    m_gasLimit = spVALUE(new VALUE(_rlp[i++]));

    auto const r = _rlp[i++];
    std::ostringstream stream;
    stream << r.toBytes();
    m_creation = false;
    if (stream.str() == "0x")
        m_creation = true;
    else
        m_to = spFH20(new FH20(r));

    m_value = spVALUE(new VALUE(_rlp[i++]));
    m_data = spBYTES(new BYTES(_rlp[i++]));

    // read access list
    m_accessList = spAccessList(new AccessList(_rlp[i++]));

    m_v = spVALUE(new VALUE(_rlp[i++]));
    m_r = spVALUE(new VALUE(_rlp[i++]));
    m_s = spVALUE(new VALUE(_rlp[i++]));
    m_secretKey = spVALUE(new VALUE(0));
    rebuildRLP();
}

dev::h256 TransactionAccessList::buildVRSHash() const
{
    dev::RLPStream stream;
    stream.appendList(8);
    TransactionAccessList::streamHeader(stream);

    // Alter output with prefixed 01 byte + tr.rlp
    dev::bytes outa = stream.out();
    outa.insert(outa.begin(), dev::byte(1));  // txType
    return dev::sha3(outa);
}

void TransactionAccessList::buildVRS()
{
    Transaction::buildVRS();
}

void TransactionAccessList::streamHeader(dev::RLPStream& _s) const
{
    // rlp([chainId, nonce, gasPrice, gasLimit, to, value, data, access_list, yParity, senderR, senderS])
    _s << m_chainID->asBigInt();

    TransactionLegacy::streamHeader(_s);

    // Access Listist
    dev::RLPStream accessList(m_accessList->list().size());
    for (auto const& el : m_accessList->list())
        accessList.appendRaw(el->asRLPStream().out());
    _s.appendRaw(accessList.out());

}

const spDataObject TransactionAccessList::asDataObject(ExportOrder _order) const
{
    spDataObject out = TransactionLegacy::asDataObject(_order);

    (*out)["chainId"] = m_chainID->asString();
    if (!out->count("accessList"))
        (*out).atKeyPointer("accessList") = m_accessList->asDataObject();
    (*out)["type"] = "0x01";
    if (_order == ExportOrder::ToolStyle)
    {
        DataObject chainIDs(m_chainID->asString());
        chainIDs.performModifier(mod_removeLeadingZerosFromHexValues);
        (*out)["chainId"] = chainIDs.asString();
        (*out)["type"] = "0x1";
        if (!m_secretKey.isEmpty() && m_secretKey.getCContent() != 0)
            (*out)["secretKey"] = m_secretKey->asString();
        (*out).performModifier(mod_removeBigIntHint);
    }
    return out;
}

void TransactionAccessList::rebuildRLP()
{
    // RLP(01 + tr.rlp)
    dev::RLPStream wrapper;
    dev::RLPStream out;
    out.appendList(_rlpHeaderSize());
    streamHeader(out);
    out << v().serializeRLP();
    out << r().serializeRLP();
    out << s().serializeRLP();

    // Alter output with prefixed 01 byte + tr.rlp
    dev::bytes outa = out.out();
    outa.insert(outa.begin(), dev::byte(1));

    // Encode bytearray into rlp
    wrapper << outa;
    m_outRlpStream = wrapper;
    m_rawRLPdata = spBYTES(new BYTES(dev::toHexPrefixed(outa)));
    m_hash = spFH32(new FH32("0x" + dev::toString(dev::sha3(outa))));
}

}  // namespace teststruct
