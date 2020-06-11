#include "InfoIncomplete.h"
#include <retesteth/testStructures/Common.h>

namespace test
{
namespace teststruct
{
InfoIncomplete::InfoIncomplete(DataObject const& _data)
{
    m_comment = _data.atKey("comment").asString();
    requireJsonFields(_data, "Info " + _data.getKey(),
        {   {"comment", {{DataType::String}, jsonField::Required}}});
}

}  // namespace teststruct
}  // namespace test
