#pragma once
#include <dataObject/Exception.h>
#include <dataObject/SPointer.h>
#include <memory>
#include <set>
#include <vector>
#include <map>

namespace dataobject
{
enum DataType
{
    String,
    Integer,
    Bool,
    Array,
    Object,
    Null,
    NotInitialized
};

/// DataObject
/// A data sturcture to manage data from json, yml
class DataObject : public GCP_SPointerBase
{
public:
    typedef GCP_SPointer<DataObject> spDataObject;
    DataObject();
    DataObject(DataObject const&) = delete;
    DataObject(DataType _type);
    DataObject(DataType _type, bool _bool);
    DataObject(std::string const& _str);
    DataObject(std::string const& _key, std::string const& _str);
    DataObject(std::string const& _key, int _val);
    DataObject(int _int);

    DataType type() const;
    void setKey(std::string const& _key);
    std::string const& getKey() const;

    std::vector<spDataObject> const& getSubObjects() const;
    std::map<string, spDataObject> const& getSubObjectKeys() const;
    std::vector<spDataObject>& getSubObjectsUnsafe();

    void addArrayObject(spDataObject const& _obj);
    DataObject& addSubObject(spDataObject const& _obj);

    DataObject& addSubObject(std::string const& _key, spDataObject const& _obj);
    void setSubObjectKey(size_t _index, std::string const& _key);
    void setKeyPos(std::string const& _key, size_t _pos);

    bool count(std::string const& _key) const;

    std::string const& asString() const;
    int asInt() const;
    bool asBool() const;

    bool operator==(bool _value) const;
    bool operator==(DataObject const& _value) const;
    DataObject& operator[](std::string const& _key);
    DataObject& operator=(DataObject const& _value) = delete;
    DataObject& operator=(std::string const& _value);
    DataObject& operator=(int _value);

    void setString(string const& _value);
    void setInt(int _value);
    void setBool(bool _value);
    void replace(DataObject const& _value);
    void renameKey(std::string const& _currentKey, std::string const& _newKey);
    void removeKey(std::string const& _key);  // vector<element> erase method with `replace()` function

    DataObject const& atKey(std::string const& _key) const;
    DataObject& atKeyUnsafe(std::string const& _key);
    DataObject const& at(size_t _pos) const;
    DataObject& atUnsafe(size_t _pos);
    DataObject const& atLastElement() const;
    DataObject& atLastElementUnsafe();

    void setVerifier(void (*f)(DataObject&));
    void performModifier(void (*f)(DataObject&), std::set<string> const& _exceptionKeys = {});
    void performVerifier(void (*f)(DataObject const&)) const;

    void clear(DataType _type = DataType::NotInitialized);

    std::string asJsonNoFirstKey() const;
    std::string asJson(int level = 0, bool pretty = true, bool nokey = false) const;
    static std::string dataTypeAsString(DataType _type);

    void setOverwrite(bool _overwrite) { m_allowOverwrite = _overwrite; }
    void setAutosort(bool _sort) { m_autosort = _sort; m_allowOverwrite = true; }
    bool isOverwritable() const { return m_allowOverwrite; }
    bool isAutosort() const { return m_autosort; }
    void clearSubobjects(DataType _type = DataType::NotInitialized)
    {
        m_subObjects.clear();
        m_subObjectKeys.clear();
        m_type = _type;
    }

private:

    DataObject& _addSubObject(spDataObject const& _obj, string const& _keyOverwrite = string());
    void _assert(bool _flag, std::string const& _comment = "") const;

    std::vector<spDataObject> m_subObjects;
    std::map<string, spDataObject> m_subObjectKeys;
    DataType m_type;
    std::string m_strKey;
    bool m_allowOverwrite = false;  // allow overwrite elements
    bool m_autosort = false;

    std::string m_strVal;
    bool m_boolVal;
    int m_intVal;

    void (*m_verifier)(DataObject&) = 0;
};

typedef GCP_SPointer<DataObject> spDataObject;

// Find index that _key should take place in when being added to ordered _objects by key
size_t findOrderedKeyPosition(string const& _key, vector<spDataObject> const& _objects);
}
