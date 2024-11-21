#pragma once
#include "../Ethereum/State.h"
#include "Filler/InfoIncomplete.h"
#include "Filler/StateTestFillerEnv.h"
#include "Filler/StateTestFillerExpectSection.h"
#include "Filler/StateTestFillerTransaction.h"
#include <libdataobj/DataObject.h>

namespace test::teststruct
{
struct StateTestInFiller : GCP_SPointerBase
{
    StateTestInFiller(spDataObject&);

    std::string const& testName() const { return m_name; }
    bool hasInfo() const { return !m_info.isEmpty(); }
    bool hasBigInt() const { return m_hasBigInt; }
    InfoIncomplete const& Info() const { return m_info; }
    StateTestFillerEnv const& Env() const { return m_env; }
    State const& Pre() const { return m_pre; }
    bool hasEmptyAccount() const { return m_hasEmptyAccounts; }

    StateTestFillerTransaction const& GeneralTr() const { return m_transaction; }
    std::vector<StateTestFillerExpectSection> const& Expects() const { return m_expectSections; }
    std::set<FORK> getAllForksFromExpectSections() const;

    std::vector<std::string> const& unitTestExceptions() const { return m_exceptions; }
    DataObject const& unitTestVerify() const { return m_verify; }
    DataObject const& unitTestVerifyBC() const { return m_verifyBC; }

private:
    StateTestInFiller() {}
    std::string m_name;
    GCP_SPointer<InfoIncomplete> m_info;
    GCP_SPointer<StateTestFillerEnv> m_env;
    spState m_pre;
    GCP_SPointer<StateTestFillerTransaction> m_transaction;
    std::vector<StateTestFillerExpectSection> m_expectSections;
    spDataObject m_verify;
    spDataObject m_verifyBC;
    bool m_hasBigInt = false;
    std::vector<std::string> m_exceptions;
    bool m_hasEmptyAccounts;
};

struct GeneralStateTestFiller
{
    GeneralStateTestFiller(spDataObject&);
    std::vector<StateTestInFiller> const& tests() const { return m_tests; }

private:
    GeneralStateTestFiller() {}
    std::vector<StateTestInFiller> m_tests;
};


}  // namespace teststruct
