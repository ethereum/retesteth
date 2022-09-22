#pragma once
#include "Exception.h"
#include "SPointer.h"
#include <memory>
#include <set>
#include <vector>
#include <map>
#include <variant>

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

class DataObjectK;
class GCP_SPointerDataObject;
typedef GCP_SPointerDataObject spDataObject;

struct VectorInfo
{
    // Use vector here to be able to quickly find insert position
    // of objects to be ordered by it's key with findOrderedKeyPosition
    std::vector<spDataObject> m_subObjects;
    std::map<string, spDataObject> m_subObjectKeys;
};
typedef std::variant<string, bool, int, VectorInfo> DataVariant;

/// DataObject
/// A data sturcture to manage data from json, yml
class DataObject : public GCP_SPointerBase
{
public:
    DataObject();
    DataObject(DataObject const&) = delete;
    DataObject(DataType _type);
    DataObject(DataType _type, bool _bool);

    // DataObject(str)
    DataObject(std::string&& _str);
    DataObject(std::string const& _str);

    // DataObject(key)
    DataObject(std::string&& _key, std::string&& _str);
    DataObject(std::string const& _key, std::string const& _str);
    DataObject(std::string&& _key, int _val);

    DataObject(int _int);
    DataType type() const;
    void setKey(std::string&& _key);
    void setKey(std::string const& _key);
    std::string const& getKey() const;
    std::string& getKeyUnsafe();

    std::vector<spDataObject> const& getSubObjects() const;
    std::map<string, spDataObject> const& getSubObjectKeys() const;
    std::vector<spDataObject>& getSubObjectsUnsafe();

    void addArrayObject(spDataObject const& _obj);
    DataObject& addSubObject(spDataObject const& _obj);

    DataObject& addSubObject(std::string&& _key, spDataObject const& _obj);
    DataObject& addSubObject(std::string const& _key, spDataObject const& _obj);
    void setSubObjectKey(size_t _index, std::string&& _key);
    void setKeyPos(std::string const& _key, size_t _pos);

    bool count(std::string const& _key) const;

    DataVariant const& asVariant() const { return m_value; }
    std::string const& asString() const;
    std::string& asStringUnsafe();

    int asInt() const;
    bool asBool() const;

    bool operator==(bool _value) const;
    bool operator==(DataObject const& _value) const;
    DataObject& operator[](std::string const& _key);
    DataObject& operator[](std::string&& _key);
    DataObject& operator=(DataObject const& _value) = delete;
    //DataObject& operator=(spDataObject const& _value);
    spDataObject copy() const;
    void copyFrom(DataObject const& _other);
    DataObject& operator=(std::string&& _value);
    DataObject& operator=(std::string const& _value);
    DataObject& operator=(int _value);

    void setString(string&& _value);
    void setInt(int _value);
    void setBool(bool _value);
    void replace(DataObject const& _value);
    void renameKey(std::string const& _currentKey, std::string&& _newKey);
    void removeKey(std::string const& _key);  // vector<element> erase method with `replace()` function

    DataObject const& atKey(std::string const& _key) const;
    DataObjectK atKeyPointer(std::string const& _key);
    spDataObject& atKeyPointerUnsafe(std::string const& _key);
    DataObject& atKeyUnsafe(std::string const& _key);
    DataObject const& at(size_t _pos) const;
    DataObject& atUnsafe(size_t _pos);
    DataObject const& atLastElement() const;
    DataObject& atLastElementUnsafe();

    void setVerifier(void (*f)(DataObject&));
    enum ModifierOption
    {
        RECURSIVE,
        NONRECURSIVE
    };
    void performModifier(
        void (*f)(DataObject&), ModifierOption _opt = ModifierOption::RECURSIVE, std::set<string> const& _exceptionKeys = {});
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
        if (m_type == DataType::Array || m_type == DataType::Object)
        {
            auto& vector = std::get<VectorInfo>(m_value);
            vector.m_subObjectKeys.clear();
            vector.m_subObjects.clear();
        }
        m_type = _type;
    }

private:

    DataObject& _addSubObject(spDataObject const& _obj, string&& _keyOverwrite = string());
    void _assert(bool _flag, std::string const& _comment = string()) const;
    DataVariant m_value;
    DataType m_type;
    std::string m_strKey;
    bool m_allowOverwrite = false;  // allow overwrite elements
    bool m_autosort = false;

    void (*m_verifier)(DataObject&) = 0;
};

// Default DataObject pointer allocator so not to type 'new DataObject()' each time
class GCP_SPointerDataObject : public GCP_SPointer<DataObject>
{
public:
    GCP_SPointerDataObject() : GCP_SPointer<DataObject>(new DataObject()){}
    GCP_SPointerDataObject(DataObject* _data) : GCP_SPointer<DataObject>(_data){}
};

// The key assigner, assign left pointer to DataObjectK's host m_data[key]
class DataObjectK
{
public:
    DataObjectK(string const& _key, DataObject& _container)
      : m_key(_key), m_data(_container) {}
    DataObjectK& operator=(spDataObject const& _value);
    DataObject& getContent() { return m_data; }
    operator spDataObject() { return m_data.atKeyPointerUnsafe(m_key); }

private:
    string m_key;
    DataObject& m_data;
};


// Can help to keep incapsulation
// DataObject move requester, require the memory pointer to be irreversably moved into it
class spDataObjectMove
{
public:
    spDataObjectMove() {}
    void assignPointer(spDataObject const& _obj) { m_obj = _obj; }
    spDataObject& getPointer() { return m_obj; }
    operator DataObject const&() const { return m_obj.getCContent(); }

private:
    spDataObject m_obj;
};

// Move memory from _obj to spDataObjectMove and flush _obj pointer
spDataObjectMove move(spDataObject& _obj);

// Find index that _key should take place in when being added to ordered _objects by key
// Heavy function, use only on export when need to construct json with sorted keys
size_t findOrderedKeyPosition(string const& _key, vector<spDataObject> const& _objects);
}
