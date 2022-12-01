#include "DebugAccountRange.h"
#include <retesteth/EthChecks.h>
#include <retesteth/testStructures/Common.h>

using namespace std;
namespace test::teststruct
{
DebugAccountRange::DebugAccountRange(DataObject const& _data)
{
    try
    {
        REQUIRE_JSONFIELDS(_data, "DebugAccountRange " + _data.getKey(),
            {{"addressMap", {{DataType::Object}, jsonField::Required}},
                {"nextKey", {{DataType::String}, jsonField::Required}}});
        for (auto const& record : _data.atKey("addressMap").getSubObjects())
            m_addresses.push_back(spFH20(new FH20(record)));
        m_nextKey = spFH32(new FH32(_data.atKey("nextKey")));
    }
    catch (std::exception const& _ex)
    {
        ETH_ERROR_MESSAGE(string("DebugAccountRange unmarshal error: ") + _ex.what());
    }
}

}  // namespace teststruct
