#include "TestRawTranasction.h"
#include <retesteth/EthChecks.h>

using namespace std;
using namespace dataobject;

namespace test::teststruct
{
TestRawTransaction::TestRawTransaction(DataObject const& _data) :
    MineBlocksResult(_data)
{
    REQUIRE_JSONFIELDS(_data, "test_rawTransaction::TestRawTransaction",
        {{"result", {{DataType::Bool, DataType::Integer}, jsonField::Required}},
            {"rejectedTransactions", {{DataType::Array}, jsonField::Optional}},
            {"acceptedTransactions", {{DataType::Array}, jsonField::Optional}}});

    if (_data.count("rejectedTransactions"))
    {
        auto const& resTr = _data.atKey("rejectedTransactions").at(0);
        ETH_ERROR_REQUIRE_MESSAGE(_data.atKey("rejectedTransactions").getSubObjects().size() == 1,
            "test_rawTransaction require only one transaction data in response!");
        ETH_ERROR_REQUIRE_MESSAGE(!_data.count("acceptedTransactions"),
            "test_rawTransaction `rejectedTransactions` section is declared together with `acceptedTransactions`!");
        m_sender = spFH20(new FH20(resTr.atKey("sender")));
        m_trHash = spFH32(new FH32(resTr.atKey("hash")));
        m_intrinsicGas = spVALUE(new VALUE(resTr.atKey("intrinsicGas")));
    }
    else if (_data.count("acceptedTransactions"))
    {
        auto const& resTr = _data.atKey("acceptedTransactions").at(0);
        ETH_ERROR_REQUIRE_MESSAGE(_data.atKey("acceptedTransactions").getSubObjects().size() == 1,
            "test_rawTransaction require only one transaction data in response!");

        m_sender = spFH20(new FH20(resTr.atKey("sender")));
        m_trHash = spFH32(new FH32(resTr.atKey("hash")));
        m_intrinsicGas = spVALUE(new VALUE(resTr.atKey("intrinsicGas")));
    }
    else
    {
        ETH_ERROR_MESSAGE("test_rawTransaction no `acceptedTransactions` nor `rejectedTransactions` is defined!");
    }
}

string const& TestRawTransaction::error() const
{
    if (m_rejectedTransactions.size() > 0)
        return (*m_rejectedTransactions.begin()).second;
    else
    {
        static string empty;
        return empty;
    }
}

}  // namespace teststruct
