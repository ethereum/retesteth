#pragma once
#ifdef DATA_YAML
#include "DataObject.h"
#include <yaml-cpp/yaml.h>

namespace dataobject
{
std::string yamlTypeAsString(YAML::NodeType::value _type);

/// Convert Yaml object to DataObject
spDataObject ConvertYamlToData(YAML::Node const& _input, bool _sort = false);

namespace ymlinternal {
extern const std::string YML_INT_TAG;
extern const std::string YML_BOOL_TAG;
}
}
#endif
