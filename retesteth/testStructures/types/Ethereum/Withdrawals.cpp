#include "Withdrawals.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>
using namespace dataobject;
using namespace test;
using namespace test::debug;
using namespace std;

namespace test::teststruct {

Withdrawals::WithdrawalRecord::WithdrawalRecord(DataObject const& _data)
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

Withdrawals::Withdrawals(spDataObjectMove _data)
{
    auto const raw = _data.getPointer();
    if (raw->type() != DataType::Array)
        ETH_FAIL_MESSAGE("Withdrawals require json array, but got: /n" + raw->asJson());

    for (auto const& record : raw->getSubObjects())
    {
        if (raw->type() != DataType::Object)
            ETH_FAIL_MESSAGE("Withdrawals::record require to be json object, but got: /n" + record->asJson());
        WithdrawalRecord rec(record);
        m_records.push_back(std::move(rec));
    }
}

spDataObject Withdrawals::asDataObject() const
{
    spDataObject ret(new DataObject(DataType::Array));
    for (auto const& record : m_records)
        (*ret).addArrayObject(record.asDataObject());
    return ret;
}

spDataObject Withdrawals::WithdrawalRecord::asDataObject() const
{
    spDataObject ret(new DataObject(DataType::Object));
    (*ret)["index"] = index->asString();
    (*ret)["validatorIndex"] = validatorIndex->asString();
    (*ret)["address"] = address->asString();
    (*ret)["amount"] = amount->asString();
    return ret;
}


}
