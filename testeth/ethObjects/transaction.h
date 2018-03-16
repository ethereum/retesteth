#pragma once
#include <testeth/DataObject.h>
#include <testeth/TestHelper.h>
#include <testeth/ethObjects/object.h>

namespace test {
	class transaction: public object
	{
		public:
		transaction(DataObject const& _transaction):
			object(_transaction)
		{
			test::requireJsonFields(_transaction, "transaction", {
				{"data", {{DataType::String}} },
				{"gasLimit", {{DataType::String}} },
				{"gasPrice", {{DataType::String}} },
				{"nonce", {{DataType::String}} },
				{"secretKey", {{DataType::String}} },
				{"to", {{DataType::String}} },
				{"value", {{DataType::String}} }
			});
			m_data["version"] = "0x01";
			// convert into rpc format
			m_data["gas"] = m_data["gasLimit"].asString();
            makeAllFieldsHex(m_data);
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
                {"data", {{DataType::Array}} },
                {"gasLimit", {{DataType::Array}} },
                {"gasPrice", {{DataType::String}} },
                {"nonce", {{DataType::String}} },
                {"secretKey", {{DataType::String}} },
                {"to", {{DataType::String}} },
                {"value", {{DataType::Array}} }
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

