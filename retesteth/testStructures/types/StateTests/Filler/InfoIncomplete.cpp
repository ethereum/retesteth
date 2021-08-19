#include "InfoIncomplete.h"
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
InfoIncomplete::InfoIncomplete(spDataObjectMove _data)
{
    m_rawData = _data.getPointer();
    m_comment = m_rawData->atKey("comment").asString();
    requireJsonFields(_data, "Info " + m_rawData->getKey(),
        {   {"comment", {{DataType::String}, jsonField::Required}}});
}

}  // namespace teststruct
}  // namespace test
