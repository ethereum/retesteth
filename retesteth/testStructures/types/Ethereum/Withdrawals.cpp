#include "Withdrawals.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
#include <libdevcore/Address.h>
using namespace dataobject;
using namespace test;
using namespace test::debug;
using namespace std;
using namespace dev;

namespace test::teststruct {

Withdrawal::Withdrawal(DataObject const& _data)
{
    REQUIRE_JSONFIELDS(_data, "WithdrawalRecord ",
        {{"index", {{DataType::String}, jsonField::Required}},
            {"validatorIndex", {{DataType::String}, jsonField::Required}},
            {"address", {{DataType::String}, jsonField::Required}},
            {"amount", {{DataType::String}, jsonField::Required}}});
    index = spVALUE(new VALUE(_data.atKey("index")));
    validatorIndex = spVALUE(new VALUE(_data.atKey("validatorIndex")));
    address = spFH20(new FH20(_data.atKey("address")));
    amount = spVALUE(new VALUE(_data.atKey("amount")));
}

Withdrawal::Withdrawal(dev::RLP const& _rlp)
{
    size_t i = 0;
    index = spVALUE(new VALUE(_rlp[i++]));
    validatorIndex = spVALUE(new VALUE(_rlp[i++]));
    address = spFH20(new FH20(_rlp[i++]));
    amount = spVALUE(new VALUE(_rlp[i++]));
}


spDataObject Withdrawal::asDataObject(ExportOrder _order) const
{
    spDataObject ret(new DataObject(DataType::Object));
    (*ret)["index"] = index->asString();
    (*ret)["validatorIndex"] = validatorIndex->asString();
    (*ret)["amount"] = amount->asString();
    if (_order == ExportOrder::ToolStyle)
        (*ret).performModifier(mod_removeLeadingZerosFromHexValues);
    (*ret)["address"] = address->asString();
    return ret;
}

const RLPStream Withdrawal::asRLPStream() const
{
    RLPStream rlp;
    rlp.appendList(4);
    rlp << index->asBigInt();
    rlp << validatorIndex->asBigInt();
    rlp << Address(address->asString());
    rlp << amount->asBigInt();
    return rlp;
}


}
