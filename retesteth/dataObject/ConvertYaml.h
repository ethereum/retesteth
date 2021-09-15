#pragma once
#include <dataObject/DataObject.h>
#include <yaml-cpp/yaml.h>

namespace dataobject
{
std::string yamlTypeAsString(YAML::NodeType::value _type);

/// Convert Yaml object to DataObject
spDataObject ConvertYamlToData(YAML::Node const& _input, bool _sort = false);
}
