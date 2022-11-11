#include "JsonReaderHelper.h"
namespace dataobject::jsonreader
{
bool isEmptyChar(char const& _char)
{
    if (_char == ' ' || _char == '\n' || _char == '\r' || _char == '\t')
        return true;
    return false;
}

}  // namespace dataobject::jsonreader
