#include "Withdrawals.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
using namespace dataobject;
using namespace test;
using namespace test::debug;
using namespace std;

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


spDataObject Withdrawal::asDataObject() const
{
    spDataObject ret(new DataObject(DataType::Object));
    (*ret)["index"] = index->asString();
    (*ret)["validatorIndex"] = validatorIndex->asString();
    (*ret)["address"] = address->asString();
    (*ret)["amount"] = amount->asString();
    return ret;
}


}
