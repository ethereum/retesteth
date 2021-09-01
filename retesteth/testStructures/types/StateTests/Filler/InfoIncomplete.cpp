#include "InfoIncomplete.h"
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
InfoIncomplete::InfoIncomplete(spDataObjectMove _data)
{
    REQUIRE_JSONFIELDS(_data, "Info " + m_rawData->getKey(),
        {   {"comment", {{DataType::String}, jsonField::Required}}});
    m_rawData = _data.getPointer();
}

}  // namespace teststruct
}  // namespace test
