#ifdef JSONCPP
#include <libdataobj/ConvertJsoncpp.h>

namespace dataobject
{
std::string jsonTypeAsString(Json::ValueType _type)
{
    switch (_type)
    {
    case Json::ValueType::nullValue:
        return "null";
    case Json::ValueType::intValue:
        return "int";
    case Json::ValueType::uintValue:
        return "uint";
    case Json::ValueType::realValue:
        return "double";
    case Json::ValueType::stringValue:
        return "string";
    case Json::ValueType::booleanValue:
        return "bool";
    case Json::ValueType::arrayValue:
        return "array";
    case Json::ValueType::objectValue:
        return "object";
    default:
        break;
    }
    return "";
}

/// Convert Json::Value object to DataObject
DataObject ConvertJsoncppToData(Json::Value const& _input)
{
    if (_input.isNull())
        return DataObject(DataType::Null);

    if (_input.isBool())
        return DataObject(DataType::Bool, _input.asBool());

    if (_input.isString())
        return DataObject(_input.asString());

    if (_input.isInt())
        return DataObject(_input.asInt());

    if (_input.isArray())
    {
        DataObject root(DataType::Array);
        for (Json::ArrayIndex i = 0; i < _input.size(); i++)
            root.addArrayObject(ConvertJsoncppToData(_input.get(i, Json::Value())));
        return root;
    }

    if (_input.isObject())
    {
        DataObject root(DataType::Object);
        Json::Value::Members const& members = _input.getMemberNames();
        size_t index = 0;
        for (auto const& i : _input)
        {
            string const& key = members.at(index++); // hopefuly members are arranged same way as auto: loop
            root.addSubObject(key, ConvertJsoncppToData(i));
        }
        return root;
    }

    // Make it an exception!
    std::cerr << "Error parsing JSON node. Element type not defined! " +
                     jsonTypeAsString(_input.type());
    return DataObject(DataType::NotInitialized);
}
}
#endif
