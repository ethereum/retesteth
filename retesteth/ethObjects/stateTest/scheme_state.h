#pragma once
#include "scheme_account.h"
#include <retesteth/DataObject.h>

namespace test {

	class scheme_state: public object
	{
		public:
		scheme_state(DataObject const& _state):
			object(_state)
		{
            for (auto const& accountObj : _state.getSubObjects())
                m_accounts.push_back(scheme_account(accountObj));
            refreshData();
		}

        std::vector<scheme_account> const& getAccounts() {return m_accounts; }
        bool hasAccount(std::string const& _address) const
        {
            for (auto const& a: m_accounts)
            {
                if (a.getData().getKey() == _address)
                    return true;
            }
            return false;
        }

        scheme_account const& getAccount(std::string const& _account) const
        {
            assert(hasAccount(_account));
            for (auto const& a: m_accounts)
            {
                if (a.getData().getKey() == _account)
                    return a;
            }
            return m_accounts[0]; // should never reach this line
        }

        // Insert precompiled account info into genesis
        DataObject getDataForRPC(std::string const &_network) const {
          static DataObject ecrecoverObj;
          static DataObject sha256Obj;
          static DataObject ripemd160Obj;
          static DataObject identityObj;
          static DataObject alt_bn128_G1_addObj;
          static DataObject alt_bn128_G1_mulObj;

          if (!ecrecoverObj.count("name")) {
            ecrecoverObj["name"] = "ecrecover";
            ecrecoverObj["linear"]["base"] = 3000;
            ecrecoverObj["linear"]["word"] = 0;

            sha256Obj["name"] = "sha256";
            sha256Obj["linear"]["base"] = 60;
            sha256Obj["linear"]["word"] = 12;

            ripemd160Obj["name"] = "sha256";
            ripemd160Obj["linear"]["base"] = 600;
            ripemd160Obj["linear"]["word"] = 120;

            identityObj["name"] = "identity";
            identityObj["linear"]["base"] = 15;
            identityObj["linear"]["word"] = 3;

            alt_bn128_G1_addObj["name"] = "alt_bn128_G1_add";
            alt_bn128_G1_addObj["linear"]["base"] = 500;
            alt_bn128_G1_addObj["linear"]["word"] = 0;

            alt_bn128_G1_mulObj["name"] = "alt_bn128_G1_mul";
            alt_bn128_G1_mulObj["linear"]["base"] = 40000;
            alt_bn128_G1_mulObj["linear"]["word"] = 0;
          }

          DataObject ret = m_data;
          ret["0000000000000000000000000000000000000001"]["precompiled"] =
              ecrecoverObj;
          ret["0000000000000000000000000000000000000002"]["precompiled"] =
              sha256Obj;
          ret["0000000000000000000000000000000000000003"]["precompiled"] =
              ripemd160Obj;
          ret["0000000000000000000000000000000000000004"]["precompiled"] =
              identityObj;

          if (_network == "Byzantium" || _network == "Constantinople") {
            ret["0000000000000000000000000000000000000005"]["precompiled"]
               ["name"] = "modexp";
            ret["0000000000000000000000000000000000000006"]["precompiled"] =
                alt_bn128_G1_addObj;
            ret["0000000000000000000000000000000000000007"]["precompiled"] =
                alt_bn128_G1_mulObj;
            ret["0000000000000000000000000000000000000008"]["precompiled"]
               ["name"] = "alt_bn128_pairing_product";
          }
          return ret;
        }

      private:
        std::vector<scheme_account> m_accounts;
        void refreshData()
        {
            //update data from account list
            std::string key = m_data.getKey();
            m_data.clear();
            m_data.setKey(key);
            for (auto const& accountObj : m_accounts)
                m_data.addSubObject(accountObj.getData());
        }
	};
}

