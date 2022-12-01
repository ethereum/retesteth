#include "ConvertYaml.h"
#include <iostream>
using namespace std;

namespace dataobject
{

std::string yamlTypeAsString(YAML::NodeType::value _type)
{
    //Undefined, Null, Scalar, Sequence, Map
    switch (_type)
    {
    case YAML::NodeType::Null:
        return "null";
    case YAML::NodeType::Scalar:
        return "scalar (int, string, bool)";
    case YAML::NodeType::Sequence:
        return "sequence (array)";
    case YAML::NodeType::Map:
        return "mpa (object)";
    case YAML::NodeType::Undefined:
        return "undefined";
    default:
        break;
    }
    return "";
}

spDataObject ConvertYamlToData(YAML::Node const& _node, bool _sort)
{
    if (_node.IsNull())
        return spDataObject(new DataObject(DataType::Null));

    if (_node.IsScalar())
    {
        if (_node.Tag() == "tag:yaml.org,2002:int")
            return spDataObject(new DataObject(_node.as<int>()));
        else
            return spDataObject(new DataObject(_node.as<string>()));
    }

    if (_node.IsMap())
    {
        spDataObject jObject(new DataObject(DataType::Object));
        if (_sort)
            (*jObject).setAutosort(true);
        for (auto const& i : _node)
            (*jObject).addSubObject(i.first.as<string>(), ConvertYamlToData(i.second, _sort));
        return jObject;
    }

    if (_node.IsSequence())
    {
        spDataObject jArray(new DataObject(DataType::Array));
        if (_sort)
            (*jArray).setAutosort(true);
        for (size_t i = 0; i < _node.size(); i++)
            (*jArray).addArrayObject(ConvertYamlToData(_node[i], _sort));
        return jArray;
    }

    // Make it an exception!
    std::cerr << "Error parsing YAML node. Element type not defined! " + yamlTypeAsString(_node.Type());
    return spDataObject(new DataObject(DataType::Null));
}

}//namespace
