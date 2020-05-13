#pragma once
#include "../object.h"
#include "../stateTest/scheme_state.h"
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>

using namespace test;

namespace test
{
class scheme_blockHeader : public object
{
public:
    scheme_blockHeader(DataObject const& _test);
    std::string const& bloom() { return m_data.atKey("bloom").asString(); }
    std::string const& coinbase() { return m_data.atKey("coinbase").asString(); }
    std::string const& difficulty() { return m_data.atKey("difficulty").asString(); }
    std::string const& extraData() { return m_data.atKey("extraData").asString(); }
    std::string const& gasLimit() { return m_data.atKey("gasLimit").asString(); }
    std::string const& gasUsed() { return m_data.atKey("gasUsed").asString(); }
    std::string const& hash()
    {
        return m_data.count("hash") ? m_data.atKey("hash").asString() : object::emptyString;
    }
    std::string const& mixHash() { return m_data.atKey("mixHash").asString(); }
    std::string const& nonce() { return m_data.atKey("nonce").asString(); }
    std::string const& number() { return m_data.atKey("number").asString(); }
    std::string const& parentHash() { return m_data.atKey("parentHash").asString(); }
    std::string const& receiptTrie() { return m_data.atKey("receiptTrie").asString(); }
    std::string const& stateRoot() { return m_data.atKey("stateRoot").asString(); }
    std::string const& timestamp() { return m_data.atKey("timestamp").asString(); }
    std::string const& transactionsTrie() { return m_data.atKey("transactionsTrie").asString(); }
    std::string const& uncleHash() { return m_data.atKey("uncleHash").asString(); }

private:
};
}
