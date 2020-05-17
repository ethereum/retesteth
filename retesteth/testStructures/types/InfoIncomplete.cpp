#include "InfoIncomplete.h"

namespace test
{
namespace teststruct
{
InfoIncomplete::InfoIncomplete(DataObject const& _data)
{
    m_comment = _data.atKey("comment").asString();
}

}  // namespace teststruct
}  // namespace test
