#include "EthGetBlockByWithdrawal.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

using namespace std;
namespace test::teststruct
{
EthGetBlockByWithdrawal::EthGetBlockByWithdrawal(spDataObjectMove _data)
{
    try
    {
        spDataObject data = _data.getPointer();
        m_withdrawal = spWithdrawal(new Withdrawal(_data));
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("EthGetBlockByWithdrawal unmarshal error: ") + _ex.what());
    }
}

}  // namespace teststruct
