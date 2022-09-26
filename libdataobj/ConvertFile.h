#pragma once
#include "DataObject.h"

namespace dataobject
{
/// Convert Json object represented as string to DataObject
spDataObject ConvertJsoncppStringToData(
    std::string const& _input, std::string const& _stopper = std::string(), bool _setAutosort = false);
}
