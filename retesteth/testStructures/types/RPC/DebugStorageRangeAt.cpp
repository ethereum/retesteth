#include "DebugStorageRangeAt.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
DebugStorageRangeAt::DebugStorageRangeAt(DataObject const& _data)
{
    try
    {
        // Convert to test storage representation
        DataObject storageCorrect;
        for (auto const& record : _data.atKey("storage").getSubObjects())
            storageCorrect[record.atKey("key").asString()] = record.atKey("value").asString();
        m_storage = spStorage(new Storage(storageCorrect));
        if (_data.count("nextKey"))
            m_nextKey = spFH32(new FH32(_data.atKey("nextKey")));
        else
        {
            ETH_WARNING_TEST("DebugStorageRangeAt key `nextKey` is not set, use `0x00..00` instead.", 6);
            m_nextKey = spFH32(new FH32(FH32::zero()));
        }
        requireJsonFields(_data, "DebugStorageRangeAt " + _data.getKey(),
            {{"complete", {{DataType::Bool}, jsonField::Required}},
             {"storage", {{DataType::Object}, jsonField::Required}},
             {"nextKey", {{DataType::String}, jsonField::Optional}}});
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("DebugStorageRangeAt unmarshal error: ") + _ex.what());
    }
}

}  // namespace teststruct
}  // namespace test
