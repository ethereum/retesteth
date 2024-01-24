#ifdef DATA_YAML
#include "ConvertYaml.h"
#include <iostream>
using namespace std;
using namespace dataobject::ymlinternal;

namespace dataobject
{

string const ymlinternal::YML_INT_TAG = "tag:yaml.org,2002:int";
string const ymlinternal::YML_BOOL_TAG = "tag:yaml.org,2002:bool";

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
        return sDataObject(DataType::Null);

    if (_node.IsScalar())
    {
        if (_node.Tag() == YML_INT_TAG)
            return sDataObject(_node.as<int>());
        else if (_node.Tag() == YML_BOOL_TAG)
            return sDataObject(DataType::Bool, _node.as<bool>());
        else
            return sDataObject(_node.as<string>());
    }

    if (_node.IsMap())
    {
        spDataObject jObject = sDataObject(DataType::Object);
        if (_sort)
            (*jObject).setAutosort(true);
        for (auto const& i : _node)
        {
            auto const key = i.first.as<string>();
            if (jObject->count(key))
                throw DataObjectException() << "parsing .yml encountered dublicated key `" + key + "`";
            (*jObject).addSubObject(key, ConvertYamlToData(i.second, _sort));
        }
        return jObject;
    }

    if (_node.IsSequence())
    {
        spDataObject jArray = sDataObject(DataType::Array);
        if (_sort)
            (*jArray).setAutosort(true);
        for (size_t i = 0; i < _node.size(); i++)
            (*jArray).addArrayObject(ConvertYamlToData(_node[i], _sort));
        return jArray;
    }

    throw DataObjectException() << "Error parsing YAML node. Element type not defined! " + yamlTypeAsString(_node.Type());
    return sDataObject(DataType::Null);
}

}//namespace
#endif
