#include "Transaction.h"
#include <Options.h>
#include <EthChecks.h>
#include <libdevcrypto/Common.h>
#include <retesteth/testStructures/Common.h>
#include <retesteth/helpers/TestOutputHelper.h>

using namespace std;
using namespace test;
using namespace test::debug;
namespace test::teststruct
{

std::string Transaction::TransactionTypeToString(TransactionType _bl)
{
    switch (_bl)
    {
        case TransactionType::LEGACY: return "LegacyTransaction";
        case TransactionType::ACCESSLIST: return "AccessListTransaction";
        case TransactionType::BASEFEE: return "BasefeeTransaction";
        case TransactionType::BLOB: return "BlobTransaction";
        default: return "UnparsedTransactionType";
    };
    return "UnparsedTransactionType";
}


void Transaction::setChainID(VALUE const& _chainID) {
    if (m_secretKey.getCContent() != 0)
    {
        m_chainID = spVALUE(_chainID.copy());
        buildVRS();
    }
    else
        ETH_DC_MESSAGE(DC::LOWLOG, "Calling Transaction::setChainID for transaction without secretKey!");
}

Transaction::Transaction()
{
    m_chainID = sVALUE(Options::get().getCurrentConfig().cfgFile().defaultChainID());
}

void Transaction::fromDataObject(DataObject const& _data)
{
    try
    {
        checkDataScheme(_data);
        _fromData(_data);
        makeSignature(_data);
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string(TransactionTypeToString(type()) + string(" convertion error: ") + _ex.what() + _data.asJson()));
    }
}


void Transaction::makeSignature(DataObject const& _data)
{
    if (_data.count("secretKey"))
    {
        setSecret(VALUE(_data.atKey("secretKey")));
        buildVRS();
    }
    else
    {
        m_v = sVALUE(_data.atKey("v"));
        m_r = sVALUE(_data.atKey("r"));
        m_s = sVALUE(_data.atKey("s"));
        rebuildRLP();
    }
}

void Transaction::buildVRS()
{
    const dev::h256 hash = buildVRSHash();
    const dev::Secret secret(m_secretKey->asString());
    dev::Signature sig = dev::sign(secret, hash);
    dev::SignatureStruct sigStruct = *(dev::SignatureStruct const*)&sig;
    ETH_FAIL_REQUIRE_MESSAGE(
        sigStruct.isValid(), TestOutputHelper::get().testName() + " Could not construct transaction signature!");

    m_v = sVALUE(dev::toCompactHexPrefixed(dev::u256(sigStruct.v), 1));
    m_r = sVALUE(dev::toCompactHexPrefixed(dev::u256(sigStruct.r)));
    m_s = sVALUE(dev::toCompactHexPrefixed(dev::u256(sigStruct.s)));
    rebuildRLP();
}

FH20 const& Transaction::sender() const
{
    if (m_sender.isEmpty())
    {
        if (m_secretKey.getCContent() != 0)
        {
            m_sender = convertSecretToPublic(m_secretKey);
            return m_sender;
        }
        else
        {
            try
            {
                bool const legacyV = (type() == TransactionType::LEGACY && m_chainID->asBigInt() == 1);
                bool const oldLegacyV = legacyV && (m_v->asDecString() == "27" || m_v->asDecString() == "28");
                dev::byte const v(legacyV ? (oldLegacyV ? m_v->asBigInt() - 27 : m_v->asBigInt() - 37) : m_v->asBigInt());

                DataObject rs;
                rs["r"] = m_r->asString();
                rs["s"] = m_s->asString();
                rs.performModifier(mod_valueToFH32);

                dev::h256 const r(rs.atKey("r").asString());
                dev::h256 const s(rs.atKey("s").asString());
                dev::h256 const recoverHash = buildVRSHash();
                dev::SignatureStruct const sig(r,s,v);
                dev::Public const pubkey = dev::recover(sig, recoverHash);
                auto const address = dev::toAddress(pubkey);
                m_sender = spFH20(new FH20(dev::toHexPrefixed(address)));
                return m_sender;
            }
            catch (std::exception const& _ex)
            {
                ETH_WARNING("Transaction::sender() const:: error recovering sender! \n" + asDataObject()->asJson());
                m_sender = spFH20(FH20::zero().copy());
                return m_sender;
            }
        }
    }
    else
        return m_sender;
}

}
