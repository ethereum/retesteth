#pragma once
#include <retesteth/DataObject.h>
#include <retesteth/TestHelper.h>
#include <retesteth/ethObjects/object.h>
#include <retesteth/ethObjects/account.h>
#include <libdevcore/RLP.h>
#include <libdevcore/SHA3.h>
#include <libdevcrypto/Common.h>
using namespace dev;

namespace test {
	class transaction: public object
	{
		public:
		transaction(DataObject const& _transaction):
			object(_transaction)
		{
			test::requireJsonFields(_transaction, "transaction", {
                {"data", {DataType::String} },
                {"gasLimit", {DataType::String} },
                {"gasPrice", {DataType::String} },
                {"nonce", {DataType::String} },
                {"secretKey", {DataType::String} },
                {"to", {DataType::String} },
                {"value", {DataType::String} }
			});
			m_data["version"] = "0x01";
            if (!m_data.at("to").asString().empty())
                m_data["to"] = account::makeHexAddress(m_data.at("to").asString());
			// convert into rpc format
			m_data["gas"] = m_data["gasLimit"].asString();
            makeAllFieldsHex(m_data);
		}

        std::string getSignedRLP() const
        {
            u256 nonce = u256(m_data.at("nonce").asString());
            u256 gasPrice = u256(m_data.at("gasPrice").asString());
            u256 gasLimit = u256(m_data.at("gasLimit").asString());
            Address trTo = Address(m_data.at("to").asString());
            u256 value = u256(m_data.at("value").asString());
            bytes data = fromHex(m_data.at("data").asString());

            dev::RLPStream s;
            s.appendList(6);
            s << nonce;
            s << gasPrice;
            s << gasLimit;
            if (m_data.at("to").asString().size() == 42)
                s << trTo;
            else
                s << "";
            s << value;
            s << data;
            h256 hash(dev::sha3(s.out()));
            Signature sig = dev::sign(dev::Secret(m_data.at("secretKey").asString()), hash);

            SignatureStruct sigStruct = *(SignatureStruct const*)&sig;
            ETH_REQUIRE_MESSAGE(sigStruct.isValid(), TestOutputHelper::get().testName() + "Could not construct transaction signature!");

            RLPStream sWithSignature;
            sWithSignature.appendList(9);
            sWithSignature << nonce;
            sWithSignature << gasPrice;
            sWithSignature << gasLimit;
            if (m_data.at("to").asString().size() == 42)
                sWithSignature << trTo;
            else
                sWithSignature << "";
            sWithSignature << value;
            sWithSignature << data;
            byte v = 27 + sigStruct.v;
            sWithSignature << v;
            sWithSignature << (u256)sigStruct.r;
            sWithSignature << (u256)sigStruct.s;
            return dev::toHexPrefixed(sWithSignature.out());
        }
    };

    class generalTransaction: public object
    {
        public:
        struct transactionInfo
        {
            transactionInfo(size_t _dataInd, size_t _gasInd, size_t _valueInd, DataObject const& _transaction) :
                gasInd(_gasInd), dataInd(_dataInd), valueInd(_valueInd), executed(false), transaction(_transaction)
                {}
            size_t gasInd;
            size_t dataInd;
            size_t valueInd;
            bool executed;
            test::transaction transaction;
        };

        generalTransaction(DataObject const& _transaction):
            object(_transaction)
        {
            test::requireJsonFields(_transaction, "transaction", {
                {"data", {DataType::Array} },
                {"gasLimit", {DataType::Array} },
                {"gasPrice", {DataType::String} },
                {"nonce", {DataType::String} },
                {"secretKey", {DataType::String} },
                {"to", {DataType::String} },
                {"value", {DataType::Array} }
            });
            for (auto& element: m_data.getSubObjectsUnsafe())
            {
                if (element.getKey() != "data")
                    makeAllFieldsHex(element);
            }
            parseGeneralTransaction();
        }

        std::vector<transactionInfo> const& getTransactions() const { return m_transactions; }
        std::vector<transactionInfo>& getTransactionsUnsafe() { return m_transactions; }

        private:
        std::vector<transactionInfo> m_transactions;
        void parseGeneralTransaction()
        {
            for (size_t dataInd = 0; dataInd < m_data.at("data").getSubObjects().size(); dataInd++)
            {
                for (size_t gasInd = 0; gasInd < m_data.at("gasLimit").getSubObjects().size(); gasInd++)
                {
                    for (size_t valueInd = 0; valueInd < m_data.at("value").getSubObjects().size(); valueInd++)
                    {
                        DataObject singleTransaction(DataType::Object);
                        DataObject data("data", m_data.at("data").getSubObjects().at(dataInd).asString());
                        DataObject gas("gasLimit", m_data.at("gasLimit").getSubObjects().at(gasInd).asString());
                        DataObject value("value", m_data.at("value").getSubObjects().at(valueInd).asString());

                        singleTransaction.addSubObject(data);
                        singleTransaction.addSubObject(gas);
                        singleTransaction.addSubObject(m_data.at("gasPrice"));
                        singleTransaction.addSubObject(m_data.at("nonce"));
                        singleTransaction.addSubObject(m_data.at("secretKey"));
                        singleTransaction.addSubObject(m_data.at("to"));
                        singleTransaction.addSubObject(value);
                        transactionInfo info(dataInd, gasInd, valueInd, singleTransaction);
                        m_transactions.push_back(info);
                    }
                }
            }
        }
    };
}

