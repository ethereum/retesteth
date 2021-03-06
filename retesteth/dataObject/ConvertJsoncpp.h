#pragma once
#ifdef JSONCPP
#include <dataObject/DataObject.h>
#include <json/value.h>

namespace dataobject
{
std::string jsonTypeAsString(Json::ValueType _type);

/// Convert Json::Value object to DataObject
DataObject ConvertJsoncppToData(Json::Value const& _input);
}
#endif
