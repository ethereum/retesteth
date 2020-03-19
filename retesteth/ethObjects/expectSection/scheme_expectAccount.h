#pragma once
#include "../object.h"
#include "../stateTest/scheme_account.h"

namespace test
{
/// Account declared in expect section state
class scheme_expectAccount : public object
{
public:
    scheme_expectAccount(DataObject const& _account) : object(_account)
    {
        m_shouldNotExist = _account.count("shouldnotexist");
        m_hasBalance = _account.count("balance");
        m_hasNonce = _account.count("nonce");
        m_hasCode = _account.count("code");
        m_hasStorage = _account.count("storage");

        if (m_hasStorage)
            scheme_account::validateStorage(m_data);

        // Make all fields hex
        m_data.setKey(scheme_account::makeHexAddress(m_data.getKey()));
        makeAllFieldsHex(m_data);
    }

    bool shouldNotExist() const { return m_shouldNotExist; }
    bool hasBalance() const { return m_hasBalance; }
    bool hasNonce() const { return m_hasNonce; }
    bool hasCode() const { return m_hasCode; }
    bool hasStorage() const { return m_hasStorage; }
    std::string getBalance() const
    {
        if (m_hasBalance)
            return m_data.atKey("balance").asString();
        return "";
    }
    void setBalance(u256 const& _balance)
    {
        m_hasBalance = true;
        m_data["balance"] = dev::toCompactHexPrefixed(_balance, 1);
    }
    std::string const& address() const { return getData().getKey(); }
    CompareResult compareStorage(DataObject const& _storage) const
    {
        CompareResult result = CompareResult::Success;
        auto checkMessage = [&result](bool _flag, CompareResult _type,
                                std::string const& _error) -> void {
            ETH_MARK_ERROR_FLAG(_flag, _error);
            if (!_flag)
                result = _type;
        };

        ETH_ERROR_REQUIRE_MESSAGE(
            _storage.type() == DataType::Object, "Storage must be of `Object` type!");
        ETH_ERROR_REQUIRE_MESSAGE(hasStorage(), "Storage was expected to be set!");

        DataObject const& expectStorage = m_data.atKey("storage");
        for (auto const& element : expectStorage.getSubObjects())
        {
            // Check that storage element is set
            string key = element.getKey();
            if (key == "0x" && _storage.count("0x00"))
                key = "0x00";
            else if (key == "0x00" && _storage.count("0x"))
                key = "0x";

            string const message = "Check State: Remote account '" + address() + "'";
            if (element.asString() == "0x" || element.asString() == "0x00")
            {
                // check that empty element is not set
                // if a post state has the value of 'key' as zero, such 'key' does not listed
                checkMessage(!_storage.count(key), CompareResult::IncorrectStorage,
                    message + " has storage key '" + element.getKey() +
                        "'. Test expected storage key: '" + element.getKey() +
                        "' to be set to zero");
            }
            else
            {
                // check that expected element exist
                checkMessage(_storage.count(key), CompareResult::IncorrectStorage,
                    message + " test expected storage key: '" + element.getKey() +
                        "' to be set to: '" + element.asString() + "', but remote key '" +
                        element.getKey() + "' does not exist!");

                if (result != CompareResult::Success)
                    return result;

                // if expected element exist, check exact value in the storage
                std::string valueInStorage = _storage.atKey(key).asString();
                checkMessage(valueInStorage == element.asString(), CompareResult::IncorrectStorage,
                    message + ": has incorrect storage [" + key + "] = " + valueInStorage +
                        ", test expected [" + key + "] = " + element.asString());
            }
        }
        checkMessage(expectStorage.getSubObjects().size() >= _storage.getSubObjects().size(),
            CompareResult::IncorrectStorage,
            TestOutputHelper::get().testName() + " Remote account '" + address() +
                "' storage has more storage records then expected!");

        if (expectStorage.getSubObjects().size() < _storage.getSubObjects().size())
        {
            for (auto const& element : _storage.getSubObjects())
            {
                string const message = "incorrect remote storage [" + element.getKey() +
                                       "] = " + element.asString() + ", test expected [" +
                                       element.getKey() + "] = 0";
                ETH_MARK_ERROR(message);
            }
        }
        return result;
    }

private:
    bool m_shouldNotExist;
    bool m_hasBalance;
    bool m_hasNonce;
    bool m_hasCode;
    bool m_hasStorage;
};
}
