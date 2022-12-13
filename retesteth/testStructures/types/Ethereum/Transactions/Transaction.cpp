#include "Transaction.h"
#include <Options.h>
#include <EthChecks.h>

using namespace test;
using namespace test::debug;
namespace test::teststruct
{

void Transaction::setChainID(VALUE const& _chainID) {
    if (m_secretKey.getCContent() != 0)
    {
        m_chainID = spVALUE(_chainID.copy());
        buildVRS();
    }
    else
        ETH_DC_MESSAGE(DC::LOWLOG, "Calling Transaction::setChainID for transaction without secretKey!");
}

Transaction::Transaction()
{
    m_chainID = spVALUE(new VALUE(Options::get().getCurrentConfig().cfgFile().defaultChainID()));
}

}
