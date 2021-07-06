#include "TransactionAccessList.h"
#include <libdevcore/Address.h>
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
#include <libdevcrypto/Common.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
TransactionAccessList::TransactionAccessList(DataObject const& _data)
  : TransactionLegacy()
{
    fromDataObject(_data);
}

void TransactionAccessList::fromDataObject(DataObject const& _data)
{
    m_secretKey = spVALUE(new VALUE(0));
    try
    {
        m_accessList = spAccessList(new AccessList(_data.atKey("accessList")));

        m_data = spBYTES(new BYTES(_data.atKey("data")));
        m_gasLimit = spVALUE(new VALUE(_data.atKey("gasLimit")));
        m_gasPrice = spVALUE(new VALUE(_data.atKey("gasPrice")));
        m_nonce = spVALUE(new VALUE(_data.atKey("nonce")));
        m_value = spVALUE(new VALUE(_data.atKey("value")));

        if (_data.atKey("to").type() == DataType::Null || _data.atKey("to").asString().empty())
            m_creation = true;
        else
        {
            m_creation = false;
            m_to = spFH20(new FH20(_data.atKey("to")));
        }

        if (_data.count("secretKey"))
            buildVRS(_data.atKey("secretKey"));
        else
        {
            m_v = spVALUE(new VALUE(_data.atKey("v")));
            if (m_v.getCContent() > dev::bigint("0xff"))
                throw test::UpwardsException("Incorrect transaction `v` value: " + m_v->asString());
            m_r = spVALUE(new VALUE(_data.atKey("r")));
            m_s = spVALUE(new VALUE(_data.atKey("s")));
            rebuildRLP();
        }
        requireJsonFields(_data, "TransactionAccessList " + _data.getKey(),
            {
                {"data", {{DataType::String}, jsonField::Required}},
                {"gasPrice", {{DataType::String}, jsonField::Required}},
                {"gasLimit", {{DataType::String}, jsonField::Required}},
                {"nonce", {{DataType::String}, jsonField::Required}},
                {"value", {{DataType::String}, jsonField::Required}},
                {"to", {{DataType::String, DataType::Null}, jsonField::Required}},
                {"secretKey", {{DataType::String}, jsonField::Optional}},
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
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("TransactionAccessList convertion error: ") + _ex.what() + _data.asJson());
    }
}

TransactionAccessList::TransactionAccessList(dev::RLP const& _rlp)
{
    m_secretKey = spVALUE(new VALUE(0));
    fromRLP(_rlp);
}

TransactionAccessList::TransactionAccessList(BYTES const& _rlp)
{
    m_secretKey = spVALUE(new VALUE(0));
    dev::bytes decodeRLP = sfromHex(_rlp.asString());
    dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);
    fromRLP(rlp);
}

void TransactionAccessList::fromRLP(dev::RLP const& _rlp)
{
    // 0 - chainID
    // 1 - nonce        4 - to      7 - v
    // 2 - gasPrice     5 - value   8 - r
    // 3 - gasLimit     6 - data    9 - s
    DataObject trData;
    size_t i = 0;
    rlpToString(_rlp[i++]);  // chainID
    trData["nonce"] = rlpToString(_rlp[i++]);
    trData["gasPrice"] = rlpToString(_rlp[i++]);
    trData["gasLimit"] = rlpToString(_rlp[i++]);
    string const to = rlpToString(_rlp[i++], 0);
    trData["to"] = to == "0x" ? "" : to;
    trData["value"] = rlpToString(_rlp[i++]);
    trData["data"] = rlpToString(_rlp[i++], 0, RLPTYPE::BYTES);

    // read access list
    spAccessList list = spAccessList(new AccessList(_rlp[i++]));
    trData["accessList"] = list.getContent().asDataObject();
    m_accessList = list;

    trData["v"] = rlpToString(_rlp[i++]);
    trData["r"] = rlpToString(_rlp[i++]);
    trData["s"] = rlpToString(_rlp[i++]);
    fromDataObject(trData);
}

void TransactionAccessList::buildVRS(VALUE const& _secret)
{
    setSecret(_secret);
    dev::RLPStream stream;
    stream.appendList(8);
    TransactionAccessList::streamHeader(stream);

    // Alter output with prefixed 01 byte + tr.rlp
    dev::bytes outa = stream.out();
    outa.insert(outa.begin(), dev::byte(1));  // txType

    dev::h256 hash(dev::sha3(outa));
    dev::Signature sig = dev::sign(dev::Secret(_secret.asString()), hash);
    dev::SignatureStruct sigStruct = *(dev::SignatureStruct const*)&sig;
    ETH_FAIL_REQUIRE_MESSAGE(
        sigStruct.isValid(), TestOutputHelper::get().testName() + " Could not construct transaction signature!");

    DataObject v = DataObject(dev::toCompactHexPrefixed(dev::u256(sigStruct.v), 1));
    DataObject r = DataObject(dev::toCompactHexPrefixed(dev::u256(sigStruct.r)));
    DataObject s = DataObject(dev::toCompactHexPrefixed(dev::u256(sigStruct.s)));
    m_v = spVALUE(new VALUE(v));
    m_r = spVALUE(new VALUE(r));
    m_s = spVALUE(new VALUE(s));
    rebuildRLP();
}

void TransactionAccessList::streamHeader(dev::RLPStream& _s) const
{
    // rlp([chainId, nonce, gasPrice, gasLimit, to, value, data, access_list, yParity, senderR, senderS])
    _s << VALUE(1).asBigInt();
    _s << nonce().asBigInt();
    _s << gasPrice().asBigInt();
    _s << gasLimit().asBigInt();
    if (Transaction::isCreation())
        _s << "";
    else
        _s << dev::Address(to().asString());
    _s << value().asBigInt();
    _s << test::sfromHex(data().asString());

    // Access Listist
    dev::RLPStream accessList(m_accessList->list().size());
    for (auto const& el : m_accessList->list())
        accessList.appendRaw(el->asRLPStream().out());

    _s.appendRaw(accessList.out());
}

DataObject const TransactionAccessList::asDataObject(ExportOrder _order) const
{
    DataObject out = TransactionLegacy::asDataObject(_order);

    out["chainId"] = "0x01";
    out["accessList"] = m_accessList->asDataObject();
    out["type"] = "0x01";
    if (_order == ExportOrder::ToolStyle)
    {
        out["chainId"] = "0x1";
        out["type"] = "0x1";
        if (!m_secretKey.isEmpty() && m_secretKey.getCContent() != 0)
            out["secretKey"] = m_secretKey->asString();
    }
    return out;
}

void TransactionAccessList::rebuildRLP()
{
    // RLP(01 + tr.rlp)
    dev::RLPStream wrapper;
    dev::RLPStream out;
    out.appendList(11);
    streamHeader(out);
    out << v().asBigInt().convert_to<dev::byte>();
    out << r().asBigInt();
    out << s().asBigInt();

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
}  // namespace test
