#include "TransactionBlob.h"
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

void TransactionBlob::checkDataScheme(DataObject const& _data) const
{
    REQUIRE_JSONFIELDS(_data, "TransactionBlob " + _data.getKey(),
        {
            {"data", {{DataType::String}, jsonField::Required}},
            {"gasLimit", {{DataType::String}, jsonField::Required}},
            {"gasPrice", {{DataType::String}, jsonField::Optional}},
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

            // Transaction type 2
            {"maxFeePerGas", {{DataType::String}, jsonField::Required}},
            {"maxPriorityFeePerGas", {{DataType::String}, jsonField::Required}},

            // Transaction type 3
            {c_maxFeePerBlobGas, {{DataType::String}, jsonField::Required}},
            {"blobVersionedHashes", {{DataType::Array}, jsonField::Required}},

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

void TransactionBlob::_fromData(DataObject const& _data)
{
    TransactionBaseFee::_fromData(_data);
    m_maxFeePerBlobGas = sVALUE(_data.atKey(c_maxFeePerBlobGas));
    for (auto const& el : _data.atKey("blobVersionedHashes").getSubObjects())
        m_blobVersionedHashes.emplace_back(FH32(el->asString()));
}

TransactionBlob::TransactionBlob(dev::RLP const& _rlp) : TransactionBaseFee()
{
    fromRLP(_rlp);
}

TransactionBlob::TransactionBlob(BYTES const& _rlp) : TransactionBaseFee()
{
    dev::bytes decodeRLP = sfromHex(_rlp.asString());
    dev::RLP rlp(decodeRLP, dev::RLP::VeryStrict);
    fromRLP(rlp);
}

void TransactionBlob::fromRLP(dev::RLP const& _rlp)
{
    if (_rlp.itemCount() != _rlpHeaderSize())
        throw test::UpwardsException(TransactionTypeToString(type())
                                     + "::fromRLP(RLP) expected to have exactly "
                                     + test::fto_string(_rlpHeaderSize()) + " elements!");

    size_t i = 0;
    m_chainID = sVALUE(_rlp[i++]);

    m_nonce = sVALUE(_rlp[i++]);
    m_maxPriorityFeePerGas = sVALUE(_rlp[i++]);
    m_maxFeePerGas = sVALUE(_rlp[i++]);
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

    // read access list
    m_accessList = spAccessList(new AccessList(_rlp[i++]));

    // read blob
    m_maxFeePerBlobGas = sVALUE(_rlp[i++]);

    auto const& listHashes = _rlp[i++];
    for (auto const& el : listHashes)
        m_blobVersionedHashes.emplace_back(FH32(el));

    m_v = sVALUE(_rlp[i++]);
    m_r = sVALUE(_rlp[i++]);
    m_s = sVALUE(_rlp[i++]);

    m_secretKey = sVALUE(0);
    rebuildRLP();
}

dev::h256 TransactionBlob::buildVRSHash() const
{
    dev::RLPStream stream;
    stream.appendList(11);
    streamHeader(stream);
    // Alter output with prefixed 03 byte + tr.rlp
    dev::bytes outa = stream.out();
    outa.insert(outa.begin(), dev::byte(3));  // txType
    return dev::sha3(outa);
}

void TransactionBlob::streamHeader(dev::RLPStream& _s) const
{
    // rlp([chainId, nonce, maxPriorityFeePerGas, maxFeePerGas, gasLimit, to, value, data, access_list, signatureYParity,
    // signatureR, signatureS])
    _s << m_chainID->asBigInt();

    _s << nonce().serializeRLP();
    _s << m_maxPriorityFeePerGas->serializeRLP();
    _s << m_maxFeePerGas->serializeRLP();
    _s << gasLimit().serializeRLP();
    if (Transaction::isCreation())
        _s << "";
    else
        _s << to().serializeRLP();
    _s << value().serializeRLP();
    _s << test::sfromHex(data().asString());

    // Access Listist
    dev::RLPStream accessList(m_accessList->list().size());
    for (auto const& el : m_accessList->list())
        accessList.appendRaw(el->asRLPStream().out());

    _s.appendRaw(accessList.out());

    _s << m_maxFeePerBlobGas->serializeRLP();
    dev::RLPStream blobHashes(m_blobVersionedHashes.size());
    for (auto const& el : m_blobVersionedHashes)
        blobHashes.append(el.serializeRLP());
    _s.appendRaw(blobHashes.out());
}

const spDataObject TransactionBlob::asDataObject(ExportOrder _order) const
{
    spDataObject out = TransactionBaseFee::asDataObject(_order);
    (*out)["type"] = "0x03";
    (*out)[c_maxFeePerBlobGas] = m_maxFeePerBlobGas->asString();
    (*out).atKeyPointer(c_blobVersionedHashes) = sDataObject(DataType::Array);
    for (auto const& el : m_blobVersionedHashes)
        (*out)[c_blobVersionedHashes].addArrayObject(sDataObject(el.asString()));

    if (_order == ExportOrder::ToolStyle)
    {
        (*out)["type"] = "0x3";
        (*out).performModifier(mod_removeLeadingZerosFromHexValues, DataObject::ModifierOption::RECURSIVE,
            {"data", "to", "input", "address", "hash", "storageKeys", "sender"});
    }
    (*out).performModifier(mod_removeBigIntHint);
    return out;
}

void TransactionBlob::rebuildRLP()
{
    // RLP(03 + tr.rlp)
    dev::RLPStream wrapper;
    dev::RLPStream out;
    out.appendList(_rlpHeaderSize());
    TransactionBlob::streamHeader(out);
    out << v().serializeRLP();
    out << r().serializeRLP();
    out << s().serializeRLP();

    // Alter output with prefixed 03 byte + tr.rlp
    dev::bytes outa = out.out();
    outa.insert(outa.begin(), dev::byte(3));

    // Encode bytearray into rlp
    wrapper << outa;
    m_outRlpStream = wrapper;
    m_rawRLPdata = sBYTES(dev::toHexPrefixed(outa));
    m_hash = sFH32("0x" + dev::toString(dev::sha3(outa)));
}


}  // namespace teststruct
