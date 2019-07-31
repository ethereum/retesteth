#pragma once
#include "../object.h"
#include <retesteth/TestHelper.h>

using namespace dev;
namespace  test {
enum CompareResult
{
    Success,
    AccountShouldNotExist,
    MissingExpectedAccount,
    IncorrectBalance,
    IncorrectNonce,
    IncorrectCode,
    IncorrectStorage,
    LogHashMismatch,
    PostHashMismatch,
    None
};

class scheme_account : public object
{
public:
    scheme_account(DataObject const& _account):
        object(_account)
    {
        requireJsonFields(_account, "account " + _account.getKey(), {
                              {"balance", {DataType::String} },
                              {"code", {DataType::String} },
                              {"nonce", {DataType::String} },
                              {"storage", {DataType::Object} }
                          });

        validateStorage(m_data);

        // Compile the code
        m_data["code"] = test::replaceCode(m_data.atKey("code").asString());
        m_data["balance"] = dev::toCompactHexPrefixed(
                    u256(m_data.atKey("balance").asString()), 1);  // fix odd strings

        // Make all fields hex
        m_data.setKey(makeHexAddress(m_data.getKey()));
        makeAllFieldsHex(m_data);
    }

    static void validateStorage(DataObject const& _accountObject)
    {
        // verify that storage in test is string and is digit or hex representation
        for (auto const& record : _accountObject.atKey("storage").getSubObjects())
        {
            string message = "account `" + _accountObject.getKey() + ":storage` record ";
            ETH_ERROR_REQUIRE_MESSAGE(record.type() == DataType::String, message + "is expected to be of string type!");
            message +=  "`" + record.asString() + "` ";
            DigitsType recordType = stringIntegerType(record.asString());
            if (recordType != DigitsType::Decimal && recordType != DigitsType::HexPrefixed)
                ETH_ERROR_MESSAGE(message + "is expected to be 0x prefixed hex string or decimal string representation!");
        }
    }
};
}
