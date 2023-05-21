#pragma once
#include "DataObject.h"

namespace dataobject
{

struct CJOptions
{
    enum class JsonParse
    {
        ALLOW_COMMENTS,
        STRICT_JSON
    };
    JsonParse jsonParse = JsonParse::STRICT_JSON;
    bool autosort = false;
    std::string stopper = std::string();
};

/// Convert Json object represented as string to DataObject
spDataObject ConvertJsoncppStringToData(
    std::string const& _input, CJOptions const& _opt = CJOptions());
}
