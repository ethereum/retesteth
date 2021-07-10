#include "TransactionBaseFee.h"

#include <libdevcore/Address.h>
#include <libdevcore/CommonIO.h>
#include <libdevcore/SHA3.h>
#include <libdevcrypto/Common.h>
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

using namespace dev;

namespace test
{
namespace teststruct
{
TransactionBaseFee::TransactionBaseFee(DataObject const& _data)
  : Transaction()
{
    fromDataObject(_data);
}

void TransactionBaseFee::fromDataObject(DataObject const& _data)
{
    m_secretKey = spVALUE(new VALUE(0));
    try
    {
        m_accessList = spAccessList(new AccessList(_data.atKey("accessList")));
        m_maxFeePerGas = spVALUE(new VALUE(_data.atKey("maxFeePerGas")));
        m_maxPriorityFeePerGas = spVALUE(new VALUE(_data.atKey("maxPriorityFeePerGas")));

        m_data = spBYTES(new BYTES(_data.atKey("data")));
        m_gasLimit = spVALUE(new VALUE(_data.atKey("gasLimit")));
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
        requireJsonFields(_data, "TransactionBaseFee " + _data.getKey(),
            {
                {"data", {{DataType::String}, jsonField::Required}},
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

                // Transaction type 2
                {"maxFeePerGas", {{DataType::String}, jsonField::Required}},
                {"maxPriorityFeePerGas", {{DataType::String}, jsonField::Required}},

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
        throw UpwardsException(string("TransactionBaseFee convertion error: ") + _ex.what() + _data.asJson());
    }
}

TransactionBaseFee::TransactionBaseFee(dev::RLP const& _rlp)
{
    m_secretKey = spVALUE(new VALUE(0));
    fromRLP(_rlp);
}

TransactionBaseFee::TransactionBaseFee(BYTES const& _rlp)
{
    m_secretKey = spVALUE(new VALUE(0));
    dev::bytes decodeRLP = sfromHex(_rlp.asString());
    dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);
    fromRLP(rlp);
}

void TransactionBaseFee::fromRLP(dev::RLP const& _rlp)
{
    size_t i = 0;
    i++;  // chainID

    m_nonce = spVALUE(new VALUE(_rlp[i++]));
    m_maxPriorityFeePerGas = spVALUE(new VALUE(_rlp[i++]));
    m_maxFeePerGas = spVALUE(new VALUE(_rlp[i++]));
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


void TransactionBaseFee::buildVRS(VALUE const& _secret)
{
    setSecret(_secret);

    dev::RLPStream stream;
    stream.appendList(9);
    streamHeader(stream);

    // Alter output with prefixed 02 byte + tr.rlp
    dev::bytes outa = stream.out();
    outa.insert(outa.begin(), dev::byte(2));  // txType

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

void TransactionBaseFee::streamHeader(dev::RLPStream& _s) const
{
    // rlp([chainId, nonce, maxPriorityFeePerGas, maxFeePerGas, gasLimit, to, value, data, access_list, signatureYParity,
    // signatureR, signatureS])
    _s << VALUE(1).asBigInt();
    _s << nonce().asBigInt();

    _s << m_maxPriorityFeePerGas->asBigInt();
    _s << m_maxFeePerGas->asBigInt();

    _s << gasLimit().asBigInt();
    if (Transaction::isCreation())
        _s << "";
    else
    {
        if (to().isBigInt())
            _s << to().asBigInt();
        else
            _s << test::sfromHex(to().asString(ExportType::RLP));
    }
    _s << value().asBigInt();
    _s << test::sfromHex(data().asString());

    // Access Listist
    dev::RLPStream accessList(m_accessList->list().size());
    for (auto const& el : m_accessList->list())
        accessList.appendRaw(el->asRLPStream().out());

    _s.appendRaw(accessList.out());
}

DataObject const TransactionBaseFee::asDataObject(ExportOrder _order) const
{
    // Because we don't use gas_price field need to explicitly output
    DataObject out;
    out["data"] = m_data->asString();
    out["gasLimit"] = m_gasLimit->asString();
    out["nonce"] = m_nonce->asString();
    if (m_creation)
    {
        if (_order != ExportOrder::ToolStyle)
            out["to"] = "";
    }
    else
        out["to"] = m_to->asString();
    out["value"] = m_value->asString();
    out["v"] = m_v->asString();
    out["r"] = m_r->asString();
    out["s"] = m_s->asString();
    if (_order == ExportOrder::ToolStyle)
    {
        out.performModifier(mod_removeLeadingZerosFromHexValues, {"data", "to"});
        out.renameKey("gasLimit", "gas");
        out.renameKey("data", "input");
        if (!m_secretKey.isEmpty() && m_secretKey.getCContent() != 0)
            out["secretKey"] = m_secretKey->asString();
    }
    if (_order == ExportOrder::OldStyle)
    {
        out.setKeyPos("r", 4);
        out.setKeyPos("s", 5);
        out.setKeyPos("v", 7);
    }

    // standard transaction output without gas_price end

    // begin eip1559 transaction info
    out["chainId"] = "0x01";
    out["type"] = "0x02";
    out["maxFeePerGas"] = m_maxFeePerGas->asString();
    out["maxPriorityFeePerGas"] = m_maxPriorityFeePerGas->asString();
    if (_order == ExportOrder::ToolStyle)
    {
        out["chainId"] = "0x1";
        out["type"] = "0x2";

        DataObject t8ntoolFields;
        t8ntoolFields["maxFeePerGas"] = m_maxFeePerGas->asString();
        t8ntoolFields["maxPriorityFeePerGas"] = m_maxPriorityFeePerGas->asString();
        t8ntoolFields.performModifier(mod_removeLeadingZerosFromHexValues);
        out["maxFeePerGas"] = t8ntoolFields["maxFeePerGas"];
        out["maxPriorityFeePerGas"] = t8ntoolFields["maxPriorityFeePerGas"];

        if (!m_secretKey.isEmpty() && m_secretKey.getCContent() != 0)
            out["secretKey"] = m_secretKey->asString();
    }
    out["accessList"] = m_accessList->asDataObject();
    return out;
}

void TransactionBaseFee::rebuildRLP()
{
    // RLP(02 + tr.rlp)
    dev::RLPStream wrapper;
    dev::RLPStream out;
    out.appendList(12);
    TransactionBaseFee::streamHeader(out);
    out << v().asBigInt().convert_to<dev::byte>();
    out << r().asBigInt();
    out << s().asBigInt();

    // Alter output with prefixed 02 byte + tr.rlp
    dev::bytes outa = out.out();
    outa.insert(outa.begin(), dev::byte(2));

    // Encode bytearray into rlp
    wrapper << outa;
    m_outRlpStream = wrapper;
    m_rawRLPdata = spBYTES(new BYTES(dev::toHexPrefixed(outa)));
    m_hash = spFH32(new FH32("0x" + dev::toString(dev::sha3(outa))));
}


}  // namespace teststruct
}  // namespace test
