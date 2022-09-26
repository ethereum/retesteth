#include "DebugStorageRangeAt.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

using namespace std;
using namespace test::debug;
namespace test::teststruct
{
DebugStorageRangeAt::DebugStorageRangeAt(DataObject const& _data)
{
    try
    {
        REQUIRE_JSONFIELDS(_data, "DebugStorageRangeAt " + _data.getKey(),
            {{"complete", {{DataType::Bool}, jsonField::Required}},
                {"storage", {{DataType::Object}, jsonField::Required}},
                {"nextKey", {{DataType::String}, jsonField::Optional}}});

        // Convert to test storage representation
        DataObject storageCorrect;
        for (auto const& record : _data.atKey("storage").getSubObjects())
            storageCorrect[record->atKey("key").asString()] = record->atKey("value").asString();
        m_storage = spStorage(new Storage(storageCorrect));
        if (_data.count("nextKey"))
            m_nextKey = spFH32(new FH32(_data.atKey("nextKey")));
        else
        {
            ETH_DC_MESSAGE(DC::LOWLOG, "DebugStorageRangeAt key `nextKey` is not set, use `0x00..00` instead.");
            m_nextKey = spFH32(new FH32(FH32::zero()));
        }
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("DebugStorageRangeAt unmarshal error: ") + _ex.what());
    }
}

}  // namespace teststruct
