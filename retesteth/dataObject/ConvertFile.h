#pragma once
#include <dataObject/DataObject.h>

namespace dataobject
{
/// Convert Json object represented as string to DataObject
DataObject ConvertJsoncppStringToData(
    std::string const& _input, string const& _stopper = string(), bool _setAutosort = false);
}
