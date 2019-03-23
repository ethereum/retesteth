#include <dataObject/ConvertYaml.h>

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

DataObject ConvertYamlToData(YAML::Node const& _node)
{
    if (_node.IsNull())
        return DataObject(DataType::Null);

    if (_node.IsScalar())
    {
        if (_node.Tag() == "tag:yaml.org,2002:int")
            return DataObject(_node.as<int>());
        else
            return DataObject(_node.as<string>());
    }

    if (_node.IsMap())
    {
        DataObject jObject(DataType::Object);
        for (auto const& i : _node)
            jObject.addSubObject(i.first.as<string>(), ConvertYamlToData(i.second));
        return jObject;
    }

    if (_node.IsSequence())
    {
        DataObject jArray(DataType::Array);
        for (size_t i = 0; i < _node.size(); i++)
            jArray.addArrayObject(ConvertYamlToData(_node[i]));
        return jArray;
    }

    std::cerr << "Error parsing YAML node. Element type not defined! " + yamlTypeAsString(_node.Type());
    return DataObject(DataType::Null);
}

}//namespace
