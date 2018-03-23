#pragma once
#include <testeth/Options.h>
#include <testeth/DataObject.h>
#include <testeth/ethObjects/account.h>
#include <testeth/ethObjects/object.h>
#include <testeth/TestHelper.h>

#include <testeth/ethObjects/env.h>
#include <testeth/ethObjects/state.h>
#include <testeth/ethObjects/transaction.h>
#include <testeth/ethObjects/expectSectionElement.h>

using namespace  test;
namespace testprivate {
    class stateTestBase: public object
    {
        public:
        stateTestBase(DataObject const& _test):
            object(_test),
            m_checker(_test),
            m_env(_test.at("env")),
            m_pre(_test.at("pre")),
            m_transaction(_test.at("transaction"))
        {

        }
        env const& getEnv() const { return m_env; }
        state const& getPre() const { return m_pre; }
        generalTransaction const& getGenTransaction() const { return m_transaction; }
        std::vector<generalTransaction::transactionInfo> const& getTransactions() const { return m_transaction.getTransactions(); }
        std::vector<generalTransaction::transactionInfo>& getTransactionsUnsafe() { return m_transaction.getTransactionsUnsafe(); }
		void checkUnexecutedTransactions()
		{
			Options const& opt = Options::get();
			if (opt.trDataIndex == -1 && opt.trGasIndex != -1 && opt.trValueIndex != -1)
			{
				for (auto const& tr: m_transaction.getTransactions())
				{
					BOOST_REQUIRE_MESSAGE(tr.executed == true, "A transaction was specified, but there is no execution results in a test! Transaction: dInd="
					+ toString(tr.dataInd) + " gInd=" + toString(tr.gasInd) + " vInd=" + toString(tr.valueInd));
				}
			}
		}

        private:
        class fieldChecker
        {
            public:
            fieldChecker(DataObject const& _test)
            {
                BOOST_CHECK_MESSAGE(_test.count("env"), "State test must have 'env' section");
                BOOST_CHECK_MESSAGE(_test.count("pre"), "State test must have 'pre' section");
                BOOST_CHECK_MESSAGE(_test.count("transaction"), "State test must have 'transaction' section");
            }
        };
        fieldChecker m_checker;
        env m_env;
        state m_pre;
        generalTransaction m_transaction;
    };
}

namespace test {
    class stateTest: public testprivate::stateTestBase
    {
        public:
        stateTest(DataObject const& _test):
            stateTestBase(_test),
            m_checker(_test),
            m_post(_test.at("post"))
        {}
        postState const& getPost() const { return m_post; }

        private:
        class fieldChecker
        {
            public:
            fieldChecker(DataObject const& _test)
            {
                requireJsonFields(_test, "stateTest " + _test.getKey(), {
					{"_info", {DataType::Object} },
					{"env", {DataType::Object} },
					{"pre", {DataType::Object} },
					{"transaction", {DataType::Object} },
					{"post", {DataType::Object} }
                });
            }
        };
        fieldChecker m_checker;
        postState m_post;
    };

    class stateTestFiller: public testprivate::stateTestBase
    {
        public:
        stateTestFiller(DataObject const& _test):
            stateTestBase(_test),
            m_checker(_test)
        {
            // parse expect section
            for (auto const& expect: _test.at("expect").getSubObjects())
            {
                expectSectionElement expElement(expect);
                m_expect.push_back(expElement);
                for (auto const& net : expElement.getNetworks())
                    m_allNetworksDeclaredInExpectSection.emplace(net);
            }
        }
        std::vector<expectSectionElement> const& getExpectSections() const { return m_expect; }
        std::set<std::string> const& getAllNetworksFromExpectSection() const { return m_allNetworksDeclaredInExpectSection; }

        private:
        class fieldChecker
        {
            public:
            fieldChecker(DataObject const& _test)
            {
                requireJsonFields(_test, "stateTestFiller " + _test.getKey(), {
					{"env", {DataType::Object} },
					{"pre", {DataType::Object} },
					{"transaction", {DataType::Object} },
					{"expect", {DataType::Array} }
                });
            }
        };
        fieldChecker m_checker;
        std::vector<expectSectionElement> m_expect;
        std::set<std::string> m_allNetworksDeclaredInExpectSection;
    };
}
