#include "BlockchainTestFillerWithdrawal.h"
#include <retesteth/testStructures/Common.h>

namespace test::teststruct
{

BlockchainTestFillerWithdrawal::BlockchainTestFillerWithdrawal(spDataObjectMove _data)
{
    auto data = _data.getPointer();
    for (auto& record : data.getContent().getSubObjectsUnsafe())
    {
        REQUIRE_JSONFIELDS(record, "FillerBlock::WithdrawalRecord ",
            {{"index", {{DataType::String}, jsonField::Required}},
                {"validatorIndex", {{DataType::String}, jsonField::Required}},
                {"address", {{DataType::String}, jsonField::Required}},
                {"amount", {{DataType::String}, jsonField::Required}}});

        #define MODIFY(INDEX, MOD) \
            record.getContent().atKeyUnsafe(INDEX).performModifier(MOD);

        MODIFY("index", mod_valueToCompactEvenHexPrefixed)
        MODIFY("validatorIndex", mod_valueToCompactEvenHexPrefixed)
        MODIFY("address", mod_valueToCompactEvenHexPrefixed)
    }
    m_withdrawals = spWithdrawals(new Withdrawals(dataobject::move(data)));
}

}
