#pragma once
#include "DataObject.h"

namespace dataobject
{
spDataObject ConvertJsoncppFileToData(
    std::string const& _file, std::string const& _stopper = std::string(), bool _setAutosort = false);

spDataObject ConvertJsoncppStringToData2(
    std::string const& _input, std::string const& _stopper = std::string(), bool _setAutosort = false);
}
