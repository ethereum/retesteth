#include "TestInfo.h"
#include <boost/test/unit_test.hpp>
#include <retesteth/helpers/TestOutputHelper.h>
#include <retesteth/Options.h>
#include <retesteth/EthChecks.h>
using namespace std;
using namespace test::debug;
using namespace boost;
using namespace boost::unit_test;

namespace test {

TestInfo::TestInfo(std::string const& _fork, int _trD, int _trG, int _trV)
  : m_sFork(_fork), m_trD(_trD), m_trG(_trG), m_trV(_trV)
{
    m_isStateTransactionInfo = true;
    m_currentTestCaseName = makeTestCaseName();
}

TestInfo::TestInfo(std::string const& _fork, size_t _trD, size_t _trG, size_t _trV)
  : m_sFork(_fork), m_trD(_trD), m_trG(_trG), m_trV(_trV)
{
    m_isStateTransactionInfo = true;
    m_currentTestCaseName = makeTestCaseName();
}

TestInfo::TestInfo(std::string const& _fork, size_t _block, std::string const& _chainName)
  : m_sFork(_fork),
    m_sChainName(_chainName),
    m_blockNumber(_block),
    m_isStateTransactionInfo(false)
{
    m_isBlockchainTestInfo = true;
    m_currentTestCaseName = makeTestCaseName();
}

TestInfo::TestInfo(std::string const& _info, std::string const& _testName) : m_sFork(_info)
{
    namespace framework = boost::unit_test::framework;
    m_isGeneralTestInfo = true;
    m_currentTestCaseName = makeTestCaseName();

    if (!_testName.empty())
        TestOutputHelper::get().setCurrentTestName(_testName);
}

std::string TestInfo::errorDebug() const
{
    if (m_sFork.empty())
        return "";

    string message;
    bool nologcolor = Options::get().nologcolor;
    if (!nologcolor)
        message = cYellow;
    message += " (" + m_currentTestCaseName + "/" + TestOutputHelper::get().testName();

    if (!m_isGeneralTestInfo)
    {
        message += ", fork: " + m_sFork;
        if (!m_sChainName.empty())
            message += ", chain: " + m_sChainName;
    }
    else
        message += ", step: " + m_sFork;

    if (m_isBlockchainTestInfo)
        message += ", block: " + to_string(m_blockNumber);
    else if (m_isStateTransactionInfo)
        message += ", TrInfo: d: " + to_string(m_trD) + ", g: " + to_string(m_trG) + ", v: " + to_string(m_trV);

    if (!m_sTransactionData.empty())
        message += ", TrData: `" + m_sTransactionData + "`";

    if (nologcolor)
        return message + ")";
    return message + ")" + cDefault;
}

string TestInfo::makeTestCaseName() const
{
    string name;
    auto const& boostTCase = framework::current_test_case();
    try
    {
        auto const& boostSuite = framework::get<test_suite>(boostTCase.p_parent_id);
        name = boostSuite.p_name.get() + "/";
    }
    catch (std::exception const&)
    {
        ETH_WARNING("Error getting parent suite from boost!" + boostTCase.p_name.get());
    }

    return name + boostTCase.p_name.get();
}


std::string TestInfo::caseName()
{
    return boost::unit_test::framework::current_test_case().p_name;
}

}
