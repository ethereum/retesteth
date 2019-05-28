#pragma once
#include "../object.h"
#include "scheme_env.h"
#include "scheme_state.h"
#include "scheme_account.h"
#include "scheme_transaction.h"
#include "scheme_postState.h"

#include <dataObject/DataObject.h>
#include <retesteth/Options.h>
#include <retesteth/TestHelper.h>

using namespace  test;
namespace testprivate {
    class scheme_stateTestBase: public object
    {
        public:
        scheme_stateTestBase(DataObject const& _test);
        scheme_env const& getEnv() const { return m_env; }
        scheme_state const& getPre() const { return m_pre; }
        scheme_generalTransaction const& getGenTransaction() const { return m_transaction; }
        std::vector<scheme_generalTransaction::transactionInfo> const& getTransactions() const { return m_transaction.getTransactions(); }
        std::vector<scheme_generalTransaction::transactionInfo>& getTransactionsUnsafe() { return m_transaction.getTransactionsUnsafe(); }
        void checkUnexecutedTransactions();
        DataObject getGenesisForRPC(
            const std::string& _network, const std::string& _sealEngine) const;

    private:
        class fieldChecker
        {
            public:
            fieldChecker(DataObject const& _test);
        };
        fieldChecker m_checker;
        scheme_env m_env;
        scheme_state m_pre;
        scheme_generalTransaction m_transaction;
    };
}
