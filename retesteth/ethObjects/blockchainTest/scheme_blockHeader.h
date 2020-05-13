#pragma once
#include "../object.h"
#include "../rpcResponse/scheme_block.h"
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
    std::string const& bloom() const { return m_data.atKey("bloom").asString(); }
    std::string const& coinbase() const { return m_data.atKey("coinbase").asString(); }
    std::string const& difficulty() const { return m_data.atKey("difficulty").asString(); }
    std::string const& extraData() const { return m_data.atKey("extraData").asString(); }
    std::string const& gasLimit() const { return m_data.atKey("gasLimit").asString(); }
    std::string const& gasUsed() const { return m_data.atKey("gasUsed").asString(); }
    std::string hash() const
    {
        /// !!! todo solve this. streamBlockHeader rely on m_data contents
        return "0x" + toString(dev::sha3(scheme_RPCBlock::streamBlockHeader(m_data).out()));
        ;
    }
    std::string const& mixHash() const { return m_data.atKey("mixHash").asString(); }
    std::string const& nonce() const { return m_data.atKey("nonce").asString(); }
    std::string const& number() const { return m_data.atKey("number").asString(); }
    std::string const& parentHash() const { return m_data.atKey("parentHash").asString(); }
    std::string const& receiptTrie() const { return m_data.atKey("receiptTrie").asString(); }
    std::string const& stateRoot() const { return m_data.atKey("stateRoot").asString(); }
    std::string const& timestamp() const { return m_data.atKey("timestamp").asString(); }
    std::string const& transactionsTrie() const
    {
        return m_data.atKey("transactionsTrie").asString();
    }
    std::string const& uncleHash() const { return m_data.atKey("uncleHash").asString(); }

private:
};
}
