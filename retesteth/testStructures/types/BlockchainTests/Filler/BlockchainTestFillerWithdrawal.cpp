#include "BlockchainTestFillerWithdrawal.h"
#include <retesteth/testStructures/Common.h>
#include <retesteth/EthChecks.h>
using namespace std;
using namespace test;

namespace test::teststruct
{

BlockchainTestFillerWithdrawal::BlockchainTestFillerWithdrawal(spDataObjectMove _data)
{
    try
    {
        auto record = _data.getPointer();
        REQUIRE_JSONFIELDS(record, "FillerBlock::WithdrawalRecord ",
            {{"index", {{DataType::String}, jsonField::Required}},
                {"validatorIndex", {{DataType::String}, jsonField::Required}},
                {"address", {{DataType::String}, jsonField::Required}},
                {"amount", {{DataType::String}, jsonField::Required}}});

        #define MODIFY(INDEX, MOD) \
            record.getContent().atKeyUnsafe(INDEX).performModifier(MOD);

        MODIFY("index", mod_valueToCompactEvenHexPrefixed)
        MODIFY("validatorIndex", mod_valueToCompactEvenHexPrefixed)
        MODIFY("amount", mod_valueToCompactEvenHexPrefixed)

        m_withdrawal = spWithdrawal(new Withdrawal(record));
    }
    catch (std::exception const& _ex)
    {
        throw UpwardsException(string("Withdrawals convertion error: ") + _ex.what() + "\n");
    }
}

}
