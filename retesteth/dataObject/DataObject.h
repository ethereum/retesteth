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
    DataObject(DataObject const&) = default;
    DataObject(DataType _type);
    DataObject(DataType _type, bool _bool);
    DataObject(std::string const& _str);
    DataObject(std::string const& _key, std::string const& _str);
    DataObject(std::string const& _key, int _val);
    DataObject(int _int);
    DataType type() const;
    void setKey(std::string const& _key);
    std::string const& getKey() const;
    std::vector<DataObject> const& getSubObjects() const;
    std::vector<DataObject>& getSubObjectsUnsafe();
    DataObject& addSubObject(DataObject const& _obj);
    DataObject& addSubObject(std::string const& _key, DataObject const& _obj);
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
        DataObject newObj(DataType::Null);
        newObj.setKey(_key);
        return _addSubObject(newObj);  // !could change the item order!
    }

    DataObject& operator=(std::string const& _value)
    {
        setString(_value);
        return *this;
    }

    void setString(string const& _value)
    {
        _assert(m_type == DataType::String || m_type == DataType::Null,
            "In DataObject=(string) DataObject must be string or Null!");
        m_type = DataType::String;
        m_strVal = _value;
    }

    DataObject& operator=(int _value)
    {
        setInt(_value);
        return *this;
    }

    void setInt(int _value)
    {
        _assert(m_type == DataType::Integer || m_type == DataType::Null,
            "In DataObject=(int) DataObject must be int or Null!");
        m_type = DataType::Integer;
        m_intVal = _value;
    }

    void setBool(bool _value)
    {
        _assert(m_type == DataType::Bool || m_type == DataType::Null,
            "In DataObject:setBool(bool) DataObject must be bool or Null!");
        m_type = DataType::Bool;
        m_boolVal = _value;
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
            //_assert(getSubObjects().size() == 1,
            //    "in DataObject::==(Object)  LType Object must have only 1 object!");
            //_assert(_value.getSubObjects().size() == 1,
            //    "in DataObject::==(Object)  RType Object must have only 1 object!");
            equal = getSubObjects().size() == _value.getSubObjects().size();
            // equal = getSubObjects().at(0) == _value.getSubObjects().at(0);
            for (size_t i = 0; i < getSubObjects().size(); i++)
            {
                equal = getSubObjects().at(i) == _value.getSubObjects().at(i);
                if (!equal)
                    break;
            }
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
        if (!m_allowOverwrite && !m_autosort)
            _assert(m_type == DataType::Null,
                "m_type == DataType::Null (DataObject& operator=). Overwriting dataobject that is "
                "not NULL");

        if (m_type != DataType::Null)
            replace(_value);         // overwrite value and key
        else
        {
            // keep the key "newkey" for object["newkey"] = object2;  declarations when object["newkey"] is null;
            string const currentKey = m_strKey;
            replace(_value);
            m_strKey = currentKey;
        }
        return *this;
    }

    void replace(DataObject const& _value);

    DataObject const& atKey(std::string const& _key) const;
    DataObject& atKeyUnsafe(std::string const& _key);
    DataObject const& at(size_t _pos) const;
    DataObject& atUnsafe(size_t _pos);
    DataObject const& atLastElement() const;
    DataObject& atLastElementUnsafe();
    void setVerifier(void (*f)(DataObject&));
    void performModifier(void (*f)(DataObject&), std::set<string> const& _exceptionKeys = {});
    void performVerifier(void (*f)(DataObject const&)) const;

    void addArrayObject(DataObject const& _obj);

    void renameKey(std::string const& _currentKey, std::string const& _newKey);

    /// vector<element> erase method with `replace()` function
    void removeKey(std::string const& _key);

    void clear(DataType _type = DataType::Null);

    std::string asJsonNoFirstKey() const;
    std::string asJson(int level = 0, bool pretty = true, bool nokey = false) const;
    static std::string dataTypeAsString(DataType _type);

    void setOverwrite(bool _overwrite) { m_allowOverwrite = _overwrite; }
    void setAutosort(bool _sort)
    {
        m_autosort = _sort;
        m_allowOverwrite = true;
    }
    bool isOverwritable() const { return m_allowOverwrite; }
    bool isAutosort() const { return m_autosort; }
    void clearSubobjects()
    {
        m_subObjects.clear();
        m_type = DataType::Null;
    }

private:
    DataObject& _addSubObject(DataObject const& _obj, string const& _keyOverwrite = string());
    void _assert(bool _flag, std::string const& _comment = "") const;

    std::vector<DataObject> m_subObjects;
    DataType m_type;
    std::string m_strKey;
    std::string m_strVal;
    bool m_allowOverwrite = false;  // allow overwrite elements
    bool m_autosort = false;
    bool m_boolVal;
    int m_intVal;
    void (*m_verifier)(DataObject&) = 0;
};

// Find index that _key should take place in when being added to ordered _objects by key
size_t findOrderedKeyPosition(string const& _key, vector<DataObject> const& _objects);
}
