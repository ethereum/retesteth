#include "State.h"
#include <retesteth/EthChecks.h>
#include <retesteth/TestHelper.h>
#include <retesteth/testStructures/Common.h>

namespace test::teststruct
{

// The Account is a part of State class and manages it's data
State::Account::Account(FH20 const& _addr, spVALUE& _balance, spVALUE& _nonce, spBYTES& _code, spStorage& _storage)
{
    m_address = spFH20(_addr.copy());
    m_balance = _balance;
    m_nonce = _nonce;
    m_code = _code;
    m_storage = _storage;
    if (m_nonce.getCContent() > c_maxNonce)
        ETH_ERROR_MESSAGE("Account `" + m_address->asString() + "` requires nonce <= (2**64)-1");
}

State::Account::Account(spDataObject& _data)
{
    REQUIRE_JSONFIELDS(_data, "Account " + _data->getKey(),
        {{"balance", {{DataType::String}, jsonField::Required}},
            {"code", {{DataType::String}, jsonField::Required}},
            {"nonce", {{DataType::String}, jsonField::Required}},
            {"storage", {{DataType::Object}, jsonField::Required}}});

    m_rawData = _data;
    if (_data->getKey().empty())
        ETH_ERROR_MESSAGE("State::Account::Account(spDataObject& _data) _data.key is empty! \n" + _data->asJson());
    m_address = spFH20(new FH20(_data->getKey()));
    m_balance = spVALUE(new VALUE(_data->atKey("balance")));
    m_nonce = spVALUE(new VALUE(_data->atKey("nonce")));
    m_code = spBYTES(new BYTES(_data->atKey("code")));
    m_storage = spStorage(new Storage(_data->atKey("storage")));
    if (m_nonce.getCContent() > c_maxNonce)
        ETH_ERROR_MESSAGE("Account `" + m_address->asString() + "` requires nonce <= (2**64)-1");
}

spDataObject const& State::Account::asDataObject() const
{
    if (m_rawData->getSubObjects().size() == 0)
    {
        (*m_rawData).setKey(m_address->asString());
        (*m_rawData)["code"] = m_code->asString();
        (*m_rawData)["nonce"] = m_nonce->asString();
        (*m_rawData)["balance"] = m_balance->asString();
        (*m_rawData).atKeyPointer("storage") = m_storage->asDataObject();
    }
    return m_rawData;
}

}  // namespace teststruct
