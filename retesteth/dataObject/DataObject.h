#pragma once
#include <dataObject/Exception.h>
#include <libdevcore/CommonIO.h>
#include <memory>
#include <vector>

namespace dataobject
{
enum DataType
{
    String,
    Integer,
    Bool,
    Array,
    Object,
    Null
};

/// DataObject
/// An data sturcture to manage data from json, yml
class DataObject
{
public:
    DataObject();
    DataObject(DataType _type);
    DataObject(DataType _type, bool _bool);
    DataObject(std::string const& _str);
    DataObject(std::string const& _key, std::string const& _str);
    DataObject(int _int);
    DataType type() const;
    void setKey(std::string const& _key);
    std::string const& getKey() const;
    std::vector<DataObject> const& getSubObjects() const;
    std::vector<DataObject>& getSubObjectsUnsafe();
    void addSubObject(DataObject const& _obj);
    void addSubObject(std::string const& _key, DataObject const& _obj);
    void setSubObjectKey(size_t _index, std::string const& _key);

    bool count(std::string const& _key) const;
    std::string const& asString() const;
    int asInt() const;
    bool asBool() const;

    void setKeyPos(std::string const& _key, size_t _pos);
    DataObject& operator[](std::string const& _key)
    {
        _assert(m_type == DataType::Null || m_type == DataType::Object,
            "m_type == DataType::Null || m_type == DataType::Object (DataObject& operator[])");
        for (auto& i : m_subObjects)
            if (i.getKey() == _key)
                return i;
        DataObject obj(DataType::Null);
        obj.setKey(_key);
        _addSubObject(obj); // !could change the item order!
        // need to find our item again
        for (auto& i : m_subObjects)
            if (i.getKey() == _key)
                return i;
        _assert(false, "key not found after rearrangment! (operator[])");
        return m_subObjects.at(0);
    }

    DataObject& operator=(std::string const& _value)
    {
        _assert(m_type == DataType::String || m_type == DataType::Null,
            "In DataObject=(string) DataObject must be string or Null!");
        m_type = DataType::String;
        m_strVal = _value;
        return *this;
    }

    DataObject& operator=(int _value)
    {
        _assert(m_type == DataType::Integer || m_type == DataType::Null,
            "In DataObject=(int) DataObject must be int or Null!");
        m_type = DataType::Integer;
        m_intVal = _value;
        return *this;
    }

    bool operator==(bool _value) const
    {
        DataObject tmp(DataType::Bool, _value);
        return *this == tmp;
    }

    bool operator==(DataObject const& _value) const
    {
        if (type() != _value.type() || getSubObjects().size() != _value.getSubObjects().size())
            return false;
        bool equal = true;
        equal = m_type == _value.type();
        equal = getKey() == _value.getKey();
        switch (m_type)
        {
        case DataType::Bool:
            equal = asBool() == _value.asBool();
            break;
        case DataType::Integer:
            equal = asInt() == _value.asInt();
            break;
        case DataType::String:
            equal = asString() == _value.asString();
            break;
        case DataType::Array:
            equal = getSubObjects().size() == _value.getSubObjects().size();
            for (size_t i = 0; i < getSubObjects().size(); i++)
            {
                equal = getSubObjects().at(i) == _value.getSubObjects().at(i);
                if (!equal)
                    break;
            }
            break;
        case DataType::Object:
            _assert(getSubObjects().size() == 1,
                "in DataObject::==(Object)  Type Object must have only 1 object!");
            _assert(_value.getSubObjects().size() == 1,
                "in DataObject::==(Object)  Type Object must have only 1 object!");
            equal = getSubObjects().size() == _value.getSubObjects().size();
            equal = getSubObjects().at(0) == _value.getSubObjects().at(0);
            break;
        default:
            _assert(false, "in DataObject::== unknown object type!");
            equal = false;
            break;
        }
        return equal;
    }

    DataObject& operator=(DataObject const& _value)
    {
        // So not to overwrite the existing data
        // Do not replace the key. Assuming that key is set upon calling DataObject[key] =
        _assert(m_type == DataType::Null, "m_type == DataType::Null (DataObject& operator=). Overwriting dataobject that is not NULL");
        m_type = _value.type();
        switch (_value.type())
        {
        case DataType::Integer:
            m_intVal = _value.asInt();
            break;
        case DataType::String:
            m_strVal = _value.asString();
            break;
        case DataType::Bool:
            m_boolVal = _value.asBool();
            break;
        default:
            break;
        }
        m_subObjects = _value.getSubObjects();
        return *this;
    }

    void replace(DataObject const& _value);

    DataObject const& at(std::string const& _key) const;

    void addArrayObject(DataObject const& _obj);

    void renameKey(std::string const& _currentKey, std::string const& _newKey);

    /// vector<element> erase method with `replace()` function
    void removeKey(std::string const& _key);

    void clear(DataType _type = DataType::Null);

    std::string asJson(int level = 0, bool pretty = true) const;
    static std::string dataTypeAsString(DataType _type);

private:
    void _addSubObject(DataObject const& _obj, string const& _keyOverwrite = string());
    void _checkDoubleKeys() const;
    void _assert(bool _flag, std::string const& _comment = "") const;

    std::vector<DataObject> m_subObjects;
    DataType m_type;
    std::string m_strKey;
    std::string m_strVal;
    bool m_boolVal;
    int m_intVal;
};
}
